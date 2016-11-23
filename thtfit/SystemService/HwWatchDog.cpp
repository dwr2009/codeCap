#include <HwWatchDog.h>
#include <SystemServiceAppIf.h>
#include <QtWidgets/QApplication>
#include <SystemSrvApi.h>
#include <QDBusError>
//#include <WatchDogApi.h>
#include <DateTime.h>
#include <memory2.h>

QDBusIf_HwWatchDog::QDBusIf_HwWatchDog(QHwWatchDog * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pHwWatchDog = pParentObj;	
	setAutoRelaySignals(true);
}

QDBusIf_HwWatchDog::~QDBusIf_HwWatchDog()
{
	m_pHwWatchDog = NULL;	
}

void QDBusIf_HwWatchDog::FeedWatchDog(int & iOutRet)
{
	if(m_pHwWatchDog)
	{
		iOutRet = m_pHwWatchDog->FeedWatchDog();
	}
	else
	{
		iOutRet = ERR_INVALID_STATE;
	}
}

void QDBusIf_HwWatchDog::WD_SetParam(int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs, int & iOutRet)
{
	if(m_pHwWatchDog)
	{
		iOutRet = m_pHwWatchDog->WD_SetParam(bAutoFeedWatchDog, nWatchDogTimeoutMs);
	}
	else
	{
		iOutRet = ERR_INVALID_STATE;
	}
}

QHwWatchDog::QHwWatchDog()
{
	m_bNeedFeedWatchDog = FALSE;
	ZeroMemory(&m_WdSckAddr, sizeof(m_WdSckAddr));
	m_WdSckAddr.sun_family = AF_UNIX;
	m_WdSckAddr.sun_path[0] = '\0';
	/*
	strncpy(&(m_WdSckAddr.sun_path[1]), WdSrvSck_HiddenBindPath, sizeof(m_WdSckAddr.sun_path)-1);
	m_WdSckAddr.sun_path[sizeof(m_WdSckAddr.sun_path)-1] = '\0';
	*/
}

QHwWatchDog::~QHwWatchDog()
{
}

INT_t QHwWatchDog::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	BOOL_t bRet;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		m_DbusIf_HwWatchDog_sp = QSharedPointer <QDBusIf_HwWatchDog>(new QDBusIf_HwWatchDog(this));
		if(m_DbusIf_HwWatchDog_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = DBusConn_sp->registerObject(DBUS_PATH_SysSrv_HwWatchDog, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus obj %s\n", DBUS_PATH_SysSrv_HwWatchDog);
			}
			break;
		}

		m_WdCtrlSckClient_sp = QSharedPointer <CUnixDatagramSocket>(new CUnixDatagramSocket);
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_WdCtrlSckClient_sp->Create();
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		/*
		bRet = connect(&m_FeedWdTimer, SIGNAL(timeout()), this, SLOT(On_FeedWatchDog()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimer(FeedWdTimer) err\n");
		}
		*/
	}while(FALSE);

	return iOutRet;
}

INT_t QHwWatchDog::ExitInstance()
{	
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_SysSrv_HwWatchDog); 		
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t QHwWatchDog::FeedWatchDog()
{
#if 0
	INT_t iOutRet = ERR_SUCCESS, iRet;
	char szSendBuf[sizeof(WD_SrvMsgHead_DEF_t)];
	char szRecvBuf[sizeof(WD_SrvMsgHead_DEF_t)+sizeof(WD_Data_FeedWatchDog_Reply_t)];
	WD_SrvMsgHead_DEF_t * pMsgHead = NULL;	

	if(m_WdCtrlSckClient_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	pMsgHead = (typeof(pMsgHead))(szSendBuf);
	pMsgHead->msgSize = sizeof(szSendBuf);
	pMsgHead->msgId = WD_MSG_FeedWatchDog;
	iRet = m_WdCtrlSckClient_sp->setSckSendTimeout(5*1000/*ms*/);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	{
		int tryTimes = 2;
		iRet = m_WdCtrlSckClient_sp->SendTo(szSendBuf, sizeof(szSendBuf), (CONST SOCKADDR *)(&m_WdSckAddr), sizeof(m_WdSckAddr));
		tryTimes--;
		if(0 > iRet)
		{
			if(ETIMEDOUT == errno || EAGAIN == errno || EWOULDBLOCK == errno)
			{
				iOutRet = ERR_TIMEOUT;
			}
			else if(ENOTCONN == errno || ECONNREFUSED == errno)
			{
				if(0 < tryTimes)
				{
				}
				iOutRet = ERR_SOCKET_SEND_FAIL;
			}
			else	//unignorable errs
			{
				LOG_BLINE("ret=%d,errno=%d,%s\n", iRet, errno, strerror(errno));
				iOutRet = ERR_SOCKET_SEND_FAIL;
			}
			goto EXIT_PROC;
		}
	}

	iRet = m_WdCtrlSckClient_sp->setSckRecvTimeout(5*1000/*ms*/);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iRet = m_WdCtrlSckClient_sp->RecvFrom(szRecvBuf, sizeof(szRecvBuf));
	if(0 > iRet)
	{
		if(ETIMEDOUT == errno || EAGAIN == errno || EWOULDBLOCK == errno)
		{
			iOutRet = ERR_TIMEOUT;
		}
		else
		{
			LOG_BLINE("ret=%d,errno=%d,%s\n", iRet, errno, strerror(errno));
			iOutRet = ERR_SOCKET_RECV_FAIL;
		}
		goto EXIT_PROC;
	}
	{
		size_t RecvedBytes = iRet;
		WD_Data_FeedWatchDog_Reply_t * pFeedWdReply;
		if(sizeof(szRecvBuf) > RecvedBytes)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		pMsgHead = (typeof(pMsgHead))szRecvBuf;
		if(sizeof(szRecvBuf) > pMsgHead->msgSize)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		if(WD_MSG_FeedWatchDog_Reply != pMsgHead->msgId)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		pFeedWdReply = (typeof(pFeedWdReply))((unsigned long)pMsgHead + sizeof(*pMsgHead));
		if(0 != pFeedWdReply->iErrNo)
		{
			iOutRet = (ERR_FAILED | (pFeedWdReply->iErrNo << 16));
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
#endif
	return 0;
}

INT_t QHwWatchDog::WD_SetParam(const int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs)
{
#if 0
	INT_t iOutRet = ERR_SUCCESS, iRet;
	char szSendBuf[sizeof(WD_SrvMsgHead_DEF_t)+sizeof(WD_Data_SetParam_t)];
	char szRecvBuf[sizeof(WD_SrvMsgHead_DEF_t)+sizeof(WD_Data_SetParam_Reply_t)];
	WD_SrvMsgHead_DEF_t * pMsgHead = NULL;

	if(m_WdCtrlSckClient_sp.isNull())
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	{
		pMsgHead = (typeof(pMsgHead))(szSendBuf);
		pMsgHead->msgSize = sizeof(szSendBuf);
		pMsgHead->msgId = WD_MSG_SetParam;
		WD_Data_SetParam_t * pParamData = (typeof(pParamData))((unsigned long)pMsgHead+sizeof(*pMsgHead));
		pParamData->bAutoFeedWatchDog = bAutoFeedWatchDog;
		pParamData->nWatchDogTimeoutMs = nWatchDogTimeoutMs;
		if(FALSE == bAutoFeedWatchDog && WD_MAX_TIMEOUT_MS < pParamData->nWatchDogTimeoutMs)
		{
			m_bNeedFeedWatchDog = TRUE;
			m_TotalWatchDogTimeoutMs = pParamData->nWatchDogTimeoutMs;
			m_WatchDogStartTimeMs = GetSysUpTimeMs();
			pParamData->nWatchDogTimeoutMs = WD_MAX_TIMEOUT_MS;
		}
		else
		{
			m_bNeedFeedWatchDog = FALSE;
		}
	}
	
	iRet = m_WdCtrlSckClient_sp->setSckSendTimeout(5*1000/*ms*/);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	{
		int tryTimes = 2;
		iRet = m_WdCtrlSckClient_sp->SendTo(szSendBuf, sizeof(szSendBuf), (CONST SOCKADDR *)(&m_WdSckAddr), sizeof(m_WdSckAddr));
		tryTimes--;
		if(0 > iRet)
		{
			if(ETIMEDOUT == errno || EAGAIN == errno || EWOULDBLOCK == errno)
			{
				iOutRet = ERR_TIMEOUT;
			}
			else if(ENOTCONN == errno || ECONNREFUSED == errno)
			{
				if(0 < tryTimes)
				{
				}
				iOutRet = ERR_SOCKET_SEND_FAIL;
			}
			else	//unignorable errs
			{
				LOG_BLINE("ret=%d,errno=%d,%s\n", iRet, errno, strerror(errno));
				iOutRet = ERR_SOCKET_SEND_FAIL;
			}
			goto EXIT_PROC;
		}
	}

	if(TRUE == m_bNeedFeedWatchDog)
	{
		unsigned int feedWdIntervalMs = (WD_MAX_TIMEOUT_MS*2/3);
		if(0 >= feedWdIntervalMs)
		{
			feedWdIntervalMs = 500;
		}
		m_FeedWdTimer.start(feedWdIntervalMs);
	}
	else
	{
		m_FeedWdTimer.stop();
	}

	iRet = m_WdCtrlSckClient_sp->setSckRecvTimeout(5*1000/*ms*/);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iRet = m_WdCtrlSckClient_sp->RecvFrom(szRecvBuf, sizeof(szRecvBuf));
	if(0 > iRet)
	{
		if(ETIMEDOUT == errno || EAGAIN == errno || EWOULDBLOCK == errno)
		{
			iOutRet = ERR_TIMEOUT;
		}
		else
		{
			LOG_BLINE("ret=%d,errno=%d,%s\n", iRet, errno, strerror(errno));
			iOutRet = ERR_SOCKET_RECV_FAIL;
		}
		goto EXIT_PROC;
	}
	{
		size_t RecvedBytes = iRet;
		WD_Data_FeedWatchDog_Reply_t * pSetParamReply = NULL;
		if(sizeof(szRecvBuf) > RecvedBytes)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		pMsgHead = (typeof(pMsgHead))szRecvBuf;
		if(sizeof(szRecvBuf) > pMsgHead->msgSize)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		if(WD_MSG_SetParam_Reply != pMsgHead->msgId)
		{
			iOutRet = ERR_INVALID_REPLY;
			goto EXIT_PROC;
		}
		pSetParamReply = (typeof(pSetParamReply))((unsigned long)pMsgHead + sizeof(*pMsgHead));
		if(0 != pSetParamReply->iErrNo)
		{
			iOutRet = (ERR_FAILED | (pSetParamReply->iErrNo << 16));
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
#endif
	return 0;
}

void QHwWatchDog::On_FeedWatchDog()
{
	INT_t iRet;
	UINT64_t curRelTimeMs = 0;
	
	if(FALSE == m_bNeedFeedWatchDog)
	{
		goto EXIT_PROC;
	}

	curRelTimeMs = GetSysUpTimeMs();
	if(curRelTimeMs - m_WatchDogStartTimeMs >= m_TotalWatchDogTimeoutMs)
	{
		m_bNeedFeedWatchDog = FALSE;
		m_FeedWdTimer.stop();
		goto EXIT_PROC;
	}
	iRet = FeedWatchDog();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

EXIT_PROC:
	;
}

