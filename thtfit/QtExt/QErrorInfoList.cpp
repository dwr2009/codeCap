#include "Api/QErrorInfoList.h"
#include <BaseErrDef.h>

QErrorInfoItem::QErrorInfoItem()
{
	m_iOutRet = ERROR_SUCCESS;
}

QErrorInfoItem::QErrorInfoItem(INT_t iOutRet, const QString & strErrDesc)
{
	m_iOutRet = iOutRet;
	m_strErrDesc = strErrDesc;
}

QErrorInfoItem::~QErrorInfoItem()
{

}

//class QErrorInfoList

QErrorInfoList::QErrorInfoList() : m_DummyErrInfoItem(new QErrorInfoItem)
{
}

QErrorInfoList::~QErrorInfoList()
{
}

INT_t QErrorInfoList::operator << (QSharedPointer <QErrorInfoItem> & ErrInfoItem_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(ErrInfoItem_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_lstErrInfoItem << ErrInfoItem_sp;
	}while(FALSE);

	return iOutRet;
}

QSharedPointer <QErrorInfoItem> &  QErrorInfoList::operator [](int iIndex)
{	
	do
	{
		if(0 > iIndex || (m_lstErrInfoItem.size() - 1 < iIndex))
		{
			return m_DummyErrInfoItem;
		}
		return m_lstErrInfoItem[iIndex];
	}while(FALSE);
}

size_t QErrorInfoList::getErrInfoListSize()
{
	return m_lstErrInfoItem.size();
}

