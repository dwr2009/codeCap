#include "Process.h"
#include <BaseErrDef.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ErrPrintHelper.h>
#include <string_ex.h>
#include <BaseTypeDef.h>

INT CreateProcess(pid_t * pProcessId, LPCSTR pszCmd, P_PROCESS_ENVI_INFO pEnvInfo) 
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iLen, iCount, iId, iScanStatus;
	char *pszBuf = NULL, **ppArgs = NULL;
	const char *pStrStartPos = NULL;
	const char *pScanStartPos = NULL;
	const char * pScanEndPos   = NULL;
	BOOL bSingleQuotes = FALSE;
	pid_t pidChild = 0;

	do{
		iLen = safe_strlen(pszCmd);
		
		if (0 >= iLen) {	//no optimization compile
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pszBuf = malloc(iLen+1);
		if(NULL == pszBuf) {
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		//
		// calculates the count of arguments
		//
		
		iCount = 1;
		iScanStatus = EXPECT_STR_START;
		
		for (pScanStartPos = pszCmd; *pScanStartPos; pScanStartPos++) {

			if (EXPECT_STR_START == iScanStatus) {
				
				if (' ' != *pScanStartPos) {
					iScanStatus = SINGLE_STR_START;
					if (*pScanStartPos == '\'') {
						bSingleQuotes = TRUE;
					}
				}

			} else if (SINGLE_STR_START == iScanStatus) {

				if (' ' == *pScanStartPos) {
					if (*(pScanStartPos - 1) == '\'') {
						bSingleQuotes = FALSE;
					}
					if (!bSingleQuotes) {
						iCount++;
						iScanStatus = EXPECT_STR_START;
					}
				}
			}
		}
		
		ppArgs = malloc((sizeof(char *)) * (iCount + 1));
		if (NULL == ppArgs) {
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		ppArgs[iCount] = NULL;

		iId = 0;
		bSingleQuotes = FALSE;
		iScanStatus = EXPECT_STR_START;

		for (pScanStartPos = pszCmd; *pScanStartPos; pScanStartPos++) {
		
			if (EXPECT_STR_START == iScanStatus) {

				if (' ' != *pScanStartPos) {
					iScanStatus = SINGLE_STR_START;
					pStrStartPos = pScanStartPos;
					if (*pScanStartPos == '\'') {
						bSingleQuotes = TRUE;
						pStrStartPos++;
					}
				}

			} else if (SINGLE_STR_START == iScanStatus) {

				if (' ' == *pScanStartPos) {
					
					pScanEndPos = pScanStartPos;
					
					if (*(pScanStartPos - 1) == '\'') {
						bSingleQuotes = FALSE;
						--pScanEndPos;
					}
					
					if (!bSingleQuotes) {
						ppArgs[iId] = malloc(pScanEndPos - pStrStartPos + 1);
						if(NULL == ppArgs[iId]) {
							iOutRet = ERROR_OUT_OF_MEMORY;
							break;
						}
						memcpy(ppArgs[iId], pStrStartPos, pScanEndPos - pStrStartPos);
						(ppArgs[iId])[pScanEndPos - pStrStartPos] = '\0';
						iScanStatus = EXPECT_STR_START;
						iId++;
					}
				}
			}
		}	

		if ('\0' == *pScanStartPos && SINGLE_STR_START == iScanStatus) {

			pScanEndPos = pScanStartPos;
			
			if (*(pScanStartPos - 1) == '\'') {
				bSingleQuotes = FALSE;
				--pScanEndPos;
			}

			if (!bSingleQuotes) {
					
				ppArgs[iId] = malloc(pScanEndPos - pStrStartPos + 1);
				if (NULL == ppArgs[iId]) {
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				memcpy(ppArgs[iId], pStrStartPos, pScanEndPos - pStrStartPos);
				(ppArgs[iId])[pScanEndPos - pStrStartPos] = '\0';
				iScanStatus = EXPECT_STR_START;
				iId++;
				ppArgs[iId] = NULL;
			}
		}

		if(iId > iCount)
		{
			PRINT_FILE_LINENO_BUG_STR;
		}

		if(pszBuf) {free(pszBuf); pszBuf = NULL;}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}	
		
		pidChild = vfork();
		if(0 == pidChild)	//child
		{
			iRet = execv(ppArgs[0], ppArgs);
			if(pEnvInfo->bPrintErrInfo)
			{
				//printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			}
			iRet = execvp(ppArgs[0], ppArgs);
			if(pEnvInfo->bPrintErrInfo)
			{
				PRINT_FILE_LINENO;
				printf("%s, %s\n", ppArgs[0], strerror(errno));
			}
			iOutRet = ERROR_EXECL_CALL_FAIL;
			//printf("[%s:%d]iOutRet=%d\n", __FILE__, __LINE__, iOutRet);
			_exit(0);			
		}
		else if(0 < pidChild)	//parent
		{
			*pProcessId = pidChild;
			/*
			int iStatus = 0;
			iRet = waitpid(pidChild, &iStatus, WNOHANG);
			//printf("[%s:%d]waitpid return %d, iStatus=%d\n", __FILE__, __LINE__, iRet, iStatus);			
			if(-1 == iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			//printf("[%s:%d]WIFEXITED(iStatus)=%d, iOutRet=%d\n", __FILE__, __LINE__, 
			//	WIFEXITED(iStatus), iOutRet);
			*/
		}
		else
		{
			iOutRet = ERROR_VFORK_CALL_FAIL;
		}
	}while(FALSE);

	if(ppArgs)
	{		
		iId = 0;
		do{
			if(NULL == ppArgs[iId]	) {break;}
			//printf("[%s:%d]%d, %s\n", __FILE__, __LINE__, iId, ppArgs[iId]);
			//printf("%d\n", iId);
			free(ppArgs[iId]); 
			ppArgs[iId] = NULL;			
			iId = iId + 1;
		}while(TRUE);		
		
		free(ppArgs);
		ppArgs = NULL;
	}
	if(pszBuf) {free(pszBuf); pszBuf = NULL;}
	
	return iOutRet;
}

INT RunProcess(P_RUN_PROCESS_PARA pRunProcessPara)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	PROCESS_ENVI_INFO oProcessEnvInfo;
	pid_t pidChild = 0;
	int iStatus = 0;

	if(NULL == pRunProcessPara)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	pRunProcessPara->iProcessStatus = 0;

	oProcessEnvInfo.bPrintErrInfo = TRUE;
	oProcessEnvInfo.iLastCrtErrNo = ERROR_SUCCESS;
		
	iRet = CreateProcess(&pidChild, pRunProcessPara->pszCmd, &oProcessEnvInfo);
	if(0 != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

CONTINUE_WAIT_PROCESS:
	iRet = waitpid(pidChild, &iStatus, 0);
	if(-1 == iRet)
	{
		iOutRet = ERR_WAITPID_FAIL;
		goto EXIT_PROC;
	}
	else	//waitpid success
	{
		pRunProcessPara->iProcessStatus = iStatus;
		//Fix me, todo:
		//WIFCONTINUED(status) 
		//	(Since Linux 2.6.10) returns true if the child process was resumed by delivery of SIGCONT.		
		if(WIFSTOPPED(iStatus))	//not exit
		{
			goto CONTINUE_WAIT_PROCESS;
		}
	}

EXIT_PROC:
	
	return iOutRet;
}

INT RunProcess2(P_RUN_PROCESS_PARA_IN pRunProcParaIn, P_RUN_PROCESS_PARA_OUT pRunProcParaOut)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	PROCESS_ENVI_INFO oProcessEnvInfo;
	pid_t pidChild = 0;
	int iStatus = 0;

	if(NULL == pRunProcParaIn || NULL == pRunProcParaOut)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	pRunProcParaOut->iProcessStatus = 0;

	oProcessEnvInfo.bPrintErrInfo = TRUE;
	oProcessEnvInfo.iLastCrtErrNo = ERROR_SUCCESS;
		
	iRet = CreateProcess(&pidChild, pRunProcParaIn->pszCmd, &oProcessEnvInfo);
	if(0 != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

CONTINUE_WAIT_PROCESS:
	iRet = waitpid(pidChild, &iStatus, 0);
	if(-1 == iRet)
	{
		iOutRet = ERR_WAITPID_FAIL;
		goto EXIT_PROC;
	}
	else	//waitpid success
	{
		pRunProcParaOut->iProcessStatus = iStatus;
		//Fix me, todo:
		//WIFCONTINUED(status) 
		//	(Since Linux 2.6.10) returns true if the child process was resumed by delivery of SIGCONT.		
		if(WIFSTOPPED(iStatus)) //not exit
		{
			goto CONTINUE_WAIT_PROCESS;
		}
	}

EXIT_PROC:
	
	return iOutRet;
}

