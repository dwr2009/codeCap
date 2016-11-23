#ifndef	_NET_MGR_ETHERNET_H_
#define	_NET_MGR_ETHERNET_H_

#include <QSharedPointer>
#include <QDBusConnection>
#include <BaseTypeDef.h>
#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QProcess>

#define	DBUS_PATH_NetworkManagerSrv_Ethernet		"/org/thtfit/SystemService/NetworkManagerSrv/Ethernet"

class QNetMgrEthernet;

class QDBusIf_NetMgrEthernet : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.NetworkManagerSrv.Ethernet")
	/*
	//optional, maybe not necessary radically.
	Q_CLASSINFO("D-Bus Introspection",
		"  <interface name=\"org.thtfit.SystemService.NetworkManagerSrv.Ethernet\">\n"
		"    <method name=\"getDefaultDeviceInfo\">\n"
		"      <arg name=\"Input\" type=\"i\" direction=\"in\" />\n"
		"      <arg name=\"Output\" type=\"s\" direction=\"out\" />\n"
		"    </method>\n"
		"  </interface>\n")
	*/
public:
	QDBusIf_NetMgrEthernet(QNetMgrEthernet * pParentObj);
public slots:
	void getDefaultDeviceInfo(int & iOutRet, QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask);
	void readSettings(int & iOutRet, QString & strDefaultDeviceName, bool & bAutoGetIP, QString & strIPv4Addr, QString & SubnetMask);
	void applySettingsForDefaultDevice(bool bAutoGetIP, QString strIPv4Addr, QString strSubnetMask, int & iOutRet);
	void getDefEthDevName(int & iOutRet, QString & strEthDevName);

private:
	QNetMgrEthernet * m_pNetMgrEthernet;
};

class QNetMgrEthernet : public QObject
{
	Q_OBJECT
	friend class QDBusIf_NetMgrEthernet;
public:
	QNetMgrEthernet();
	INT_t setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp);
	virtual const QString getDefaultEthDevName();
	INT_t InitInstance();
	INT_t ExitInstance();
	void getDefaultDeviceInfo(int & iOutRet, QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask,
		BOOL_t * CONST pbUseDhcp = NULL, quint32 * pIpV4Addr = NULL, quint32 * pSubnetMaskV4Addr = NULL);
	INT_t getSavedNetworkCfgInfo(QString & strDefDevName, BOOL_t * CONST pbUseDhcp, quint32 * CONST pIpV4Addr, quint32 * CONST pSubnetMaskV4Addr);
	void readSettings(int & iOutRet, QString & strDefaultDeviceName, bool & bAutoGetIP, QString & strIPv4Addr, QString & SubnetMask);
	void applySettingsForDefaultDevice(bool bAutoGetIP, QString strIPv4Addr, QString strSubnetMask, int & iOutRet,
		BOOL_t bSaveCfg = TRUE);	
	INT_t applyCurSettings();
	BOOL_t HaveAutoIpSet();
	INT_t setIpV4Addr(const QString & strIpV4Addr);
	INT_t saveIpV4Addr(const QString & strIpV4Addr);
	INT_t setSubnetMask(const QString & strSubnetMask);
	INT_t saveSubnetMask(const QString & strSubnetMask);
	INT_t setDhcp(bool bUseDhcp, BOOL_t bApply = TRUE);
	INT_t saveDhcp(bool bUseDhcp);
	INT_t saveCurSettings();
	void onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
		const QString & strGatewayAddr,const QString & strDns);
private:
	INT_t startDhcpcProcess();
	INT_t StopDhcpcProcess();
	INT_t doNetworkConfig(bool bAutoGetIP, CONST QString & strIPv4Addr, CONST QString & strSubnetMask);
	INT_t UpdateMp7xxGlobalStatusData();
private:
	QWeakPointer <QDBusConnection> m_DBusConn_wp;
	QSharedPointer <QDBusIf_NetMgrEthernet> m_DBusIfAdaptor_NetMgrEthernet_sp;
	QString m_strDefaultDeviceName;
	BOOL_t m_bAutoGetIP;
	QString m_strIPv4Addr;
	QString m_strSubnetMask;
	QSharedPointer <QProcess> m_DhcpcProcess_sp;
};

#endif	//_NET_MGR_ETHERNET_H_

