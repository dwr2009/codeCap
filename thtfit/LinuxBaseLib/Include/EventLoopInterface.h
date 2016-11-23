#ifndef	_EVENT_LOOP_INTERFACE_H_
#define	_EVENT_LOOP_INTERFACE_H_

#include <WeakPtr.h>
#include <BaseTypeDef.h>
#include <FileDescriptorEventIf.h>
#include <TimerListenerInterface.h>

using namespace CppBase;

class IEventLoopInterface
{
public:
	virtual INT_t RegisterFdNotify(CONST int iFd, CONST IFileDescriptorEventIf::FD_EVENT_FLAGS FdEvtFlags,
		CONST WeakPtr <IFileDescriptorEventIf> FdEvtIf_wp) = 0;
	virtual INT_t getFdNotifyInfo(CONST int iFd, OUT IFileDescriptorEventIf::FD_EVENT_FLAGS & FdEvtFlags,
		OUT WeakPtr <IFileDescriptorEventIf> & FdEvtIf_wp) = 0;
	virtual INT_t UnregisterFdNotify(CONST int iFd) = 0;

#if 1/*added by lxj 2013-1-4*/
	virtual INT_t RegisterTimer(WeakPtr <CppBase::ITimerListener> TimerListener_wp, OUT UINT_t & TimerId, INT_t TimeoutMs) = 0;
	virtual INT_t UnregisterTimer(CONST UINT_t TimerId) = 0;
#endif
};

#endif	//_EVENT_LOOP_INTERFACE_H_

