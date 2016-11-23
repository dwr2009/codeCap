#include "LinuxSignalHandlerEx.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

static CLinuxSignalHandlerEx g_LinuxSignalHandler;

CLinuxSignalHandlerEx::CLinuxSignalHandlerEx()
{
}

CLinuxSignalHandlerEx::~CLinuxSignalHandlerEx()
{
}

VOID CLinuxSignalHandlerEx::SigHandler(int SigNo)
{
	//LOG_BLINE("ThrdId:%d, Sig:%d\n", pthread_self(), SigNo);
	g_LinuxSignalHandler.SigHandlerInternal(SigNo);
}

VOID CLinuxSignalHandlerEx::SigHandlerInternal(int SigNo)
{
	INT_t iRet;
	pthread_t ThreadId = pthread_self();
	
	do
	{
		SIGNAL_HANDLER_LIST SigHandlerListToTrig;
		SharedPtr <CSignalHandlerItem> NewSigHandlerItem_sp;
		{
			CAutoLock AutoRangeLocker(&m_SigDataMutex);
			SIGNAL_HANDLER_LIST::iterator itSigHandler;
			for(itSigHandler = m_lstSignalHandler.begin(); itSigHandler != m_lstSignalHandler.end(); itSigHandler++)
			{
				SharedPtr <CSignalHandlerItem> SigHandlerItem_sp = *itSigHandler;
				if(SigHandlerItem_sp->ChkMatch(ThreadId, SigNo))
				{
					NewSigHandlerItem_sp = SigHandlerItem_sp->Clone();
					SigHandlerListToTrig.push_back(NewSigHandlerItem_sp);
				}
			}
		}
		SIGNAL_HANDLER_LIST::iterator itSigHandler;
		for(itSigHandler = SigHandlerListToTrig.begin(); itSigHandler != SigHandlerListToTrig.end(); itSigHandler++)
		{
			SharedPtr <CSignalHandlerItem> SigHandlerItem_sp = *itSigHandler;
			iRet = SigHandlerItem_sp->TriggerIfMatch(ThreadId, SigNo);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);
}

INT_t CLinuxSignalHandlerEx::HookSignal(int SigNo, PFN_SignalHandleCb pfnSignalHandlerCb, 
	PVOID pCbContext/* = NULL*/, pthread_t ThreadId/* = 0*/)
{
	return (g_LinuxSignalHandler.HookSignalInternal(SigNo, pfnSignalHandlerCb, pCbContext, ThreadId));
}

INT_t CLinuxSignalHandlerEx::HookSignalInternal(int SigNo, PFN_SignalHandleCb pfnSignalHandlerCb, 
	PVOID pCbContext/* = NULL*/, pthread_t ThreadId/* = 0*/)
{
	INT_t iOutRet = ERROR_SUCCESS;

	CAutoLock AutoRangeLocker(&m_SigDataMutex);

	do
	{
		if(0 == ThreadId)
		{
			ThreadId = pthread_self();
		}
		//Check whether registered
		BOOL_t bFound = FALSE;
		REGISTERED_SIGNAL_INFO_LIST::iterator itRegisteredSigInfo;
RESCAN_REG_SIG_LIST:
		for(itRegisteredSigInfo = m_lstRegisteredSigInfo.begin(); 
			itRegisteredSigInfo != m_lstRegisteredSigInfo.end(); itRegisteredSigInfo++)
		{
			SharedPtr <CRegisteredSignalInfo> RegisteredSigInfo_sp = *itRegisteredSigInfo;
			if(RegisteredSigInfo_sp.get())
			{
				if(RegisteredSigInfo_sp->m_SigNo == SigNo)
				{
					bFound = TRUE;
					break;
				}
			}
			else
			{
				m_lstRegisteredSigInfo.erase(itRegisteredSigInfo);
				goto RESCAN_REG_SIG_LIST;
			}
		}
		if(FALSE == bFound)	//need reg
		{
			SharedPtr <CRegisteredSignalInfo> RegisteredSigInfo_sp(new CRegisteredSignalInfo(SigNo));
			if(NULL == RegisteredSigInfo_sp.get())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			RegisteredSigInfo_sp->m_pOldSigHandler = signal(SigNo, SigHandler);
			if(SIG_ERR == RegisteredSigInfo_sp->m_pOldSigHandler)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				iOutRet = ERR_REGISTER_FAIL;
				break;
			}
			m_lstRegisteredSigInfo.push_back(RegisteredSigInfo_sp);
		}
		//Check whether exists
		bFound = FALSE;
		SIGNAL_HANDLER_LIST::iterator itSigHandler;
RESCAN_SIG_HANDLER_LIST:
		for(itSigHandler = m_lstSignalHandler.begin(); 
			itSigHandler != m_lstSignalHandler.end(); itSigHandler++)
		{
			SharedPtr <CSignalHandlerItem> SigHandlerItem_sp = *itSigHandler;
			if(SigHandlerItem_sp.get())
			{
				if(SigHandlerItem_sp->ChkMatch(ThreadId, SigNo))
				{
					bFound = TRUE;
					break;
				}
			}
			else
			{
				m_lstSignalHandler.erase(itSigHandler);
				goto RESCAN_SIG_HANDLER_LIST;
			}
		}
		if(FALSE == bFound) //need add
		{
			SharedPtr <CSignalHandlerItem> SigHandlerItem_sp(
				new CSignalHandlerItem(pCbContext, ThreadId, pfnSignalHandlerCb, SigNo));
			if(NULL == SigHandlerItem_sp.get())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_lstSignalHandler.push_back(SigHandlerItem_sp);
		}
		
	}while(FALSE);

	return iOutRet;
}

