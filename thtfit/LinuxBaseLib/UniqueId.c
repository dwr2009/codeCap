#include "UniqueId.h"
#include "ThreadMutex.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

#define	MAX_ID_ARRAY_SIZE			64
#define	ID_FREE_FLAG				0

static BOOL_t g_bInited = FALSE;
static P_THREAD_MUTEX g_pIdMutex = NULL;
static INT g_arrId[MAX_ID_ARRAY_SIZE] = {0, };

INT UniqueId_Init()
{
	INT iOutRet = ERROR_SUCCESS, iId, iRet;

	do{
		if(g_bInited)
		{
			iOutRet = ERROR_ALREADY_INITED;
			break;
		}

		if(NULL == g_pIdMutex)
		{
			g_pIdMutex = CreateThreadMutex();
			if(NULL == g_pIdMutex)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}

		for(iId=0; iId<MAX_ID_ARRAY_SIZE; iId++)
		{
			g_arrId[iId] = ID_FREE_FLAG;
		}

		g_bInited = TRUE;
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		iRet = UniqueId_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_BUG_STR;
		}
	}

	return iOutRet;
}

INT UniqueId_Uninit()
{
	INT iOutRet = ERROR_SUCCESS, iRet, iId;
	BOOL_t bHaveReleasedId = FALSE;

	do{
		if(FALSE == g_bInited)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(g_pIdMutex)
		{
			iRet = DestroyThreadMutex(g_pIdMutex);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_IRET_BUG_STR;
			}
			
			g_pIdMutex = NULL;
		}

		for(iId=0; iId<MAX_ID_ARRAY_SIZE; iId++)
		{
			if(ID_FREE_FLAG != g_arrId[iId])
			{
				bHaveReleasedId = TRUE;
			}
			g_arrId[iId] = ID_FREE_FLAG;
		}
		if(bHaveReleasedId)
		{
			PRINT_FILE_LINENO_BUG_STR;
		}
		
		g_bInited = FALSE;
	}while(FALSE);
	
	return iOutRet;
}

INT UniqueId_getNextId(P_INT pUniqueId)
{
	INT iOutRet = ERROR_SUCCESS, iRet, iId;
	BOOL_t bIsLocked = FALSE, bFound = FALSE;

	if(g_pIdMutex)
	{
		iRet = ThreadMutex_Lock(g_pIdMutex);
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
		if(NULL == pUniqueId)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		//find a free id
		for(iId=0; iId<MAX_ID_ARRAY_SIZE; iId++)
		{
			if(ID_FREE_FLAG == g_arrId[iId])
			{
				//found
				*pUniqueId = iId + 1;
				g_arrId[iId] = *pUniqueId;
				bFound = TRUE;
				break;
			}
			else if(0 > g_arrId[iId])
			{
				PRINT_FILE_LINENO_BUG_STR;
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
		if(g_pIdMutex)
		{
			iRet = ThreadMutex_Unlock(g_pIdMutex);
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

INT UniqueId_ReleaseId(const INT iUniqueId)
{
	INT iOutRet = ERROR_SUCCESS, iRet, iId;
	BOOL_t bIsLocked = FALSE, bFound = FALSE;;

	if(g_pIdMutex)
	{
		iRet = ThreadMutex_Lock(g_pIdMutex);
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
		if(0 >= iUniqueId || MAX_ID_ARRAY_SIZE < iUniqueId)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bFound = FALSE;
		//find the id
		for(iId=0; iId<MAX_ID_ARRAY_SIZE; iId++)
		{
			if(iUniqueId == g_arrId[iId])
			{
				//found
				g_arrId[iId] = ID_FREE_FLAG;
				bFound = TRUE;
				break;
			}
			else if(0 > g_arrId[iId])
			{
				PRINT_FILE_LINENO_BUG_STR;
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
		if(g_pIdMutex)
		{
			iRet = ThreadMutex_Unlock(g_pIdMutex);
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

