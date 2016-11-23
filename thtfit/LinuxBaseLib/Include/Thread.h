#ifndef	_THREAD_H_
#define	_THREAD_H_

#include <BaseTypeDef.h>
#include <pthread.h>

typedef struct
{
	INT iSizeOfThis;
	BOOL_t bDetached;
	size_t ulStackSize;
}PTHREAD_CREATE_PARAMS, *P_PTHREAD_CREATE_PARAMS;

class CThread
{
public:
	CThread();
	virtual ~CThread();	
	virtual INT CreateThread(P_PTHREAD_CREATE_PARAMS pPThrdCreateParams = NULL);
	virtual PVOID ThreadLoop();
	virtual BOOL_t GetThreadHasEnded();
	virtual BOOL_t getThreadHasEnded();
	virtual BOOL_t getThreadIsRunning();
	virtual INT SetThreadDetach();
	virtual pthread_t GetThreadId();
	static INT PThrdCreateParamInit(P_PTHREAD_CREATE_PARAMS pPThrdCreateParams);
	static PVOID ThreadProc(PVOID pPara);
	virtual VOID Interrupt();
	static VOID Interrupt(pthread_t threadId);
private:
	pthread_t m_iThreadId;
	BOOL_t m_bThrdAttrHasInit;
	pthread_attr_t m_oThreadAttr;
	BOOL_t m_bThreadHasEnded;
	BOOL_t m_bThreadIsRunning;
};

#endif	//_THREAD_H_

