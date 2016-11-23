#include "EventThread.h"
#include <unistd.h>
#include <memory.h>

CEventThread::CEventThread()
{

}

CEventThread::~CEventThread()
{

}

INT CEventThread::Run()
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	PTHREAD_CREATE_PARAMS oPThrdCreateParams;

	do{
		//create the thread		
		ZeroMemory(&oPThrdCreateParams, sizeof(oPThrdCreateParams));
		oPThrdCreateParams.iSizeOfThis = sizeof(oPThrdCreateParams);
		iRet = CThread::PThrdCreateParamInit(&oPThrdCreateParams);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERROR_PTHREAD_ATTR_INIT_FAIL;
			break;
		}
		oPThrdCreateParams.bDetached = TRUE; 
		iRet = CreateThread(&oPThrdCreateParams);
		if(ERROR_SUCCESS != iRet)	//failed to create the thread
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CEventThread::Stop()
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do{
		iRet = PostMessage(EVT_ID_QUIT_EVENT_LOOP, 0, NULL);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

#define	THREAD_END_CHK_INTERVAL_MS		50
		//wait for the termination of the thread
		INT iChkCnt = 0;
		while(TRUE)
		{
			if(getThreadHasEnded())
			{
				break;
			}
			usleep(THREAD_END_CHK_INTERVAL_MS*1000);
			iChkCnt++;
			if(0 == ((THREAD_END_CHK_INTERVAL_MS * iChkCnt) % (5*1000)))	//prompt once per 5s
			{
				LOG_BLINE("WaitForThread(%u) to exit timeout\n", GetThreadId());
			}
		}
	}while(FALSE);

	return iOutRet;
}

PVOID CEventThread::ThreadLoop()
{
	PVOID pOutRet = NULL;
	INT iRet;

	do
	{
		iRet = InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		iRet = DoEvtLoop();
	}while(FALSE);

	iRet = ExitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}

	return pOutRet;
}

INT CEventThread::InitInstance()
{
	INT iOutRet = ERROR_SUCCESS;

	return iOutRet;
}

INT CEventThread::ExitInstance()
{
	INT iOutRet = ERROR_SUCCESS;

	return iOutRet;
}

