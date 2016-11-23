#ifndef _Q_UPGRADE_VIEW_SERVER_H
#define _Q_UPGRADE_VIEW_SERVER_H
#include <QtWidgets/QApplication>
#include <QWidget>
#include <QObject>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>
#include <QLabel>
#include <QTimer>
#include <BaseErrDef.h>
#include "BaseTypeDef.h"
#include <QSharedPointer>
#include <QWeakPointer>


#define QDBUS_CONN_NAME_UPGRADE_VIEW_SERVER 	"UpgradeViewServer"
#define DBUS_UPGRADE_VIEW_SERVER_NAME  			"org.thtfit.UpgradeViewServer"
#define DBUS_UPGRADE_VIEW_PATH  				"/org/thtfit/UpgradeViewServer"


class UpgradeViewServer;

class QDBusIf_UpgradeViewServer : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.UpgradeViewServer")

public:
	explicit QDBusIf_UpgradeViewServer(UpgradeViewServer * pRealObj);
	

public slots:
	void setViewText(const QString & strText, qint32 iTimeoutMs ,int & iOutRet);
	void setViewFont(qint32 iPointSize,qint32 isBold,qint32 iWeight,int & iOutRet);
	void setViewColor(qint32 ired,qint32 igreed,qint32 iblue,int & iOutRet);

signals:

private:
	UpgradeViewServer * m_pRealObj;
};


class DisplayWindow : public QWidget
{	
public:
	DisplayWindow(QWidget* parent=0);
	virtual ~DisplayWindow();
	void setViewText(const QString & strText);
	void setViewFont(int iPointSize,bool isBold,int iWeight);
	void setViewColor(int ired=0,int igreed=0,int iblue=0);

private:
	QLabel 		m_upgradetext;
	QFont  		m_font;
	QPalette	m_Palette;
};


class UpgradeViewServer : public QApplication
{
	Q_OBJECT

public:

	UpgradeViewServer(int argc, char *argv[]);
	virtual ~UpgradeViewServer();
	INT_t InitInstance();
	INT_t ExitInstance();
	virtual int exec();
	static VOID SigHandler(VOID * pCbContext, int SigNo);
	int setViewText(const QString & strText, qint32 iTimeoutMs = -1);
	int setViewFont(qint32 iPointSize,qint32 isBold,qint32 iWeight);
	int setViewColor(qint32 ired,qint32 igreed,qint32 iblue);

public slots:
	void On_TimeoutHide();

private:
	QTimer m_viewtimeoutremove;
	QSharedPointer<QDBusConnection> m_Upgrade_DBusConn_sp;
	QSharedPointer<QDBusIf_UpgradeViewServer> m_DbusIf_UpgradeView_sp;
	bool m_bUpgradeDbusServIsReged;
	DisplayWindow upgradeview;
};




#endif
