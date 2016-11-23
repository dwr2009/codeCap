#ifndef	_MESSAGE_ITEM_H_
#define	_MESSAGE_ITEM_H_

#include <QSharedPointer>
#include <BaseTypeDef.h>

class QMessageItem
{
public:
	QMessageItem();
	virtual ~QMessageItem();
	virtual quint16 getMsgId();
	virtual INT_t setMsgId(quint16 uiMsgId);
	virtual INT_t getOutRet();
	virtual void setOutRet(INT_t iOutRet);
protected:
	quint16 m_uiMsgId;
	INT_t m_iOutRet;
};

typedef QSharedPointer <QMessageItem> MessageItem_SharedPointer_t;

#endif	//_MESSAGE_ITEM_H_

