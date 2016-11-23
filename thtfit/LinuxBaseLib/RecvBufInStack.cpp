#include "RecvBufInStack.h"
#include <stdio.h>
#include <string.h>
#include <math2.h>
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <Compiler.h>

CRecvBufInStack::CRecvBufInStack(PBYTE pBufInStack, INT_t BufSize)
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

CRecvBufInStack::~CRecvBufInStack()
{
}

int CRecvBufInStack::SetBufSize(int iSize)
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

int CRecvBufInStack::SetReadStartOff(int iOffset)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(0 > iOffset || m_iBufSize <= iOffset)
	{
		return ERROR_INVALID_PARAMETER;
	}

	m_iDataOffset = iOffset;

	return iOutRet;
}

int CRecvBufInStack::GetBufSize()
{
	return m_iBufSize;
}

INT CRecvBufInStack::CopyData(PBYTE pBuf, INT iSize)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		if(0 < iSize)
		{
			if(NULL == pBuf)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else if(0 == iSize)
		{

		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = SetBufSize(iSize);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(0 == iSize)
		{
			break;
		}

		memcpy(m_pBuf, pBuf, iSize);
	}while(FALSE);

	return iOutRet;
}

int CRecvBufInStack::GetCurReadOff()
{
	return m_iDataOffset;
}

int CRecvBufInStack::GetDataWithAutoOff(PBYTE pBuf, int iSizeToGet)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(NULL == pBuf || 0 > iSizeToGet)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if(m_iBufSize < m_iDataOffset + iSizeToGet)
	{
		return ERROR_OUT_OF_RANGE;
	}

	memcpy(pBuf, m_pBuf+m_iDataOffset, iSizeToGet);
	m_iDataOffset += iSizeToGet;

	return iOutRet;
}

INT_t CRecvBufInStack::getDataWithAutoOff(INT_t * CONST piData)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(NULL == piData)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if(m_iBufSize < m_iDataOffset + sizeof(*piData))
	{
		return ERROR_OUT_OF_RANGE;
	}

	memcpy(piData, m_pBuf+m_iDataOffset, sizeof(*piData));
	m_iDataOffset += sizeof(*piData);

	return iOutRet;
}

int CRecvBufInStack::GetDataWithAutoOff(PBYTE pBuf, unsigned long ulSizeToGet)
{
	return GetDataWithAutoOff(pBuf, (int)ulSizeToGet);
}

int CRecvBufInStack::GetDataWithAutoOff(OUT LPCSTR & pStr, OUT size_t & StrLen)
{
	int iOutRet = ERROR_SUCCESS;
	UINT32_t StrLenField;

	do
	{
		if(m_iBufSize < m_iDataOffset + 4)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy(&StrLenField, m_pBuf+m_iDataOffset, 4);
		m_iDataOffset += 4;
		
		if(m_iBufSize < m_iDataOffset + (int)StrLenField)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		pStr = (typeof(pStr))(m_pBuf+m_iDataOffset);
		m_iDataOffset += StrLenField;

		StrLen = StrLenField;
	}while(FALSE);

	return iOutRet;
}

PBYTE CRecvBufInStack::GetBufPtr()
{
	return m_pBuf;
}

