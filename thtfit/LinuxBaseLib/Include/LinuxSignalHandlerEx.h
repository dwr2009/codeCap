#ifndef	_LINUX_SIGNAL_HANDLER_H_
#define	_LINUX_SIGNAL_HANDLER_H_

#include <SharedPtr.h>
#include <list>
#include "SignalHandlerItem.h"
#include "RegisteredSignalInfo.h"
#include <AutoLock.h>
#include <Mutex2.h>

using namespace CppBase;
using namespace std;

typedef list < SharedPtr<CSignalHandlerItem> > SIGNAL_HANDLER_LIST;
typedef list < SharedPtr<CRegisteredSignalInfo> > REGISTERED_SIGNAL_INFO_LIST;

class CLinuxSignalHandlerEx
{
public:
	CLinuxSignalHandlerEx();
	virtual ~CLinuxSignalHandlerEx();
	virtual INT_t HookSignalInternal(int SigNo, PFN_SignalHandleCb pfnSignalHandlerCb, PVOID pCbContext = NULL, pthread_t ThreadId = 0);
	static INT_t HookSignal(int SigNo, PFN_SignalHandleCb pfnSignalHandlerCb, PVOID pCbContext = NULL, pthread_t ThreadId = 0);
	VOID SigHandlerInternal(int SigNo);
	static VOID SigHandler(int SigNo);
private:
	CMutex2 m_SigDataMutex;
	SIGNAL_HANDLER_LIST m_lstSignalHandler;
	REGISTERED_SIGNAL_INFO_LIST m_lstRegisteredSigInfo;
};

#endif	//_LINUX_SIGNAL_HANDLER_H_

