#include <MemoryMapFile.h>
#include <BaseErrDef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ErrPrintHelper.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <dirent2.h>
#include <memory.h>

CMemoryMapFile::CMemoryMapFile(LPCSTR pszFilePath, size_t FileSizeIfNeedCreate)
{
	int iRet;
	
	m_iMapFileFd = INVALID_FILE_DESCRIPTOR;
	m_pMappedAddr = NULL;
	m_iLastErrNo = ERROR_SUCCESS;
	m_bFileIsMapped = FALSE;
	m_FileSizeToMap = 0;
	m_bFileIsCreatedByMe = FALSE;

	do
	{
		if(NULL == pszFilePath || 0 >= FileSizeIfNeedCreate || MAX_SIZE_TO_MAP < FileSizeIfNeedCreate)
		{
			m_iLastErrNo = ERROR_INVALID_PARAMETER;
			break;
		}
		m_iMapFileFd = open(pszFilePath, O_RDWR);
		if(-1 == m_iMapFileFd && ENOENT == errno)
		{
			m_iMapFileFd = open(pszFilePath, O_RDWR | O_CREAT | O_EXCL);
			if(0 <= m_iMapFileFd)
			{
				m_bFileIsCreatedByMe = TRUE;
			}
			else if(-1 == m_iMapFileFd && EEXIST == errno)
			{
				m_iMapFileFd = open(pszFilePath, O_RDWR);
				if(0 > m_iMapFileFd)
				{
					m_iLastErrNo = ERROR_FILE_OPEN_FAIL;
					m_iMapFileFd = INVALID_FILE_DESCRIPTOR;
					break;
				}
			}
			else if(0 > m_iMapFileFd)
			{
				m_iLastErrNo = ERROR_FILE_OPEN_FAIL;
				m_iMapFileFd = INVALID_FILE_DESCRIPTOR;
				break;
			}
		}
		else if(0 > m_iMapFileFd)
		{
			m_iLastErrNo = ERROR_FILE_OPEN_FAIL;
			m_iMapFileFd = INVALID_FILE_DESCRIPTOR;
			break;
		}

		//set F_GETFD to the file descriptor flag
		{
			int iFileDescriptorFlag = fcntl(m_iMapFileFd, F_GETFD);
			if(-1 != iFileDescriptorFlag)
			{
				iFileDescriptorFlag |= FD_CLOEXEC;
				iRet = fcntl(m_iMapFileFd, F_SETFD, iFileDescriptorFlag);
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
		
		struct stat oFileStat;
		iRet = fstat(m_iMapFileFd, &oFileStat);
		if(0 != iRet)
		{
			m_iLastErrNo = ERROR_FILE_STAT_FAIL;
			break;
		}
		if(0 == oFileStat.st_size)
		{
			iRet = ftruncate(m_iMapFileFd, FileSizeIfNeedCreate);
			if(0 != iRet)
			{
				m_iLastErrNo = ERR_FILE_TRUNCATE_FAIL;
				break;
			}
		}
		m_FileSizeToMap = FileSizeIfNeedCreate;
	}while(FALSE);

	if(ERROR_SUCCESS != m_iLastErrNo)
	{
		CloseFile();
	}
}

CMemoryMapFile::~CMemoryMapFile()
{
	UnmapFile();
	CloseFile();
}

void * CMemoryMapFile::getMappedAddr()
{
	PVOID pOutMappedAddr = NULL;

	do
	{
		if(0 > m_iMapFileFd)
		{			
			break;
		}
		if(m_bFileIsMapped)
		{
			break;
		}
		m_bFileIsMapped = TRUE;
		m_pMappedAddr = mmap(0, m_FileSizeToMap, PROT_READ | PROT_WRITE, MAP_SHARED, m_iMapFileFd, 0);
		if(MAP_FAILED == m_pMappedAddr)
		{
			m_pMappedAddr = NULL;
			break;
		}
		if(m_bFileIsCreatedByMe)
		{
			ZeroMemory(m_pMappedAddr, m_FileSizeToMap);
		}
	}while(FALSE);

	pOutMappedAddr = m_pMappedAddr;
	
	return pOutMappedAddr;
}

VOID CMemoryMapFile::UnmapFile()
{
	int iRet;
	
	do
	{
		if(0 > m_iMapFileFd || NULL == m_pMappedAddr)
		{
			break;
		}
		iRet = munmap(m_pMappedAddr, m_FileSizeToMap);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(FALSE);
}

VOID CMemoryMapFile::CloseFile()
{
	int iRet;
	
	if(INVALID_FILE_DESCRIPTOR != m_iMapFileFd)
	{
		iRet = close(m_iMapFileFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_iMapFileFd = INVALID_FILE_DESCRIPTOR;
	}
}

INT_t CMemoryMapFile::Lock()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(0 > m_iMapFileFd)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = flock(m_iMapFileFd, LOCK_EX);
		if(0 != iRet)
		{
			iOutRet = ERR_MUTEX_LOCK_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CMemoryMapFile::Unlock()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(0 > m_iMapFileFd)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = flock(m_iMapFileFd, LOCK_UN);
		if(0 != iRet)
		{
			iOutRet = ERR_MUTEX_UNLOCK_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

BOOL_t CMemoryMapFile::FileIsCreatedByMe()
{
	return m_bFileIsCreatedByMe;
}

