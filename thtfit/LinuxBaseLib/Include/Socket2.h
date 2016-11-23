#ifndef	_SOCKET2_H_
#define	_SOCKET2_H_

#include	<stdio.h>
#include	<SocketTypeDef.h>
#include	<BaseTypeDef.h>
#include	<String.h>

#define	ERROR_CSOCKET2_BASE				1000
#define	ERROR_SOCKET_BIND_FAIL			(ERROR_CSOCKET2_BASE+1)
#define	ERROR_GETSOCKNAME_FAIL			(ERROR_CSOCKET2_BASE+2)
#define	ERROR_CONVERT_IP_FAIL			(ERROR_CSOCKET2_BASE+3)
#define	ERROR_SELECT_FDS_EXCEPTION		(ERROR_CSOCKET2_BASE+4)

class CSocket2
{
public:
	CSocket2();
	virtual ~CSocket2();
	INT Create(INT iSckType, LPCSTR pszAddrToBind = NULL,  WORD usPortToBind = 0, 
		BOOL bReuseAddr = FALSE);
	virtual INT AttachSck(SOCKET sckNew);
	virtual INT Close();
	INT GetSockName(CString * pStrAddr, WORD * pusPort);
	INT GetSockPort(WORD * pusPort);
	virtual INT SetSocketRecvBufSize(INT iRecvBufSize);
	virtual INT getSocketRecvBufSize(P_INT piRecvBufSize);
	virtual INT setSocketSendBufSize(INT iSendBufSize);
	virtual INT getSocketSendBufSize(P_INT piSendBufSize);
	SOCKET GetSocketHandle();
	INT SendTo(LPBYTE pSendBuf, size_t iBufSize, INT iFlags = 0, 
		P_SOCKADDR pSckAddrPeer = NULL, socklen_t iToLen = 0);
	INT RecvFrom(LPBYTE pRecvBuf, size_t iBufSize, INT iFlags = 0, 
		P_SOCKADDR pSckAddrPeer = NULL, socklen_t * piFromLen = NULL, UINT uiTimeoutMS = 0);
	INT RecvMsg(struct msghdr * pMsgHdr, INT iFlags = 0, INT_t iTimeoutMS = 0);
	int GetLastCrtErrNo();
	INT GetLastLocalErrNo();
	virtual INT setSckRecvTimeout(INT_t iTimeoutMs);
	virtual INT getSckRecvTimeout(INT_t * piTimeoutMs);
protected:
	INT m_iLocalErrNo;
	SOCKET m_hSocket;
	int m_iCrtErrNo;
};

#endif	//_SOCKET2_H_

