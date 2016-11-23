#include <ThreadMessageQueue.h>
#include <Compiler.h>
#include <DateTime.h>

#define	MSG_QUEUE_MAX_ITEM_NUM			1000

CMessageQueueItem::CMessageQueueItem()
{
	//LogFileLineFormat("CMessageQueueItem()");
}

CMessageQueueItem::~CMessageQueueItem()
{
	//LogFileLineFormat("~CMessageQueueItem()");

	int iRet;
	
	iRet = m_EventOnProcessed.SetEvent();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_CRT_ERRINFO;
	}
}

INT CMessageQueueItem::ResetEvent()
{
	return m_EventOnProcessed.ResetEvent();
}

INT CMessageQueueItem::SetEvent()
{
	return m_EventOnProcessed.SetEvent();
}

INT CMessageQueueItem::WaitEvent()
{
	return m_EventOnProcessed.WaitEvent();
}

INT CMessageQueueItem::WaitEventTimeout(INT iTimeoutMs)
{
	return m_EventOnProcessed.WaitEventTimeout(iTimeoutMs);
}

INT CMessageQueueItem::SetMessageData(SharedPtr<CGenericMsgData> & GenericMsgData)
{
	INT iOutRet = ERROR_SUCCESS;

	do
	{
		m_GenericMsgData = GenericMsgData;		
	}while(FALSE);

	return iOutRet;
}

SharedPtr<CGenericMsgData> CMessageQueueItem::GetMessageData()
{
	return m_GenericMsgData;
}

/*	--------------------	CThreadMessageQueue		------------------------------	*/
CThreadMessageQueue::CThreadMessageQueue()
{
	m_bEventListenerIsValid = FALSE;
}

CThreadMessageQueue::~CThreadMessageQueue()
{
	INT iRet = 0;

	iRet = m_MsgQueueEvent.SetEvent();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_CRT_ERRINFO;
	}
}

INT_t CThreadMessageQueue::setEventListener(CONST WeakPtr <CThreadMessageQueue::IEventListener> EventListener_wp)
{
	INT_t iOutRet = ERR_SUCCESS;

	m_EventListener_wp = EventListener_wp;
	m_bEventListenerIsValid = TRUE;

	return iOutRet;
}

INT CThreadMessageQueue::PostMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	POSITION pPos = NULL;
	BOOL_t bLocked = FALSE;

	iRet = m_MsgQueueItemListLocker.Lock();
	if(ERROR_SUCCESS == iRet)
	{
		bLocked = TRUE;
	}
	else
	{
		PRINT_BFILE_LINENO_CRT_ERRINFO;
	}

	do
	{
		if(NULL == MsgQueueItem_sp.get())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(CC_UNLIKELY(MSG_QUEUE_MAX_ITEM_NUM <= m_MsgQueueItemList.GetCount()))
		{
			iOutRet = ERR_BUSY;
			break;
		}
		pPos = m_MsgQueueItemList.AddTail(MsgQueueItem_sp);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_MsgQueueEvent.SetEvent();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}

	if(m_bEventListenerIsValid)
	{
		SharedPtr <IEventListener> EventListener_sp = m_EventListener_wp.Promote();
		if(FALSE == EventListener_sp.isNull())
		{
			EventListener_sp->onNewMessageInQueue();
		}
		else
		{
			m_EventListener_wp.Clear();
			m_bEventListenerIsValid = FALSE;
		}
	}

	return iOutRet;
}

INT CThreadMessageQueue::SendMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{		
		if(NULL == MsgQueueItem_sp.get())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = MsgQueueItem_sp->ResetEvent();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iOutRet = PostMessage(MsgQueueItem_sp);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		iRet = MsgQueueItem_sp->WaitEvent();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CThreadMessageQueue::SendMessageTimeout(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp, 
	INT_t iTimeoutMs)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{		
		if(NULL == MsgQueueItem_sp.get())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = MsgQueueItem_sp->ResetEvent();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iOutRet = PostMessage(MsgQueueItem_sp);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		iRet = MsgQueueItem_sp->WaitEventTimeout(iTimeoutMs);
		if(ERROR_SUCCESS != iRet)	//maybe ERR_TIMEOUT
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CThreadMessageQueue::GetMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	MsgQueueItem_sp.Release();

	do
	{		
TRY_GET_NEXT_MSG:
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		
		if(0 < m_MsgQueueItemList.GetCount())
		{
			iRet = m_MsgQueueItemList.GetHeadAndRemove(&MsgQueueItem_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
			break;
		}
		else // no msg in queue
		{
			iRet = m_MsgQueueEvent.ResetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
		}
		
		if(bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Unlock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = FALSE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		iRet = m_MsgQueueEvent.WaitEvent();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		goto TRY_GET_NEXT_MSG;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
	
	return iOutRet;
}

INT CThreadMessageQueue::GetMessageTimeout(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp, 
	INT_t iTimeoutMs)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	MsgQueueItem_sp.Release();

	do
	{		
TRY_GET_NEXT_MSG:
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		
		if(0 < m_MsgQueueItemList.GetCount())
		{
			iRet = m_MsgQueueItemList.GetHeadAndRemove(&MsgQueueItem_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
			//printf("m_uiMsgId=%d\n",MsgQueueItem_sp->GetMessageData()->m_uiMsgId);
			break;
		}
		else // no msg in queue
		{
			iRet = m_MsgQueueEvent.ResetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}
		}

		if(bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Unlock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = FALSE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		iRet = m_MsgQueueEvent.WaitEventTimeout(iTimeoutMs);
		if(ERROR_SUCCESS != iRet)	//maybe ERR_TIMEOUT		
		{
			iOutRet = iRet;
			break;
		}

		goto TRY_GET_NEXT_MSG;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
	
	return iOutRet;
}

INT CThreadMessageQueue::PeekMessage(SharedPtr<CMessageQueueItem> & MsgQueueItem_sp)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	do
	{		
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		
		if(0 < m_MsgQueueItemList.GetCount())
		{
			iRet = m_MsgQueueItemList.GetHeadAndRemove(&MsgQueueItem_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
			break;
		}
		else
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
	
	return iOutRet;
}

INT CThreadMessageQueue::RemoveAllMessage()
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;

	do
	{		
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		
		m_MsgQueueItemList.RemoveAll();
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
	
	return iOutRet;
}

BOOL_t CThreadMessageQueue::HaveMessage()
{
	BOOL_t bOutRet = FALSE;
	INT_t iRet;
	BOOL_t bLocked = FALSE;

	do
	{
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		if(0 < m_MsgQueueItemList.GetCount())
		{
			bOutRet = TRUE;
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}

	return bOutRet;
}

INT_t CThreadMessageQueue::WaitMessageTimeout(INT_t TimeoutMs)
{

	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	BOOL_t bLocked = FALSE;
	INT_t LeftTimeoutMs = TimeoutMs;
	UINT64_t StartTimeMs = GetSysUpTimeMs(), CurTimeMs;

	do
	{		
TRY_GET_NEXT_MSG:
		if(FALSE == bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		//LOG_BLINE("MsgCnt=%d\n", m_MsgQueueItemList.GetCount());
		if(0 < m_MsgQueueItemList.GetCount())
		{
			break;
		}
		else
		{
			iRet = m_MsgQueueEvent.ResetEvent();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_CRT_ERR;
			}

			CurTimeMs = GetSysUpTimeMs();
			LeftTimeoutMs -= (CurTimeMs - StartTimeMs);
			if(0 >= LeftTimeoutMs)
			{
				iOutRet = ERR_TIMEOUT;
				break;
			}
		}
		
		if(bLocked)
		{
			iRet = m_MsgQueueItemListLocker.Unlock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = FALSE;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}

		iRet = m_MsgQueueEvent.WaitEventTimeout(TimeoutMs);
		if(ERROR_SUCCESS != iRet)	//maybe ERR_TIMEOUT		
		{
			iOutRet = iRet;
			break;
		}

		//ERROR_SUCCESS, need re-check the queue
		goto TRY_GET_NEXT_MSG;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_MsgQueueItemListLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}
	
	return iOutRet;
}

