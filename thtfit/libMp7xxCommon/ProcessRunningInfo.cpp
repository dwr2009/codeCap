#include "ProcessRunningInfo.h"
#include <ProcessList.h>
#include <string_ex.h>
#include <StackBufString.h>

BOOL_t CProcessRunningInfo::NxserverIsRunning()
{
	BOOL_t bIsRunning = FALSE;
	INT_t iRet;
	CProcessList oProcList;

	iRet = oProcList.ScanSysProcess();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}

	{
		const PROCESS_ITEM_INFO_LIST & oProcItemInfoList = oProcList.getProcessList();
		P_PROCESS_ITEM_INFO pProcItemInfo = NULL;
		POSITION pPos = NULL;
		pPos = oProcItemInfoList.GetHeadPosition();
		while(pPos)
		{
			pProcItemInfo = oProcItemInfoList.GetNext(IN OUT pPos);
			if(pProcItemInfo)
			{
				//LOG_BLINE("split(%s)\n", (LPCSTR)pProcItemInfo->strCmdLine);
				CString::STD_STRING_LIST sCmdLineParaList = pProcItemInfo->strCmdLine.Split(' ');
				if(0 < sCmdLineParaList.size())
				{	
					DECLARE_CLS_STACK_BUF_STRING(strExeFile, 1024);
					const char * pszBasename = NULL;
					strExeFile = sCmdLineParaList.front();
					pszBasename = basename((LPCSTR)strExeFile);
					//LOG_BLINE("basename=%s\n", pszBasename);
					if(0 == strcmp_safe("nxserver", pszBasename) ||
						strstr((LPCSTR)pProcItemInfo->strCmdLine, "./start nxserver"))
					{
						bIsRunning = TRUE;
						goto EXIT_PROC;
					}
				}
			}
		}
	}

EXIT_PROC:
	
	oProcList.ClearProcessList();

	return bIsRunning;
}

BOOL_t NxserverIsRunning(void)
{
	return CProcessRunningInfo::NxserverIsRunning();
}
