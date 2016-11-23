#include <AutoScopeLock.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

CAutoScopeLock::CAutoScopeLock(CMutexImplInterface * pMutexImplInterface)
{
	INT_t iRet;
	m_bIsLocked = FALSE;
	m_pMutexImplInterface = pMutexImplInterface;
	iRet = m_pMutexImplInterface->Lock();
	if(ERROR_SUCCESS == iRet)
	{
		m_bIsLocked = TRUE;
	}
	else
	{
		errno = m_pMutexImplInterface->GetCrtErrNo();
		PRINT_BFILE_LINENO_CRT_ERRINFO;
	}
}

CAutoScopeLock::~CAutoScopeLock()
{
	INT_t iRet;
	if(m_bIsLocked)
	{
		iRet = m_pMutexImplInterface->Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			m_bIsLocked = FALSE;
		}
		else
		{
			errno = m_pMutexImplInterface->GetCrtErrNo();
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
}

