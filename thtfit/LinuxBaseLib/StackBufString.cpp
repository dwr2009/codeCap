#include "StackBufString.h"
#include <BaseErrDef.h>
#include <string_ex.h>
#include <string.h>
#include <math2.h>
#include <stdio.h>
#include <ErrPrintHelper.h>
#include <stdarg.h>
#include <stdlib.h>
#include <String.h>

CStackBufString::CStackBufString(char * pBuf, INT iBufSize)
{
	m_iLastErrNo = ERROR_SUCCESS;
	m_pszBuf = pBuf;
	m_iBufSize = iBufSize;
	m_bStrLenMaybeChanged = TRUE;
	m_iCachedStrLen = 0;

	if(0 >= iBufSize || NULL == m_pszBuf)
	{
		m_iBufSize = 0;
		m_iLastErrNo = ERROR_INVALID_PARAMETER;
	}
	else
	{
		m_pszBuf[0] = '\0';
	}
}

CStackBufString::~CStackBufString()
{

}

CStackBufString & CStackBufString::operator =(LPCSTR pSrc)
{
	INT iStrLen;
	
	m_iLastErrNo = ERROR_SUCCESS;
	iStrLen = safe_strlen(pSrc);

	do{
		if(0 == iStrLen)
		{
			if(m_pszBuf && 0 < m_iBufSize)
			{
				m_pszBuf[0] = '\0';
				m_iCachedStrLen = 0;
				m_bStrLenMaybeChanged = FALSE;				
			}
			break;
		}

		if(m_iBufSize - 1 < iStrLen)
		{
			if(m_pszBuf && 0 < m_iBufSize)
			{
				memcpy(m_pszBuf, pSrc, m_iBufSize);
				m_iCachedStrLen = m_iBufSize - 1;
				m_bStrLenMaybeChanged = FALSE;				
			}
			m_iLastErrNo = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}

		memcpy(m_pszBuf, pSrc, iStrLen+1);
		m_iCachedStrLen = iStrLen;
		m_bStrLenMaybeChanged = FALSE;				
	}while(FALSE);	
	
	return *this;
}

CStackBufString & CStackBufString::operator= (CString & strSrc)
{
	return (operator= ((LPCSTR)(strSrc)));
}

const CStackBufString & CStackBufString::operator +=(LPCSTR pszAdded)
{
	m_iLastErrNo = ERROR_SUCCESS;
	UINT uiStringLength = getStringLength();
	
	do{
		if(NULL == pszAdded)
		{
			m_iLastErrNo = ERROR_INVALID_PARAMETER;
			break;
		}
		if(NULL == m_pszBuf || 0 > m_iBufSize)
		{
			m_iLastErrNo = ERROR_INVALID_STATE;
			break;
		}
		if((INT)(uiStringLength + strlen(pszAdded)) > m_iBufSize - 1)
		{
			m_iLastErrNo = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}
		strcpy(m_pszBuf + uiStringLength, pszAdded);
		m_bStrLenMaybeChanged = TRUE;
	}while(FALSE);

	return *this;
}

INT CStackBufString::setContent(const char cChar, const int iCharCnt)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(m_iBufSize - 1 < iCharCnt)
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}

		if(NULL == m_pszBuf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		memset(m_pszBuf, cChar, iCharCnt);
		m_pszBuf[GetMinVal(m_iBufSize - 1, iCharCnt)] = '\0';
		m_iCachedStrLen = GetMinVal(m_iBufSize - 1, iCharCnt);
		m_bStrLenMaybeChanged = FALSE;						
	}while(FALSE);

	return iOutRet;
}

INT_t CStackBufString::setContent(LPCSTR pszSrc, size_t SizeToFetch)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_iBufSize - 1 < (typeof(m_iBufSize))SizeToFetch)
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}

		if(NULL == m_pszBuf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
	
		memcpy(m_pszBuf, pszSrc, SizeToFetch);
		m_pszBuf[GetMinVal(m_iBufSize - 1, SizeToFetch)] = '\0';
		m_bStrLenMaybeChanged = TRUE;
		m_iCachedStrLen = 0;
	}while(FALSE);
	
	return iOutRet;
}

INT CStackBufString::Format(LPCSTR pszFmt, ...)
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == pszFmt)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(NULL == m_pszBuf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		va_list VarList;
		
		va_start(VarList, pszFmt);
		iRet = vsnprintf(m_pszBuf, m_iBufSize, pszFmt, VarList);				
		va_end(VarList);

		if(iRet >= m_iBufSize)
		{
			m_pszBuf[m_iBufSize-1] = '\0';
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			m_iCachedStrLen = m_iBufSize - 1;
			m_bStrLenMaybeChanged = FALSE;						
			break;
		}
		else if(0 > iRet)
		{
			m_pszBuf[m_iBufSize-1] = '\0';
			iOutRet = ERROR_FAILED;
			break;
		}

		m_pszBuf[iRet] = '\0';
		m_iCachedStrLen = iRet;
		m_bStrLenMaybeChanged = FALSE;						
	}while(FALSE);

	return iOutRet;
}

CStackBufString::operator LPCSTR() const
{
	return m_pszBuf;
}

CStackBufString::operator INT_t() const
{
	if(m_pszBuf)
	{
		return atoi(m_pszBuf);
	}
	else
	{
		return 0;
	}
}

BOOL_t CStackBufString::Equal(LPCSTR pszStr1, size_t SizeToCmp)
{
	BOOL bOutRet = FALSE;

	do{
		if(NULL == m_pszBuf && NULL == pszStr1)
		{
			bOutRet = TRUE;
			break;
		}

		if(NULL == m_pszBuf)
		{
			if('\0' == pszStr1[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(NULL == pszStr1)
		{
			if('\0' == m_pszBuf[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(0 == strncmp(m_pszBuf, pszStr1, SizeToCmp))
		{
			bOutRet = TRUE;
			break;
		}
		else
		{
			bOutRet = FALSE;
			break;
		}
	}while(FALSE);

	return bOutRet;
}
BOOL_t CStackBufString::Equal(LPCSTR pszStr1)
{
	BOOL bOutRet = FALSE;

	do{
		if(NULL == m_pszBuf && NULL == pszStr1)
		{
			bOutRet = TRUE;
			break;
		}

		if(NULL == m_pszBuf)
		{
			if('\0' == pszStr1[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(NULL == pszStr1)
		{
			if('\0' == m_pszBuf[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(0 == strcmp(m_pszBuf, pszStr1))
		{
			bOutRet = TRUE;
			break;
		}
		else
		{
			bOutRet = FALSE;
			break;
		}
	}while(FALSE);

	return bOutRet;
}

BOOL_t CStackBufString::NotEqual(LPCSTR pszStr1)
{
	return !Equal(pszStr1);
}

LPSTR CStackBufString::get_BufPtr()
{
	m_bStrLenMaybeChanged = TRUE;						

	return m_pszBuf;
}

LPCSTR CStackBufString::getConstData() CONST
{
	return m_pszBuf;
}

INT CStackBufString::get_BufSize()
{
	return m_iBufSize;
}

INT CStackBufString::get_LastErrNo()
{
	return m_iLastErrNo;
}

UINT CStackBufString::getStringLength()
{
	if(m_bStrLenMaybeChanged)
	{
		if(m_pszBuf)
		{
			m_iCachedStrLen = strlen(m_pszBuf);
		}
		else
		{
			m_iCachedStrLen = 0;
		}
		m_bStrLenMaybeChanged = FALSE;
	}

	return m_iCachedStrLen;
}

void CStackBufString::SafeEndNullChar(INT iDataLen/* = -1*/)
{
	if(m_pszBuf)
	{
		if(-1 == iDataLen)
		{
			m_pszBuf[m_iBufSize-1] = '\0';
		}
		else if(0 <= iDataLen && m_iBufSize > iDataLen)
		{
			m_pszBuf[iDataLen] = '\0';
		}
	}
	else
	{
		PRINT_FILE_LINENO_BUG_STR;
	}
}

VOID CStackBufString::ScanfParam_Init(P_SCANF_PARAM pScanfParam)
{
	pScanfParam->iFieldsScanned = 0;
}

INT CStackBufString::Scanf(OUT SCANF_PARAM & oScanfParam, LPCSTR pszFmt, ...)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == m_pszBuf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		va_list VarList;
		
		va_start(VarList, pszFmt);
		oScanfParam.iFieldsScanned = vsscanf(m_pszBuf, pszFmt, VarList);				
		va_end(VarList);

		if(EOF == oScanfParam.iFieldsScanned)
		{
			iOutRet = ERR_SCANF_FAILURE;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

CString::STD_STRING_LIST CStackBufString::Split(char DeliminatedChar, CStackBufString::SplitOptions eSplitOptions/* = KeepEmptyParts*/)
{
	CString::STD_STRING_LIST StrList;
	char * pScanStartPos = NULL;
	char * pDeliminatedCharPos = NULL;

	do
	{
		//NULL
		if(NULL == m_pszBuf)
		{
			break;
		}
		//Empty
		if('\0' == m_pszBuf[0])
		{
			break;
		}
		//Available string
		pScanStartPos = m_pszBuf;
		while(TRUE)
		{
			pDeliminatedCharPos = strchr(pScanStartPos, DeliminatedChar);
			if(pDeliminatedCharPos)
			{
				if(0 < (pDeliminatedCharPos - pScanStartPos) || (CStackBufString::KeepEmptyParts == eSplitOptions))
				{
					try
					{
						StrList.push_back(CString(pScanStartPos, pDeliminatedCharPos - pScanStartPos));
					}
					catch(std::bad_alloc BadAllocEx)
					{
						break;
					}
				}
				pScanStartPos = pDeliminatedCharPos + 1;
			}
			else	//last one
			{
				if('\0' != pScanStartPos[0])
				{
					try
					{
						StrList.push_back(pScanStartPos);
					}
					catch(std::bad_alloc BadAllocEx)
					{				
					}
				}
				break;
			}
		}
	}while(FALSE);

	return StrList;
}

BOOL_t CStackBufString::toBool()
{
	BOOL_t bOutRet = FALSE;
	
	do
	{
		if(NULL == m_pszBuf)
		{
			break;
		}
		if('\0' == m_pszBuf[0])
		{
			break;
		}
		bOutRet = (atoi(m_pszBuf)?TRUE:FALSE);
	}while(FALSE);

	return bOutRet;
}

