#include <LinuxSocket.h>
#include <BaseErrDef.h>
#include <unistd.h>
#include <ErrPrintHelper.h>
#include <memory.h>

CLinuxSocket::CLinuxSocket()
{
	m_hSocket = INVALID_SOCKET;
}

CLinuxSocket::~CLinuxSocket()
{
	CloseSocket();
}

INT CLinuxSocket::setSocketSendBufSize(INT iSendBufSize)
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

INT CLinuxSocket::getSocketSendBufSize(P_INT piSendBufSize)
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
			iOutRet = ERR_SCK_GETOPT_FAIL;
			break;
		}		
	}while(FALSE);

	return iOutRet;
}

void CLinuxSocket::CloseSocket()
{
	INT iRet;
	
	if(INVALID_SOCKET != m_hSocket)
	{
		iRet = close(m_hSocket);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_hSocket = INVALID_SOCKET;
	}
}

SOCKET CLinuxSocket::getSocketHandle()
{
	return m_hSocket;
}

INT CLinuxSocket::Attach(SOCKET hSocket)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		CloseSocket();
		m_hSocket = hSocket;
	}while(FALSE);

	return iOutRet;
}

INT CLinuxSocket::Connect(SOCKADDR * pSckAddr, socklen_t sizeAddrLen)
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do{
		iRet = connect(m_hSocket, pSckAddr, sizeAddrLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SCK_CONNECT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CLinuxSocket::Send(PVOID pBuf, size_t sizeBuf, INT iFlags/* = 0*/, size_t * psizeSended/* = NULL*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	m_iLastCrtErrNo = ERROR_SUCCESS;
	
	do{
		if(NULL == pBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
CONTINUE_SEND:
		iRet = send(m_hSocket, pBuf, sizeBuf, iFlags);
		if(0 > iRet)
		{
			m_iLastCrtErrNo = errno;
			if(EINTR == m_iLastCrtErrNo)
			{				
				usleep(10*1000);
				goto CONTINUE_SEND;
			}
			else
			{
				iOutRet = ERROR_SOCKET_SEND_FAIL;
				break;
			}
		}		
		if(psizeSended)
		{
			*psizeSended = iRet;
		}
	}while(FALSE);

	return iOutRet;
}

INT CLinuxSocket::Receive(PVOID pBuf, size_t sizeBuf, INT iFlags, size_t * psizeRead,
	INT_t iTimeoutMs/* = -1*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet; 
	BOOL_t bNeedRestoreRecvTimeoutCfg = FALSE;
	INT_t iOldRecvTimeoutMs = 0;

	do{
		if(NULL == pBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 <= iTimeoutMs)
		{			
			iRet = getSckRecvTimeout(&iOldRecvTimeoutMs);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = setSckRecvTimeout(iTimeoutMs);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			bNeedRestoreRecvTimeoutCfg = TRUE;
		}
		iRet = recv(m_hSocket, pBuf, sizeBuf, iFlags);
		if(0 > iRet)
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				iOutRet = ERROR_RECV_TIMEOUT;
			}
			else
			{
				iOutRet = ERROR_SOCKET_RECV_FAIL;
			}
			break;
		}
		if(psizeRead)
		{
			*psizeRead = iRet;
		}
	}while(FALSE);

	if(bNeedRestoreRecvTimeoutCfg)
	{
		iRet = setSckRecvTimeout(iOldRecvTimeoutMs);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}

	return iOutRet;
}

inline int CLinuxSocket::getLastCrtErrNo()
{
	return m_iLastCrtErrNo;
}

INT CLinuxSocket::setSocketRecvBufSize(INT iSendBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	int iBufSizeVal = iSendBufSize;
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

INT CLinuxSocket::getSocketRecvBufSize(P_INT piSendBufSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	socklen_t ulParaLen = sizeof(*piSendBufSize);

	do{
		if(NULL == piSendBufSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, piSendBufSize, &ulParaLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_SCK_GETOPT_FAIL;
			break;
		}		
	}while(FALSE);

	return iOutRet;
}

INT CLinuxSocket::setSckRecvTimeout(INT_t iTimeoutMs)
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

INT CLinuxSocket::getSckRecvTimeout(INT_t * piTimeoutMs)
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

INT_t CLinuxSocket::setSckSendTimeout(INT_t iTimeoutMs)
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
		iRet = setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, &valTimeout, sizeof(valTimeout));
		if(SOCKET_ERROR == iRet)
		{
			iOutRet = ERR_SCK_SETOPT_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

