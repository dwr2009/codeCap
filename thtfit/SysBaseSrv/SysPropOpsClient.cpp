#include "SysPropOpsClient.h"
#include <ErrPrintHelper.h>
#include <SendBuf2.h>
#include <SendBufInStack.h>
#include <memory.h>
#include "SysBaseSrvApi.h"
#include <ErrPrintHelper.h>
#include <ParcelProtocol.h>
#include <DateTime.h>
#include <AutoLock.h>
#include "nxclient_external_api.h"

static CSysPropOpsClient & getObj_SysPropOpsClient();

static CSysPropOpsClient & getObj_SysPropOpsClient()
{
	static CSysPropOpsClient g_SysPropOpsClient;
	return g_SysPropOpsClient;
}

INT_t SysProp_setPersistent(LPCSTR pszKey, LPCSTR pszValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.setPersistent(pszKey, pszValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_setPersistent(LPCSTR pszKey, CONST INT_t iVal)
{
	INT_t iRet;
	DECLARE_CLS_STACK_BUF_STRING(strVal, 32);
	iRet = strVal.Format("%d", iVal);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_BUG_STR;
		strVal = "0";
	}
	return SysProp_setPersistent(pszKey, (LPCSTR)strVal);
}

INT_t SysProp_setTemp(LPCSTR pszKey, LPCSTR pszValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.setTemp(pszKey, pszValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_get(LPCSTR pszKey, char * pValueBuf, CONST size_t ValBufSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.get(pszKey, pValueBuf, ValBufSize);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}


int SysProp_get3(const char * pszKey, char * pszValBuf, const size_t valBufSize)
{
     return SysProp_get(pszKey, pszValBuf, valBufSize);
}



INT_t SysProp_get(LPCSTR pszKey, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.get(pszKey, OUT strValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_get(LPCSTR pszKey, OUT CString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.get(pszKey, OUT strValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_get(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.get(iPropId, OUT strPropName, OUT strValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_del(LPCSTR pszKey)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.del(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t SysProp_getCount(INT_t * CONST piCount)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSysPropOpsClient & SysPropOpsClient = getObj_SysPropOpsClient();

	do
	{
		iRet = SysPropOpsClient.getCount(piCount);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

CSysPropOpsClient::CSysPropOpsClient()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	m_bInited = FALSE;
	m_SendSerialNo = 0;

	do
	{
		m_CommSck_sp = SharedPtr <CUnixDatagramSocket> (new CUnixDatagramSocket);
		if(m_CommSck_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		ZeroMemory(&m_SrvSckAddr, sizeof(m_SrvSckAddr));
		m_SrvSckAddr.sun_family = AF_UNIX;
		snprintf(m_SrvSckAddr.sun_path, sizeof(m_SrvSckAddr.sun_path), "%s", SBS_SOCKET_PATH);
		m_SrvSckAddr.sun_path[sizeof(m_SrvSckAddr.sun_path)-1] = '\0';
		m_SrvSckAddrLen = sizeof(m_SrvSckAddr);
		iRet = m_CommSck_sp->Create();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_CommSck_sp->setSckSendTimeout(3*1000);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_CommSck_sp->setSckRecvTimeout(4*1000);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		m_bInited = TRUE;
	}while(FALSE);
}

CSysPropOpsClient::~CSysPropOpsClient()
{
	INT_t iRet;
	
	if(m_CommSck_sp.isValid())
	{
		iRet = m_CommSck_sp->Close();
		PrintIRetBugIfFail(iRet)
	}
}

INT_t CSysPropOpsClient::setPersistent(LPCSTR pszKey, LPCSTR pszValue)
{
	return set(pszKey, pszValue, TRUE);
}

INT_t CSysPropOpsClient::setTemp(LPCSTR pszKey, LPCSTR pszValue)
{
	return set(pszKey, pszValue, FALSE);
}

INT_t CSysPropOpsClient::set(LPCSTR pszKey, LPCSTR pszValue, BOOL_t bPersistent)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		{
			CAutoLock AutoRangeLocker(&m_SharedDataMutex);
			if(FALSE == m_bInited)
			{
				iOutRet = ERR_INIT_FAIL;
				break;
			}
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = (bPersistent?MSG_ID_SysProp_SetPersistent:MSG_ID_SysProp_SetTemp);
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszValue);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::get(LPCSTR pszKey, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		if(FALSE == m_bInited)
		{
			iOutRet = ERR_INIT_FAIL;
			break;
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_Get;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//read the property value from the RecvBuf
		{
			LPCSTR pszPropValue = NULL;
			size_t PropValLen = 0;
			iRet = RecvBuf.GetDataWithAutoOff(OUT pszPropValue, OUT PropValLen);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			strValue = pszPropValue;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::get(LPCSTR pszKey, OUT CString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		if(FALSE == m_bInited)
		{
			iOutRet = ERR_INIT_FAIL;
			break;
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_Get;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//read the property value from the RecvBuf
		{
			LPCSTR pszPropValue = NULL;
			size_t PropValLen = 0;
			iRet = RecvBuf.GetDataWithAutoOff(OUT pszPropValue, OUT PropValLen);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			strValue = pszPropValue;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::get(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		if(FALSE == m_bInited)
		{
			iOutRet = ERR_INIT_FAIL;
			break;
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_getById;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(iPropId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//read the property name and value from the RecvBuf
		{
			//name
			LPCSTR pszPropName = NULL;
			size_t PropNameLen = 0;
			iRet = RecvBuf.GetDataWithAutoOff(OUT pszPropName, OUT PropNameLen);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			strPropName = pszPropName;
			//value
			LPCSTR pszPropValue = NULL;
			size_t PropValLen = 0;
			iRet = RecvBuf.GetDataWithAutoOff(OUT pszPropValue, OUT PropValLen);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			strValue = pszPropValue;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::get(LPCSTR pszKey, char * pValueBuf, CONST size_t ValBufSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		if(FALSE == m_bInited)
		{
			iOutRet = ERR_INIT_FAIL;
			break;
		}

		if(NULL == pValueBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_Get;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//read the property value from the RecvBuf
		{
			LPCSTR pszPropValue = NULL;
			size_t PropValLen = 0;
			iRet = RecvBuf.GetDataWithAutoOff(OUT pszPropValue, OUT PropValLen);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if((ValBufSize - 1) < PropValLen)
			{
				iOutRet = ERROR_NOT_ENOUGH_BUFFER;
				break;
			}
			memcpy(pValueBuf, pszPropValue, PropValLen);
			pValueBuf[PropValLen] = '\0';
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::del(LPCSTR pszKey)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		{
			CAutoLock AutoRangeLocker(&m_SharedDataMutex);
			if(FALSE == m_bInited)
			{
				iOutRet = ERR_INIT_FAIL;
				break;
			}
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_Del;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(pszKey);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::getCount(INT_t * CONST piCount)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_SendBufInStack(SendBuf, (16*1024));
	DECLARE_CLS_RecvBufInStack(RecvBuf, (16*1024));
	UINT16_t MsgId;

	do
	{
		{
			CAutoLock AutoRangeLocker(&m_SharedDataMutex);
			if(FALSE == m_bInited)
			{
				iOutRet = ERR_INIT_FAIL;
				break;
			}
		}
		iRet = PackageSendHeader(OUT SendBuf, PARCEL_MSG_TYPE_METHOD_CALL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgId = MSG_ID_SysProp_getCount;
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&MsgId), 2);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = DoMethodCallTransaction(OUT SendBuf, OUT RecvBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//read the return value from the RecvBuf
		{
			iRet = RecvBuf.getDataWithAutoOff(piCount);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

VOID CSysPropOpsClient::UpdateNextSendSerialNo(OUT UINT32_t & NextSendSerialNo)
{	
	CAutoLock AutoRangeLocker(&m_SharedDataMutex);
	m_SendSerialNo++;
	NextSendSerialNo = m_SendSerialNo;
}

INT_t CSysPropOpsClient::PackageSendHeader(OUT CSendBufInStack & SendBuf, BYTE MsgType)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t NextSendSerialNo;

	do
	{
		iRet = SendBuf.SetWriteStartOff(0);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SendBuf.WriteDataWithAutoOff(&MsgType, 1);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		UpdateNextSendSerialNo(OUT NextSendSerialNo);
		iRet = SendBuf.WriteDataWithAutoOff((PBYTE)(&NextSendSerialNo), 4);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsClient::DoMethodCallTransaction(OUT CSendBufInStack & SendBuf, OUT CRecvBufInStack & RecvBuf)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SOCKADDR RecvedSckAddr;
	socklen_t RecvedSckAddrLen;
	UINT64_t StartTimeMs = GetSysUpTimeMs(), CurTimeMs;
	ssize_t SizeToSend;

	CAutoLock AutoRangeLocker(&m_SharedDataMutex);

	do
	{
		m_SrvSckAddrLen = sizeof(m_SrvSckAddr);
		SizeToSend = SendBuf.GetCurWriteOff();
		iRet = m_CommSck_sp->SendTo((PVOID)(SendBuf.GetBufPtr()), SizeToSend, 
			(const SOCKADDR*)(&m_SrvSckAddr), m_SrvSckAddrLen);
		if(-1 == iRet && ETIMEDOUT == errno)
		{
			iOutRet = ERR_TIMEOUT;
			break;
		}
		else if(SizeToSend != iRet)
		{
			iOutRet = ERROR_SOCKET_SEND_FAIL;
			//PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		while(TRUE)
		{
			RecvedSckAddrLen = sizeof(RecvedSckAddr);
			iRet = m_CommSck_sp->RecvFrom(RecvBuf.GetBufPtr(), RecvBuf.GetBufSize(), &RecvedSckAddr,
				&RecvedSckAddrLen);
			if(CC_UNLIKELY(-1 == iRet && ETIMEDOUT == errno))
			{
				iOutRet = ERR_TIMEOUT;
				break;
			}
			else if(CC_UNLIKELY(0 >= iRet))
			{
				if(EAGAIN == errno || EWOULDBLOCK == errno)
				{
					iOutRet = ERR_TIMEOUT;
				}
				else
				{
					iOutRet = ERROR_SOCKET_RECV_FAIL;
				}
				//PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
			else	//SUCCESS
			{
				BOOL_t bRecvedReplyPackage = FALSE;
				BYTE ReplyMsgType;
				UINT32_t ReplySerialNo;
				INT_t iReplyOutRet;
				
				do
				{
					iRet = RecvBuf.SetBufSize(iRet);
					if(ERROR_SUCCESS != iRet)
					{
						break;
					}
					iRet = RecvBuf.GetDataWithAutoOff(&ReplyMsgType, 1);
					if(ERROR_SUCCESS != iRet)
					{
						break;
					}
					if(PARCEL_MSG_TYPE_METHOD_REPLY != ReplyMsgType)
					{
						break;
					}
					iRet = RecvBuf.GetDataWithAutoOff((PBYTE)(&ReplySerialNo), 4);
					if(ERROR_SUCCESS != iRet)
					{
						break;
					}
					if(ReplySerialNo != m_SendSerialNo)
					{
						break;
					}
					iRet = RecvBuf.GetDataWithAutoOff((PBYTE)(&iReplyOutRet), 4);
					if(ERROR_SUCCESS != iRet)
					{
						break;
					}
					iOutRet = iReplyOutRet;
					bRecvedReplyPackage = TRUE;
				}while(FALSE);
				if(bRecvedReplyPackage)
				{
					break;
				}
				CurTimeMs = GetSysUpTimeMs();
				if((4*1000) <= (CurTimeMs - StartTimeMs))
				{
					iOutRet = ERR_TIMEOUT;
					break;
				}
			}
		}
	}while(FALSE);
	
	return iOutRet;
}

