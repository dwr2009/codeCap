#include	"SendBuf2.h"
#include	<stdio.h>
#include	<math2.h>
#include	<BaseTypeDef.h>
#include	<BaseErrDef.h>
#include <string.h>

int CSendBuf2::m_iObjCnt = 0;

int CSendBuf2::getObjCnt()
{
	return m_iObjCnt;
}

CSendBuf2::CSendBuf2()
{
	m_iObjCnt++;
	m_iDataOffset = 0;
	m_pBuf = NULL;
	m_iBufSize = 0;
	m_iAllocatedMemSize = 0;
}

CSendBuf2::~CSendBuf2()
{
	if(m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = NULL;
	}
	m_iObjCnt--;
}

int CSendBuf2::SetBufSize(int iSize, bool bRemainOldMemSizeIfPossible/*false*/)
{
	int iOutRet = ERROR_SUCCESS;
	void * pVoid = NULL;
	
	if(0 > iSize)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if(0 == iSize)
	{
		if(m_pBuf)
		{
			delete m_pBuf;
			m_pBuf = NULL;
		}
		m_iBufSize = 0;
		m_iAllocatedMemSize = 0;
		return ERROR_SUCCESS;
	}

	do{
		if(m_pBuf)
		{
			if(bRemainOldMemSizeIfPossible)
			{
				if(iSize <= m_iAllocatedMemSize)
				{
					m_iBufSize = iSize;
					break;
				}
			}
			
			pVoid = new unsigned char[iSize];
			if(NULL == pVoid)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			memmove(pVoid, m_pBuf, GetMinVal(m_iBufSize, iSize));
			m_iBufSize = iSize;
			m_iAllocatedMemSize = iSize;
			delete m_pBuf;
			m_pBuf = NULL;

			m_pBuf = (unsigned char *)pVoid;
		}
		else
		{
			m_pBuf = new unsigned char[iSize];
			if(NULL == m_pBuf)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_iBufSize = iSize;
			m_iAllocatedMemSize = iSize;
		}
	}while(false);

	return iOutRet;
}

int CSendBuf2::SetWriteStartOff(int iOffset)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(0 > iOffset || m_iBufSize <= iOffset)
	{
		return ERROR_INVALID_PARAMETER;
	}

	m_iDataOffset = iOffset;

	return iOutRet;
}

int CSendBuf2::GetBufSize()
{
	if(NULL ==  m_pBuf)
	{
		return 0;
	}

	return m_iBufSize;
}

int CSendBuf2::GetCurWriteOff()
{
	return m_iDataOffset;
}

int CSendBuf2::WriteDataWithAutoOff(CONST PBYTE pBuf, int iSizeToWrite)
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

int CSendBuf2::WriteDataWithAutoOff(int iVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&iVal), sizeof(iVal));
}

int CSendBuf2::WriteDataWithAutoOff(unsigned int uiVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&uiVal), sizeof(uiVal));
}

int CSendBuf2::WriteDataWithAutoOff(unsigned char ucVal)
{
	return WriteDataWithAutoOff(&ucVal, sizeof(ucVal));
}

int CSendBuf2::WriteDataWithAutoOff(void * pVal)
{
	return WriteDataWithAutoOff((unsigned char *)(&pVal), sizeof(pVal));
}

INT_t CSendBuf2::WriteDataWithAutoOff(CStackBufString & strValue)
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

		if(m_iBufSize < m_iDataOffset + (INT_t)StrLenWithNull)
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}
		memcpy( m_pBuf+m_iDataOffset, (LPCSTR)strValue, StrLenWithNull);
		m_iDataOffset += StrLenWithNull;
	}while(FALSE);

	return iOutRet;
}

PBYTE CSendBuf2::GetBufPtr()
{
	return m_pBuf;
}

