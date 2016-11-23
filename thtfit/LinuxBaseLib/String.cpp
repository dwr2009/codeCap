#include	"String.h"
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <math2.h>
#include	<ErrPrintHelper.h>
#include <stdlib.h>
#include <StackBufString.h>

static char g_szNullStr[1] = {'\0'};

CString operator+(const CString & str1, LPCSTR pszStr2)
{
	CString strResult;
	size_t sizeStr2;

	if(pszStr2)
	{
		sizeStr2 = strlen(pszStr2);
	}
	else
	{
		sizeStr2 = 0;
	}

	if(0 < sizeStr2)
	{
		INT iRet;
		size_t sizeOldStr = str1.GetStrLength();
		size_t sizeNewStr = sizeOldStr + sizeStr2;
		
		if(0 < sizeOldStr)
		{
			iRet = strResult.SetStrLength(sizeNewStr);
			if(ERROR_SUCCESS == iRet)
			{
				memcpy(strResult.GetBufPtr(), str1, sizeOldStr);
				memcpy((char *)(strResult.GetBufPtr()) + sizeOldStr, pszStr2, sizeStr2);
				(strResult.GetBufPtr()[sizeNewStr]) = '\0';
			}
			else
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
		}
		else
		{
			strResult = pszStr2;
		}

		return strResult;
	}
	else 
	{
		return str1;
	}
}

CString::CString() 
{
	CommonConstruct();
}

CString::CString(const char * pszSrc)
{
	//LOG_BLINE("CString::CString(const char * pszSrc)\n");
	CommonConstruct();
	
	*this = pszSrc;
	m_bStrLenChanged = TRUE;
}

CString::CString(const char * & pszSrc)
{
	//LOG_BLINE("CString::CString(const char * & pszSrc)\n");
	CommonConstruct();
	
	*this = pszSrc;
	m_bStrLenChanged = TRUE;
}

CString::CString(const CString & oStringSrc)
{
	//LOG_BLINE("CString::CString(const CString & oStringSrc)\n");
	CommonConstruct();
	
	*this = oStringSrc;
	m_bStrLenChanged = TRUE;
}

CString::CString(LPCSTR pszSrc, size_t SrcSizeToFetch)
{
	CommonConstruct();

	m_iLastErrNo = setContent(pszSrc, SrcSizeToFetch);
}

CString::CString(INT_t iSrc)
{
	int iRet;
	char szConvBuf[32];
	CommonConstruct();
	szConvBuf[0] = '\0';
	iRet = snprintf(szConvBuf, sizeof(szConvBuf), "%d", iSrc);
	if(0 >= iRet)
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
	szConvBuf[sizeof(szConvBuf)-1] = '\0';
	operator=(szConvBuf);
}

VOID CString::CommonConstruct()
{
	m_pBuf = NULL;
	m_bStrLenChanged = TRUE;
	m_iBufLength = 0;
	m_iCachedStrLen = 0;
	m_iLastErrNo = ERROR_SUCCESS;
}

CString::~CString()
{
	if(m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
	m_iBufLength = 0;
	m_bStrLenChanged = TRUE;
}

void CString::FreeMem()
{
	if(m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
	m_iBufLength = 0;
	m_bStrLenChanged = TRUE;
}

const CString & CString::operator=(LPCSTR pSrc)
{
	int iLen = 0;
	char * pszNewBuf = NULL;
	m_iLastErrNo = ERROR_SUCCESS;

	//LOG_BLINE("const CString & CString::operator=(LPCSTR pSrc)\n");

	if(pSrc != m_pBuf)
	{
		do{
			if(NULL == pSrc) 
			{
				pszNewBuf = new char[1];
				if(NULL == pszNewBuf) 
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					m_iLastErrNo = ERROR_OUT_OF_MEMORY;
					break;
				}
				pszNewBuf[0] = '\0';
				m_iBufLength = 0;
			}
			else
			{
				iLen = strlen(pSrc);
				pszNewBuf = new char[iLen+1];
				if(NULL == pszNewBuf) 
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					m_iLastErrNo = ERROR_OUT_OF_MEMORY;
					break;
				}
				m_iBufLength = iLen;
				strcpy(pszNewBuf, pSrc);
				pszNewBuf[iLen] = '\0';
			}
		}while(FALSE);

		if(m_pBuf)
		{
			delete[] m_pBuf;
			m_pBuf = NULL;
		}

		m_pBuf = pszNewBuf;

		m_bStrLenChanged = TRUE;
	}
	else
	{
		PRINT_FILE_LINENO_BUG_STR;
	}

	return *this;
}

const CString & CString::operator=(char * pSrc)
{
	return (operator=)((LPCSTR)pSrc);
}

const CString & CString::operator=(const CString & oStringSrc)
{
	return (operator=)((LPCSTR)oStringSrc);
}

const CString & CString::operator = (const CStackBufString & strSrc)
{
	return (operator=)(strSrc.getConstData());
}

const CString & CString::operator=(const char cChar)
{
	if(m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
		m_iBufLength = 0;
	}

	do{
		m_pBuf = new char[2];
		if(NULL == m_pBuf)
		{
			m_iLastErrNo = ERROR_OUT_OF_MEMORY;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		m_pBuf[0] = cChar;
		m_pBuf[1] = '\0';
		m_iBufLength = 1;
	}while(FALSE);

	m_bStrLenChanged = TRUE;

	return *this;
}

const CString & CString::operator+=(const char cChar)
{
	if(m_pBuf)
	{
		do{
			int iLen = 0;
			char * pszNewBuf = NULL;
			iLen = strlen(m_pBuf);
			pszNewBuf = new char[iLen+1+1];
			if(NULL == pszNewBuf)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;
			}
			m_iBufLength = iLen+1;
			strcpy(pszNewBuf, m_pBuf);
			pszNewBuf[iLen] = cChar;
			pszNewBuf[iLen+1] = '\0';

			if(m_pBuf)
			{
				delete[] m_pBuf;
				m_pBuf = NULL;
			}

			m_pBuf = pszNewBuf;
		}while(false);
	}
	else
	{
		do{
			m_pBuf = new char[2];
			if(NULL == m_pBuf)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;
			}
			m_iBufLength = 1;
			m_pBuf[0] = cChar;
			m_pBuf[1] = '\0';
		}while(false);
	}

	m_bStrLenChanged = TRUE;

	return *this;
}

int CString::SetContent(const char cChar, const int iCharCnt)
{
	int iOutRet = 0;

	if(0 > iCharCnt) {return ERROR_INVALID_PARAMETER;}

	if(m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
		m_iBufLength = 0;
	}

	m_pBuf = new char[iCharCnt+1];
	if(NULL == m_pBuf) {return ERROR_OUT_OF_MEMORY;}

	m_iBufLength = iCharCnt;
	
	int iId = 0;

	for(iId=0; iId<iCharCnt; iId++)
	{
		m_pBuf[iId] = cChar;
	}
	m_pBuf[iCharCnt] = '\0';

	m_bStrLenChanged = TRUE;

	return iOutRet;
}

INT_t CString::setContent(LPCSTR pszSrc, size_t SrcSizeToFetch)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == pszSrc || '\0' == pszSrc[0])
		{
			m_pBuf = new char[1];
			if(NULL == m_pBuf)
			{
				m_iLastErrNo = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_pBuf[0] = '\0';
			m_bStrLenChanged = FALSE;
			m_iCachedStrLen = 0;
			m_iBufLength = 1;
			break;
		}
		m_iBufLength = SrcSizeToFetch+1;
		m_pBuf = new char[m_iBufLength];
		if(NULL == m_pBuf)
		{
			m_iLastErrNo = ERROR_OUT_OF_MEMORY;
			m_iBufLength = 0;
			m_iCachedStrLen = 0;
			m_bStrLenChanged = FALSE;
			break;
		}
		memcpy(m_pBuf, pszSrc, SrcSizeToFetch);
		m_pBuf[SrcSizeToFetch] = '\0';
		m_bStrLenChanged = TRUE;
		m_iCachedStrLen = 0;
	}while(FALSE);

	return iOutRet;
}

char * CString::GetBufPtr()
{		
	m_bStrLenChanged = TRUE;

	if(m_pBuf)
	{
		return m_pBuf;
	}
	else
	{
		return g_szNullStr;
	}
}

void CString::Attach(char * pszSrc)
{
	if(m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
		m_iBufLength = 0;
	}

	m_pBuf = pszSrc;
	m_bStrLenChanged = TRUE;
}

char * CString::Detach()
{
	char * pszRet = NULL;

	pszRet = m_pBuf;
	m_pBuf = NULL;
	m_iBufLength = 0;

	m_bStrLenChanged = TRUE;

	return pszRet;
}

int CString::SetStrLength(int iLen)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(0 > iLen)
	{
		return ERROR_INVALID_PARAMETER;
	}

	char * pszNewBuf = new char[iLen+1];
	if(NULL == pszNewBuf)
	{
		return ERROR_OUT_OF_MEMORY;
	}
	pszNewBuf[iLen] = '\0'; pszNewBuf[0] = '\0'; 

	if(m_pBuf)
	{
		int iMinLen = GetMinVal(iLen, strlen(m_pBuf));
		memcpy(pszNewBuf, m_pBuf, iMinLen);
		pszNewBuf[iMinLen] = '\0';
		delete m_pBuf; m_pBuf = NULL;
	}
	m_pBuf = pszNewBuf;
	m_iBufLength = iLen;

	m_bStrLenChanged = TRUE;

	return iOutRet;
}

int CString::GetStrLength() const
{
	int iOutRet = 0;

	do{
		if(FALSE == m_bStrLenChanged)
		{
			iOutRet = m_iCachedStrLen;
			break;
		}

		if(m_pBuf)
		{
			iOutRet = strlen(m_pBuf);
		}
		((CString *)this)->m_iCachedStrLen = iOutRet;
		((CString *)this)->m_bStrLenChanged = FALSE;
	}while(FALSE);
	
	return iOutRet;
}

int CString::GetBufLength()
{
	return m_iBufLength;
}

bool CString::operator==(const CString & str1)
{
	return operator==((const char *)str1); 
}

bool CString::operator==(const char * pszStr2)
{
	bool bOutRet = false;
	const char * pStr1 = NULL;
	const char * pStr2 = NULL;

	pStr1 = pszStr2;
	pStr2 = GetBufPtr();

	if(NULL == pStr1 || NULL == pStr2)
	{
		bOutRet = false;
	}
	else 
	{
		bOutRet = (0 == strcmp(pStr1, pStr2));
	}

	return bOutRet;
}

bool CString::operator!=(const CString & str1)
{
	return operator!=((const char *)str1); 
}

bool CString::operator !=(const char * pszStr2)
{
	return !(operator==(pszStr2));
}

bool CString::Less(CONST CString & str2) CONST
{
	LPCSTR pszStr1 = *this;
	LPCSTR pszStr2 = str2;
	return (0 > strcmp(pszStr1, pszStr2));
}

int CString::InsertAt(int iIndex, char cInChar)
{
	int iOutRet = 0, iLen = 0;
	char * pNewStr = NULL;

	if(0 > iIndex) {return ERROR_INVALID_PARAMETER;}

	do{
		if(0 == m_pBuf)
		{
			m_pBuf = new char[2];
			if(NULL == m_pBuf)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_iBufLength = 1;
			m_pBuf[0] = cInChar;  m_pBuf[1] = '\0';
		}
		else 
		{
			iLen = strlen(m_pBuf);
			
			pNewStr = new char[iLen+1+1];
			if(NULL == pNewStr)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			
			if((iLen - 1) < iIndex)
			{
				strncpy(pNewStr, m_pBuf, iLen);
				pNewStr[iLen] = cInChar;				
			}
			else
			{
				strncpy(pNewStr, m_pBuf, iIndex);
				pNewStr[iIndex] = cInChar;
				strncpy(pNewStr+iIndex+1, m_pBuf+iIndex, iLen-iIndex);
			}
			
			pNewStr[iLen+1] = '\0';

			delete m_pBuf;  m_pBuf = NULL;
			m_pBuf = pNewStr;
			m_iBufLength = iLen+1;
		}
	}while(false);

	m_bStrLenChanged = TRUE;

	return iOutRet;
}

int CString::Replace(int iIndex, char cInChar)
{
	int iOutRet = 0, iLen = 0;

	if(0 > iIndex) {return ERROR_INVALID_PARAMETER;}

	if(NULL == m_pBuf) {return ERROR_INDEX_IS_OUT_OF_RANGE;}
	
	iLen = strlen(m_pBuf);

	if((iLen - 1) < iIndex) {return ERROR_INVALID_PARAMETER;}

	m_pBuf[iIndex] = cInChar;

	return iOutRet;
}

int CString::RemoveAt(int iIndex)
{
	int iOutRet = 0, iLen = 0;
	char * pNewStr = NULL;

	if(0 > iIndex) {return ERROR_INVALID_PARAMETER;}

	if(NULL == m_pBuf) {return ERROR_INDEX_IS_OUT_OF_RANGE;}

	iLen = strlen(m_pBuf);

	if((iLen - 1) < iIndex) {return ERROR_INVALID_PARAMETER;}

	pNewStr = new char[iLen+1]; 
	if(NULL == pNewStr)
	{
		return ERROR_OUT_OF_MEMORY;
	}

	if(0 == iIndex)
	{
		strncpy(pNewStr, m_pBuf+1, iLen-1);
	}
	else	 if((iLen-1) > iIndex)
	{
		strncpy(pNewStr, m_pBuf, iIndex);
		strncpy(pNewStr+iIndex, m_pBuf+iIndex+1, iLen-iIndex-1);		
	}
	else if((iLen-1) == iIndex)
	{
		strncpy(pNewStr, m_pBuf, iIndex);
	}
	pNewStr[iLen-1] = '\0';

	if(m_pBuf)
	{
		delete m_pBuf;  m_pBuf = NULL;
	}

	m_pBuf = pNewStr;
	m_iBufLength = iLen-1;

	m_bStrLenChanged = TRUE;
	
	return iOutRet;
}

int CString::GetLastErrNo()
{
	return m_iLastErrNo;
}

BOOL_t CString::Equal(LPCSTR pszStr1, CString::CaseSensitivity eCaseSensitivity/* = CString::CaseSensitive*/)
{
	BOOL_t bOutIsEqual = FALSE;
	size_t Str1Len = (pszStr1?strlen(pszStr1):0);

	do
	{
		if(GetStrLength() != (typeof(int))Str1Len)
		{
			break;
		}
		bOutIsEqual = Equal(pszStr1, Str1Len, eCaseSensitivity);
	}while(FALSE);

	return bOutIsEqual;
}

BOOL_t CString::Equal(LPCSTR pszStr1, UINT uiBytesToCmp, CString::CaseSensitivity eCaseSensitivity/* = CString::CaseSensitive*/)
{
	BOOL bOutRet = FALSE;
	int iRet;

	do
	{
		if(NULL == m_pBuf && NULL == pszStr1)
		{
			bOutRet = TRUE;
			break;
		}

		if(NULL == m_pBuf)
		{
			if('\0' == pszStr1[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(NULL == pszStr1)
		{
			if('\0' == m_pBuf[0])
			{
				bOutRet = TRUE;
				break;
			}
		}

		if(0 == uiBytesToCmp)
		{
			if(m_pBuf[0] == '\0' && pszStr1[0] == '\0')
			{
				bOutRet = TRUE;
			}
			else
			{
				bOutRet = FALSE;
			}
			break;
		}

		if(CaseInsensitive == eCaseSensitivity)
		{
			iRet = strncasecmp(m_pBuf, pszStr1, uiBytesToCmp);
		}
		else
		{
			iRet = strncmp(m_pBuf, pszStr1, uiBytesToCmp);
		}
		if(0 == iRet)
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

BOOL_t CString::Equal(CString & Str1)
{
	return Equal((LPCSTR)Str1);
}

LPCSTR CString::SearchStr(LPCSTR pStr1, UINT uiStartOff)
{
	LPCSTR pszOutRet = NULL;

	do{
		if(NULL == pStr1)
		{
			break;
		}

		if(GetStrLength() < (int)uiStartOff)
		{
			break;
		}

		if(NULL == m_pBuf)
		{
			break;
		}

		pszOutRet = strstr(m_pBuf+uiStartOff, pStr1);
	}while(FALSE);

	return pszOutRet;
}

CString::STD_STRING_LIST CString::Split(char DeliminatedChar, CString::SplitOptions eSplitOptions/* = KeepEmptyParts*/)
{
	STD_STRING_LIST StrList;
	char * pScanStartPos = NULL;
	char * pDeliminatedCharPos = NULL;

	do
	{
		//NULL
		if(NULL == m_pBuf)
		{
			break;
		}
		//Empty
		if('\0' == m_pBuf[0])
		{
			break;
		}
		//Available string
		pScanStartPos = m_pBuf;
		while(TRUE)
		{
			pDeliminatedCharPos = strchr(pScanStartPos, DeliminatedChar);
			if(pDeliminatedCharPos)
			{
				//LOG_BLINE("Found \n");
				if(0 < (pDeliminatedCharPos - pScanStartPos) || (CString::KeepEmptyParts == eSplitOptions))
				{
					//LOG_BLINE("StrList.push_back(CString(pScanStartPos, pDeliminatedCharPos - pScanStartPos))\n");
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
				if('\0' == *pScanStartPos)
				{
					break;	//end the scan
				}
			}
			else	//last one
			{
				//LOG_BLINE("StrList.push_back(CString(pScanStartPos))\n");
				StrList.push_back(CString(pScanStartPos));
				break;
			}
		}
	}while(FALSE);

	return StrList;
}

INT_t CString::toInt()
{
	INT_t iOutRet = 0;

	do
	{
		if(NULL == m_pBuf)
		{
			break;
		}
		iOutRet = atoi(m_pBuf);
	}while(FALSE);

	return iOutRet;
}

char CString::operator[](int iIndex)
{
	char cOutRet = 0;
	
	do
	{
		if(0 > iIndex || GetStrLength() <= iIndex)
		{
			break;
		}
		if(NULL == m_pBuf)
		{
			break;
		}
		cOutRet = m_pBuf[iIndex];
	}while(FALSE);

	return cOutRet;
}

INT CString::SetStr(LPCSTR pszSrc, BOOL_t bRemaingMemIfPossible/*FALSE*/)
{
	INT iOutRet = ERROR_SUCCESS;
	size_t sizeStr;
	m_iLastErrNo = ERROR_SUCCESS;

	do{	
		if(NULL == pszSrc)
		{
			break;
		}
		
		sizeStr = strlen(pszSrc);
		if(bRemaingMemIfPossible && m_iBufLength >= (typeof(m_iBufLength))sizeStr)
		{
			memcpy(m_pBuf, pszSrc, sizeStr+1);			
		}
		else
		{
			*this = pszSrc;
		}
	}while(FALSE);

	m_bStrLenChanged = TRUE;
	
	return iOutRet;
}

CString::operator const char *() const
{		
	if(m_pBuf)
	{
		return m_pBuf;
	}
	else
	{
		return g_szNullStr;
	}
}

bool operator < (CONST CString & str1, CONST CString & str2)
{
	return str1.Less(str2);
}

