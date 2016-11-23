#ifndef	_NET_MGR_WIFI_H_
#define	_NET_MGR_WIFI_H_

#include <QSharedPointer>
#include <QDBusConnection>
#include <BaseTypeDef.h>
#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QProcess>
#include "WpasCtrl.h"
#include <QSocketNotifier>
#include "SystemSrvApi.h"

typedef struct tagScanResult {
    // The network name.
    QString SSID;

    /// The address of the access point.
    QString BSSID;

    // Describes the authentication, key management, and encryption schemes
    // supported by the access point.
    QString capabilities;

    // The detected signal level in dBm. At least those are the units used by
    // the TI driver.
    int level;

    // The frequency in MHz of the channel over which the client is communicating
    // with the access point.
    int frequency;
} ScanResult;

typedef struct tagListNetworks {
    int netId;
    QString ssid;
    QString bssid;
    QString flags;
} ListNetworks;

typedef struct tagWifiCfg {
    int bEnable;
    int bDhcp;
    int bSaveCfg;

    QString strIPv4Addr;
    QString strSubnetMask;

    QString strGateway;
    QString strDomainName;
    QString strDns1;
    QString strDns2;

    QString strSSID;
    QString strKey;
    int iChannel;
    QString strSecurity;
    int iWepKeyIndex;
    QString strType;
} WifiCfg;

typedef enum {
    EnableDhcp, IPv4Addr, SubnetMask, Gateway, DomainName, Dns1, Dns2, SSID, Key, Channel, Security, KeyIndex, Type
} DBusWifiType;

class QNetMgrWifi;
class QDBusIf_NetMgrWifi : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.NetworkManagerSrv.Wifi")
public:
	QDBusIf_NetMgrWifi(QNetMgrWifi * pParentObj);

public slots:

private:
	QNetMgrWifi * m_pNetMgrWifi;
};

class QNetMgrWifi : public QObject
{
    Q_OBJECT
    friend class QDBusIf_NetMgrWifi;

public:

public:
    QNetMgrWifi();
    void getDefaultDeviceInfo(DBusWifiType type, QString & value, int & iOutRet);

    INT_t setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp);
    int IsEnable();

    INT_t InitInstance();
    INT_t ExitInstance();

    int OpenWpaCtrl();
    int CloseWpaCtrl();
    int ConnectToAp();
    INT_t saveCurSettings();
    INT_t getCurSettings();
    void ShowCurSettings();

    void setStatus(int bEnable, int & iOutRet);
    void getStatus(int & iOutRet, int & iStatus);
    void getSignalLevel(int & iOutRet, int &iStatus);

    int getCurDeviceName();
    int getInterfaces();
    int scanRequest();
    int getScanResults();
    int getCurrentBSSID(QString & strBSSID);

    int disconnect();
    int reconnect();

    int UpdateNextCfg(WifiCfg & cfg);

    int listConfigured();

    int addNetwork();
    int removeNetwork(int netId);
    int removeNetworkAll();
    int setNetworkVariable(int netId, const char *name, const char *value);
    int setNetworkParam(int netId, const char *name, const char *value, bool quote);

    int enableNetwork(int netId, int disableOthers);
    int disableNetwork(int netId);

    int setScanMode(int setActive);
    int ping();
    int RecvWpaCtrlEvent(char *buf, size_t buflen);
    BOOL_t HasWpaCtrlEvent();
    int reassociate();

    int getResolveConf(QString & strDomainName, QString & strDns1, QString & strDns2);
    int doNetworkConfig();
    int SendFinishedState(int status, const char *msg);

    QSharedPointer <QProcess> m_DhcpcProcess_sp;
    QSharedPointer <QProcess> m_WpaProcess_sp;
    int m_bExitThread;
    BOOL_t m_bDevActive;
    
    int startWpaSupplicant();
    int stopWpaSupplicant();
	INT_t onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
		const QString & strGatewayAddr,const QString & strDns);
public slots:
	void onChkWifiExistence();
	void onWpaCtrlEvent(int iFd);
private:
    INT_t startDhcpcProcess();
    INT_t stopDhcpcProcess();
	INT_t ChkWpaSupplicantProcess(OUT BOOL_t & bIsRunning);
	INT_t ChkDhcpcProcess(OUT BOOL_t & bIsRunning, OUT pid_t & pidDhcpc);
private:
    QWeakPointer <QDBusConnection> m_DBusConn_wp;
    QSharedPointer <QDBusIf_NetMgrWifi> m_DBusIfAdaptor_NetMgrWifi_sp;

    WpasCtrl m_wpaCtrl;
    WifiCfg m_cfgNow;

    QString m_strDefaultDeviceName;

    QList<ScanResult> m_ScanList;
    QList<ListNetworks> m_listNetworks;
    
    char m_szReply[2048];
	QTimer m_timerChkWifiExistence;
	QSharedPointer <QSocketNotifier> m_WpaCtrlEventFdNotifier_sp;
	SystemSrv::WPA_STATE m_eCurWpaState;
};

#endif	//_NET_MGR_WIFI_H_

