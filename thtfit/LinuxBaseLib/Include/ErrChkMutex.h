#if	!defined(_ERR_CHK_MUTEX_H_)
#define	_ERR_CHK_MUTEX_H_

#include <MutexImplInterface.h>
#include <pthread.h>
#include <CrtErr.h>
#include <CCplusplusBridge.h>

#ifdef	__cplusplus

class CErrChkMutex : public CMutexImplInterface, public CCrtErr
{
public:
	CErrChkMutex();
	virtual ~CErrChkMutex();
	virtual INT_t Lock();
	virtual INT_t Unlock();
	virtual int GetCrtErrNo();
private:
	pthread_mutex_t m_Mutex;
};

#endif	//__cplusplus

typedef HANDLE H_CErrChkMutex;

DECLARE_EXTERN_C_BLOCK_START

H_CErrChkMutex CErrChkMutex_new();
INT_t CErrChkMutex_Lock(H_CErrChkMutex hErrChkMutex);
INT_t CErrChkMutex_Unlock(H_CErrChkMutex hErrChkMutex);
int CErrChkMutex_GetCrtErrNo(H_CErrChkMutex hErrChkMutex);
VOID CErrChkMutex_delete(H_CErrChkMutex hErrChkMutex);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_ERR_CHK_MUTEX_H_

