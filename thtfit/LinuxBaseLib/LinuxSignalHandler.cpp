#include <LinuxSignalHandler_C.h>
#include <LinuxSignalHandler.h>
#include <memory.h>
#include <signal.h>

static CLinuxSignalHandler * g_pLinuxSignalHandler = NULL;

void CLinuxSignalHandler::SysSignalHandler(int iSigNo)
{
	P_LINUX_SIG_HANDLER_ITEM pLinuxSigHandlerItem = NULL;
	POSITION pPos = NULL;
	pthread_t CurThreadId = pthread_self();

	PRINT_BFILE_LINENO;
	printf("SysSignalHandler, Sig %d\n", iSigNo);

	do
	{
		if(NULL == g_pLinuxSignalHandler)
		{
			break;
		}

		if(g_pLinuxSignalHandler->m_bSigLocked)
		{
			g_pLinuxSignalHandler->m_iPendingSigNo = iSigNo;
			g_pLinuxSignalHandler->m_bSigDelayPending = TRUE;
			break;
		}
		
		pPos = g_pLinuxSignalHandler->m_lstLinuxSigHandlerItem.GetHeadPosition();
		while(pPos)
		{
			pLinuxSigHandlerItem = g_pLinuxSignalHandler->m_lstLinuxSigHandlerItem.GetNext(IN OUT pPos);
			if(pLinuxSigHandlerItem)
			{
				if(iSigNo == pLinuxSigHandlerItem->iSigNo && 
					SIG_RECV_THIS_THREAD == pLinuxSigHandlerItem->eSigRecvTargetType &&
					CurThreadId == pLinuxSigHandlerItem->pidTargetThread)
				{
					LINUX_SIG_INFO oLinuxSigInfo;
					ZeroMemory(&oLinuxSigInfo, sizeof(oLinuxSigInfo));
					oLinuxSigInfo.iSigNo = iSigNo;
					pLinuxSigHandlerItem->pfnSigHandlerCallback(pLinuxSigHandlerItem->pCallbackContext,
						&oLinuxSigInfo);
				}
			}
		}
	}while(FALSE);
}

CLinuxSignalHandler::CLinuxSignalHandler()
{
	if(g_pLinuxSignalHandler)
	{
		PRINT_BFILE_LINENO_BUG_STR;		
	}
	
	g_pLinuxSignalHandler = this;
	m_bSigDelayPending = FALSE;
	m_bSigLocked = FALSE;
}

CLinuxSignalHandler::~CLinuxSignalHandler()
{
	m_bSigDelayPending = FALSE;
	m_bSigLocked = TRUE;
	DestroySigHandlerList();
	g_pLinuxSignalHandler = NULL;
}

INT CLinuxSignalHandler::RegisterSigHandler(P_REG_SIG_HANDLER_PARAM_IN pRegSigHandlerParamIn)
{
	INT iOutRet = ERROR_SUCCESS;
	P_LINUX_SIG_HANDLER_ITEM pLinuxSigHandlerItem = NULL;
	POSITION pPos = NULL;
	BOOL_t bFound = FALSE;
	pthread_t CurThreadId = pthread_self();

	m_bSigLocked = TRUE;

	do
	{
		if(NULL == pRegSigHandlerParamIn)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		//find an old one
		bFound = FALSE;
		pPos = m_lstLinuxSigHandlerItem.GetHeadPosition();
		while(pPos)
		{
			pLinuxSigHandlerItem = m_lstLinuxSigHandlerItem.GetNext(IN OUT pPos);
			if(pLinuxSigHandlerItem->iSigNo == pRegSigHandlerParamIn->iSigNo &&
				pLinuxSigHandlerItem->eSigRecvTargetType == pRegSigHandlerParamIn->eSigRecvTargetType &&
				pLinuxSigHandlerItem->pidTargetThread == CurThreadId)
			{
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			pLinuxSigHandlerItem->pfnSigHandlerCallback = pRegSigHandlerParamIn->pfnSigHandlerCallback;
			pLinuxSigHandlerItem->pCallbackContext = pRegSigHandlerParamIn->pCallbackContext;
			pLinuxSigHandlerItem = NULL;	//detach
			break;
		}
		//create a new one
		pLinuxSigHandlerItem = new LINUX_SIG_HANDLER_ITEM;
		if(NULL == pLinuxSigHandlerItem)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pLinuxSigHandlerItem->iSigNo = pRegSigHandlerParamIn->iSigNo;
		pLinuxSigHandlerItem->eSigRecvTargetType = pRegSigHandlerParamIn->eSigRecvTargetType;
		pLinuxSigHandlerItem->pidTargetThread = CurThreadId;
		pLinuxSigHandlerItem->pfnSigHandlerCallback = pRegSigHandlerParamIn->pfnSigHandlerCallback;
		pLinuxSigHandlerItem->pCallbackContext = pRegSigHandlerParamIn->pCallbackContext;
		pPos = m_lstLinuxSigHandlerItem.AddTail(pLinuxSigHandlerItem);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pLinuxSigHandlerItem = NULL;	//detach
		//reg sys sig handler
		sighandler_t pRet = NULL;
		pRet = signal(pRegSigHandlerParamIn->iSigNo, SysSignalHandler);
		if(SIG_ERR == pRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		else if(pRet)
		{
			PRINT_BFILE_LINENO;
			printf("unimplemented multiple signal handlers\n");
		}
		/*
		PRINT_BFILE_LINENO;
		printf("RegSig %d\n", pRegSigHandlerParamIn->iSigNo);
		*/
	}while(FALSE);

	if(pLinuxSigHandlerItem)
	{
		delete pLinuxSigHandlerItem;
		pLinuxSigHandlerItem = NULL;
	}

	m_bSigLocked = FALSE;

	if(m_bSigDelayPending)
	{
		SysSignalHandler(m_iPendingSigNo);
		m_bSigDelayPending = FALSE;
	}

	return iOutRet;
}

INT CLinuxSignalHandler::UnregisterSigHandler(P_UNREG_SIG_HANDLER_PARAM_IN pUnregSigHandlerParamIn)
{
	INT iOutRet = ERROR_SUCCESS;
	pthread_t CurThreadId = pthread_self();
	POSITION pPos = NULL, pCurPos = NULL;
	P_LINUX_SIG_HANDLER_ITEM pLinuxSigHandlerItem = NULL;
	BOOL_t bFound = FALSE;

	m_bSigLocked = TRUE;

	do
	{
		if(NULL == pUnregSigHandlerParamIn)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		//find an existed one
		bFound = FALSE;
		pCurPos = NULL;
		pPos = m_lstLinuxSigHandlerItem.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pLinuxSigHandlerItem = m_lstLinuxSigHandlerItem.GetNext(IN OUT pPos);
			if(pLinuxSigHandlerItem &&
				pLinuxSigHandlerItem->iSigNo == pUnregSigHandlerParamIn->iSigNo &&
				pLinuxSigHandlerItem->pidTargetThread == CurThreadId)
			{
				bFound = TRUE;
				delete pLinuxSigHandlerItem;
				pLinuxSigHandlerItem = NULL;
				m_lstLinuxSigHandlerItem.RemoveAt(pCurPos);
			}
		}
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	m_bSigLocked = FALSE;

	if(m_bSigDelayPending)
	{
		SysSignalHandler(m_iPendingSigNo);
		m_bSigDelayPending = FALSE;
	}

	return iOutRet;
}

VOID CLinuxSignalHandler::DestroySigHandlerList()
{
	P_LINUX_SIG_HANDLER_ITEM pLinuxSigHandlerItem = NULL;
	POSITION pPos = NULL;

	do
	{
		pPos = m_lstLinuxSigHandlerItem.GetHeadPosition();
		while(pPos)
		{
			pLinuxSigHandlerItem = m_lstLinuxSigHandlerItem.GetNext(IN OUT pPos);
			if(pLinuxSigHandlerItem)
			{
				//reg sys sig handler
				sighandler_t pRet = NULL;
				pRet = signal(pLinuxSigHandlerItem->iSigNo, SIG_DFL);
				if(SIG_ERR == pRet)
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				//
				delete pLinuxSigHandlerItem;
				pLinuxSigHandlerItem = NULL;
			}
		}
		m_lstLinuxSigHandlerItem.RemoveAll();
	}while(FALSE);
}

