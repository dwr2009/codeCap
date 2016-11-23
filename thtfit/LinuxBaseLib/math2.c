#include "math2.h"
#include <BaseErrDef.h>
#include <stdio.h>
#include <stdlib.h>
#include "string_ex.h"
#include <strings.h>
#include <string.h>

int GetMinVal(int a, int b)
{
	return (a<b?a:b);
}

UINT getMinVal_UINT(UINT a, UINT b)
{
	return (a<b?a:b);
}

int GetMaxVal(int a, int b)
{
	return (a>b?a:b);
}

INT ConvertValFromStr(LPCSTR pszStr, UINT * puiVal/*IN OUT*/)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iLen = 0;
	UINT uiVal = 0U;
	char * pszRet = NULL;
	const char * pszFromStr = NULL;

	do{
		if(NULL == pszStr)
		{
			iOutRet = ERROR_SUCCESS;
			break;
		}

		iLen = strlen(pszStr);

		if(' ' == *pszStr)
		{
			pszRet = malloc(iLen+1);
			if(NULL == pszRet)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			iRet = TrimLeft(pszRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}			
			pszFromStr = pszRet;
		}
		else
		{
			pszFromStr = pszStr;
		}

		if(0 == strncasecmp("0x", pszFromStr, 2))
		{
			iRet = sscanf(pszFromStr, "%x", &uiVal);
		}
		else
		{
			iRet = sscanf(pszFromStr, "%u", &uiVal);
		}
		if(1 != iRet)
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}

		if(puiVal)
		{
			*puiVal = uiVal;
		}
	}while(FALSE);

	if(pszRet)
	{
		free(pszRet);
		pszRet = NULL;
	}

	return iOutRet;
}

BOOL_t IsInRange(CONST UINT32_t uiTest, CONST UINT32_t uiMinVal, CONST UINT32_t uiMaxVal)
{
	return (uiMinVal <= uiTest && uiMaxVal >= uiTest);
}

BOOL_t IsNotInRange(CONST UINT32_t uiTest, CONST UINT32_t uiMinVal, CONST UINT32_t uiMaxVal)
{
	return (!IsInRange(uiTest, uiMinVal, uiMaxVal));
}

