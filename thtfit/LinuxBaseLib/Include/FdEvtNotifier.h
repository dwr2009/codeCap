#ifndef	_FD_EVT_NOTIFIER_H_
#define	_FD_EVT_NOTIFIER_H_

#include <BaseTypeDef.h>
#include <FileDescriptorEventIf.h>
#include <TimerListenerInterface.h>
#include <WeakPtr.h>

using namespace CppBase;

class CFdEvtNotifier
{
public:
	CFdEvtNotifier();
	virtual ~CFdEvtNotifier();
	static INT_t RegisterFdNotify(CONST int iFd, CONST IFileDescriptorEventIf::FD_EVENT_FLAGS FdEvtFlags,
		CONST WeakPtr <IFileDescriptorEventIf> FdEvtIf_wp);
	static INT_t getFdNotifyInfo(CONST int iFd, OUT IFileDescriptorEventIf::FD_EVENT_FLAGS & FdEvtFlags,
		OUT WeakPtr <IFileDescriptorEventIf> & FdEvtIf_wp);
	static INT_t UnregisterFdNotify(CONST int iFd);

#if 1/*added by lxj 2013-1-4*/
	static INT_t RegisterTimer(WeakPtr <CppBase::ITimerListener> TimerListener_wp, OUT UINT_t & TimerId, INT_t TimeoutMs);
	static INT_t UnregisterTimer(CONST UINT_t TimerId);
#endif
};

#endif	//_FD_EVT_NOTIFIER_H_

