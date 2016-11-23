#include "UnixSocket.h"
#include <unistd.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <memory.h>
#include <pthread.h>

CUnixSocket::CUnixSocket()	
{
	m_hSocket = INVALID_SOCKET;
	ZeroMemory(&m_sckaddrLocal, sizeof(m_sckaddrLocal));
	m_bDelPathOnClose = TRUE;
}

CUnixSocket::~CUnixSocket()
{
	CloseSocket();
}

void CUnixSocket::CloseSocket()
{
	INT iRet;
	
	if(INVALID_SOCKET != m_hSocket)
	{
		struct sockaddr_un sckaddrLocal;
		socklen_t SckAddrLen;
		
		sckaddrLocal.sun_family = AF_UNIX;
		SckAddrLen = sizeof(sckaddrLocal);
		iRet = getsockname(m_hSocket, (SOCKADDR *)(&sckaddrLocal), &SckAddrLen);
		CLinuxSocket::CloseSocket();
		if(ERROR_SUCCESS == iRet)
		{
			if(m_bDelPathOnClose)
			{
				iRet = remove(sckaddrLocal.sun_path);
				if(ERROR_SUCCESS != iRet && ENOENT != errno)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
				}
			}
		}
		else
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
	}
}

INT CUnixSocket::Create(INT iType/*SOCK_DGRAM*/, LPCSTR pszPathToBind/* = NULL*/, BOOL_t bAutoGenePath/* = FALSE*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	struct sockaddr_un sckaddrLocal;
	socklen_t SckAddrLen;

	m_iLastCrtErrNo = 0;

	do{
		CloseSocket();

		m_hSocket = socket(AF_UNIX, iType, 0);
		if(0 > m_hSocket)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}

		ZeroMemory(&sckaddrLocal, sizeof(sckaddrLocal));
		sckaddrLocal.sun_family = AF_UNIX;
		if(bAutoGenePath)
		{
			snprintf(sckaddrLocal.sun_path, sizeof(sckaddrLocal.sun_path), "/tmp/P%uT%luS%u",
				getpid(), pthread_self(), m_hSocket);
			sckaddrLocal.sun_path[sizeof(sckaddrLocal.sun_path)-1] = '\0';
			iRet = bind(m_hSocket, (SOCKADDR *)(&sckaddrLocal), sizeof(sckaddrLocal));
		}
		else
		{
			if(pszPathToBind)
			{
				if('\0' == pszPathToBind[0])	//invisible path
				{
					sckaddrLocal.sun_path[0] = '\0';
					strncpy(&(sckaddrLocal.sun_path[1]), &(pszPathToBind[1]), sizeof(sckaddrLocal.sun_path)-1);
				}
				else	//visible path
				{
					strncpy(sckaddrLocal.sun_path, pszPathToBind, sizeof(sckaddrLocal.sun_path));
				}
				sckaddrLocal.sun_path[sizeof(sckaddrLocal.sun_path)-1] = '\0';
				iRet = bind(m_hSocket, (SOCKADDR *)(&sckaddrLocal), sizeof(sckaddrLocal));
				m_iLastCrtErrNo = errno;
				/*
				PRINT_FILE_LINENO;
				printf("iRet=%d, errno=%d\n", iRet, m_iLastCrtErrNo);
				*/
				//if(SOCKET_ERROR == iRet && E
			}
			else
			{
				//auto bind
				iRet = bind(m_hSocket, (SOCKADDR *)(&sckaddrLocal), sizeof(short));
			}
		}
		if(ERROR_SUCCESS != iRet)
		{
			m_iLastCrtErrNo = errno;
			iOutRet = ERR_SCK_BIND_FAIL;
			break;
		}
		
		m_sckaddrLocal.sun_family = AF_UNIX;
		SckAddrLen = sizeof(m_sckaddrLocal);
		iRet = getsockname(m_hSocket, (SOCKADDR *)(&m_sckaddrLocal), &SckAddrLen);
		if(ERROR_SUCCESS == iRet)
		{
			/*
			PRINT_FILE_LINENO;
			printf("PathBinded=%s\n", m_sckaddrLocal.sun_path);
			*/
		}
		else
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		/*
		{
			int iRecvBufSize = 0;
			socklen_t ulParaLen = sizeof(iRecvBufSize);
				
			iRet = getsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, &iRecvBufSize, &ulParaLen);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}	
			else
			{
				PRINT_FILE_LINENO;
				printf("RecvBuf=%d\n", iRecvBufSize);
			}
		}
		*/
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		CloseSocket();
	}

	return iOutRet;
}

INT CUnixSocket::Close()
{
	INT iOutRet = ERROR_SUCCESS;

	CloseSocket();

	return iOutRet;
}

INT CUnixSocket::Connect(LPCSTR pszUnixSckPath)
{
	INT iOutRet = ERROR_SUCCESS;
	struct sockaddr_un sckaddrPeer;
	socklen_t sizeAddrLen = sizeof(sckaddrPeer);

	do{
		if(NULL == pszUnixSckPath)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		ZeroMemory(&sckaddrPeer, sizeof(sckaddrPeer));
		sckaddrPeer.sun_family = AF_UNIX;
		if('\0' == sckaddrPeer.sun_path[0])	//invisible path
		{
			sckaddrPeer.sun_path[0] = '\0';
			strncpy(&(sckaddrPeer.sun_path[1]), &(pszUnixSckPath[1]), sizeof(sckaddrPeer.sun_path)-1);
		}
		else	//visible path
		{
			strncpy(sckaddrPeer.sun_path, pszUnixSckPath, sizeof(sckaddrPeer.sun_path));
		}
		sckaddrPeer.sun_path[sizeof(sckaddrPeer.sun_path)-1] = '\0';
		iOutRet = CLinuxSocket::Connect((SOCKADDR *)(&sckaddrPeer), sizeAddrLen);
	}while(FALSE);
	
	return iOutRet;
}

INT CUnixSocket::getSockName(struct sockaddr_un * psckaddrLocal)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == psckaddrLocal)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		*psckaddrLocal = m_sckaddrLocal;
	}while(FALSE);

	return iOutRet;
}

INT CUnixSocket::SendTo(PVOID pData, size_t sizeData, CONST SOCKADDR * pScKAddrPeer/* = NULL*/, socklen_t
	PeerAddrLen/* = 0*/, INT iFlags/* = 0*/)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		iOutRet = sendto(m_hSocket, pData, sizeData, iFlags, pScKAddrPeer, PeerAddrLen);
	}while(FALSE);

	return iOutRet;
}

INT CUnixSocket::RecvFrom(PVOID pData, size_t sizeData, SOCKADDR * pScKAddrPeer/* = NULL*/, socklen_t
	* pPeerAddrLen/* = NULL*/, INT iFlags/* = 0*/)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		iOutRet = recvfrom(m_hSocket, pData, sizeData, iFlags, pScKAddrPeer, pPeerAddrLen);
	}while(FALSE);

	return iOutRet;
}

INT CUnixSocket::setDelPathOnClose(BOOL_t bDel)
{
	m_bDelPathOnClose = bDel;
	
	return ERROR_SUCCESS;
}

