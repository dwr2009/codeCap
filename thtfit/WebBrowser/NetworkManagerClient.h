#ifndef	_NETWORK_MANAGER_CLIENT_H_
#define	_NETWORK_MANAGER_CLIENT_H_

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <BaseTypeDef.h>
#include <QSharedPointer>

// PATH name
#define	DBUS_PATH_IrInputCtrlSrv			 "/org/thtfit/SystemService/IrInputCtrl"

// Interface name
#define	IF_NAME_SysSrv_IrInputCtrlSrv			"org.thtfit.SystemService.IrInputCtrl"

// METHOD names
#define	METHOD_EnableDisableInput		"EnableDisableInput"
#define	METHOD_getInputEnabled		"getInputEnabled"

class QNetworkManagerClient : public QObject
{
	Q_OBJECT
public:
	QNetworkManagerClient();
	virtual ~QNetworkManagerClient();
	virtual INT_t setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp);
	virtual INT_t getDefaultGatewayAddr(QString & strGateway);
	virtual INT_t getResolveConf(QString & strDomain, QString & strNameServerList);
	virtual INT_t readSettings(QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2);
	virtual INT_t applySettings(CONST QString & strDefaultGateway, CONST QString & strDns1, CONST QString & strDns2);
	virtual INT_t ResetSnmpdV3Password();

	virtual INT_t getWifiCfg(int type, QString & value);
	virtual INT_t getWifiStatus(int & iCurStatus);
	virtual INT_t getNetworkCfg(const QString & strInParam, QString & strOutParam);
	virtual INT_t setWifiCfg(const QString & strParam);
	virtual INT_t setNetworkCfg(const QString & strParam);
	virtual INT_t setWifiStatus(int iCurStatus);
	virtual INT_t getWifiSignalLevel(int & iCurrSigLevel);
public slots:
	void OnTimer_PrepareDBus();
private:
private:
	QWeakPointer <QDBusConnection> m_DBusConn_wp;
	QSharedPointer <QDBusInterface> m_DBusIf_NetworkManager_sp;
};

#endif	//_NETWORK_MANAGER_CLIENT_H_

