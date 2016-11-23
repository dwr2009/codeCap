#include	"Mutex2.h"
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>

CMutex2::CMutex2()
{
	int iRet = 0;

	m_iCrtErrNo = 0;

	do{
		iRet = pthread_mutexattr_init(&m_MutexAttr);
		if(0 != iRet)
		{
			SetCrtErrNo(errno);
			break;
		}
		iRet = pthread_mutexattr_settype(&m_MutexAttr, PTHREAD_MUTEX_RECURSIVE );
		if(0 != iRet)
		{
			SetCrtErrNo(errno);
			break;
		}
		iRet = pthread_mutex_init(&m_oMutex, &m_MutexAttr);
		if(0 != iRet)
		{
			SetCrtErrNo(errno);
			break;
		}
	}while(FALSE);
}

CMutex2::~CMutex2()
{
	int iRet = 0;

	iRet = pthread_mutex_destroy(&m_oMutex);
	if(0 != iRet)
	{
		SetCrtErrNo(errno);
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}

	iRet = pthread_mutexattr_destroy(&m_MutexAttr);
	if(0 != iRet)
	{
		SetCrtErrNo(errno);
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
}

int CMutex2::Lock()
{
	int iOutRet = 0, iRet = 0;

	while(TRUE)
	{
		iRet = pthread_mutex_lock(&m_oMutex);
		if(0 == iRet)	//success
		{
			break;
		}
		else if(0 != iRet && EINTR == errno)
		{
			;	//continue
		}
		else	//other errors
		{
			SetCrtErrNo(errno);
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}

	iOutRet = iRet;

	return iOutRet;
}

INT_t CMutex2::TryLock()
{
	INT_t iOutRet = ERROR_SUCCESS;
	int iRet;

	do
	{
		iRet = pthread_mutex_trylock(&m_oMutex);
		if(0 == iRet)
		{
		}
		else if(EBUSY == iRet)
		{
			iOutRet = ERR_BUSY;
			break;
		}
		else
		{
			iOutRet = ERR_MUTEX_TRY_LOCK_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int CMutex2::Unlock()
{
	int iOutRet = 0, iRet = 0;

	while(TRUE)
	{
		iRet = pthread_mutex_unlock(&m_oMutex);
		if(0 == iRet)
		{
			break;
		}
		else if(0 != iRet && EINTR == errno)
		{
			;	//continue
		}
		else	//other errors
		{
			SetCrtErrNo(errno);
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}

	iOutRet = iRet;

	return iOutRet;
}


