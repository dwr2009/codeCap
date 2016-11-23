#include "NexusPlayer.h"
#include <MediaPlayerApi.h>
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <NexusExtApi.h>

QNexusPlayer::QNexusPlayer()
{
	INT_t iRet;
	
	m_bNxServerIsConnected = FALSE;

	m_NexusExt_sp = SharedPtr <CNexusExt> (CNexusExtImpl_new());
	if(m_NexusExt_sp.isValid())
	{
		ConnectToNxServer();
	}
}

QNexusPlayer::~QNexusPlayer()
{
	INT_t iRet;

	if(m_NexusExt_sp.isValid())
	{
		DisconnectFromNxServer();
	}
	m_NexusExt_sp.Clear();
}

INT_t QNexusPlayer::setThisWp(WeakPtr <QNexusPlayer> this_wp)
{
	INT_t iRet;

	m_this_wp = this_wp;
	iRet = m_NexusExt_sp->setEventListener(m_this_wp);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	return ERR_SUCCESS;
}


INT_t QNexusPlayer::Play(const QString & strUrl, CONST BOOL_t bLoopMode/* = FALSE*/)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->Play(strUrl.toUtf8().constData(), bLoopMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::SwitchToPlay()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->SwitchToPlay();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::SwitchToPause()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->SwitchToPause();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}
/*closed caption*/
INT_t QNexusPlayer::setShowClosedCaption(qint32 iCcValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->setShowClosedCaption(iCcValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}


INT_t QNexusPlayer::mibSwitchToFastForward(qint32 iPlaySpeed)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->mibSwitchToFastForward(iPlaySpeed);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::SwitchToFastForward()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->SwitchToFastForward();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::SwitchToRewind()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->SwitchToRewind();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::getPlayMode(OUT MediaPlayer::PlayMode & ePlayMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->getPlayMode(OUT ePlayMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::Stop()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;		
	if(m_NexusExt_sp.isValid())
	{
		iRet = m_NexusExt_sp->StopPlayback();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::ChangeVolume(int VolumePercentToChange)

{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isValid())
	{
		iRet = m_NexusExt_sp->ChangeVolume(VolumePercentToChange);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
EXIT_PROC:
	
	return iOutRet;
}


INT_t QNexusPlayer::setVideoPosition(int x, int y, int w, int h)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isValid())
	{
		iRet = m_NexusExt_sp->setVideoPosition(x, y, w, h);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, 
	VIDEO_OUTPUT_MODE * peNewVoMode /*= NULL*/,VIDEO_OUTPUT_MODE * peOldVoMode /*= NULL*/)
{
    INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
    iRet = m_NexusExt_sp->ChangeVideoOutputMode(eVoMode, peNewVoMode, peOldVoMode);
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}

void QNexusPlayer::OnEndOfStream()
{
	//LOG_BLINE("CNexusExt::OnEndOfStream\n");
	emit sigOnEndOfStream();
}

void QNexusPlayer::onVidDecStreamChanged()
{
	emit sigOnVidDecStreamChanged();
}

#ifdef ENABLE_DTV
void QNexusPlayer::NotifyTuPlayerPropmtMsg(int MsgTag)
{
	emit sigNotifyTuPlayerPropmtMsg(MsgTag);
}

void QNexusPlayer::NotifyScanProgress(UINT32_t PgmNum,UINT32_t Progress)
{	
	emit sigMPNotifyPlayerScanProgress(PgmNum,Progress);
}

void QNexusPlayer::Tuner_StartScan()
{
	emit sigTuner_StartScan();
}
void QNexusPlayer::Tuner_StopScan()
{
	emit sigTuner_StopScan();
}

INT_t QNexusPlayer::GetFirstChNum(unsigned int* OMajor,unsigned int* OMinor)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->GetFirstChNum(OMajor, OMinor);
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}

INT_t QNexusPlayer::GetSignalStrength(unsigned int* Strength)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->GetSignalStrength(Strength);
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}

INT_t QNexusPlayer::ResetDuration()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->ResetDuration();
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;	
}


INT_t QNexusPlayer::SetChannelUp()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->SetChannelUp();
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;	
}

INT_t QNexusPlayer::SetChannelDown()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->SetChannelDown();
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;	
}

INT_t QNexusPlayer::GetTunerStandardMode(int* iStdMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->GetTunerStandardMode(iStdMode);
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;	
}

INT_t QNexusPlayer::GetEntryChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
    iRet = m_NexusExt_sp->GetEntryChannelInfo(iEntryId,pChannelNum,pNumofSubChannel,pStrPids);
	if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}
#endif


INT_t QNexusPlayer::setDisplayOutputMode(INT32_t displayMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusExt_sp->setDisplayOutputMode(displayMode);
	if(ERR_SUCCESS != iRet){
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::getDisplayOutputMode(INT32_t& displayMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusExt_sp->getDisplayOutputMode(displayMode);
	if(ERR_SUCCESS != iRet){
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::setvideoColorSpace(INT32_t colorspace)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;	
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusExt_sp->setvideoColorSpace(colorspace);
	if(ERR_SUCCESS != iRet){
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::getvideoColorSpace(INT32_t& colorspace)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusExt_sp->getvideoColorSpace(colorspace);
	if(ERR_SUCCESS != iRet){
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}



void QNexusPlayer::ConnectToNxServer()
{	
	INT_t iOutRet = ERR_SUCCESS, iRet;
	
	if(TRUE == m_bNxServerIsConnected)
	{
		goto EXIT_PROC;
	}

	if(m_NexusExt_sp.isValid())
	{
		iRet = m_NexusExt_sp->NexusExt_NxClient_Join(MP_NexusNxClientName);
		if(ERR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			goto EXIT_PROC;
		}
	}

	m_bNxServerIsConnected = TRUE;	
	
EXIT_PROC:
	if(ERR_SUCCESS != iOutRet)
	{
		DisconnectFromNxServer();
	}
}

void QNexusPlayer::DisconnectFromNxServer()
{
	INT_t iRet;
	
	if(FALSE == m_bNxServerIsConnected)
	{
		goto EXIT_PROC;
	}

	if(m_NexusExt_sp.isValid())
	{
		iRet = m_NexusExt_sp->NexusExt_NxClient_Uninit();
		if(ERR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			goto EXIT_PROC;
		}
	}

	m_bNxServerIsConnected = FALSE;
	
EXIT_PROC:
	;
}

void QNexusPlayer::ReconnectToNxServer()
{
	DisconnectFromNxServer();
	ConnectToNxServer();
}

INT_t QNexusPlayer::scanI2cDev()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->scanI2cDev();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::setGpioOutputVal(const int iGpioNum, const bool bVal)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->setGpioOutputVal(iGpioNum, bVal);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::setStereoscopic3DFormat(int* pStereoscopic3D)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->setStereoscopic3DFormat(pStereoscopic3D);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::setVidWinAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->setVidWinAspectRatio(eDispAspectRatio);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::changeHdmiDisplaySettings(int iDispParam,int iValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->changeHdmiDisplaySettings(iDispParam,iValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	if(ERROR_FILE_WRITE_FAIL == iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::getMonitorInfo(OUT CString & strMonitorType)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->getMonitorInfo(strMonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::MibsetMonitorInfo(qint32 MonitorType)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->MibsetMonitorInfo(MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::MibgetMonitorInfo(OUT qint32 & MonitorType)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->MibgetMonitorInfo(MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::getDisplayParam(const int iDispParam,OUT int &oValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->getDisplayParam(iDispParam,oValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QNexusPlayer::setDisplayParam(int iDispParam,int iValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_NexusExt_sp->setDisplayParam(iDispParam,iValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;

}

INT_t QNexusPlayer::setOutputSpdifMode(int iSpdifMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setOutputSpdifMode(iSpdifMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setHdmiAudioOutput(int bHdmiAudioOutput)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setHdmiAudioOutput(bHdmiAudioOutput);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setAnalogAudioMute(int bAnalogAudioMute)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setAnalogAudioMute(bAnalogAudioMute);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec,int izone)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setHwRtcTime(iyear,imonth,imday,ihour,imin,isec,izone);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::getHwRtcTime(OUT CString& ConfigDateTime)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->getHwRtcTime(ConfigDateTime);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer:: getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->getDisplayPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer:: getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->getVideoPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer:: setOsdPosition(int x,int y,int width,int height,int type)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setOsdPosition(x,y,width,height,type);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


INT_t QNexusPlayer:: setOsdSize(int width,int height)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setOsdSize(width,height);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

//sync
INT_t QNexusPlayer::getCurrentTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->getCurrentTime(uTimestamp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::getLastTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->getLastTime(uTimestamp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setSyncTime(const LONG & Offset)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setSyncTime(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setSyncSeek(const LONG & Offset)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setSyncSeek(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::Seek(const unsigned int & Offset)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->Seek(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::setHdmiCecCtrl(int HdmiCecCmd)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->setHdmiCecCtrl(HdmiCecCmd);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


#ifdef ENABLE_DTV
INT_t QNexusPlayer::PlayTuner(const QString & strUrl)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->PlayTuner(strUrl.toUtf8().constData());
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::TunerScan()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->TunerScan();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::GetTunerChannelMode(int* iTuChMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->GetTunerChannelMode(iTuChMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QNexusPlayer::SetTunerChannelMode(int iTuChMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->SetTunerChannelMode(iTuChMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


INT_t QNexusPlayer::SetTunerStandardMode(int iTuStMode)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusExt_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusExt_sp->SetTunerStandardMode(iTuStMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

#endif


