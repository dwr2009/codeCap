#ifndef	_EVENT_THREAD_H_
#define	_EVENT_THREAD_H_

#include <BaseTypeDef.h>
#include <Thread.h>
#include <EventCenter.h>

class CEventThread : protected CThread, protected CEventCenter
{
public:
	CEventThread();
	virtual ~CEventThread();
	virtual INT Run();
	virtual INT Stop();
protected:
	virtual INT InitInstance();
	virtual PVOID ThreadLoop();
	virtual INT ExitInstance();
};

#endif	//_EVENT_THREAD_H_

