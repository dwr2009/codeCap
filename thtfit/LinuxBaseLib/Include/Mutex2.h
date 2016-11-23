#ifndef	_MUTEX2_H_
#define	_MUTEX2_H_

#include <pthread.h>
#include <CrtErr.h>
#include <BaseTypeDef.h>

class CMutex2 : public CCrtErr
{
public:
	CMutex2();
	~CMutex2();
	int Lock();
	INT_t TryLock();
	int Unlock();
protected:
	pthread_mutex_t m_oMutex;
private:
	pthread_mutexattr_t m_MutexAttr;
};

#endif	//_MUTEX2_H_

