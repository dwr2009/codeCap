#include	"Socket2.h"
#include	<BaseErrDef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include	<errno.h>
#include	<string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include	<math2.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory2.h>
//#include <stropts.h>
#include <sys/ioctl.h>

CSocket2::CSocket2()
{
	m_hSocket = INVALID_SOCKET;
	m_iCrtErrNo = ERROR_SUCCESS;
}

CSocket2::~CSocket2()
{
	INT iRet = ERROR_SUCCESS;
	
	if(INVALID_SOCKET != m_hSocket)
	{
		iRet = close(m_hSocket);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_hSocket = INVALID_SOCKET;
	}
}

INT CSocket2::Create(INT iSckType, LPCSTR pszAddrToBind/*NULL*/, WORD usPortToBind/*0*/,
	BOOL bReuseAddr/*FALSE*/)
{
	INT_t  iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	INT_t iNBIO;
	
	if(INVALID_SOCKET != m_hSocket)
	{
		iRet = close(m_hSocket);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_hSocket = INVALID_SOCKET;
	}

	do{
		//Create a socket
		m_hSocket = socket(PF_INET, iSckType, 0);
		if(INVALID_SOCKET == m_hSocket)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}


		//Bind it to the specified addr
		SOCKADDR_IN oSckAddrToBind;
		oSckAddrToBind.sin_family = PF_INET; 
		oSckAddrToBind.sin_port = htons(usPortToBind);
		if(pszAddrToBind)
		{
			oSckAddrToBind.sin_addr.s_addr = inet_addr(pszAddrToBind);
			if(INADDR_NONE == oSckAddrToBind.sin_addr.s_addr)
			{
				m_iCrtErrNo = 0;
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else
		{
			oSckAddrToBind.sin_addr.s_addr = INADDR_ANY;
		}

		if(bReuseAddr)
		{
			int iReuseAddr = 1;
			iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, &iReuseAddr, sizeof(iReuseAddr));
			if(0 != iRet)
			{
				m_iCrtErrNo = errno;
				iOutRet = ERROR_SOCKET_BIND_FAIL;
				break;
			}
		}
		iRet = bind(m_hSocket, (SOCKADDR *)(&oSckAddrToBind), sizeof(oSckAddrToBind));
		if(-1 == iRet)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_SOCKET_BIND_FAIL;
			break;
		}

		struct timeval valTimeout;
		valTimeout.tv_sec = 0; valTimeout.tv_usec = (130*1000);
		iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, &valTimeout, sizeof(valTimeout));
		if(SOCKET_ERROR == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		
		iNBIO = 0;
		iRet = ioctl(m_hSocket, FIONBIO, &iNBIO);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_IOCTL_FAILED;
		}
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::AttachSck(SOCKET sckNew)
{
	INT iOutRet = ERROR_SUCCESS;

	do{	
		if(0 > sckNew)
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = m_iLocalErrNo;
			break;
		}
		Close();

		m_hSocket = sckNew;
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::Close()
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0;

	if(INVALID_SOCKET != m_hSocket)
	{
		iRet = close(m_hSocket);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_hSocket = INVALID_SOCKET;
	}

	return iOutRet;
}

int CSocket2::GetLastCrtErrNo()
{
	return m_iCrtErrNo;
}

INT CSocket2::GetSockName(CString * pStrAddr, WORD * pusPort)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	char * pszRet = NULL;

	do{
		if(NULL == pStrAddr || NULL == pusPort)
		{
			m_iCrtErrNo = ERROR_SUCCESS;
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		SOCKADDR_IN oSckAddr;
		INT iLen = sizeof(oSckAddr);
		iRet = getsockname(m_hSocket, (SOCKADDR *)(&oSckAddr), (socklen_t *)(&iLen));
		if(-1 == iRet)
		{
			m_iCrtErrNo = errno;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_GETSOCKNAME_FAIL;
			break;
		}

		iRet = pStrAddr->SetStrLength(32);
		if(ERROR_SUCCESS != iRet)
		{
			m_iCrtErrNo = errno;
			iOutRet = iRet;
			break;
		}

		pszRet = inet_ntoa(oSckAddr.sin_addr);
		if(NULL == pszRet)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_CONVERT_IP_FAIL;
			break;
		}

		iLen = GetMinVal(pStrAddr->GetStrLength(), strlen(pszRet));
		strncpy((char *)((const char *)(*pStrAddr)), pszRet, iLen);
		((char *)((const char *)(*pStrAddr)))[iLen] = '\0';
		*pusPort = ntohs(oSckAddr.sin_port);
	}while(false);

	return iOutRet;
}

INT CSocket2::GetSockPort(unsigned short * pusPort)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		if(NULL == pusPort)
		{
			m_iCrtErrNo = ERROR_SUCCESS;
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		SOCKADDR_IN oSckAddr;
		INT iLen = sizeof(oSckAddr);
		iRet = getsockname(m_hSocket, (SOCKADDR *)(&oSckAddr), (socklen_t *)(&iLen));
		if(-1 == iRet)
		{
			m_iCrtErrNo = errno;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_GETSOCKNAME_FAIL;
			break;
		}

		*pusPort = ntohs(oSckAddr.sin_port);
	}while(false);

	return iOutRet;
}

INT CSocket2::SetSocketRecvBufSize(INT iRecvBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	int iBufSizeVal = iRecvBufSize;
	socklen_t ulParaLen = sizeof(iBufSizeVal);

	do{
		iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, &iBufSizeVal, ulParaLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SETSOCKOPT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::getSocketRecvBufSize(P_INT piRecvBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	socklen_t ulParaLen = sizeof(*piRecvBufSize);

	do{
		if(NULL == piRecvBufSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, piRecvBufSize, &ulParaLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SETSOCKOPT_FAIL;
			break;
		}		
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::setSocketSendBufSize(INT iSendBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	int iBufSizeVal = iSendBufSize;
	socklen_t ulParaLen = sizeof(iBufSizeVal);

	do{
		iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, &iBufSizeVal, ulParaLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SETSOCKOPT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::getSocketSendBufSize(P_INT piSendBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	socklen_t ulParaLen = sizeof(*piSendBufSize);

	do{
		if(NULL == piSendBufSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_SNDBUF, piSendBufSize, &ulParaLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SETSOCKOPT_FAIL;
			break;
		}		
	}while(FALSE);

	return iOutRet;
}

SOCKET CSocket2::GetSocketHandle()
{
	return m_hSocket;
}

INT CSocket2::SendTo(LPBYTE pSendBuf, size_t iBufSize, INT iFlags/*0*/, 
	P_SOCKADDR pSckAddrPeer/*NULL*/, socklen_t iToLen/*0*/)
{
	INT iOutRet = 0, iRet = 0;

	m_iCrtErrNo = ERROR_SUCCESS;
	m_iLocalErrNo = ERROR_SUCCESS;

	do
	{
		if(NULL == pSendBuf || 0 >= iBufSize) 
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = SOCKET_ERROR;
			break;
		}
		if(pSckAddrPeer && 0 >= iToLen) 
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = SOCKET_ERROR;
			break;
		}

		iRet = sendto(m_hSocket, pSendBuf, iBufSize, iFlags, pSckAddrPeer, iToLen);
		iOutRet = iRet;
	}while(FALSE);
		
	return iOutRet;
}

INT CSocket2::RecvFrom(LPBYTE pRecvBuf, size_t iBufSize, INT iFlags/*0*/, 
	P_SOCKADDR pSckAddrPeer/*NULL*/, socklen_t * piFromLen/*NULL*/, UINT uiTimeoutMS/*0*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	m_iCrtErrNo = ERROR_SUCCESS;
	m_iLocalErrNo = ERROR_SUCCESS;

	do{	
		if(NULL == pRecvBuf || 0 >= iBufSize) 
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = SOCKET_ERROR;
			break;
		}
		if(pSckAddrPeer && NULL == piFromLen) 
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = SOCKET_ERROR;
			break;
		}

		if(uiTimeoutMS)
		{
			fd_set fdsRead;
			struct timeval timeoutVal = {0, 0, };
			FD_ZERO(&fdsRead);
			FD_SET(m_hSocket, &fdsRead);
			timeoutVal.tv_sec = uiTimeoutMS / 1000; 
			timeoutVal.tv_usec = (uiTimeoutMS % 1000) * 1000;
			iRet = select(m_hSocket+1, &fdsRead, NULL, NULL, &timeoutVal);
			if(-1 == iRet)
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_CALL_SYSTEM_FAILED;
				iOutRet = SOCKET_ERROR;
				break;
			}
			else if(0 == iRet)
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_RECV_TIMEOUT;
				iOutRet = SOCKET_ERROR;
				break;
			}

			if(FALSE == FD_ISSET(m_hSocket, &fdsRead))
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_UNKNOWN;
				iOutRet = SOCKET_ERROR;
				break;
			}
		}

		iRet = recvfrom(m_hSocket, pRecvBuf, iBufSize, iFlags, pSckAddrPeer, piFromLen);
		iOutRet = iRet;
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::RecvMsg(struct msghdr * pMsgHdr, INT iFlags/* = 0*/, INT_t iTimeoutMS/* = 0*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	m_iCrtErrNo = ERROR_SUCCESS;
	m_iLocalErrNo = ERROR_SUCCESS;

	do{	
		if(NULL == pMsgHdr) 
		{
			m_iLocalErrNo = ERROR_INVALID_PARAMETER;
			iOutRet = SOCKET_ERROR;
			break;
		}

		if(0 < iTimeoutMS)
		{
			fd_set fdsRead;
			struct timeval timeoutVal = {0, 0,};
			FD_ZERO(&fdsRead);
			FD_SET(m_hSocket, &fdsRead);
			timeoutVal.tv_sec = iTimeoutMS / 1000; 
			timeoutVal.tv_usec = (iTimeoutMS % 1000) * 1000;
			iRet = select(m_hSocket+1, &fdsRead, NULL, NULL, &timeoutVal);
			if(-1 == iRet)
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_CALL_SYSTEM_FAILED;
				iOutRet = SOCKET_ERROR;
				break;
			}
			else if(0 == iRet)
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_RECV_TIMEOUT;
				iOutRet = SOCKET_ERROR;
				break;
			}

			if(FALSE == FD_ISSET(m_hSocket, &fdsRead))
			{
				m_iCrtErrNo = errno;
				m_iLocalErrNo = ERROR_UNKNOWN;
				iOutRet = SOCKET_ERROR;
				break;
			}
		}

		iRet = recvmsg(m_hSocket, pMsgHdr, iFlags);
		iOutRet = iRet;
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::GetLastLocalErrNo()
{
	return m_iLocalErrNo;
}

INT CSocket2::setSckRecvTimeout(INT_t iTimeoutMs)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	struct timeval valTimeout;

	do{
		ZeroMemory(&valTimeout, sizeof(valTimeout));	//no timeout
		if(0 < iTimeoutMs)
		{
			valTimeout.tv_sec = (iTimeoutMs / 1000); 
			valTimeout.tv_usec = ((iTimeoutMs % 1000) * 1000);
		}
		iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, &valTimeout, sizeof(valTimeout));
		if(SOCKET_ERROR == iRet)
		{
			iOutRet = ERR_SCK_SETOPT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CSocket2::getSckRecvTimeout(INT_t * piTimeoutMs)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	struct timeval valTimeout;
	socklen_t scklen = sizeof(valTimeout);
	
	do{
		ZeroMemory(&valTimeout, sizeof(valTimeout));	//no timeout
		iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, &valTimeout, &scklen);
		if(SOCKET_ERROR == iRet)
		{
			iOutRet = ERR_SCK_SETOPT_FAIL;
			break;
		}
		if(piTimeoutMs)
		{
			*piTimeoutMs = (valTimeout.tv_sec * 1000) + (valTimeout.tv_usec / 1000);
		}
	}while(FALSE);

	return iOutRet;
}

