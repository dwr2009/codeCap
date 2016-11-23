#include "MediaSrv.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ArrayOpsMacro.h>
#include <memory.h>
#include <glib/giochannel.h>
#include <TransactionMsgData.h>
#include <TransactionMsgId.h>
#include <AutoScopeLock.h>
#include <Mp7xxGlobalStatus.h>
#include "MediaPlayerAppIf.h"
#include <gst/gstelement.h>
#include <Thread.h>
#include <DateTime.h>
#include "TransactionMsgData.h"
#include "DBusMessage.h"
#include <DBusParameterList.h>
#include "MPGenericMsgId.h"
#include "InvArgException.h"
#include "BaseErrException.h"
#include <SysBaseSrvApi.h>
#include <UrlInfo.h>
#include "SnmpCmdConv.h"
#include "DbgLogSwitchDef.h"

void QMediaSrv::PlayParams_Init(QMediaSrv::P_PLAY_PARAMS pPlayParams)
{
	if(NULL == pPlayParams)
	{
		goto EXIT_PROC;
	}
	ZeroMemory(pPlayParams, sizeof(*pPlayParams));
EXIT_PROC:
	;
}

QMediaSrv::QMediaSrv()
{
	INT_t iRet;
	this->moveToThread(this);	//very important
	
	m_hThisThreadId = 0;
	m_bShouldExitThread = FALSE;
	m_iPlayStatus = PLAY_STAT_IDLE;
	m_threadId = 0;
	m_PlaybackId = 0;	//invalid
	m_bIsCompletePlaybackEos = TRUE;
	m_eVoMode = VO_MODE_HDMI_720p50;
	m_eHdmi3DTvMode = H3DTvMode_Normal;
	m_bVidDispAspectRatioAtNextPlay_Valid = FALSE;	
	m_CurVidDispAspectRatio = DispAspectRatio_DEFAULT;
	m_VidDispAspectRatioAtNextPlay = DispAspectRatio_DEFAULT;

	mibPlaySpeed = 5;
	mibCcValue = 0;

	iRet=setMpSrvPlayStatus(PLAY_STAT_IDLE);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QMediaSrv::~QMediaSrv()
{
}

INT_t QMediaSrv::InitInstance()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CGeneralApp> GeneralApp_sp;
	int argc;
	char ** argv = NULL;
	bool bRet;

	iRet = setEventListener(m_this_wp);	//to CThreadMessageQueue
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	GeneralApp_sp = CGeneralApp::getGeneralApp();
	if(GeneralApp_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	GeneralApp_sp->getCmdLineArgs(OUT argc, OUT argv);

	m_NexusPlayer_sp = SharedPtr <QNexusPlayer> (new QNexusPlayer);
	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	iRet = m_NexusPlayer_sp->setThisWp(m_NexusPlayer_sp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	bRet = connect(this, SIGNAL(sigNewMsgInThreadMsgQueue()), this, SLOT(onNewMsgInThreadMsgQueue()), Qt::AutoConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigNewMsgInThreadMsgQueue) err\n");
	}

	m_CnkB85LcmLedScr_sp = SharedPtr <CCnkB85Lcm> (new CCnkB85Lcm);
	if(m_CnkB85LcmLedScr_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	if(FALSE == m_CnkB85LcmLedScr_sp->ChkInit())
	{
		iOutRet = ERR_INIT_FAIL;
		goto EXIT_PROC;
	}
	iRet = m_CnkB85LcmLedScr_sp->InitLCD();
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iRet = m_CnkB85LcmLedScr_sp->DisplayStr("READY");
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	//Read the previous ethernet IP
	do
	{
		iRet = SysProp_del(SysProp_KEY_PrevEthernetIP);
		if(ERROR_SUCCESS != iRet)
		{
			if(ERROR_NOT_FOUND != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;	
			}
		}
		DECLARE_CLS_STACK_BUF_STRING(strCurEthernetIP, 64);
		iRet = SysProp_get(SysProp_KEY_CurEthernetIP, OUT strCurEthernetIP);
		if(ERROR_SUCCESS != iRet)
		{
			if(ERROR_NOT_FOUND != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;	
			}
			break;
		}
		iRet = m_CnkB85LcmLedScr_sp->DisplayStr((LPCSTR)strCurEthernetIP);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	//callback
	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigOnEndOfStream()), this, SLOT(onPlaybackEos()), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigOnEndOfStream) err\n");
	}
	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigOnVidDecStreamChanged()), this, SLOT(onVidDecStreamChanged()), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigOnVidDecStreamChanged) err\n");
	}

#ifdef ENABLE_DTV	
	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigNotifyTuPlayerPropmtMsg(int)), this, SLOT(onNotifyTuPlayerPropmtMsg(int)), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigNotifyTuPlayerPropmtMsg) err\n");
	}

	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigMPNotifyPlayerScanProgress(int,int)), this, SLOT(onMPNotifyScanProgress(int,int)), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigMPNotifyPlayerScanProgress) err\n");
	}

	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigTuner_StartScan()), this, SLOT(onTuner_StartScan()), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigTuner_StartScan) err\n");
	}

	bRet = connect(&(*m_NexusPlayer_sp), SIGNAL(sigTuner_StopScan()), this, SLOT(onTuner_StopScan()), Qt::DirectConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(NexusPlayer.sigTuner_StopScan) err\n");
	}
#endif	
	
	//Mute audio volume
	//iRet = ChangeVolume(-100, NULL);
	iRet = ChangeVolume(0);//abner
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	#if 1/*modify by xuweiwei 2014-9-25 for init Video display aspect radio*/
	do
	{
		CString strValue;
		SharedPtr <CTinyXmlDoc3> XmlDoc_sp;
		SharedPtr <CGeneralApp> GeneralApp_sp;
		GeneralApp_sp = CGeneralApp::getGeneralApp();
		if(GeneralApp_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		XmlDoc_sp = GeneralApp_sp->getXmlCfgDoc();
		if(XmlDoc_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		{
			CAutoScopeLock autoRangeLocker(&(XmlDoc_sp->m_Mutex));
			iRet = XmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_VideoMode, OUT strValue, "0");
			//LOG_BLINE("strValue:%d\n",strValue.toInt());
			if(ERROR_SUCCESS == iRet)
			{
				m_CurVidDispAspectRatio = (DISPLAY_ASPECT_RATIO)(-1);
				switch( (DISPLAY_ASPECT_RATIO)(atoi(strValue)) )
				{
					case DispAspectRatio_DEFAULT:
					case DispAspectRatio_16_9:
					case DispAspectRatio_4_3:
					case DispAspectRatio_FULL:
						m_VidDispAspectRatioAtNextPlay = (DISPLAY_ASPECT_RATIO)(atoi(strValue));
						break;
					default:
						m_VidDispAspectRatioAtNextPlay = DispAspectRatio_DEFAULT;
						break;
				}
				m_bVidDispAspectRatioAtNextPlay_Valid = TRUE;
				iRet = applyVideoDispAspectRatio(m_VidDispAspectRatioAtNextPlay);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(false);
	#endif		
	

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::ExitInstance()
{
	INT_t iOutRet = ERR_SUCCESS;

	m_CnkB85LcmLedScr_sp.Clear();
	m_NexusPlayer_sp.Clear();

	usleep(500*1000);	//give nxclient more time to cleanup
	
	return iOutRet;
}

/*
* thread context: other theads
*/
void QMediaSrv::onNewMessageInQueue()	//CThreadMessageQueue callback
{
	emit sigNewMsgInThreadMsgQueue();
}

/*
* thread context: local thread
*/
void QMediaSrv::OnNewMessage()	 //indicates there are new messages in the msg queue
{
	int iRet;
	SharedPtr<CMessageQueueItem> TransactionMsg;

	while(TRUE)
	{
		iRet = GetMessageTimeout(TransactionMsg, 0/*only chk msg*/);
		if(CC_LIKELY(ERROR_SUCCESS == iRet))
		{
			if(CC_LIKELY(TransactionMsg.isValid()))
			{
				ProcessMsg(TransactionMsg);
				TransactionMsg.Clear();
			}
		}
		else if(CC_LIKELY(ERR_TIMEOUT == iRet))
		{
			//no msg
			break;
		}
		else if(CC_LIKELY(ERR_LINUX_SIGNAL_INT == iRet))
		{
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}
	}
}

void QMediaSrv::onNewMsgInThreadMsgQueue()
{
	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	OnNewMessage();

EXIT_PROC:
	;
}

//we would like a callback from other threads
void QMediaSrv::onPlaybackEos()
{
	INT_t iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;
	
	if(pthread_self() == m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
        LOG_BLINE("no mem\n");
        goto EXIT_PROC;
	}
	MsgData_sp = SharedPtr <CGenericMsgData>(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
        LOG_BLINE("no mem\n");
        goto EXIT_PROC;
	}
	MsgData_sp->m_uiMsgId = MsgSignal_PlaybackEos;
	{
        CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
        MsgData_sp->m_uiParam = m_PlaybackId;
	}
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
        PRINT_BFILE_LINENO_IRET_STR;
        goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
        PRINT_BFILE_LINENO_IRET_STR;
        goto EXIT_PROC;
	}	

EXIT_PROC:
	;
}

//we would like a callback from other threads
void QMediaSrv::onVidDecStreamChanged()
{
	INT_t iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;
	
	if(pthread_self() == m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	MsgData_sp = SharedPtr <CGenericMsgData>(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	MsgData_sp->m_uiMsgId = MsgSignal_VidDecStreamChanged;
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}	

EXIT_PROC:
	;
}

#ifdef ENABLE_DTV
void QMediaSrv::onNotifyTuPlayerPropmtMsg(int MsgTag)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;
	SharedPtr <CMsgData_PropmtMsg> MsgData_PropmtMsg_sp(new CMsgData_PropmtMsg);

	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}
	
	if(MsgData_PropmtMsg_sp.isNull())
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}

	MsgData_PropmtMsg_sp->m_uiMsgId = MP_MSG_PROPMT_MSG_DONE;
	MsgData_PropmtMsg_sp->m_ProMsgNum = MsgTag;
	MsgData_sp = MsgData_PropmtMsg_sp;
		
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}	

EXIT_PROC:
	;
}

void QMediaSrv::onMPNotifyScanProgress(int PgmNum,int Progress)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;
	SharedPtr <CMsgData_ScanProgress> MsgData_ScanProgress_sp(new CMsgData_ScanProgress);

	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	if(MsgData_ScanProgress_sp.isNull())
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	
	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}

	MsgData_ScanProgress_sp->m_uiMsgId = MP_MSG_SCANPROGRESS;
	MsgData_ScanProgress_sp->m_ProgramNum = PgmNum;
	MsgData_ScanProgress_sp->m_ScanProgress = Progress;
	MsgData_sp = MsgData_ScanProgress_sp;
		
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}	

EXIT_PROC:
	;
}

void QMediaSrv::onTuner_StartScan()
{
	INT_t iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;

	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	MsgData_sp = SharedPtr <CGenericMsgData>(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	MsgData_sp->m_uiMsgId = MP_MSG_STARTSCAN;

	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}	

EXIT_PROC:
	;

}

void QMediaSrv::onTuner_StopScan()
{
	INT_t iRet;
	SharedPtr <CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> MsgData_sp;
	
	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}

	ThreadMsg_sp = SharedPtr<CMessageQueueItem>(new CMessageQueueItem);
	if(ThreadMsg_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	
	MsgData_sp = SharedPtr <CGenericMsgData>(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
		LOG_BLINE("no mem\n");
		goto EXIT_PROC;
	}
	MsgData_sp->m_uiMsgId = MP_MSG_SCANDONE;

	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}	

EXIT_PROC:
	;

}
#endif

VOID QMediaSrv::ProcessMsg(SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iRet;
	SharedPtr<CGenericMsgData> GenMsgData_sp;

	if(pthread_self() != m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		goto EXIT_PROC;
	}


	GenMsgData_sp = TransactionMsg->GetMessageData();

	if(CC_UNLIKELY(GenMsgData_sp.isNull()))
	{
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		goto EXIT_PROC;
	}

	if(MSG_CMD_setDataSource == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setDataSource(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_Prepare == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_Prepare(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_Play == GenMsgData_sp->m_uiMsgId)
	{
		On_Play(TransactionMsg, GenMsgData_sp);
	}
    else if(MSG_CMD_ChangeVidOutMode == GenMsgData_sp->m_uiMsgId)
    {
        On_ChangeVideoOutputMode(TransactionMsg, GenMsgData_sp);
    }
	else if(MSG_CMD_Stop == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_Stop(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setVideoPosition == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setVideoPosition(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MsgSignal_PlaybackEos == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_PlaybackEos(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MsgSignal_VidDecStreamChanged == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_VidDecStreamChanged(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_DispOnLedScr == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_DispOnLedScr(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_scanI2cDev == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_scanI2cDev(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setGpioOutputVal == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setGpioOutputVal(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setPlayMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setPlayMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_getPlayMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getPlayMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setHdmi3DTvMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setHdmi3DTvMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_getMonitorInfo == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getMonitorInfo(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setPlaySpeedCtrl == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet =On_setPlaySpeedCtrl(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setVidDispAspectRatio == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setVidDispAspectRatio(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_getVidDispAspectRatio == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getVidDispAspectRatio(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setOutputSpdifMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setOutputSpdifMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_getOutputSpdifMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getOutputSpdifMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	
#ifdef ENABLE_DTV
	else if(MSG_CMD_PlayTuner == GenMsgData_sp->m_uiMsgId)
	{
		iRet = On_PlayTuner(GenMsgData_sp,TransactionMsg);
		if(ERROR_SUCCESS != iRet)
		{
			iRet = StopTunerPlayer();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
		}
	}
	else if(MSG_CMD_TunerScan == GenMsgData_sp->m_uiMsgId)
	{
		iRet = On_TunerScan(GenMsgData_sp, TransactionMsg);
		if(ERROR_SUCCESS != iRet)
		{
			iRet = StopTunerPlayer();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
		}
	}
	else if(MSG_CMD_GetTunerChannelMode == GenMsgData_sp->m_uiMsgId)
	{
		On_GetTunerChannelMode(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_SetTunerChannelMode == GenMsgData_sp->m_uiMsgId)
	{
		On_SetTunerChannelMode(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_TunerMode == GenMsgData_sp->m_uiMsgId)
	{
		On_SetTunerStandardMode(GenMsgData_sp,TransactionMsg);
	}
	else if(MP_MSG_PROPMT_MSG_DONE == GenMsgData_sp->m_uiMsgId)
	{
		On_NotifyTuPlayerPropmtMsg(GenMsgData_sp,TransactionMsg);
	}
	else if(MP_MSG_SCANPROGRESS == GenMsgData_sp->m_uiMsgId)
	{
		On_NotifyScanProgress(GenMsgData_sp,TransactionMsg);
	}
	else if(MP_MSG_STARTSCAN == GenMsgData_sp->m_uiMsgId)
	{
		On_Tuner_StartScan(GenMsgData_sp,TransactionMsg);
	}
	else if(MP_MSG_SCANDONE == GenMsgData_sp->m_uiMsgId)
	{		
		On_Tuner_StopScan(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_GetFirstChNum == GenMsgData_sp->m_uiMsgId)
	{
		On_GetFirstChNum(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_GetSignalStrength == GenMsgData_sp->m_uiMsgId)
	{
		On_GetSignalStrength(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_ResetDuration == GenMsgData_sp->m_uiMsgId)
	{
		On_ResetDuration(GenMsgData_sp, TransactionMsg);
	}
	else if(MSG_CMD_ChannelUp == GenMsgData_sp->m_uiMsgId)
	{
		On_SetChannelUp(TransactionMsg);
	}
	else if(MSG_CMD_ChannelDown == GenMsgData_sp->m_uiMsgId)
	{
		On_SetChannelDown(TransactionMsg);
	}
	else if(MSG_CMD_GetTunerMode == GenMsgData_sp->m_uiMsgId)
	{
		On_GetTunerStandardMode(GenMsgData_sp,TransactionMsg);
	}
	else if(MSG_CMD_GetEntryChannelInfo == GenMsgData_sp->m_uiMsgId)
	{
		On_GetEntryChannelInfo(GenMsgData_sp,TransactionMsg);
	}
#endif
	else if(MSG_CMD_getDisplayOutputMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getDisplayOutputMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setDisplayOutputMode == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setDisplayOutputMode(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_getvideoColorSpace == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_getvideoColorSpace(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else if(MSG_CMD_setvideoColorSpace == GenMsgData_sp->m_uiMsgId)
	{
		GenMsgData_sp->m_iOutRet = On_setvideoColorSpace(GenMsgData_sp);
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else	
	{
		GenMsgData_sp->m_iOutRet = ERR_INVALID_CMD;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}

	
EXIT_PROC:
	;
}

INT_t QMediaSrv::On_setDataSource(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS;
	SharedPtr <CMsgData_setDataSource> MsgData_setDataSource_sp;

	MsgData_setDataSource_sp = GenMsgData_sp.DynamicCast<CMsgData_setDataSource>();
	if(MsgData_setDataSource_sp.isNull())
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		PRINT_BFILE_LINENO_BUG_STR;
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		if(PLAY_STAT_IDLE != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			goto EXIT_PROC;
		}
	}

	m_sDataSrcUrl = MsgData_setDataSource_sp->m_strDataSrcUrl;
	if(Sw_LogMediaFileInfo)
	{
		LOG_BLINE("url: \"%s\"\n", qPrintable(m_sDataSrcUrl));
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_Prepare(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t iOldPlayStaus;

	Q_UNUSED(GenMsgData_sp);

	//LOG_BLINE("OnPrepare\n");

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		if(PLAY_STAT_IDLE != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			goto EXIT_PROC;
		}
	}	

	//implement the SNMP command "videoMode"
	if(TRUE == m_bVidDispAspectRatioAtNextPlay_Valid)
	{
		DISPLAY_ASPECT_RATIO eVidDispAspectRatioAtNextPlay;
		m_bVidDispAspectRatioAtNextPlay_Valid = FALSE;
		eVidDispAspectRatioAtNextPlay = m_VidDispAspectRatioAtNextPlay;
		iRet = applyVideoDispAspectRatio(eVidDispAspectRatioAtNextPlay);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		//state
		iOldPlayStaus = m_iPlayStatus;
		m_iPlayStatus = PLAY_STAT_PREPARED;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStaus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}		
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_Play(SharedPtr<CMessageQueueItem> & TransactionMsg, SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CMsgData_Play> MsgData_Play_sp = GenMsgData_sp.DynamicCast<CMsgData_Play>();
	

	if(MsgData_Play_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		MsgData_Play_sp->m_iOutRet = iOutRet;
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		INT_t iOldPlayStatus = m_iPlayStatus;
		if(PLAY_STAT_PREPARED != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			MsgData_Play_sp->m_iOutRet = iOutRet;
			goto EXIT_PROC;
		}
		m_iPlayStatus = PLAY_STAT_PLAYING;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStatus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		m_bIsCompletePlaybackEos = TRUE;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
		m_PlaybackId++;
		if(0 == m_PlaybackId)
		{
			m_PlaybackId++;
		}
	}

	if(m_bVidDispAspectRatioAtNextPlay_Valid)
	{			
		DISPLAY_ASPECT_RATIO eVidDispAspectRatioAtNextPlay;
		m_bVidDispAspectRatioAtNextPlay_Valid = FALSE;
		{
			CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
			eVidDispAspectRatioAtNextPlay = m_VidDispAspectRatioAtNextPlay;
		}

		iRet = applyVideoDispAspectRatio(eVidDispAspectRatioAtNextPlay);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}



	//detect 3D
	{	
		
		DECLARE_CLS_STACK_BUF_STRING(strUrl, (4*1024));
		strUrl=m_sDataSrcUrl.toLatin1();
		CUrlInfo FileUrlInfo(strUrl);
		int Stereioscopic3DFmt;
		BOOL_t bIs3DFile = FALSE;
		
		
		if(FileUrlInfo.Filename().Equal("TAB_", 4))
		{
			bIs3DFile = TRUE;
			Stereioscopic3DFmt = 2;	/*NEXUS_VideoOrientation_e3D_OverUnder*/
		}
		else if(FileUrlInfo.Filename().Equal("SBS_", 4))
		{
			bIs3DFile = TRUE;
			Stereioscopic3DFmt = 1; /*NEXUS_VideoOrientation_e3D_LeftRight*/
		}
		
		m_CurRightStereoscopic3D = Stereioscopic3DFmt;
		if(H3DTvMode_Force2D != m_eHdmi3DTvMode)
		{
			if(bIs3DFile)
			{
				iRet = setStereoscopic3DFormat(&Stereioscopic3DFmt);
			}
			else
			{
				//no arguments, will use default 2D.
				iRet = setStereoscopic3DFormat();
			}
			if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}


	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		BOOL_t bLoopMode = FALSE;
		m_iRemaingLoopCnt = MsgData_Play_sp->LoopCount;
		if(1 < m_iRemaingLoopCnt)
		{
			bLoopMode = TRUE;
		}
		iRet = m_NexusPlayer_sp->Play(m_sDataSrcUrl, bLoopMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:

	MsgData_Play_sp->m_iOutRet = iOutRet;
	{		
		CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
		MsgData_Play_sp->m_PlaybackId = m_PlaybackId;
	}
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

INT_t QMediaSrv::On_setShowClosedCaption(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iRet, iOutRet = ERR_SUCCESS;
	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusPlayer_sp->setShowClosedCaption(GenMsgData_sp->m_uiParam);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}


INT_t QMediaSrv::On_setPlaySpeedCtrl(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iRet, iOutRet = ERR_SUCCESS;
	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusPlayer_sp->mibSwitchToFastForward(GenMsgData_sp->m_uiParam);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_setPlayMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	MediaPlayer::PlayMode ePlayMode;

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		if(PLAY_STAT_PLAYING != m_iPlayStatus)
		{
			iOutRet = ERR_INV_STATE;
			goto EXIT_PROC;
		}
	}

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	ePlayMode = (typeof(ePlayMode))(GenMsgData_sp->m_uiParam);
	if(MediaPlayer::PlayMode_Normal == ePlayMode)
	{
		iRet = m_NexusPlayer_sp->SwitchToPlay();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else if(MediaPlayer::PlayMode_Pause == ePlayMode)
	{
		iRet = m_NexusPlayer_sp->SwitchToPause();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else if(MediaPlayer::PlayMode_FastForward == ePlayMode)
	{
		iRet = m_NexusPlayer_sp->SwitchToFastForward();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else if(MediaPlayer::PlayMode_Rewind == ePlayMode)
	{
		iRet = m_NexusPlayer_sp->SwitchToRewind();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else
	{
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_getPlayMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	MediaPlayer::PlayMode ePlayMode = PlayMode_Normal;
	int iPlayStatus;

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		iPlayStatus = m_iPlayStatus;
	}

	if(PLAY_STAT_UNKNOWN == iPlayStatus || PLAY_STAT_IDLE == iPlayStatus || PLAY_STAT_PREPARED == iPlayStatus)
	{
		ePlayMode = PlayMode_Stopped;
	}
	else if(PLAY_STAT_PLAYING == iPlayStatus)
	{
		if(m_NexusPlayer_sp.isNull())
		{
			iOutRet = ERR_INV_STATE;
			goto EXIT_PROC;
		}
		iRet = m_NexusPlayer_sp->getPlayMode(OUT ePlayMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}		
	}
	GenMsgData_sp->m_uiParam = ePlayMode;
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_setDisplayOutputMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT_t displayMode;

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	displayMode = (GenMsgData_sp->m_uiParam);

	iRet = m_NexusPlayer_sp->setDisplayOutputMode(displayMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_getDisplayOutputMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT_t displayMode;

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusPlayer_sp->getDisplayOutputMode(OUT displayMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}		

	GenMsgData_sp->m_uiParam = displayMode;
	
EXIT_PROC:
	
	return iOutRet;
}



INT_t QMediaSrv::On_setvideoColorSpace(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT_t icolorspace;
	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	icolorspace = (GenMsgData_sp->m_uiParam);

	iRet = m_NexusPlayer_sp->setvideoColorSpace(icolorspace);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_getvideoColorSpace(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	INT_t icolorspace;

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_NexusPlayer_sp->getvideoColorSpace(OUT icolorspace);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}		

	GenMsgData_sp->m_uiParam = icolorspace;
	
EXIT_PROC:
	
	return iOutRet;
}




/*
* local thread
*/
INT_t QMediaSrv::On_ChangeVideoOutputMode(
	SharedPtr<CMessageQueueItem> & TransactionMsg, SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
    INT_t iOutRet = ERR_SUCCESS, iRet;
    SharedPtr <CMsgData_ChangeVidOutMode> MsgData_ChangeVidOutMode_sp = GenMsgData_sp.DynamicCast<CMsgData_ChangeVidOutMode>();

    if(MsgData_ChangeVidOutMode_sp.isNull()){
        PRINT_BFILE_LINENO_BUG_STR;
        iOutRet = ERROR_INVALID_STATE;
        goto EXIT_PROC;
    }

	if( MsgData_ChangeVidOutMode_sp->m_eVoModeToSet == VO_MODE_NotSet)
	{
		MsgData_ChangeVidOutMode_sp->m_eNewVoMode = this->m_eVoMode;
		MsgData_ChangeVidOutMode_sp->m_eOldVoMode = this->m_eVoMode;
		goto EXIT_PROC;
	}

    if(m_NexusPlayer_sp.isNull()){
        PRINT_BFILE_LINENO_BUG_STR;
        iOutRet = ERROR_INVALID_STATE;
        goto EXIT_PROC;
    }
	printf("[%s:%d] m_eVoModeToSet = %d\n",__FILE__,__LINE__,MsgData_ChangeVidOutMode_sp->m_eVoModeToSet);
    iRet = m_NexusPlayer_sp->ChangeVideoOutputMode(MsgData_ChangeVidOutMode_sp->m_eVoModeToSet, 
		&MsgData_ChangeVidOutMode_sp->m_eNewVoMode, &MsgData_ChangeVidOutMode_sp->m_eOldVoMode);
    if(ERR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }
    m_eVoMode = MsgData_ChangeVidOutMode_sp->m_eVoModeToSet;
	
	if(MsgData_ChangeVidOutMode_sp->m_eNewVoMode != MsgData_ChangeVidOutMode_sp->m_eOldVoMode)
	{		
		emit sigVidOutModeChangedEvent(MsgData_ChangeVidOutMode_sp->m_eNewVoMode);
	}

EXIT_PROC:

    MsgData_ChangeVidOutMode_sp->m_iOutRet = iOutRet;
    iRet = TransactionMsg->SetEvent();
    if(CC_UNLIKELY(ERROR_SUCCESS != iRet)) {
        PRINT_BFILE_LINENO_IRET_CRT_ERR;
    }

    return iOutRet;
}

INT_t QMediaSrv::setStereoscopic3DFormat(int* pStereoscopic3D)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setStereoscopic3DFormat(pStereoscopic3D);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;

}

INT_t QMediaSrv::getDisplayParam(const int iDispParam,int &oValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getDisplayParam(iDispParam,oValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t QMediaSrv::setDisplayParam(int iDispParam,int iValue)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setDisplayParam(iDispParam,iValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	
	return iOutRet;
}


INT_t QMediaSrv::getOutputSpdifMode(int &oSpdifMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT iSpdifMode;
	iRet = sendSimpleMessage(MSG_CMD_getOutputSpdifMode, 0,&iSpdifMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	oSpdifMode=iSpdifMode;

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::setOutputSpdifMode(int iSpdifMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	iRet = sendSimpleMessage(MSG_CMD_setOutputSpdifMode, iSpdifMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::setHdmiAudioOutput(int bHdmiAudioOutput)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setHdmiAudioOutput(bHdmiAudioOutput);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
	
}

INT_t QMediaSrv::setAnalogAudioMute(int bAnalogAudioMute)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setAnalogAudioMute(bAnalogAudioMute);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec, int izone)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setHwRtcTime(iyear,imonth,imday,ihour,imin,isec,izone);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:

	return iOutRet;
	
}

INT_t QMediaSrv::getHwRtcTime(QString & ConfigDateTime)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
	CString strConfigDateTime;

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getHwRtcTime(strConfigDateTime);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	ConfigDateTime=strConfigDateTime;
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv:: getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getDisplayPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv:: getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getVideoPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv:: setOsdPosition(int x,int y,int width,int height,int type)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setOsdPosition(x,y,width,height,type);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setOsdSize(int w, int h)
{
    INT_t iOutRet = ERROR_SUCCESS,iRet;
    CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

    if(m_NexusPlayer_sp.isNull())
    {
        PRINT_BFILE_LINENO_BUG_STR;
        iOutRet = ERROR_INVALID_STATE;
        goto EXIT_PROC;
    }

    iRet = m_NexusPlayer_sp->setOsdSize(w,h);
    if(ERR_SUCCESS != iRet)
    {
        iOutRet = iRet;
        goto EXIT_PROC;
    }
    
EXIT_PROC:
    return iOutRet;
}

INT_t QMediaSrv::getCurrentTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getCurrentTime(uTimestamp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::getLastTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getLastTime(uTimestamp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setSyncTime(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setSyncTime(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setSyncSeek(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setSyncSeek(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::Seek(const unsigned int & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);

	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->Seek(Offset);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setHdmiCecCtrl(CONST HDMI_CEC_CTRL_CMD eHdmiCecCmd)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
	
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->setHdmiCecCtrl(eHdmiCecCmd);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
			
EXIT_PROC:
	return iOutRet;

}

#ifdef ENABLE_DTV
INT_t QMediaSrv::PlayTuner(quint32 * pPlaybackId)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_Play> MsgTuner_Play_sp(new CMsgData_Play);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(NULL == MsgTuner_Play_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(NULL == ThreadMsg_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		MsgTuner_Play_sp->m_uiMsgId = MSG_CMD_PlayTuner;
		MsgData_sp = MsgTuner_Play_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(NULL != pPlaybackId)
		{
			*pPlaybackId = MsgTuner_Play_sp->m_PlaybackId;
		}
		iOutRet = MsgTuner_Play_sp->m_iOutRet;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaSrv::TunerScan()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgDataTunerScan_sp(new CGenericMsgData);

	do{
		if(NULL == MsgDataTunerScan_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(NULL == ThreadMsg_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		MsgDataTunerScan_sp->m_uiMsgId = MSG_CMD_TunerScan;
		iRet = ThreadMsg_sp->SetMessageData(MsgDataTunerScan_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iOutRet = MsgDataTunerScan_sp->m_iOutRet;
		
	}while(FALSE);
	
	return iOutRet;

}

INT_t QMediaSrv::getTunerChannelMode(quint32 * TunerChannleMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_GetTuChMode> MsgGetChannelMode_sp (new CMsgData_GetTuChMode);
	SharedPtr <CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;
	
	do{
		if(MsgGetChannelMode_sp.isNull() || ThreadMsg_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(NULL == TunerChannleMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		MsgGetChannelMode_sp->m_uiMsgId = MSG_CMD_GetTunerChannelMode;
		MsgData_sp = MsgGetChannelMode_sp;

		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
				
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(NULL != TunerChannleMode)
		{
			*TunerChannleMode = MsgGetChannelMode_sp->m_OChannelMode;
		}
		
		iOutRet = MsgGetChannelMode_sp->m_iOutRet;		
	}while(FALSE);
	
	return iOutRet;
}

INT_t QMediaSrv::setTunerChannelMode(qint32 TunerChannleMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_SetTuChMode> MsgSetChannelMode_sp(new CMsgData_SetTuChMode);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do
	{
		if(MsgSetChannelMode_sp.isNull() || ThreadMsg_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		MsgSetChannelMode_sp->m_uiMsgId = MSG_CMD_SetTunerChannelMode;
		MsgSetChannelMode_sp->m_iChannelMode = TunerChannleMode;
		MsgData_sp = MsgSetChannelMode_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t QMediaSrv::TunerMode(qint32 iStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_SetTuMode> MsgSetTuStandardMode_sp(new CMsgData_SetTuMode);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(MsgSetTuStandardMode_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(ThreadMsg_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		MsgSetTuStandardMode_sp->m_uiMsgId = MSG_CMD_TunerMode;
		MsgSetTuStandardMode_sp->m_iStdMode = iStdMode;
		MsgData_sp = MsgSetTuStandardMode_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaSrv::GetFirstChNumber(quint32* OMajor,quint32* OMinor)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_GetFirstChNum> MsgGetChannelNumber_sp(new CMsgData_GetFirstChNum);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(MsgGetChannelNumber_sp.isNull() || ThreadMsg_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		MsgGetChannelNumber_sp->m_uiMsgId = MSG_CMD_GetFirstChNum;
		MsgData_sp = MsgGetChannelNumber_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(NULL != OMajor && NULL != OMinor)
		{
			(*OMajor) = MsgGetChannelNumber_sp->m_major;
			(*OMinor) = MsgGetChannelNumber_sp->m_minor;
		}
		
		iOutRet = MsgGetChannelNumber_sp->m_iOutRet;
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaSrv::GetTunerChannelStrength(quint32* Strength)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_GetStrength> MsgGetChannelStrength_sp(new CMsgData_GetStrength);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(MsgGetChannelStrength_sp.isNull() || ThreadMsg_sp.isNull() || NULL == Strength)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		MsgGetChannelStrength_sp->m_uiMsgId = MSG_CMD_GetSignalStrength;
		MsgData_sp = MsgGetChannelStrength_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(NULL != Strength)
		{
			*Strength = MsgGetChannelStrength_sp->m_channelStrength;
		}
		
		iOutRet = MsgGetChannelStrength_sp->m_iOutRet;
	}while(FALSE);

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::ResetDuration()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	iRet = sendSimpleMessage(MSG_CMD_ResetDuration);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::ChannelUp()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	iRet = sendSimpleMessage(MSG_CMD_ChannelUp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::ChannelDown()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	iRet = sendSimpleMessage(MSG_CMD_ChannelDown);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::GetTunerMode(qint32* iStdMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_GetTunerMode> MsgGetChanStdMode_sp(new CMsgData_GetTunerMode);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(MsgGetChanStdMode_sp.isNull() || ThreadMsg_sp.isNull() || NULL == iStdMode)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		MsgGetChanStdMode_sp->m_uiMsgId = MSG_CMD_GetTunerMode;
		MsgData_sp = MsgGetChanStdMode_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(NULL != iStdMode)
		{
			*iStdMode = MsgGetChanStdMode_sp->m_channelStdMode;
		}
		
		iOutRet = MsgGetChanStdMode_sp->m_iOutRet;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaSrv::GetEntryChannelInfo(IN INT32_t EntryId ,OUT INT32_t *pChannelNumber,OUT INT32_t *pSubChannelCount,OUT CStackBufString &StrProgramPids)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CMsgData_GetEntryInfo> MsgData_EntryInfo_sp( new CMsgData_GetEntryInfo);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	do{
		if(NULL == MsgData_EntryInfo_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(NULL == ThreadMsg_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		MsgData_EntryInfo_sp->m_EntryId = EntryId;
		MsgData_EntryInfo_sp->m_uiMsgId = MSG_CMD_GetEntryChannelInfo;
		MsgData_sp = MsgData_EntryInfo_sp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(NULL != pChannelNumber && NULL != pSubChannelCount)
		{
			*pChannelNumber = MsgData_EntryInfo_sp->m_ChannelNum;
			*pSubChannelCount = MsgData_EntryInfo_sp->m_SubChannlNum;

			iRet = StrProgramPids.Format("%s",MsgData_EntryInfo_sp->m_StrProgramPids);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
			iOutRet = MsgData_EntryInfo_sp->m_iOutRet;
		}
	}while(FALSE);

	return iOutRet;
}
#endif

INT_t QMediaSrv::sendSimpleMessage(CONST UINT_t msgId, CONST UINT_t uParam/* = 0*/, OUT UINT_t * puParam/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	if(pthread_self() == m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		iOutRet = ERR_INCORRECT_THREAD_CONTEXT;
		goto EXIT_PROC;
	}

	{
		SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
		SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
		
		if(ThreadMsg_sp.isNull() || MsgData_sp.isNull())
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			goto EXIT_PROC;
		}
		
		MsgData_sp->m_uiMsgId = msgId;
		MsgData_sp->m_uiParam = uParam;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		iOutRet = MsgData_sp->m_iOutRet;
		if(puParam)
		{
			*puParam = MsgData_sp->m_uiParam;
		}
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::sendComplexMessage(CONST UINT_t msgId, CONST UINT_t uParam/* = 0*/, 
	IN OUT WeakPtr<CObject> ObjParam_wp/* = WeakPtr<CObject>()*/, OUT UINT_t * puParam/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	if(pthread_self() == m_threadId)
	{
		LOG_BLINE("BUG,inv thread ctx(caller:0x%08lx,handler:0x%08lx).\n", pthread_self(), m_threadId);
		iOutRet = ERR_INCORRECT_THREAD_CONTEXT;
		goto EXIT_PROC;
	}

	{
		SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
		SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
		
		if(ThreadMsg_sp.isNull() || MsgData_sp.isNull())
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			goto EXIT_PROC;
		}
		
		MsgData_sp->m_uiMsgId = msgId;
		MsgData_sp->m_uiParam = uParam;
		MsgData_sp->m_objParam_wp = ObjParam_wp;
		iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		iRet = SendMessage(ThreadMsg_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		iOutRet = MsgData_sp->m_iOutRet;
		if(puParam)
		{
			*puParam = MsgData_sp->m_uiParam;
		}
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_scanI2cDev(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	Q_UNUSED(GenMsgData_sp);

	if(m_NexusPlayer_sp.isNull()){
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->scanI2cDev();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_setGpioOutputVal(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int iGpioNum;
	bool bVal;

	if(m_NexusPlayer_sp.isNull()){
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iGpioNum = (GenMsgData_sp->m_uiParam & 0x7FFF);
	if(GenMsgData_sp->m_uiParam & 0x8000)
	{
		bVal = true;
	}
	else
	{
		bVal = false;
	}
	iRet = m_NexusPlayer_sp->setGpioOutputVal(iGpioNum, bVal);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_setHdmi3DTvMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	HDMI_3D_TV_MODE eHdmi3DTvMode;

	eHdmi3DTvMode = (typeof(eHdmi3DTvMode))(GenMsgData_sp->m_uiParam);
	if(FALSE == (H3DTvMode_MinValue <= eHdmi3DTvMode && H3DTvMode_MaxValue >= eHdmi3DTvMode))
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	if(VO_MODE_SPLIT_MODE_MIN_BOUNDARY < m_eVoMode && VO_MODE_SPLIT_MODE_MAX_BOUNDARY > m_eVoMode)	//split HDMI+VGA
	{
		//need not switch to 2D
		goto EXIT_PROC;
	}
	
	m_eHdmi3DTvMode = eHdmi3DTvMode;

	if(H3DTvMode_Normal == m_eHdmi3DTvMode)
	{
		iRet = setStereoscopic3DFormat(&m_CurRightStereoscopic3D);
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	else if(H3DTvMode_Force2D == m_eHdmi3DTvMode)
	{
		//no arguments, will use default 2D.
		iRet = setStereoscopic3DFormat();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
EXIT_PROC:

	return iOutRet;
}

/*
* return value: MonInfoParam_sp->m_sMonitorInfo
*/
INT_t QMediaSrv::On_getMonitorInfo(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CObject> ObjParam_sp;
	SharedPtr <CMonitorInfoParam> MonInfoParam_sp;

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	ObjParam_sp = GenMsgData_sp->m_objParam_wp.Promote();
	if(ObjParam_sp.isNull())
	{
		goto EXIT_PROC;
	}
	MonInfoParam_sp = ObjParam_sp.DynamicCast<CMonitorInfoParam>();
	if(MonInfoParam_sp.isNull())
	{
		goto EXIT_PROC;
	}

#if 1 /*Add by xuweiwei 2014-9-16 for get monitor info*/
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->getMonitorInfo(MonInfoParam_sp->m_sMonitorInfo);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
#endif
		
EXIT_PROC:

	return iOutRet;
}

/*
* local thread
*/
INT_t QMediaSrv::On_setVidDispAspectRatio(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CObject> ObjParam_sp;
	SharedPtr <CVidDispAspectRatioParam> VidDispAspectRatioParam_sp;
	DISPLAY_ASPECT_RATIO eVideoDispAspectRato;

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	ObjParam_sp = GenMsgData_sp->m_objParam_wp.Promote();
	if(ObjParam_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		goto EXIT_PROC;
	}
	VidDispAspectRatioParam_sp = ObjParam_sp.DynamicCast<CVidDispAspectRatioParam>();
	if(VidDispAspectRatioParam_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		goto EXIT_PROC;
	}

	eVideoDispAspectRato = (typeof(eVideoDispAspectRato))(VidDispAspectRatioParam_sp->m_iVidDispAspectRatio);
	if(DAR_ONLY_APPLY_ONCE_AT_NEXT_PLAY & VidDispAspectRatioParam_sp->m_uOptFlags)
	{
#if 1/*added by xuweiwei 2014-4-2*/
		if( m_VidDispAspectRatioAtNextPlay != eVideoDispAspectRato)
		{
			SharedPtr <CTinyXmlDoc3> XmlDoc_sp;
			SharedPtr <CGeneralApp> GeneralApp_sp;
			bool bCreateNodeIfNotExist = TRUE;
			GeneralApp_sp = CGeneralApp::getGeneralApp();;
			if(GeneralApp_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				goto SAVE_CFG_FINISH;
			}
			XmlDoc_sp = GeneralApp_sp->getXmlCfgDoc();
			if(XmlDoc_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				goto SAVE_CFG_FINISH;
			}
			{
				CAutoScopeLock autoRangeLocker(&(XmlDoc_sp->m_Mutex));
				iOutRet = XmlDoc_sp->setElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_VideoMode, 
					eVideoDispAspectRato, bCreateNodeIfNotExist);
				if(ERROR_SUCCESS != iOutRet)
				{
					LOG_BLINE("err %d\n", iOutRet);
					goto SAVE_CFG_FINISH;
				}
				bool bRet = XmlDoc_sp->SaveFile(MP_SETTING_XML_DOC_PATH);
				if(false == bRet)
				{
					LOG_BLINE("err\n");
					goto SAVE_CFG_FINISH;
				}
			}
			sync();
SAVE_CFG_FINISH:
			;
		}
#endif
		m_VidDispAspectRatioAtNextPlay = eVideoDispAspectRato;
		m_bVidDispAspectRatioAtNextPlay_Valid = TRUE;		
	}
	else	//inv param
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

#if	0
	/* for dbg */
	//implement the SNMP command "videoMode"
	if(TRUE == m_bVidDispAspectRatioAtNextPlay_Valid)
	{
		DISPLAY_ASPECT_RATIO eVidDispAspectRatioAtNextPlay;
		m_bVidDispAspectRatioAtNextPlay_Valid = FALSE;
		eVidDispAspectRatioAtNextPlay = m_VidDispAspectRatioAtNextPlay;
		iRet = applyVideoDispAspectRatio(eVidDispAspectRatioAtNextPlay);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
#endif	
		
EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_getVidDispAspectRatio(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	GenMsgData_sp->m_uiParam = m_CurVidDispAspectRatio;

EXIT_PROC:

	return iOutRet;	
}

INT_t QMediaSrv::On_getOutputSpdifMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strOutputSpdifMode, 64);
	CStackBufString::SCANF_PARAM oScanfParam;
	CStackBufString::ScanfParam_Init(&oScanfParam);
	int pOutSpdifMode;
		
	iRet = SysProp_get(SysProp_KEY_OutputSpdifMode, OUT strOutputSpdifMode);
	if(ERROR_SUCCESS == iRet)
	{
		iRet = strOutputSpdifMode.Scanf(IN OUT oScanfParam, "%d", &pOutSpdifMode);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else
	{
		pOutSpdifMode = 2;
	}

	if(Mp7xxCommon::SnmpOutSpdifMode_Disabled > pOutSpdifMode || pOutSpdifMode > Mp7xxCommon::SnmpOutSpdifMode_Uncompressed)
	{
		iOutRet = ERR_OUT_OF_RANGE;
		goto EXIT_PROC;
	}

	GenMsgData_sp->m_uiParam=pOutSpdifMode;

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_setOutputSpdifMode(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT32_t iSnmpOutSpdifMode = Mp7xxCommon::SnmpOutSpdifMode_Uncompressed;
	DECLARE_CLS_STACK_BUF_STRING(strOutputSpdifMode, 64);

	if(m_NexusPlayer_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iSnmpOutSpdifMode = GenMsgData_sp->m_uiParam;

	if(Mp7xxCommon::SnmpOutSpdifMode_Disabled > iSnmpOutSpdifMode || iSnmpOutSpdifMode > Mp7xxCommon::SnmpOutSpdifMode_Uncompressed)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	//SAVE
	iRet = strOutputSpdifMode.Format("%d", iSnmpOutSpdifMode);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	iRet = SysProp_setPersistent(SysProp_KEY_OutputSpdifMode, (LPCSTR)strOutputSpdifMode);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = setMruaOutputSpdifMode(iSnmpOutSpdifMode);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}				

EXIT_PROC:

return iOutRet;

}


INT_t QMediaSrv::applyVideoDispAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio, IN BOOL_t bForce/* = FALSE*/)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	/*fixed when QMediaSrv is init but not run so currentThreadId() is not equal m_hThisThreadId*/
	static bool initRun=true;
	if((CC_UNLIKELY(currentThreadId() != m_hThisThreadId)) && !initRun)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}
	
	initRun=false;
	
	if((FALSE == bForce) && (m_CurVidDispAspectRatio == eDispAspectRatio))	//no force, same
	{
		goto EXIT_PROC;	//not need set
	}
	
	if(m_NexusPlayer_sp.isValid())
	{
		//LOG_BLINE("MediaPlayer_setVidWinAspectRatio\n");
		iRet = m_NexusPlayer_sp->setVidWinAspectRatio(eDispAspectRatio);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

#if 1 /*Add by xuweiwei 2014-4-2*/
    if( m_CurVidDispAspectRatio != eDispAspectRatio )
    {
        m_CurVidDispAspectRatio = eDispAspectRatio;
    }
#endif 
	
EXIT_PROC:

	return iOutRet; 
}


INT_t QMediaSrv::changeHdmiDisplaySettings(int iDispParam,int iValue)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	static bool initRun=true;
	if((CC_UNLIKELY(currentThreadId() != m_hThisThreadId)) && !initRun)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}

	initRun=false;
	
	if(m_NexusPlayer_sp.isValid())
	{
		iRet = m_NexusPlayer_sp->changeHdmiDisplaySettings(iDispParam,iValue);
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
	}	

EXIT_PROC:
	return iOutRet; 
}


INT_t QMediaSrv::setMruaOutputSpdifMode(INT_t iSnmpOutSpdifMode, BOOL_t bApply/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	static bool initRun=true;
	if((CC_UNLIKELY(currentThreadId() != m_hThisThreadId)) && !initRun)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}
	
	initRun=false;
	
	if(bApply)
	{
		if(m_NexusPlayer_sp.isValid())
		{
			iRet = m_NexusPlayer_sp->setOutputSpdifMode(iSnmpOutSpdifMode);
			if(ERR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				goto EXIT_PROC;
			}
		}
	}
	
EXIT_PROC:

	return iOutRet; 	
}


#ifdef ENABLE_DTV
INT_t QMediaSrv::On_PlayTuner(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg)
{

	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CMsgData_Play> MsgData_PlayTuner_sp = GenMsgData_sp.DynamicCast<CMsgData_Play>();
	INT32_t iChannelMode  = 0;
	
	if(MsgData_PlayTuner_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		INT_t iOldPlayStatus = m_iPlayStatus;
		if(PLAY_STAT_PREPARED != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			MsgData_PlayTuner_sp->m_iOutRet = iOutRet;
			iRet = TransactionMsg->SetEvent();
			if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
			goto EXIT_PROC;
		}
		
		m_iPlayStatus = PLAY_STAT_PLAYING;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		if(iOldPlayStatus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		m_bIsCompletePlaybackEos = TRUE;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
		MsgData_PlayTuner_sp->m_iOutRet = ERROR_SUCCESS;
		m_PlaybackId++;
		if(0 == m_PlaybackId)
		{
			m_PlaybackId++;
		}
		MsgData_PlayTuner_sp->m_PlaybackId = m_PlaybackId;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}

	//Play Tuner Default Use 2D mode
	iRet = setStereoscopic3DFormat();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		iRet = m_NexusPlayer_sp->PlayTuner(m_sDataSrcUrl);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	
	return iOutRet;
}

INT_t QMediaSrv::On_TunerScan(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CGenericMsgData> MsgData_TunerScan_sp(new CGenericMsgData);
	
	if(MsgData_TunerScan_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
		MsgData_TunerScan_sp->m_iOutRet = ERROR_SUCCESS;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		iRet = m_NexusPlayer_sp->TunerScan();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
		
EXIT_PROC:		

	return iOutRet;
}

INT_t QMediaSrv::On_GetTunerChannelMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_GetTuChMode> MsgData_GetTuChMode_sp = GenMsgData_sp.DynamicCast<CMsgData_GetTuChMode>();
	INT32_t iTuChMode = 0;
	
	if(MsgData_GetTuChMode_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		iRet = m_NexusPlayer_sp->GetTunerChannelMode(&iTuChMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:		
	
	MsgData_GetTuChMode_sp->m_iOutRet = iOutRet;
	MsgData_GetTuChMode_sp->m_OChannelMode = iTuChMode;
	
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	return iOutRet;

}

INT_t QMediaSrv::On_SetTunerChannelMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_SetTuChMode> MsgData_SetTuChMode_sp = GenMsgData_sp.DynamicCast<CMsgData_SetTuChMode>();
	INT32_t iTuChMode = 0;

	if(MsgData_SetTuChMode_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iTuChMode = MsgData_SetTuChMode_sp->m_iChannelMode;
	
	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		iRet = m_NexusPlayer_sp->SetTunerChannelMode(iTuChMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:
	
	MsgData_SetTuChMode_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;

}

INT_t QMediaSrv::On_SetTunerStandardMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_SetTuMode> MsgData_SetTuMode_sp = GenMsgData_sp.DynamicCast<CMsgData_SetTuMode>();
	INT32_t iTuStdMode = -1;
	
	if(MsgData_SetTuMode_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iTuStdMode = MsgData_SetTuMode_sp->m_iStdMode;

	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->SetTunerStandardMode(iTuStdMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:

	MsgData_SetTuMode_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	return iOutRet;
	
}

INT_t QMediaSrv::On_NotifyTuPlayerPropmtMsg(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_PropmtMsg> MsgData_TuScanMode_sp = GenMsgData_sp.DynamicCast<CMsgData_PropmtMsg>();
	INT32_t InputPara  = 0;
	
	if(MsgData_TuScanMode_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	InputPara = MsgData_TuScanMode_sp->m_ProMsgNum;
	emit sigNotifyTunerPlayerPropmtMsg(InputPara);	

EXIT_PROC:

	MsgData_TuScanMode_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
	
}

INT_t QMediaSrv::On_NotifyScanProgress(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_ScanProgress> MsgData_ScanProgress_sp = GenMsgData_sp.DynamicCast<CMsgData_ScanProgress>();
	INT32_t iProgramNum  = 0;
	INT32_t ScanProgress=0;
	
	if(MsgData_ScanProgress_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iProgramNum = MsgData_ScanProgress_sp->m_ProgramNum;
	ScanProgress= MsgData_ScanProgress_sp->m_ScanProgress;
	
	emit sigNotifyScanProgress(iProgramNum,ScanProgress);	
	

EXIT_PROC:
	
	MsgData_ScanProgress_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

INT_t QMediaSrv::On_Tuner_StartScan(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
	
	if(MsgData_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}
	
	emit sigTuner_StartScan();	

EXIT_PROC:
	
	MsgData_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

INT_t QMediaSrv::On_Tuner_StopScan(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
	
	if(MsgData_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}
	
	emit sigTuner_StopScan();	

EXIT_PROC:
	
	MsgData_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

INT_t QMediaSrv::On_GetFirstChNum(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_GetFirstChNum> MsgData_GetFirstChNumber_sp = GenMsgData_sp.DynamicCast<CMsgData_GetFirstChNum>();
	UINT32_t iMajor = 0,iMinor = 0;
	
	if(MsgData_GetFirstChNumber_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		MsgData_GetFirstChNumber_sp->m_iOutRet = iOutRet;
		goto EXIT_PROC;
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->GetFirstChNum(&iMajor,&iMinor);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	MsgData_GetFirstChNumber_sp->m_major = iMajor;
	MsgData_GetFirstChNumber_sp->m_minor = iMinor;

EXIT_PROC:

	MsgData_GetFirstChNumber_sp->m_iOutRet = iOutRet;

	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

INT_t QMediaSrv::On_GetSignalStrength(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_GetStrength> MsgData_GetStrength_sp = GenMsgData_sp.DynamicCast<CMsgData_GetStrength>();
	UINT32_t iStrength = 0;
	
	if(MsgData_GetStrength_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		MsgData_GetStrength_sp->m_iOutRet = iOutRet;
		goto EXIT_PROC;
	}
	
	{
		CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
		if(PLAY_STAT_PLAYING != m_iPlayStatus)
		{
			MsgData_GetStrength_sp->m_channelStrength = iStrength;
			MsgData_GetStrength_sp->m_iOutRet = iOutRet;
			goto EXIT_PROC;
		}
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->GetSignalStrength(&iStrength);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	MsgData_GetStrength_sp->m_channelStrength = iStrength;

EXIT_PROC:
	
	MsgData_GetStrength_sp->m_iOutRet = iOutRet;

	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	return iOutRet;
}

INT_t QMediaSrv::On_ResetDuration(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;

	iOutRet = ERROR_SUCCESS;
	GenMsgData_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}  
	
	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->ResetDuration();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	} 

EXIT_PROC:
	  
	return iOutRet;
}

INT_t QMediaSrv::On_SetChannelUp(SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr<CGenericMsgData> GenMsgData_sp;

	GenMsgData_sp = TransactionMsg->GetMessageData();
	if(GenMsgData_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
		if(PLAY_STAT_PLAYING != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			GenMsgData_sp->m_iOutRet = iOutRet;
			iRet = TransactionMsg->SetEvent();
			if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
			goto EXIT_PROC;
		}
	}

	iOutRet = ERROR_SUCCESS;
	GenMsgData_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->SetChannelUp();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	} 

EXIT_PROC:
	  
	return iOutRet;
}

INT_t QMediaSrv::On_SetChannelDown(SharedPtr<CMessageQueueItem> & TransactionMsg)
{
   	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr<CGenericMsgData> GenMsgData_sp;

	GenMsgData_sp = TransactionMsg->GetMessageData();
	if(GenMsgData_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		iRet = TransactionMsg->SetEvent();
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
		if(PLAY_STAT_PLAYING != m_iPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			GenMsgData_sp->m_iOutRet = iOutRet;
			iRet = TransactionMsg->SetEvent();
			if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
			goto EXIT_PROC;
		}
	}

	iOutRet = ERROR_SUCCESS;
	GenMsgData_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->SetChannelDown();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	} 

EXIT_PROC:
	  
	return iOutRet;
}

INT_t QMediaSrv::On_GetTunerStandardMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	SharedPtr <CMsgData_GetTunerMode> MsgData_GetTuMode_sp = GenMsgData_sp.DynamicCast<CMsgData_GetTunerMode>();
	INT32_t iStdMode = -1;

	if(MsgData_GetTuMode_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->GetTunerStandardMode(&iStdMode);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:

	MsgData_GetTuMode_sp->m_iOutRet = iOutRet;
	MsgData_GetTuMode_sp->m_channelStdMode = iStdMode;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
	
}

INT_t QMediaSrv::On_GetEntryChannelInfo(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_GetEntryInfo> CMsgData_GetEntryInfo_sp = GenMsgData_sp.DynamicCast<CMsgData_GetEntryInfo>();

	if(CMsgData_GetEntryInfo_sp.isNull())
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	ZeroMemory(CMsgData_GetEntryInfo_sp->m_StrProgramPids,64);
	if(FALSE == m_NexusPlayer_sp.isNull())
	{	
		iRet = m_NexusPlayer_sp->GetEntryChannelInfo(CMsgData_GetEntryInfo_sp->m_EntryId,
									&CMsgData_GetEntryInfo_sp->m_ChannelNum,
									&CMsgData_GetEntryInfo_sp->m_SubChannlNum,
									CMsgData_GetEntryInfo_sp->m_StrProgramPids);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}


EXIT_PROC:

	CMsgData_GetEntryInfo_sp->m_iOutRet = iOutRet;
	iRet = TransactionMsg->SetEvent();
	if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR;
	}

	return iOutRet;
}

INT_t QMediaSrv::StopTunerPlayer()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	BOOL_t bSendPlayEos = TRUE;
	INT_t iCurrPlayStatus= PLAY_STAT_IDLE;
	INT_t iOldPlayStatus;
	
	iOutRet = getMpSrvPlayStatus(&iCurrPlayStatus);
	if(ERROR_SUCCESS != iOutRet) 
	{
		PRINT_BFILE_LINENO_IRET_CRT_ERR
	}

	if(PLAY_STAT_TUNER_SCANNING == iCurrPlayStatus)
	{
		bSendPlayEos = FALSE;
	}

	if(m_NexusPlayer_sp.isValid())
	{
		iRet = m_NexusPlayer_sp->Stop();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		iOldPlayStatus = m_iPlayStatus;
	}

	//new state
	{
		m_iPlayStatus = PLAY_STAT_IDLE;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStatus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

EXIT_PROC:
	
	return iOutRet;
}

#endif

INT_t QMediaSrv::On_Stop(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	INT_t iOldPlayStatus,iCurrPlayStatus;

	if(currentThreadId() != m_hThisThreadId)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		iOutRet = getMpSrvPlayStatus(&iCurrPlayStatus);
		if(ERROR_SUCCESS != iOutRet) 
		{
			//PRINT_BFILE_LINENO_RMSTATUS;
		}

		if(PLAY_STAT_IDLE == m_iPlayStatus && PLAY_STAT_IDLE == iCurrPlayStatus)
		{
			goto EXIT_PROC;
		}
	}

	m_bIsCompletePlaybackEos = FALSE;	//user break

	if(m_NexusPlayer_sp.isValid())
	{
		iRet = m_NexusPlayer_sp->Stop();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	//return playbackId if possible
	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		iOldPlayStatus = m_iPlayStatus;
	}
	
	if(PLAY_STAT_PLAYING == iOldPlayStatus)
	{
		SharedPtr <CMsgData_StopPlayback> stopPlaybackMsgData_sp;
		stopPlaybackMsgData_sp = GenMsgData_sp.DynamicCast<CMsgData_StopPlayback>();
		if(stopPlaybackMsgData_sp.isValid())
		{
			UINT32_t playbackId;
			QString strUrl;
			{
				CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
				playbackId = m_PlaybackId;
			
			}
			{
				strUrl = m_sDataSrcUrl;
			}
			{
				CAutoScopeLock autoScopeLock(&(stopPlaybackMsgData_sp->mLock));
				stopPlaybackMsgData_sp->m_PlaybackId = playbackId;
				stopPlaybackMsgData_sp->m_strUrl = strUrl.toUtf8().constData();
			}
		}
	}

	//new state
	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		//new play state
		iOldPlayStatus = m_iPlayStatus;
		m_iPlayStatus = PLAY_STAT_IDLE;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStatus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_setVideoPosition(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_setVideoPosition> MsgData_setVidPos_sp;

	if(currentThreadId() != m_hThisThreadId)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}

	MsgData_setVidPos_sp = GenMsgData_sp.DynamicCast<CMsgData_setVideoPosition>();
	if(MsgData_setVidPos_sp.isNull())
	{
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}

	if(m_NexusPlayer_sp.isValid())
	{
		iRet = m_NexusPlayer_sp->setVideoPosition(MsgData_setVidPos_sp->x, MsgData_setVidPos_sp->y,
			MsgData_setVidPos_sp->Width, MsgData_setVidPos_sp->Height);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_PlaybackEos(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t eosPlaybackId = 0;
	INT_t iOldPlayStatus;

	if(GenMsgData_sp.isNull())
	{
		LOG_BLINE("BUG\n");
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}

	eosPlaybackId = GenMsgData_sp->m_uiParam;
	{		
		CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
		if(eosPlaybackId != m_PlaybackId)
		{
			LOG_BLINE("inv gst eos,eosId=%d,playbackId=%d\n", eosPlaybackId, m_PlaybackId);
			goto EXIT_PROC;
		}
	}

	m_iRemaingLoopCnt--;
	if(0 < m_iRemaingLoopCnt)
	{
		goto EXIT_PROC;
	}

	LOG_BLINE("PlaybackEos\n");

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		if(PLAY_STAT_IDLE == m_iPlayStatus)
		{
			goto EXIT_PROC;
		}
	}

	if(m_NexusPlayer_sp.isValid())
	{
		iRet = m_NexusPlayer_sp->Stop();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}

	{
		CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
		//new play state
		iOldPlayStatus = m_iPlayStatus;
		m_iPlayStatus = PLAY_STAT_IDLE;
		iRet = setMpSrvPlayStatus(m_iPlayStatus);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(iOldPlayStatus != m_iPlayStatus)
		{
			iRet = m_PlayStatChangedEvent.SetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

	{
		quint32 playbackId = 0;
		QString strPlaybackUrl;
		{
			CAutoScopeLock autoRangeLocker(&m_PlaybackIdMutex);
			playbackId = m_PlaybackId;
		}
		strPlaybackUrl = m_sDataSrcUrl;
		emit sigPlaybackEos(playbackId, strPlaybackUrl, m_bIsCompletePlaybackEos);	//async
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::On_VidDecStreamChanged(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	iRet = applyVideoDispAspectRatio(m_CurVidDispAspectRatio, TRUE/*force*/);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	iRet = changeHdmiDisplaySettings(DispPARAM_Max,Matrixcoefficients_Max);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

				
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::On_DispOnLedScr(SharedPtr<CGenericMsgData> & GenMsgData_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr <CMsgData_DispOnLedScr> MsgData_DispOnLedScr_sp;

	MsgData_DispOnLedScr_sp = GenMsgData_sp.DynamicCast<CMsgData_DispOnLedScr>();
	if(MsgData_DispOnLedScr_sp.isNull())
	{
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}

	iOutRet = DisplayOnLedScreen(MsgData_DispOnLedScr_sp->m_strDispContent, MsgData_DispOnLedScr_sp->m_StartPosToDisp);
	if(ERROR_SUCCESS != iOutRet)
	{
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

/*
ThreadContext: any
*/
INT_t QMediaSrv::DisplayOnLedScreen(LPCSTR strContentToDisp, CONST INT_t StartPosToDisp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	if(NULL == strContentToDisp)	//no err
	{
		goto EXIT_PROC;
	}

	iRet = m_CnkB85LcmLedScr_sp->DisplayStr(strContentToDisp, StartPosToDisp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

//thread proc
void QMediaSrv::run()
{
	int iThreadExitCode;
	
	m_hThisThreadId = QThread::currentThreadId();

	m_threadId = pthread_self();
		
	iThreadExitCode = QThread::exec();
	
EXIT_PROC:	

	m_threadId = 0;
}

void QMediaSrv::quit()
{
	bool bRet;
	INT_t iRet;
	UINT64_t startTimeMs;

	if(currentThreadId() == m_hThisThreadId)
	{
		LOG_BLINE("BUG,incorrect thread context.\n");
		goto EXIT_PROC;
	}

	startTimeMs = GetSysUpTimeMs();

	iRet = Stop();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	m_bShouldExitThread = TRUE;
	QThread::quit();
	{
		int iWaitTimeCntMs = 0;
		int iWaitOnceIntervalMs = 100;
		while(TRUE)
		{
			if(true == isFinished())
			{
				break;
			}
			bRet = wait(iWaitOnceIntervalMs);	//ms
			if(true == bRet)
			{
				break;
			}
			iWaitTimeCntMs += iWaitOnceIntervalMs;
			if(5*1000 <= iWaitTimeCntMs)
			{
				LOG_BLINE("WaitThreadExit(MediaSrv) timeout.\n");
				iWaitTimeCntMs = 0;
			}
		}
	}

	usleep(500*1000);	//give nxclient more time to cleanup
	
EXIT_PROC:
	;
	LOG_BLINE("elapsed=%llums\n", GetSysUpTimeMs()-startTimeMs);
}

INT_t QMediaSrv::setDataSource(const QString & strUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr<CGenericMsgData> GenericMsgData_sp;
	SharedPtr <CMsgData_setDataSource> MsgData_setDataSource_sp(new CMsgData_setDataSource);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);	

	if(NULL == MsgData_setDataSource_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	if(NULL == ThreadMsg_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_setDataSource_sp->m_uiMsgId = MSG_CMD_setDataSource;
	MsgData_setDataSource_sp->m_strDataSrcUrl = strUrl;
	GenericMsgData_sp = MsgData_setDataSource_sp;
	iRet = ThreadMsg_sp->SetMessageData(GenericMsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = SendMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iOutRet = MsgData_setDataSource_sp->m_iOutRet;

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::Prepare()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CGenericMsgData> MsgData_Prepare_sp(new CGenericMsgData);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);

	if(NULL == MsgData_Prepare_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	if(NULL == ThreadMsg_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_Prepare_sp->m_uiMsgId = MSG_CMD_Prepare;
	iRet = ThreadMsg_sp->SetMessageData(MsgData_Prepare_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = SendMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iOutRet = MsgData_Prepare_sp->m_iOutRet;

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode /*= NULL*/,
	VIDEO_OUTPUT_MODE * peOldVoMode /*= NULL*/)
{
    INT_t iOutRet = ERROR_SUCCESS, iRet;
    SharedPtr <CMsgData_ChangeVidOutMode> MsgData_ChangeVidOutMode_sp(new CMsgData_ChangeVidOutMode);
    SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
    SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);

    if( ( MsgData_ChangeVidOutMode_sp.get() == NULL ) || ( ThreadMsg_sp.get() == NULL ) || ( MsgData_sp.get() == NULL) )
    {
        iOutRet = -ERROR_OUT_OF_MEMORY;
        goto EXIT_PROC;
    }
	LOG_LINE("[%s,%d]eVoMode is %d\n",__FILE__, __LINE__,eVoMode);
    MsgData_ChangeVidOutMode_sp->m_uiMsgId = MSG_CMD_ChangeVidOutMode;
    MsgData_ChangeVidOutMode_sp->m_eVoModeToSet = eVoMode;
    MsgData_sp = MsgData_ChangeVidOutMode_sp;
    iRet = ThreadMsg_sp->SetMessageData( MsgData_sp );
    if(ERROR_SUCCESS != iRet){
        iOutRet = iRet>0?-iRet:iRet;
        goto EXIT_PROC;
    }

    iRet = SendMessage( ThreadMsg_sp );
    if(ERROR_SUCCESS != iRet){
        iOutRet = iRet>0?-iRet:iRet;
        goto EXIT_PROC;
    }

    if(peNewVoMode)
    {
        *peNewVoMode = MsgData_ChangeVidOutMode_sp->m_eNewVoMode;
    }

    if(peOldVoMode)
    {
        *peOldVoMode = MsgData_ChangeVidOutMode_sp->m_eOldVoMode;
    }

    if(ERROR_SUCCESS != MsgData_sp->m_iOutRet)
    {
        iOutRet = MsgData_sp->m_iOutRet;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaSrv::setVideoPosition(int x, int y, int w, int h)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CGenericMsgData> MsgData_sp;
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);	
	SharedPtr<CMsgData_setVideoPosition> MsgData_setVidPos_sp(new CMsgData_setVideoPosition);

	if(ThreadMsg_sp.isNull() || MsgData_setVidPos_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_setVidPos_sp->m_uiMsgId = MSG_CMD_setVideoPosition;
	MsgData_setVidPos_sp->x = x;
	MsgData_setVidPos_sp->y = y;
	MsgData_setVidPos_sp->Width = w;
	MsgData_setVidPos_sp->Height = h;
	MsgData_sp = MsgData_setVidPos_sp;
	
    iRet = ThreadMsg_sp->SetMessageData( MsgData_sp );
    if(ERROR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

    iRet = PostMessage( ThreadMsg_sp );
    if(ERROR_SUCCESS != iRet){
        iOutRet = iRet;
        goto EXIT_PROC;
    }

    iOutRet = MsgData_sp->m_iOutRet;

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::ChangeVidOutModeAsync(VIDEO_OUTPUT_MODE eVoMode, 
	VIDEO_OUTPUT_MODE * peNewVoMode /*= NULL*/,VIDEO_OUTPUT_MODE * peOldVoMode /*= NULL*/)
{
    INT_t iOutRet = ERROR_SUCCESS, iRet;
    SharedPtr <CMsgData_ChangeVidOutMode> MsgData_ChangeVidOutMode_sp(new CMsgData_ChangeVidOutMode);
    SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
    SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);

    if( ( MsgData_ChangeVidOutMode_sp.get() == NULL ) || ( ThreadMsg_sp.get() == NULL ) || ( MsgData_sp.get() == NULL) )
    {
        iOutRet = -ERROR_OUT_OF_MEMORY;
        goto EXIT_PROC;
    }

    if(peOldVoMode)
    {
        CAutoScopeLock AutoRangeLocker(&m_SharedDataMutex);
        *peOldVoMode = m_eVoMode;
    }

    MsgData_ChangeVidOutMode_sp->m_uiMsgId = MSG_CMD_ChangeVidOutMode;
    MsgData_ChangeVidOutMode_sp->m_eVoModeToSet = eVoMode;
    MsgData_sp = MsgData_ChangeVidOutMode_sp;
    iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
    if(ERROR_SUCCESS != iRet)
    {
        iOutRet = iRet>0?-iRet:iRet;
        goto EXIT_PROC;
    }

    iRet = PostMessage(ThreadMsg_sp);
    if(ERROR_SUCCESS != iRet)
    {
        iOutRet = iRet>0?-iRet:iRet;
        goto EXIT_PROC;
    }

    if(peNewVoMode)
    {
        *peNewVoMode = eVoMode;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaSrv::DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_DispOnLedScr> MsgData_DispOnLedScr_sp(new CMsgData_DispOnLedScr);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;

	if(MsgData_DispOnLedScr_sp.isNull() || ThreadMsg_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_DispOnLedScr_sp->m_uiMsgId = MSG_CMD_DispOnLedScr;
	MsgData_DispOnLedScr_sp->m_strDispContent = strContentToDisp.toUtf8().constData();
	MsgData_DispOnLedScr_sp->m_StartPosToDisp = dispStartPos;	// start from 1

	MsgData_sp = MsgData_DispOnLedScr_sp;
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::scanI2cDev()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	iRet = sendSimpleMessage(MSG_CMD_scanI2cDev);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::setGpioOutputVal(const int iGpioNum, const bool bVal)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT_t uParam;

	uParam = (iGpioNum & 0x7FFF) | (true==bVal?0x8000:0x0000);
	iRet = sendSimpleMessage(MSG_CMD_setGpioOutputVal, uParam);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaSrv::Play(quint32 * pPlaybackId, IN QMediaSrv::P_PLAY_PARAMS pPlayParams)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_Play> MsgData_Play_sp(new CMsgData_Play);
	SharedPtr<CMessageQueueItem> ThreadMsg_sp(new CMessageQueueItem);
	SharedPtr <CGenericMsgData> MsgData_sp;
	
	if(NULL == MsgData_Play_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	if(NULL == ThreadMsg_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_Play_sp->m_uiMsgId = MSG_CMD_Play;
	if(pPlayParams)
	{
		MsgData_Play_sp->LoopCount = pPlayParams->LoopCount;	
		if(0 >= MsgData_Play_sp->LoopCount)
		{
			iOutRet = ERR_INV_PARAM;
			goto EXIT_PROC;
		}
	}
	else
	{
		MsgData_Play_sp->LoopCount = 1;	
	}
	MsgData_sp = MsgData_Play_sp;
	iRet = ThreadMsg_sp->SetMessageData(MsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	iRet = SendMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iOutRet = MsgData_Play_sp->m_iOutRet;
	if(pPlaybackId)
	{
		*pPlaybackId = MsgData_Play_sp->m_PlaybackId;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::setPlayMode(const int iPlayMode)
{
	return sendSimpleMessage(MSG_CMD_setPlayMode, iPlayMode);
}

INT_t QMediaSrv::getPlayMode(OUT INT_t & iPlayMode)
{
	INT_t iOutRet = ERR_SUCCESS;
	UINT_t uPlayMode;

	iOutRet = sendSimpleMessage(MSG_CMD_getPlayMode, IN 0, OUT &uPlayMode);
	iPlayMode = uPlayMode;

	return iOutRet;
}

INT_t QMediaSrv::getDisplayOutputMode(qint32 & displayoutputMode)
{
	INT_t iOutRet = ERR_SUCCESS;
	UINT_t uPlayMode;

	iOutRet = sendSimpleMessage(MSG_CMD_getDisplayOutputMode, IN 0, OUT &uPlayMode);
	displayoutputMode = uPlayMode;

	return iOutRet;
}

INT_t QMediaSrv::setDisplayOutputMode(qint32 displayoutputMode)
{
	return sendSimpleMessage(MSG_CMD_setDisplayOutputMode, displayoutputMode);
}

INT_t QMediaSrv::getvideoColorSpace(qint32 & colorspace)
{
	INT_t iOutRet = ERR_SUCCESS;
	UINT_t icolorspace;
	iOutRet = sendSimpleMessage(MSG_CMD_getvideoColorSpace, IN 0, OUT &icolorspace);
	colorspace = icolorspace;
	return iOutRet;
}

INT_t QMediaSrv::setvideoColorSpace(qint32 colorspace)
{
	return sendSimpleMessage(MSG_CMD_setvideoColorSpace, colorspace);
}

INT_t QMediaSrv::setHdmi3DTvMode(CONST HDMI_3D_TV_MODE eHdmi3DTvMode)
{
	return sendSimpleMessage(MSG_CMD_setHdmi3DTvMode, eHdmi3DTvMode);
}

INT_t QMediaSrv::getMonitorInfo(OUT QString & strMonitorInfo)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CMonitorInfoParam> MonInfoPara_sp(new CMonitorInfoParam);
	if(MonInfoPara_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	iRet = sendComplexMessage(MSG_CMD_getMonitorInfo, 0/*not used*/, (WeakPtr<CObject>)MonInfoPara_sp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	strMonitorInfo = MonInfoPara_sp->m_sMonitorInfo;
	
EXIT_PROC:
	return iOutRet;
}

//
INT_t QMediaSrv::MibsetMonitorInfo(qint32 MonitorType)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->MibsetMonitorInfo(MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::MibgetMonitorInfo(OUT qint32 & MonitorType)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	if(m_NexusPlayer_sp.isNull())
	{
		PRINT_BFILE_LINENO_BUG_STR;
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iRet = m_NexusPlayer_sp->MibgetMonitorInfo(OUT MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::getShowClosedCaption(OUT qint32 & iCcValue)
{
	INT_t iOutRet = ERR_SUCCESS;
	
	iCcValue = mibCcValue;
	return iOutRet;
}


INT_t QMediaSrv::setShowClosedCaption(qint32 iCcValue)
{
	int iOutRet = ERR_SUCCESS; 
	
	SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	mibCcValue = iCcValue;
	MsgData_sp->m_uiParam = iCcValue;   
	iOutRet = On_setShowClosedCaption(MsgData_sp);
	
	if(ERR_SUCCESS != iOutRet)
	{
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}


//
/*add 2015-05-28 Realization mib speedcontrol*/
INT_t QMediaSrv::setPlaySpeed(qint32 iPlaySpeed)
{
	int iOutRet = ERR_SUCCESS; 
	
	SharedPtr <CGenericMsgData> MsgData_sp(new CGenericMsgData);
	if(MsgData_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	
	MsgData_sp->m_uiParam = iPlaySpeed;
	mibPlaySpeed = iPlaySpeed;   //In order to store 
	iOutRet = On_setPlaySpeedCtrl(MsgData_sp);
	
	if(ERR_SUCCESS != iOutRet)
	{
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}
INT_t QMediaSrv::getPlaySpeed(OUT qint32 & iPlaySpeed)
{
	INT_t iOutRet = ERR_SUCCESS;
	UINT_t uPlayMode;
	
	iPlaySpeed = mibPlaySpeed;
	return iOutRet;
}
//

INT_t QMediaSrv::setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	SharedPtr <CVidDispAspectRatioParam> VidDispAspectRatioParam_sp(new CVidDispAspectRatioParam);
	//LOG_BLINE("QMediaSrv::setVideoDispAspectRatio\n");
	if(VidDispAspectRatioParam_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	VidDispAspectRatioParam_sp->m_iVidDispAspectRatio = iVidAspectRatio;
	VidDispAspectRatioParam_sp->m_uOptFlags = uOptFlags;
	iRet = sendComplexMessage(MSG_CMD_setVidDispAspectRatio, 0/*not used*/, (WeakPtr<CObject>)VidDispAspectRatioParam_sp);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::getVideoDispAspectRatio(OUT qint32 & iVidDispAspectRatio)
{
	INT_t iOutRet = ERR_SUCCESS;
	UINT_t uParam;

	iOutRet = sendSimpleMessage(MSG_CMD_getVidDispAspectRatio, IN 0, OUT &uParam);
	iVidDispAspectRatio = uParam;

	return iOutRet;
}

INT_t QMediaSrv::Stop(quint32 * pPlaybackId/* = NULL*/, QString * pStrUrl/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CMsgData_StopPlayback> MsgData_Stop_sp;
	SharedPtr<CMessageQueueItem> ThreadMsg_sp;
	SharedPtr <CGenericMsgData> GenericMsgData_sp;

	if(Sw_LogPlaybackStatus)
	{
		LOG_BLINE("Stop\n");
	}

	MsgData_Stop_sp = SharedPtr <CMsgData_StopPlayback>(new CMsgData_StopPlayback);
	ThreadMsg_sp = SharedPtr<CMessageQueueItem> (new CMessageQueueItem);
	if(NULL == MsgData_Stop_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	if(NULL == ThreadMsg_sp.get())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	MsgData_Stop_sp->m_uiMsgId = MSG_CMD_Stop;
	GenericMsgData_sp = MsgData_Stop_sp;
	iRet = ThreadMsg_sp->SetMessageData(GenericMsgData_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = PostMessage(ThreadMsg_sp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	if(0 < m_threadId)
	{
		CThread::Interrupt(m_threadId);
	}

	while(TRUE)
	{
		if(false == isRunning())	//thread is not running
		{
			iOutRet = ERR_INV_STATE;
			break;
		}
		iRet = ThreadMsg_sp->WaitEventTimeout(100);	//ms
		if(ERROR_SUCCESS == iRet)
		{
			iOutRet = (MsgData_Stop_sp->m_iOutRet);
			break;
		}
		if(false == isRunning())	//thread is not running
		{
			iOutRet = ERR_INV_STATE;
			break;
		}
		{
			CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
			if(PLAY_STAT_IDLE == m_iPlayStatus)
			{
				iRet = ThreadMsg_sp->WaitEventTimeout(0);
				if(ERROR_SUCCESS == iRet)
				{
					iOutRet = (MsgData_Stop_sp->m_iOutRet);
				}
				else if(ERR_TIMEOUT == iRet)
				{
					iOutRet = ERR_SUCCESS;
				}
				else	//err
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				break;
			}
		}
	}
	//get outputs
	{
		CAutoScopeLock autoScopeLock(&(MsgData_Stop_sp->mLock));
		if(pStrUrl)
		{
			*pStrUrl = (LPCSTR)(MsgData_Stop_sp->m_strUrl);
		}
		if(pPlaybackId)
		{
			*pPlaybackId = MsgData_Stop_sp->m_PlaybackId;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaSrv::ChangeVolume(int VolumePercentToChange)
{	
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	CAutoScopeLock autoRangeLocker(&m_PlayStatusMutex);
	if(FALSE == m_NexusPlayer_sp.isNull())
	{
		iRet = m_NexusPlayer_sp->ChangeVolume(VolumePercentToChange);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	
	return iOutRet;

}

