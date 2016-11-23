#include <ByteArray2.h>

CByteArray2::CByteArray2()
{
}
CByteArray2::~CByteArray2()
{
}

INT_t CByteArray2::Append(BYTE * pArg, INT_t iCnt)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t iOldSize;
	
	do
	{
		if(NULL == pArg || 0 > iCnt)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == iCnt)
		{
			break;
		}
		iOldSize = m_nSize;
		iRet = SetSize(m_nSize+iCnt, -1);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		memcpy(&(m_pData[iOldSize]), pArg, iCnt);
	}while(FALSE);

	return iOutRet;
}

INT_t CByteArray2::operator = (CONST CByteArray2 & otherByteArray2)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	size_t otherSize = otherByteArray2.GetSize();

	do
	{
		if(0 > otherSize)
		{
			otherSize = 0;
		}
		iRet = SetSize(otherSize, -1);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		memcpy(m_pData, otherByteArray2.GetData(), otherSize);
	}while(FALSE);

	return iOutRet;
}

