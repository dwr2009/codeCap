#ifndef	_UNIX_SOCKET_H_
#define	_UNIX_SOCKET_H_

#include <BaseTypeDef.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h>
#include <LinuxSocket.h>

class CUnixSocket : public CLinuxSocket
{
public:
	CUnixSocket();
	virtual ~CUnixSocket();
	virtual INT Create(INT iType = SOCK_DGRAM, LPCSTR pszPathToBind = NULL, BOOL_t bAutoGenePath = FALSE);
	virtual INT Close();
	virtual INT Connect(LPCSTR pszUnixSckPath);
	virtual INT getSockName(struct sockaddr_un * psckaddrLocal);
	virtual INT SendTo(PVOID pData, size_t sizeData, CONST SOCKADDR * pScKAddrPeer = NULL, socklen_t
		PeerAddrLen = 0, INT iFlags = 0);
	virtual INT RecvFrom(PVOID pData, size_t sizeData, SOCKADDR * pScKAddrPeer = NULL, socklen_t
		* pPeerAddrLen = NULL, INT iFlags = 0);
	virtual INT setDelPathOnClose(BOOL_t bDel);
private:
	void CloseSocket();
protected:
	struct sockaddr_un m_sckaddrLocal;
	BOOL_t m_bDelPathOnClose;
};

#endif	//_UNIX_SOCKET_H_

