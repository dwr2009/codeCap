#ifndef	_NET_MGR_ETHERNET_CLIENT_H_
#define	_NET_MGR_ETHERNET_CLIENT_H_

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <BaseTypeDef.h>
#include <QSharedPointer>

class QNetMgrEthernetClient : public QObject
{
	Q_OBJECT
public:
	QNetMgrEthernetClient();
	virtual ~QNetMgrEthernetClient();
	virtual INT_t setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp);
	virtual INT_t getDefaultDeviceInfo(QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask);
	virtual INT_t readSettings(QString & strDefaultDeviceName, bool & bAutoGetIP, QString & strIPv4Addr, QString & SubnetMask);
	virtual INT_t applySettingsForDefaultDevice(bool bAutoGetIP, QString & strIPv4Addr, QString & strSubnetMask);
public slots:
	void OnTimer_PrepareDBus();
private:
private:
	QWeakPointer <QDBusConnection> m_DBusConn_wp;
	QSharedPointer <QDBusInterface> m_DBusIf_NetMgrEthernet_sp;
};

#endif	//_NET_MGR_ETHERNET_CLIENT_H_

