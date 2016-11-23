#include	"ThreadMutex.h"
#include <stdlib.h>
#include	<BaseErrDef.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include	<ErrPrintHelper.h>

static pthread_mutex_t g_oRecursiveMutex = 
#ifdef	ANDROID
	PTHREAD_RECURSIVE_MUTEX_INITIALIZER
#else	// !ANDROID
	PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#endif	//ANDROID
	;

struct _THREAD_MUTEX{
	pthread_mutex_t oThreadMutex;
};

P_THREAD_MUTEX CreateThreadMutex()
{
	P_THREAD_MUTEX pThreadMutex = NULL;

	do{
		pThreadMutex = malloc(sizeof(THREAD_MUTEX));
		if(NULL == pThreadMutex)
		{			
			break;
		}

		memset(pThreadMutex, 0x00, sizeof(*pThreadMutex));

		pThreadMutex->oThreadMutex = g_oRecursiveMutex;
	}while(FALSE);

	return pThreadMutex;
}

INT ThreadMutex_Lock(P_THREAD_MUTEX pThreadMutex)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		if(NULL == pThreadMutex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = pthread_mutex_lock(&(pThreadMutex->oThreadMutex));
		if(0 != iRet)
		{
			iOutRet = ERR_MUTEX_LOCK_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT ThreadMutex_Unlock(P_THREAD_MUTEX pThreadMutex)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		if(NULL == pThreadMutex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

CONTINUE_WAIT:
		iRet = pthread_mutex_unlock(&(pThreadMutex->oThreadMutex));
		if((-1 == iRet && EINTR == errno) || (EINTR == iRet))
		{
			goto CONTINUE_WAIT;
		}
		else if(0 != iRet)
		{
			iOutRet = ERR_MUTEX_UNLOCK_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT DestroyThreadMutex(P_THREAD_MUTEX pThreadMutex)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		if(NULL == pThreadMutex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = pthread_mutex_destroy(&(pThreadMutex->oThreadMutex));
		if(0 != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}

		free(pThreadMutex);
		pThreadMutex = NULL;
	}while(FALSE);

	return iOutRet;
}

