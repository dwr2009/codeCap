#include "SysBaseSrv.h"
#include <SignalInfo.h>
#include <signal.h>
#include <ParcelProtocol.h>
#include <DirectoryManage.h>

CSysBaseSrvApp::CSysBaseSrvApp()
{
	m_TimerId_SaveSysPropData = 0;
}

CSysBaseSrvApp::~CSysBaseSrvApp()
{
	UnregTimer_SaveSysPropData();
}

void CSysBaseSrvApp::SigHandler(int iSigNo)
{
	INT iRet;
	CSysBaseSrvApp * pApp = (typeof(pApp))GetApp();

	printf("Signal:%s\n", CSignalInfo::GetSingalDescFromSigNo(iSigNo));
	if(SIGINT == iSigNo || SIGTERM == iSigNo || SIGQUIT == iSigNo)
	{
		if(pApp)
		{
			iRet = pApp->PostMessage(EVT_ID_QUIT_EVENT_LOOP, 0, NULL);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}
}

INT CSysBaseSrvApp::InitInstance(int argc, char * argv[])
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	C_UNUSED(iRet);

	do
	{
		//Catch SIGINT,SIGTERM and SIGQUIT signals
		{
			sighandler_t pOldSigHandler = NULL;
			
			pOldSigHandler = signal(SIGINT, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pOldSigHandler = signal(SIGTERM, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pOldSigHandler = signal(SIGQUIT, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
		}
		m_oSrvSck_sp = SharedPtr <CUnixDatagramSocket> (new CUnixDatagramSocket);
		if(m_oSrvSck_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_oSysPropOps_sp = SharedPtr <CSysPropOps> (new CSysPropOps);
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = CreateDirIfNotExist(SYS_DATA_DIR);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_oSysPropOps_sp->Load();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		iRet = m_oSrvSck_sp->Create(SBS_SOCKET_PATH);
		if(ERROR_SUCCESS != iRet)
		{
			if(ERR_SCK_BIND_FAIL == iRet)
			{
				LOG_BLINE("iRet=%d,%s\n", iRet, strerror(m_oSrvSck_sp->getLastCrtErrNo()));
			}
			iOutRet = iRet;
			break;
		}
		{
			int hSck = m_oSrvSck_sp->getSocketHandle();
			iRet = RegisterFdEventSelect(hSck, EVENT_SELECT_READ);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			iRet = m_oSrvSck_sp->setSckSendTimeout(3*1000);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		iRet = m_SrvSckReadBuf.SetBufSize(16*1024);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_SrvSckSendBuf.SetBufSize(16*1024);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::ExitInstance()
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(m_oSysPropOps_sp.isValid())
		{
		}
		if(m_oSrvSck_sp.isValid())
		{
			iRet = m_oSrvSck_sp->Close();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			m_oSrvSck_sp.Clear();
		}
		m_oSysPropOps_sp.Clear();
	}while(FALSE);

	return iOutRet;
}

VOID CSysBaseSrvApp::OnTimer(UINT uiTimerId)
{
	INT_t iRet;
	if(m_TimerId_SaveSysPropData == uiTimerId)
	{
		UnregTimer_SaveSysPropData();
		iRet = SaveSysPropData();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

INT CSysBaseSrvApp::EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam)
{
	INT iOutRet = ERROR_SUCCESS;

	switch(uiEvtId)
	{
		case EVT_ID_FD_READ:
		{
			int iFd = uiParam;
			OnFdRead(iFd);
			break;
		}
		case EVT_ID_FD_WRITE:
		{
			int iFd = uiParam;
			C_UNUSED(iFd);
			iOutRet = CEventApp::EventProc(uiEvtId, uiParam, pParam);
			break;
		}
		case EVT_ID_TIMER:
		{
			UINT uiTimerId = uiParam;
			/*
			PRINT_FILE_LINENO;
			printf("Timer Id:%u, CurTime:%llu ms\n", uiTimerId, GetCurTimeMs());
			*/	
			OnTimer(uiTimerId);
			break;
		}
		default:
		{
			iOutRet = CEventApp::EventProc(uiEvtId, uiParam, pParam);
		}
	}
 
	return iOutRet;
}

VOID CSysBaseSrvApp::OnFdRead(int iFd)
{
	if(m_oSrvSck_sp->getSocketHandle() == iFd)
	{
		OnSrvSckRead();
	}
}

VOID CSysBaseSrvApp::UnregTimer_SaveSysPropData()
{
	INT_t iRet;

	if(m_TimerId_SaveSysPropData)
	{
		iRet = UnregisterTimer(m_TimerId_SaveSysPropData);
		if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		m_TimerId_SaveSysPropData = 0;
	}
}

VOID CSysBaseSrvApp::RegTimer_SaveSysPropData()
{
	INT_t iRet;	

	if(m_TimerId_SaveSysPropData)
	{
		UnregTimer_SaveSysPropData();
	}

	iRet = RegisterTimer(1*1000/*timeout*/, OUT m_TimerId_SaveSysPropData);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

INT_t CSysBaseSrvApp::SaveSysPropData()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = m_oSysPropOps_sp->Save();
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		sync();
	}while(FALSE);

	return iOutRet;
}

VOID CSysBaseSrvApp::OnSrvSckRead()
{
	INT_t iRet;
	SOCKADDR PeerSckAddr;
	socklen_t PeerSckLen = sizeof(PeerSckAddr);

	do
	{
		{
			BOOL_t bRemainOldMemSizeIfPossible = TRUE;
			iRet = m_SrvSckReadBuf.SetBufSize((16*1024), bRemainOldMemSizeIfPossible);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
		}
		iRet = m_oSrvSck_sp->RecvFrom(m_SrvSckReadBuf.GetBufPtr(), m_SrvSckReadBuf.GetBufSize(),
			&PeerSckAddr, &PeerSckLen, MSG_DONTWAIT);
		if(-1 == iRet)
		{
			if((EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno))
			{
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			break;
		}
		//LOG_BLINE("iRet=%d\n", iRet);
		{
			BOOL_t bRemainOldMemSizeIfPossible = TRUE;
			iRet = m_SrvSckReadBuf.SetBufSize(iRet, bRemainOldMemSizeIfPossible);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
		}
		iRet = ProcessRequestReply(m_SrvSckReadBuf, IN OUT m_SrvSckSendBuf);
		if(ERROR_SUCCESS != iRet)
		{
			//LOG_BLINE("iRet=%d\n", iRet);
			break;
		}
		if(0 < m_SrvSckSendBuf.GetBufSize())
		{
			ssize_t SizeToSend = m_SrvSckSendBuf.GetCurWriteOff();
			iRet = m_oSrvSck_sp->SendTo(m_SrvSckSendBuf.GetBufPtr(), SizeToSend,
				&PeerSckAddr, PeerSckLen);
			if(SizeToSend != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
		}
	}while(FALSE);
}

INT_t CSysBaseSrvApp::ProcessRequestReply(CRecvBuf2 & SrvSckReadBuf, IN OUT CSendBuf2 & SrvSckSendBuf)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE ParcelMsgType;
	UINT32_t ParcelSerialNo;

	do
	{
		{
			BOOL_t bRemainOldMemSizeIfPossible = TRUE;
			iRet = m_SrvSckSendBuf.SetBufSize((16*1024), bRemainOldMemSizeIfPossible);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	
		iRet = SrvSckReadBuf.SetReadStartOff(0);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SrvSckReadBuf.GetDataWithAutoOff(&ParcelMsgType, 1);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SrvSckReadBuf.GetDataWithAutoOff((PBYTE)&ParcelSerialNo, 4);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = SrvSckSendBuf.SetWriteStartOff(0);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if(PARCEL_MSG_TYPE_METHOD_CALL == ParcelMsgType)
		{
			BYTE ReplyParcelMsgType = PARCEL_MSG_TYPE_METHOD_REPLY;
			iRet = SrvSckSendBuf.WriteDataWithAutoOff(&ReplyParcelMsgType, 1);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = SrvSckSendBuf.WriteDataWithAutoOff((CONST PBYTE)&ParcelSerialNo, 4);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = OnMethodCall(SrvSckReadBuf, IN OUT SrvSckSendBuf);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = ERR_INVALID_CMD;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::OnMethodCall(CRecvBuf2 & SrvSckReadBuf, IN OUT CSendBuf2 & SrvSckSendBuf)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet = 0;
	int WriteStartOff = SrvSckSendBuf.GetCurWriteOff();
	UINT16_t MethodId;

	do
	{
		iRet = SrvSckSendBuf.WriteDataWithAutoOff((CONST PBYTE)&iReplyRet, 4);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SrvSckReadBuf.GetDataWithAutoOff((PBYTE)&MethodId, 2);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
		if(MSG_ID_SysProp_SetPersistent == MethodId || MSG_ID_SysProp_SetTemp == MethodId)
		{
			size_t KeyLen = 0, ValueLen = 0;
			LPCSTR pszKey = NULL, pszValue = NULL;
			iRet = SrvSckReadBuf.GetDataWithAutoOff(OUT pszKey, OUT KeyLen);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = ERROR_INVALID_PARAMETER;
				break;
			}
			iRet = SrvSckReadBuf.GetDataWithAutoOff(OUT pszValue, OUT ValueLen);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = ERROR_INVALID_PARAMETER;
				break;
			}
			iReplyRet = SysProp_Set(pszKey, KeyLen, pszValue, ValueLen, (MSG_ID_SysProp_SetPersistent == MethodId));
		}
		else if(MSG_ID_SysProp_Get == MethodId)
		{
			LPCSTR pszKey = NULL;
			size_t KeyLen = 0;
			DECLARE_CLS_STACK_BUF_STRING(strValue, 16*1024);
			iRet = SrvSckReadBuf.GetDataWithAutoOff(OUT pszKey, OUT KeyLen);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = ERROR_INVALID_PARAMETER;
				break;
			}
			iReplyRet = SysProp_Get(pszKey, OUT strValue);
			if(ERROR_SUCCESS != iReplyRet)
			{
				break;
			}
			//LOG_BLINE("Value:%s\n", (LPCSTR)strValue);
			iReplyRet = SrvSckSendBuf.WriteDataWithAutoOff(strValue);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = iRet;
				break;
			}
		}
		else if(MSG_ID_SysProp_getById == MethodId)
		{
			INT_t iPropId;
			DECLARE_CLS_STACK_BUF_STRING(strPropName, 4*1024);
			DECLARE_CLS_STACK_BUF_STRING(strValue, 16*1024);
			iRet = SrvSckReadBuf.getDataWithAutoOff(&iPropId);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = ERROR_INVALID_PARAMETER;
				break;
			}
			iReplyRet = SysProp_getById(iPropId, OUT strPropName, OUT strValue);
			if(ERROR_SUCCESS != iReplyRet)
			{
				break;
			}
			iReplyRet = SrvSckSendBuf.WriteDataWithAutoOff(strPropName);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = iRet;
				break;
			}
			iReplyRet = SrvSckSendBuf.WriteDataWithAutoOff(strValue);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = iRet;
				break;
			}
		}
		else if(MSG_ID_SysProp_getCount == MethodId)
		{
			INT_t iPropCount;
			iReplyRet = SysProp_getCount(OUT iPropCount);
			if(ERROR_SUCCESS != iReplyRet)
			{
				break;
			}
			iReplyRet = SrvSckSendBuf.WriteDataWithAutoOff(iPropCount);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = iRet;
				break;
			}
		}
		else if(MSG_ID_SysProp_Del == MethodId)
		{
			size_t KeyLen = 0;
			LPCSTR pszKey = NULL;
			iRet = SrvSckReadBuf.GetDataWithAutoOff(OUT pszKey, OUT KeyLen);
			if(ERROR_SUCCESS != iRet)
			{
				iReplyRet = ERROR_INVALID_PARAMETER;
				break;
			}
			iReplyRet = SysProp_Del(pszKey, KeyLen);
		}
		else
		{
			iReplyRet = ERR_INVALID_CMD;
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iReplyRet)
	{
		do
		{
			iRet = SrvSckSendBuf.SetWriteStartOff(WriteStartOff);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = SrvSckSendBuf.WriteDataWithAutoOff((CONST PBYTE)&iReplyRet, 4);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}while(FALSE);
	}

	return iOutRet;
}

INT_t CSysBaseSrvApp::SysProp_Set(LPCSTR pszKey, size_t KeyLen, LPCSTR pszVal, size_t ValueLen, BOOL_t bPersistent)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	C_UNUSED(iRet);

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		{
			DECLARE_CLS_STACK_BUF_STRING(strKey, 512);
			DECLARE_CLS_STACK_BUF_STRING(strValue, 512);
			iRet = strKey.setContent(pszKey, KeyLen);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iRet = strValue.setContent(pszVal, ValueLen);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			//LOG_BLINE("Key=%s,Len=%d,Val=%s,Len=%d\n", (LPCSTR)strKey, KeyLen, (LPCSTR)strValue, ValueLen);
		}
		iOutRet = m_oSysPropOps_sp->setKeyValPair(pszKey, KeyLen, pszVal, ValueLen, bPersistent);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		if(FALSE == bPersistent)
		{
			break;
		}
		RegTimer_SaveSysPropData();
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::SysProp_Get(LPCSTR pszKey, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_oSysPropOps_sp->getKeyValPair(pszKey, OUT strValue);
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::SysProp_getById(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strPropVal)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_oSysPropOps_sp->getById(iPropId, OUT strPropName, OUT strPropVal);
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::SysProp_getCount(OUT INT_t & iPropCount)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_oSysPropOps_sp->getCount(iPropCount);
	}while(FALSE);

	return iOutRet;
}

INT_t CSysBaseSrvApp::SysProp_Del(LPCSTR pszKey, size_t KeyLen)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_oSysPropOps_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_oSysPropOps_sp->DelKey(pszKey, KeyLen);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		RegTimer_SaveSysPropData();
	}while(FALSE);

	return iOutRet;
}

int main(int argc, char * argv[])
{
	int iRet;
	CSysBaseSrvApp oSysBaseSrvApp;

	iRet = oSysBaseSrvApp.Run(argc, argv);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	return 0;
}
	
