#include "QWidgetEx.h"
#include <ErrPrintHelper.h>

QWidgetEx::QWidgetEx()
{
}

QWidgetEx::~QWidgetEx()
{
}

VOID QWidgetEx::AllWidgets_setWindowOpacity(QWidget * pRootWidget, qreal OpaqueLevel)
{
	QObjectList ChildrenList;
	int iId;

	do
	{
		if(NULL == pRootWidget)
		{
			break;
		}
	
		//LOG_BLINE("Widget:%p,setWindowOpacity\n", pRootWidget);
		pRootWidget->setWindowOpacity(OpaqueLevel);
	
		ChildrenList = pRootWidget->children();
		for(iId = 0; iId < ChildrenList.count(); iId++)
		{
			QObject * pChild = ChildrenList.at(iId);
			//LOG_BLINE("pChild:%p\n", pChild);
			if(pChild && pChild->isWidgetType())
			{
				QWidget * pChildWidget = (typeof(pChildWidget))pChild;
				AllWidgets_setWindowOpacity(pChildWidget, OpaqueLevel);
			}
		}
	}while(FALSE);
}

