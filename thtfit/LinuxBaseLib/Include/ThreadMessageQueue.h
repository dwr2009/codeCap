#ifndef	_THREAD_MESSAGE_QUEUE_H_
#define	_THREAD_MESSAGE_QUEUE_H_

#include <GenericMsgData.h>
#include <EventEx.h>
#include <BaseList.h>
#include <Mutex2.h>
#include <SharedPtr.h>
#include <ErrChkMutex.h>

using namespace CppBase;

class CMessageQueueItem
{
public:
	CMessageQueueItem();
	virtual ~CMessageQueueItem();
	virtual INT ResetEvent();
	virtual INT SetEvent();
	virtual INT WaitEvent();
	virtual INT WaitEventTimeout(INT_t iTimeoutMs);
	virtual INT SetMessageData(SharedPtr<CGenericMsgData> & GenericMsgData);
	virtual SharedPtr<CGenericMsgData> GetMessageData();
private:
	SharedPtr<CGenericMsgData> m_GenericMsgData;	//sp, child pointer
	CEventEx m_EventOnProcessed;
};

typedef SharedPtr<CMessageQueueItem> MSG_QUEUE_ITEM_sp;
typedef CList <MSG_QUEUE_ITEM_sp, MSG_QUEUE_ITEM_sp> MSG_QUEUE_ITEM_LIST;

class CThreadMessageQueue
{
public:
	class IEventListener
	{
	public:
		virtual ~IEventListener() {;};
		virtual void onNewMessageInQueue() = 0;
	};
public:
	CThreadMessageQueue();
	virtual ~CThreadMessageQueue();
	virtual INT_t setEventListener(CONST WeakPtr <CThreadMessageQueue::IEventListener> EventListener_wp);
	virtual INT PostMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp);
	virtual INT SendMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp);
	virtual INT SendMessageTimeout(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp, 
		INT_t iTimeoutMs);
	virtual INT GetMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp);
	virtual INT GetMessageTimeout(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp, 
		INT_t iTimeoutMs);
	virtual INT PeekMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp);
	virtual INT RemoveAllMessage();
	virtual BOOL_t HaveMessage();
	virtual INT_t WaitMessageTimeout(INT_t TimeoutMs);
private:
	MSG_QUEUE_ITEM_LIST m_MsgQueueItemList;
	CErrChkMutex m_MsgQueueItemListLocker;
	CEventEx m_MsgQueueEvent;
	WeakPtr <IEventListener> m_EventListener_wp;
	BOOL_t m_bEventListenerIsValid;
};

#endif	//_THREAD_MESSAGE_QUEUE_H_

