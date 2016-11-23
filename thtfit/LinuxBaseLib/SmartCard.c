#include <SmartCard.h>
#include <memory.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stropts.h>
#include <asm/tango3/scard.h>
#include <sys/select.h>

static BYTE g_abPingData[] = { 0x21, 0x00, 0x07, 0xf3, 0x68, 0xe7, 0xa7, 0x8b, 0xf5, 0xe9, 0x6a };
static BYTE g_abPongData[] = { 0x12, 0x00, 0x07, 0xf3, 0x68, 0xe7, 0xa7, 0x8b, 0xf5, 0xe9, 0x59 };

struct _SMART_CARD_INST
{
	int iSmartCardFd;
};

INT SmartCard_New(P_SMART_CARD_INST * ppSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	P_SMART_CARD_INST pSmartCardInst = NULL;

	do
	{
		if(NULL == ppSmartCardInst)
		{
			break;
		}
		*ppSmartCardInst = NULL;
		
		pSmartCardInst = malloc(sizeof(*pSmartCardInst));
		if(NULL == pSmartCardInst)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		ZeroMemory(pSmartCardInst, sizeof(*pSmartCardInst));
		pSmartCardInst->iSmartCardFd = INVALID_FILE_DESCRIPTOR;

		*ppSmartCardInst = pSmartCardInst;
		pSmartCardInst = NULL;	//detached
	}while(FALSE);

	if(pSmartCardInst)
	{
		iRet = SmartCard_Del(pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}	
		pSmartCardInst = NULL;
	}

	return iOutRet;
}

INT SmartCard_Open(P_SMART_CARD_INST pSmartCardInst,
	CONST P_SCARD_SEL_MATCH_PARAM pSCardSelMatchParam)
{
	INT iOutRet = ERROR_SUCCESS;
	CHAR szSCardDevFilePath[1024];
	
	do
	{
		if(NULL == pSCardSelMatchParam)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(INVALID_FILE_DESCRIPTOR != pSmartCardInst->iSmartCardFd)
		{
			iOutRet = ERR_ALREADY_OPENED;
			break;
		}
		if(pSCardSelMatchParam->bSCardInstId_Valid)
		{
			snprintf(szSCardDevFilePath, sizeof(szSCardDevFilePath), "/dev/scard%d", 
				pSCardSelMatchParam->iSCardInstId);
			szSCardDevFilePath[sizeof(szSCardDevFilePath)-1] = '\0';
			pSmartCardInst->iSmartCardFd = open(szSCardDevFilePath, O_RDWR);
			if(INVALID_FILE_DESCRIPTOR == pSmartCardInst->iSmartCardFd)
			{
				iOutRet = ERROR_FILE_OPEN_FAIL;
				break;
			}
			//set F_GETFD to the file descriptor flag
			{
				int iFileDescriptorFlag = fcntl(mMemDevFd, F_GETFD);
				if(-1 != iFileDescriptorFlag)
				{
					iFileDescriptorFlag |= FD_CLOEXEC;
					iRet = fcntl(m_LogFd, F_SETFD, iFileDescriptorFlag);
					if(-1 == iRet)
					{
						PRINT_BFILE_LINENO_CRT_ERRINFO;
					}
				}
				else
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
			}
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT SmartCard_PingPong(P_SMART_CARD_INST pSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BYTE abReadBuf[512];

	do
	{
		if(NULL == pSmartCardInst)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 > pSmartCardInst->iSmartCardFd)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = write(pSmartCardInst->iSmartCardFd, g_abPingData, sizeof(g_abPingData));
		if(sizeof(g_abPingData) != iRet)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
		iRet = read(pSmartCardInst->iSmartCardFd, abReadBuf, sizeof(g_abPongData));
		if(sizeof(g_abPongData) != iRet)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT SmartCard_ResetCard(P_SMART_CARD_INST pSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pSmartCardInst)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = ioctl(pSmartCardInst->iSmartCardFd, SCARD_IOC_WARMRESET);
		if(0 != iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT SmartCard_Write(P_SMART_CARD_INST pSmartCardInst, PBYTE pData, size_t sizeData)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pSmartCardInst)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(pData && 0 > sizeData)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(INVALID_FILE_DESCRIPTOR == pSmartCardInst->iSmartCardFd)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		iRet = write(pSmartCardInst->iSmartCardFd, pData, sizeData);
		if(sizeData != iRet || 0 > iRet)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT SmartCard_Read(P_SMART_CARD_INST pSmartCardInst, PBYTE pDataBuf, size_t sizeDataBuf,
	size_t * psizeRead)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	size_t sizeRead = 0;

	do
	{
		if(NULL == pSmartCardInst)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(pDataBuf && 0 > sizeDataBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == sizeDataBuf)
		{
			break;
		}
		if(INVALID_FILE_DESCRIPTOR == pSmartCardInst->iSmartCardFd)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		iRet = read(pSmartCardInst->iSmartCardFd, pDataBuf, sizeDataBuf);
		if(0 > iRet)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
		sizeRead = iRet;
	}while(FALSE);

	if(psizeRead)
	{
		*psizeRead = sizeRead;
	}
	
	return iOutRet;
}

INT SmartCard_WaitData(P_SMART_CARD_INST pSmartCardInst, INT iTimeoutMs)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	fd_set fdsRead;
	struct timeval tmTimeout, *ptmTimeout = NULL;

	ZeroMemory(&fdsRead, sizeof(fdsRead));

	do
	{
		if(NULL == pSmartCardInst)
		{
			 break;
		}
		if(INVALID_FILE_DESCRIPTOR == pSmartCardInst->iSmartCardFd)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		FD_ZERO(&fdsRead);
		FD_SET(pSmartCardInst->iSmartCardFd, &fdsRead);
		if(0 <= iTimeoutMs)
		{
			tmTimeout.tv_sec = iTimeoutMs / 1000;
			tmTimeout.tv_usec = (iTimeoutMs % 1000) * 1000;
			ptmTimeout = &tmTimeout;
		}
		else
		{
			ptmTimeout = NULL;
		}
		iRet = select(pSmartCardInst->iSmartCardFd + 1, &fdsRead, NULL, NULL, ptmTimeout);
		if(-1 == iRet || 0 > iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		else if(0 == iRet)	//timeout
		{
			iOutRet = ERR_TIMEOUT;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT SmartCard_ClearReadBuf(P_SMART_CARD_INST pSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BYTE szReadBuf[1024];

	do
	{
		if(NULL == pSmartCardInst)
		{
			 break;
		}
		
		iRet = SmartCard_WaitData(pSmartCardInst, 0);
		if(ERROR_SUCCESS != iRet && ERR_TIMEOUT != iRet)
		{
			iOutRet = iRet;
			break;
		}
		else if(ERR_TIMEOUT == iRet)
		{
			break;
		}

		iRet = read(pSmartCardInst->iSmartCardFd, szReadBuf, sizeof(szReadBuf));
		if(0 > iRet)
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT SmartCard_Close(P_SMART_CARD_INST pSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pSmartCardInst)
		{
			 break;
		}
		if(INVALID_FILE_DESCRIPTOR != pSmartCardInst->iSmartCardFd)
		{
			iRet = close(pSmartCardInst->iSmartCardFd);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}	
			pSmartCardInst->iSmartCardFd = INVALID_FILE_DESCRIPTOR;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT SmartCard_Del(P_SMART_CARD_INST pSmartCardInst)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pSmartCardInst)
		{
			 break;
		}
		iRet = SmartCard_Close(pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		free(pSmartCardInst);
		pSmartCardInst = NULL;
	}while(FALSE);

	return iOutRet;
}

