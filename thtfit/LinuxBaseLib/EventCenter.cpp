#include "EventCenter.h"
#include <math2.h>
#include <DateTime.h>
#include <memory.h>
#include <unistd.h>

#define	ALLOWED_MAX_TIMER_CNT			(512)

CEventCenter::CEventCenter()
{	
	INT_t iRet;
	BOOL_t bAutoGenePath = TRUE;

	m_uiLastRegisteredTimerId = 0;

	iRet = m_sckMsgQueueMonitor.Create(NULL, bAutoGenePath);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}
	iRet = m_sckMsgQueueMonitor.setSocketSendBufSize(2*1024);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}
	/*
	{
		INT iSendBufSize;
		iRet = m_sckMsgQueueMonitor.getSocketSendBufSize(&iSendBufSize);
		if(ERROR_SUCCESS == iRet)
		{
			PRINT_FILE_LINENO;
			printf("SendBufSize=%d\n", iSendBufSize);
		}
		else
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
	}
	*/
	iRet = m_sckMsgQueueMonitor.setSocketRecvBufSize(2*1024);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}
	/*
	{
		INT iRecvBufSize;
		iRet = m_sckMsgQueueMonitor.getSocketRecvBufSize(&iRecvBufSize);
		if(ERROR_SUCCESS == iRet)
		{
			PRINT_FILE_LINENO;
			printf("RecvBufSize=%d\n", iRecvBufSize);
		}
		else
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
	}
	*/
}

CEventCenter::~CEventCenter()
{

}

INT CEventCenter::DoEvtLoop()
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	MSG_QUEUE_ITEM oMsgQueueItem;

	iRet = RegisterFdEventSelect(m_sckMsgQueueMonitor.getSocketHandle(), EVENT_SELECT_READ);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}

	while(TRUE)
	{
		iRet = GetMessage(OUT oMsgQueueItem);
		if(ERROR_SUCCESS == iRet)
		{
			if(EVT_ID_QUIT_EVENT_LOOP == oMsgQueueItem.uiMsgId)
			{				
				/*
				PRINT_FILE_LINENO;
				printf("EVT_ID_QUIT_EVENT_LOOP\n");
				*/
				break;
			}
			DispatchMessage(oMsgQueueItem);
		}
		else
		{
			if(ERROR_NOT_FOUND != iRet)
			{				
				PRINT_FILE_LINENO_IRET_CRT_ERR;
				break;
			}
		}
	}
	/*
	PRINT_FILE_LINENO;
	printf("CEventCenter::DoEvtLoop exit\n");
	*/
	return iOutRet;
}

INT CEventCenter::EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam)
{
	INT iOutRet = ERROR_SUCCESS;

	/*
	PRINT_FILE_LINENO;
	printf("EventProc, uiParam(%d)\n", uiParam);
	*/

	return iOutRet;
}

INT CEventCenter::RegisterFdEventSelect(CONST int iFd, CONST EVENT_SELECT_FLAG eEvtSelFlags)
{
	INT iOutRet = ERROR_SUCCESS;
	P_EVENT_SELECT_ITEM pEvtSelItem = NULL;
	POSITION pPos = NULL;
	BOOL_t bFound;

	do{
		if(0 > iFd)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		pPos = m_lstEvtSel.GetHeadPosition();
		while(pPos)
		{
			pEvtSelItem = m_lstEvtSel.GetNext(pPos);
			if(pEvtSelItem)
			{
				if(iFd == pEvtSelItem->iFd)
				{
					pEvtSelItem->eEvtSelFlags = eEvtSelFlags;
					bFound = TRUE;
					break;
				}
			}
		}
		pEvtSelItem = NULL;
		if(bFound)
		{
			break;
		}

		pEvtSelItem = new EVENT_SELECT_ITEM;
		if(NULL == pEvtSelItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pEvtSelItem->iFd = iFd;
		pEvtSelItem->eEvtSelFlags = eEvtSelFlags;
		pPos = m_lstEvtSel.AddTail(pEvtSelItem);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			goto EVT_SEL_ITEM_ADD_END;
		}
		pEvtSelItem = NULL;
EVT_SEL_ITEM_ADD_END:
		if(pEvtSelItem)
		{
			delete pEvtSelItem;
			pEvtSelItem = NULL;
		}	
	}while(FALSE);

	return iOutRet;	
}

INT CEventCenter::GetFdEventSelectFlag(CONST int iFd, EVENT_SELECT_FLAG & eEvtSelFlags)
{
	INT iOutRet = ERROR_SUCCESS;
	P_EVENT_SELECT_ITEM pEvtSelItem = NULL;
	POSITION pPos = NULL;
	BOOL_t bFound;

	do{
		if(0 > iFd)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		pPos = m_lstEvtSel.GetHeadPosition();
		while(pPos)
		{
			pEvtSelItem = m_lstEvtSel.GetNext(pPos);
			if(pEvtSelItem)
			{
				if(iFd == pEvtSelItem->iFd)
				{
					eEvtSelFlags = pEvtSelItem->eEvtSelFlags;
					bFound = TRUE;
					break;
				}
			}
		}
		pEvtSelItem = NULL;
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	return iOutRet; 
}

INT CEventCenter::UnregisterFdEventSelect(CONST int iFd)
{
	INT iOutRet = ERROR_SUCCESS;
	P_EVENT_SELECT_ITEM pEvtSelItem = NULL;
	POSITION pPos = NULL, pCurPos;
	BOOL_t bFound;

	do{
		if(0 > iFd)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		pPos = m_lstEvtSel.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pEvtSelItem = m_lstEvtSel.GetNext(pPos);
			if(pEvtSelItem)
			{
				if(iFd == pEvtSelItem->iFd)
				{
					m_lstEvtSel.RemoveAt(pCurPos);
					delete pEvtSelItem;
					pEvtSelItem = NULL;
					bFound = TRUE;
					break;
				}
			}
		}
		pEvtSelItem = NULL;
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	return iOutRet; 
}

INT CEventCenter::RegisterTimer(CONST UINT uiTimeoutMS, OUT UINT & uiTimerId)
{
	INT iOutRet = ERROR_SUCCESS;
	UINT uiNewTimerId;
	POSITION pPos = NULL, pCurPos;
	P_TIMER_SET_ITEM pTimerSetItem = NULL, pNewTimerSetItem = NULL;
	BOOL bFound;
	UINT64 uiCurTimeMS = GetCurTimeMs();

	do
	{
		if(ALLOWED_MAX_TIMER_CNT <= m_lstTimerSet.GetCount())
		{
			iOutRet = ERR_NO_MORE_RESOURCE;
			break;
		}

		m_uiLastRegisteredTimerId++;
		uiNewTimerId = m_uiLastRegisteredTimerId;
		if(0 == uiNewTimerId)
		{
			uiNewTimerId = 1;
		}
		
		while(TRUE)
		{
			bFound = FALSE;
			pPos = m_lstTimerSet.GetHeadPosition();
			while(pPos)
			{
				pTimerSetItem = m_lstTimerSet.GetNext(pPos);
				if(pTimerSetItem)
				{
					if(uiNewTimerId == pTimerSetItem->uiTimerId)
					{
						bFound = TRUE;
						break;
					}
				}
			}
			if(FALSE == bFound)
			{
				break;
			}
			uiNewTimerId++;
			if(uiNewTimerId == m_uiLastRegisteredTimerId)
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		
		pNewTimerSetItem = new TIMER_SET_ITEM;
		if(NULL == pNewTimerSetItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pNewTimerSetItem->uiTimerId = uiNewTimerId;
		pNewTimerSetItem->uiTimeoutMS = uiTimeoutMS;
		pNewTimerSetItem->uiStartTimeMS = uiCurTimeMS;

		pCurPos = NULL;
		pPos = m_lstTimerSet.GetHeadPosition();
		if(pPos)
		{
			while(pPos)
			{
				pCurPos = pPos;
				pTimerSetItem = m_lstTimerSet.GetNext(pPos);
				if(pTimerSetItem)
				{
					UINT uiLeftTimeMS = 0;
					if(pTimerSetItem->uiTimeoutMS >= (uiCurTimeMS - pTimerSetItem->uiStartTimeMS))
					{
						uiLeftTimeMS = pTimerSetItem->uiTimeoutMS - (uiCurTimeMS - pTimerSetItem->uiStartTimeMS);
					}
					else
					{
						uiLeftTimeMS = 0;
					}
					if(uiLeftTimeMS > uiTimeoutMS)
					{
						pPos = m_lstTimerSet.InsertBefore(pCurPos, pNewTimerSetItem);
						break;
					}
				}
				if(NULL == pPos)
				{
					pPos = m_lstTimerSet.AddTail(pNewTimerSetItem);
					break;
				}
			}
		}
		else
		{
			pPos = m_lstTimerSet.AddTail(pNewTimerSetItem);
		}
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			goto TIMER_SET_ITEM_ADD_END;
		}
		pNewTimerSetItem = NULL;
		uiTimerId = uiNewTimerId;
		m_uiLastRegisteredTimerId = uiTimerId;
TIMER_SET_ITEM_ADD_END:
		if(pNewTimerSetItem)
		{
			delete pNewTimerSetItem;
			pNewTimerSetItem = NULL;
		}	
	}while(FALSE);

	return iOutRet;
}

INT CEventCenter::UnregisterTimer(CONST UINT uiTimerId)
{
	INT iOutRet = ERROR_SUCCESS;
	POSITION pPos = NULL, pCurPos = NULL;
	P_TIMER_SET_ITEM pTimerSetItem = NULL;
	BOOL bFound;

	do{
		bFound = FALSE;
		pPos = m_lstTimerSet.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pTimerSetItem = m_lstTimerSet.GetNext(pPos);
			if(pTimerSetItem)
			{
				if(uiTimerId == pTimerSetItem->uiTimerId)
				{
					m_lstTimerSet.RemoveAt(pCurPos);
					delete pTimerSetItem;
					pTimerSetItem = NULL;
					bFound = TRUE;
					break;
				}
			}
		}
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CEventCenter::PostMessage(UINT uiMsgId, UINT uiParam, PVOID pParam)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	P_MSG_QUEUE_ITEM pMsgQueueItem = NULL;
	POSITION pPos;

	do{
		pMsgQueueItem = new MSG_QUEUE_ITEM;
		if(NULL == pMsgQueueItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pMsgQueueItem->uiMsgId = uiMsgId;
		pMsgQueueItem->uiParam = uiParam;
		pMsgQueueItem->pParam = pParam;
		{
			INT iRet;
			iRet = m_mutexMsgQueue.Lock();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
			}
		}
		pPos = m_lstMsgQueue.AddTail(pMsgQueueItem);
		{
			INT iRet;
			iRet = m_mutexMsgQueue.Unlock();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
			}
		}
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			goto ADD_MESSAGE_END;
		}
		pMsgQueueItem = NULL;	//detached

		{
			struct sockaddr_un sckaddrPeer;
			ZeroMemory(&sckaddrPeer, sizeof(sckaddrPeer));
			iRet = m_sckMsgQueueMonitor.getSockName(&sckaddrPeer);
			if(ERROR_SUCCESS == iRet)
			{
				char szBuf[1];
				socklen_t SckNameLen = sizeof(sckaddrPeer);
				iRet = m_sckMsgQueueMonitor.SendTo(szBuf, sizeof(szBuf), (CONST SOCKADDR *)(&sckaddrPeer),
					SckNameLen);
				if(1 != iRet)
				{
					PRINT_FILE_LINENO_IRET_CRT_ERR;
				}
			}
			else
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
			}
		}
		/*		
		PRINT_FILE_LINENO;
		printf("CEventCenter::PostMessage exit\n");
		*/
ADD_MESSAGE_END:
		if(pMsgQueueItem)
		{
			delete pMsgQueueItem;
			pMsgQueueItem = NULL;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT CEventCenter::DispatchMessage(CONST MSG_QUEUE_ITEM & oMsgQueueItem)
{
	INT iRet = 0, iOutRet = 0;
	
	if(EVT_ID_FD_READ == oMsgQueueItem.uiMsgId && 
		(UINT)m_sckMsgQueueMonitor.getSocketHandle() == oMsgQueueItem.uiParam)
	{
		char szBuf[1024];
		iRet = m_sckMsgQueueMonitor.RecvFrom(szBuf, sizeof(szBuf), NULL, NULL, MSG_DONTWAIT);
		if(0 >= iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
		else
		{
			/*
			PRINT_FILE_LINENO;
			printf("recved %d byte(s)\n", iRet);
			*/
		}
	}
	else
	{
		iOutRet = EventProc(oMsgQueueItem.uiMsgId, oMsgQueueItem.uiParam, oMsgQueueItem.pParam);		
	}

	return iOutRet;
}

INT CEventCenter::GetMessage(MSG_QUEUE_ITEM & oMsgQueueItem)
{
	INT iOutRet = ERROR_NOT_FOUND, iRet;
	P_MSG_QUEUE_ITEM pMsgQueueItem = NULL;
	fd_set fdsRead, fdsWrite, fdsExcept;
	P_EVENT_SELECT_ITEM pEvtSelItem = NULL;
	POSITION pPos = NULL;
	int iMaxFd = -1;
	UINT uiTimeoutMS;
	struct timeval timeTimeout, *ptimeTimeout;

	do{
		/*
		PRINT_FILE_LINENO;
		printf("CEventCenter::GetMessage enter\n");
		*/
		ZeroMemory(&oMsgQueueItem, sizeof(oMsgQueueItem));
		{
			INT iRet;
			iRet = m_mutexMsgQueue.Lock();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
			}
		}
		iRet = m_lstMsgQueue.GetHeadAndRemove(&pMsgQueueItem);
		{
			INT iRet;
			iRet = m_mutexMsgQueue.Unlock();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_CRT_ERR;
			}
		}
		if(ERROR_SUCCESS == iRet)
		{
			/*
			PRINT_FILE_LINENO;
			printf("get a msg from msg_queue\n");
			*/
			if(pMsgQueueItem)
			{
				oMsgQueueItem = *pMsgQueueItem;
				delete pMsgQueueItem;
				pMsgQueueItem = NULL;
				iOutRet = ERROR_SUCCESS;
				break;
			}
		}

		FD_ZERO(&fdsRead);
		FD_ZERO(&fdsWrite);
		FD_ZERO(&fdsExcept);
		pPos = m_lstEvtSel.GetHeadPosition();
		while(pPos)
		{
			pEvtSelItem = m_lstEvtSel.GetNext(pPos);
			if(pEvtSelItem)
			{
				iMaxFd = GetMaxVal(iMaxFd, pEvtSelItem->iFd);
				if(pEvtSelItem->eEvtSelFlags & EVENT_SELECT_READ)
				{
					FD_SET(pEvtSelItem->iFd, &fdsRead);
				}
				if(pEvtSelItem->eEvtSelFlags & EVENT_SELECT_WRITE)
				{
					FD_SET(pEvtSelItem->iFd, &fdsWrite);
				}
				if(pEvtSelItem->eEvtSelFlags & EVENT_SELECT_EXCEPT)
				{
					FD_SET(pEvtSelItem->iFd, &fdsExcept);
				}
			}
		}
				
		iRet = GetMinTimerWaitTimeMS(OUT uiTimeoutMS);
		if(ERROR_SUCCESS == iRet)
		{
			timeTimeout.tv_sec = uiTimeoutMS / 1000;
			timeTimeout.tv_usec = (uiTimeoutMS % 1000) * 1000;
			ptimeTimeout = &timeTimeout;
		}
		else
		{
			ptimeTimeout = NULL;
		}
		iRet = select(iMaxFd+1, &fdsRead, &fdsWrite, &fdsExcept, ptimeTimeout);
		if(0 < iRet)
		{
			pPos = m_lstEvtSel.GetHeadPosition();
			while(pPos)
			{
				pEvtSelItem = m_lstEvtSel.GetNext(pPos);
				if(pEvtSelItem)
				{
					if(FD_ISSET(pEvtSelItem->iFd, &fdsRead))
					{
						oMsgQueueItem.uiMsgId = EVT_ID_FD_READ;
						oMsgQueueItem.uiParam = pEvtSelItem->iFd;
						oMsgQueueItem.pParam = NULL;				
						iOutRet = ERROR_SUCCESS;
						break;
					}
					if(FD_ISSET(pEvtSelItem->iFd, &fdsWrite))
					{
						oMsgQueueItem.uiMsgId = EVT_ID_FD_WRITE;
						oMsgQueueItem.uiParam = pEvtSelItem->iFd;
						oMsgQueueItem.pParam = NULL;
						iOutRet = ERROR_SUCCESS;
						break;
					}
					if(FD_ISSET(pEvtSelItem->iFd, &fdsExcept))
					{
						oMsgQueueItem.uiMsgId = EVT_ID_FD_EXCEPT;
						oMsgQueueItem.uiParam = pEvtSelItem->iFd;
						oMsgQueueItem.pParam = NULL;
						iOutRet = ERROR_SUCCESS;
						break;
					}
				}
			}			
		}
		else if(0 == iRet)	//timeout
		{
			iOutRet = GetTimerMessage(oMsgQueueItem);
		}
		else
		{
			if(EINTR != errno)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			FixupEventSelectListing();
		}
	}while(FALSE);

	return iOutRet;
}

INT CEventCenter::GetMinTimerWaitTimeMS(UINT & uiWaitTimeMS)
{
	INT iOutRet = ERROR_SUCCESS;
	UINT64 uiCurTimeMS = GetCurTimeMs();
	UINT uiElapsedTimeMS;
	POSITION pPos, pCurPos;
	P_TIMER_SET_ITEM pTimerSetItem = NULL;
	INT iWaitTimeMS = 0;

	do{
		pPos = m_lstTimerSet.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pTimerSetItem = m_lstTimerSet.GetNext(pPos);
			if(pTimerSetItem)
			{
				if(pTimerSetItem->uiStartTimeMS > uiCurTimeMS)
				{
					pTimerSetItem->uiStartTimeMS = uiCurTimeMS;
				}
			}
			else
			{
				m_lstTimerSet.RemoveAt(pCurPos);
			}
		}
		
		pPos = m_lstTimerSet.GetHeadPosition();
		if(pPos)
		{
			pTimerSetItem = m_lstTimerSet.GetNext(pPos);
			uiElapsedTimeMS = uiCurTimeMS - pTimerSetItem->uiStartTimeMS;
			if(uiElapsedTimeMS >=  pTimerSetItem->uiTimeoutMS)
			{
				iWaitTimeMS = 0;
			}
			else
			{
				iWaitTimeMS = pTimerSetItem->uiTimeoutMS - uiElapsedTimeMS;
			}
		}
		else
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		uiWaitTimeMS = iWaitTimeMS;
	}while(FALSE);

	return iOutRet;
}

INT CEventCenter::GetTimerMessage(MSG_QUEUE_ITEM & oMsgQueueItem)
{
	INT iOutRet = ERROR_NOT_FOUND;
	UINT64 uiCurTimeMS = GetCurTimeMs();
	POSITION pPos, pCurPos;
	P_TIMER_SET_ITEM pTimerSetItem = NULL;
	UINT uiElapsedTimeMS;

	do{
		pPos = m_lstTimerSet.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pTimerSetItem = m_lstTimerSet.GetNext(pPos);
			if(pTimerSetItem)
			{				
				uiElapsedTimeMS = uiCurTimeMS - pTimerSetItem->uiStartTimeMS;
				if(uiElapsedTimeMS >= pTimerSetItem->uiTimeoutMS)
				{
					m_lstTimerSet.RemoveAt(pCurPos);
					oMsgQueueItem.uiMsgId = EVT_ID_TIMER;
					oMsgQueueItem.uiParam = pTimerSetItem->uiTimerId;
					oMsgQueueItem.pParam = NULL;
					delete pTimerSetItem;
					pTimerSetItem = NULL;
					iOutRet = ERROR_SUCCESS;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

void CEventCenter::FixupEventSelectListing()
{
	int iRet;
	POSITION pPos = NULL, pCurPos = NULL;
	P_EVENT_SELECT_ITEM pEventSelectItem = NULL;

	pPos = m_lstEvtSel.GetHeadPosition();
	while(pPos)
	{
		pCurPos = pPos;
		pEventSelectItem = m_lstEvtSel.GetNext(pPos);
		if(pEventSelectItem)
		{
			fd_set fdsRead;
			struct timeval tvTimeout;

			FD_ZERO(&fdsRead);
			FD_SET(pEventSelectItem->iFd, &fdsRead);
			tvTimeout.tv_usec = 0;
			tvTimeout.tv_sec = 0;

			iRet = select(pEventSelectItem->iFd + 1, &fdsRead, NULL, NULL, &tvTimeout);
			if(0 > iRet)
			{	
				m_lstEvtSel.RemoveAt(pCurPos);
				delete pEventSelectItem;
				pEventSelectItem = NULL;
			}
		}
		else
		{
			m_lstEvtSel.RemoveAt(pCurPos);
		}
	}
}

