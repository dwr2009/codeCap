#ifndef	_BASELIST_H_
#define	_BASELIST_H_

#include	<BaseTypeDef.h>
#include	<BaseErrDef.h>
#include	<ErrPrintHelper.h>

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE = const TYPE&>
class CList
{
protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		TYPE data;
	};
public:
// Construction
	CList();

// Attributes (head and tail)
	// count of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// peek at head or tail
	TYPE * GetHeadAddr();
	TYPE * GetTailAddr();

// Operations
	int GetHeadAndRemove(TYPE * pData);
	// get head or tail (and remove it) - don't call on empty list !
	void RemoveHead();
	void RemoveTail();

	// add before head or after tail
	POSITION AddHead(ARG_TYPE newElement);
	POSITION AddTail(ARG_TYPE newElement);
	void AddTail(CList <TYPE, ARG_TYPE> * pNewList);

	// remove all elements
	void RemoveAll();

	// iteration
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	TYPE& GetNext(POSITION& rPVOID); // return *POSITION++
	const TYPE& GetNext(POSITION& rPVOID) const; // return *POSITION++
	TYPE& GetPrev(POSITION& rPVOID); // return *POSITION--
	const TYPE& GetPrev(POSITION& rPVOID) const; // return *POSITION--

	// getting/modifying an element at a given POSITION
	TYPE& GetAt(POSITION pPos);
	const TYPE& GetAt(POSITION pPos) const;
	void SetAt(POSITION pos, ARG_TYPE newElement);
	void RemoveAt(POSITION pPos);

	// inserting before or after a given POSITION
	POSITION InsertBefore(POSITION pPos, ARG_TYPE newElement);
	POSITION InsertAfter(POSITION pPos, ARG_TYPE newElement);

	// helper functions (note: O(n) speed)
	POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	POSITION FindIndex(int nIndex) const;
		// get the 'nIndex'th element (may return NULL)

// Implementation
protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	int m_nCount;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);

public:
	~CList();
};

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
int CList<TYPE, ARG_TYPE>::GetCount() const
{
	return m_nCount; 
}

template<class TYPE, class ARG_TYPE>
BOOL CList<TYPE, ARG_TYPE>::IsEmpty() const
{
	return (0 == m_nCount); 
}

template<class TYPE, class ARG_TYPE>
TYPE * CList<TYPE, ARG_TYPE>::GetHeadAddr()
{
	if(NULL == m_pNodeHead) {return NULL;}
	return &(m_pNodeHead->data); 
}

template<class TYPE, class ARG_TYPE>
TYPE * CList<TYPE, ARG_TYPE>::GetTailAddr()
{
	if(NULL == m_pNodeTail) {return NULL;}
	return &(m_pNodeTail->data); 
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::GetHeadPosition() const
{
	return (POSITION)m_pNodeHead; 
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::GetTailPosition() const
{
	return (POSITION) m_pNodeTail; 
}

template<class TYPE, class ARG_TYPE>
TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION & pPos) // return *POSITION++
{ 
	CNode* pNode = (CNode*) pPos;
	pPos = (POSITION)(pNode->pNext);
	
	return pNode->data; 
}
template<class TYPE, class ARG_TYPE>
const TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION & pPos) const // return *POSITION++
{	
	CNode* pNode = (CNode*) pPos;
	pPos = (POSITION)(pNode->pNext);
	
	return pNode->data; 
}

template<class TYPE, class ARG_TYPE>
TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPVOID) // return *POSITION--
{ 
	CNode* pNode = (CNode*) rPVOID;
	rPVOID = (POSITION)(pNode->pPrev);
	return pNode->data; 
}

template<class TYPE, class ARG_TYPE>
const TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPVOID) const // return *POSITION--
{ 
	CNode* pNode = (CNode*) rPVOID;
	rPVOID = (POSITION) pNode->pPrev;
	return pNode->data; 
}

template<class TYPE, class ARG_TYPE>
TYPE& CList<TYPE, ARG_TYPE>::GetAt(POSITION pPos)
{ 
	CNode* pNode = (CNode*)pPos;
	return pNode->data; 
}

template<class TYPE, class ARG_TYPE>
const TYPE& CList<TYPE, ARG_TYPE>::GetAt(POSITION pPos) const
{
	CNode* pNode = (CNode*) pPos;
	return pNode->data; 
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::SetAt(POSITION pPos, ARG_TYPE newElement)
{
	CNode* pNode = (CNode*) pPos;
	pNode->data = newElement; 
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::CList()
{
	m_nCount = 0;
	m_pNodeHead = NULL;
	m_pNodeTail = NULL;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAll()
{
	CNode * pCurNode = NULL;

	if(0 == m_nCount && (m_pNodeHead || m_pNodeTail))
	{
		PRINT_FILE_LINENO_BUG_STR;
	}
	
	pCurNode = m_pNodeHead;
	while(pCurNode)
	{
		CNode * pNextNode = pCurNode->pNext;
		delete pCurNode;
		pCurNode = pNextNode;
	}

	m_nCount = 0;
	m_pNodeHead = NULL;
	m_pNodeTail = NULL;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::~CList()
{
	RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: CNode's are stored in CPlex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a CPlex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of CPlex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::CNode*
CList<TYPE, ARG_TYPE>::NewNode(CNode* pPrev, CNode* pNext)
{
	CNode * pNewNode = NULL;

	pNewNode = new CNode();
	if(NULL == pNewNode) {return NULL;}

	CNode* pNode = pNewNode;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;

	return pNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::FreeNode(CNode* pNode)
{
	if(0 >= m_nCount) {return;}

	delete pNode;
	m_nCount--;

	// if no more elements, cleanup completely
	if (m_nCount == 0)
	{
		RemoveAll();
	}
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	if(NULL == pNewNode) {return NULL;}
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
	{
		m_pNodeHead->pPrev = pNewNode;
	}
	else
	{
		m_pNodeTail = pNewNode;
	}
	m_pNodeHead = pNewNode;
	
	return (POSITION)pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	if(NULL == pNewNode) {return NULL;}
	
	pNewNode->data = newElement;
	
	if(m_pNodeTail)
	{
		m_pNodeTail->pNext = pNewNode;
	}
	else
	{
		m_pNodeHead = pNewNode;
	}
	m_pNodeTail = pNewNode;
	
	return pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(CList <TYPE, ARG_TYPE> * pNewList)
{
	POSITION pPos = NULL;

	if(pNewList)
	{
		pPos = pNewList->GetHeadPosition();
		while(pPos)
		{
			AddTail(pNewList->GetNext(pPos));
		}
	}
}

template<class TYPE, class ARG_TYPE>
int CList<TYPE, ARG_TYPE>::GetHeadAndRemove(TYPE * pData)
{
	int iOutRet = 0;
	POSITION pPos = NULL;

	if(NULL == pData) 
	{
		return ERROR_INVALID_PARAMETER;
	}
	
	if(0 >= GetCount())
	{
		return ERROR_NOT_FOUND;
	}

	pPos = GetHeadPosition();
	if(NULL == pPos)
	{
		return ERROR_NOT_FOUND;
	}
	
	*pData = GetAt(pPos);

	RemoveHead();

	return iOutRet;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveHead()
{
	if(NULL == m_pNodeHead) 
	{
		PRINT_FILE_LINENO_BUG_STR;
		return;
	}

	CNode* pOldNode = m_pNodeHead;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
	{
		m_pNodeHead->pPrev = NULL;
	}
	else
	{
		m_pNodeTail = NULL;
	}
	FreeNode(pOldNode);
	
	return ;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveTail()
{
	if(NULL == m_pNodeTail) 
	{
		PRINT_FILE_LINENO_BUG_STR;
		return;
	}

	CNode* pOldNode = m_pNodeTail;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
	{
		m_pNodeTail->pNext = NULL;
	}
	else
	{
		m_pNodeHead = NULL;
	}
	FreeNode(pOldNode);
	
	return;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertBefore(POSITION pPos, ARG_TYPE newElement)
{
	if (pPos == NULL)
	{
		return AddHead(newElement); // insert before nothing -> head of the list
	}

	// Insert it before pPos
	CNode* pOldNode = (CNode*) pPos;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertAfter(POSITION pPos, ARG_TYPE newElement)
{
	if (pPos == NULL)
	{
		return AddTail(newElement); // insert after nothing -> tail of the list
	}

	// Insert it before pPos
	CNode* pOldNode = (CNode*) pPos;
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAt(POSITION pPos)
{
	CNode* pOldNode = (CNode*) pPos;

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::FindIndex(int nIndex) const
{
	if (nIndex >= m_nCount || nIndex < 0)
	{
		return NULL;  // went too far
	}

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		pNode = pNode->pNext;
	}
	return (POSITION) pNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, POSITION startAfter) const
{
	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
	{
		if(pNode->data == searchValue)
		{
			return (POSITION)pNode;
		}
	}
	return NULL;
}

//Predefined sub class
typedef CList <void *, void *> CPtrList;

#endif	//_BASELIST_H_

