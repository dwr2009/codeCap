#ifndef	_LINUX_SOCKET_H_
#define	_LINUX_SOCKET_H_

#include <BaseTypeDef.h>
#include <sys/socket.h>
#include <stdio.h>

class CLinuxSocket
{
public:
	CLinuxSocket();
	virtual ~CLinuxSocket();
	virtual INT setSocketSendBufSize(INT iSendBufSize);
	virtual INT getSocketSendBufSize(P_INT piSendBufSize);
	virtual INT setSocketRecvBufSize(INT iSendBufSize);
	virtual INT getSocketRecvBufSize(P_INT piSendBufSize);
	virtual INT setSckRecvTimeout(INT_t iTimeoutMs);
	virtual INT getSckRecvTimeout(INT_t * piTimeoutMs);
	virtual INT_t setSckSendTimeout(INT_t iTimeoutMs);
	virtual SOCKET getSocketHandle();
	virtual INT Attach(SOCKET hSocket);
	virtual INT Connect(SOCKADDR * pSckAddr, socklen_t sizeAddrLen);
	virtual INT Send(PVOID pBuf, size_t sizeBuf, INT iFlags = 0, size_t * psizeSended = NULL);
	virtual INT Receive(PVOID pBuf, size_t sizeBuf, INT iFlags, size_t * psizeRead, 
		INT_t iTimeoutMs = -1);
	virtual int getLastCrtErrNo();
protected:
	void CloseSocket();
protected:
	SOCKET m_hSocket;
	int m_iLastCrtErrNo;
};

#endif	//_LINUX_SOCKET_H_

