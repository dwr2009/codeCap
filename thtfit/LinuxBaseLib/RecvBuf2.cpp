#include "RecvBuf2.h"
#include <stdio.h>
#include <string.h>
#include <math2.h>
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <Compiler.h>

CRecvBuf2::CRecvBuf2()
{
	m_iDataOffset = 0;
	m_pBuf = NULL;
	m_iBufSize = 0;
	m_iAllocatedMemSize = 0;
}

CRecvBuf2::~CRecvBuf2()
{
	FreeBuf();
}

int CRecvBuf2::SetBufSize(int iSize, BOOL_t bRemainOldMemSizeIfPossible/*FALSE*/)
{
	int iOutRet = ERROR_SUCCESS;
	
	do{
		if(0 > iSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(0 == iSize)
		{
			FreeBuf();
			iOutRet = ERROR_SUCCESS;
			break;
		}
		
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
		}
		RenewBuf(iSize);
	}while(false);

	return iOutRet;
}

int CRecvBuf2::SetReadStartOff(int iOffset)
{
	int iOutRet = ERROR_SUCCESS;
	
	if(0 > iOffset || m_iBufSize <= iOffset)
	{
		return ERROR_INVALID_PARAMETER;
	}

	m_iDataOffset = iOffset;

	return iOutRet;
}

int CRecvBuf2::GetBufSize()
{
	if(NULL ==  m_pBuf)
	{
		return 0;
	}

	return m_iBufSize;
}

INT CRecvBuf2::CopyData(PBYTE pBuf, INT iSize, BOOL bRemainOldMemSizeIfPossible/* = FALSE*/)
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

		iRet = SetBufSize(iSize, bRemainOldMemSizeIfPossible);
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

int CRecvBuf2::GetCurReadOff()
{
	return m_iDataOffset;
}

int CRecvBuf2::GetDataWithAutoOff(unsigned char * pBuf, int iSizeToGet)
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

INT_t CRecvBuf2::getDataWithAutoOff(INT_t * CONST piBuf)
{
	return GetDataWithAutoOff((PBYTE)piBuf, (int)sizeof(*piBuf));
}

int CRecvBuf2::GetDataWithAutoOff(unsigned char * pBuf, unsigned long ulSizeToGet)
{
	return GetDataWithAutoOff(pBuf, (int)ulSizeToGet);
}

int CRecvBuf2::GetDataWithAutoOff(OUT LPCSTR & pStr, OUT size_t & StrLen)
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

PBYTE CRecvBuf2::GetBufPtr()
{
	return m_pBuf;
}

PBYTE CRecvBuf2::NewBuf(size_t sizeNew)
{
	PBYTE pOutRet = NULL;

	do{
		if(NULL == pOutRet)
		{
			pOutRet = new BYTE[sizeNew];
		}
	}while(FALSE);

	return pOutRet;
}

INT CRecvBuf2::RenewBuf(size_t sizeNew)
{
	INT iOutRet = ERROR_SUCCESS;
	PVOID pVoid = NULL;

	do{
		pVoid = NewBuf(sizeNew);
		if(NULL == pVoid)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(m_pBuf)
		{
			memcpy(pVoid, m_pBuf, GET_MIN_VAL(sizeNew, (typeof(sizeNew))m_iBufSize));
			FreeBuf();
		}
		m_pBuf = (typeof(m_pBuf))pVoid;
		m_iBufSize = sizeNew;
		m_iAllocatedMemSize = sizeNew;
	}while(FALSE);

	return iOutRet;
}

void CRecvBuf2::FreeBuf()
{
	INT iRet = ERROR_SUCCESS;

	C_UNUSED(iRet);
	
	if(m_pBuf)
	{
		delete m_pBuf;
		m_pBuf = NULL;
		m_iBufSize = 0;
		m_iAllocatedMemSize = 0;
	}
}

