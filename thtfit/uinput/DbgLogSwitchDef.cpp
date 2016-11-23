#include "DbgLogSwitchDef.h"
#include <DbgLogSwitch.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <ErrPrintHelper.h>

#define	DbgLogSwDesc_LogDbugCmd				"LogDbusCmd"
#define	DbgLogSwDesc_LogUinput				"LogUinput"

static BOOL_t g_Disabled = FALSE;
BOOL_t * g_pLogDbusCmdSw = &g_Disabled;
BOOL_t * g_pLogUinputSw = &g_Disabled;

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
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogUinput);
		if(pbRet)
		{
			g_pLogUinputSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogUinput);
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

