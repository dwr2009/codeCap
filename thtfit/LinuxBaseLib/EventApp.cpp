#include "EventApp.h"

static CEventApp * g_pEventApp = NULL;

CEventApp::CEventApp()
{
	g_pEventApp = this;
}

CEventApp::~CEventApp()
{
	g_pEventApp = NULL;
}

INT CEventApp::InitInstance(int argc, char * argv[])
{
	INT iOutRet = ERROR_SUCCESS;

	return iOutRet;
}

INT CEventApp::ExitInstance()
{
	INT iOutRet = ERROR_SUCCESS;

	return iOutRet;
}

INT CEventApp::Run(int argc, char * argv[])
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do{
		iOutRet = InitInstance(argc, argv);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		iOutRet = DoEvtLoop();
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	iRet = ExitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}

	return iOutRet;
}

CEventApp * GetApp()
{
	return g_pEventApp;
}

