#include	<ErrPrintHelper.h>
#include	<BaseErrDef.h>
#include	<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include	<errno.h>
#include	<string.h>
#include <unistd.h>
#include	<BaseTypeDef.h>
#include "GlobalShareMem.h"
#include <Compiler.h>
#include <memory.h>
#include <FileCreator.h>
#include <dirent2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

CGlobalShareMem::CGlobalShareMem()
{
	m_iShmId = -1;
	m_bRemoveShmIdIfCreatedByMe = TRUE;
	m_pShmAddr = NULL;
	m_bCreated = FALSE;
	m_iLastCrtErrNo = 0;
}

CGlobalShareMem::~CGlobalShareMem()
{
	DetachOrDestroyShareMem();
}

int CGlobalShareMem::GetLastCrtErrNo()
{
	return m_iLastCrtErrNo;
}

int CGlobalShareMem::DetachOrDestroyShareMem()
{
	int iOutRet = 0, iRet = 0;
	
	if(-1 != m_iShmId)
	{
		if(m_pShmAddr)
		{
			iRet = shmdt(m_pShmAddr);
			if(-1 == iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			m_pShmAddr = NULL;
		}
		
		if(m_bCreated)
		{
			if(m_bRemoveShmIdIfCreatedByMe)
			{
				iRet = shmctl(m_iShmId, IPC_RMID, NULL);
				if(-1 == iRet && EINVAL != errno)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
				}
			}
			m_bCreated = FALSE;
		}
		
		m_iShmId = -1;
	}

	return iOutRet;
}

int CGlobalShareMem::CreateOrGetShareMem(key_t iKey, int iSize)
{
	int iOutRet = 0, iRet = 0, iShmSize = 0;

	do{
		if(0 >= iSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		int iPageSize = getpagesize();
		//printf("iPageSize=%d\n", iPageSize);
		iShmSize = (iSize+iPageSize-1)/iPageSize*iPageSize;
		//printf("iShmSize=%d\n", iShmSize);

		//attempt to create a share mem
		iRet = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
		m_bCreated = true;
		m_iLastCrtErrNo = errno;
		if(-1 == iRet && EEXIST == m_iLastCrtErrNo)
		{
			m_bCreated = false;
			iRet = shmget(iKey, iShmSize, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
			if(-1 == iRet && EEXIST == errno)
			{				
				m_iLastCrtErrNo = errno;
				iOutRet = ERROR_CALL_SYSTEM_FAILED;
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;			
			}
			else if(-1 == iRet && EINVAL)	//maybe iShmSize is too large
			{
				iRet = shmget(iKey, 4/*size*/, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
				if(0 > iRet)
				{
					m_iLastCrtErrNo = errno;
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = ERROR_CALL_SYSTEM_FAILED;
					break;
				}
				int iOldShmId = iRet;
				iRet = shmctl(iOldShmId, IPC_RMID, NULL);
				if(-1 == iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
				}
				iRet = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
				if(0 > iRet)
				{
					m_iLastCrtErrNo = errno;
					iOutRet = ERROR_CALL_SYSTEM_FAILED;
					break;
				}
				m_bCreated = true;
				m_iLastCrtErrNo = errno;
				if(-1 == iRet && EEXIST == m_iLastCrtErrNo)
				{
					m_bCreated = false;
					iRet = shmget(iKey, iShmSize, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
					if(-1 == iRet && EEXIST == errno)
					{				
						m_iLastCrtErrNo = errno;
						iOutRet = ERROR_CALL_SYSTEM_FAILED;
						PRINT_FILE_LINENO_CRT_ERRINFO;
						break;			
					}
					else if(-1 == iRet)
					{
						m_iLastCrtErrNo = errno;
						iOutRet = ERROR_NOT_FOUND;
						PRINT_FILE_LINENO_CRT_ERRINFO;
						break;			
					}
				}
			}
			else if(-1 == iRet)
			{
				m_iLastCrtErrNo = errno;
				iOutRet = ERROR_NOT_FOUND;
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;			
			}
		}
		else if(-1 == iRet)
		{
			m_bCreated = false;
			iOutRet = ERROR_CALL_SYSTEM_FAILED;
			break;
		}

		m_iShmId = iRet;

		m_pShmAddr = shmat(m_iShmId, NULL, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
		if(-1 == int(m_pShmAddr))
		{
			m_pShmAddr = NULL;
			iOutRet = ERROR_CALL_SYSTEM_FAILED;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(false);

	return iOutRet;
}

int CGlobalShareMem::CreateOrGetShareMem(LPCSTR pszKeyFilePath, int iSize)
{
	int iOutRet = 0, iRet;
	key_t iKey = 0;

	do{
		if(NULL == pszKeyFilePath || 0 >= iSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iKey = ftok(pszKeyFilePath, 0xFFFFFFFF);
		if(-1 == iKey)
		{
			//LOG_BLINE("errno=%d\n", errno);
			if(ENOENT == errno)
			{
				int iFd = INVALID_FILE_DESCRIPTOR;
				iFd = open(pszKeyFilePath, O_RDONLY | O_CREAT);
				if(-1 == iFd)
				{
					iOutRet = ERROR_FILE_OPEN_FAIL;
					m_iLastCrtErrNo = errno;
					break;
				}
				iRet = close(iFd);
				if(ERROR_SUCCESS != iRet)
				{
					m_iLastCrtErrNo = errno;
					iOutRet = ERR_FILE_CLOSE_FAIL;
					break;
				}
				iKey = ftok(pszKeyFilePath, 0xFFFFFFFF);
				if(-1 == iKey)
				{
					iOutRet = ERROR_FILE_STAT_FAIL;
					break;
				}
			}
		}

		DetachOrDestroyShareMem();

		iOutRet = CreateOrGetShareMem(iKey, iSize);
	}while(false);

	if(ERROR_SUCCESS != iOutRet)
	{
		DetachOrDestroyShareMem();
	}

	return iOutRet;
}

INT CGlobalShareMem::CreateOrGetShareMem(CONST P_IPC_KEY_VAL_IN pIpcKeyValIn, CONST INT iSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet, bExist;
	key_t iKey = 0;

	do{
		if(CC_UNLIKELY(NULL == pIpcKeyValIn || 0 >= iSize))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(CC_UNLIKELY(NULL == pIpcKeyValIn->pszFilePath || 0 == pIpcKeyValIn->iId || 
			0xFF < pIpcKeyValIn->iId))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bExist = FALSE;
		iRet = ExistFile(pIpcKeyValIn->pszFilePath, &bExist);
		if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
		{
			iOutRet = iRet;
			break;
		}
		if(CC_LIKELY(FALSE == bExist))
		{
			BYTE abData[1];
			iRet = BufferDataToFile(pIpcKeyValIn->pszFilePath, abData, 0);
			if(CC_UNLIKELY(ERROR_SUCCESS != iRet))
			{
				iOutRet = iRet;
				break;
			}
		}

		iKey = ftok(pIpcKeyValIn->pszFilePath, pIpcKeyValIn->iId);
		if(-1 == iKey)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;
			break;
		}

		/*
		PRINT_FILE_LINENO;
		printf("Key:0x%08x, Path:%s, Id:%d\n", iKey, pIpcKeyValIn->pszFilePath, pIpcKeyValIn->iId);
		*/
		
		DetachOrDestroyShareMem();
		
		iOutRet = CreateOrGetShareMem(iKey, iSize);
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		DetachOrDestroyShareMem();
	}

	return iOutRet;
}

void * CGlobalShareMem::GetShareMemAddr()
{
	return m_pShmAddr;
}

bool CGlobalShareMem::GetIsCreated()
{
	return m_bCreated;
}

INT_t CGlobalShareMem::setRemoveShmIdIfCreatedByMe(CONST BOOL_t bNeedRemove)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_bRemoveShmIdIfCreatedByMe = bNeedRemove;
	}while(FALSE);

	return iOutRet;
}

