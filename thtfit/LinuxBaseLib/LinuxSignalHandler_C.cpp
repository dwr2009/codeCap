#include <LinuxSignalHandler_C.h>
#include <LinuxSignalHandler.h>

static BOOL_t g_bInitialized = FALSE;
static CLinuxSignalHandler * g_pLinuxSigHandler = NULL;

INT LinuxSigHandler_Init()
{
	INT iOutRet = ERROR_SUCCESS;

	if(g_bInitialized)
	{
		iOutRet = ERROR_ALREADY_INITED;
		goto EXIT_PROC;
	}

	if(NULL == g_pLinuxSigHandler)
	{
		g_pLinuxSigHandler = new CLinuxSignalHandler;
		if(NULL == g_pLinuxSigHandler)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			goto EXIT_PROC;
		}
	}
	g_bInitialized = TRUE;
	
EXIT_PROC:	
	return iOutRet;
}

INT LinuxSigHandler_RegSigHandler(P_REG_SIG_HANDLER_PARAM_IN pRegSigHandlerParamIn)
{
	INT iOutRet = ERROR_SUCCESS;

	if(NULL == g_pLinuxSigHandler)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iOutRet = g_pLinuxSigHandler->RegisterSigHandler(pRegSigHandlerParamIn);

EXIT_PROC:
	return iOutRet;
}

INT LinuxSigHandler_UnregSigHandler(P_UNREG_SIG_HANDLER_PARAM_IN pUnregSigHandlerParamIn)
{
	INT iOutRet = ERROR_SUCCESS;

	if(NULL == g_pLinuxSigHandler)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	iOutRet = g_pLinuxSigHandler->UnregisterSigHandler(pUnregSigHandlerParamIn);

EXIT_PROC:
	return iOutRet;
}

INT LinuxSigHandler_Uninit()
{
	INT iOutRet = ERROR_SUCCESS;

	if(FALSE == g_bInitialized)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(g_pLinuxSigHandler)
	{
		delete g_pLinuxSigHandler;
		g_pLinuxSigHandler = NULL;
	}

	g_bInitialized = FALSE;

EXIT_PROC:
	return iOutRet;
}

