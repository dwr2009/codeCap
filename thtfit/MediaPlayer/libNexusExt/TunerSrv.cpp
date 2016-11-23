#ifndef	ALLOW_OS_CODE
#define	ALLOW_OS_CODE
#endif	//ALLOW_OS_CODE
#include "TunerSrv.h"
#include <memory.h>
#include <Compiler.h>
#include <AutoLock.h>
#include <string.h>
#include <stdlib.h>
#include <DateTime.h>
//#include <drx_driver.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "DbgLogSwitchDef.h"
#include "StackBufString.h"
//#include "MediaPlayerApp.h"
#include <SysBaseSrvApi.h>
#include "CMxl603TunerIface.h"
//#include "En4020TunerIface.h"
#include "FixChannelTable.h"
#include "tspsimgr3.h"
#include "nexus_surface_client.h"
#include "nexus_platform.h"
#include "nexus_simple_video_decoder.h"
#include "nexus_simple_audio_decoder.h"
#include "nexus_input_client.h"
#include "nxclient.h"
#include "ProcessRunningInfo.h"


static UINT32 CalculateCRC(UINT8_t *buffer, UINT32 size)
{
	UINT32_t crc_table[256];
	UINT32_t c;
	UINT16_t n, k;
   
	for (n = 0; n < 256; n++) {
		c = (UINT32_t) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	
	// get crc
	c = 0xffffffffL;
	for (n = 0; n < size; n++) {
		c = crc_table[(c ^ buffer[n]) & 0xff] ^ (c >> 8);
	}

	return c ^ 0xffffffffL;
}

#define DRXJ2_EN4020_TUNER_I2C_ADDR 0x52
#define MN88436_MXL603_TUNER_I2C_ADDR 0x30

INT_t CTunerSrv::SaveProgramTable()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t chFd = -1;
	ssize_t ret = -1;

	do{
		chFd = open(PROGRAM_TABLE_FILE,O_WRONLY|O_CREAT | O_SYNC);
		if(chFd < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		m_DtvProgramCfg.crc = CalculateCRC((UINT8_t*)&m_DtvProgramCfg, sizeof(m_DtvProgramCfg)/2);

		ret = write(chFd, &m_DtvProgramCfg, sizeof(m_DtvProgramCfg));
		fsync(chFd);
        close(chFd);
		if(ret < 0)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnReloadProgram()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		iRet = LoadProgramTable();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::LoadProgramTable()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t chFd = -1;
	UINT32_t current_crc;
	UINT32_t calculated_crc;

	do{
		ZeroMemory(&m_DtvProgramCfg,sizeof(m_DtvProgramCfg));

		if(0 != access(PROGRAM_TABLE_FILE,F_OK))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		
		chFd = open(PROGRAM_TABLE_FILE,O_RDONLY | O_SYNC);
		if(chFd < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		iRet = read(chFd,&m_DtvProgramCfg,sizeof(m_DtvProgramCfg));
		if(0 >= iRet)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
		
		close(chFd);

		current_crc = m_DtvProgramCfg.crc;
		m_DtvProgramCfg.crc = 0;
		calculated_crc = CalculateCRC((UINT8_t*)&m_DtvProgramCfg, sizeof(m_DtvProgramCfg)/2);
		if(current_crc != calculated_crc)
		{
			iOutRet = ERROR_LOAD_FAIL;
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		m_DtvProgramCfg.numDChannel = 0;
	}

	return iOutRet;
}

INT_t CTunerSrv::OnGetChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t iNumProg = 0,i = 0;
	char tmp[64] = {0};

	do{
		if(iEntryId < 0 || NULL == pChannelNum 
			|| NULL == pNumofSubChannel || NULL == pStrPids)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if((iEntryId-1) >= m_TotalChannel || m_ChProgInfo[iEntryId-1].numProgram <= 0)
		{
			iOutRet = ERR_NO_MORE_DATA;
			break;
		}

		(*pNumofSubChannel) = m_ChProgInfo[iEntryId-1].numProgram;

		(*pChannelNum) = m_ChProgInfo[iEntryId-1].pRgProgram.program_info[0].major_number;
		
		iNumProg = m_ChProgInfo[iEntryId-1].numProgram;
		
		for(i = 0; i < iNumProg;i ++)
		{
			if(strlen(pStrPids) >= sizeof(tmp)-1)
			{
				break;
			}

			ZeroMemory(tmp,sizeof(tmp));
			
			sprintf(tmp,"%d  ",m_ChProgInfo[iEntryId-1].pRgProgram.program_info[i].minor_number);
			strcat(pStrPids,tmp);
		}
		
	}while(FALSE);

	return iOutRet;
}

pProgramInfo CTunerSrv::ProgramInfoNew(UINT16_t numProgram)
{
	pProgramInfo pPGInfo = NULL;

	do{
		pPGInfo = (pProgramInfo) malloc(numProgram*sizeof(ProgramInfo));
		if(NULL != pPGInfo)
		{
			ZeroMemory(pPGInfo,sizeof(numProgram*sizeof(ProgramInfo)));
		}
		
	}while(FALSE);

	return pPGInfo;
}

INT_t CTunerSrv::ProgramInfoDel(pProgramInfo pPGInfo)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pPGInfo)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		free(pPGInfo);

		pPGInfo = NULL;
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::FillProgramInfo(UINT16_t numProgram,UINT32_t offset,tspsimgr_program* pPGInfo)
{

	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32 Idx,j = 0;

	do{
		if(NULL == pPGInfo || 0 == numProgram)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		for(Idx=offset; Idx < offset+numProgram; Idx++)
		{
			pPGInfo[j].pcr_pid = m_DtvProgramCfg.astDChannel[Idx].pcr_pid;
			pPGInfo[j].map_pid = m_DtvProgramCfg.astDChannel[Idx].map_pid;
			pPGInfo[j].program_number = m_DtvProgramCfg.astDChannel[Idx].program_number;
			pPGInfo[j].free_CA_mode = m_DtvProgramCfg.astDChannel[Idx].free_CA_mode;
			
			//video
			pPGInfo[j].num_video_pids = m_DtvProgramCfg.astDChannel[Idx].num_video_pids;
			memcpy(pPGInfo[j].video_pids,m_DtvProgramCfg.astDChannel[Idx].video_pids,sizeof(pPGInfo[j].video_pids));

			//audio
			pPGInfo[j].num_audio_pids = m_DtvProgramCfg.astDChannel[Idx].num_audio_pids;
			memcpy(pPGInfo[j].audio_pids,m_DtvProgramCfg.astDChannel[Idx].audio_pids,sizeof(pPGInfo[j].audio_pids));

			// other
			pPGInfo[j].num_other_pids = m_DtvProgramCfg.astDChannel[Idx].num_other_pids;
			memcpy(pPGInfo[j].other_pids,m_DtvProgramCfg.astDChannel[Idx].other_pids,sizeof(pPGInfo[j].other_pids));

			ZeroMemory(pPGInfo[j].short_name,sizeof(pPGInfo[j].short_name));
			pPGInfo[j].major_number = m_DtvProgramCfg.astDChannel[Idx].major_number;
			pPGInfo[j].minor_number = m_DtvProgramCfg.astDChannel[Idx].minor_number;
			pPGInfo[j].programIndex = m_DtvProgramCfg.astDChannel[Idx].programIndex;
			memcpy(pPGInfo[j].short_name,m_DtvProgramCfg.astDChannel[Idx].short_name,sizeof(pPGInfo[j].short_name));
			j ++;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::RestoreProgramInfo()
{

	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t Index ;
	UINT16_t numProgram = 0;
	UINT16_t chIdx;
	UINT32_t i = 0;

	do{
		if(0 == m_DtvProgramCfg.numDChannel)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

	
		for(i = 0; i < MAX_NUMBER_OF_CHANNELS; i ++)
		{
			ZeroMemory(m_ChProgInfo[i].pRgProgram.program_info,sizeof(m_ChProgInfo[i].pRgProgram.program_info));
		}

		ZeroMemory(m_ChProgInfo,sizeof(m_ChProgInfo));
		
		Index = 0;
		chIdx = 0;
		
		while(Index < m_DtvProgramCfg.numDChannel)
		{
			if(chIdx >= MAX_NUMBER_OF_CHANNELS)
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}

			if(m_DtvProgramCfg.astDChannel[Index].numProgram > 0)
			{
				numProgram = m_DtvProgramCfg.astDChannel[Index].numProgram;
				m_ChProgInfo[chIdx].freq = m_DtvProgramCfg.astDChannel[Index].freq;
				m_ChProgInfo[chIdx].chBandwidth = m_DtvProgramCfg.uiBandwidth;
				m_ChProgInfo[chIdx].chStandard = m_DtvProgramCfg.uiStandard;
				m_ChProgInfo[chIdx].chConstellation = m_DtvProgramCfg.uiConstellation;
				m_ChProgInfo[chIdx].numProgram = numProgram;

				iRet = FillProgramInfo(numProgram,Index,m_ChProgInfo[chIdx].pRgProgram.program_info);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
			Index += numProgram;
			chIdx ++;
		}

		m_TotalChannel = chIdx;
	}while(FALSE);

	return iOutRet;

}

CTunerSrv::CTunerSrv(media_player_t MediaPlayer)
{
	INT_t iRet;
	m_TotalChannel = 0;
	m_bInited = FALSE;
	m_bDtvChannelLoadSucc = FALSE;
	m_currChIdx = 0;
	m_currPgIdx = 0;
	m_currProgramIndex = 0;
	m_pCbScanProgress = NULL;
	m_iTuPlayStatus = PLAY_STAT_IDLE;
	m_DefaultMajorNo = MAJOR_NO_VCT_DEFAULT;
	m_bNeedSetTunerMode = FALSE;
	m_FixedFreqKHz = 0;
	m_StopTunerFlag=FALSE;
	m_parserBand=(NEXUS_ParserBand)-1;
	m_MediaPlayer=MediaPlayer;
}
CTunerSrv::~CTunerSrv()
{
	INT32_t iRet = ERROR_SUCCESS;
	UINT32_t Idx = 0;
	m_TunerIface_sp.Clear();
}

INT_t CTunerSrv::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CBaseTunerIface> FactoryTunerIface_sp;

	if(m_TunerIface_sp.isNull())
	{
		FactoryTunerIface_sp=SharedPtr<CBaseTunerIface>(new CMxl603TunerIface());
	}

	m_TunerIface_sp=FactoryTunerIface_sp;
	
	if(m_TunerIface_sp.isNull())
	{
		LOG_BLINE("CANN'T CreatFactoryTuner\n");
		iOutRet = ERROR_NOT_FOUND;
		goto EXIT_PROC;
	}

	iRet = m_TunerIface_sp->InitVal();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}
	
	ZeroMemory(&m_ChProgInfo,sizeof(m_ChProgInfo));
	
	iRet = LoadProgramTable();
	if(CC_UNLIKELY(ERROR_SUCCESS == iRet))
	{
		iRet = RestoreProgramInfo();
		if(ERROR_SUCCESS == iRet)
		{
			m_bDtvChannelLoadSucc = TRUE;
		}
	}
	else
	{
		m_bDtvChannelLoadSucc = FALSE;
	}

	iRet = SysPropChannelParaValid();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet)) 
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}
	
	m_bInited = TRUE;

EXIT_PROC:	
	return iOutRet;
}


BOOL_t CTunerSrv::OnCheckInit()
{
	return m_bInited;
}

INT_t CTunerSrv::FindVirtualChannelNumInProgramTable(pTunerUrlInfo pUrlInfo,bool *pbFound)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT32_t Index,SubChIdx;
	BOOL_t bFound = FALSE;

	do{
		if(NULL == pUrlInfo || NULL == pbFound)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		Index = 0;
		SubChIdx = 0;
		
		while(Index <= m_TotalChannel)
		{
			if(0 == m_ChProgInfo[Index].numProgram)
			{
				Index ++;
				continue;
			}

			for(SubChIdx = 0; SubChIdx < m_ChProgInfo[Index].numProgram;SubChIdx ++)
			{
				if(pUrlInfo->MajorlNum == m_ChProgInfo[Index].pRgProgram.program_info[SubChIdx].major_number
					&& pUrlInfo->MinorlNum == m_ChProgInfo[Index].pRgProgram.program_info[SubChIdx].minor_number)
				{
					bFound = TRUE;
					break;
				}
			}

			if(TRUE == bFound)
			{
				m_currChIdx = Index;
				m_currPgIdx = SubChIdx;
				break;
			}
			
			Index ++;
		}

		if(TRUE == bFound)
		{
			*pbFound = TRUE;
			break;
		}
		else
		{
			*pbFound = FALSE;
			m_currChIdx = 0;
			m_currPgIdx = 0;
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnSetScanProgressCb(pTunerScanProgressCk pCbTuScanProgress)
{
	INT_t iOutRet = ERROR_SUCCESS;

	if(NULL == pCbTuScanProgress)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	m_pCbScanProgress = pCbTuScanProgress;

EXIT_PROC:

	return iOutRet;
}

INT_t CTunerSrv::OnSetChannelInfo()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t Idx = 0;
	StandardChannle iOutStandard = STANDARD_UNKNOW;
	INT32_t iChannelMode = 0;
	INT32_t currFreqInHz = 0;

	do{
		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = OnGetTunerChannelMode(&iChannelMode);

		if(ERROR_SUCCESS == iRet)
		{}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iChannelMode = CHANNEL_MODE_DEFAULT;
		}
		else
		{
			iOutRet = iRet;
			break;
		}

		Idx = m_currChIdx;
		if(CHANNEL_MODE_FIXED_SCANNED == iChannelMode)
		{
			if(Idx >= m_TotalChannel)
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}
		}
		
		if(TRUE == m_bNeedSetTunerMode)
		{
			INT32_t iModeType = -1;

			m_bNeedSetTunerMode = FALSE;
			
			iRet = OnGetTunerMode(&iModeType);
			if((ERROR_SUCCESS == iRet) && 
				(iModeType >= 0 && iModeType <= 2))
			{
				iRet = SetChannelStandMode(&iModeType);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					iOutRet = iRet;
					break;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
				
		if(CHANNEL_MODE_FIXED_SCANNED == iChannelMode 
			|| CHANNEL_MODE_DEFAULT == iChannelMode)
		{
			iRet = m_TunerIface_sp->GetStandard(&iOutStandard);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			if(iOutStandard != m_ChProgInfo[Idx].chStandard)
			{
				iOutRet = ERR_TU_STANDARD_DISAGREEMENT;
				break;
			}

			currFreqInHz = m_ChProgInfo[Idx].freq;
		}
		else
		{
			currFreqInHz = m_FixedFreqKHz*1000;
		}
		
		LOG_BLINE("FREQ : %d\n",currFreqInHz);
		iRet = m_TunerIface_sp->SetChannelInfo(currFreqInHz);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		m_FixedFreqKHz = 0;

	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnWaitLock(UINT32_t *pLock)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == pLock || m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = m_TunerIface_sp->WaitForLock(pLock);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnSetTunerChannelMode(INT32_t *pInChMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strTunerChannelMode, 32);
	INT32_t InChMode = 0;
	INT32_t iSetTuStdMode = 0;

	do{
		if(NULL == pInChMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		InChMode = *pInChMode;
		iRet = strTunerChannelMode.Format("%d", InChMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SysProp_setPersistent(SysProp_KEY_TunerChannelMode, (LPCSTR)strTunerChannelMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(CHANNEL_MODE_FIXED_ATSC == InChMode)
		{
			iSetTuStdMode = 0;
			iRet = OnSetTunerMode(&iSetTuStdMode);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(CHANNEL_MODE_FIXED_QAM64 == InChMode)
		{
			iSetTuStdMode = 1;
			iRet = OnSetTunerMode(&iSetTuStdMode);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(CHANNEL_MODE_FIXED_QAM256 == InChMode)
		{
			iSetTuStdMode = 2;
			iRet = OnSetTunerMode(&iSetTuStdMode);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnGetTunerChannelMode(INT32_t *pOutChMode)
	
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	DECLARE_CLS_STACK_BUF_STRING(TunerChannelModeValue, 32);
	CStackBufString::SCANF_PARAM oScanfParam;
	CStackBufString::ScanfParam_Init(&oScanfParam);

	do{
		if(NULL == pOutChMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = SysProp_get(SysProp_KEY_TunerChannelMode, OUT TunerChannelModeValue);
		if(ERROR_NOT_FOUND == iRet)
		{
			//default value
			*pOutChMode = CHANNEL_MODE_DEFAULT;
			break;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			//PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}

		
		iRet = TunerChannelModeValue.Scanf(IN OUT oScanfParam, "%d", pOutChMode);
		if(ERROR_SUCCESS != iRet)
		{
			//PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);
	
	return iOutRet;
}

void CTunerSrv::SetMediaStartSetting(media_player_start_settings* mediaPlayStartSet)
{
	m_playsetting=*mediaPlayStartSet;
}

INT_t CTunerSrv::OnPsfSetPmt()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t Idx = 0,PgIdx = 0;
	INT32_t iTunerChMode = 0;
	
	media_player_t pMediaPlayer=NULL;
	
	{
		CAutoLock AutoRangeLocker(&m_SharedDataLock);
		pMediaPlayer=m_MediaPlayer;
	}
	
	do{
		iRet = OnGetTunerChannelMode(&iTunerChMode);
		if(ERROR_NOT_FOUND == iRet)
		{
			iTunerChMode = CHANNEL_MODE_DEFAULT;
		}
		else if(ERROR_SUCCESS == iRet)
		{
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		
		if(CHANNEL_MODE_DEFAULT == iTunerChMode
			|| CHANNEL_MODE_FIXED_SCANNED == iTunerChMode)
		{
			Idx = m_currChIdx;

			if(Idx >= m_TotalChannel )
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}
			
			PgIdx = m_currPgIdx;
			
			if(PgIdx > m_ChProgInfo[Idx].numProgram)
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}
			
			iRet = media_player_tuner_start(pMediaPlayer,&m_playsetting,&m_ChProgInfo[Idx].pRgProgram.program_info[PgIdx]);
			
			if(iRet)
			{
				iOutRet = ERR_TU_SETPMT_FAIL;
				break;
			}

		}
		else	//fixed table
		{
			m_playsetting.pmtIndex = m_TunerUrlInfo.MinorlNum - 1;	
			iRet = media_player_tuner_start(pMediaPlayer,&m_playsetting,NULL);
			if(iRet)
			{
				iOutRet = ERR_TU_SETPMT_FAIL;
				break;
			}
		}

		INT_t iOldPlayStatus = m_iTuPlayStatus;
		m_iTuPlayStatus = PLAY_STAT_PLAYING;
		iRet = m_Mp7xxGlobalStatusData.setMpSrvPlayStatus(m_iTuPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStatus != m_iTuPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t CTunerSrv::OnSetUrl(LPCSTR pszChStrIndex,bool *pbFoundInChTab)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	const char *pStr = NULL;

	do{
		if(NULL == pszChStrIndex ||NULL == pbFoundInChTab)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		{
			CAutoLock AutoRangeLocker(&m_SharedDataLock);
			m_strChIndexUrl = pszChStrIndex;	//copy
		}
		
		pStr = strchr(pszChStrIndex,'#');
		if(NULL == pStr)
		{
			iRet = ParseChannelInfoUrl(pszChStrIndex);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			if(1)
			{
				LOG("CH:%d-%d\n",m_TunerUrlInfo.MajorlNum,m_TunerUrlInfo.MinorlNum);
			}

			iRet = FindVirtualChannelNumInProgramTable(&m_TunerUrlInfo,pbFoundInChTab);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			UINT32_t i = 0,j = 0;
			char tmp[64] = {0};
			UINT32 ProgramIndex = 0;
			BOOL_t bFound = FALSE;
			
			while(pStr[i] != '\0')
			{
				if(pStr[i] == '#')
				{
					i ++;
					continue;
				}

				tmp[j++] = pStr[i++];
			}

			ProgramIndex = atoi(tmp);

			LOG("CH INDEX %d\n",ProgramIndex);
			i = 0;
			while(i < m_TotalChannel)
			{
				if(0 == m_ChProgInfo[i].numProgram)
				{
					i ++;
					continue;
				}

				for(j = 0; j < m_ChProgInfo[i].numProgram;j ++)
				{
					if(ProgramIndex == m_ChProgInfo[i].pRgProgram.program_info[j].programIndex)
					{
						bFound = TRUE;
						break;
					}
				}

				if(TRUE == bFound)
				{
					m_currChIdx = i;
					m_currPgIdx = j;
					m_currProgramIndex = ProgramIndex;
					break;
				}

				i ++;
			}

			if(TRUE == bFound)
			{
				(*pbFoundInChTab) = TRUE;
				break;
			}
			else
			{
				(*pbFoundInChTab) = FALSE;
				m_currChIdx = 0;
				m_currPgIdx = 0;
				m_currProgramIndex = 0;
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OpenTuner()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT32_t iModeType = -1;
	media_player_t pMediaPlayer;
	
	do
	{
		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = m_TunerIface_sp->OpenTunerModule();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
	
		iRet = OnGetTunerMode(&iModeType);
		if((ERROR_SUCCESS == iRet) && 
			(iModeType >= 0 && iModeType <= 2))
		{
			iRet = SetChannelStandMode(&iModeType);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				iOutRet = iRet;
				break;
			}
		}
		else 
		{
			if(TRUE == m_bDtvChannelLoadSucc)
			{
				if(STANDARD_ITU_B != m_DtvProgramCfg.uiStandard
					&& STANDARD_8VSB != m_DtvProgramCfg.uiStandard)
				{
					break;
				}
				
				iRet = m_TunerIface_sp->SetStandard(m_DtvProgramCfg.uiStandard,m_DtvProgramCfg.uiConstellation);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					iOutRet = iRet;
					break;
				}
			}
			else
			{
				iRet = m_TunerIface_sp->SetStandard(STANDARD_8VSB,0);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
			}
		}

		{
			CAutoLock AutoRangeLocker(&m_SharedDataLock);
			pMediaPlayer=m_MediaPlayer;
			iRet=media_player_tuner_prepare(pMediaPlayer,&m_parserBand);
			if(iRet || -1 == m_parserBand)
			{
				iOutRet = iRet;
				break;
			}
		}
		
		m_StopTunerFlag=FALSE;
		
	}while(FALSE);
	
	return iOutRet;
}

INT_t CTunerSrv::CloseTuner()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	
	INT_t iOldPlayStatus = m_iTuPlayStatus;
	m_iTuPlayStatus = PLAY_STAT_IDLE;
	iRet = m_Mp7xxGlobalStatusData.setMpSrvPlayStatus(m_iTuPlayStatus);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	if(iOldPlayStatus != m_iTuPlayStatus)
	{
		iRet = m_PlayStatChangedEvent.SetEvent();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	m_StopTunerFlag=TRUE;

EXIT_PROC:

	return iOutRet;
}

INT_t CTunerSrv::FindChannelIndexInFixedTable(LPCSTR pChStrIndex,bool *pFound)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT32_t iChannelMode = 0;
	const char *pStr = NULL;
	UINT32_t idx = 0;

	do{
		if(NULL == pChStrIndex || NULL == pFound)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		{
			CAutoLock AutoRangeLocker(&m_SharedDataLock);
			m_strChIndexUrl = pChStrIndex;	//copy
		}
		
		ZeroMemory(&m_TunerUrlInfo,sizeof(TunerUrlInfo));
		pStr = strchr(pChStrIndex,'#');
		if(NULL != pStr)
		{
			*pFound = FALSE;
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		else
		{
			iRet = ParseChannelInfoUrl(pChStrIndex);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}	
		}

		iRet = OnGetTunerChannelMode(&iChannelMode);
		if(ERROR_SUCCESS == iRet)
		{}
		else if(ERROR_NOT_FOUND == iRet)
		{
			LOG_BLINE("Bug iRet : %d\n",iRet);
		}
		else
		{
			iOutRet = iRet;
			break;
		}

		if(CHANNEL_MODE_FIXED_ATSC == iChannelMode)
		{
			for(idx = 0; idx < sizeof(FixChannelFreq_ATSCTable)/sizeof(ChannelFreq); idx ++)
			{
				if(m_TunerUrlInfo.MajorlNum == FixChannelFreq_ATSCTable[idx].channelIndex)
				{
					*pFound = TRUE;
					m_FixedFreqKHz = FixChannelFreq_ATSCTable[idx].freqKhz;
					break;
				}
			}
		}
		else if(CHANNEL_MODE_FIXED_QAM64 == iChannelMode 
			|| CHANNEL_MODE_FIXED_QAM256 == iChannelMode)
		{
			for(idx = 0; idx < sizeof(FixChannelFreq_CATVTable)/sizeof(ChannelFreq); idx ++)
			{
				if(m_TunerUrlInfo.MajorlNum == FixChannelFreq_CATVTable[idx].channelIndex)
				{
					*pFound = TRUE;
					m_FixedFreqKHz = FixChannelFreq_CATVTable[idx].freqKhz;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnChannelUp()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t mainChIdx = 0;
	UINT32_t subChIdx = 0;
	UINT32_t Lock = 0;
	bool bWaitPatForce = FALSE;
	media_player_t pMediaPlayer;
	
	{
		CAutoLock AutoRangeLocker(&m_SharedDataLock);
		pMediaPlayer=m_MediaPlayer;
	}
	
	do{
		if(NULL == pMediaPlayer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		
		
		mainChIdx = m_currChIdx;
		subChIdx = m_currPgIdx;
		
		//NEW CHANNEL	
		if( (subChIdx +1) >= m_ChProgInfo[mainChIdx].numProgram)
		{
			UINT32_t Locked = 0;
			
			if(mainChIdx+1 >= m_TotalChannel)
			{
				mainChIdx = 0;
			}
			else
			{
				mainChIdx ++;
			}
			
			if(0 == m_ChProgInfo[mainChIdx].numProgram)
			{
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
			
			if(subChIdx == m_ChProgInfo[mainChIdx].numProgram)
			{
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
			
			media_player_stop(pMediaPlayer);
			
			iRet = m_TunerIface_sp->SetChannelInfo(m_ChProgInfo[mainChIdx].freq);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			iRet = m_TunerIface_sp->WaitForLock(&Locked);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			LOG_BLINE("Locked : %d\n",Locked);
			if(0 == Locked)
			{
				iOutRet = ERR_TU_GET_LOCK_FAIL;
			}
			
			bWaitPatForce = TRUE;
		}
		else // IN THE SAME CHANNEL
		{	
			if( ++subChIdx == m_ChProgInfo[mainChIdx].numProgram)
			{
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
			
			media_player_stop(pMediaPlayer);
			
			bWaitPatForce = FALSE;
		}
		
		LOG("UP CH:%d-%d\n",mainChIdx+1,subChIdx+1);
		
		iRet = media_player_tuner_start(pMediaPlayer,&m_playsetting,&m_ChProgInfo[mainChIdx].pRgProgram.program_info[subChIdx]);
		if(iRet)
		{
			iOutRet = ERR_TU_SETPMT_FAIL;
			break;
		}
		else if(ERROR_SUCCESS == iRet)
		{}
		else
		{
			iOutRet = ERR_TU_SETPMT_FAIL;
			break;
		}
		
		//UPDATE
		m_currChIdx = mainChIdx ;
		m_currPgIdx = subChIdx;
	}while(FALSE);
	
	return iOutRet;
}

INT_t CTunerSrv::OnChannelDown()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t mainChIdx = 0,subChIdx = 0;
	UINT32_t Lock = 0;
	bool bWaitPatForce = FALSE;
	media_player_t pMediaPlayer;

	{
		CAutoLock AutoRangeLocker(&m_SharedDataLock);
		pMediaPlayer=m_MediaPlayer;
	}
	
	
	do{
		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		mainChIdx = m_currChIdx;
		subChIdx = m_currPgIdx;
		
		if(0 == subChIdx)//New channel
		{
			UINT32_t Locked = 0;
			
			if(0 == mainChIdx)//FIRST
			{
				mainChIdx = m_TotalChannel -1;
			}
			else
			{
				mainChIdx --;
			}
			
			if(0 == m_ChProgInfo[mainChIdx].numProgram)
			{
				iOutRet = ERROR_NOT_FOUND;
				break;
			}

			subChIdx = m_ChProgInfo[mainChIdx].numProgram -1;
			
			do{
				subChIdx --;
			}while(subChIdx > 0);

			media_player_stop(pMediaPlayer);
			
			iRet = m_TunerIface_sp->SetChannelInfo(m_ChProgInfo[mainChIdx].freq);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			iRet = m_TunerIface_sp->WaitForLock(&Locked);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			LOG_BLINE("Locked : %d\n",Locked);
			if(0 == Locked)
			{
				iOutRet = ERR_TU_GET_LOCK_FAIL;
			}
			
			bWaitPatForce = TRUE;
		}
		else //In Same Channel
		{

			if(--subChIdx < 0)
			{
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
			
			media_player_stop(pMediaPlayer);
			
			bWaitPatForce = FALSE;
		}
		
		LOG("DOWN CH:%d-%d\n",mainChIdx+1,subChIdx+1);
		
		iRet = media_player_tuner_start(pMediaPlayer,&m_playsetting,&m_ChProgInfo[mainChIdx].pRgProgram.program_info[subChIdx]);
		if(iRet)
		{
			iOutRet = ERR_TU_SETPMT_FAIL;
			break;
		}
		else if(ERROR_SUCCESS == iRet)
		{}
		else
		{
			iOutRet = ERR_TU_SETPMT_FAIL;
			break;
		}

		//UPDATE
		m_currChIdx = mainChIdx;
		m_currPgIdx = subChIdx;
		
	}while(FALSE);
	
	return iOutRet;
}


INT_t CTunerSrv::OnReInitTunerModules()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	do{
		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = m_TunerIface_sp->ReInitModules();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnGetCurrStdMode(INT32_t *pStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	do{
		if(NULL == pStdMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(TRUE == m_bDtvChannelLoadSucc)
		{
			if(STANDARD_8VSB == m_DtvProgramCfg.uiStandard)
			{
				(*pStdMode) = 0;
			}
			else
			{
				if(5 == m_DtvProgramCfg.uiConstellation)
				{
					(*pStdMode) = 1;
				}
				else
				{
					(*pStdMode) = 2;
				}
			}
		}
		else 
		{
			(*pStdMode) = 0;
		}

	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnGetTunerMode(INT32_t *pChStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	DECLARE_CLS_STACK_BUF_STRING(TunerStdModeValue, 32);
	CStackBufString::SCANF_PARAM oScanfParam;
	CStackBufString::ScanfParam_Init(&oScanfParam);

	do{
		if(NULL == pChStdMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = SysProp_get(SysProp_KEY_TunerStdMode, OUT TunerStdModeValue);
		if(ERROR_SUCCESS != iRet)
		{
			//PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
		
		iRet = TunerStdModeValue.Scanf(IN OUT oScanfParam, "%x", pChStdMode);
		if(ERROR_SUCCESS != iRet)
		{
			//PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnSetTunerMode(INT32_t *pStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	DECLARE_CLS_STACK_BUF_STRING(strTunerStdMode, 32);
	INT32_t  iStdMode = -1;

	do{
		if(NULL == pStdMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iStdMode = (*pStdMode);
		
		iRet = strTunerStdMode.Format("0x%08x", iStdMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SysProp_setPersistent(SysProp_KEY_TunerStdMode, (LPCSTR)strTunerStdMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		m_bNeedSetTunerMode = TRUE;
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::SetChannelStandMode(INT32_t *pIStdModeType)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	INT32_t iModeType = -1;

	do{
		if(m_TunerIface_sp.isNull() || NULL == pIStdModeType)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iModeType = (*pIStdModeType);

		if(0 == iModeType)
		{
			iRet = m_TunerIface_sp->SetStandard(STANDARD_8VSB,iModeType);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}

			m_DtvProgramCfg.uiStandard = STANDARD_8VSB;
		}
		else
		{
			iRet = m_TunerIface_sp->SetStandard(STANDARD_ITU_B,iModeType);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}

			m_DtvProgramCfg.uiStandard = STANDARD_ITU_B;
		}
		usleep(1000);

		if(Sw_LogTuner)
		{
			LOG("Set Standard Done,ModeType:%d\n\n",iModeType);
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t CTunerSrv::ParseChannelInfoUrl(LPCSTR pStrChannelUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	char strBuf[64];
	UINT_t strLen = 0;
	const char *pMiddle = NULL;
	UINT_t iPos = 0;
	
	do{
		if(NULL == pStrChannelUrl)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		ZeroMemory(strBuf,sizeof(strBuf));
		strLen = strlen(pStrChannelUrl);
		if(strLen <= 8 && strLen-8 >= 64)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		pMiddle = strchr(pStrChannelUrl,'-');
		if(NULL == pMiddle)
		{
			memcpy(strBuf,pStrChannelUrl+8,strLen-8);
			m_TunerUrlInfo.MajorlNum = atoi(strBuf);
			m_TunerUrlInfo.MinorlNum = 1;
		}
		else
		{
			memcpy(strBuf,pStrChannelUrl+8,pMiddle-pStrChannelUrl-8);
			m_TunerUrlInfo.MajorlNum = atoi(strBuf);
			ZeroMemory(strBuf,sizeof(strBuf));
			memcpy(strBuf,pMiddle+1,strLen-(pMiddle-pStrChannelUrl)-1);
			m_TunerUrlInfo.MinorlNum = atoi(strBuf);
		}
	}while(FALSE);
	
	return iOutRet;
}

void CTunerSrv::DumpChannelInfo()
{
	UINT32_t i = 0;
	UINT16_t j = 0;
/*	
	do{
		LOG_BLINE("***********************************************\n");
		LOG_BLINE("numDChannel		%d\n",m_DtvProgramCfg.numDChannel);
		LOG_BLINE("uiBandwidth		%d\n",m_DtvProgramCfg.uiBandwidth);
		LOG_BLINE("uiStandard			%d\n",m_DtvProgramCfg.uiStandard);
		LOG_BLINE("uiConstellation		%d\n",m_DtvProgramCfg.uiConstellation);
		LOG_BLINE("***********************************************\n\n\n\n");

		LOG_BLINE("**************Digital Channel Info**************\n");
		for(i = 0;i < m_DtvProgramCfg.numDChannel;i ++)
		{
			for(j = 0;j < m_DtvProgramCfg.astDChannel[i].numVidEs;j ++)
			{
				if(j >= MAX_ES_NUMBER)
					break;
				LOG_BLINE("Vid ES INFO		%d/%d\n",j+ 1,m_DtvProgramCfg.astDChannel[i].numVidEs);
				LOG_BLINE("streamtype       0x%x\n",m_DtvProgramCfg.astDChannel[i].rgVidEs[j].stream_type);
				LOG_BLINE("elementary_pid   0x%x\n",m_DtvProgramCfg.astDChannel[i].rgVidEs[i].elementary_pid);  
				LOG_BLINE("ISO 639 code     %c%c%c\n",m_DtvProgramCfg.astDChannel[i].rgVidEs[i].ISO639_language[0],
									m_DtvProgramCfg.astDChannel[i].rgVidEs[i].ISO639_language[1],
									m_DtvProgramCfg.astDChannel[i].rgVidEs[i].ISO639_language[2]);
			}

			for(j = 0;j < m_DtvProgramCfg.astDChannel[i].numAudEs;j ++)
			{
				if(j >= MAX_ES_NUMBER)
					break;
				LOG_BLINE("AUD ES INFO      %d / %d\n",j+1,m_DtvProgramCfg.astDChannel[i].numAudEs);
				LOG_BLINE("streamtype       0x%x\n",m_DtvProgramCfg.astDChannel[i].rgAudEs[i].stream_type);
				LOG_BLINE("elementary_pid   0x%x\n",m_DtvProgramCfg.astDChannel[i].rgAudEs[i].elementary_pid); 
				LOG_BLINE("ISO 639 code     %c%c%c\n",m_DtvProgramCfg.astDChannel[i].rgAudEs[i].ISO639_language[0],
									m_DtvProgramCfg.astDChannel[i].rgAudEs[i].ISO639_language[1],
									m_DtvProgramCfg.astDChannel[i].rgAudEs[i].ISO639_language[2]);
			}

			for(j = 0;j < m_DtvProgramCfg.astDChannel[i].numSubtitleEs;j ++)
			{
				if(j >= MAX_ES_NUMBER)
					break;
				LOG_BLINE("Subtitle ES      %d / %d\n",j+1,m_DtvProgramCfg.astDChannel[i].numSubtitleEs);
				LOG_BLINE("streamtype       0x%x\n",m_DtvProgramCfg.astDChannel[i].rgSubtitleEs[i].stream_type);
				LOG_BLINE("elementary_pid   0x%x\n",m_DtvProgramCfg.astDChannel[i].rgSubtitleEs[i].elementary_pid);
				LOG_BLINE("ISO 639 code     %c%c%c\n",m_DtvProgramCfg.astDChannel[i].rgSubtitleEs[i].ISO639_language[0],
									m_DtvProgramCfg.astDChannel[i].rgSubtitleEs[i].ISO639_language[1],
									m_DtvProgramCfg.astDChannel[i].rgSubtitleEs[i].ISO639_language[2]);
			}

	    		for(j = 0;j < m_DtvProgramCfg.astDChannel[i].numTeletextEs;j ++)
			{
				if(j >= MAX_ES_NUMBER)
					break;
				LOG_BLINE("Teletext ES      %d / %d\n",j+1,m_DtvProgramCfg.astDChannel[i].numTeletextEs);
				LOG_BLINE("streamtype       0x%x\n",m_DtvProgramCfg.astDChannel[i].rgTeletextEs[i].stream_type);
				LOG_BLINE("elementary_pid   0x%x\n",m_DtvProgramCfg.astDChannel[i].rgTeletextEs[i].elementary_pid);
				LOG_BLINE("ISO 639 code     %c%c%c\n",m_DtvProgramCfg.astDChannel[i].rgTeletextEs[i].ISO639_language[0],
									m_DtvProgramCfg.astDChannel[i].rgTeletextEs[i].ISO639_language[1],
									m_DtvProgramCfg.astDChannel[i].rgTeletextEs[i].ISO639_language[2]);
			}

			LOG_BLINE("acName		%s\n",m_DtvProgramCfg.astDChannel[i].acName);

			for(j = 0;j < MAX_SERVICE_NUMBER; j ++)
			{
				LOG_BLINE("ext_name%d		%s\n",j + 1, m_DtvProgramCfg.astDChannel[i].ext_name[j]);
			}

			LOG_BLINE("audioIdx		%d\n",m_DtvProgramCfg.astDChannel[i].audioIdx);
			LOG_BLINE("videoIdx		%d\n",m_DtvProgramCfg.astDChannel[i].videoIdx);
			LOG_BLINE("teletextIdx		%d\n",m_DtvProgramCfg.astDChannel[i].teletextIdx);
			LOG_BLINE("subtitleIdx		%d\n",m_DtvProgramCfg.astDChannel[i].subtitleIdx);
			LOG_BLINE("pcr_pid		0x%02x\n",m_DtvProgramCfg.astDChannel[i].pcr_pid);
			LOG_BLINE("numProgram		0x%02x\n",m_DtvProgramCfg.astDChannel[i].numProgram);
			LOG_BLINE("program_number		0x%02x\n",m_DtvProgramCfg.astDChannel[i].program_number);
			LOG_BLINE("program_map_PID		0x%02x\n",m_DtvProgramCfg.astDChannel[i].program_map_PID);
			LOG_BLINE("pmt_index		%d\n",m_DtvProgramCfg.astDChannel[i].pmt_index);
			LOG_BLINE("free_CA_mode		%d\n",m_DtvProgramCfg.astDChannel[i].free_CA_mode);
			LOG_BLINE("uiFreq		%d\n",m_DtvProgramCfg.astDChannel[i].uiFreq);
			LOG_BLINE("uiLock		%d\n",m_DtvProgramCfg.astDChannel[i].uiLock);
			LOG_BLINE("uiSkip		%d\n",m_DtvProgramCfg.astDChannel[i].uiSkip);
		}
	}while(FALSE);
*/
}


void CTunerSrv::DumpProgInfo(ProgramInfo* pProg)
{
	UINT16_t i,j;

	do
	{
	    	if(NULL == pProg)
			break;
		
    		LOG_BLINE("lock        %d\n",pProg->lock);
    		LOG_BLINE("skip        %d\n",pProg->skip);

		for(i=0;i<pProg->numVidEs;i++)
		{
			LOG_BLINE("Vid ES INFO      %d / %d\n",i+1,pProg->numVidEs);
			LOG_BLINE("streamtype       0x%x\n",pProg->rgVidEs[i].stream_type);
			LOG_BLINE("elementary_pid   0x%x\n",pProg->rgVidEs[i].elementary_pid);  
			LOG_BLINE("ISO 639 code     %c%c%c\n",pProg->rgVidEs[i].ISO639_language[0],
				pProg->rgVidEs[i].ISO639_language[1],
				pProg->rgVidEs[i].ISO639_language[2]);
		}

    		for(i=0;i<pProg->numAudEs;i++)
		{
			LOG_BLINE("AUD ES INFO      %d / %d\n",i+1,pProg->numAudEs);
			LOG_BLINE("streamtype       0x%x\n",pProg->rgAudEs[i].stream_type);
			LOG_BLINE("elementary_pid   0x%x\n",pProg->rgAudEs[i].elementary_pid); 
			LOG_BLINE("ISO 639 code     %c%c%c\n",pProg->rgAudEs[i].ISO639_language[0],
				pProg->rgAudEs[i].ISO639_language[1],
				pProg->rgAudEs[i].ISO639_language[2]);
		}

		for(i=0;i<pProg->numSubtitleEs;i++)
		{
			LOG_BLINE("Subtitle ES      %d / %d\n",i+1,pProg->numSubtitleEs);
			LOG_BLINE("streamtype       0x%x\n",pProg->rgSubtitleEs[i].stream_type);
			LOG_BLINE("elementary_pid   0x%x\n",pProg->rgSubtitleEs[i].elementary_pid);
			LOG_BLINE("ISO 639 code     %c%c%c\n",pProg->rgSubtitleEs[i].ISO639_language[0],
				pProg->rgSubtitleEs[i].ISO639_language[1],
				pProg->rgSubtitleEs[i].ISO639_language[2]);
		}

    		for(i=0;i<pProg->numTeletextEs;i++)
		{
			LOG_BLINE("Teletext ES      %d / %d\n",i+1,pProg->numTeletextEs);
			LOG_BLINE("streamtype       0x%x\n",pProg->rgTeletextEs[i].stream_type);
			LOG_BLINE("elementary_pid   0x%x\n",pProg->rgTeletextEs[i].elementary_pid);
			LOG_BLINE("ISO 639 code     %c%c%c\n",pProg->rgTeletextEs[i].ISO639_language[0],
				pProg->rgTeletextEs[i].ISO639_language[1],
				pProg->rgTeletextEs[i].ISO639_language[2]);
		}

		LOG_BLINE("program_number               0x%x\n",pProg->program_number);
		LOG_BLINE("short name                   %s\n",  pProg->short_name);
	
		LOG_BLINE("PCR PID          0x%x\n",pProg->pcr_pid);
		LOG_BLINE("program_map_PID  0x%x\n",pProg->program_map_PID);
		
	}while(FALSE);
    
}


INT_t CTunerSrv::FillChannelInfo(tspsimgr_program* pPgmInfo,INT32_t freq,BandwidthChannel typeBW,StandardChannle typeStandard,UINT32_t Index,
	UINT16_t numProgram,UINT32_t Constellation)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	do{
		if(NULL == pPgmInfo || Index >= MAX_DIGITAL_PROGRAM)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
			
		m_DtvProgramCfg.uiBandwidth = typeBW;
		m_DtvProgramCfg.uiStandard = typeStandard;
		m_DtvProgramCfg.uiConstellation = Constellation;
		m_DtvProgramCfg.astDChannel[Index].freq = freq;
		m_DtvProgramCfg.astDChannel[Index].numProgram = numProgram;
		
		m_DtvProgramCfg.astDChannel[Index].free_CA_mode = pPgmInfo->free_CA_mode;
		m_DtvProgramCfg.astDChannel[Index].pcr_pid = pPgmInfo->pcr_pid;

	
		m_DtvProgramCfg.astDChannel[Index].map_pid = pPgmInfo->map_pid;
		m_DtvProgramCfg.astDChannel[Index].program_number = pPgmInfo->program_number;
		m_DtvProgramCfg.astDChannel[Index].programIndex = pPgmInfo->programIndex;
		
		//video
		m_DtvProgramCfg.astDChannel[Index].num_video_pids = pPgmInfo->num_video_pids;
		memcpy(m_DtvProgramCfg.astDChannel[Index].video_pids,pPgmInfo->video_pids,sizeof(pPgmInfo->video_pids));
		
		//audio
		m_DtvProgramCfg.astDChannel[Index].num_audio_pids = pPgmInfo->num_audio_pids;
		memcpy(m_DtvProgramCfg.astDChannel[Index].audio_pids,pPgmInfo->audio_pids,sizeof(pPgmInfo->audio_pids));

		//other
		m_DtvProgramCfg.astDChannel[Index].num_other_pids = pPgmInfo->num_other_pids;
		memcpy(m_DtvProgramCfg.astDChannel[Index].other_pids,pPgmInfo->other_pids,sizeof(pPgmInfo->other_pids));

		ZeroMemory(m_DtvProgramCfg.astDChannel[Index].short_name,sizeof(m_DtvProgramCfg.astDChannel[Index].short_name));
		m_DtvProgramCfg.astDChannel[Index].major_number = pPgmInfo->major_number;
		m_DtvProgramCfg.astDChannel[Index].minor_number = pPgmInfo->minor_number;
		memcpy(m_DtvProgramCfg.astDChannel[Index].short_name,pPgmInfo->short_name,sizeof(pPgmInfo->short_name));
		m_DtvProgramCfg.numDChannel ++;
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnScanProgram()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t bLock  = 0;
	INT32_t freq = 0;
	UINT8 BW = 0;
	UINT32_t Index = 0,j = 0;
	UINT16_t numPrograms;
	UINT16_t scanProgress = 0;
	ChannelProgramInfo chPgInfo[MAX_NUMBER_OF_CHANNELS];
	UINT16_t sumProgram = 0;
	StandardChannle iOutStandard = STANDARD_UNKNOW;
	DvbVctType iTypeOfVCT = TYPEOFDVB_VCT_UNKNOW;
	m_StopTunerFlag=FALSE;
	media_player_t pMediaPlayer;
	TsScan tsinfo;
	UINT32_t programNumber = 1;
	
	{
		CAutoLock AutoRangeLocker(&m_SharedDataLock);
		pMediaPlayer=m_MediaPlayer;
	}

	if(CC_UNLIKELY(m_TunerIface_sp.isNull()))
	{
		iOutRet = ERROR_INVALID_PARAMETER;
	    goto EXIT_PROC;
	}

	ZeroMemory(chPgInfo,sizeof(chPgInfo));

	if(TRUE == m_bNeedSetTunerMode)
	{
		INT32_t iModeType = -1;
		m_bNeedSetTunerMode = FALSE;
		iRet = OnGetTunerMode(&iModeType);
		if((ERROR_SUCCESS == iRet) && 
			(iModeType >= 0 && iModeType <= 2))
		{
			iRet = SetChannelStandMode(&iModeType);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				iOutRet = iRet;
				goto EXIT_PROC;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	{
		INT_t iOldPlayStatus = m_iTuPlayStatus;
		m_iTuPlayStatus = PLAY_STAT_TUNER_SCANNING;
		iRet = m_Mp7xxGlobalStatusData.setMpSrvPlayStatus(m_iTuPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		if(iOldPlayStatus != m_iTuPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}
		
	iRet = m_TunerIface_sp->SetScanParam();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	while(TRUE)
	{

		if(m_StopTunerFlag)
		{
			break;
		}
	
		if(Index >= MAX_NUMBER_OF_CHANNELS)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}		

		iRet = m_TunerIface_sp->SetNextFreqChannel(&bLock,&scanProgress);
		if(ERR_TU_SCAN_DONE == iRet)
		{
			if(NULL != m_pCbScanProgress)
			{
				scanProgress = 100;
				m_pCbScanProgress->TuScanProgressCallback(m_pCbScanProgress->context,&sumProgram,&scanProgress);
			}
			
			if(FALSE == bLock)
			{
				iOutRet = ERROR_SUCCESS;
				break;
			}

		}
		else if(ERR_TU_DEVICE_BUSY == iRet)
		{
			if(Sw_LogTuner)
			{
				LOG("Progress %3d %%\n", scanProgress/10);
			}
			
			scanProgress = scanProgress/10;
			if(NULL != m_pCbScanProgress)
			{
				m_pCbScanProgress->TuScanProgressCallback(m_pCbScanProgress->context,&sumProgram, &scanProgress);
			}
		}
		else if (ERR_TU_SCAN_ERROR == iRet)
		{
			iOutRet = iRet;
			break;
		}

		 
		if(TRUE == bLock)
		{
			numPrograms= 0;
			iRet = m_TunerIface_sp->GetStandard(&iOutStandard);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
				iOutRet = iRet;
				break;
			}
			
			chPgInfo[Index].chStandard = iOutStandard;
			
			if(STANDARD_8VSB == iOutStandard)
			{
				iTypeOfVCT = TYPEOFDVB_VCT_TERRESTRIAL;
				chPgInfo[Index].chConstellation = 0;
			}
			else if(STANDARD_ITU_B == iOutStandard)
			{
				UINT32_t Constellation = 0;
				iRet = m_TunerIface_sp->GetCurrConstellation(&Constellation);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				chPgInfo[Index].chConstellation = Constellation;
				
				iTypeOfVCT = TYPEOFDVB_VCT_CABLE;
			}
			else
			{
				iTypeOfVCT = TYPEOFDVB_VCT_UNKNOW;
			}
			
			iRet = m_TunerIface_sp->GetChannleInfo(&freq,&BW);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
				iOutRet = iRet;
				break;
			}
			
			if(Sw_LogTuner)
			{
				LOG("LockedFreq=%dKHz\n", freq/1000);
			}
			
			chPgInfo[Index].freq = freq;
			chPgInfo[Index].chBandwidth = (typeof(BandwidthChannel))BW;
			iRet = m_TunerIface_sp->GetChannelStrength(&chPgInfo[Index].strength);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			ZeroMemory(&tsinfo,sizeof(tsinfo));
			tsinfo.pid=0x0;
			tsinfo.parserBand=m_parserBand;
			tsinfo.tstype=TS_PAT;
			iRet=tspsimgr_scan_program(&tsinfo);
			if(ERR_Tuner_Not_Found == iRet)
			{
				Index ++;
				bLock = FALSE;
				continue;
			}
			else if( ERROR_SUCCESS == iRet)
			{
				UINT16_t i = 0;
				
				chPgInfo[Index].pRgProgram=tsinfo.pRgProgram;
				numPrograms=chPgInfo[Index].numProgram=chPgInfo[Index].pRgProgram.num_programs;
				if(0 == numPrograms)
				{
					Index ++;
					bLock = FALSE;
					continue;
				}

				for(i = 0; i < numPrograms;i ++)
				{
					chPgInfo[Index].pRgProgram.program_info[i].programIndex = programNumber ++;
				}
			}
			else
			{
				iOutRet = ERR_TU_USER_STOP_SCAN;
				break;
			}
			
			if(Sw_LogTuner)
			{
				LOG("ScanFreqVCTn");
			}

			ZeroMemory(&tsinfo,sizeof(tsinfo));
			tsinfo.pid=0x1FFB;
			tsinfo.parserBand=m_parserBand;
			tsinfo.tstype=TS_VCT;
			iRet=tspsimgr_scan_program(&tsinfo);
			
			if(0 == iRet)
			{
				UINT16_t chIdx = 0;
				UINT16_t pgIdx = 0;
				for(chIdx = 0; chIdx < tsinfo.TmpVctInfo.num_channels_in_section;chIdx ++)
				{	
					for(pgIdx = 0; pgIdx < numPrograms;pgIdx ++)
					{
						if(chPgInfo[Index].pRgProgram.program_info[pgIdx].program_number == tsinfo.TmpVctInfo.serivce_item[chIdx].program_number)
						{
							chPgInfo[Index].pRgProgram.program_info[pgIdx].free_CA_mode = tsinfo.TmpVctInfo.serivce_item[chIdx].access_controlled;
							chPgInfo[Index].pRgProgram.program_info[pgIdx].major_number = tsinfo.TmpVctInfo.serivce_item[chIdx].major_channel_number;
							chPgInfo[Index].pRgProgram.program_info[pgIdx].minor_number = tsinfo.TmpVctInfo.serivce_item[chIdx].minor_channel_number;
							memcpy(chPgInfo[Index].pRgProgram.program_info[pgIdx].short_name,tsinfo.TmpVctInfo.serivce_item[chIdx].short_name,sizeof(chPgInfo[Index].pRgProgram.program_info[pgIdx].short_name));
						}
						
					}
				}
			}
			else if(ERR_Tuner_Not_Found == iRet)
			{
				UINT16 Idx = 0;
				for(Idx = 0;Idx < numPrograms;Idx ++)
				{
					chPgInfo[Index].pRgProgram.program_info[Idx].major_number = m_DefaultMajorNo;
					chPgInfo[Index].pRgProgram.program_info[Idx].minor_number = Idx + 1;
				}
			}
			else //USE DEFAULT  ONLY USE TEST
			{
				iOutRet = ERR_TU_USER_STOP_SCAN;
				break;
			}

			chPgInfo[Index].numProgram = numPrograms;
			sumProgram+=numPrograms;
			Index ++;
			bLock = FALSE;
		
		}
		usleep(100);
	}
	
	if((ERR_TU_SCAN_DONE == iOutRet || ERROR_SUCCESS == iOutRet) && sumProgram > 0)
	{
		UINT32_t Idx = 0;
		UINT16_t numPrograms = 0;
		ZeroMemory(&m_DtvProgramCfg,sizeof(DtvChannelCfg));
		
		for(Index = 0;Index < MAX_NUMBER_OF_CHANNELS; Index ++)
		{
			if(0 == chPgInfo[Index].pRgProgram.num_programs)
			{
				continue;
			}
			
			numPrograms = 0;
			for(j = 0;j < chPgInfo[Index].pRgProgram.num_programs; j ++)
			{
				numPrograms ++;
			}

			for(j = 0;j < chPgInfo[Index].pRgProgram.num_programs; j ++)
			{
				iRet = FillChannelInfo(&chPgInfo[Index].pRgProgram.program_info[j],chPgInfo[Index].freq,
					chPgInfo[Index].chBandwidth,chPgInfo[Index].chStandard,Idx,numPrograms,chPgInfo[Index].chConstellation);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_FILE_LINENO_IRET_CRT_ERR;
				}
				Idx ++;
			}
			
			ZeroMemory(&chPgInfo[Index].pRgProgram.program_info,sizeof(chPgInfo[Index].pRgProgram.program_info));
		}

		iRet = SaveProgramTable();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}

		iRet = RestoreProgramInfo();
		if(ERROR_SUCCESS == iRet)
		{
			m_bDtvChannelLoadSucc = TRUE;
		}
		else
		{
			m_bDtvChannelLoadSucc = FALSE;
		}
	}
	else
	{
		INT_t iOldPlayStatus = m_iTuPlayStatus;
		m_iTuPlayStatus = PLAY_STAT_IDLE;
		iRet = m_Mp7xxGlobalStatusData.setMpSrvPlayStatus(m_iTuPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		if(iOldPlayStatus != m_iTuPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

	iRet = SysPropChannelParaValid();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet)) 
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

EXIT_PROC:
	
	return iOutRet;
}

INT_t CTunerSrv::SysPropChannelParaValid()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strChannelParaChannelParaValid, 32);
	
	do{
		iRet = strChannelParaChannelParaValid.Format("%d", m_bDtvChannelLoadSucc);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = SysProp_setPersistent(SysProp_KEY_TunerScanStatus, (LPCSTR)strChannelParaChannelParaValid);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnEventTuneProgram()
{
/*
	INT_t iOutRet = ERROR_SUCCESS;
	RMstatus eRMstatus = RM_OK;

	do{
		if(PLAY_STAT_PLAYING != m_iTuPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		eRMstatus = Poll_PsfDemuxEvents();
		if(RM_OK != eRMstatus)
		{
			PRINT_BFILE_LINENO_RMSTATUS;
		}

		eRMstatus = StcCompensationRun();
		if(RM_PENDING == eRMstatus)
		{
		}
		else if(RM_OK != eRMstatus)
		{
			PRINT_BFILE_LINENO_RMSTATUS;
		}
	}while(FALSE);

	return iOutRet;
*/

}

INT_t CTunerSrv::OnClosePsfDemux()
{
/*
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	RMstatus eRMstatus = RM_OK;
	
	do{
		{
			INT_t iOldPlayStatus = m_iTuPlayStatus;
			m_iTuPlayStatus = PLAY_STAT_IDLE;
			iRet = m_Mp7xxGlobalStatusData.setMpSrvPlayStatus(m_iTuPlayStatus);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			if(iOldPlayStatus != m_iTuPlayStatus)
			{
				iRet = m_PlayStatChangedEvent.SetEvent();
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		
		eRMstatus = Uninit_PsfDemux();
		if(RM_OK != eRMstatus)
		{
			PRINT_BFILE_LINENO_RMSTATUS;
		}

	}while(FALSE);

	return iOutRet;
*/

}
INT_t CTunerSrv::OnGetChannelNumber(OUT UINT32_t *pMajor,OUT UINT32_t *pMinor)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	do{
		if(NULL == pMajor || NULL == pMinor)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(m_TotalChannel <= 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(m_ChProgInfo[0].numProgram <= 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		(*pMajor) = m_ChProgInfo[0].pRgProgram.program_info[0].major_number;
		(*pMinor) = m_ChProgInfo[0].pRgProgram.program_info[0].minor_number;
		
	}while(FALSE);

	return iOutRet;
}

INT_t CTunerSrv::OnGetStrength(OUT UINT32_t *pStrength)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	UINT32_t OutStrength = 0;
	UINT16_t IStrength = 0;

	do{
		if(NULL == pStrength)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(m_TunerIface_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = m_TunerIface_sp->GetChannelStrength(&IStrength);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		OutStrength |= IStrength;
		if(OutStrength > 0 && OutStrength <= 20)
		{
			OutStrength = 1;
		}
		else if(OutStrength > 20 && OutStrength <= 40)
		{
			OutStrength = 2;
		}
		else if(OutStrength > 40 && OutStrength <= 60)
		{
			OutStrength = 3;
		}
		else if(OutStrength > 60 && OutStrength <80)
		{
			OutStrength = 4;
		}
		else if(OutStrength >= 80)
		{
			OutStrength = 5;
		}
		else 
		{
			OutStrength = 0;
		}
		
		if(NULL != pStrength)
		{
			*pStrength = OutStrength;
		}
	}while(FALSE);

	return iOutRet;
}

