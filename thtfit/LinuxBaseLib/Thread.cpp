#include 	"Thread.h"
#include	<stdio.h>
#include	<ErrPrintHelper.h>
#include	<BaseErrDef.h>
#include	<errno.h>
#include	<string.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>
#include <Compiler.h>

CThread::CThread()
{
	m_iThreadId = 0;
	m_bThreadHasEnded = TRUE;
	m_bThreadIsRunning = FALSE;
	m_bThrdAttrHasInit = FALSE;	
}

CThread::~CThread()
{
	int iRet = 0;
	if(m_bThrdAttrHasInit)
	{
		iRet = pthread_attr_destroy(&m_oThreadAttr);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
		m_bThrdAttrHasInit = FALSE;
	}
}

INT CThread::CreateThread(P_PTHREAD_CREATE_PARAMS pPThrdCreateParams/*NULL*/)
{
	int iOutRet = 0, iRet = 0;

	do{
		if(pPThrdCreateParams)
		{
			if(sizeof(PTHREAD_CREATE_PARAMS) != pPThrdCreateParams->iSizeOfThis)
			{
				PRINT_FILE_LINENO;
				printf("iSizeOfThis=%d, should be %d\n", pPThrdCreateParams->iSizeOfThis, 
					sizeof(PTHREAD_CREATE_PARAMS));
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}

		if(m_bThrdAttrHasInit)
		{
			iRet = pthread_attr_destroy(&m_oThreadAttr);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_BUG_STR;
			}
			m_bThrdAttrHasInit = FALSE;
		}

		iRet = pthread_attr_init(&m_oThreadAttr);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_BUG_STR;
			iOutRet = ERROR_PTHREAD_ATTR_INIT_FAIL;
			break;
		}
		m_bThrdAttrHasInit = TRUE;
		
		if(pPThrdCreateParams)
		{
			if(pPThrdCreateParams->bDetached)
			{
				iRet = pthread_attr_setdetachstate(&m_oThreadAttr, PTHREAD_CREATE_DETACHED);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = ERROR_PTHREAD_ATTR_SET_FAIL;
					break;
				}
			}	
			
			if(pPThrdCreateParams->ulStackSize)
			{
				size_t ulStackSize = 0;
				int iPageSize = getpagesize();
				ulStackSize = (pPThrdCreateParams->ulStackSize + (iPageSize-1)) / iPageSize * iPageSize;		
				iRet = pthread_attr_setstacksize(&m_oThreadAttr, ulStackSize);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = ERROR_PTHREAD_ATTR_SET_FAIL;
					break;
				}
			}
		}

		iRet = pthread_create(&m_iThreadId, &m_oThreadAttr, ThreadProc, this);
		if(0 == iRet)
		{
			m_bThreadHasEnded = false;
			m_bThreadIsRunning = TRUE;
		}
		else
		{
			iOutRet = ERROR_PTHREAD_CREATE_FAIL;
			break;
		}
	}while(FALSE);
	
	iOutRet = iRet;

	return iOutRet;
}

BOOL_t CThread::GetThreadHasEnded()
{
	return m_bThreadHasEnded;
}

BOOL_t CThread::getThreadHasEnded()
{
	return m_bThreadHasEnded;
}

BOOL_t CThread::getThreadIsRunning()
{
	return m_bThreadIsRunning;
}

PVOID CThread::ThreadLoop()
{
	void * pOutRet = 0;


	return pOutRet;
}

INT CThread::SetThreadDetach()
{
	int iOutRet = 0, iRet = 0;

	iRet = pthread_detach(m_iThreadId);
	iOutRet = iRet;

	return iOutRet;
}

pthread_t CThread::GetThreadId()
{
	return m_iThreadId;
}

INT CThread::PThrdCreateParamInit(P_PTHREAD_CREATE_PARAMS pPThrdCreateParams)
{
	INT iOutRet = ERROR_SUCCESS;

	if(NULL == pPThrdCreateParams) {return ERROR_INVALID_PARAMETER;}

	if(sizeof(PTHREAD_CREATE_PARAMS) != pPThrdCreateParams->iSizeOfThis)
	{
		PRINT_FILE_LINENO_BUG_STR;
		return ERROR_INVALID_PARAMETER;
	}

	pPThrdCreateParams->bDetached = FALSE;
#ifdef	__ARM_ARCH_4T__
	pPThrdCreateParams->ulStackSize = (16*1024);
#else
	pPThrdCreateParams->ulStackSize = 0;	//default
#endif

	return iOutRet;
}

PVOID CThread::ThreadProc(PVOID pPara)
{
	void * pOutRet = NULL;	
	CThread * pThread = NULL;
	INT iRet = ERROR_SUCCESS;

	//设置立即Cancel线程
	iRet = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
	
	pThread = (CThread *)pPara;
	
	if(pThread)
	{
		pOutRet = pThread->ThreadLoop();
		pThread->m_bThreadHasEnded = TRUE;
		pThread->m_bThreadIsRunning = FALSE;
	}
	else 
	{
		PRINT_FILE_LINENO;
		printf("invalid argument\n");
	}
	/*	
	PRINT_FILE_LINENO;
	printf("CThread::ThreadProc exit\n");
	*/
	return pOutRet;
}

VOID CThread::Interrupt()
{
	int iRet;

	if(0 < m_iThreadId)
	{
		iRet = pthread_kill(m_iThreadId, SIGCHLD);
		if(CC_UNLIKELY(0 != iRet && ESRCH != iRet))
		{
			LOG_LINE("SendSigToThrd,%s\n", strerror(errno));
		}
	}
}

VOID CThread::Interrupt(pthread_t threadId)
{
	int iRet;

	if(0 < threadId)
	{
		iRet = pthread_kill(threadId, SIGCHLD);
		if(CC_UNLIKELY(0 != iRet && ESRCH != iRet))
		{
			LOG_LINE("SendSigToThrd,%s\n", strerror(errno));
		}
	}
}


