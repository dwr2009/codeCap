#include "ThreadSpecific.h"
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include "List.h"
#include "ThreadMutex.h"
#include <ErrPrintHelper.h>
#include <stdlib.h>
#include <unistd.h>

struct tagTHREAD_SPECIFIC_DICTIONARY {
	pthread_t tidOwner;
	UINT uiKey;
	PVOID pData;
};

static BOOL_t g_bInitialized = FALSE;
static P_LIST g_pDictionaryList = NULL;
static P_THREAD_MUTEX g_pDictMutex = NULL;

INT ThrdSpecificDict_Init()
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do{
		if(NULL == g_pDictionaryList)
		{
			g_pDictionaryList = List_new();
			if(NULL == g_pDictionaryList)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}

		if(NULL == g_pDictMutex)
		{
			g_pDictMutex = CreateThreadMutex();
			if(NULL == g_pDictMutex)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		iRet = ThrdSpecificDict_Uninit();
		if(ERROR_SUCCESS != iOutRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}
	else
	{
		g_bInitialized = TRUE;
	}

	return iOutRet;
}

INT ThrdSpecificDict_Uninit()
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	POSITION pPos = NULL;
	P_THREAD_SPECIFIC_DICTIONARY pThrdSpecificDict = NULL;

	do{
		if(NULL == g_pDictionaryList)
		{
			break;
		}

		if(0 < List_GetItemCount(g_pDictionaryList))
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}
		
		pPos = List_GetHeadPosition(g_pDictionaryList);
		while(pPos)
		{
			pThrdSpecificDict = List_GetNext(g_pDictionaryList, &pPos);
			if(pThrdSpecificDict)
			{
				free(pThrdSpecificDict);
				pThrdSpecificDict = NULL;
			}
		}
		List_RemoveAll(g_pDictionaryList);
		List_delete(g_pDictionaryList);
		g_pDictionaryList = NULL;

		if(g_pDictMutex)
		{
			iRet = DestroyThreadMutex(g_pDictMutex);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
			
			g_pDictMutex = NULL;
		}
	}while(FALSE);

	g_bInitialized = FALSE;

	return iOutRet;
}

INT ThrdSpecificDict_setKeyValue(const UINT uiKey, const PVOID pData)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	P_THREAD_SPECIFIC_DICTIONARY pThrdSpecificDict = NULL;
	pthread_t tidSelf = pthread_self();
	POSITION pPos = NULL;
	BOOL_t bIsLocked = FALSE;
	BOOL_t bFound = FALSE;

	/*
	PRINT_BFILE_LINENO;
	printf("ThrdSpecificDict_setKeyValue, Key:%d, pData:%p\n", uiKey, pData);
	*/

	if(g_pDictMutex)
	{
		iRet = ThreadMutex_Lock(g_pDictMutex);
		if(ERROR_SUCCESS == iRet)
		{
			bIsLocked = TRUE;
		}
		else
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	do{
		if(FALSE == g_bInitialized)
		{
			iRet = ThrdSpecificDict_Init();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}

		bFound = FALSE;
		//check
		pPos = List_GetHeadPosition(g_pDictionaryList);
		while(pPos)
		{
			pThrdSpecificDict = List_GetNext(g_pDictionaryList, &pPos);
			if(pThrdSpecificDict)
			{
				if(pThrdSpecificDict->tidOwner == tidSelf && pThrdSpecificDict->uiKey == uiKey)
				{
					bFound = TRUE;
					pThrdSpecificDict->pData = pData;	// overwrite
					break;
				}
			}
		}

		if(bFound)
		{
			break;
		}

		//not found it
		pThrdSpecificDict = malloc(sizeof(THREAD_SPECIFIC_DICTIONARY));
		if(NULL == pThrdSpecificDict)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		pThrdSpecificDict->tidOwner = tidSelf;
		pThrdSpecificDict->uiKey = uiKey;
		pThrdSpecificDict->pData = pData;

		pPos = List_AddTail(g_pDictionaryList, pThrdSpecificDict);
		if(NULL == pPos)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			if(pThrdSpecificDict)
			{
				free(pThrdSpecificDict);
				pThrdSpecificDict = NULL;
				break;
			}
			break;
		}
	}while(FALSE);

	if(bIsLocked)
	{
		if(g_pDictMutex)
		{
			iRet = ThreadMutex_Unlock(g_pDictMutex);
			if(ERROR_SUCCESS == iRet)
			{
				bIsLocked = FALSE;
			}
			else
			{
				PRINT_FILE_LINENO_BUG_STR;
			}
		}
		else
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
	}

	return iOutRet;
}

INT ThrdSpecificDict_getValueFromKey(const UINT uiKey, PVOID * ppData)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bIsLocked = FALSE;
	BOOL_t bFound = FALSE;
	POSITION pPos = NULL;
	P_THREAD_SPECIFIC_DICTIONARY pThrdSpecificDict = NULL;
	pthread_t tidSelf = pthread_self();

	if(g_pDictMutex)
	{
		iRet = ThreadMutex_Lock(g_pDictMutex);
		if(ERROR_SUCCESS == iRet)
		{
			bIsLocked = TRUE;
		}
		else
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	do{
		if(NULL == ppData)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(FALSE == g_bInitialized)
		{
			iRet = ThrdSpecificDict_Init();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}

		bFound = FALSE;
		//check
		pPos = List_GetHeadPosition(g_pDictionaryList);
		while(pPos)
		{
			pThrdSpecificDict = List_GetNext(g_pDictionaryList, &pPos);
			if(pThrdSpecificDict)
			{
				if(pThrdSpecificDict->tidOwner == tidSelf && pThrdSpecificDict->uiKey == uiKey)
				{
					bFound = TRUE;
					*ppData = pThrdSpecificDict->pData;
					break;
				}
			}
		}
		
		if(bFound)
		{
			break;
		}
		else
		{
			iOutRet = ERROR_NOT_FOUND;
			*ppData = NULL;
			break;
		}
	}while(FALSE);

	if(bIsLocked)
	{
		if(g_pDictMutex)
		{
			iRet = ThreadMutex_Unlock(g_pDictMutex);
			if(ERROR_SUCCESS == iRet)
			{
				bIsLocked = FALSE;
			}
			else
			{
				PRINT_FILE_LINENO_BUG_STR;
			}
		}
		else
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
	}

	return iOutRet;
}

INT ThrdSpecificDict_ReleaseKey(const UINT uiKey)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bIsLocked = FALSE;
	BOOL_t bFound = FALSE;
	POSITION pPos = NULL, pCurPos = NULL;
	P_THREAD_SPECIFIC_DICTIONARY pThrdSpecificDict = NULL;
	pthread_t tidSelf = pthread_self();

	/*
	PRINT_BFILE_LINENO;
	printf("ThrdSpecificDict_ReleaseKey, Key:%d\n", uiKey);
	*/

	if(g_pDictMutex)
	{
		iRet = ThreadMutex_Lock(g_pDictMutex);
		if(ERROR_SUCCESS == iRet)
		{
			bIsLocked = TRUE;
		}
		else
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	do{
		if(FALSE == g_bInitialized)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}
		
		bFound = FALSE;
		//check
		pPos = List_GetHeadPosition(g_pDictionaryList);
		while(pPos)
		{
			pCurPos = pPos;
			pThrdSpecificDict = List_GetNext(g_pDictionaryList, &pPos);
			if(pThrdSpecificDict)
			{
				if(pThrdSpecificDict->tidOwner == tidSelf && pThrdSpecificDict->uiKey == uiKey)
				{
					bFound = TRUE;
					List_RemoveAt(g_pDictionaryList, pCurPos);
					break;
				}
			}
		}
		
		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	if(bIsLocked)
	{
		if(g_pDictMutex)
		{
			iRet = ThreadMutex_Unlock(g_pDictMutex);
			if(ERROR_SUCCESS == iRet)
			{
				bIsLocked = FALSE;
			}
			else
			{
				PRINT_FILE_LINENO_BUG_STR;
			}
		}
		else
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
	}

	return iOutRet;
}

INT ThrdSpecificDict_DestroySelfThrdData()
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bIsLocked = FALSE;
	POSITION pPos = NULL, pCurPos = NULL;
	P_THREAD_SPECIFIC_DICTIONARY pThrdSpecificDict = NULL;
	pthread_t tidSelf = pthread_self();

	if(g_pDictMutex)
	{
		iRet = ThreadMutex_Lock(g_pDictMutex);
		if(ERROR_SUCCESS == iRet)
		{
			bIsLocked = TRUE;
		}
		else
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	do{		
		if(FALSE == g_bInitialized)
		{
			iRet = ThrdSpecificDict_Init();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}

		//check
		pPos = List_GetHeadPosition(g_pDictionaryList);
		while(pPos)
		{
			pCurPos = pPos;
			pThrdSpecificDict = List_GetNext(g_pDictionaryList, &pPos);
			if(pThrdSpecificDict)
			{
				if(pThrdSpecificDict->tidOwner == tidSelf)
				{
					free(pThrdSpecificDict);
					pThrdSpecificDict = NULL;
				}
			}
			List_RemoveAt(g_pDictionaryList, pCurPos);
		}
	}while(FALSE);

	if(bIsLocked)
	{
		if(g_pDictMutex)
		{
			iRet = ThreadMutex_Unlock(g_pDictMutex);
			if(ERROR_SUCCESS == iRet)
			{
				bIsLocked = FALSE;
			}
			else
			{
				PRINT_FILE_LINENO_BUG_STR;
			}
		}
		else
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
	}

	return iOutRet;
}

