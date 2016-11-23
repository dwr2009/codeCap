#ifndef	_THREAD_DATA_H_
#define	_THREAD_DATA_H_

#include <WeakPtr.h>
#include <list>
#include <pthread.h>
#include <Mutex2.h>
#include <EventLoopInterface.h>
#include <map>

using namespace CppBase;
using namespace std;

class CThreadData
{
public:
	CThreadData();
	virtual ~CThreadData();
	virtual INT_t setEvtLoopIf(WeakPtr <IEventLoopInterface> EvtLoopIf_wp);
	virtual SharedPtr <IEventLoopInterface> getEvtLoopIf();
private:
	CMutex2 m_mutexSharedData;
	WeakPtr <IEventLoopInterface> m_EvtLoopIf_wp;
};

typedef map < pthread_t, SharedPtr <CThreadData> > ThreadData_MAP;

class CThreadDataManager
{
public:
	CThreadDataManager();
	virtual ~CThreadDataManager();	
	virtual SharedPtr <CThreadData> getThreadData();
private:
	CMutex2 m_mutexSharedData;
	ThreadData_MAP m_mapThreadData;
};

SharedPtr <CThreadData> getThreadData();

#endif	//_THREAD_DATA_H_

