#include <stdio.h>
#include "string_ex.h"
#include <string.h>
#include <ctype.h>
#include <BaseErrDef.h>
#include <stdlib.h>
#include <BaseTypeDef.h>

void TruncTailingNewline(char * pszSrc)
{
	int iLen = 0;
	char * pszCmp = NULL;

	if(NULL == pszSrc) {return;}

	iLen = strlen(pszSrc);
	if(0 >= iLen) {return;}
	pszCmp = pszSrc + iLen - 1;
	while(pszSrc <= pszCmp)
	{
		if('\r' == *pszCmp || '\n' == *pszCmp)
		{
			pszCmp--;
		}
		else
		{
			break;
		}	
	}
	pszCmp++;
	*pszCmp = '\0';
}

LPSTR GetNextPosForSscanf(LPCSTR pszSrc)
{
	LPCSTR pszPos = pszSrc;

	while(*pszPos)
	{
		if(' ' != *pszPos && '\t' != *pszPos)
		{
			break;
		}
		pszPos++;
	}

	return (LPSTR)pszPos;
}

void CharToLower(char * pcSrc)
{
	if('A' <= *pcSrc && 'Z' >= *pcSrc)
	{
		(*pcSrc) += 32;
	}
}

void StrToLower(char * pszSrc)
{
	if(NULL ==  pszSrc)
	{
		goto EXIT_PROC;
	}
	
	while(*pszSrc)
	{
		CharToLower(pszSrc);
		pszSrc++;
	}

EXIT_PROC:
	;
}

void CharToUpper(char * pcSrc)
{
	if('a' <= *pcSrc && 'z' >= *pcSrc)
	{
		(*pcSrc) -= 32;
	}
}

void StrToUpper(char * pszSrc)
{
	while(*pszSrc)
	{
		CharToUpper(pszSrc);
		pszSrc++;
	}
}

int StrIsDigit(const char * pszStr)
{
	int iIsDigit = 0, iLen = 0;
	const char * pScanPos = pszStr;

	if(NULL == pScanPos) {return iIsDigit;}

	do{	
		if('\0' == *pScanPos) {break;}

		iLen = strlen(pszStr);
		
		while(*pScanPos)
		{
			if(0 == isdigit(*pScanPos)) {break;}
			pScanPos	++;
		}

		if(pScanPos - pszStr >= iLen)
		{
			iIsDigit = 1;
		}
	}while(0);

	return iIsDigit;
}

BOOL_t IsDigitString(LPCSTR pszStr)
{
	BOOL_t bOutRet = FALSE;
	size_t sizeStr;
	LPCSTR pScanPos = pszStr;

	do{
		if(NULL == pszStr)
		{
			break;
		}

		if('\0' == *pszStr)
		{
			break;
		}

		sizeStr = strlen(pszStr);
	
		while(*pScanPos)
		{
			if(0 == isdigit(*pScanPos)) 	//not digit
			{
				break;
			}
			pScanPos	++;
		}

		if(pScanPos - pszStr >= sizeStr)
		{
			bOutRet = TRUE;
		}
	}while(FALSE);

	return bOutRet;
}

int strcmp_safe(const char *s1, const char *s2)
{
	int iOutRet = 0;

	do{
		if(NULL == s1 && NULL == s2)
		{
			iOutRet = 0;	//is euqal
			break;
		}

		if(NULL == s1)
		{
			if('\0' == s2[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
			else 
			{
				iOutRet = *s2;
				break;
			}
		}

		if(NULL == s2)
		{
			if('\0' == s1[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
			else 
			{
				iOutRet = *s1;
				break;
			}
		}

		iOutRet = strcmp(s1, s2);
	}while(0);

	return iOutRet;
}

int strcasecmp_safe(const char *s1, const char *s2)
{
	int iOutRet = 0;

	do{
		if(NULL == s1 && NULL == s2)
		{
			iOutRet = 0;	//is euqal
			break;
		}

		if(NULL == s1)
		{
			if('\0' == s2[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
		}

		if(NULL == s2)
		{
			if('\0' == s1[0])
			{
				iOutRet = 0;	//is equal
				break;
			}
		}

#ifdef _WIN32
		iOutRet = stricmp(s1, s2);
#else
		iOutRet = strcasecmp(s1, s2);
#endif
	}while(0);

	return iOutRet;
}

int safe_strncmp(const char *s1, const char *s2, size_t n)
{
	int iOutRet = 0;

	do{
		if(NULL == s1 && NULL == s2)
		{
			iOutRet = 0;	//is euqal
			break;
		}

		if(NULL == s1)
		{
			if('\0' == s2[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
		}

		if(NULL == s2)
		{
			if('\0' == s1[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
		}

		iOutRet = strncmp(s1, s2, n);
	}while(FALSE);

	return iOutRet;
}

int safe_strncasecmp(const char *s1, const char *s2, size_t n)
{
	int iOutRet = 0;

	do{
		if(NULL == s1 && NULL == s2)
		{
			iOutRet = 0;	//is euqal
			break;
		}

		if(NULL == s1)
		{
			if('\0' == s2[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
		}

		if(NULL == s2)
		{
			if('\0' == s1[0])
			{
				iOutRet = 0;	//is euqal
				break;
			}
		}

		iOutRet = strncasecmp(s1, s2, n);
	}while(FALSE);

	return iOutRet;
}

size_t safe_strlen(LPCSTR pszSrc)
{
	if(NULL == pszSrc)
	{
		return 0;
	}

	return strlen(pszSrc);
}

const char * StrChr(const char * pszSearchIn, const char cToBeSearched, int nSrchCnt)
{
	const char * pszOutRet = NULL;
	const char * pszSrchPos = NULL;

	do{
		if(NULL == pszSearchIn)
		{
			pszOutRet = NULL;
			break;
		}

		pszSrchPos = pszSearchIn;
		while(*pszSrchPos)
		{
			if(nSrchCnt <= pszSrchPos - pszSearchIn)
			{
				break;
			}
			if(cToBeSearched == *pszSrchPos)
			{
				pszOutRet = pszSrchPos;
				break;
			}
			pszSrchPos++;
		}
	}while(0);

	return pszOutRet;
}

char * safe_strchr(const char *s, int c)
{
	char * pszOutRet = NULL;

	if(NULL == s || 0 == c)
	{
		goto EXIT_PROC;
	}

	pszOutRet = strchr(s, c);
	
EXIT_PROC:
	return pszOutRet;
}

void TrimRight(char * pszSrc)
{
	if(pszSrc)
	{
		int iSrcLen = strlen(pszSrc);
		int iId = iSrcLen - 1;
		while(0 <= iId)
		{
			if(0 == iId && ' ' == pszSrc[iId])
			{
				pszSrc[iId] = '\0';
			}
			else if(' ' != pszSrc[iId])
			{
				if(iSrcLen - 1 > iId)
				{
					pszSrc[iId+1] = '\0';
				}
				else if(iSrcLen - 1 == iId)
				{
					
				}
				break;
			}
			iId--;
		}
	}
}

int TrimLeft(char * pszSrc)
{
	int iOutRet = 0;
	char * pszNew = NULL;
	int iSrcLen = 0;
	const char * pszScanPos = NULL;
		
	do{
		if(NULL == pszSrc)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iSrcLen = strlen(pszSrc);
		if(0 == iSrcLen)
		{
			break;
		}

		pszNew = malloc(iSrcLen+1);
		if(NULL == pszNew)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(' ' != pszSrc[0])
		{
			break;
		}

		strcpy(pszNew, pszSrc);
		pszScanPos = pszNew;
		while(' ' == *pszScanPos)
		{
			pszScanPos++;
		}		

		strcpy(pszSrc, pszScanPos);
	}while(0);

	if(pszNew)
	{
		free(pszNew);
		pszNew = NULL;
	}

	return iOutRet;
}

PBYTE memchrWithCnt(const PBYTE pSrc, BYTE ucCharToSearch, UINT uiMaxBytesCnt, P_INT piStatCnt)
{
	PBYTE pOutRet = NULL;
	INT iId = 0;

	do 
	{
		if(NULL == pSrc || 0 == uiMaxBytesCnt)
		{
			pOutRet = NULL;
			break;
		}

		for(iId=0; iId<uiMaxBytesCnt; iId++)
		{
			if(pSrc[iId] == ucCharToSearch)
			{
				if(NULL == pOutRet)
				{
					pOutRet = pSrc+iId;
				}
				if(piStatCnt)
				{
					(*piStatCnt)++;
				}
			}
		}
	} while (FALSE);

	return pOutRet;
}

PBYTE MemSearchMem(const PBYTE pSrcBuf, const INT iSrcBufLen, const PBYTE pToSearchBuf, const INT iToSearchBufLen)
{
	PBYTE pFinded = NULL;
	INT iSrcBufOff = 0;

	do 
	{
		if(NULL == pSrcBuf || NULL == pToSearchBuf || 0 >= iSrcBufLen || 0 >= iToSearchBufLen || 
			iSrcBufLen < iToSearchBufLen)
		{
			pFinded = NULL;
			break;
		}

CONTINUE_SEARCH:
		if(0 == memcmp(pSrcBuf+iSrcBufOff, pToSearchBuf, iToSearchBufLen))
		{
			pFinded = pSrcBuf + iSrcBufOff;
			break;
		}

		iSrcBufOff++;

		if(iSrcBufLen - iSrcBufOff < iToSearchBufLen)
		{
			break;
		}

		goto CONTINUE_SEARCH;
	} while (FALSE);

	return pFinded;
}
