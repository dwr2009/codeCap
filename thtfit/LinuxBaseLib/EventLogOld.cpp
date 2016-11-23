#include <unistd.h>
#include <EventLog.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent2.h>

INT_t CEventLog::InitVal()
{
	m_LogFd = -1;
	m_bStopEvtLog = FALSE;
	m_iLastErrNo = ERROR_SUCCESS;
	m_bFirstLogFile = FALSE;
	return ERROR_SUCCESS;
}

INT_t CEventLog::OpenEventLogFile(BOOL_t reCreate)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do{		
		if(FALSE == IsFirstLogFile() 
			&& FALSE == IsSecondLogFile())
		{
			m_LogFd = open(EVENT_LOG_FILE_PATH1,O_CREAT|O_WRONLY , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
			if(m_LogFd < 0)
			{
				iOutRet = ERROR_FILE_OPEN_FAIL;
				break;
			}
			m_bFirstLogFile = TRUE;
		}
		else if(TRUE == IsFirstLogFile() 
			&& FALSE == IsSecondLogFile())
		{
			if(TRUE == reCreate)
			{
				m_LogFd = open(EVENT_LOG_FILE_PATH2,O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
				if(m_LogFd < 0)
				{
					iOutRet = ERROR_FILE_OPEN_FAIL;
					break;
				}
				m_bFirstLogFile = FALSE;
			}
			else
			{
				m_LogFd = open(EVENT_LOG_FILE_PATH1,O_CREAT|O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
				if(m_LogFd < 0)
				{
					iOutRet = ERROR_FILE_OPEN_FAIL;
					break;
				}
				m_bFirstLogFile = TRUE;
			}
			
		}
		else if(TRUE == IsFirstLogFile()
			&& TRUE == IsSecondLogFile())
		{
			if(TRUE == reCreate)
			{
				if(TRUE == m_bFirstLogFile)
				{
					unlink(EVENT_LOG_FILE_PATH2);
					m_LogFd = open(EVENT_LOG_FILE_PATH2,O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
					if(m_LogFd < 0)
					{
						iOutRet = ERROR_FILE_OPEN_FAIL;
						break;
					}
					m_bFirstLogFile = FALSE;
				}
				else
				{
					unlink(EVENT_LOG_FILE_PATH1);
					m_LogFd = open(EVENT_LOG_FILE_PATH1,O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
					if(m_LogFd < 0)
					{
						iOutRet = ERROR_FILE_OPEN_FAIL;
						break;
					}
					m_bFirstLogFile = TRUE;
				}
			}
			else
			{
				off_t lFileSize1 = 0, lFileSize2 = 0;
				iRet = GetFileLength(EVENT_LOG_FILE_PATH1,&lFileSize1);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = ERROR_FILE_STAT_FAIL;
					break;
				}

				iRet = GetFileLength(EVENT_LOG_FILE_PATH2,&lFileSize2);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = ERROR_FILE_STAT_FAIL;
					break;
				}

				if(lFileSize1 > lFileSize2)
				{
					m_LogFd = open(EVENT_LOG_FILE_PATH2,O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
					if(m_LogFd < 0)
					{
						iOutRet = ERROR_FILE_OPEN_FAIL;
						break;
					}
					m_bFirstLogFile = FALSE;
				}
				else
				{
					m_LogFd = open(EVENT_LOG_FILE_PATH1,O_APPEND|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
					if(m_LogFd < 0)
					{
						iOutRet = ERROR_FILE_OPEN_FAIL;
						break;
					}
					m_bFirstLogFile = TRUE;
				}
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
			sync();
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
			sync();
			close(m_LogFd);
			m_LogFd = -1;
		}
		
		iRet = InitVal();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = OpenEventLogFile(FALSE);
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

BOOL_t CEventLog::IsFirstLogFile()
{
	if(access(EVENT_LOG_FILE_PATH1,F_OK))
	{
		return FALSE;
	}
	else {return TRUE;}
}

BOOL_t CEventLog::IsSecondLogFile()
{
	if(access(EVENT_LOG_FILE_PATH2,F_OK))
	{
		return FALSE;
	}
	else {return TRUE;}
}

INT_t CEventLog::GetLogFileSize(UINT_t *pFileSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	off_t llFileLen = 0;
	
	do{
		if(NULL == pFileSize || m_LogFd < 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = lseek(m_LogFd,0,SEEK_SET);
		if(iRet < 0)
		{
			iOutRet = ERROR_FILE_SEEK_FAIL;
			break;
		}
		
		llFileLen = lseek(m_LogFd, 0, SEEK_END);
		if(llFileLen < 0)
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
		
		if(ERROR_SUCCESS != m_iLastErrNo)
		{
			//LOG_BLINE("m_iLastErrNo : %d\n",m_iLastErrNo);
			//break;
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
		
		InitVal();

		iRet = OpenEventLogFile(FALSE);
		if(iRet < 0)
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
		sync();
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

INT_t CEventLog::WEventLog(TpyeOfEventLog LogType,void* pLogData,UINT_t LogDataSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	UINT_t LogFileSize = 0;

	do{
		if(TYPE_LOG_UNKNOW == LogType ||NULL == pLogData ||0 == LogDataSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
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
		if(TRUE == m_bStopEvtLog)	
		{break;}
			
		//Get yet opened file size 
		iRet = GetLogFileSize(&LogFileSize);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		if((LogFileSize + LogDataSize) > MAX_SIZE_OF_EVENTLOG_FILE)
		{
			if(m_LogFd > 0)
			{
				sync();
				close(m_LogFd);
				m_LogFd = -1;
			}
			
			m_LogFd = OpenEventLogFile(TRUE);
		}
		
		if(m_LogFd < 0)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		iRet = write(m_LogFd,pLogData,LogDataSize);
		if(iRet != LogDataSize)
		{
			perror("write");
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

