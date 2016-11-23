#include	"EventEx.h"
#include <string.h>
#include	<unistd.h>
#include	<BaseErrDef.h>
#include	<ErrPrintHelper.h>
#include	<BaseTypeDef.h>
#include	<BaseErrDef.h>
#include <memory2.h>
#include <Compiler.h>
#include <DateTime.h>

#define	MIN_SEM_VAL_ON_EVENT		100

CEventEx::CEventEx():m_bInited(FALSE)
{
	int iRet = 0;

	m_iCrtErrNo = 0;
	m_bHasEvent = FALSE;
	m_bNeedDestroyMutex = FALSE;
	m_bNeedDestroyMutexAttr = FALSE;
	m_bNeedDestroyCondVar = FALSE;
#ifdef	ANDROID
#else	//!ANDROID
	m_bNeedDestroyCondVarAttr= FALSE;
#endif	//ANDROID

	do
	{
		iRet = pthread_mutexattr_init(&m_MutexAttr);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_bNeedDestroyMutexAttr = TRUE;
		
		iRet = pthread_mutexattr_settype(&m_MutexAttr, PTHREAD_MUTEX_NORMAL);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		iRet = pthread_mutex_init(&m_Mutex, &m_MutexAttr);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_bNeedDestroyMutex = TRUE;

#ifdef	ANDROID
#else	//!ANDROID
		iRet = pthread_condattr_init(&m_CondVarAttr);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_bNeedDestroyCondVarAttr = TRUE;
#endif	//ANDROID

#ifdef	ANDROID
		iRet = pthread_cond_init(&m_CondVar, NULL);
#else	//!ANDROID
		iRet = pthread_cond_init(&m_CondVar, &m_CondVarAttr);
#endif	//ANDROID
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_bNeedDestroyCondVar = TRUE;

		m_bInited = TRUE;
	}while(FALSE);
}

CEventEx::~CEventEx()
{
	int iRet = 0;

	if(CC_LIKELY(m_bNeedDestroyCondVar))
	{
		iRet = pthread_cond_destroy(&m_CondVar);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_bNeedDestroyCondVar = FALSE;
	}

#ifdef	ANDROID
#else	//!ANDROID
	if(CC_LIKELY(m_bNeedDestroyCondVarAttr))
	{
		iRet = pthread_condattr_destroy(&m_CondVarAttr);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_bNeedDestroyCondVarAttr = FALSE;
	}
#endif	//ANDROID
	
	if(CC_LIKELY(m_bNeedDestroyMutex))
	{
		iRet = pthread_mutex_destroy(&m_Mutex);
		if(CC_UNLIKELY(0 != iRet))
		{
			LOG_BLINE("iRet=%d,%s\n", iRet, strerror(errno));
		}
		m_bNeedDestroyMutex = FALSE;
	}

	if(CC_LIKELY(m_bNeedDestroyMutexAttr))
	{
		iRet = pthread_mutexattr_destroy(&m_MutexAttr);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_bNeedDestroyMutexAttr = FALSE;
	}
}

int CEventEx::SetEvent()
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	m_iCrtErrNo = 0;

	do
	{
		if(CC_UNLIKELY(FALSE == m_bInited))
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}
		
		iRet = pthread_mutex_lock(&m_Mutex);
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;		
		}

		//LogFileLineFormat("SetEvent");

		if(m_bHasEvent)
		{
			break;
		}

		m_bHasEvent = TRUE;
		iRet = pthread_cond_broadcast(&m_CondVar);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}while(FALSE);

	if(CC_LIKELY(bLocked))
	{
		iRet = pthread_mutex_unlock(&m_Mutex);
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;		
		}
	}

	return iOutRet;
}

int CEventEx::ResetEvent(BOOL_t * CONST pbPrevEventState/* = NULL*/)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	m_iCrtErrNo = 0;

	do
	{
		if(CC_UNLIKELY(FALSE == m_bInited))
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(FALSE == bLocked)
		{
			iRet = pthread_mutex_lock(&m_Mutex);
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;		
			}
		}

		if(pbPrevEventState)
		{
			*pbPrevEventState = m_bHasEvent;
		}

		m_bHasEvent = FALSE;
	}while(FALSE);

	if(CC_LIKELY(bLocked))
	{
		iRet = pthread_mutex_unlock(&m_Mutex);
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;		
		}
	}

	return iOutRet;
}

int CEventEx::WaitEvent()
{
	int iOutRet = ERROR_SUCCESS, iRet = 0;
	BOOL_t bLocked = FALSE;

	m_iCrtErrNo = ERROR_SUCCESS;

	do
	{
		if(CC_UNLIKELY(FALSE == m_bInited))
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(CC_LIKELY(FALSE == bLocked))
		{
			iRet = pthread_mutex_lock(&m_Mutex);
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR; 	
			}
		}

		if(CC_UNLIKELY(m_bHasEvent))
		{
			break;
		}

CONTINUE_WAIT:
		iRet = pthread_cond_wait(&m_CondVar, &m_Mutex);
		if(CC_LIKELY(0 == iRet))
		{
		}
		else if(m_bHasEvent)
		{			
		}
		else if(EAGAIN == iRet)
		{
			goto CONTINUE_WAIT;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR; 	
		}
	}while(FALSE);

	if(CC_LIKELY(bLocked))
	{
		iRet = pthread_mutex_unlock(&m_Mutex);
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;		
		}
	}

	return iOutRet;
}

INT CEventEx::WaitEventTimeout(INT iTimeoutMs)
{
	int iOutRet = ERROR_SUCCESS, iRet = 0;
	BOOL_t bLocked = FALSE;
	struct timespec tsAbsTimeToWait;

	m_iCrtErrNo = ERROR_SUCCESS;

	do
	{
		if(CC_UNLIKELY(FALSE == m_bInited))
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}

		if(CC_LIKELY(FALSE == bLocked))
		{
			iRet = pthread_mutex_lock(&m_Mutex);
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR; 	
			}
		}

		if(CC_UNLIKELY(m_bHasEvent))
		{
			break;
		}
		else if(CC_UNLIKELY(0 >= iTimeoutMs))	//only test
		{
			iOutRet = ERR_TIMEOUT;
			break;
		}

		INT_t iLeftTimeMs = iTimeoutMs;
		UINT64_t StartTimeMs = GetSysUpTimeMs();
		UINT64_t CurTimeMs;
CONTINUE_WAIT:
		iRet = clock_gettime( CLOCK_REALTIME, &tsAbsTimeToWait);
		if(0 != iRet)
		{
			iOutRet = ERROR_CALL_SYSTEM_FAILED;
			break;
		}
		//LogFileLineFormat("tv_sec=%ld,tv_nsec=%ld", tsAbsTimeToWait.tv_sec, tsAbsTimeToWait.tv_nsec);
		tsAbsTimeToWait.tv_sec += iLeftTimeMs / 1000;
		tsAbsTimeToWait.tv_nsec += (iLeftTimeMs % 1000) * 1000 * 1000;
		if(1*1000*1000*1000 <= tsAbsTimeToWait.tv_nsec)
		{
			tsAbsTimeToWait.tv_sec += 1;
			tsAbsTimeToWait.tv_nsec -= 1*1000*1000*1000;
		}
		//LogFileLineFormat("tv_sec=%ld,tv_nsec=%ld", tsAbsTimeToWait.tv_sec, tsAbsTimeToWait.tv_nsec);

		iRet = pthread_cond_timedwait(&m_CondVar, &m_Mutex, &tsAbsTimeToWait);
		if(CC_LIKELY(0 == iRet))
		{
			//LogFileLineFormat("pthread_cond_timedwait signal");
		}
		else if(m_bHasEvent)
		{			
		}
		else if(CC_UNLIKELY(ETIMEDOUT == iRet))
		{
			iOutRet = ERR_TIMEOUT;
		}
		else if(EINTR == iRet)
		{
			iOutRet = ERR_LINUX_SIGNAL_INT;
		}
		else if(EAGAIN == iRet)
		{
			CurTimeMs = GetSysUpTimeMs();
			iLeftTimeMs -= (CurTimeMs - StartTimeMs);
			if(0 >= iLeftTimeMs)
			{
				iOutRet = ERR_TIMEOUT;
				break;
			}
			goto CONTINUE_WAIT;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR; 	
		}
	}while(FALSE);

	if(CC_LIKELY(bLocked))
	{
		iRet = pthread_mutex_unlock(&m_Mutex);
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;		
		}
	}

	return iOutRet;
}

int CEventEx::GetCrtErrNo()
{
	return m_iCrtErrNo;
}


HANDLE EventEx_new()
{
	CEventEx * pEventEx = NULL;

	do
	{
		pEventEx = new CEventEx();
		if(NULL == pEventEx)
		{
			break;
		}		
	}while(FALSE);

	return pEventEx;
}

INT_t EventEx_SetEvent(HANDLE pHandle)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CEventEx * pEventEx = (typeof(pEventEx))pHandle;

	do
	{
		if(NULL == pEventEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = pEventEx->SetEvent();
	}while(FALSE);

	return iOutRet;
}

INT_t EventEx_ResetEvent(HANDLE pHandle)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CEventEx * pEventEx = (typeof(pEventEx))pHandle;

	do
	{
		if(NULL == pEventEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = pEventEx->ResetEvent();
	}while(FALSE);

	return iOutRet;
}

INT_t EventEx_WaitEvent(HANDLE pHandle)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CEventEx * pEventEx = (typeof(pEventEx))pHandle;

	do
	{
		if(NULL == pEventEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = pEventEx->WaitEvent();
	}while(FALSE);

	return iOutRet;
}

INT_t EventEx_delete(HANDLE pHandle)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CEventEx * pEventEx = (typeof(pEventEx))pHandle;

	do
	{
		if(NULL == pEventEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		delete pEventEx;
		pEventEx = NULL;
	}while(FALSE);

	return iOutRet;
}

