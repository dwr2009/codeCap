#ifndef	_QWIDGET_EX_H_
#define	_QWIDGET_EX_H_

#include <QtWidgets/QtWidgets>
#include <BaseTypeDef.h>

class QWidgetEx : public QWidget
{
public:
	QWidgetEx();
	virtual ~QWidgetEx();
	static VOID AllWidgets_setWindowOpacity(QWidget * pRootWidget, qreal OpaqueLevel);
};

#endif	//_QWIDGET_EX_H_
