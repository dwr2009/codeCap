#ifndef	_ARRAY_H_
#define	_ARRAY_H_

#include "AfxTempl.h"
#include "BaseErrDef.h"
#include "ErrPrintHelper.h"
#include <new>
#include <BaseTypeDef.h>

template < class TYPE, class ARG_TYPE = const TYPE& > 
class CArray 
{
public:
	CArray();
	virtual ~CArray();
	int Add(ARG_TYPE newElement);
	virtual INT_t Append(TYPE * pArg, INT_t iCnt);
	int SetAtGrow(int nIndex, ARG_TYPE newElement);	
	int SetSize(int nNewSize, int nGrowBy = -1);
	int GetSize() const;
	void RemoveAll();
	// Accessing elements
	const TYPE& GetAt(int nIndex) const;
	TYPE& GetAt(int nIndex);
	void SetAt(int nIndex, ARG_TYPE newElement);
	const TYPE& ElementAt(int nIndex) const;
	TYPE& ElementAt(int nIndex);
	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();
	// overloaded operator helpers
	const TYPE& operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	INT RemoveAt(int nIndex, int nCount = 1);

	BOOL_t Exist(ARG_TYPE ElementToFind);
	INT_t AddIfNotExist(ARG_TYPE ElementToAdd);
	INT_t RemoveIfExist(ARG_TYPE ElementToRemove);
	
// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount	
};

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray()
{
	if (m_pData != NULL)
	{
		for( int i = 0; i < m_nSize; i++ )
			(m_pData + i)->~TYPE();
		delete[] (BYTE*)m_pData;
	}
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
{ 
	return SetAtGrow(m_nSize, newElement);
}

template<class TYPE, class ARG_TYPE>
INT_t CArray<TYPE, ARG_TYPE>::Append(TYPE * pArg, INT_t iCnt)
{ 
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t iId;
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
		for(iId=0; iId<iCnt; iId++)
		{
			m_pData[iOldSize+iId] = pArg[iId];
		}
	}while(FALSE);

	return iOutRet;
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	int iRet;

	iRet = ERROR_SUCCESS;

	if(nIndex < 0)
		return ERROR_INDEX_IS_OUT_OF_RANGE;
		
	if (nIndex >= m_nSize)
	{
		iRet = SetSize(nIndex+1, -1);
	}

	if(ERROR_SUCCESS != iRet) {return iRet;}
	
	m_pData[nIndex] = newElement;

	return iRet;
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy/*-1*/)
{
	if(nNewSize < 0 )
		return ERROR_INDEX_IS_OUT_OF_RANGE;
	
	if (nGrowBy >= 0)
		m_nGrowBy = nGrowBy;  // set new size

	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			for( int i = 0; i < m_nSize; i++ )
				(m_pData + i)->~TYPE();
			delete[] (BYTE*)m_pData;
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create buffer big enough to hold number of requested elements or
		// m_nGrowBy elements, whichever is larger.
		size_t nAllocSize = (nNewSize>m_nGrowBy?nNewSize:m_nGrowBy);
		m_pData = (TYPE*) new BYTE[(size_t)nAllocSize * sizeof(TYPE)];
		if(NULL == m_pData) {return ERROR_OUT_OF_MEMORY;}
		memset((void*)m_pData, 0, (size_t)nAllocSize * sizeof(TYPE));
		for( int i = 0; i < nNewSize; i++ )
		{
			::new( (void*)( m_pData + i ) ) TYPE;
		}
		m_nSize = nNewSize;
		m_nMaxSize = nAllocSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			memset((void*)(m_pData + m_nSize), 0, (size_t)(nNewSize-m_nSize) * sizeof(TYPE));
			for( int i = 0; i < nNewSize-m_nSize; i++ )
			{
				::new( (void*)( m_pData + m_nSize + i ) ) TYPE;
			}
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			for( int i = 0; i < m_nSize-nNewSize; i++ )
				(m_pData + nNewSize + i)->~TYPE();
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		nGrowBy = m_nGrowBy;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize / 8;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		TYPE* pNewData = (TYPE*) new BYTE[(size_t)nNewMax * sizeof(TYPE)];
		if(NULL == pNewData) return ERROR_OUT_OF_MEMORY;

		// copy new data from old
		memcpy(pNewData, m_pData, (size_t)m_nSize * sizeof(TYPE));

		// construct remaining elements
		memset((void*)(pNewData + m_nSize), 0, (size_t)(nNewSize-m_nSize) * sizeof(TYPE));
		for( int i = 0; i < nNewSize-m_nSize; i++ )
		{
			::new( (void*)( pNewData + m_nSize + i ) ) TYPE;
		}

		// get rid of old stuff (note: no destructors called)
		delete[] (BYTE*)m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}

	return ERROR_SUCCESS;
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::GetSize() const
{
	return m_nSize; 
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAll()
{
	SetSize(0, -1); 
}

template<class TYPE, class ARG_TYPE>
const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const
{
	return (const TYPE*)m_pData; 
}

template<class TYPE, class ARG_TYPE>
TYPE* CArray<TYPE, ARG_TYPE>::GetData()
{
	return (TYPE*)m_pData; 
}

template<class TYPE, class ARG_TYPE>
const TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
{
	return GetAt(nIndex); 
}

template<class TYPE, class ARG_TYPE>
TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex)
{
	return ElementAt(nIndex); 
}

template<class TYPE, class ARG_TYPE>
TYPE& CArray<TYPE, ARG_TYPE>::GetAt(int nIndex)
{ 
	if(nIndex >= 0 && nIndex < m_nSize)
		return m_pData[nIndex]; 
	else 
	{
		PRINT_FILE_LINENO_BUG_STR;
	}
}

template<class TYPE, class ARG_TYPE>
const TYPE& CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
{
	if(nIndex >= 0 && nIndex < m_nSize)
		return m_pData[nIndex]; 
	else 
	{
		PRINT_FILE_LINENO_BUG_STR;
		return m_pData[0];
	}
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
{ 
	if(nIndex >= 0 && nIndex < m_nSize)
	{
		m_pData[nIndex] = newElement;
	}
	else 
	{
		LOG_BLINE("BUG,nIndex=%d,m_nSize=%d\n", nIndex, m_nSize);
	}
}

template<class TYPE, class ARG_TYPE>
const TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex) const
{ 
	if(nIndex >= 0 && nIndex < m_nSize)
		return m_pData[nIndex]; 
	else 
	{
		PRINT_FILE_LINENO_BUG_STR;
	}
}
template<class TYPE, class ARG_TYPE>
TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
{ 
	if(nIndex >= 0 && nIndex < m_nSize)
		return m_pData[nIndex]; 
	else 
	{
		PRINT_FILE_LINENO_BUG_STR;
		return m_pData[0];
	}
}

template<class TYPE, class ARG_TYPE>
INT CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	if(!(nIndex >= 0 && nCount > 0 && nIndex + nCount <= m_nSize))
	{
		return ERROR_INVALID_PARAMETER;
	}

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements<TYPE>(&m_pData[nIndex], nCount);
	if (nMoveCount)
		memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;

	return ERROR_SUCCESS;
}

template<class TYPE, class ARG_TYPE>
BOOL_t CArray<TYPE, ARG_TYPE>::Exist(ARG_TYPE ElementToFind)
{
	BOOL_t bExist = FALSE;
	INT_t iId;

	do
	{
		if(NULL == m_pData)
		{
			break;
		}
		for(iId = 0; iId < m_nSize; iId++)
		{
			if(m_pData[iId] == ElementToFind)
			{
				bExist = TRUE;
				break;				
			}
		}
	}while(FALSE);

	return bExist;
}

template<class TYPE, class ARG_TYPE>
INT_t CArray<TYPE, ARG_TYPE>::AddIfNotExist(ARG_TYPE ElementToAdd)
{
	INT_t iOutRet = ERROR_SUCCESS;
	INT_t iRet;

	do
	{
		if(Exist(ElementToAdd))
		{
			break;
		}
		iRet = Add(ElementToAdd);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

template<class TYPE, class ARG_TYPE>
INT_t CArray<TYPE, ARG_TYPE>::RemoveIfExist(ARG_TYPE ElementToRemove)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t iId;
	BOOL_t bRestart = FALSE;

	do
	{
		if(NULL == m_pData)
		{
			break;
		}
		while(bRestart)
		{
			bRestart = FALSE;
			for(iId = 0; iId < m_nSize; iId++)
			{
				if(m_pData[iId] == ElementToRemove)
				{
					iRet = RemoveAt(iId, 1);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					bRestart = TRUE;
					break;
				}
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

typedef CArray <unsigned char>	CByteArray;
typedef CArray <INT>	CIntArray;

#endif	//_ARRAY_H_

