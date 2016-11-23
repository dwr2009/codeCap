#include <stdio.h>
#include <BaseTypeDef.h>
#include <DbgLogSwitch.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
	int iProcessExitCode = 0, iRet;
	BOOL_t bDbgLogSwitchLib_Inited = FALSE;

	do
	{
		iRet = DbgLogSwitch_Init();
		if(ERROR_SUCCESS == iRet)
		{
			bDbgLogSwitchLib_Inited = TRUE;
		}
		else
		{
			LOG_BLINE("DbgLogSwitch_Init err %d\n", iRet);
			break;
		}
		if(3 == argc)
		{
			int iSwitchId = atoi(argv[1]);
			BOOL_t bEnable = atoi(argv[2]);
			iRet = DbgLogSwitch_setSwitch(iSwitchId, bEnable);
			if(ERROR_SUCCESS != iRet)
			{
				LOG("setSwitch(%d, %d) err %d\n", iSwitchId, bEnable, iRet);
			}
		}
		else if(1 == argc)
		{
		}
		else
		{
			LOG("Usage: %s <DbgSwitchId> <Enable>\n", argv[0]);
			break;
		}
		{
			int iDbgSwCnt, iIndex;
			BOOL_t bEnable;
			LPCSTR pszDesc = NULL;
			iDbgSwCnt = DbgLogSwitch_getSwitchCount();
			LOG("Id	Enable	Desc\n");
			for(iIndex = 0; iIndex < iDbgSwCnt; iIndex++)
			{
				iRet = DbgLogSwitch_getSwitchInfo(iIndex, &bEnable, &pszDesc);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("DbgLogSwitch_getSwitchInfo err %d\n", iRet);
					break;
				}
				LOG("%d	%d	%s\n", iIndex, bEnable, pszDesc);
			}
		}
	}while(FALSE);

	if(bDbgLogSwitchLib_Inited)
	{
		bDbgLogSwitchLib_Inited = FALSE;
		iRet = DbgLogSwitch_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

#if	0	//only for test
	{
		int fdLargeFile = INVALID_FILE_DESCRIPTOR;
		do
		{
			fdLargeFile = open("/MP7XX/LocalMedia/Video/Skyfall.mp4", O_RDONLY);
			if(0 > fdLargeFile)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
			__off64_t offCurFd = 0;
			/*
			offCurFd = lseek64(fdLargeFile, 3*1024*1024*1024UL, SEEK_SET);
			if(0 > offCurFd)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
			LOG_BLINE("Seek(3G),offCur=%lld\n", offCurFd);
			*/
			offCurFd = lseek64(fdLargeFile, 0, SEEK_END);
			if(0 > offCurFd)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}			
			LOG_BLINE("Seek(End),offCur=%lld\n", offCurFd);
		}while(FALSE);
		if(INVALID_FILE_DESCRIPTOR != fdLargeFile)
		{
			iRet = close(fdLargeFile);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			fdLargeFile = INVALID_FILE_DESCRIPTOR;
		}
	}
#endif

	return iProcessExitCode;
}

