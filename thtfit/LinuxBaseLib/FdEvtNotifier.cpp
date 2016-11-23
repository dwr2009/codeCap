#include <FdEvtNotifier.h>
#include <BaseErrDef.h>
#include <ThreadData.h>

CFdEvtNotifier::CFdEvtNotifier()
{

}

CFdEvtNotifier::~CFdEvtNotifier()
{

}

/*
ThreadContext: event loop thread
*/
INT_t CFdEvtNotifier::RegisterFdNotify(CONST int iFd, CONST IFileDescriptorEventIf::FD_EVENT_FLAGS FdEvtFlags,
	CONST WeakPtr <IFileDescriptorEventIf> FdEvtIf_wp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CThreadData> ThreadData_sp;
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		ThreadData_sp = getThreadData();
		if(ThreadData_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		EvtLoopIf_sp = ThreadData_sp->getEvtLoopIf();
		if(EvtLoopIf_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		iRet = EvtLoopIf_sp->RegisterFdNotify(iFd, FdEvtFlags, FdEvtIf_wp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

/*
ThreadContext: event loop thread
*/
INT_t CFdEvtNotifier::getFdNotifyInfo(CONST int iFd, OUT IFileDescriptorEventIf::FD_EVENT_FLAGS & FdEvtFlags,
	OUT WeakPtr <IFileDescriptorEventIf> & FdEvtIf_wp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CThreadData> ThreadData_sp;
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		ThreadData_sp = getThreadData();
		if(ThreadData_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		EvtLoopIf_sp = ThreadData_sp->getEvtLoopIf();
		if(EvtLoopIf_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		iRet = EvtLoopIf_sp->getFdNotifyInfo(iFd, FdEvtFlags, FdEvtIf_wp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

/*
ThreadContext: event loop thread
*/
INT_t CFdEvtNotifier::UnregisterFdNotify(CONST int iFd)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CThreadData> ThreadData_sp;
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		ThreadData_sp = getThreadData();
		if(ThreadData_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		EvtLoopIf_sp = ThreadData_sp->getEvtLoopIf();
		if(EvtLoopIf_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		iRet = EvtLoopIf_sp->UnregisterFdNotify(iFd);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2013-1-4*/
/*
ThreadContext: event loop thread
*/
INT_t CFdEvtNotifier::RegisterTimer(WeakPtr <CppBase::ITimerListener> TimerListener_wp, OUT UINT_t & TimerId, INT_t TimeoutMs)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CThreadData> ThreadData_sp;
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		ThreadData_sp = getThreadData();
		if(ThreadData_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		EvtLoopIf_sp = ThreadData_sp->getEvtLoopIf();
		if(EvtLoopIf_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		iRet = EvtLoopIf_sp->RegisterTimer(TimerListener_wp, TimerId, TimeoutMs);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

/*
ThreadContext: event loop thread
*/
INT_t CFdEvtNotifier::UnregisterTimer(CONST UINT_t TimerId)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	SharedPtr <CThreadData> ThreadData_sp;
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		ThreadData_sp = getThreadData();
		if(ThreadData_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		EvtLoopIf_sp = ThreadData_sp->getEvtLoopIf();
		if(EvtLoopIf_sp.isNull())
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		iRet = EvtLoopIf_sp->UnregisterTimer(TimerId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}
#endif