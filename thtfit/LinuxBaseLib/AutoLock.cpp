#include <AutoLock.h>
#ifdef	ANDROID
#include <cutils/compiler.h>
#else	//!ANDROID
#include <Compiler.h>
#endif	//ANDROID
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

CAutoLock::CAutoLock(CMutex2 * pMutexLocker):m_bLocked(FALSE)
{
	INT_t iRet;

	m_pMutexLocker = pMutexLocker;
	if(CC_LIKELY(m_pMutexLocker))
	{
		iRet = m_pMutexLocker->Lock();
		if(CC_LIKELY(ERROR_SUCCESS == iRet))
		{
			m_bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
	}
	else
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
}

CAutoLock::~CAutoLock()
{
	INT_t iRet;
	if(CC_LIKELY(m_pMutexLocker))
	{
		if(CC_LIKELY(m_bLocked))
		{
			iRet = m_pMutexLocker->Unlock();
			if(CC_LIKELY(ERROR_SUCCESS == iRet))
			{
				m_bLocked = FALSE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
		}
	}
	else
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
}

