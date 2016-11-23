#include "Checksum.h"
#include <stdlib.h>
#ifdef	_WIN32
#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#endif	//_WIN32
#include "ErrPrintHelper.h"
#include <BaseErrDef.h>

UINT32 CheckSum32(LPBYTE pBuf, INT iBufSize)
{
	INT iBlockSize = iBufSize / 4;
	INT iBlockResidue = iBufSize % 4;
	INT iBlkId = 0;
	UINT32 uiChecksumVal = 0U;

	for(iBlkId=0; iBlkId<iBlockSize; iBlkId++)
	{
		uiChecksumVal += ((UINT32 *)pBuf)[iBlkId];
	}

	if(iBlockResidue)
	{
		UINT32 uiResidueVal = 0;
		memcpy(&uiResidueVal, &(((UINT32 *)pBuf)[iBlockSize]), iBlockResidue);
		uiChecksumVal += uiResidueVal;
	}

	return uiChecksumVal;
}

INT CalcFileChecksum32(const char * pszFilePath, UINT32 * puiChecksumVal)
{
#ifdef	_WIN32
#define	READ_BUF_SIZE		16*1024*1024
#else
#define	READ_BUF_SIZE		4*1024
#endif

	INT iOutRet = 0, iRet = 0;
	LPBYTE pReadBuf = NULL;
	FILE * fpIn = NULL;
	INT iBytesRead = 0;
	UINT32 uiChecksumVal = 0U;

	do{
		if(NULL == pszFilePath || NULL == puiChecksumVal)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pReadBuf = (LPBYTE)malloc(READ_BUF_SIZE);
		if(NULL == pReadBuf)
		{
#ifdef	_WIN32
			iOutRet = ERROR_OUTOFMEMORY;
#else	//ndef _WIN32
			iOutRet = ERROR_OUT_OF_MEMORY;
#endif	//_WIN32
			break;
		}

		fpIn = fopen(pszFilePath, "rb");
		if(NULL == fpIn)
		{
			iOutRet = ERR_FILE_OPEN_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iBytesRead = 0;
		uiChecksumVal = 0U;

		while(TRUE)
		{
			iBytesRead = fread(pReadBuf, 1, READ_BUF_SIZE, fpIn);
			if(0 == iBytesRead)
			{
				break;
			}
			else if(0 > iBytesRead)
			{
				iOutRet = ERR_FILE_READ_FAIL;
				break;
			}
						
			uiChecksumVal += CheckSum32(pReadBuf, iBytesRead);
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		if(puiChecksumVal)
		{
			*puiChecksumVal = uiChecksumVal;
		}
	}while(FALSE);

	if(fpIn)
	{
		fclose(fpIn);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		fpIn = NULL;
	}

	if(pReadBuf)
	{
		free(pReadBuf);
		pReadBuf = NULL;
	}

	return iOutRet;
}

UINT8_t CalcCrc8(CONST UINT8_t uiInitialCrc8Val, PBYTE pData, CONST size_t sizeData)
{
	UINT8_t uiCrc8Val = uiInitialCrc8Val;
	INT8_t iId = 0;
	size_t sizeDataLen = sizeData;

	while(sizeDataLen--)
	{
		uiCrc8Val ^= *pData++;
		for(iId=0; iId<8; iId++)
		{
			if(uiCrc8Val & 0x01)
			{
				uiCrc8Val = (uiCrc8Val >> 1) ^ 0x8C;
			}
			else
			{
				uiCrc8Val >>= 1;
			}
		}
	}

	return uiCrc8Val;
}

