#include <ProcessList.h>
#include <dirent.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <string_ex.h>
#include <File.h>
#include <stdlib.h>
#include <StackBufString.h>
#include <ErrPrintHelper.h>
#include <dirent2.h>

CProcessList::CProcessList()
{
	
}

CProcessList::~CProcessList()
{
	ClearProcessList();
}

INT CProcessList::ScanSysProcess()
{
	INT iOutRet = ERROR_SUCCESS;
	int iRet;
	DIR *pDir = NULL;
	struct dirent * pDirEntry = NULL;
	P_PROCESS_ITEM_INFO pProcessItemInfo = NULL;
	POSITION pPos = NULL;
	UINT uiProcessId = 0;	
	CFile oProcessInfoFile;
	DECLARE_CLS_STACK_BUF_STRING(strFilename, 256);
	DECLARE_CLS_STACK_BUF_STRING(strCmdLine, 1024);
	INT iBytesRead = 0, iId = 0;

	do
	{
		ClearProcessList();

		pDir = opendir("/proc");
		if(NULL == pDir)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		while(TRUE)
		{
			pDirEntry = readdir(pDir);
			if(NULL == pDirEntry)
			{
				break;
			}
			
			//check
			if(FALSE == IsDigitString(pDirEntry->d_name))
			{
				continue;
			}

			uiProcessId = atoi(pDirEntry->d_name);
			iRet = strFilename.Format("/proc/%d/cmdline", uiProcessId);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
			iRet = oProcessInfoFile.Open(strFilename, "rt");
			if(ERROR_SUCCESS != iRet)
			{
				if(ENOENT == oProcessInfoFile.GetLastCrtErrNo())
				{
				}
				else
				{
					PRINT_FILE_LINENO_IRET_BUG_STR;
				}
				continue;
			}
			
			iBytesRead = 0;
			iId = 0;
			iRet = oProcessInfoFile.Read((LPBYTE)(strCmdLine.get_BufPtr()), strCmdLine.get_BufSize() - 1, &iBytesRead);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
				continue;
			}
			if(0 >= iBytesRead)
			{
				continue;
			}
			strCmdLine.get_BufPtr()[iBytesRead] = '\0';
			strCmdLine.get_BufPtr()[strCmdLine.get_BufSize()-1] = '\0';
			while(TRUE)
			{
				if(0 >  iBytesRead - 1 - iId)
				{
					PRINT_FILE_LINENO_BUG_STR;
				}
				char * pNullPos = (char *)(memchr(strCmdLine.get_BufPtr() + iId, '\0', iBytesRead - 1 - iId));
				if(NULL == pNullPos)
				{
					break;
				}
				*pNullPos = ' ';
				iId = (pNullPos - strCmdLine.get_BufPtr()) + 1;
			}
			iRet = oProcessInfoFile.Close();
			PrintIRetBugIfFail(iRet);

			/*
			PRINT_FILE_LINENO;
			printf("Pid:%d,strCmdLine=%s\n", uiProcessId, (LPCSTR)strCmdLine);
			*/
			
			pProcessItemInfo = new PROCESS_ITEM_INFO;
			if(NULL == pProcessItemInfo)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}			
			pProcessItemInfo->uiProcessId = uiProcessId;
			pProcessItemInfo->strCmdLine = (LPCSTR)strCmdLine;
			pPos = m_lstProcessItemInfo.AddTail(pProcessItemInfo);
			if(NULL == pPos)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			pProcessItemInfo = NULL;	//detached
		}
	}while(FALSE);

	if(pProcessItemInfo)
	{
		delete pProcessItemInfo;
		pProcessItemInfo = NULL;
	}

	if(pDir)
	{
		iRet = closedir(pDir);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		pDir = NULL;
	}

	return iOutRet;
}

const PROCESS_ITEM_INFO_LIST & CProcessList::getProcessList()
{
	return m_lstProcessItemInfo;
}

BOOL_t CProcessList::existProcess(pid_t processId)
{
	BOOL_t bOutRet = FALSE;
	INT_t iRet;
	DECLARE_CLS_STACK_BUF_STRING(strProcessProcDir, 2048);
	iRet = strProcessProcDir.Format("/proc/%d", processId);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}
	iRet = ExistDir(strProcessProcDir, &bOutRet);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		bOutRet = FALSE;
		goto EXIT_PROC;
	}
EXIT_PROC:
	return bOutRet;
}

VOID CProcessList::ClearProcessList()
{
	POSITION pPos = NULL;
	P_PROCESS_ITEM_INFO pProcessItemInfo = NULL;

	pPos = m_lstProcessItemInfo.GetHeadPosition();
	while(pPos)
	{
		pProcessItemInfo = m_lstProcessItemInfo.GetNext(pPos);
		if(pProcessItemInfo)
		{
			delete pProcessItemInfo;
			pProcessItemInfo = NULL;
		}
	}
	m_lstProcessItemInfo.RemoveAll();
}

