#ifndef	_UDP_SOCKET_H_
#define	_UDP_SOCKET_H_

#include <BaseTypeDef.h>
#include "Socket2.h"

class CUdpSocket : public CSocket2
{
public:
	CUdpSocket();
	~CUdpSocket();
	INT Create(LPCSTR pszAddrToBind = NULL, WORD usPortToBind = 0, BOOL bReuseAddr = FALSE);
	virtual INT Close();
	virtual INT JoinMulticastSession(UINT32 uiMulticastAddr);
	virtual INT LeaveMulticastSession(UINT32 uiMulticastAddr);
	virtual BOOL_t getJoinedMulticastSession();
	virtual INT setBroadcastFeature(BOOL_t bEnable);
private:
	BOOL_t m_bJoinedMulticastSession;
	UINT32_t m_uiMulticastSessionAddr;
};

#endif	//_UDP_SOCKET_H_

