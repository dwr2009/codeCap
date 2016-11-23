#include "DbgLogSwitchDef.h"
#include <DbgLogSwitch.h>
#include <BaseErrDef.h>
#include <stdio.h>
#include <ErrPrintHelper.h>

#define	DbgLogSwDesc_LogDbugCmd				"LogDbusCmd"
#define	DbgLogSwDesc_LogMediaFileInfo		"LogMediaFileInfo"
#define	DbgLogSwDesc_LogPlaybackStatus		"LogPlaybackStatus"
#define	DbgLogSwDesc_LogVideoOutputInfo		"LogVideoOutputInfo"
#define	DbgLogSwDesc_LogVideoSync			"LogVideoSync"
#define	DbgLogSwDesc_LogTuner				"LogTuner"

static BOOL_t g_Disabled = FALSE;

BOOL_t * g_pLogDbusCmdSw = &g_Disabled;
BOOL_t * g_pLogMediaFileInfoSw = &g_Disabled;
BOOL_t * g_pLogPlaybackStatus = &g_Disabled;
BOOL_t * g_pLogVideoOutputInfo = &g_Disabled;
BOOL_t * g_pLogVideoSync = &g_Disabled;
BOOL_t * g_pLogTuner = &g_Disabled;

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
		//LogDbusCmd
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogDbugCmd);
		if(pbRet)
		{
			g_pLogDbusCmdSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogDbugCmd);
		}
		//LogMediaFileInfo
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogMediaFileInfo);
		if(pbRet)
		{
			g_pLogMediaFileInfoSw = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogMediaFileInfo);
		}
		//DbgLogSwDesc_LogPlaybackStatus
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogPlaybackStatus);
		if(pbRet)
		{
			g_pLogPlaybackStatus = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogPlaybackStatus);
		}
		//DbgLogSwDesc_LogVideoOutputInfo
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogVideoOutputInfo);
		if(pbRet)
		{
			g_pLogVideoOutputInfo = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogVideoOutputInfo);
		}
		//DbgLogSwDesc_LogVideoSync
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogVideoSync);
		if(pbRet)
		{
			g_pLogVideoSync = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogVideoSync);
		}
		//DbgLogSwDesc_LogTuner
		pbRet = DbgLogSwitch_RegSwitch(DbgLogSwDesc_LogTuner);
		if(pbRet)
		{
			g_pLogTuner = pbRet;
		}
		else
		{
			LOG_BLINE("RegSw(%s) failed\n", DbgLogSwDesc_LogTuner);
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

