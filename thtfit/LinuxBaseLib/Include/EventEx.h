#ifndef	_EVENT_EX_H_
#define	_EVENT_EX_H_

#include <semaphore.h>
#include <BaseTypeDef.h>
#include <pthread.h>
#include <CCplusplusBridge.h>

#ifdef	__cplusplus

class CEventEx
{
public:
	CEventEx();
	~CEventEx();
	int SetEvent();
	int ResetEvent(BOOL_t * CONST pbPrevEventState = NULL);
	int WaitEvent();
	INT WaitEventTimeout(INT iTimeoutMs);
	int GetCrtErrNo();
private:	
	BOOL_t m_bInited;
	BOOL_t m_bHasEvent;
	pthread_mutex_t m_Mutex;
	pthread_mutexattr_t m_MutexAttr;
	BOOL_t m_bNeedDestroyMutexAttr;
	BOOL_t m_bNeedDestroyMutex;
	pthread_cond_t m_CondVar;
#ifdef	ANDROID
#else	//!ANDROID
	pthread_condattr_t m_CondVarAttr;
	BOOL_t m_bNeedDestroyCondVarAttr;
#endif	//ANDROID
	BOOL_t m_bNeedDestroyCondVar;
	int m_iCrtErrNo;
};

#endif	//__cplusplus

DECLARE_EXTERN_C_BLOCK_START

HANDLE EventEx_new();
INT_t EventEx_SetEvent(HANDLE pHandle);
INT_t EventEx_ResetEvent(HANDLE pHandle);
INT_t EventEx_WaitEvent(HANDLE pHandle);
INT_t EventEx_delete(HANDLE pHandle);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_EVENT_EX_H_

