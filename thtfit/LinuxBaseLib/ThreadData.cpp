#include <ThreadData.h>
#include <AutoLock.h>
#include <BaseErrDef.h>

CThreadData::CThreadData()
{

}

CThreadData::~CThreadData()
{

}

INT_t CThreadData::setEvtLoopIf(WeakPtr <IEventLoopInterface> EvtLoopIf_wp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		CAutoLock AutoRangeLocker(&m_mutexSharedData);
		m_EvtLoopIf_wp = EvtLoopIf_wp;
	}while(FALSE);

	return iOutRet;
}

SharedPtr <IEventLoopInterface> CThreadData::getEvtLoopIf()
{
	SharedPtr <IEventLoopInterface> EvtLoopIf_sp;

	do
	{
		CAutoLock AutoRangeLocker(&m_mutexSharedData);
		EvtLoopIf_sp = m_EvtLoopIf_wp.Promote();
	}while(FALSE);

	return EvtLoopIf_sp;
}

//CThreadDataManager

CThreadDataManager::CThreadDataManager()
{

}

CThreadDataManager::~CThreadDataManager()
{

}

SharedPtr <CThreadData> CThreadDataManager::getThreadData()
{
	SharedPtr <CThreadData> ThreadData_sp;
	pthread_t tidThis = pthread_self();
	ThreadData_MAP::iterator itThreadData;

	do
	{
		CAutoLock AutoRangeLocker(&m_mutexSharedData);
		try
		{
			itThreadData = m_mapThreadData.find(tidThis);
			if(m_mapThreadData.end() == itThreadData)
			{
				ThreadData_sp = SharedPtr <CThreadData> (new CThreadData);
				if(ThreadData_sp.isNull())
				{
					break;
				}
				m_mapThreadData[tidThis] = ThreadData_sp;
			}
			else
			{
				ThreadData_sp = itThreadData->second;
			}
		}
#if	defined(__EXCEPTIONS)
		catch(std::bad_alloc & BadAllocEx)
		{
			break;
		}
#endif	//defined(__EXCEPTIONS)
	}while(FALSE);
	
	return ThreadData_sp;
}

static CThreadDataManager & getThreadDataManager()
{
	static CThreadDataManager g_ThreadDataManager;
		
	return g_ThreadDataManager;
}

SharedPtr <CThreadData> getThreadData()
{
	SharedPtr <CThreadData> ThreadData_sp;
	CThreadDataManager & ThreadDataMgr = getThreadDataManager();		

	do
	{
		ThreadData_sp = ThreadDataMgr.getThreadData();
	}while(FALSE);

	return ThreadData_sp;
}

