#ifndef	_Q_WB_STATUS_BAR_H_
#define	_Q_WB_STATUS_BAR_H_

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>
#include <QWeakPointer>
#include <BaseTypeDef.h>
#include <QTimer>
#include <QKeyEvent>

class IWbStatusBar;

class QWbStatusBarItem : public QObject
{
	Q_OBJECT
	friend class QWbStatusBar;
public:
	QWbStatusBarItem(QWeakPointer <IWbStatusBar> & WbStatusBarIf_wp);
	virtual ~QWbStatusBarItem();
	virtual INT_t setText(const QString & strText, int iTimeoutMs = -1);
	virtual INT_t remove();
	virtual INT_t Hide(QWbStatusBarItem * pWbStatusBarItem);
public slots:
	void On_TimeoutHide();
private:
	quint32 m_uiItemId;
	BOOL_t m_bVisible;
	QString m_strText;
	QWeakPointer <IWbStatusBar> m_WbStatusBarIf_wp;
	QTimer m_timerTimeoutRemove;
};

typedef QList < QWeakPointer <QWbStatusBarItem> > WbStatusBarItem_WP_LIST;

class IWbStatusBar
{
public:
	virtual void On_WbStatusBarItem_Changed(QWbStatusBarItem * pWbStatusBarItem) = 0;
	virtual INT_t remove(QWbStatusBarItem * pWbStatusBarItem) = 0;
	virtual INT_t Hide(QWbStatusBarItem * pWbStatusBarItem) = 0;
};

class QWbStatusBar : public QWidget, public IWbStatusBar
{
public:
	QWbStatusBar();
	virtual ~QWbStatusBar();
	virtual void On_WbStatusBarItem_Changed(QWbStatusBarItem * pWbStatusBarItem);
	virtual INT_t remove(QWbStatusBarItem * pWbStatusBarItem);
	virtual INT_t Hide(QWbStatusBarItem * pWbStatusBarItem);
	virtual QSharedPointer <QWbStatusBarItem> RegisterStatusBarItem();
private:
	void RecalcDisplay();
public:
	QWeakPointer <QWbStatusBar> m_this_wp;
private:
	WbStatusBarItem_WP_LIST m_lstWbStatusBarItem;
	QLabel m_lblStatusText;
	quint32 m_uiLastItemId;
};


class QInputChannelWin : public QWidget
{
	Q_OBJECT
public:
	QInputChannelWin(QWidget * parentObj);
	virtual ~QInputChannelWin();
	 INT_t setText(const QString & strText, int iTimeoutMs = -1);
protected:
	virtual void keyPressEvent(QKeyEvent * event);
public slots:
	void On_TimeoutHide();
signals:
	void inputHide_timeOut(QString strChannelNumber);
private:
	QLabel m_lblStatusText;
	QString m_strText;
	QTimer m_timerTimeoutHide;
};

#endif	//_Q_WB_STATUS_BAR_H_

