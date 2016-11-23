#ifndef	_SIGNAL_HANDLER_ITEM_H_
#define	_SIGNAL_HANDLER_ITEM_H_

#include <BaseTypeDef.h>
#include <pthread.h>
#include <SharedPtr.h>

using namespace CppBase;

typedef VOID (*PFN_SignalHandleCb)(VOID * pCbContext, int SigNo);

class CSignalHandlerItem
{
public:
	CSignalHandlerItem();
	CSignalHandlerItem(PVOID pCbContext, pthread_t ThreadId, PFN_SignalHandleCb pfnSigHandlerCb, int SigNo);
	INT_t TriggerIfMatch(pthread_t ThreadId, int SigNo);
	BOOL_t ChkMatch(pthread_t ThreadId, int SigNo);
	SharedPtr<CSignalHandlerItem> Clone();
protected:
	
protected:
	VOID * m_pCbContext;
	pthread_t m_ThreadId;
	PFN_SignalHandleCb m_pfnSigHandlerCb;
	int m_SigNo;
};

#endif	//_SIGNAL_HANDLER_ITEM_H_

