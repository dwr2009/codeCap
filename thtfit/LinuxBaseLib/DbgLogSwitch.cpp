#include <DbgLogSwitch.h>
#include <FileMutexLocker.h>
#include <GlobalShareMem.h>
#include <memory.h>

#define	MAX_DBG_LOG_SWITCH_COUNT		(200)

#define	DLS_SHARED_MEM_SIZE				(sizeof(DBG_LOG_SWITCH_HEADER) + (sizeof(DBG_LOG_SWITCH_ITEM) * MAX_DBG_LOG_SWITCH_COUNT))

class CDbgLogSwitch
{
public:
	typedef struct
	{
		INT_t CurSwitchCount;
	}DBG_LOG_SWITCH_HEADER, *P_DBG_LOG_SWITCH_HEADER;
	typedef struct
	{
		BOOL_t bEnable;
		char szDesc[64];
	}DBG_LOG_SWITCH_ITEM, *P_DBG_LOG_SWITCH_ITEM;
public:
	CDbgLogSwitch();
	~CDbgLogSwitch();
	INT_t getLastErrNo();
	BOOL_t * RegSwitch(LPCSTR pszSwitchDesc);
	INT_t getSwitchCount();
	INT_t getSwitchInfo(INT_t iSwitchId, BOOL_t * pbEnable, LPCSTR * ppszDesc);
	INT_t setSwitch(INT_t iSwitchId, BOOL_t bEnable);
private:
	CFileMutexLocker m_SharedMemLocker;
	CGlobalShareMem m_SharedData;
	INT_t m_iLastErrNo;
	P_DBG_LOG_SWITCH_HEADER m_pSharedDataHeader;
	P_DBG_LOG_SWITCH_ITEM m_pDbgLogSwitchItem;
};

CDbgLogSwitch::CDbgLogSwitch() : m_SharedMemLocker("/tmp/DbgLogSwitchLocker")
{
	int iRet;
	BOOL_t bLocked = FALSE;

	m_pSharedDataHeader = NULL;
	m_pDbgLogSwitchItem = NULL;
	m_iLastErrNo = ERROR_SUCCESS;
	
	do
	{
		if(ERROR_SUCCESS != m_SharedMemLocker.GetLastErrNo() || ERROR_SUCCESS != m_SharedMemLocker.GetLastCrtErrNo())
		{
			m_iLastErrNo = m_SharedMemLocker.GetLastErrNo();
			LOG_BLINE("Err=%d,CrtErr=%d\n", m_SharedMemLocker.GetLastErrNo(), m_SharedMemLocker.GetLastCrtErrNo());
			break;
		}
		if(FALSE == bLocked)
		{
			iRet = m_SharedMemLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		iRet = m_SharedData.CreateOrGetShareMem("/tmp/DbgLogSwitchDataKey", DLS_SHARED_MEM_SIZE);
		if(ERROR_SUCCESS != iRet)
		{
			m_iLastErrNo = iRet;
			break;
		}
		iRet = m_SharedData.setRemoveShmIdIfCreatedByMe(FALSE);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		m_pSharedDataHeader = (typeof(m_pSharedDataHeader))(((PBYTE)m_SharedData.GetShareMemAddr()));
		m_pDbgLogSwitchItem = (typeof(m_pDbgLogSwitchItem))((((PBYTE)m_pSharedDataHeader) + sizeof(*m_pSharedDataHeader)));
		if(m_SharedData.GetIsCreated())
		{			
			ZeroMemory(m_pSharedDataHeader, sizeof(*m_pSharedDataHeader));
			ZeroMemory(m_pDbgLogSwitchItem, (sizeof(DBG_LOG_SWITCH_ITEM) * MAX_DBG_LOG_SWITCH_COUNT));
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_SharedMemLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}	
}

CDbgLogSwitch::~CDbgLogSwitch()
{
}

INT_t CDbgLogSwitch::getLastErrNo()
{
	return m_iLastErrNo;
}

BOOL_t * CDbgLogSwitch::RegSwitch(LPCSTR pszSwitchDesc)
{
	BOOL_t * pbOutRet = NULL;
	int iRet, iIndex;
	BOOL_t bLocked = FALSE;
	P_DBG_LOG_SWITCH_ITEM pDbgLogSwitchItem = NULL;
	BOOL_t bExist;

	m_iLastErrNo = ERROR_SUCCESS;

	do
	{
		if(NULL == pszSwitchDesc)
		{
			break;
		}
		if('\0' == pszSwitchDesc[0])
		{
			break;
		}
		if(FALSE == bLocked)
		{
			iRet = m_SharedMemLocker.Lock();
			if(ERROR_SUCCESS == iRet)
			{
				bLocked = TRUE;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		//Check whether it has been registered
		bExist = FALSE;
		for(iIndex = 0; iIndex < m_pSharedDataHeader->CurSwitchCount; iIndex++)
		{
			pDbgLogSwitchItem = m_pDbgLogSwitchItem + iIndex;
			if(0 == strcmp(pszSwitchDesc, pDbgLogSwitchItem->szDesc))
			{
				bExist = TRUE;
				pbOutRet = &(pDbgLogSwitchItem->bEnable);
				break;
			}
		}
		if(bExist)
		{
			break;
		}
		
		if(MAX_DBG_LOG_SWITCH_COUNT <= m_pSharedDataHeader->CurSwitchCount)
		{
			break;
		}
		m_pSharedDataHeader->CurSwitchCount++;
		pDbgLogSwitchItem = m_pDbgLogSwitchItem + (m_pSharedDataHeader->CurSwitchCount - 1);
		pDbgLogSwitchItem->bEnable = FALSE;
		strncpy(pDbgLogSwitchItem->szDesc, pszSwitchDesc, sizeof(pDbgLogSwitchItem->szDesc));
		pDbgLogSwitchItem->szDesc[sizeof(pDbgLogSwitchItem->szDesc)-1] = '\0';
		pbOutRet = &(pDbgLogSwitchItem->bEnable);
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_SharedMemLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return pbOutRet;
}

INT_t CDbgLogSwitch::getSwitchCount()
{
	INT_t iOutSwitchCnt = -1, iRet;
	BOOL_t bLocked = FALSE;

	m_iLastErrNo = ERROR_SUCCESS;

	if(FALSE == bLocked)
	{
		iRet = m_SharedMemLocker.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	do
	{
		iOutSwitchCnt = m_pSharedDataHeader->CurSwitchCount;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_SharedMemLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutSwitchCnt;
}

INT_t CDbgLogSwitch::getSwitchInfo(INT_t iSwitchId, BOOL_t * pbEnable, LPCSTR * ppszDesc)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	P_DBG_LOG_SWITCH_ITEM pDbgLogSwitchItem = NULL;
	BOOL_t bLocked = FALSE;

	if(FALSE == bLocked)
	{
		iRet = m_SharedMemLocker.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	m_iLastErrNo = ERROR_SUCCESS;

	do
	{		
		if(0 > iSwitchId || (m_pSharedDataHeader->CurSwitchCount - 1) < iSwitchId ||
			NULL == pbEnable || NULL == ppszDesc)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		pDbgLogSwitchItem = m_pDbgLogSwitchItem + iSwitchId;
		*pbEnable = pDbgLogSwitchItem->bEnable;
		*ppszDesc = pDbgLogSwitchItem->szDesc;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_SharedMemLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

INT_t CDbgLogSwitch::setSwitch(INT_t iSwitchId, BOOL_t bEnable)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	P_DBG_LOG_SWITCH_ITEM pDbgLogSwitchItem = NULL;
	BOOL_t bLocked = FALSE;

	if(FALSE == bLocked)
	{
		iRet = m_SharedMemLocker.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	m_iLastErrNo = ERROR_SUCCESS;

	do
	{
		if(0 > iSwitchId || (m_pSharedDataHeader->CurSwitchCount - 1) < iSwitchId)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		pDbgLogSwitchItem = m_pDbgLogSwitchItem + iSwitchId;
		pDbgLogSwitchItem->bEnable = bEnable;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_SharedMemLocker.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	return iOutRet;
}

static CDbgLogSwitch * g_pDbgLogSwitch = NULL;

INT_t DbgLogSwitch_Init(void)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(g_pDbgLogSwitch)
		{
			iOutRet = ERR_ALREADY_INITIALIZED;
			break;
		}
		g_pDbgLogSwitch = new CDbgLogSwitch();
		if(NULL == g_pDbgLogSwitch)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iOutRet = g_pDbgLogSwitch->getLastErrNo();
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		iRet = DbgLogSwitch_Uninit();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

BOOL_t * DbgLogSwitch_RegSwitch(LPCSTR pszSwitchDesc)
{
	BOOL_t * pbOutRet = NULL;

	do
	{
		if(NULL == g_pDbgLogSwitch)
		{
			break;
		}
		pbOutRet = g_pDbgLogSwitch->RegSwitch(pszSwitchDesc);
	}while(FALSE);

	return pbOutRet;
}

INT_t DbgLogSwitch_getSwitchCount()
{
	INT_t iOutSwitchCnt = -1;

	do
	{
		if(NULL == g_pDbgLogSwitch)
		{
			break;
		}
		iOutSwitchCnt = g_pDbgLogSwitch->getSwitchCount();
	}while(FALSE);

	return iOutSwitchCnt;
}

INT_t DbgLogSwitch_getSwitchInfo(INT_t iSwitchId, BOOL_t * pbEnable, LPCSTR * ppszDesc)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == g_pDbgLogSwitch)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}
		iOutRet = g_pDbgLogSwitch->getSwitchInfo(iSwitchId, pbEnable, ppszDesc);
	}while(FALSE);

	return iOutRet;
}

INT_t DbgLogSwitch_setSwitch(INT_t iSwitchId, BOOL_t bEnable)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == g_pDbgLogSwitch)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}
		iOutRet = g_pDbgLogSwitch->setSwitch(iSwitchId, bEnable);
	}while(FALSE);

	return iOutRet;
}

INT_t DbgLogSwitch_Uninit(void)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == g_pDbgLogSwitch)
		{
			break;
		}
		delete g_pDbgLogSwitch;
		g_pDbgLogSwitch = NULL;
	}while(FALSE);

	return iOutRet;
}

