#include "RuaMemory.h"
#include <File.h>
#include <CCplusplusBridge.h>

CRuaMemory::CRuaMemory()
{

}

CRuaMemory::~CRuaMemory()
{

}

INT CRuaMemory::get_RuaMemorySnapshot(RUA_MEMORY_SNAPSHOT_COLLECTION & RuaMemSnapshotCollection)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	CFile oRuaMemRscFile;
	char szBuf[512];
	RUA_MEMORY_SNAPSHOT itemRuaMemSnapshot;
	
	do{
		RuaMemSnapshotCollection.RemoveAll();
		
		iRet = oRuaMemRscFile.Open("/proc/driver/em8xxx/0/resources", "rt");
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		//Skip the first line
		iRet = oRuaMemRscFile.ReadLine((LPBYTE)szBuf, sizeof(szBuf), OPT_TRIM_TAILING_NEWLINE);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		while(TRUE)
		{
			iRet = oRuaMemRscFile.ReadLine((LPBYTE)szBuf, sizeof(szBuf), OPT_TRIM_TAILING_NEWLINE);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			szBuf[sizeof(szBuf)-1] = '\0';

			if(0 == szBuf[0])
			{
				break;
			}

			iRet = sscanf(szBuf, "*[MM%d]* %u [%p, %u", &(itemRuaMemSnapshot.iMemBankId),
				&(itemRuaMemSnapshot.uiSerialNumber), &(itemRuaMemSnapshot.pMemAddr), 
				&(itemRuaMemSnapshot.uiSize));
			if(4 != iRet)
			{
				continue;
			}

			iRet = RuaMemSnapshotCollection.Add(itemRuaMemSnapshot);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			//*
			PRINT_FILE_LINENO;
			printf("RuaMem%d: %u, %p, %u\n", itemRuaMemSnapshot.iMemBankId, itemRuaMemSnapshot.uiSerialNumber, itemRuaMemSnapshot.pMemAddr,
				itemRuaMemSnapshot.uiSize);
			//*/
		}
	}while(FALSE);

	iRet = oRuaMemRscFile.Close();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_BUG_STR;
	}

	return iOutRet;
}


BOOL_t CRuaMemory::ExistRuaMemAddr(PVOID pRuaMemAddr, int * piMemBankId)
{
	BOOL_t bOutExist = FALSE;
	INT iRet, iId;
	RUA_MEMORY_SNAPSHOT_COLLECTION oRuaMemSnapshotColle;

	do
	{
		if(NULL == pRuaMemAddr)
		{
			break;
		}

		iRet = get_RuaMemorySnapshot(OUT oRuaMemSnapshotColle);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		for(iId = 0; iId < oRuaMemSnapshotColle.GetSize(); iId++)
		{
			if(pRuaMemAddr == oRuaMemSnapshotColle[iId].pMemAddr)
			{
				if(piMemBankId)
				{
					*piMemBankId = oRuaMemSnapshotColle[iId].iMemBankId;
				}
				bOutExist = TRUE;
				break;
			}
		}
	}while(FALSE);

	return bOutExist;
}


