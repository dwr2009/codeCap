#include <GuiOsd2FrameBufferCtrl.h>
#include <Mp7xxGlobalStatus.h>
#include <unistd.h> 
#include <sys/types.h>
#include <ArrayOpsMacro.h>
#include <ProcessList.h>
#if defined(__mips__)
#include <rmdef/rmdef.h>
#include <rua/include/rua.h>
#include <rmcore/include/rmstatustostring.h>

CGuiOsd2FrameBufferCtrl::CGuiOsd2FrameBufferCtrl()
{
	INT_t iRet;
	RMstatus eRMstatus;
	
	m_pRua = NULL;
	m_bHasAddRef = FALSE;
	m_pLumaVirtAddr = NULL;
	m_surfaceLumaSize = 0;
	m_surfaceWidth = 0;
	m_surfaceHeight = 0;
	m_bSurfaceLumaAddrIsLocked = FALSE;
	m_bSurfaceLumaAddrIsMapped = FALSE;

	eRMstatus = RUACreateInstance((struct RUA **)(&(m_pRua)), 0);
	if(RMFAILED(eRMstatus))
	{
		m_pRua = NULL;
		PRINT_BFILE_LINENO_RMSTATUS;
	}

	iRet = GuiOsd2_AddRef();
	if(ERR_SUCCESS == iRet)
	{
		m_bHasAddRef = TRUE;
	}
	else
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	if(m_bHasAddRef)
	{
		iRet = GuiOsd2_mmapSurface();
		if(ERR_SUCCESS == iRet)
		{
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

CGuiOsd2FrameBufferCtrl::~CGuiOsd2FrameBufferCtrl()
{
	INT_t iRet;
	RMstatus eRMstatus;

	iRet = GuiOsd2_munmapSurface();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	//LOG_BLINE("CGuiOsd2FrameBufferCtrl::~CGuiOsd2FrameBufferCtrl,m_bHasAddRef=%d\n", m_bHasAddRef);

	if(m_bHasAddRef)
	{
		iRet = GuiOsd2_RemoveRef();
		if(ERR_SUCCESS == iRet)
		{
			m_bHasAddRef = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	if(m_pRua)
	{
		eRMstatus = RUADestroyInstance((struct RUA *)m_pRua);
		if(RMFAILED(eRMstatus))
		{
			PRINT_BFILE_LINENO_RMSTATUS;
		}
		m_pRua = NULL;
	}
}

INT_t CGuiOsd2FrameBufferCtrl::getSurfaceInfo(OUT UINT8_t * & pSurfaceLumaVirtAddr, OUT UINT32_t & surfaceWidth, OUT UINT32_t & surfaceHeight)
{
	INT_t iOutRet = ERR_SUCCESS;

	if(FALSE == m_bSurfaceLumaAddrIsMapped || NULL == m_pLumaVirtAddr)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	pSurfaceLumaVirtAddr = m_pLumaVirtAddr;
	surfaceWidth = m_surfaceWidth;
	surfaceHeight = m_surfaceHeight;

EXIT_PROC:
	return iOutRet;
}

INT_t CGuiOsd2FrameBufferCtrl::GuiOsd2_AddRef()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	BOOL_t bGlobalStatusIsLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	BOOL_t bNeedLock;
	CONST pid_t callerProcessId = getpid();
	INT_t iId;
	BOOL_t bFound;
	RMstatus eRMstatus;

	if(FALSE == bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Lock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	GuiOsd2_removeNonexistingProcessRef();

	bNeedLock = FALSE;
	pStatusInfo = Mp7xxGlobalStatus_getStatusInfo(bNeedLock);
	if(NULL == pStatusInfo)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(0 == pStatusInfo->secondOsdInfo.surfaceAddr)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	bFound = FALSE;
	for(iId=0; iId<(typeof(iId))ARRAY_COUNT(pStatusInfo->secondOsdInfo.usingCnt); iId++)
	{
		if(callerProcessId == pStatusInfo->secondOsdInfo.usingCnt[iId].processId)
		{
			pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt++;
			bFound = TRUE;
			break;
		}
	}
	if(FALSE == bFound)
	{
		for(iId=0; iId<(typeof(iId))ARRAY_COUNT(pStatusInfo->secondOsdInfo.usingCnt); iId++)
		{
			if(0 == pStatusInfo->secondOsdInfo.usingCnt[iId].processId)
			{
				pStatusInfo->secondOsdInfo.usingCnt[iId].processId = callerProcessId;
				pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt++;
				bFound = TRUE;
				break;
			}
		}
	}
	if(FALSE == bFound)
	{
		iOutRet = ERR_OUT_OF_RANGE;
		goto EXIT_PROC;
	}

	eRMstatus = RUAAcquireAddress((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceAddr);
	if(RMFAILED(eRMstatus))
	{
		PRINT_BFILE_LINENO_RMSTATUS;
	}

EXIT_PROC:
	if(bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Unlock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

INT_t CGuiOsd2FrameBufferCtrl::GuiOsd2_RemoveRef()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	BOOL_t bGlobalStatusIsLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	BOOL_t bNeedLock;
	CONST pid_t callerProcessId = getpid();
	INT_t iId;
	BOOL_t bFound;
	RMstatus eRMstatus;

	//LOG_BLINE("pid(%d),GuiOsd2_RemoveRef\n", callerProcessId);

	if(FALSE == bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Lock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	bNeedLock = FALSE;
	pStatusInfo = Mp7xxGlobalStatus_getStatusInfo(bNeedLock);
	if(NULL == pStatusInfo)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(0 == pStatusInfo->secondOsdInfo.surfaceAddr)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	GuiOsd2_removeNonexistingProcessRef();

	bFound = FALSE;
	for(iId=0; iId<(typeof(iId))ARRAY_COUNT(pStatusInfo->secondOsdInfo.usingCnt); iId++)
	{
		if(callerProcessId == pStatusInfo->secondOsdInfo.usingCnt[iId].processId)
		{
			if(0 < pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt)
			{
				pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt--;
				bFound = TRUE;
			}
			else
			{
				iOutRet = ERR_OUT_OF_RANGE;
			}
			if(0 >= pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt)
			{
				pStatusInfo->secondOsdInfo.usingCnt[iId].processId = 0;
			}
			break;
		}
	}
	if(ERR_SUCCESS != iOutRet)
	{
		goto EXIT_PROC;
	}
	if(FALSE == bFound)
	{
		iOutRet = ERR_OUT_OF_RANGE;
		goto EXIT_PROC;
	}

	//LOG_BLINE("RUAReleaseAddress(0x%08x)\n", pStatusInfo->secondOsdInfo.surfaceAddr);
	eRMstatus = RUAReleaseAddress((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceAddr);
	if(RMFAILED(eRMstatus))
	{
		PRINT_BFILE_LINENO_RMSTATUS;
	}

EXIT_PROC:
	if(bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Unlock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

VOID CGuiOsd2FrameBufferCtrl::GuiOsd2_removeNonexistingProcessRef()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	BOOL_t bGlobalStatusIsLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	BOOL_t bNeedLock;
	INT_t iId;
	BOOL_t bExist;
	RMstatus eRMstatus;

	if(FALSE == bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Lock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	bNeedLock = FALSE;
	pStatusInfo = Mp7xxGlobalStatus_getStatusInfo(bNeedLock);
	if(NULL == pStatusInfo)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(0 == pStatusInfo->secondOsdInfo.surfaceAddr)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	for(iId=0; iId<(typeof(iId))ARRAY_COUNT(pStatusInfo->secondOsdInfo.usingCnt); iId++)
	{
		bExist = CProcessList::existProcess(pStatusInfo->secondOsdInfo.usingCnt[iId].processId);
		if(FALSE == bExist)
		{
			while(0 < pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt)
			{
				eRMstatus = RUAReleaseAddress((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceAddr);
				if(RMFAILED(eRMstatus))
				{
					PRINT_BFILE_LINENO_RMSTATUS;
				}
				pStatusInfo->secondOsdInfo.usingCnt[iId].refCnt--;
			}
			pStatusInfo->secondOsdInfo.usingCnt[iId].processId = 0;
		}
	}

EXIT_PROC:
	if(bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Unlock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

INT_t CGuiOsd2FrameBufferCtrl::GuiOsd2_mmapSurface()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	BOOL_t bGlobalStatusIsLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	BOOL_t bNeedLock;
	RMstatus eRMstatus = RM_OK;

	if(FALSE == bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Lock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	if(m_bSurfaceLumaAddrIsMapped)
	{
		goto EXIT_PROC;
	}

	bNeedLock = FALSE;
	pStatusInfo = Mp7xxGlobalStatus_getStatusInfo(bNeedLock);
	if(NULL == pStatusInfo)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(0 == pStatusInfo->secondOsdInfo.surfaceAddr || 0 == pStatusInfo->secondOsdInfo.surfaceLumaAddr)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	if(FALSE == m_bHasAddRef)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	if(NULL == m_pRua)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(FALSE == m_bSurfaceLumaAddrIsLocked)
	{
		eRMstatus = RUALock((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceLumaAddr, pStatusInfo->secondOsdInfo.surfaceLumaSize);
		if(RMFAILED(eRMstatus))
		{
			iOutRet = ERR_RUA_LOCK_FAIL;
			goto EXIT_PROC;
		}
		m_bSurfaceLumaAddrIsLocked = TRUE;
	}

	if(FALSE == m_bSurfaceLumaAddrIsMapped)
	{
		m_pLumaVirtAddr = RUAMap((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceLumaAddr, pStatusInfo->secondOsdInfo.surfaceLumaSize);
		if(NULL == m_pLumaVirtAddr)
		{
			iOutRet = ERR_RUA_MAP_FAIL;
			goto EXIT_PROC;
		}
		m_bSurfaceLumaAddrIsMapped = TRUE;
	}

	m_surfaceLumaSize = pStatusInfo->secondOsdInfo.surfaceLumaSize;
	m_surfaceWidth = pStatusInfo->secondOsdInfo.surfaceWidth;
	m_surfaceHeight = pStatusInfo->secondOsdInfo.surfaceHeight;

EXIT_PROC:
	if(bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Unlock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

INT_t CGuiOsd2FrameBufferCtrl::GuiOsd2_munmapSurface()
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	BOOL_t bGlobalStatusIsLocked = FALSE;
	CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO pStatusInfo = NULL;
	RMstatus eRMstatus = RM_OK;
	BOOL_t bNeedLock;

	if(FALSE == bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Lock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = TRUE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	if(FALSE == m_bHasAddRef)
	{
		iOutRet = ERR_NOT_READY;
		goto EXIT_PROC;
	}

	if(NULL == m_pRua)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	bNeedLock = FALSE;
	pStatusInfo = Mp7xxGlobalStatus_getStatusInfo(bNeedLock);
	if(NULL == pStatusInfo)
	{
		iOutRet = ERR_INVALID_STATE;
		goto EXIT_PROC;
	}

	if(m_bSurfaceLumaAddrIsMapped)
	{
		RUAUnMap((struct RUA *)m_pRua, m_pLumaVirtAddr, pStatusInfo->secondOsdInfo.surfaceLumaSize);
		m_bSurfaceLumaAddrIsMapped = FALSE;
	}
	
	if(m_bSurfaceLumaAddrIsLocked)
	{
		eRMstatus = RUAUnLock((struct RUA *)m_pRua, pStatusInfo->secondOsdInfo.surfaceLumaAddr, pStatusInfo->secondOsdInfo.surfaceLumaSize);
		if(RMFAILED(eRMstatus))
		{
			iOutRet = ERR_RUA_UNLOCK_FAIL;
			goto EXIT_PROC;
		}
		m_bSurfaceLumaAddrIsLocked = FALSE;
		m_pLumaVirtAddr = NULL;
		m_surfaceLumaSize = 0;
		m_surfaceWidth = 0;
		m_surfaceHeight = 0;
	}

EXIT_PROC:
	if(bGlobalStatusIsLocked)
	{
		iRet = Mp7xxGlobalStatus_Unlock();
		if(ERR_SUCCESS == iRet)
		{
			bGlobalStatusIsLocked = FALSE;
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

#endif
