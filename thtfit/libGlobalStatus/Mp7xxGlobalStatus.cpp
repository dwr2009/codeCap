#include "Api/Mp7xxGlobalStatus.h"
#include <memory.h>

CMp7xxGlobalStatus::CMp7xxGlobalStatus() :
	CGlobalStatus("/tmp/Mp7xxGlobalStatus", sizeof(CMp7xxGlobalStatus::GLOBAL_STATUS_INFO))
{
	INT_t iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();

	if(pStatusInfo)
	{
		if(m_StatusMapFile.FileIsCreatedByMe())
		{
			ZeroMemory(pStatusInfo, sizeof(*pStatusInfo));
			pStatusInfo->sizeOfThisStruct = sizeof(*pStatusInfo);
#if 1/*added by lxj 2012-11-21*/
			pStatusInfo->iPlayStatus = PLAY_STAT_IDLE;
#endif
		}
		else
		{
			if(sizeof(CMp7xxGlobalStatus::GLOBAL_STATUS_INFO) != pStatusInfo->sizeOfThisStruct)
			{
				LOG_BLINE("BUG,CurStructSize=%d,OldStructSize=%d\n", sizeof(CMp7xxGlobalStatus::GLOBAL_STATUS_INFO),
					pStatusInfo->sizeOfThisStruct);
			}
		}
	}
	else
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

CMp7xxGlobalStatus::~CMp7xxGlobalStatus()
{

}

INT_t CMp7xxGlobalStatus::Lock()
{
	return m_StatusMapFile.Lock();
}

INT_t CMp7xxGlobalStatus::Unlock()
{
	return m_StatusMapFile.Unlock();
}

CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO CMp7xxGlobalStatus::getStatusInfo(CONST BOOL_t bNeedLock/* = TRUE*/)
{
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	INT_t iRet;
	BOOL_t bLocked = FALSE;

	if(bNeedLock)
	{
		iRet = m_StatusMapFile.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	
	return pStatusInfo;
}

INT_t CMp7xxGlobalStatus::setCurEthSettings(BOOL_t bAutoGetIP, UINT32_t uiIpV4Addr, UINT32_t uiSubnetMaskV4)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->CurEthSettings.bAutoGetIP = bAutoGetIP;
		pStatusInfo->CurEthSettings.uiIpV4Addr = uiIpV4Addr;
		pStatusInfo->CurEthSettings.uiSubnetMaskV4 = uiSubnetMaskV4;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::getCurEthSettings(OUT BOOL_t & bAutoGetIP, OUT UINT32_t & uiIpV4Addr, OUT UINT32_t & uiSubnetMaskV4)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bAutoGetIP = pStatusInfo->CurEthSettings.bAutoGetIP;
		uiIpV4Addr = pStatusInfo->CurEthSettings.uiIpV4Addr;
		uiSubnetMaskV4 = pStatusInfo->CurEthSettings.uiSubnetMaskV4;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::setEnableLogging(BOOL_t bEnableCeLabsLogging)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bEnableCeLabsLogging = bEnableCeLabsLogging;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::getEnableLogging(OUT BOOL_t & bEnableCeLabsLogging)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnableCeLabsLogging = pStatusInfo->bEnableCeLabsLogging;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::set_UsbIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bUsbIsMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::get_UsbIsMounted(OUT BOOL_t & bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bMounted = pStatusInfo->bUsbIsMounted;
	}while(FALSE);

	return iOutRet;
}

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::set_SdIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bSdIsMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::get_SdIsMounted(OUT BOOL_t & bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bMounted = pStatusInfo->bSdIsMounted;
	}while(FALSE);

	return iOutRet;
}

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::set_HddIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bHddIsMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

/*
Because 32bit ops is atomic to cpu, does not need mutex.
*/
INT_t CMp7xxGlobalStatus::get_HddIsMounted(OUT BOOL_t & bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bMounted = pStatusInfo->bHddIsMounted;
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::get_WritingFs_Info(BOOL_t * CONST pbWritingFs, time_t * CONST ptmLastTimeToWriteFs)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(pbWritingFs)
		{
			*pbWritingFs = pStatusInfo->bWritingFs;
		}
		if(ptmLastTimeToWriteFs)
		{
			*ptmLastTimeToWriteFs = pStatusInfo->tmLastTimeToWriteFs;
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::set_WritingFs_Info(CONST BOOL_t bWritingFs, CONST time_t tmLastTimeToWriteFs)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bWritingFs = bWritingFs;
		pStatusInfo->tmLastTimeToWriteFs = tmLastTimeToWriteFs;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::TestAndSet_LedIndicatorHwInited(CONST BOOL_t bInited, CONST BOOL_t bNewVal, BOOL_t * CONST pbOldVal)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(pbOldVal)
		{
			*pbOldVal = pStatusInfo->bLedIndicator_HwInited;
		}
		if(pStatusInfo->bLedIndicator_HwInited == bInited)
		{
			pStatusInfo->bLedIndicator_HwInited = bNewVal;
		}
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::set_LedBusyCounter(CONST INT_t iIncrement, OUT INT_t & iCurVal)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
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
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->iLedBusyCounter += iIncrement;
		iCurVal = pStatusInfo->iLedBusyCounter;
	}while(FALSE);

	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

static CMp7xxGlobalStatus & getMp7xxGlobalStatus();

static CMp7xxGlobalStatus & getMp7xxGlobalStatus()
{
	static CMp7xxGlobalStatus g_Mp7xxGlobalStatus;
	return g_Mp7xxGlobalStatus;
}

INT_t get_UsbIsMounted(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == pbMounted)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bMounted;
		iOutRet = oMp7xxGlobalStatus.get_UsbIsMounted(OUT bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			*pbMounted = FALSE;
			break;
		}
		*pbMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

INT_t set_UsbIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		iOutRet = oMp7xxGlobalStatus.set_UsbIsMounted(bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t get_SdIsMounted(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == pbMounted)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bMounted;
		iOutRet = oMp7xxGlobalStatus.get_SdIsMounted(OUT bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			*pbMounted = FALSE;
			break;
		}
		*pbMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

INT_t set_SdIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		iOutRet = oMp7xxGlobalStatus.set_SdIsMounted(bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t get_HddIsMounted(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == pbMounted)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bMounted;
		iOutRet = oMp7xxGlobalStatus.get_HddIsMounted(OUT bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			*pbMounted = FALSE;
			break;
		}
		*pbMounted = bMounted;
	}while(FALSE);

	return iOutRet;
}

INT_t set_HddIsMounted(CONST BOOL_t bMounted)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		iOutRet = oMp7xxGlobalStatus.set_HddIsMounted(bMounted);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t TestAndSet_LedIndicatorHwInited(CONST BOOL_t bInited, CONST BOOL_t bNewVal, BOOL_t * CONST pbOldVal)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();
	do
	{
		iOutRet = oMp7xxGlobalStatus.TestAndSet_LedIndicatorHwInited(bInited, bNewVal, pbOldVal);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t set_LedBusyCounter(CONST INT_t iIncrement, INT_t * CONST piCurVal)
{
	INT_t iOutRet = ERROR_SUCCESS, iCurVal;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();
	do
	{
		iOutRet = oMp7xxGlobalStatus.set_LedBusyCounter(iIncrement, OUT iCurVal);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		if(piCurVal)
		{
			*piCurVal = iCurVal;
		}
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2012-8-2 for enabled features, export by function*/
INT_t getEnablePlayer(BOOL_t * CONST bEnablePlayer)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == bEnablePlayer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bEnabled;
		iOutRet = oMp7xxGlobalStatus.getEnablePlayer(OUT bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			*bEnablePlayer = FALSE;
			break;
		}
		*bEnablePlayer = bEnabled;
	}while(FALSE);

	return iOutRet;
}

INT_t getEnableDolby(BOOL_t * CONST bEnableDolby)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == bEnableDolby)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bEnabled;
		iOutRet = oMp7xxGlobalStatus.getEnableDolby(OUT bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			*bEnableDolby = FALSE;
			break;
		}
		*bEnableDolby = bEnabled;
	}while(FALSE);

	return iOutRet;
}

INT_t getEnableWMA(BOOL_t * CONST bEnableWMA)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == bEnableWMA)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bEnabled;
		iOutRet = oMp7xxGlobalStatus.getEnableWMA(OUT bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			*bEnableWMA = FALSE;
			break;
		}
		*bEnableWMA = bEnabled;
	}while(FALSE);

	return iOutRet;
}

INT_t getEnableAdobeFlash(BOOL_t * CONST bEnableAdobeFlash)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == bEnableAdobeFlash)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bEnabled;
		iOutRet = oMp7xxGlobalStatus.getEnableAdobeFlash(OUT bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			*bEnableAdobeFlash = FALSE;
			break;
		}
		*bEnableAdobeFlash = bEnabled;
	}while(FALSE);

	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-9-29 for irControl*/
INT_t getIrControlEnabled(INT_t* CONST pbEnabled)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == pbEnabled)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		INT_t bEnabled;
		iOutRet = oMp7xxGlobalStatus.getIrControlEnabled(OUT bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			*pbEnabled = FALSE;
			break;
		}
		*pbEnabled = bEnabled;
	}while(FALSE);

	return iOutRet;
}

INT_t setIrControlEnabled(CONST INT_t bEnabled)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		iOutRet = oMp7xxGlobalStatus.setIrControlEnabled(bEnabled);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}
#endif

/*added by lxj 2012-8-2 for enabled features,
   Note: Because 32bit ops is atomic to cpu, does not need mutex.*/
#if 1
INT_t CMp7xxGlobalStatus::setEnabledFeatures(CONST UINT32_t uiEnabledFeatures)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->uiEnabledFeatures = uiEnabledFeatures;
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getEnabledFeatures(OUT int &uiEnabledFeatures)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		uiEnabledFeatures = pStatusInfo->uiEnabledFeatures;
	}while(FALSE);

	return iOutRet;
}


INT_t CMp7xxGlobalStatus::getEnablePlayer(OUT BOOL_t &bEnablePlayer)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnablePlayer = ((pStatusInfo->uiEnabledFeatures & DK_PLAYER_ENABLE ) == DK_PLAYER_ENABLE);
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getEnableDolby(OUT BOOL_t &bEnableDolby)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnableDolby = ((pStatusInfo->uiEnabledFeatures & DK_DOLBY_ENABLE ) == DK_DOLBY_ENABLE);
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getEnableWMA(OUT BOOL_t &bEnableWMA)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnableWMA = ((pStatusInfo->uiEnabledFeatures & DK_WMA_ENABLED ) == DK_WMA_ENABLED);
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getEnableAdobeFlash(OUT BOOL_t &bEnableAdobeFlash)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnableAdobeFlash = ((pStatusInfo->uiEnabledFeatures & DK_ADOBE_FLASH_ENALBED ) == DK_ADOBE_FLASH_ENALBED);
	}while(FALSE);

	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-9-29 for irControl*/
INT_t CMp7xxGlobalStatus::setIrControlEnabled(CONST INT_t bEnabled)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->bIrControlEnabled = bEnabled;
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getIrControlEnabled(OUT INT_t &bEnabled)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		bEnabled = pStatusInfo->bIrControlEnabled;
	}while(FALSE);

	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-11-20*/
INT_t CMp7xxGlobalStatus::setMpSrvPlayStatus(CONST INT_t iPlayStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pStatusInfo->iPlayStatus = iPlayStatus;
	}while(FALSE);

	return iOutRet;
}

INT_t CMp7xxGlobalStatus::getMpSrvPlayStatus(INT_t * CONST pPlayStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	do
	{
		pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
		if(pPlayStatus == NULL ||
			NULL == pStatusInfo)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		*pPlayStatus = pStatusInfo->iPlayStatus;
	}while(FALSE);

	return iOutRet;
}

INT_t getMpSrvPlayStatus(INT_t * CONST pPlayStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		if(NULL == pPlayStatus)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		INT_t iPlayStatus;
		iOutRet = oMp7xxGlobalStatus.getMpSrvPlayStatus(OUT &iPlayStatus);
		if(ERROR_SUCCESS != iOutRet)
		{
			*pPlayStatus = PLAY_STAT_UNKNOWN;
			break;
		}
		*pPlayStatus = iPlayStatus;
	}while(FALSE);

	return iOutRet;
}

INT_t setMpSrvPlayStatus(CONST INT_t iPlayStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	do
	{
		iOutRet = oMp7xxGlobalStatus.setMpSrvPlayStatus(iPlayStatus);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

#endif

INT_t CMp7xxGlobalStatus::setCurGuiFbSurfaceAddr(CONST UINT32_t curGuiFbSurfaceAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
	if(NULL == pStatusInfo)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	pStatusInfo->curGuiFbSurfaceAddr = curGuiFbSurfaceAddr;

EXIT_PROC:
	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t CMp7xxGlobalStatus::getCurGuiFbSurfaceAddr(OUT UINT32_t & curGuiFbSurfaceAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;

	if(FALSE == bLocked)
	{
		iRet = m_StatusMapFile.Lock();
		if(ERROR_SUCCESS == iRet)
		{
			bLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	pStatusInfo = (typeof(pStatusInfo))m_StatusMapFile.getMappedAddr();
	if(NULL == pStatusInfo)
	{
		iOutRet = ERROR_INVALID_STATE;
		goto EXIT_PROC;
	}

	curGuiFbSurfaceAddr = pStatusInfo->curGuiFbSurfaceAddr;

EXIT_PROC:
	if(bLocked)
	{
		iRet = m_StatusMapFile.Unlock();
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

INT_t Mp7xxGlobalStatus_Lock()
{
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();
	return oMp7xxGlobalStatus.Lock();
}

INT_t Mp7xxGlobalStatus_Unlock()
{
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();
	return oMp7xxGlobalStatus.Unlock();
}

CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO Mp7xxGlobalStatus_getStatusInfo(CONST BOOL_t bNeedLock/* = TRUE*/)
{
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();
	return oMp7xxGlobalStatus.getStatusInfo(bNeedLock);
}

INT_t setCurGuiFbSurfaceAddr(CONST UINT32_t curGuiFbSurfaceAddr)
{
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	return oMp7xxGlobalStatus.setCurGuiFbSurfaceAddr(curGuiFbSurfaceAddr);
}

INT_t getCurGuiFbSurfaceAddr(OUT UINT32_t & curGuiFbSurfaceAddr)
{
	CMp7xxGlobalStatus & oMp7xxGlobalStatus = getMp7xxGlobalStatus();

	return oMp7xxGlobalStatus.getCurGuiFbSurfaceAddr(OUT curGuiFbSurfaceAddr);
}

