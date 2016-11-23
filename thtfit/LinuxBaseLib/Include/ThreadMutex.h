#ifndef	_THREAD_MUTEX_H_
#define	_THREAD_MUTEX_H_

#include	<BaseTypeDef.h>
#include <pthread.h>

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

typedef struct _THREAD_MUTEX THREAD_MUTEX;
typedef THREAD_MUTEX * P_THREAD_MUTEX;

P_THREAD_MUTEX CreateThreadMutex();
INT ThreadMutex_Lock(P_THREAD_MUTEX pThreadMutex);
INT ThreadMutex_Unlock(P_THREAD_MUTEX pThreadMutex);
INT DestroyThreadMutex(P_THREAD_MUTEX pThreadMutex);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_THREAD_MUTEX_H_

