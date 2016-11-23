#ifndef	_Q_ERROR_INFO_LIST_H_
#define	_Q_ERROR_INFO_LIST_H_

#include <QList>
#include <BaseTypeDef.h>
#include <QSharedPointer>

class QErrorInfoItem
{
public:
	QErrorInfoItem();
	QErrorInfoItem(INT_t iOutRet, const QString & strErrDesc);
	virtual ~QErrorInfoItem();
public:
	INT_t m_iOutRet;
	QString m_strErrDesc;
};

typedef QList < QSharedPointer <QErrorInfoItem> > QErrorInfoItem_SP_LIST;

class QErrorInfoList
{
public:
	QErrorInfoList();
	virtual ~QErrorInfoList();
	virtual INT_t operator << (QSharedPointer <QErrorInfoItem> & ErrInfoItem_sp);
	virtual QSharedPointer <QErrorInfoItem> &  operator [](int iIndex);
	virtual size_t getErrInfoListSize();
private:
	QErrorInfoItem_SP_LIST m_lstErrInfoItem;
	QSharedPointer <QErrorInfoItem> m_DummyErrInfoItem;
};

#endif	//_Q_ERROR_INFO_LIST_H_

