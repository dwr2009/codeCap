#include	"List.h"
#include <stdlib.h>
#include <MemOpHelper.h>
#include	<BaseErrDef.h>
#include <memory2.h>

typedef struct _LIST_ITEM_DATA
{
	void * pData;
	struct _LIST_ITEM_DATA * pPrevItem;
	struct _LIST_ITEM_DATA * pNextItem;
}LIST_ITEM_DATA, *P_LIST_ITEM_DATA;

struct _LIST
{
	int iItemCount;
	P_LIST_ITEM_DATA pListItemHead, pListItemTail;
};

P_LIST List_new()
{
	P_LIST pListOutRet = NULL;

	pListOutRet = malloc(sizeof(LIST));
	if(pListOutRet)
	{
		ZeroMemory(pListOutRet, sizeof(*pListOutRet));
		pListOutRet->iItemCount = 0;
		pListOutRet->pListItemHead = NULL;
		pListOutRet->pListItemTail = NULL;
	}

	return pListOutRet;
}

void List_delete(P_LIST pThis)
{
	if(pThis)
	{
		List_RemoveAll(pThis);
		
		SAFE_FREE(pThis);
	}
}

POSITION List_AddTail(P_LIST pThis, void * pData)
{
	POSITION pPosOutRet = NULL;
	P_LIST_ITEM_DATA pListItemDataToAdd = NULL;

	do{
		if(NULL == pThis || NULL == pData)
		{
			pPosOutRet = NULL;
			break;
		}

		pListItemDataToAdd = malloc(sizeof(LIST_ITEM_DATA));
		if(NULL == pListItemDataToAdd)
		{
			pPosOutRet = NULL;
			break;
		}

		ZeroMemory(pListItemDataToAdd, sizeof(*pListItemDataToAdd));
		pListItemDataToAdd->pData = pData;
		pListItemDataToAdd->pNextItem = NULL;

		if(pThis->pListItemHead)
		{
			P_LIST_ITEM_DATA pLastListItemData = NULL;
			pLastListItemData = pThis->pListItemHead;
			while(pLastListItemData->pNextItem)
			{
				pLastListItemData = pLastListItemData->pNextItem;
			}
			pListItemDataToAdd->pPrevItem = pLastListItemData;
			pLastListItemData->pNextItem = pListItemDataToAdd;
		}
		else
		{
			pListItemDataToAdd->pPrevItem = NULL;
			pThis->pListItemHead = pListItemDataToAdd;
		}
		pThis->pListItemTail = pListItemDataToAdd;
		pThis->iItemCount++;

		pPosOutRet = pThis->pListItemTail;
	}while(0);

	return pPosOutRet;
}

int List_GetItemCount(P_LIST pThis)
{
	if(pThis)
	{
		return pThis->iItemCount;
	}
	else
	{
		return 0;
	}
}

void List_RemoveAll(P_LIST pThis)
{
	if(pThis)
	{
		if(pThis->pListItemHead)
		{
			P_LIST_ITEM_DATA pCurListItemData = NULL, pNextListItemData = NULL;
			pCurListItemData = pThis->pListItemHead;
			while(pCurListItemData)
			{
				pNextListItemData = pCurListItemData->pNextItem;
				free(pCurListItemData);
				pCurListItemData = pNextListItemData;
			}
			pThis->pListItemHead = NULL;
			pThis->pListItemTail = NULL;
		}
		pThis->iItemCount = 0;
	}
}

POSITION List_GetHeadPosition(P_LIST pThis)
{
	if(NULL == pThis) {return NULL;}

	return pThis->pListItemHead;
}

POSITION List_GetTailPosition(P_LIST pThis) 
{
	if(NULL == pThis) {return NULL;}

	return pThis->pListItemTail;
}

PVOID List_GetNext(P_LIST pThis, POSITION * ppPos)
{
	PVOID pVoidRet = NULL;
	P_LIST_ITEM_DATA pListItemData = NULL;
	
	if(NULL == pThis || NULL == ppPos || NULL == *ppPos) {return NULL;}

	pListItemData = (P_LIST_ITEM_DATA)(*ppPos);

	pVoidRet = pListItemData->pData;

	*ppPos = pListItemData->pNextItem;

	return pVoidRet;
}

PVOID List_GetAt(P_LIST pThis, POSITION pPos)
{
	PVOID pVoidRet = NULL;
	P_LIST_ITEM_DATA pListItemData = NULL;
	
	if(NULL == pThis || NULL == pPos) {return NULL;}

	pListItemData = (P_LIST_ITEM_DATA)pPos;

	pVoidRet = pListItemData->pData;

	return pVoidRet;
}

void List_RemoveAt(P_LIST pThis, POSITION pPos)
{
	P_LIST_ITEM_DATA pListItemDataToRM = (P_LIST_ITEM_DATA)pPos;

	if(NULL == pThis || NULL == pPos) {return;}

	if(pListItemDataToRM == pThis->pListItemHead)
	{
		pThis->pListItemHead = pListItemDataToRM->pNextItem;
	}
	else
	{
		pListItemDataToRM->pPrevItem->pNextItem = pListItemDataToRM->pNextItem;
	}
	
	if(pListItemDataToRM == pThis->pListItemTail)
	{
		pThis->pListItemTail = pListItemDataToRM->pPrevItem;
	}
	else
	{
		pListItemDataToRM->pNextItem->pPrevItem = pListItemDataToRM->pPrevItem;
	}

	SAFE_FREE(pListItemDataToRM);

	pThis->iItemCount--;
}

int List_ExistTargetData(P_LIST pThis, PVOID pVoidData)
{
	POSITION pPos = NULL;
	PVOID pDataToCmp = NULL;
	int bExist = 0;
	
	pPos = List_GetHeadPosition(pThis);
	while(pPos)
	{
		pDataToCmp = List_GetNext(pThis, &pPos);
		if(pVoidData == pDataToCmp)
		{
			bExist = 1;
			break;
		}
	}

	return bExist;
}

