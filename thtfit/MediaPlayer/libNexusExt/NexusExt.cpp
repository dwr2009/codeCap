#include "NexusExtApi.h"
#include <nxclient.h>
#include <string.h>
#include <ErrPrintHelper.h>
#include "media_player.h"
#include "media_volume.h"
#include "NexusExt.h"
#include "display.h"
#include "SysBaseSrvApi.h"
#include <nexus_i2c.h>
#include "nexus_platform.h"
#include <SnmpCmdConv.h>
#include "CRtcInterface.h"
#include "CRtcIsl1208Device.h"
#include "CNexusI2cBus.h"
#include "nexus_cec.h"
#include <DateTime.h>
#include <ProcessRunningInfo.h>
#include <stdlib.h>
#include <time.h>
#include <SharedPtr.h>
#include "AutoScopeLock.h"
#include "DbgLogSwitchDef.h"
#include "bchp_memc_arb_0.h"
#include "bchp_memc_arb_1.h"
#include "SystemTimezone.h"

//extern "C"{
#include <strings.h>
//}

#define	BI2C_ERR_NO_ACK				0x00110001

CNexusExt * CNexusExtImpl_new()
{
	return (new CNexusExtImpl());
}

CNexusExtImpl::CNexusExtImpl()
    :m_bFirstOutputModeSet(true),m_iYPrPbValue(0)
{
	INT_t iRet;
	
	m_pMediaPlayer = NULL;
	m_eNexusDispAspectRatio = NEXUS_DisplayAspectRatio_eAuto;
	m_bFullVideo = false;
	ccSertype = 0;

	iMibmonitortype = 7;//default undefined
	iRet = CreateMediaPlayer();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

#ifdef ENABLE_DTV
	m_bTunerOpenSucc = FALSE;
#endif

}

CNexusExtImpl::~CNexusExtImpl()
{
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	
	if(pMediaPlayer)
	{
		media_player_stop(pMediaPlayer);
		media_player_destroy(pMediaPlayer);
		pMediaPlayer = NULL;
		m_pMediaPlayer = NULL;
	}

#ifdef ENABLE_DTV
	m_TunerSrv_sp.Clear();
	m_bTunerOpenSucc=FALSE;
#endif	

}

INT_t CNexusExtImpl::setEventListener(WeakPtr <CNexusExt::CEventListener> evtListener_wp)
{	
	INT_t iOutRet = ERR_SUCCESS;

	m_EvtListener_wp = evtListener_wp;

	return iOutRet;
}

INT_t CNexusExtImpl::NexusExt_NxClient_Join(LPCSTR pszClientName/* = NULL*/)
{
	INT_t iOutRet = ERR_SUCCESS;
	NxClient_JoinSettings oNxJoinSettings;
	NEXUS_Error nexusErr;

	NxClient_GetDefaultJoinSettings(&oNxJoinSettings);
	if(NULL != pszClientName)
	{
		snprintf(oNxJoinSettings.name, sizeof(oNxJoinSettings.name), "%s", pszClientName);
		oNxJoinSettings.name[sizeof(oNxJoinSettings.name)-1] = '\0';
	}
	oNxJoinSettings.tunnelCapable = FALSE;
	oNxJoinSettings.timeout = 0;
	oNxJoinSettings.session = 0;

	UINT64_t uSysUpTimeMs, uSysUpStartTimeMs;
	uSysUpStartTimeMs = GetSysUpTimeMs();
RETRY_NX_JOIN:
	nexusErr = NxClient_Join(&oNxJoinSettings);
	if(NEXUS_SUCCESS != nexusErr)
	{
		uSysUpTimeMs = GetSysUpTimeMs();
		if((5*1000) > (uSysUpTimeMs - uSysUpStartTimeMs))
		{
			if(TRUE == NxserverIsRunning())
			{
				//LOG_BLINE("nxserver is running, retry.\n");
				sleep(1);
				goto RETRY_NX_JOIN;
			}
		}
		LOG_BLINE("nexusErr=%d\n", nexusErr);
		iOutRet = ERR_NxClientJoin_FAILED;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::NexusExt_NxClient_Uninit()
{
	INT_t iOutRet = ERR_SUCCESS;

	NxClient_Uninit();
	
	return iOutRet;
}

INT_t CNexusExtImpl::Play(CONST LPCSTR pszUrl, CONST BOOL_t bLoopMode/* = FALSE*/)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	media_player_start_settings mediaPlayerStartSettings;
	media_player_t pMediaPlayer = NULL;

    
	iRet = CreateMediaPlayer();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	media_player_get_default_start_settings(&mediaPlayerStartSettings);
	mediaPlayerStartSettings.videoWindowType = NxClient_VideoWindowType_eMain;	/* full screen capable */
	mediaPlayerStartSettings.loopMode = NEXUS_PlaybackLoopMode_eLoop;
	mediaPlayerStartSettings.eof = pfnOnEndOfStreamCb;
	mediaPlayerStartSettings.pfnVidDecStreamChangedCb = vidDecStreamChangedCb;
	mediaPlayerStartSettings.context = this;
	mediaPlayerStartSettings.stream_url = pszUrl;
	mediaPlayerStartSettings.maxFormat=NEXUS_VideoFormat_e4096x2160p60hz;
	mediaPlayerStartSettings.smoothResolutionChange=true;
	media_player_stop_cc(pMediaPlayer);

	iRet = media_player_start(pMediaPlayer, &mediaPlayerStartSettings);
	if(0 != iRet)
	{
		iOutRet = ERR_MediaPlayer_StartFail;
		goto EXIT_PROC;
	}

	if(media_get_status(pMediaPlayer) == CCPREPARE)//ccstatus prepare
	{
		iRet = ColseCapture(pMediaPlayer,&mediaPlayerStartSettings,ccSertype);
		if(!iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::SwitchToPlay()
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;
	
	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	if(NEXUS_PlaybackState_ePlaying == oPlaybackStatus.state)	//already OK
	{
		goto EXIT_PROC;
	}

	iNexusErr = media_player_trick(m_pMediaPlayer, NEXUS_NORMAL_DECODE_RATE);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_Trick_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::SwitchToPause()
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	if(NEXUS_PlaybackState_ePaused == oPlaybackStatus.state)	//already OK
	{
		goto EXIT_PROC;
	}

	iNexusErr = media_player_trick(m_pMediaPlayer, 0/*pause*/);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_Trick_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

/*Closed Caption*/

INT_t CNexusExtImpl::setShowClosedCaption(int iCcValue)
{
	INT_t iOutRet = ERR_SUCCESS;
	media_player_t pMediaPlayer = NULL;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	ccSertype = iCcValue;
	media_player_stop_cc(pMediaPlayer);
	ColseCapture(pMediaPlayer,NULL,ccSertype);

EXIT_PROC:
	return iOutRet;
}


INT_t CNexusExtImpl::mibSwitchToFastForward(int iPlaySpeed)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iNexusErr = media_player_trick(m_pMediaPlayer, iPlaySpeed*NEXUS_NORMAL_DECODE_RATE);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_Trick_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::SwitchToFastForward()
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_trick(m_pMediaPlayer, 8*NEXUS_NORMAL_DECODE_RATE);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_Trick_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::SwitchToRewind()
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_trick(m_pMediaPlayer, -8*NEXUS_NORMAL_DECODE_RATE);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_Trick_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::getPlayMode(OUT MediaPlayer::PlayMode & ePlayMode)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	if(NEXUS_PlaybackState_ePaused == oPlaybackStatus.state)
	{
		ePlayMode = PlayMode_Pause;
	}
	else if(NEXUS_PlaybackState_eStopped == oPlaybackStatus.state)
	{
		ePlayMode = PlayMode_Stopped;
	}
	else if(NEXUS_PlaybackState_eTrickMode == oPlaybackStatus.state)
	{
		if(NEXUS_NORMAL_DECODE_RATE < oPlaybackStatus.trickModeSettings.rate)
		{
			ePlayMode = PlayMode_FastForward;
		}
		else if(0 > oPlaybackStatus.trickModeSettings.rate)
		{
			ePlayMode = PlayMode_Rewind;
		}
		else if(0 == oPlaybackStatus.trickModeSettings.rate)
		{
			ePlayMode = PlayMode_Pause;
		}
	}
	else
	{
		ePlayMode = PlayMode_Normal;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::getCurrentTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	uTimestamp = oPlaybackStatus.position;

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::getLastTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	uTimestamp = oPlaybackStatus.last;

EXIT_PROC:
	return iOutRet;
}


INT_t CNexusExtImpl::setSyncTime(const LONG & Offset)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}


	iNexusErr = media_player_get_playback_status(m_pMediaPlayer, &oPlaybackStatus);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_Player_GetStatus_Err;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_trick(m_pMediaPlayer, NEXUS_NORMAL_DECODE_RATE + Offset);

	//iNexusErr = media_player_seek(m_pMediaPlayer,-Offset,SEEK_CUR);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_setSyncTime_Err;
		goto EXIT_PROC;
	}


EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setSyncSeek(const LONG & Offset)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_seek(m_pMediaPlayer,-Offset,SEEK_CUR);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_setSyncTime_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setDisplayOutputMode(int displayMode)
{
	INT_t iOutRet=ERR_SUCCESS,iRet;
	NEXUS_ComponentOutputSettings pSettings;
    NEXUS_ComponentOutputSettings rSettions;
    int iCurType;
    NxClient_GetComponentOutputSettings(&pSettings);
    
    
	SysProp_setPersistent("DisplayOutputMode", displayMode);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
    if(m_bFirstOutputModeSet)
    {
        NEXUS_Platform_ReadRegister(0x006e6c3c,&m_iYPrPbValue);
        m_bFirstOutputModeSet=false;
    }
    if(NEXUS_ComponentOutputType_eRGB==pSettings.type)
    {
        LOG_BLINE("Old output is eRGB\n");
    }
    else
    {
        LOG_BLINE("Old output is eYPrPb\n");
    }
    LOG_BLINE("displayMode is %d iYPrPbValue is %08x\n",displayMode,m_iYPrPbValue);
	if(displayMode = 1)
	{

#if 0 //disabled by cloudy
		uint32_t value;  
        
		NEXUS_Platform_WriteRegister(BCHP_MEMC_ARB_0_CLIENT_INFO_120/*0x011011e4*/,0x00aa7022);  
		NEXUS_Platform_WriteRegister(BCHP_MEMC_ARB_1_CLIENT_INFO_135/*0x01181220*/,0x3ffff020/*0x0037c020*/); 
		NEXUS_Platform_WriteRegister(BCHP_MEMC_ARB_0_CLIENT_INFO_135,0x0037c020);

		NEXUS_Platform_ReadRegister(0x006e6c40,&value);/* Source Select Register for HDSRC_0 */
        LOG_BLINE("displayMode is %d value is %08x\n",displayMode,value);
		NEXUS_Platform_WriteRegister(0x006e6c3c,value);/* Source Select Register for SDSRC_0 */
		NEXUS_Platform_WriteRegister(0x006E202C,0x116036E5);
		pSettings.type = NEXUS_ComponentOutputType_eRGB;
		pSettings.dacs.RGB.noSync = true;
		pSettings.dacs.RGB.dacGreen = NEXUS_VideoDac_e0;
	    pSettings.dacs.RGB.dacRed = NEXUS_VideoDac_e1;
	    pSettings.dacs.RGB.dacBlue = NEXUS_VideoDac_e2;
		pSettings.dacs.RGB.dacHSync=NEXUS_VideoDac_eNone;
        iCurType=NEXUS_ComponentOutputType_eRGB;
#else

        //default is YPrPb
        //because set VGA ,case system to dead,so disable 
        return ERROR_UNKNOWN;
#endif
        
	}
	else
	{
		NEXUS_Platform_WriteRegister(0x006e6c3c,m_iYPrPbValue);
		pSettings.type = NEXUS_ComponentOutputType_eYPrPb;
		pSettings.dacs.YPrPb.dacY = NEXUS_VideoDac_e0;
		pSettings.dacs.YPrPb.dacPr = NEXUS_VideoDac_e1;
		pSettings.dacs.YPrPb.dacPb = NEXUS_VideoDac_e2;
        iCurType=NEXUS_ComponentOutputType_eYPrPb;
	}
	LOG_BLINE("pSettings.type is %08x\n",pSettings.type);
	iRet=NxClient_SetComponentOutputSettings(&pSettings);
	if(iRet)
	{
		iOutRet=iRet;
        LOG_BLINE("config failed\n");
		goto EXIT_PROC;
	}
    NxClient_GetComponentOutputSettings(&rSettions);
    if(rSettions.type!=iCurType)
    {
        LOG_BLINE("set config failed\n");
    }
	
EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::changeHdmiDisplaySettings(int iDispParam,int iValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT_t nexusErr = NEXUS_SUCCESS;
	media_player_t pMediaPlayer = NULL;
	pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	nexusErr = MediaPlayer_changeHdmiDisplaySettings(m_pMediaPlayer,iDispParam,iValue);
	if(NEXUS_SUCCESS != nexusErr)
	{
		iOutRet = nexusErr;
		goto EXIT_PROC;
	}
	if(ERROR_FILE_WRITE_FAIL == nexusErr)
	{
		iOutRet = nexusErr;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


INT_t CNexusExtImpl::getDisplayOutputMode(int & displayMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strDisplayOutputMode, 64);
	CStackBufString::SCANF_PARAM oScanfParam;
	CStackBufString::ScanfParam_Init(&oScanfParam);
	int iDisplayOutputMode=0;
		
	iRet = SysProp_get("DisplayOutputMode", OUT strDisplayOutputMode);
	if(ERROR_SUCCESS == iRet)
	{
		iRet = strDisplayOutputMode.Scanf(IN OUT oScanfParam, "%d", &iDisplayOutputMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else
	{
		iDisplayOutputMode = 0;
	}

	displayMode=iDisplayOutputMode;

EXIT_PROC:

	return iOutRet;

}

INT_t CNexusExtImpl::setvideoColorSpace(int colorspace)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	NEXUS_ColorSpace aaa;

	if(colorspace < NEXUS_ColorSpace_eAuto || colorspace >= NEXUS_ColorSpace_eMax)
	{
		iOutRet = ERROR_NOT_SUPPORTED;
		goto EXIT_PROC;
	}

	iRet = ChangeVidColorSpace((NEXUS_ColorSpace)colorspace);
	if(iRet != ERR_SUCCESS)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet=iRet;
		goto EXIT_PROC;
	}	

	iRet = SysProp_setPersistent("VideoColorSpace", colorspace);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

EXIT_PROC:

	return iOutRet;

}


INT_t CNexusExtImpl::getvideoColorSpace(int & colorspace)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strColorSpace, 64);
	CStackBufString::SCANF_PARAM oScanfParam;
	CStackBufString::ScanfParam_Init(&oScanfParam);
	int icolorspace=0;
		
	iRet = SysProp_get("VideoColorSpace", OUT strColorSpace);
	if(ERROR_SUCCESS == iRet)
	{
		iRet = strColorSpace.Scanf(IN OUT oScanfParam, "%d", &icolorspace);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	colorspace=icolorspace;

EXIT_PROC:
	return iOutRet;
}





INT_t CNexusExtImpl::Seek(const unsigned int & Offset)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr;
	NEXUS_PlaybackStatus oPlaybackStatus;

	if(NULL == m_pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = media_player_seek(m_pMediaPlayer,Offset,SEEK_CUR);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_Nexus_setSyncTime_Err;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}



INT_t CNexusExtImpl::StopPlayback()
{
	INT_t iOutRet = ERR_SUCCESS;
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	
	if(m_pMediaPlayer)
	{
		media_player_stop(pMediaPlayer);
	}

	media_player_stop_cc(pMediaPlayer);
	if(!m_TunerSrv_sp.isNull())
	{
		m_TunerSrv_sp->CloseTuner();
		m_bTunerOpenSucc=FALSE;
	}
	
	return iOutRet;
}

INT_t CNexusExtImpl::ChangeVolume(int VolumePercentToChange)
{
	
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAudioVolumeExt ChAudioVolume;
	CHANGE_VOLUME_DIRECTION chvoldir;

	do
	{
		iRet = getVolumeLevel(ChAudioVolume);
		if(iRet)
		{
			iOutRet = ERR_MediaPlayer_GetVolumeFail;
			break;
		}
		
		if(0 > VolumePercentToChange)
		{
			VolumePercentToChange = 0;
		}
		else if(100 < VolumePercentToChange)
		{
			VolumePercentToChange = 100;
		}

		if(0 > VolumePercentToChange)
		{
			VolumePercentToChange = 0;
		}
		else if(100 < VolumePercentToChange)
		{
			VolumePercentToChange = 100;
		}

		ChAudioVolume.m_left = getVolValFromPercent(VolumePercentToChange);
		ChAudioVolume.m_right = getVolValFromPercent(VolumePercentToChange);

		iRet = setVolumeLevel(ChAudioVolume);
		if(iRet)
		{
			iOutRet = ERR_MediaPlayer_SetVolumeFail;
			break;
		}
	}while(FALSE);

#if 0
		LOG_BLINE("VolumePercentToChange = %d\n",VolumePercentToChange);
		LOG_BLINE("left vol = %d\n",ChAudioVolume.m_left);
		LOG_BLINE("right vol = %d\n",ChAudioVolume.m_right);
#endif

	return iOutRet;
}

INT_t CNexusExtImpl::ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, 
	VIDEO_OUTPUT_MODE * peNewVoMode/* = NULL*/,VIDEO_OUTPUT_MODE * peOldVoMode/* = NULL*/)
{
    INT_t iOutRet = ERR_SUCCESS, iRet;
    NEXUS_VideoFormat nexusVidFmt;
    VIDEO_OUTPUT_MODE vidOutMode = VO_MODE_NotSet;

	//LOG_BLINE("eVoMode=%d\n", eVoMode);
	nexusVidFmt = getCurNexusDispVidFmt();	//NEXUS_Format
	//LOG_LINE("CurNexusDispVidFmt nexusVidFmt=%d\n",nexusVidFmt);
	//Nexus_Format => VIDEO_OUTPUT_MODE
	iRet = NexusVidFmt_to_VidOutMode(IN nexusVidFmt, OUT &vidOutMode);
    if(iRet != ERR_SUCCESS)
    {
		if(ERR_NOT_FOUND == iRet)
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		else	//other errs
		{
	    	iOutRet = iRet;
	    	goto EXIT_PROC;
		}
    }

	if(peOldVoMode)
	{
		*peOldVoMode = vidOutMode;
	}
	
	if(peNewVoMode)
	{
		*peNewVoMode = vidOutMode;
	}

    if(eVoMode == VO_MODE_NotSet)
    {
        LOG_BLINE(" VO_MODE_NotSet");
		goto EXIT_PROC;
    }
   
    iRet = VidOutMode_to_NexusVidFmt(IN eVoMode, OUT &nexusVidFmt);
    if(iRet != ERR_SUCCESS)
    {
    	iOutRet=iRet;
    	goto EXIT_PROC;
    }
    
	//LOG_BLINE("eVoMode=%d,nexusVidFmt=%d\n",eVoMode, nexusVidFmt);

#if 1 /*Add by xuweiwei 2014-8-6 for Save Video Output Mode Value*/
	if(nexusVidFmt <= NEXUS_VideoFormat_eUnknown || nexusVidFmt >= NEXUS_VideoFormat_eMax)
	{
        iOutRet = ERROR_NOT_SUPPORTED;
        goto EXIT_PROC;
    }
    

#if 0
	iRet = NxClient_GetDisplayStatus( &displayStatus );
	if(NEXUS_SUCCESS != iRet){
		iOutRet = ERR_Nexus_GetDispStatus_Err;
		goto EXIT_PROC;
	}
	
	hdmiStatus = displayStatus.hdmi.status;

	if(hdmiStatus.connected)
	{
		if(hdmiStatus.videoFormatSupported[nexusVidFmt] == false) /*not support*/
		{ 
			nexusVidFmt = hdmiStatus.preferredVideoFormat;
			iRet=NexusVidFmt_to_VidOutMode(IN nexusVidFmt, OUT &vidOutMode);
			if(iRet != ERR_SUCCESS)
			{
				PRINT_BFILE_LINENO_BUG_STR;
			}
			else
			{
				if(peNewVoMode)
				{
					*peNewVoMode = vidOutMode;
				}
			}
		}
	}
#endif
    iRet = ChangeVidOutputMode(nexusVidFmt);
    if(iRet != ERR_SUCCESS)
    {
    	PRINT_BFILE_LINENO_IRET_STR;
		iOutRet=iRet;
		goto EXIT_PROC;
    }	

	iRet = SysProp_setPersistent("VideoOutputMode", nexusVidFmt);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	if(peNewVoMode)
	{
		*peNewVoMode = eVoMode;
	}
#endif


EXIT_PROC:

	return iOutRet;
}

INT_t CNexusExtImpl::CreateMediaPlayer()
{
	INT_t iOutRet = ERR_SUCCESS;
	
	if(NULL == m_pMediaPlayer)
	{
		media_player_create_settings mediaPlayerCreateSettings;
		media_player_get_default_create_settings(&mediaPlayerCreateSettings);
		mediaPlayerCreateSettings.decodeVideo = TRUE;
		mediaPlayerCreateSettings.decodeAudio = TRUE;
		mediaPlayerCreateSettings.dtcpEnabled = FALSE;
		mediaPlayerCreateSettings.maxWidth = 4096;
		mediaPlayerCreateSettings.maxHeight = 2160;
		m_pMediaPlayer = media_player_create(&mediaPlayerCreateSettings);
		if(NULL == m_pMediaPlayer)
		{
			iOutRet = ERR_MediaPlayer_CreateFail;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setVideoPosition(int x, int y, int w, int h)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error iNexusErr = NEXUS_SUCCESS;
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;

    //LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,w,h);
	if(NULL == pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iNexusErr = MediaPlayer_setVidPos(pMediaPlayer, x, y, w, h);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_FAILED;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setOsdSize(int w, int h)
{
    INT_t iOutRet = ERR_SUCCESS;
    NEXUS_Error iNexusErr = NEXUS_SUCCESS;
    media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;

    //LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,w,h);
    if(NULL == pMediaPlayer)
    {
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    MediaPlayer_setOsdSize(pMediaPlayer,w,h);

EXIT_PROC:
    return iOutRet;
}


VOID CNexusExtImpl::pfnOnEndOfStreamCb(void * pContext)
{
	CNexusExtImpl * pNexusExt = (typeof(pNexusExt))pContext;
	if(NULL == pNexusExt)
	{
		LOG_BLINE("BUG,pContext is NULL.\n");
		goto EXIT_PROC;
	}
	
	pNexusExt->OnEndOfStream();

EXIT_PROC:
	;
}

VOID CNexusExtImpl::OnEndOfStream()
{
	SharedPtr <CEventListener> evtListener_sp = m_EvtListener_wp.Promote();
	if(evtListener_sp.isNull())
	{
		goto EXIT_PROC;
	}
	evtListener_sp->OnEndOfStream();
	
EXIT_PROC:
	;
}

VOID CNexusExtImpl::vidDecStreamChangedCb(void * pCtx)
{
	CNexusExtImpl * pNexusExt = (typeof(pNexusExt))pCtx;

	if(CC_UNLIKELY(NULL == pNexusExt))
	{
		LOG_BLINE("BUG,pContext is NULL.\n");
		goto EXIT_PROC;
	}
	pNexusExt->onVidDecStreamChanged();

EXIT_PROC:
	;
}

VOID CNexusExtImpl::onVidDecStreamChanged()
{
	SharedPtr <CEventListener> evtListener_sp = m_EvtListener_wp.Promote();
	if(evtListener_sp.isNull())
	{
		goto EXIT_PROC;
	}
	evtListener_sp->onVidDecStreamChanged();
EXIT_PROC:
	;
}

INT_t CNexusExtImpl::scanI2cDev()
{
	INT_t iOutRet = ERR_SUCCESS;
	int iId;
	NEXUS_Error nexusErr;
	NEXUS_I2cSettings nexusI2cSettings;

	NEXUS_I2c_GetDefaultSettings(&nexusI2cSettings);
	nexusI2cSettings.clockRate = NEXUS_I2cClockRate_e100Khz;
	nexusI2cSettings.interruptMode = true;
	nexusI2cSettings.burstMode = false;
	nexusI2cSettings.timeout = 0;	//use defalt
	nexusI2cSettings.softI2c = false;
	nexusI2cSettings.fourByteXferMode = false;
	nexusI2cSettings.use5v = false;
	for(iId=0; iId<NEXUS_NUM_I2C_CHANNELS; iId++)
	{
		NEXUS_I2cHandle nexusI2cHandle = NEXUS_I2c_Open(iId, &nexusI2cSettings);
		BYTE i2cSlaveAddr;
		if(NULL == nexusI2cHandle)
		{
			LOG_BLINE("OpenI2cCh %d failed\n", iId);
			goto ONE_I2C_CH_SCAN_FINISH;
		}
		for(i2cSlaveAddr = 0x00; i2cSlaveAddr <= 0x7F; i2cSlaveAddr++)
		{
			BYTE byteRead;
			nexusErr = NEXUS_I2c_Read(nexusI2cHandle, i2cSlaveAddr, 0, &byteRead, 1);
			if(NEXUS_SUCCESS == nexusErr)
			{
				LOG("Found i2c dev(unshift) on ch%d: 0x%02x\n", iId, i2cSlaveAddr);
			}
			else
			{
				if(BI2C_ERR_NO_ACK == nexusErr)
				{
				}
				else if(NEXUS_NOT_INITIALIZED == nexusErr)
				{
					LOG_BLINE("err 0x%08x(NEXUS_NOT_INITIALIZED),ch=%d\n", nexusErr, iId);
				}
				else
				{
					LOG_BLINE("I2cRead err 0x%08x,ch=%d\n", nexusErr, iId);
				}
			}
		}
ONE_I2C_CH_SCAN_FINISH:
		if(nexusI2cHandle)
		{
			NEXUS_I2c_Close(nexusI2cHandle);
			nexusI2cHandle = NULL;
		}
	}

	/*
	//power on the tuner on thtfit 7445c0 board
	iRet = setGpioOutputVal(58, 1);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	*/
#if	0	//nexus not implemented
	//soft i2c for tuner on moca i2c bus
	{
		NEXUS_GpioHandle hGpioClk = NULL;
		NEXUS_GpioHandle hGpioData = NULL;
		NEXUS_GpioSettings gpioSettings;
		NEXUS_I2cHandle nexusI2cHandle = NULL;
		BYTE i2cSlaveAddr;
	
		NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eSpecial, &gpioSettings);
		gpioSettings.mode = NEXUS_GpioMode_eOutputOpenDrain/*NEXUS_GpioMode_eOutputPushPull*/;
		gpioSettings.value = NEXUS_GpioValue_eHigh;
		gpioSettings.interruptMode = NEXUS_GpioInterrupt_eDisabled;
		gpioSettings.interrupt.callback = NULL;
		gpioSettings.interrupt.context = NULL;
		gpioSettings.interrupt.param = NULL;
		gpioSettings.maskEdgeInterrupts = FALSE;

		hGpioClk = NEXUS_Gpio_Open(NEXUS_GpioType_eSpecial, 4, &gpioSettings);
		if(NULL == hGpioClk)
		{
			iOutRet = ERR_OpenNexusGpio_FAIL;
			goto DETECT_MOCA_I2C_END;
		}
		hGpioData = NEXUS_Gpio_Open(NEXUS_GpioType_eSpecial, 5, &gpioSettings);
		if(NULL == hGpioClk)
		{
			iOutRet = ERR_OpenNexusGpio_FAIL;
			goto DETECT_MOCA_I2C_END;
		}
		//goto DETECT_MOCA_I2C_END;
		
		nexusI2cSettings.clockRate = NEXUS_I2cClockRate_e100Khz;
		nexusI2cSettings.interruptMode = true;
		nexusI2cSettings.burstMode = false;
		nexusI2cSettings.timeout = 0;	//use defalt
		nexusI2cSettings.softI2c = true;
		nexusI2cSettings.clockGpio = hGpioClk;
		nexusI2cSettings.dataGpio = hGpioData;
		nexusI2cSettings.fourByteXferMode = false;
		nexusI2cSettings.use5v = false;

		nexusI2cHandle = NEXUS_I2c_Open(0, &nexusI2cSettings);
		if(NULL == nexusI2cHandle)
		{
			LOG_BLINE("OpenMocaI2c failed\n");
			goto DETECT_MOCA_I2C_END;
		}
		for(i2cSlaveAddr = 0x00; i2cSlaveAddr <= 0x7F; i2cSlaveAddr++)
		{
			BYTE byteRead;
			nexusErr = NEXUS_I2c_Read(nexusI2cHandle, i2cSlaveAddr, 0, &byteRead, 1);
			if(NEXUS_SUCCESS == nexusErr)
			{
				LOG("Found i2c dev(unshift) on MocaI2c: 0x%02x\n", i2cSlaveAddr);
			}
			else
			{
				if(BI2C_ERR_NO_ACK == nexusErr)
				{
				}
				else
				{
					LOG_BLINE("I2cRead err 0x%08x,ch=%d\n", nexusErr, iId);
				}
			}
		}
DETECT_MOCA_I2C_END:
		if(nexusI2cHandle)
		{
			NEXUS_I2c_Close(nexusI2cHandle);
			nexusI2cHandle = NULL;
		}
		if(hGpioData)
		{
			NEXUS_Gpio_Close(hGpioData);
			hGpioData = NULL;
		}
		if(hGpioClk)
		{
			NEXUS_Gpio_Close(hGpioClk);
			hGpioClk = NULL;
		}
	}
#endif

	return iOutRet;
}

INT_t CNexusExtImpl::setGpioOutputVal(const int iGpioNum, const bool bVal)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_GpioHandle hGpio = NULL;
	NEXUS_GpioSettings gpioSettings;
	
	NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eStandard, &gpioSettings);
	gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
	gpioSettings.value = (true==bVal?NEXUS_GpioValue_eHigh:NEXUS_GpioValue_eLow);
	gpioSettings.interruptMode = NEXUS_GpioInterrupt_eDisabled;
	gpioSettings.interrupt.callback = NULL;
	gpioSettings.interrupt.context = NULL;
	gpioSettings.interrupt.param = 0;
	gpioSettings.maskEdgeInterrupts = FALSE;
	hGpio = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, iGpioNum, &gpioSettings);
	if(NULL == hGpio)
	{
		iOutRet = ERR_OpenNexusGpio_FAIL;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	if(hGpio)
	{
		NEXUS_Gpio_Close(hGpio);
		hGpio = NULL;
	}
	return iOutRet;
}

#if 1 /*Add by xuweiwei 2014-9-9*/
INT_t CNexusExtImpl::setStereoscopic3DFormat(int* pStereoscopic3D/* = NULL*/)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	NxClient_DisplaySettings displaySettings;
	NxClient_GetDisplaySettings(&displaySettings);

	do
	{
		if(pStereoscopic3D == NULL)
		{
			displaySettings.display3DSettings.orientation = NEXUS_VideoOrientation_e2D;
			break;
		}
		
		NEXUS_VideoOrientation inStereoscopic3DValue=(NEXUS_VideoOrientation)(*pStereoscopic3D);
		
		if(NEXUS_VideoOrientation_eMax<=inStereoscopic3DValue || NEXUS_VideoOrientation_e2D > inStereoscopic3DValue )
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			goto EXIT_PROC;
		}

		if(NEXUS_VideoOrientation_e3D_LeftRight == inStereoscopic3DValue)
		{
			displaySettings.display3DSettings.orientation = NEXUS_VideoOrientation_e3D_LeftRight;
		}
		else if(NEXUS_VideoOrientation_e3D_OverUnder == inStereoscopic3DValue)
		{
			displaySettings.display3DSettings.orientation = NEXUS_VideoOrientation_e3D_OverUnder;
		}
		else
		{
			displaySettings.display3DSettings.orientation = NEXUS_VideoOrientation_e2D;
		}
	}while(false);

	iRet=NxClient_SetDisplaySettings(&displaySettings);
	if(0 != iRet)
	{
		iOutRet = ERR_FAILED;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

#endif

INT_t CNexusExtImpl::setVidWinAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio)
{
	INT_t iOutRet = ERR_SUCCESS;	
	NEXUS_Error nexusErr;

	if(DispAspectRatio_DEFAULT == eDispAspectRatio)
	{
		m_bFullVideo = false;
		m_eNexusDispAspectRatio = NEXUS_DisplayAspectRatio_eAuto;
	}
	else if(DispAspectRatio_4_3 == eDispAspectRatio)
	{
		m_bFullVideo = false;
		m_eNexusDispAspectRatio = NEXUS_DisplayAspectRatio_e4x3;
	}
	else if(DispAspectRatio_16_9 == eDispAspectRatio)
	{
		m_bFullVideo = false;
		m_eNexusDispAspectRatio = NEXUS_DisplayAspectRatio_e16x9;
	}
	else if(DispAspectRatio_FULL == eDispAspectRatio)
	{
		m_bFullVideo = true;
	}
	else
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	
	{
		nexusErr = MediaPlayer_setVidWinAspectRatio(m_pMediaPlayer, m_eNexusDispAspectRatio, m_bFullVideo);
		if(NEXUS_SUCCESS != nexusErr)
		{
			iOutRet = ERR_Nexus_setVidWinAspectRatio_Err;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::MibsetMonitorInfo(int MonitorType)
{
	INT_t iOutRet = ERROR_SUCCESS;
	iMibmonitortype = MonitorType;
	return iOutRet;
}

INT_t CNexusExtImpl::MibgetMonitorInfo(OUT int & MonitorType)
{
	INT_t iOutRet = ERROR_SUCCESS;
	MonitorType = iMibmonitortype;
	return iOutRet;
}

#if 1 /*Add by xuweiwei 2014-9-16*/
INT_t CNexusExtImpl::getMonitorInfo(OUT CString &strMonitorType)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strTmpMonitorType, 128);
	NEXUS_HdmiOutputBasicEdidData MonitorInfo;	
	
	do
	{
		iRet=NxClient_GetMonitorInfo(&MonitorInfo);
		if(iRet!=ERROR_SUCCESS)
		{
			iOutRet=iRet;
			break;
		}
		
		MonitorInfo.monitorName[sizeof(MonitorInfo.monitorName)-1] = '\0';
		iRet = strTmpMonitorType.Format("%02x%02x,%02x%02x,%s",MonitorInfo.vendorID[0],MonitorInfo.vendorID[1],MonitorInfo.productID[0],MonitorInfo.productID[1],MonitorInfo.monitorName);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(0 == strncasecmp("sony",MonitorInfo.monitorName,4))
			iMibmonitortype = 1;	
		else if(0 == strncasecmp("lg",MonitorInfo.monitorName,2))
			iMibmonitortype = 2;
		else if(0 == strncasecmp("nec",MonitorInfo.monitorName,3))
			iMibmonitortype = 3;
		else if(0 == strncasecmp("panasonic",MonitorInfo.monitorName,9))
			iMibmonitortype = 4;
		else if(0 == strncasecmp("pioneer",MonitorInfo.monitorName,7))
			iMibmonitortype = 5;
		else if(0 == strncasecmp("samsung",MonitorInfo.monitorName,7))
			iMibmonitortype = 6;
		else 
			iMibmonitortype = 7;
		strMonitorType = strTmpMonitorType;
	}while(FALSE);
	
	return iOutRet;
}
#endif


#if 1 /*Add by xuweiwei 2014-9-25*/
INT_t CNexusExtImpl::getDisplayParam(const int iDispParam,OUT int &oValue)
{

	INT_t iOutRet = ERROR_SUCCESS;
	NxClient_PictureQualitySettings pqSettings;
	NEXUS_HdmiOutputSettings hdmiOutputSettings;
	NxClient_GetPictureQualitySettings(&pqSettings);	
	INT_t iScalerValue = 0;
	switch(iDispParam)
	{
	case DispPARAM_Brightness:
		iScalerValue=pqSettings.graphicsColor.brightness;
		break;
	case DispPARAM_Contrast:
		iScalerValue=pqSettings.graphicsColor.contrast;
		break;
	case DispPARAM_Saturation:
		iScalerValue=pqSettings.graphicsColor.saturation;
		break;
	case DispPARAM_Hue:
		iScalerValue=pqSettings.graphicsColor.hue;
		break;
	case DispPARAM_ColorSpace:
		NxClient_GetHdmiDisplaySettings(&hdmiOutputSettings);
		iScalerValue=hdmiOutputSettings.colorSpace;
		oValue=iScalerValue;
		goto PROC_EXIT;
	case DispPARAM_ColorDepth:
		{
			NxClient_GetHdmiDisplaySettings(&hdmiOutputSettings);	
			if(hdmiOutputSettings.colorDepth == 0)
			{
				iScalerValue=ColorDepth_Auto;
			}
			else if(hdmiOutputSettings.colorDepth == 8)
			{
				iScalerValue=ColorDepth_8bit;
			}
			else if(hdmiOutputSettings.colorDepth == 10)
			{
				iScalerValue=ColorDepth_10bit;
			}		
			else if(hdmiOutputSettings.colorDepth == 12)
			{
				iScalerValue=ColorDepth_12bit;
			}
			oValue=iScalerValue;
			goto PROC_EXIT;
		}
	case DispPARAM_matrixcoefficients:
		{
			NxClient_GetHdmiDisplaySettings(&hdmiOutputSettings);
			if(hdmiOutputSettings.overrideMatrixCoefficients == false)
			{
				iScalerValue=Matrixcoefficients_Auto;
			}
			else if(hdmiOutputSettings.matrixCoefficients == NEXUS_MatrixCoefficients_eItu_R_BT_709)//bt709
			{
				iScalerValue=Matrixcoefficients_Bt709;
			}
			else if(hdmiOutputSettings.matrixCoefficients == NEXUS_MatrixCoefficients_eSmpte_170M)//Smpte_170M
			{
				iScalerValue=Matrixcoefficients_Smpte_170M;
			}
			else if(hdmiOutputSettings.matrixCoefficients == NEXUS_MatrixCoefficients_eItu_R_BT_470_2_BG)//BT_470_2_BG
			{
				iScalerValue=Matrixcoefficients_BT_470_2_BG;
			}
			else if(hdmiOutputSettings.matrixCoefficients == NEXUS_MatrixCoefficients_eItu_R_BT_2020_NCL)//2020ncl
			{
				iScalerValue=Matrixcoefficients_2020ncl;
			}
			else if(hdmiOutputSettings.matrixCoefficients == NEXUS_MatrixCoefficients_eItu_R_BT_2020_CL)//2020cl
			{
				iScalerValue=Matrixcoefficients_2020cl;
			}
			oValue=iScalerValue;
			goto PROC_EXIT;
		}
	default:
		iOutRet = ERROR_INVALID_PARAMETER;
		break;
	}

    oValue = (float)100/65535*(iScalerValue+32768)+0.5;

PROC_EXIT:
 	return iOutRet;
}

INT_t CNexusExtImpl::setDisplayParam(int iDispParam,int iValue)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	
	NxClient_PictureQualitySettings pqSettings;
	INT_t iScalerValue;
	if(0 > iValue)
	{
		iValue = 0;
	}
	else if(100 < iValue)
	{
		iValue = 100;
	}	
	NxClient_GetPictureQualitySettings(&pqSettings);
	iScalerValue = (float)65535/100*iValue-32768;
	switch(iDispParam)
	{
	case DispPARAM_Brightness:
		pqSettings.graphicsColor.brightness = iScalerValue;
		break;
	case DispPARAM_Contrast:
		pqSettings.graphicsColor.contrast = iScalerValue;
		break;
	case DispPARAM_Saturation:
		pqSettings.graphicsColor.saturation = iScalerValue;
		break;
	case DispPARAM_Hue:
		pqSettings.graphicsColor.hue = iScalerValue;
		break;
	case DispPARAM_ColorSpace:
	case DispPARAM_ColorDepth:
	case DispPARAM_matrixcoefficients:
		{
			iRet=changeHdmiDisplaySettings(iDispParam,iValue);
			if(iRet) {iOutRet=iRet;}
			goto EXIT_PROC;
		}
	default:
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	iRet = NxClient_SetPictureQualitySettings(&pqSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setOutputSpdifMode(int iSpdifMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	NxClient_AudioSettings audioSettings;
	NxClient_GetAudioSettings(&audioSettings);


	if(Mp7xxCommon::SnmpOutSpdifMode_Disabled == iSpdifMode)
	{
		audioSettings.spdif.outputMode=NxClient_AudioOutputMode_eNone;
	}
	else if(Mp7xxCommon::SnmpOutSpdifMode_Compressed == iSpdifMode) 
	{
		audioSettings.spdif.outputMode=NxClient_AudioOutputMode_ePassthrough;
	}
	else if(Mp7xxCommon::SnmpOutSpdifMode_Uncompressed  == iSpdifMode)
	{
		audioSettings.spdif.outputMode=NxClient_AudioOutputMode_ePcm;
	}
	else
	{
		audioSettings.spdif.outputMode=NxClient_AudioOutputMode_ePcm;
	}

	iRet = NxClient_SetAudioSettings(&audioSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:	
	return iOutRet;
}

#endif


#if 1 /*Add by xuweiwei 2014-9-30*/
INT_t CNexusExtImpl::setHdmiAudioOutput(int bHdmiAudioOutput)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	NxClient_AudioSettings audioSettings;
	NxClient_GetAudioSettings(&audioSettings);
	bool iHdmiAudioOutput=(typeof(iHdmiAudioOutput))(bHdmiAudioOutput);

	if(iHdmiAudioOutput)
	{
		audioSettings.hdmi.outputMode=NxClient_AudioOutputMode_eAuto;	
	}
	else
	{
		audioSettings.hdmi.outputMode=NxClient_AudioOutputMode_eNone;
	}

	iRet = NxClient_SetAudioSettings(&audioSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:	
	return iOutRet;

}


INT_t CNexusExtImpl::setAnalogAudioMute(int bAnalogAudioMute)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	NxClient_AudioSettings audioSettings;
	NxClient_GetAudioSettings(&audioSettings);
	bool iAnalogAudioMute=(typeof(iAnalogAudioMute))(bAnalogAudioMute);
	/*LOG_BLINE("bAnalogAudioMute:%d\n",bAnalogAudioMute);*/
	audioSettings.dac.muted = bAnalogAudioMute;
	iRet = NxClient_SetAudioSettings(&audioSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:	
	return iOutRet;
}

INT_t CNexusExtImpl::setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec, int izone)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CNexusI2cBus *pNexusI2cBus=NULL;
	CRtcIsl1208Device *pRtcIsl1208Device=NULL;
	int iI2cBus=DEFAULT_RTC_I2C_BUS_ID;
	struct tm tmUtcTimeToSet,tmRtcTimeToSet,*ptmRet;
	time_t timeToSetSystem;

	static int CurrentZone = 0; 
	
	#if 0

	static int iZoneOffset = 0; 
 	char zone_buf[10] = {0};

	LOG_LINE("iyear:%d,imonth:%d,imday:%d,ihour:%d,imin:%d,isec:%d izone:%d\n",iyear,imonth,imday,ihour,imin,isec,izone);

	 /*setting time zone*/

	iZoneOffset = izone -iZoneOffset;
	iZoneOffset > 0 ? snprintf(zone_buf,sizeof(zone_buf),"GMT+%d", izone) 
		: snprintf(zone_buf,sizeof(zone_buf),"GMT%d", izone);

	LOG_LINE("zone_buf is %s\n",zone_buf);
	setenv("TZ", zone_buf, 1);
	tzset();

	system("date -u");
	#endif

	//system("date .....");
	memset(&tmUtcTimeToSet,0, sizeof(tmUtcTimeToSet));
	//modify by bert 2016/08/23	
	//tmUtcTimeToSet.tm_year = iyear-1970;
	tmUtcTimeToSet.tm_year = iyear-1900;

	tmUtcTimeToSet.tm_mon = imonth - 1;
	tmUtcTimeToSet.tm_mday = imday;
	tmUtcTimeToSet.tm_hour = ihour;
	tmUtcTimeToSet.tm_min = imin;
	tmUtcTimeToSet.tm_sec = isec;

	iRet = CSystemTimezone::setSystemGlobalTimeZone(izone);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	tzset();

	/*setting syetem time*/
	timeToSetSystem = mktime(&tmUtcTimeToSet);
	
	iRet = stime(&timeToSetSystem);
	if(0 != iRet)
	{
		LOG_BLINE("setting system time failed.\n");
		iOutRet=iRet;
		return iOutRet;
	}

	memset(&tmRtcTimeToSet,0, sizeof(tmRtcTimeToSet));
	ptmRet = gmtime_r(&timeToSetSystem, &tmRtcTimeToSet);

	if(NULL == ptmRet)
	{
		iOutRet = ERROR_CALL_SYSTEM_FAILED;
		goto EXIT_PROC;
	}
	 
	pNexusI2cBus=new CNexusI2cBus(iI2cBus);
	if(NULL == pNexusI2cBus)
	{
		iOutRet=ERROR_OUT_OF_MEMORY;
		return iOutRet;
	}

	pRtcIsl1208Device=new CRtcIsl1208Device(pNexusI2cBus);
    if(NULL == pNexusI2cBus)
	{
		iOutRet=ERROR_OUT_OF_MEMORY;
		delete pNexusI2cBus;
		return iOutRet;
	}
	//system("date -u");
    /*seting i2c device time*/
	//modify by bert 2016/08/23
	//pRtcIsl1208Device->setYear(tmRtcTimeToSet.tm_year);
	pRtcIsl1208Device->setYear(tmRtcTimeToSet.tm_year+1900);

	pRtcIsl1208Device->setMonth(tmRtcTimeToSet.tm_mon + 1);
	pRtcIsl1208Device->setDay(tmRtcTimeToSet.tm_mday);
	pRtcIsl1208Device->setHour(tmRtcTimeToSet.tm_hour);
	pRtcIsl1208Device->setMinute(tmRtcTimeToSet.tm_min);
	pRtcIsl1208Device->setSecond(tmRtcTimeToSet.tm_sec);

	iRet=pRtcIsl1208Device->setTimerToRtcDevice();
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	delete pRtcIsl1208Device;
	delete pNexusI2cBus;

	return iOutRet;
}

INT_t CNexusExtImpl::getHwRtcTime(OUT CString & ConfigDateTime)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CNexusI2cBus *pNexusI2cBus=NULL;
	CRtcIsl1208Device *pRtcIsl1208Device=NULL;
	int iI2cBus=DEFAULT_RTC_I2C_BUS_ID;
	DECLARE_CLS_STACK_BUF_STRING(strTmpConfigDateTime, 128);

	pNexusI2cBus=new CNexusI2cBus(iI2cBus);
	if(NULL == pNexusI2cBus)
	{
		iOutRet=ERROR_OUT_OF_MEMORY;
		return iOutRet;
	}

	pRtcIsl1208Device=new CRtcIsl1208Device(pNexusI2cBus);
	if(NULL == pNexusI2cBus)
	{
		iOutRet=ERROR_OUT_OF_MEMORY;
		delete pNexusI2cBus;
		return iOutRet;
	}

	iRet=pRtcIsl1208Device->getTimerFromRtcDevice();
	if(!iRet)
	{
		iRet = strTmpConfigDateTime.Format("%d/%d/%d %02d:%02d:%02d\n",pRtcIsl1208Device->getYear(),pRtcIsl1208Device->getMonth(),\
			pRtcIsl1208Device->getDay(),pRtcIsl1208Device->getHour(),pRtcIsl1208Device->getMinute(),pRtcIsl1208Device->getSecond());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		ConfigDateTime = strTmpConfigDateTime;
	}
	else
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

EXIT_PROC:
	 delete pRtcIsl1208Device;
	 delete pNexusI2cBus;

	return iOutRet;

}

INT_t CNexusExtImpl:: getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	int iNexusErr = NEXUS_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
	MediaPlayer_getOsdPos(pMediaPlayer,&x,&y,&width,&height);
	//LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,width,height);

EXIT_PROC:
	return iNexusErr;
}

INT_t CNexusExtImpl:: getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	INT_t iOutRet = ERROR_SUCCESS;
	NEXUS_Error iNexusErr = NEXUS_SUCCESS;
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;

	if(NULL == pMediaPlayer)
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iNexusErr = MediaPlayer_getVidPos(pMediaPlayer,&x,&y,&width,&height);
    //LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,width,height);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iOutRet = ERR_FAILED;
		goto EXIT_PROC;
	}
	
EXIT_PROC:	
	return iOutRet;
}

INT_t CNexusExtImpl:: setOsdPosition(int x,int y,int width,int height,int type)
{
	INT_t iOutRet = ERROR_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;
	media_player_t pMediaPlayer = (typeof(pMediaPlayer))m_pMediaPlayer;
    LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,width,height);
	iOutRet=MediaPlayer_setOsdPos(pMediaPlayer,x,y,width,height);
	if(NEXUS_SUCCESS != iOutRet)
	{
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t CNexusExtImpl::setHdmiCecCtrl(int HdmiCecCmd)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	NEXUS_CecMessage transmitMessage;
	NEXUS_CecSettings cecSettings;
/*	NEXUS_CecMessage* transmitMessage;
	transmitMessage=(NEXUS_CecMessage*)malloc(sizeof(*transmitMessage));
	memset(transmitMessage,0,sizeof(*transmitMessage));
*/
	iRet=NxClient_GetCecSettings(&cecSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

	cecSettings.enabled = true;
	cecSettings.deviceType=NEXUS_CecDeviceType_eTv;
	cecSettings.cecController=NEXUS_CecController_eTx;
	cecSettings.disableLogicalAddressPolling =false ;

	iRet=NxClient_SetCecSettings(&cecSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}


	transmitMessage.destinationAddr = 0;
	transmitMessage.length = 5;
	
	switch(HdmiCecCmd)
	{
	case 1:
		transmitMessage.buffer[0] = NEXUS_CEC_OpImageViewOn;
		break;
	case 2:
		transmitMessage.buffer[0] = NEXUS_CEC_OpStandby;
		break;
	case 3:
		transmitMessage.buffer[0] = NEXUS_CEC_OpUserControlPressed;
		break;
	case 4:
		transmitMessage.buffer[0] = NEXUS_CEC_OpUserControlPressed;
		break;
	case 5:
		transmitMessage.buffer[0] = NEXUS_CEC_OpUserControlPressed;
		break;
	default:
		iOutRet = ERROR_INVALID_PARAMETER;
		break;
	}

	iRet=NxClient_Cec_TransmitMessage(&transmitMessage);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
	
}

#endif


#ifdef ENABLE_DTV
INT_t CNexusExtImpl::GetEntryChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CEventListener> evtListener_sp;
	{
		CAutoScopeLock AutoRangeLocker(&m_TunerScanMutex);
		evtListener_sp = m_EvtListener_wp.Promote();
	}

	if(evtListener_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet=ERR_TUNER_OPEN_FAIL;
		PRINT_BFILE_LINENO_IRET_STR;
		evtListener_sp->NotifyTuPlayerPropmtMsg(ERR_NO_DEV);
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnGetChannelInfo(iEntryId,pChannelNum,pNumofSubChannel,pStrPids);
	if(ERROR_SUCCESS != iRet )
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	
	return iOutRet;
	
}

INT_t CNexusExtImpl::PlayTuner(CONST LPCSTR pszUrl)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT32_t iChannelMode  = 0;
	bool bChFound = FALSE;
	UINT32_t Lock = 0;
	SharedPtr <CEventListener> evtListener_sp;

	{
		CAutoScopeLock AutoRangeLocker(&m_TunerScanMutex);
		evtListener_sp = m_EvtListener_wp.Promote();
	}

	if(evtListener_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet=ERR_TUNER_OPEN_FAIL;
		evtListener_sp->NotifyTuPlayerPropmtMsg(ERR_NO_DEV);
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnGetTunerChannelMode(&iChannelMode);
	if(ERROR_SUCCESS != iRet )
	{
		if(ERROR_NOT_FOUND == iRet)
		{
			iChannelMode = CHANNEL_MODE_DEFAULT;
		}
		else
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
	if(CHANNEL_MODE_DEFAULT == iChannelMode 
			|| CHANNEL_MODE_FIXED_SCANNED == iChannelMode)
	{
		iRet = m_TunerSrv_sp->OnSetUrl((LPCSTR)pszUrl, &bChFound);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = ERROR_NOT_FOUND;
			evtListener_sp->NotifyTuPlayerPropmtMsg(ERROR_NOT_FOUND);			
			goto EXIT_PROC;
		}
	}
	else
	{
		iRet = m_TunerSrv_sp->FindChannelIndexInFixedTable((LPCSTR)pszUrl, &bChFound);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = ERROR_NOT_FOUND;
			evtListener_sp->NotifyTuPlayerPropmtMsg(ERROR_NOT_FOUND);
			goto EXIT_PROC;
		}
	}

	{
		media_player_start_settings mediaPlayerStartSettings;
		media_player_get_default_start_settings(&mediaPlayerStartSettings);
		mediaPlayerStartSettings.videoWindowType = NxClient_VideoWindowType_eMain;
		mediaPlayerStartSettings.eof = pfnOnEndOfStreamCb;
		mediaPlayerStartSettings.pfnVidDecStreamChangedCb = vidDecStreamChangedCb;
		mediaPlayerStartSettings.context = this;
		m_TunerSrv_sp->SetMediaStartSetting(&mediaPlayerStartSettings);
	}

	iRet = m_TunerSrv_sp->OnSetChannelInfo();
	if(ERROR_SUCCESS != iRet)
	{	
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnWaitLock(&Lock);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	if(Sw_LogTuner)
	{
		LOG("Lock=%d\n",Lock);
	}
	
	iRet = PsfPlayback();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;	
	
}

INT_t CNexusExtImpl::TunerScan()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CEventListener> evtListener_sp;

	{
		CAutoScopeLock AutoRangeLocker(&m_TunerScanMutex);
		evtListener_sp = m_EvtListener_wp.Promote();
	}

	if(evtListener_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet =ERR_TUNER_OPEN_FAIL;
		PRINT_BFILE_LINENO_IRET_STR;
		evtListener_sp->NotifyTuPlayerPropmtMsg(ERR_NO_DEV);
		goto EXIT_PROC;
	}

	TunerScanProgressCk TunerScanPar;
	TunerScanPar.context=this;
	TunerScanPar.TuScanProgressCallback=CallBackScanProgress;
	
	iRet = m_TunerSrv_sp->OnSetScanProgressCb(&TunerScanPar);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}

	evtListener_sp->Tuner_StartScan();
	
	iRet=m_TunerSrv_sp->OnScanProgram();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
	}
	
	evtListener_sp->Tuner_StopScan();

EXIT_PROC:
	
	return iOutRet;
}

INT_t CNexusExtImpl::GetTunerChannelMode(int* iTuChMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet=ERR_TUNER_OPEN_FAIL;
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnGetTunerChannelMode(iTuChMode);
	if(ERROR_SUCCESS == iRet)
	{
	}
	else if(ERROR_NOT_FOUND == iRet)
	{
	}
	else
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t CNexusExtImpl::SetTunerChannelMode(int iTuChMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet=ERR_TUNER_OPEN_FAIL;
		goto EXIT_PROC;
	}
	
	iRet = m_TunerSrv_sp->OnSetTunerChannelMode(&iTuChMode);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t CNexusExtImpl::SetTunerStandardMode(int iTuStMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet=ERR_TUNER_OPEN_FAIL;
		goto EXIT_PROC;
	}
	
	iRet = m_TunerSrv_sp->OnSetTunerMode(&iTuStMode);
	if(ERROR_SUCCESS == iRet)
	{
	}
	else if(ERROR_NOT_FOUND == iRet)
	{
	}
	else
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t CNexusExtImpl::GetFirstChNum(unsigned int* OMajor,unsigned int* OMinor)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CTunerSrv> pTunerSrv_sp;
	UINT32_t iMajor=0;
	UINT32_t iMinord=0;

	if(m_TunerSrv_sp.isNull())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnGetChannelNumber(&iMajor,&iMinord);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	*OMajor=iMajor;
	*OMinor=iMinord;
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t CNexusExtImpl::ResetDuration()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t iCurrPlayStatus= PLAY_STAT_IDLE;
	SharedPtr <CEventListener> evtListener_sp;

	{
		CAutoScopeLock AutoRangeLocker(&m_TunerScanMutex);
		evtListener_sp = m_EvtListener_wp.Promote();
	}

	if(evtListener_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	
	iRet=OpenTunerPlayer();
	if( ERR_SUCCESS != iRet)
	{
		iOutRet =ERR_TUNER_OPEN_FAIL;
		PRINT_BFILE_LINENO_IRET_STR;
		evtListener_sp->NotifyTuPlayerPropmtMsg(ERR_NO_DEV);
		goto EXIT_PROC;
	}
	
	iOutRet = getMpSrvPlayStatus(&iCurrPlayStatus);
	if(ERROR_SUCCESS != iOutRet) 
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR
	}
	
	if(PLAY_STAT_PLAYING == iCurrPlayStatus
		|| PLAY_STAT_TUNER_SCANNING == iCurrPlayStatus)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnReInitTunerModules();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:		
		return iOutRet;

}


INT_t CNexusExtImpl::SetChannelUp()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	
	if(m_TunerSrv_sp.isNull())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}
	
	iRet = m_TunerSrv_sp->OnChannelUp();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:		

	return iOutRet;

}
INT_t CNexusExtImpl::SetChannelDown()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	if(m_TunerSrv_sp.isNull())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}
	
	iRet = m_TunerSrv_sp->OnChannelDown();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:		

	return iOutRet;

}

INT_t CNexusExtImpl::GetTunerStandardMode(int* iStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	iRet=OpenTunerPlayer();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}
	
	iRet = m_TunerSrv_sp->OnGetTunerMode(iStdMode);
	if(ERROR_SUCCESS == iRet)
	{
	}
	else if(ERROR_NOT_FOUND == iRet)
	{
		iRet = m_TunerSrv_sp->OnGetCurrStdMode(iStdMode);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:		

	return iOutRet;
}

INT_t CNexusExtImpl::GetSignalStrength(unsigned int* Strength)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CTunerSrv> pTunerSrv_sp;
	UINT32_t iStrength;

	if(m_TunerSrv_sp.isNull())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}

	iRet = m_TunerSrv_sp->OnGetStrength(&iStrength);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	*Strength=iStrength;
	
EXIT_PROC:
	return iOutRet;
}


INT_t CNexusExtImpl::PsfPlayback()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	
	iRet = m_TunerSrv_sp->OnPsfSetPmt();
	if(ERROR_SUCCESS != iRet)
	{
		//PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
	}
	
EXIT_PROC:
	return iOutRet;

}

void CNexusExtImpl::CallBackScanProgress(void* pContext,UINT16_t *pRrogramNum,UINT16_t *pScanRrogress)
{
	CNexusExtImpl * pNexusExt = (typeof(pNexusExt))pContext;
	if(CC_UNLIKELY(NULL == pNexusExt))
	{
		LOG_BLINE("BUG,pContext is NULL.\n");
		goto EXIT_PROC;
	}

	if(NULL == pRrogramNum || NULL == pScanRrogress)
	{
		goto EXIT_PROC;
	}
	
	pNexusExt->FN_cbScanProgress(pRrogramNum, pScanRrogress);

EXIT_PROC:

	;	
}


void CNexusExtImpl::FN_cbScanProgress(UINT16_t *pRrogramNum,UINT16_t *pScanRrogress)
{
	INT_t iRet;
	UINT32_t uiProgramNum = 0;
	UINT32_t uiScanRrogress = 0;
	SharedPtr <CEventListener> evtListener_sp;

	do{
		if(NULL == pRrogramNum || NULL == pScanRrogress)
		{
			break;
		}

		{
			CAutoScopeLock AutoRangeLocker(&m_TunerScanMutex);
			evtListener_sp = m_EvtListener_wp.Promote();
		}

		if(evtListener_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		
		uiProgramNum |= *pRrogramNum;
		uiScanRrogress |= *pScanRrogress;
		
		evtListener_sp->NotifyScanProgress(uiProgramNum,uiScanRrogress);
	}while(FALSE);

}


INT_t CNexusExtImpl::OpenTunerPlayer()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT64_t StartTimeMs = GetSysUpTimeMs(), CurTimeMs;

	iRet = CreateMediaPlayer();
	if(ERR_SUCCESS != iRet || NULL == m_pMediaPlayer)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	if(m_TunerSrv_sp.isNull())
	{
		m_TunerSrv_sp = SharedPtr <CTunerSrv> (new CTunerSrv(m_pMediaPlayer));
		if(m_TunerSrv_sp.isNull())
		{
			iOutRet=ERROR_OUT_OF_MEMORY;
			goto EXIT_PROC; 
		}
		
		iRet=m_TunerSrv_sp->InitInstance();
		if(ERR_SUCCESS !=iRet )
		{
			iOutRet=iRet;
			goto EXIT_PROC;
		}
		
	}

	if(FALSE == m_TunerSrv_sp->OnCheckInit())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;	
	}

	if(FALSE == m_bTunerOpenSucc)
	{
		iRet=m_TunerSrv_sp->OpenTuner();
		if( ERROR_SUCCESS != iRet)
		{
			iOutRet=iRet;
			m_bTunerOpenSucc=FALSE;
			goto EXIT_PROC;
		}
		m_bTunerOpenSucc=TRUE;
	}
	
	CurTimeMs = GetSysUpTimeMs();

	LOG("OpenTuner took %llu ms\n", (CurTimeMs-StartTimeMs));

	return iOutRet;

EXIT_PROC:
	
	if(!m_TunerSrv_sp.isNull())
	{
		m_TunerSrv_sp.Clear();
	}

	return iOutRet;
}

#endif



