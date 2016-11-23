#ifndef	_EVENT_CENTER_H_
#define	_EVENT_CENTER_H_

#include <BaseList.h>
#include <Mutex2.h>
#include <UnixDatagramSocket.h>

class CEventCenter
{
public:
	typedef enum
	{
		EVENT_SELECT_NONE = 0,
		EVENT_SELECT_READ = (1 << 0),
		EVENT_SELECT_WRITE = (1 << 1),
		EVENT_SELECT_EXCEPT = (1 << 2),
	}EVENT_SELECT_FLAG, *P_EVENT_SELECT_FLAG;
	typedef struct
	{
		int iFd;
		EVENT_SELECT_FLAG eEvtSelFlags;
	}EVENT_SELECT_ITEM, *P_EVENT_SELECT_ITEM;
	typedef CList <P_EVENT_SELECT_ITEM, P_EVENT_SELECT_ITEM &> EVENT_SELECT_LIST;
	typedef enum
	{
		EVT_ID_NONE = 0,
		EVT_ID_QUIT_EVENT_LOOP,
		EVT_ID_FD_READ,
		EVT_ID_FD_WRITE,
		EVT_ID_FD_EXCEPT,
		EVT_ID_TIMER,
		EVT_ID_USER,
	}EVENT_ID, *P_EVENT_ID;
	typedef struct
	{
		UINT uiTimerId;
		UINT uiTimeoutMS;
		UINT64 uiStartTimeMS;
	}TIMER_SET_ITEM, *P_TIMER_SET_ITEM;
	typedef CList <P_TIMER_SET_ITEM, P_TIMER_SET_ITEM &> TIMER_SET_LIST;
	typedef struct
	{
		UINT uiMsgId;
		UINT uiParam;
		PVOID pParam;
	}MSG_QUEUE_ITEM, *P_MSG_QUEUE_ITEM;
	typedef CList <P_MSG_QUEUE_ITEM, P_MSG_QUEUE_ITEM &> MSG_QUEUE_LIST;
public:
	CEventCenter();
	virtual ~CEventCenter();
	virtual INT DoEvtLoop();
	virtual INT EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam);
	virtual INT RegisterFdEventSelect(CONST int iFd, CONST EVENT_SELECT_FLAG eEvtSelFlags);
	virtual INT GetFdEventSelectFlag(CONST int iFd, EVENT_SELECT_FLAG & eEvtSelFlags);
	virtual INT UnregisterFdEventSelect(CONST int iFd);
	virtual INT RegisterTimer(CONST UINT uiTimeoutMS, OUT UINT & uiTimerId);
	virtual INT UnregisterTimer(CONST UINT uiTimerId);
	virtual INT GetMessage(MSG_QUEUE_ITEM & oMsgQueueItem);
	virtual INT PostMessage(UINT uiMsgId, UINT uiParam, PVOID pParam);
	virtual INT DispatchMessage(CONST MSG_QUEUE_ITEM & oMsgQueueItem);	
private:
	INT GetMinTimerWaitTimeMS(UINT & uiWaitTimeMS);
	INT GetTimerMessage(MSG_QUEUE_ITEM & oMsgQueueItem);
	void FixupEventSelectListing();
protected:
	EVENT_SELECT_LIST m_lstEvtSel;
	TIMER_SET_LIST m_lstTimerSet;
	MSG_QUEUE_LIST m_lstMsgQueue;
	CMutex2 m_mutexMsgQueue; 
	UINT m_uiLastRegisteredTimerId;
	CUnixDatagramSocket m_sckMsgQueueMonitor;
};

#endif	//_EVENT_CENTER_H_

