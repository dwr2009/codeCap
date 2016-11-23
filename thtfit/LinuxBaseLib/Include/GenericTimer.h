#ifndef	_GENERIC_TIMER_H_
#define	_GENERIC_TIMER_H_

#include <list>
#include <Mutex2.h>
#include <SharedPtr.h>
#include <WeakPtr.h>
#include "TimerListenerInterface.h"

using namespace CppBase;
using namespace std;

namespace CppBase
{

class CGenericTimer;

class CSyncTimerItem
{
	friend class CGenericTimer;
public:
	CSyncTimerItem();
	virtual ~CSyncTimerItem();
private:
	UINT_t m_TimerId;
	WeakPtr <ITimerListener> m_TimerListener_wp;
	INT_t m_TimeoutMs;
	INT_t m_LeftTimeoutMs;
	BOOL_t m_bChecked;
};

typedef list < SharedPtr<CSyncTimerItem> > SYNC_TIMER_LIST;

class CGenericTimer
{
public:
	typedef enum
	{
		TIMER_ID_INVALID = 0,
	}TIMER_ID_TYPE, *P_TIMER_ID_TYPE;
public:
	CGenericTimer();
	virtual ~CGenericTimer();
	virtual INT_t RegisterTimer(WeakPtr <CppBase::ITimerListener> TimerListener_wp, OUT UINT_t & TimerId, INT_t TimeoutMs);
	virtual INT_t UnregisterTimer(CONST UINT_t TimerId);
	virtual INT_t CheckAndTrig();
	virtual INT_t getNextTimeoutMs(OUT INT_t & NextTimeoutMs);
private:
	UINT_t getFreeTimerId_Locked();
protected:
	SYNC_TIMER_LIST m_SyncTimerList;
	CMutex2 m_SharedDataMutex;
	UINT_t m_TimerId;
	UINT64_t m_PrevChkTimeMs;
private:
	BOOL_t m_bIsCheckingTimer;
};

}

#endif	//_GENERIC_TIMER_H_

