#include "SignalHandlerItem.h"
#include <BaseErrDef.h>

CSignalHandlerItem::CSignalHandlerItem()
{
	m_pCbContext = NULL;
	m_ThreadId = 0;
	m_pfnSigHandlerCb = NULL;
	m_SigNo = 0;
}

CSignalHandlerItem::CSignalHandlerItem(PVOID pCbContext, pthread_t ThreadId, PFN_SignalHandleCb pfnSigHandlerCb, int SigNo)
{
	m_pCbContext = pCbContext;
	m_ThreadId = ThreadId;
	m_pfnSigHandlerCb = pfnSigHandlerCb;
	m_SigNo = SigNo;
}

INT_t CSignalHandlerItem::TriggerIfMatch(pthread_t ThreadId, int SigNo)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(FALSE == ChkMatch(ThreadId, SigNo))
		{
			iOutRet = ERR_MISMATCH;
			break;
		}
		if(NULL == m_pfnSigHandlerCb)
		{
			break;
		}
		m_pfnSigHandlerCb(m_pCbContext, SigNo);
	}while(FALSE);

	return iOutRet;
}

BOOL_t CSignalHandlerItem::ChkMatch(pthread_t ThreadId, int SigNo)
{	
	return (m_ThreadId == ThreadId && m_SigNo == SigNo);
}

SharedPtr <CSignalHandlerItem> CSignalHandlerItem::Clone()
{
	SharedPtr <CSignalHandlerItem> NewSigHandlerItem_sp(
		new CSignalHandlerItem(m_pCbContext, m_ThreadId, m_pfnSigHandlerCb, m_SigNo));

	return NewSigHandlerItem_sp;
}

