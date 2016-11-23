#ifndef	_WEB_BROWSER_SRV_H_
#define	_WEB_BROWSER_SRV_H_

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QDBusConnection>
#include "DataCommService.h"

//QDbus connection name
#define	QDBUS_CONN_NAME_WebBrowser_Srv		"WebBrowserSrv"

class CWebBrowserSrv : public QThread
{
	Q_OBJECT
public:
	CWebBrowserSrv();
	virtual ~CWebBrowserSrv();
	virtual qint64 getCurFileTransferredSize();
	INT_t setTransferControlStatus(int TransferCtrlCmdCode);
	INT_t getTransferControl(int & TransferStatusCode);
public slots:
	int On_FsMountPointSymLink(const QString & strSymLinkFilePath);
	int On_FsMountPointSymUnlink(const QString & strSymUnlinkFilePath);
private:
	virtual void run();
private:
	Qt::HANDLE m_WebBrowserSrvThreadId;
	QSharedPointer <QDBusConnection> m_DBusConn_sp;	
	QSharedPointer <CDataCommService> m_DataCommSrv_sp;
	QMutex m_mutexDataCommSrvSp;
};

#endif	//_WEB_BROWSER_SRV_H_

