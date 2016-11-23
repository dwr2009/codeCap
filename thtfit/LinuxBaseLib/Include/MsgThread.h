#ifndef	_MSG_THREAD_H_
#define	_MSG_THREAD_H_

#include <Thread.h>
#include <ThreadMessageQueue.h>

class CMsgThread : public CThread, public CThreadMessageQueue
{
public:
	CMsgThread();
	virtual ~CMsgThread();
};

#endif	//_MSG_THREAD_H_
