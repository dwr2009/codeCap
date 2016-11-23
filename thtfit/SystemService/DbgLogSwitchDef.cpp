#include "DbgLogSwitchDef.h"
#include <DbgLogSwitch.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <ErrPrintHelper.h>

#define	DbgLogSwDesc_LogDbugCmd				"LogDbusCmd"
#define	DbgLogSwDesc_LogSysIoEvt			"LogSysIoEvt"
#define	DbgLogSwDesc_LogWifiConnection		"LogWifiConnection"

static BOOL_t g_Disabled = FALSE;
BOOL_t * g_pLogDbusCmdSw = &g_Disabled;
BOOL_t * g_pLogSysIoEvtSw = &g_Disabled;
BOOL_t * g_pLogWifiConnection = &g_Disabled;

INT_t RegDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t * pbRet = NULL;

	do
	{
		iRet = DbgLogSwitch_Init();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogDbugCmd);
		if(pbRet)
		{
			g_pLogDbusCmdSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogDbugCmd);
		}
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogSysIoEvt);
		if(pbRet)
		{
			g_pLogSysIoEvtSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogSysIoEvt);
		}
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogWifiConnection);
		if(pbRet)
		{
			g_pLogWifiConnection = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogWifiConnection);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t UnregDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = DbgLogSwitch_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

