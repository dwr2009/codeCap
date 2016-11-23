#include	"UdpSocket.h"
#include <arpa/inet.h>

CUdpSocket::CUdpSocket()
{
	m_bJoinedMulticastSession = FALSE;
	m_uiMulticastSessionAddr = 0;
}

CUdpSocket::~CUdpSocket()
{
	INT iRet = ERROR_SUCCESS;
	
	if(m_bJoinedMulticastSession)
	{
		iRet = LeaveMulticastSession(m_uiMulticastSessionAddr);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}
}

INT CUdpSocket::Create(LPCSTR pszAddrToBind/*NULL*/, unsigned short usPortToBind/*0*/,
	BOOL bReuseAddr/*FALSE*/)
{
	INT iOutRet = 0;

	do{		
		iOutRet = CSocket2::Create(SOCK_DGRAM, pszAddrToBind, usPortToBind, bReuseAddr);		
	}while(FALSE);

	return iOutRet;
}

INT CUdpSocket::Close() 
{
	INT iOutRet = 0, iRet = 0;

	if(m_bJoinedMulticastSession)
	{
		iRet = LeaveMulticastSession(m_uiMulticastSessionAddr);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	iRet = CSocket2::Close();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}

	return iOutRet;
}

INT CUdpSocket::JoinMulticastSession(UINT32 uiMulticastAddr)
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0;

	do{
		if(m_bJoinedMulticastSession)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		struct ip_mreq reqMulticast;

		memset(&reqMulticast, 0x00, sizeof(reqMulticast));

		reqMulticast.imr_multiaddr.s_addr = htonl(uiMulticastAddr);   
		if(INADDR_NONE == reqMulticast.imr_multiaddr.s_addr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}		
		reqMulticast.imr_interface.s_addr   =   INADDR_ANY;   

		iRet = setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &reqMulticast, sizeof(reqMulticast));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SOCKET_SETSOCKOPT_FAIL;
			break;
		}

		m_bJoinedMulticastSession = TRUE;
		m_uiMulticastSessionAddr = uiMulticastAddr;
	}while(FALSE);
	
	return iOutRet;
}

INT CUdpSocket::LeaveMulticastSession(UINT32 uiMulticastAddr)
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0;

	do{
		if(FALSE == m_bJoinedMulticastSession)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		struct ip_mreq reqMulticast;

		memset(&reqMulticast, 0x00, sizeof(reqMulticast));

		reqMulticast.imr_multiaddr.s_addr   =   htonl(uiMulticastAddr);   
		if(INADDR_NONE == reqMulticast.imr_multiaddr.s_addr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}		
		reqMulticast.imr_interface.s_addr   =   INADDR_ANY;   

		iRet = setsockopt(m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &reqMulticast, sizeof(reqMulticast));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SOCKET_SETSOCKOPT_FAIL;
			break;
		}

		m_bJoinedMulticastSession = FALSE;
		m_uiMulticastSessionAddr = 0;
#if	0
		PRINT_FILE_LINENO;
		printf("LeaveMulticastSession\n");
#endif		
	}while(FALSE);
	
	return iOutRet;
}

BOOL CUdpSocket::getJoinedMulticastSession()
{
	return m_bJoinedMulticastSession;
}

INT CUdpSocket::setBroadcastFeature(BOOL_t bEnable)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	int iOptVal = bEnable, iOptLen = sizeof(iOptVal);
	
	iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, &iOptVal, iOptLen);
	if(SOCKET_ERROR == iRet)
	{
		iOutRet = ERROR_SOCKET_IOCTL_FAIL;
	}		

	return iOutRet;
}

