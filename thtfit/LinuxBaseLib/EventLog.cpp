#include <unistd.h>
#include <EventLog.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <dirent2.h>
#include <StackBufString.h>
#include <stdlib.h>
#include <EventLogExt.h>
#include <Compiler.h>

INT_t CEventLog::InitVal()
{
	m_LogFd = -1;
	m_bStopEvtLog = FALSE;
	m_PreBlankSize = 0;
	m_iLastErrNo = ERROR_SUCCESS;
	return ERROR_SUCCESS;
}

INT_t CEventLog::OpenEventLogFile()
{
	INT_t iOutRet = ERROR_SUCCESS;
	int iRet;

	do{
		if(FALSE == IsEventLogFileExist())
		{
			m_LogFd = open(EVENT_LOG_FILE_PATH,O_CREAT|O_RDWR , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
			if(m_LogFd < 0)
			{
				iOutRet = ERROR_FILE_OPEN_FAIL;
				break;
			}
		}
		else
		{
			m_LogFd = open(EVENT_LOG_FILE_PATH,O_APPEND|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
			if(m_LogFd < 0)
			{
				iOutRet = ERROR_FILE_OPEN_FAIL;
				break;
			}
		}

		//set F_GETFD to the file descriptor flag
		{
			int iFileDescriptorFlag = fcntl(m_LogFd, F_GETFD);
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
	}while(FALSE);

	return iOutRet;
}

INT_t CEventLog::NotifyEvtLog(BOOL_t bStopWEvt)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	BOOL_t bLocked = FALSE;
	
	do{
		if(FALSE == bLocked)
		{
			iRet = m_FileLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		m_bStopEvtLog = bStopWEvt;
		if(m_LogFd > 0)
		{
			close(m_LogFd);
			m_LogFd = -1;
		}
		
	}while(FALSE);
	
	if(TRUE == bLocked)
	{
		iRet = m_FileLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	return iOutRet;
}

BOOL_t CEventLog::IsEventLogFileExist()
{
	if(access(EVENT_LOG_FILE_PATH,F_OK))
	{
		return FALSE;
	}
	else {return TRUE;}
}


INT_t CEventLog::ReInitEvtLog()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	BOOL_t bLocked = FALSE;
	
	do{
		if(FALSE == bLocked)
		{
			iRet = m_FileLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		if(m_LogFd > 0)
		{
			close(m_LogFd);
			m_LogFd = -1;
		}

		if(TRUE == IsEventLogFileExist())
		{
			iRet = unlink(EVENT_LOG_FILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		iRet = InitVal();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = OpenEventLogFile();
		if(iRet < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
	}while(FALSE);

	if(TRUE == bLocked)
	{
		iRet = m_FileLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

INT_t CEventLog::GetLogFileSize(UINT_t *pFileSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	off_t llFileLen = 0;

	C_UNUSED(iRet);
	
	do
	{
		if(CC_UNLIKELY(NULL == pFileSize))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(CC_UNLIKELY(0 > m_LogFd))
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		llFileLen = lseek(m_LogFd, 0, SEEK_END);
		if(CC_UNLIKELY(llFileLen < 0))
		{
			iOutRet = ERROR_FILE_SEEK_FAIL;
			break;
		}

		*pFileSize = llFileLen;
	}while(FALSE);

	return iOutRet;
}

CEventLog::CEventLog():m_FileLocker("/tmp/EventLogLocker")
{
	BOOL_t bLocked = FALSE;
	INT_t iRet;
	
	do{		
		if(ERROR_SUCCESS != m_FileLocker.GetLastErrNo() 
			|| ERROR_SUCCESS != m_FileLocker.GetLastCrtErrNo())
		{
			m_iLastErrNo = m_FileLocker.GetLastErrNo();
			LOG_BLINE("Err=%d,CrtErr=%d\n", m_FileLocker.GetLastErrNo(), m_FileLocker.GetLastCrtErrNo());
		}
		
		if(FALSE == bLocked)
		{
			iRet = m_FileLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		//del OLD_EVENT_LOG_FILE_PATH1 and OLD_EVENT_LOG_FILE_PATH2 because we are using the new logging solution
		iRet = remove(OLD_EVENT_LOG_FILE_PATH1);
		if(ERROR_SUCCESS != iRet && ENOENT != errno)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iRet = remove(OLD_EVENT_LOG_FILE_PATH2);
		if(ERROR_SUCCESS != iRet && ENOENT != errno)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		
		iRet = InitVal();
		if(ERROR_SUCCESS != iRet)
		{
			m_iLastErrNo = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = OpenEventLogFile();
		if(ERROR_SUCCESS != iRet)
		{
			m_iLastErrNo = ERROR_FILE_OPEN_FAIL;
			break;
		}
	}while(FALSE);

	if(TRUE == bLocked)
	{
		iRet = m_FileLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

CEventLog::~CEventLog()
{
	BOOL_t bLocked = FALSE;
	INT_t iRet;
	
	if(FALSE == bLocked)
	{
		iRet = m_FileLocker.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	if(m_LogFd > 0)
	{
		close(m_LogFd);
		m_LogFd = -1;
	}

	if(TRUE == bLocked)
	{
		iRet = m_FileLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

INT_t CEventLog::GetLastErrNo()
{
	return m_iLastErrNo;
}

INT_t CEventLog::CheckLogDirFreeSize(UINT_t *pFreeSizeMB)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	 struct statfs dataDir; 
	 UINT_t freeSize = 0;

	do{
		if(NULL == pFreeSizeMB)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = statfs("/data",&dataDir);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;
			break;
		}
		
		freeSize = (dataDir.f_bsize * dataDir.f_bavail)/MBYTE_TO_BYTE;
		if(NULL != pFreeSizeMB)
		{
			*pFreeSizeMB = freeSize;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CEventLog::LostHalfEvtLogFile(UINT_t LogFileSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	off_t iOffset = 0;
	BYTE cBuf = 0;
	INT_t bakFileFD = -1;
	BYTE staticBuf[STATIC_MAX_BUF_SIZE] = {0};
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	do{
		if(m_LogFd < 0 || 0 == LogFileSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iOffset = LogFileSize/2;
		iRet = lseek(m_LogFd,iOffset,SEEK_SET);
		if(iRet < 0)
		{
			iOutRet = ERROR_FILE_SEEK_FAIL;
			break;
		}
		//get next line pos
		do
		{
			iRet = read(m_LogFd,&cBuf,sizeof(cBuf));
			if(iRet != sizeof(cBuf))
			{
				iOutRet =ERROR_FILE_READ_FAIL;
				break;
			}
		}while(cBuf != '\n');

		bakFileFD = open(EVT_LOG_FILE_BAKE_PATH,O_CREAT|O_WRONLY , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
		if(bakFileFD < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		//¡Áa¡ä????t
		while(TRUE)
		{
			iRet = read(m_LogFd,staticBuf,sizeof(staticBuf));
			if(iRet > 0)
			{
				write(bakFileFD,staticBuf,iRet);
			}
			else{break;}
		}

		if(bakFileFD > 0)
		{
			close(bakFileFD);
			bakFileFD = -1;
		}

		if(m_LogFd > 0)
		{
			close(m_LogFd);
			m_LogFd = -1;
		}

		strShellCmd.Format("cp %s %s",EVT_LOG_FILE_BAKE_PATH,EVENT_LOG_FILE_PATH);
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = InitVal();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = OpenEventLogFile();
		if(iRet < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CEventLog::InverEscapeSequence(LPCSTR pInStr, UINT32_t inSize, LPSTR pOutStr, UINT32_t outSize,
	OUT size_t & BytesWrittenWithNull)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT32_t i = 0;

	BytesWrittenWithNull = 0;

	do{
		if(CC_UNLIKELY(NULL == pInStr || NULL == pOutStr))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 >= inSize)
		{
			break;
		}
		while(*pInStr != '\0')
		{
			if(*pInStr == '\t')
			{
				if(2 > (int)((outSize - 1) - i - 1))
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
				pOutStr[i++] = '\\';
				pOutStr[i++] = '\t';
			}
			else if(*pInStr == '\r')
			{
				if(2 > (int)((outSize - 1) - i - 1))
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
				pOutStr[i++] = '\\';
				pOutStr[i++] = '\r';
			}
			else if(*pInStr == '\\')
			{
				if(2 > (int)((outSize - 1) - i - 1))
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
				pOutStr[i++] = '\\';
				pOutStr[i++] = '\\';
			}
			else if(*pInStr == '\n')
			{
				if(2 > (int)((outSize - 1) - i - 1))
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
				pOutStr[i++] = '\\';
				pOutStr[i++] = '\n';
			}
			else
			{
				if(1 > (int)((outSize - 1) - i - 1))
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
				pOutStr[i++] = *pInStr;
			}

			pInStr++;
		}		
	}while(FALSE);

	if(pOutStr)
	{
		pOutStr[i] = '\0';
		i++;
		BytesWrittenWithNull = i;
	}

	return iOutRet;
}

INT_t CEventLog::WEventLog(CString::STD_STRING_LIST & wStrList)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	UINT_t LogFileSize = 0;
	CString::STD_STRING_LIST::iterator CstrIndex;
	UINT_t strLen = 0;
	char tmp[2*1024];

	do{
		if(FALSE == bLocked)
		{
			iRet = m_FileLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		if(TRUE == m_bStopEvtLog)	
		{
			iOutRet = ERR_WRITE_LOG_PENGDING;
			break;
		}
			
		//Get yet opened file size 
		iRet = GetLogFileSize(&LogFileSize);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if((LogFileSize -  m_PreBlankSize) >= FILE_SIZE_BLANKING)
		{
			UINT_t mediaFreeSizeMB = 0;
			
			iRet = CheckLogDirFreeSize(&mediaFreeSizeMB);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			if(mediaFreeSizeMB <= LOG_DIR_MIN_FREE_SIZE_MB || (LogFileSize/(1024*1024)) >= LOG_FILE_MAX_SIZE_MB)
			{
				iRet = LostHalfEvtLogFile(LogFileSize);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				m_PreBlankSize = LogFileSize/2;
			}
			else
			{
				m_PreBlankSize = LogFileSize;
			}
		}
		
		if(m_LogFd < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		iRet = lseek(m_LogFd, 0, SEEK_END);
		if(iRet < 0)
		{
			LOG_BLINE("SeekToEnd err %s\n", strerror(errno));
		}
		
		for(CstrIndex = wStrList.begin(); CstrIndex != wStrList.end(); CstrIndex++)
		{
			CString tempStr = *CstrIndex;
			size_t BytesWrittenWithNull = 0, OutStrLen;
			strLen = (*CstrIndex).GetStrLength();
			iRet = InverEscapeSequence((LPCSTR)tempStr, (UINT32_t)strLen, tmp, sizeof(tmp), OUT BytesWrittenWithNull);
			tmp[sizeof(tmp)-1] = '\0';
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(0 < BytesWrittenWithNull)
			{
				OutStrLen = BytesWrittenWithNull - 1;
			}
			else
			{
				OutStrLen = 0;
			}
			if(0 < OutStrLen)
			{
				iRet = write(m_LogFd, tmp, OutStrLen);
				if(iRet != (typeof(iRet))OutStrLen)
				{
					LOG_BLINE("WrFile err,Req=%d,Written=%d\n", OutStrLen, iRet);
					iOutRet = ERROR_FILE_WRITE_FAIL;
					break;
				}
			}
			iRet = write(m_LogFd,"\t",1);
			if(iRet != 1)
			{
				LOG_BLINE("WrFile err,Req=%d,Written=%d\n", 1, iRet);
				iOutRet = ERROR_FILE_WRITE_FAIL;
				break;
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		write(m_LogFd,"\n",1);
		if(iRet != 1)
		{
			LOG_BLINE("WrFile err,Req=%d,Written=%d\n", 1, iRet);
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);
	
	if(TRUE == bLocked)
	{
		iRet = m_FileLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	return iOutRet;
}	

static CEventLog *g_pCEventLog = NULL;

INT_t UnInitEventLog(VOID)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == g_pCEventLog)
		{
			break;
		}
		delete g_pCEventLog;
		g_pCEventLog = NULL;
	}while(FALSE);

	return iOutRet;
}

INT_t InitEventLog(VOID)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL != g_pCEventLog)
		{
			iOutRet = ERR_ALREADY_INITIALIZED;
			break;
		}

		g_pCEventLog = new CEventLog();
		if(NULL == g_pCEventLog)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iOutRet = g_pCEventLog->GetLastErrNo();
		
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		iRet = UnInitEventLog();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	return iOutRet;
}

INT_t WriteEventLog(CString::STD_STRING_LIST& StrList)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == g_pCEventLog)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = g_pCEventLog->WEventLog(StrList);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);
	
	return iOutRet;
}

INT_t StopWriteEventLog(VOID)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == g_pCEventLog)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = g_pCEventLog->NotifyEvtLog(TRUE);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);
	
	return iOutRet;
}

INT_t ReInitEventLog(VOID)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == g_pCEventLog)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = g_pCEventLog->ReInitEvtLog();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

