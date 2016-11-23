#include "FileCreator.h"
#include <BaseErrDef.h>
#include <stdio.h>
#include <ErrPrintHelper.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

INT BufferDataToFile(LPCSTR pszFilePath, PBYTE pBufData, UINT uiDataLen)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	FILE * pFile = NULL;

	do{
		if(NULL == pszFilePath || NULL == pBufData)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pFile = fopen(pszFilePath, "wb");
		if(NULL == pFile)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		iRet = fwrite(pBufData, 1, uiDataLen, pFile);
		if(uiDataLen != iRet)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);

	if(pFile)
	{
		iRet = fclose(pFile);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		pFile = NULL;
	}

	return iOutRet;
}

INT_t FileDataToBuffer(LPCSTR pszFilePath, PBYTE pBuf, size_t bufSize, size_t * pBytesRead)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	int fdInFile = INVALID_FILE_DESCRIPTOR;
	unsigned long readOffset = 0;
	
	fdInFile = open(pszFilePath, O_RDONLY);
	if(INVALID_FILE_DESCRIPTOR == fdInFile)
	{
		iOutRet = ERR_FILE_OPEN_FAIL;
		goto EXIT_PROC;
	}

READ_NEXT_BLOCK:
	iRet = read(fdInFile, pBuf+readOffset, bufSize-readOffset);
	//LOG_BLINE("bufSize=%lu, readOffset=%lu, iRet=%d\n", bufSize, readOffset, iRet);
	if(0 > iRet)
	{
		iOutRet = ERR_FILE_READ_FAIL;
		goto EXIT_PROC;
	}
	else if(0 < iRet)
	{
		readOffset += iRet;
		if(readOffset < bufSize)
		{
			goto READ_NEXT_BLOCK;
		}
	}
	if(pBytesRead)
	{
		*pBytesRead = readOffset;
	}
	
EXIT_PROC:

	if(INVALID_FILE_DESCRIPTOR != fdInFile)
	{
		iRet = close(fdInFile);
		if(ERR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		fdInFile = INVALID_FILE_DESCRIPTOR;
	}
	
	return iOutRet;
}

