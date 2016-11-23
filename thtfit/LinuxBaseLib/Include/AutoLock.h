#ifndef	_AUTO_LOCK_H_
#define	_AUTO_LOCK_H_

#include <BaseTypeDef.h>
#include <Mutex2.h>

class CAutoLock
{
public:
	CAutoLock(CMutex2 * pMutexLocker);
	~CAutoLock();
private:
	CMutex2 * m_pMutexLocker;
	BOOL_t m_bLocked;
};

#endif	//_AUTO_LOCK_H_
