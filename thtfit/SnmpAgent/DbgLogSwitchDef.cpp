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

static int g_InitCallCnt = 0;

INT_t RegDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t * pbRet = NULL;

	do
	{
		if(0 < g_InitCallCnt)
		{
			g_InitCallCnt++;
			break;	
		}
		
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
		g_InitCallCnt++;
	}while(FALSE);

	return iOutRet;
}

INT_t UnregDbgLogSwitch()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		g_InitCallCnt--;
		if(0 < g_InitCallCnt)
		{
			break;
		}
		iRet = DbgLogSwitch_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		g_pLogDbusCmdSw = &g_Disabled;
		g_pLogUinputSw = &g_Disabled;
	}while(FALSE);

	return iOutRet;
}

