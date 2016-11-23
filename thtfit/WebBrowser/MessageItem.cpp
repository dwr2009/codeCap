#include "MessageItem.h"
#include <BaseErrDef.h>

QMessageItem::QMessageItem()
{
	m_uiMsgId = 0;
}

QMessageItem::~QMessageItem()
{
}

quint16 QMessageItem::getMsgId()
{
	return m_uiMsgId;
}

INT_t QMessageItem::setMsgId(quint16 uiMsgId)
{
	m_uiMsgId = uiMsgId;
	
	return ERROR_SUCCESS;
}

INT_t QMessageItem::getOutRet()
{
	return m_iOutRet;
}

void QMessageItem::setOutRet(INT_t iOutRet)
{
	m_iOutRet = iOutRet;
}

