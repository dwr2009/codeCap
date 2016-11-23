#include <ErrChkMutex.h>
#include <BaseErrDef.h>
#include <errno.h>

CErrChkMutex::CErrChkMutex() : 
#if	defined(ANDROID)
	m_Mutex((pthread_mutex_t)PTHREAD_ERRORCHECK_MUTEX_INITIALIZER)
#else	//!	ANDROID
	m_Mutex((pthread_mutex_t)PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP)
#endif	//ANDROID
{
	
}

CErrChkMutex::~CErrChkMutex()
{
	
}

INT_t CErrChkMutex::Lock()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	SetCrtErrNo(0);

	do
	{
		iRet = pthread_mutex_lock(&m_Mutex);
		if(0 != iRet)
		{
			SetCrtErrNo(errno);
			iOutRet = ERR_MUTEX_LOCK_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CErrChkMutex::Unlock()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	SetCrtErrNo(0);

	do
	{
		iRet = pthread_mutex_unlock(&m_Mutex);
		if(0 != iRet)
		{
			SetCrtErrNo(errno);
			iOutRet = ERR_MUTEX_UNLOCK_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int CErrChkMutex::GetCrtErrNo()
{
	return CCrtErr::GetCrtErrNo();
}


H_CErrChkMutex CErrChkMutex_new()
{
	return (new CErrChkMutex);
}

INT_t CErrChkMutex_Lock(H_CErrChkMutex hErrChkMutex)
{
	CErrChkMutex * pErrChkMutex = (typeof(pErrChkMutex))hErrChkMutex;
	return pErrChkMutex->Lock();
}

INT_t CErrChkMutex_Unlock(H_CErrChkMutex hErrChkMutex)
{
	CErrChkMutex * pErrChkMutex = (typeof(pErrChkMutex))hErrChkMutex;
	return pErrChkMutex->Unlock();
}

int CErrChkMutex_GetCrtErrNo(H_CErrChkMutex hErrChkMutex)
{
	CErrChkMutex * pErrChkMutex = (typeof(pErrChkMutex))hErrChkMutex;
	return pErrChkMutex->GetCrtErrNo();
}

VOID CErrChkMutex_delete(H_CErrChkMutex hErrChkMutex)
{
	CErrChkMutex * pErrChkMutex = (typeof(pErrChkMutex))hErrChkMutex;
	delete pErrChkMutex;
}

