#ifndef	_SYS_PROP_OPS_CLIENT_H_
#define	_SYS_PROP_OPS_CLIENT_H_

#include <UnixDatagramSocket.h>
#include <StackBufString.h>
#include <SendBufInStack.h>
#include <RecvBufInStack.h>
#include <SharedPtr.h>
#include <Mutex2.h>

using namespace CppBase;

class CSysPropOpsClient
{
public:
	CSysPropOpsClient();
	~CSysPropOpsClient();
	INT_t setPersistent(LPCSTR pszKey, LPCSTR pszValue);
	INT_t setTemp(LPCSTR pszKey, LPCSTR pszValue);
	INT_t set(LPCSTR pszKey, LPCSTR pszValue, BOOL_t bPersistent);
	INT_t get(LPCSTR pszKey, OUT CStackBufString & strValue);
	INT_t get(LPCSTR pszKey, OUT CString & strValue);
	INT_t get(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strValue);
	INT_t get(LPCSTR pszKey, char * pValueBuf, CONST size_t ValBufSize);
	INT_t del(LPCSTR pszKey);
	INT_t getCount(INT_t * CONST piCount);
private:
	VOID UpdateNextSendSerialNo(OUT UINT32_t & NextSendSerialNo);
	INT_t PackageSendHeader(OUT CSendBufInStack & SendBuf, BYTE MsgType);
	INT_t DoMethodCallTransaction(OUT CSendBufInStack & SendBuf, OUT CRecvBufInStack & RecvBuf);
private:
	BOOL_t m_bInited;
	SharedPtr <CUnixDatagramSocket> m_CommSck_sp;
	UINT32_t m_SendSerialNo;
	struct sockaddr_un m_SrvSckAddr;
	socklen_t m_SrvSckAddrLen;
	CMutex2 m_SharedDataMutex;
};

#endif	//_SYS_PROP_OPS_CLIENT_H_

