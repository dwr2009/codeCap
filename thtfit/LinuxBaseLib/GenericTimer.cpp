#include "Include/GenericTimer.h"
#include <AutoLock.h>
#include <BaseErrDef.h>
#include <DateTime.h>

namespace CppBase
{

CSyncTimerItem::CSyncTimerItem()
{
	m_TimerId = 0;
	m_TimeoutMs = 0;
	m_LeftTimeoutMs = 0;
}

CSyncTimerItem::~CSyncTimerItem()
{

}


CGenericTimer::CGenericTimer()
{
	m_TimerId = 0;
	m_PrevChkTimeMs = 0;
	m_bIsCheckingTimer = FALSE;
}

CGenericTimer::~CGenericTimer()
{

}

UINT_t CGenericTimer::getFreeTimerId_Locked()
{
	UINT_t FreeTimerId = 0;
	BOOL_t bFound;
	SYNC_TIMER_LIST::iterator itSyncTimerItem;

	do
	{
		while(TRUE)
		{
			m_TimerId++;
			if(0 == m_TimerId)
			{
				m_TimerId++;
			}
			//Find whether exists
			bFound = FALSE;
			for(itSyncTimerItem = m_SyncTimerList.begin(); itSyncTimerItem != m_SyncTimerList.begin(); itSyncTimerItem++)
			{
				SharedPtr <CSyncTimerItem> SyncTimerItem_sp;
				SyncTimerItem_sp = (*itSyncTimerItem);
				if(NULL == SyncTimerItem_sp.get())
				{
					m_SyncTimerList.erase(itSyncTimerItem);
					continue;
				}
				
				SharedPtr <ITimerListener> TempTimerListener_sp;
				TempTimerListener_sp = SyncTimerItem_sp->m_TimerListener_wp.Promote();
				if(NULL == TempTimerListener_sp.get())
				{
					m_SyncTimerList.erase(itSyncTimerItem);
					continue;
				}
				
				if(SyncTimerItem_sp->m_TimerId == m_TimerId)
				{
					bFound = TRUE;
					break;
				}
			}
			if(FALSE == bFound)
			{
				break;
			}
		}
		FreeTimerId = m_TimerId;
	}while(FALSE);

	return FreeTimerId;
}

INT_t CGenericTimer::RegisterTimer(WeakPtr <CppBase::ITimerListener> TimerListener_wp, OUT UINT_t & TimerId, INT_t TimeoutMs)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SYNC_TIMER_LIST::iterator itSyncTimerItem;
	SharedPtr <ITimerListener> TimerListener_sp;

	CAutoLock AutoRangeLocker(&m_SharedDataMutex);	

	do
	{
		if(0 > TimeoutMs)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		TimerListener_sp = TimerListener_wp.Promote();
		if(NULL == TimerListener_sp.get())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		SharedPtr <CSyncTimerItem> SyncTimerItem_sp(new CSyncTimerItem);
		if(NULL == SyncTimerItem_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		TimerId = getFreeTimerId_Locked();
		SyncTimerItem_sp->m_TimerListener_wp = TimerListener_sp;
		SyncTimerItem_sp->m_TimerId = TimerId;
		SyncTimerItem_sp->m_TimeoutMs = TimeoutMs;
		SyncTimerItem_sp->m_LeftTimeoutMs = SyncTimerItem_sp->m_TimeoutMs;
		try
		{
			m_SyncTimerList.push_back(SyncTimerItem_sp);
		}
		catch(std::bad_alloc & AllocEx)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGenericTimer::UnregisterTimer(CONST UINT_t TimerId)
{
	INT_t iOutRet = ERROR_SUCCESS;
	BOOL_t bFound;
	SYNC_TIMER_LIST::iterator itSyncTimerItem;

	do
	{
		bFound = FALSE;
		for(itSyncTimerItem = m_SyncTimerList.begin(); itSyncTimerItem != m_SyncTimerList.end(); itSyncTimerItem++)
		{
			SharedPtr <CSyncTimerItem> SyncTimerItem_sp;
			SyncTimerItem_sp = (*itSyncTimerItem);
			if(NULL == SyncTimerItem_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}
			
			SharedPtr <ITimerListener> TempTimerListener_sp;
			TempTimerListener_sp = SyncTimerItem_sp->m_TimerListener_wp.Promote();
			if(NULL == TempTimerListener_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}
			
			if(SyncTimerItem_sp->m_TimerId == TimerId)
			{
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			m_SyncTimerList.erase(itSyncTimerItem);
		}
		else
		{
			iOutRet = ERROR_NOT_FOUND;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGenericTimer::CheckAndTrig()
{
	INT_t iOutRet = ERROR_SUCCESS;
	SYNC_TIMER_LIST::iterator itSyncTimerItem;
	UINT64_t CurTimeMs = GetSysUpTimeMs();	
	UINT64_t ElapsedTimeMs;

	do
	{
		//prevent function reentrant
		if(m_bIsCheckingTimer)
		{
			break;
		}
		m_bIsCheckingTimer = TRUE;
		
		if(0 == m_PrevChkTimeMs)	//first time
		{
			m_PrevChkTimeMs = CurTimeMs;
		}
		ElapsedTimeMs = CurTimeMs - m_PrevChkTimeMs;
		
		for(itSyncTimerItem = m_SyncTimerList.begin(); itSyncTimerItem != m_SyncTimerList.end(); itSyncTimerItem++)
		{
			SharedPtr <CSyncTimerItem> SyncTimerItem_sp;
			SyncTimerItem_sp = (*itSyncTimerItem);
			if(NULL == SyncTimerItem_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}
			SyncTimerItem_sp->m_bChecked = FALSE;
		}
		
RESCAN_TIMER_LIST:
		for(itSyncTimerItem = m_SyncTimerList.begin(); itSyncTimerItem != m_SyncTimerList.end(); itSyncTimerItem++)
		{
			SharedPtr <CSyncTimerItem> SyncTimerItem_sp;
			SyncTimerItem_sp = (*itSyncTimerItem);
			if(NULL == SyncTimerItem_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}
			
			SharedPtr <ITimerListener> TempTimerListener_sp;
			TempTimerListener_sp = SyncTimerItem_sp->m_TimerListener_wp.Promote();
			if(NULL == TempTimerListener_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}

			if(FALSE == SyncTimerItem_sp->m_bChecked)
			{
				SyncTimerItem_sp->m_bChecked = TRUE;
				SyncTimerItem_sp->m_LeftTimeoutMs -= ElapsedTimeMs;
				if(0 >= SyncTimerItem_sp->m_LeftTimeoutMs)
				{
					TempTimerListener_sp->OnTimer(SyncTimerItem_sp->m_TimerId);
					SyncTimerItem_sp->m_LeftTimeoutMs = SyncTimerItem_sp->m_TimeoutMs;
					goto RESCAN_TIMER_LIST;
				}
			}
		}
		//Reclocking
		m_PrevChkTimeMs = CurTimeMs;
		//prevent function reentrant
		m_bIsCheckingTimer = FALSE;
	}while(FALSE);

	return iOutRet;
}

INT_t CGenericTimer::getNextTimeoutMs(OUT INT_t & NextTimeoutMs)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SYNC_TIMER_LIST::iterator itSyncTimerItem;

	NextTimeoutMs = -1;	//infinite

	do
	{
		for(itSyncTimerItem = m_SyncTimerList.begin(); itSyncTimerItem != m_SyncTimerList.end(); itSyncTimerItem++)
		{
			SharedPtr <CSyncTimerItem> SyncTimerItem_sp;
			SyncTimerItem_sp = (*itSyncTimerItem);
			if(NULL == SyncTimerItem_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}
			
			SharedPtr <ITimerListener> TempTimerListener_sp;
			TempTimerListener_sp = SyncTimerItem_sp->m_TimerListener_wp.Promote();
			if(NULL == SyncTimerItem_sp.get())
			{
				m_SyncTimerList.erase(itSyncTimerItem);
				continue;
			}

			if(0 > NextTimeoutMs)
			{
				NextTimeoutMs = SyncTimerItem_sp->m_LeftTimeoutMs;
			}
			else
			{
				if(SyncTimerItem_sp->m_LeftTimeoutMs < NextTimeoutMs)
				{
					NextTimeoutMs = SyncTimerItem_sp->m_LeftTimeoutMs;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

}

