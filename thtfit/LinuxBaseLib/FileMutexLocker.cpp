#include "FileMutexLocker.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <dirent2.h>

CFileMutexLocker::CFileMutexLocker(LPCSTR pszLockerFile)
{
	INT iRet = 0;
	BOOL_t bExist = FALSE;
	
	m_iFdLock = -1;
	m_iLastErrNo = 0;
	m_iLastCrtErrNo = 0;
	m_bCreatedLockerFile = FALSE;
	m_pidHold = 0;
	m_uiLockCount = 0;

	do
	{
		if(NULL == pszLockerFile)
		{
			m_iLastCrtErrNo = 0;
			m_iLastErrNo = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = ExistFile(pszLockerFile, &bExist);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			bExist = FALSE;
		}

		m_iFdLock = open(pszLockerFile, O_RDONLY | O_CREAT);
		if(-1 == m_iFdLock)
		{
			m_iLastErrNo = ERROR_FILE_OPEN_FAIL;
			m_iLastCrtErrNo = errno;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		//set F_GETFD to the file descriptor flag
		{
			int iFileDescriptorFlag = fcntl(m_iFdLock, F_GETFD);
			if(-1 != iFileDescriptorFlag)
			{
				iFileDescriptorFlag |= FD_CLOEXEC;
				iRet = fcntl(m_iFdLock, F_SETFD, iFileDescriptorFlag);
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
		if(FALSE == bExist)
		{
			m_bCreatedLockerFile = TRUE;
		}
		m_strLockerFilePath = pszLockerFile;
	}while(FALSE);
}

CFileMutexLocker::~CFileMutexLocker()
{
	INT iRet = 0;
	
	if(-1 != m_iFdLock)
	{
		if(-1 == close(m_iFdLock))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_iFdLock = -1;
	}

	if(m_bCreatedLockerFile)
	{
		m_bCreatedLockerFile = FALSE;
		iRet = remove((LPCSTR)m_strLockerFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
}

int CFileMutexLocker::Lock()
{
	int iRet = 0, iOutRet = 0;

	if(getpid() == m_pidHold)	//held already
	{
		m_uiLockCount++;
	}
	else
	{
		iRet = flock(m_iFdLock, LOCK_EX);
		if(ERROR_SUCCESS == iRet)
		{
			m_pidHold = getpid();
			m_uiLockCount++;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			m_iLastCrtErrNo = errno;
			m_iLastErrNo = ERROR_CALL_SYSTEM_FAILED;
			iOutRet = m_iLastErrNo;
		}
	}

	return iOutRet;
}

int CFileMutexLocker::Unlock()
{
	int iRet = 0, iOutRet = 0;

	do
	{
		if(getpid() == m_pidHold)	//held already
		{
			m_uiLockCount--;
		}
		else
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(0 == m_uiLockCount)
		{
			m_pidHold = 0;
			iRet = flock(m_iFdLock, LOCK_UN);
			if(ERROR_SUCCESS == iRet)
			{
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				m_iLastCrtErrNo = errno;
				m_iLastErrNo = ERROR_CALL_SYSTEM_FAILED;
				iOutRet = m_iLastErrNo;
			}
		}
	}while(FALSE);

	return iOutRet;
}

int CFileMutexLocker::GetLastErrNo()
{
	return m_iLastErrNo;
}

int CFileMutexLocker::GetLastCrtErrNo()
{
	return m_iLastCrtErrNo;
}

