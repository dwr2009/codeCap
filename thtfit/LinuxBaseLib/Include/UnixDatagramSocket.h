#ifndef	_UNIX_DATAGRAM_SOCKET_H_
#define	_UNIX_DATAGRAM_SOCKET_H_

#include <UnixSocket.h>

class CUnixDatagramSocket : public CUnixSocket
{
public:
	CUnixDatagramSocket();
	virtual ~CUnixDatagramSocket();
	virtual INT Create(LPCSTR pszPathToBind = NULL, BOOL_t bAutoGenePath = FALSE);
};

#endif	//_UNIX_DATAGRAM_SOCKET_H_

