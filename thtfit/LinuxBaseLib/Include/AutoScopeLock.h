#if	!defined(_AUTO_SCOPE_LOCK_H_)
#define	_AUTO_SCOPE_LOCK_H_

#include <MutexImplInterface.h>

class CAutoScopeLock
{
public:
	explicit CAutoScopeLock(CMutexImplInterface * pMutexImplInterface);
	~CAutoScopeLock();
private:
	CMutexImplInterface * m_pMutexImplInterface;
	BOOL_t m_bIsLocked;
};

#endif	//_AUTO_SCOPE_LOCK_H_
