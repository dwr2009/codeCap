#include	"SendBufInStack.h"
#include	<stdio.h>
#include	<math2.h>
#include	<BaseTypeDef.h>
#include	<BaseErrDef.h>
#include <string.h>
#include <Compiler.h>

CSendBufInStack::CSendBufInStack(PBYTE pBufInStack, INT_t BufSize)
{
	if(CC_UNLIKELY(NULL == pBufInStack || 0 > BufSize))
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
	
	m_iDataOffset = 0;
	m_pBuf = pBufInStack;
	m_iBufSize = BufSize;
	m_iAllocatedMemSize = BufSize;
}

CSendBufInStack::~CSendBufInStack()
{
}

int CSendBufInStack::SetBufSize(int iSize)
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > iSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(0 == iSize)
		{
			m_iBufSize = 0;
			iOutRet = ERROR_SUCCESS;
			break;
		}

		if(iSize > m_iAllocatedMemSize)
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}
		
		m_iBufSize = iSize;
	}while(false);

	return iOutRet;
}

int CSendBufInStack::SetWriteStartOff(int iOffset)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(0 > iOffset || m_iBufSize <= iOffset)
	{
		return ERROR_INVALID_PARAMETER;
	}

	m_iDataOffset = iOffset;

	return iOutRet;
}

int CSendBufInStack::GetBufSize()
{
	return m_iBufSize;
}

int CSendBufInStack::GetCurWriteOff()
{
	return m_iDataOffset;
}

int CSendBufInStack::WriteDataWithAutoOff(CONST PBYTE pBuf, int iSizeToWrite)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(NULL == pBuf || 0 > iSizeToWrite)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if(m_iBufSize < m_iDataOffset + iSizeToWrite)
	{
		return ERROR_OUT_OF_RANGE;
	}

	memcpy( m_pBuf+m_iDataOffset, pBuf, iSizeToWrite);
	m_iDataOffset += iSizeToWrite;

	return iOutRet;
}

int CSendBufInStack::WriteDataWithAutoOff(int iVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&iVal), sizeof(iVal));
}

int CSendBufInStack::WriteDataWithAutoOff(unsigned int uiVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&uiVal), sizeof(uiVal));
}

int CSendBufInStack::WriteDataWithAutoOff(unsigned char ucVal)
{
	return WriteDataWithAutoOff(&ucVal, sizeof(ucVal));
}

int CSendBufInStack::WriteDataWithAutoOff(void * pVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&pVal), sizeof(pVal));
}

INT_t CSendBufInStack::WriteDataWithAutoOff(CStackBufString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT32_t StrLenWithNull = strValue.getStringLength() + 1;

	do
	{
		if(m_iBufSize < m_iDataOffset + 4)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy( m_pBuf+m_iDataOffset, &StrLenWithNull, 4);
		m_iDataOffset += 4;

		if(m_iBufSize < m_iDataOffset + StrLenWithNull)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy( m_pBuf+m_iDataOffset, (LPCSTR)strValue, StrLenWithNull);
		m_iDataOffset += StrLenWithNull;
	}while(FALSE);

	return iOutRet;
}

INT_t CSendBufInStack::WriteDataWithAutoOff(LPCSTR pszVal)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT32_t StrLenWithNull;

	do
	{
		if(NULL == pszVal)
		{
			pszVal = "";
		}

		StrLenWithNull = strlen(pszVal) + 1;
		if(m_iBufSize < m_iDataOffset + 4)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy(m_pBuf+m_iDataOffset, &StrLenWithNull, 4);
		m_iDataOffset += 4;

		if(m_iBufSize < m_iDataOffset + StrLenWithNull)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy( m_pBuf+m_iDataOffset, pszVal, StrLenWithNull);
		m_iDataOffset += StrLenWithNull;
	}while(FALSE);

	return iOutRet;
}

PBYTE CSendBufInStack::GetBufPtr()
{
	return m_pBuf;
}

