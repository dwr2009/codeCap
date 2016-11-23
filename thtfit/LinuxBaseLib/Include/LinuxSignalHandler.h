#ifndef	_LINUX_SIGNAL_HANDLER_H_
#define	_LINUX_SIGNAL_HANDLER_H_

#include <BaseList.h>
#include <pthread.h>

typedef CList <P_LINUX_SIG_HANDLER_ITEM, P_LINUX_SIG_HANDLER_ITEM &> LINUX_SIG_HANDLER_ITEM_LIST;

class CLinuxSignalHandler
{
public:
	CLinuxSignalHandler();
	~CLinuxSignalHandler();
	INT RegisterSigHandler(P_REG_SIG_HANDLER_PARAM_IN pRegSigHandlerParamIn);
	INT UnregisterSigHandler(P_UNREG_SIG_HANDLER_PARAM_IN pUnregSigHandlerParamIn);
private:
	static void SysSignalHandler(int iSigNo);
	VOID DestroySigHandlerList();
private:
	LINUX_SIG_HANDLER_ITEM_LIST m_lstLinuxSigHandlerItem;
	BOOL_t m_bSigDelayPending;
	BOOL_t m_bSigLocked;
	int m_iPendingSigNo;
};

#endif	//_LINUX_SIGNAL_HANDLER_H_

