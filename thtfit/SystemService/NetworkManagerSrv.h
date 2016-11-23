#ifndef	_NETWORK_MANAGER_SRV_H_
#define	_NETWORK_MANAGER_SRV_H_

#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QtCore/qobjectdefs.h>
#include <BaseTypeDef.h>
#include "NetMgrEthernet.h"
#include "NetMgrWifi.h"
#include "SysBaseSrvApi.h"
#include <NetRouteTable.h>
#include <NetIF.h>
#include <QSettings>
#include <QDBusVariant>
#include <QTimer>
#include <QDebug>

#define	DBUS_PATH_NetworkManagerSrv		"/org/thtfit/SystemService/NetworkManagerSrv"
#define DbusIf_SysSrv_NetMgrSrv         "org.thtfit.SystemService.NetworkManagerSrv"

class QNetworkManagerSrv;

class QDBusIf_NetworkManagerSrv : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.NetworkManagerSrv")	
public:
	QDBusIf_NetworkManagerSrv(QNetworkManagerSrv * pParentObj);
signals:
	void SntpUpdatedSysTime();
public slots:
	void getDefaultGatewayAddr(int & iOutRet, QString & strGatewayAddr);
	void getResolveConf(int & iOutRet, QString & strDomain, QString & strNameServer);
	void readSettings(int & iOutRet, QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2);
	void applySettings(QString strDefaultGateway, QString strDns1, QString strDns2, int & iOutRet);

	void setNetworkCfg(QString strParam, int & iOutRet);
	void getNetworkCfg(QString strInParam, int & iOutRet, QString & strOutParam);

	void setWifiCfg(QString strParam, int & iOutRet);
	void getWifiCfg(int type, QString & value, int & iOutRet);

	void ResetSnmpdV3Password(int & iOutRet);
	void setSnmpdV3Only(int bEnable, int & iOutRet);
	void getSnmpdV3Only(int & iOutRet, int & bEnable);
	void setPrimaryTimeServer(QString strServerName, int & iOutRet);
	void getPrimaryTimeServer(int & iOutRet, QString & strServerName);
	void setSecondaryTimeServer(QString strServerName, int & iOutRet);
	void getSecondaryTimeServer(int & iOutRet, QString & strServerName);
	void setTrapReceiverAddress(const QString & strRecvAddr,int & iOutRet);
	void getTrapReceiverAddress(int & iOutRet, QString & strTrapRecvAddr);
	void setSnmpCommunity(const QString & strRecvAddr,int & iOutRet);
	void getSnmpCommunity(int & iOutRet, QString & strTrapRecvAddr);	
	void setLoggingEnable(int iLogEnable,int & iOutRet);
	void getLoggingEnable(int & iOutRet,int & iLogEnable);
	void setWifiStatus(int bEnable, int & iOutRet);
	void getWifiStatus(int & iOutRet, int & iStatus);
	void getWifiSignalLevel(int & iOutRet, int & iStatus);
private:
	QNetworkManagerSrv * m_pNetworkManagerSrv;
};

class QNetworkCfgInfo
{
public:
	QNetworkCfgInfo();
	~QNetworkCfgInfo();
	INT_t Reset();
public:
	QString m_strDefEthDevName;
	BOOL_t m_bEthUseDhcp;
	quint32 m_EthIpV4Addr;
	quint32 m_EthSubnetMaskV4Addr;
	quint32 m_GatewayV4Addr;
	QString m_strDomainName;
	QString m_strDns1V4Addr;
	QString m_strDns2V4Addr;
};

class QNetworkManagerSrv : public QObject
{
	Q_OBJECT		
	friend class QDBusIf_NetworkManagerSrv;
public:
	QNetworkManagerSrv();
	INT_t InitInstance();
	INT_t ExitInstance();
	virtual const QString getDefaultEthDevName();
	INT_t applyCurSettings();
	INT_t saveCurSettings();
	void onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
		const QString & strGatewayAddr,const QString & strDns);
private:
	void getDefaultGatewayAddr(int & iOutRet, QString & strGatewayAddr);
	void getResolveConf(int & iOutRet, QString & strDomain, QString & strNameServer);
	INT_t getResolveConf(QString & strDomainName, QStringList & DnsList);
	INT_t getResolveConf(QString & strDomainName, QString & strDns1, QString & strDns2);
	void readSettings(int & iOutRet, QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2);
	void applySettings(QString strDefaultGateway, QString strDns1, QString strDns2, int & iOutRet);	

	void setNetworkCfg(const QString & strParam, int & iOutRet);
	void getNetworkCfg(const QString & strInParam, int & iOutRet, QString & strOutParam);

	void setWifiCfg(const QString & strParam, int & iOutRet);
	void getWifiCfg(DBusWifiType type, QString & value, int & iOutRet);

	INT_t getCurNetworkCfgInfo(OUT QNetworkCfgInfo & oNetworkCfgInfo);
	INT_t getSavedNetworkCfgInfo(OUT QNetworkCfgInfo & oNetworkCfgInfo);
	void ResetSnmpdV3Password(int & iOutRet);
	INT_t ChangeSnmpdV3PwdDynamically();
	INT_t ChangeSnmpdV3Pwd_RestartSrv();
	void setSnmpdV3Only(int bEnable, int & iOutRet);
	void getSnmpdV3Only(int & iOutRet, int & bEnable);
	void setPrimaryTimeServer(const QString & strServerName, int & iOutRet);
	void getPrimaryTimeServer(int & iOutRet, QString & strServerName);
	void setSecondaryTimeServer(const QString & strServerName, int & iOutRet);
	void getSecondaryTimeServer(int & iOutRet, QString & strServerName);
	INT_t StopSntpProcess();
	INT_t StartSntpProcess();
	INT_t RestartSntpProcess();
	INT_t setTrapReceiverAddress(const QString & strRecvAddr,int & iOutRet);
	void getTrapReceiverAddress(int & iOutRet, QString & strTrapRecvAddr);
	void setSnmpCommunity(const QString & strCommunity,int & iOutRet);
	void getSnmpCommunity(int & iOutRet, QString & strCommunity);
	void setLoggingEnable(int iLogEnable,int & iOutRet);
	INT_t UpdateMp7xxEnableLoggingGlobalStatusData(bool bEnable);
	void getLoggingEnable(int & iOutRet,int & iLogEnable);
	void setWifiStatus(int bEnable, int & iOutRet);
	void getWifiStatus(int & iOutRet, int & iStatus);
	void getWifiSignalLevel(int & iOutRet, int & iStatus);
signals:
	void SntpUpdatedSysTime();
public slots:
	int SnmpdReloadCfg();
	void RestartSntpProcessIfNeeded();	
	void SntpFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
	QSharedPointer <QNetMgrEthernet> m_NetMgrEthernet_sp;
	QSharedPointer <QNetMgrWifi> m_NetMgrWifi_sp;
	QSharedPointer <QDBusIf_NetworkManagerSrv> m_DBusIfAdaptor_NetworkManagerSrv_sp;
	QSharedPointer <CIPv4NetRouteTable> m_IPv4NetRouteTable_sp;
	QString m_strDefaultIPv4GatewayAddr;
	QString m_strDomainName, m_strDns1IPv4Addr, m_strDns2IPv4Addr;
	QTimer m_SnmpdReloadCfgTimer;
	QString m_strSntpPriTimerServer, m_strSntpSecTimerServer;
	QSharedPointer <QProcess> m_SntpProcess_sp;
	QTimer m_SntpStartDelayTimer;
	BOOL_t m_bSntpNeedRetry;
};

#endif	//_NETWORK_MANAGER_SRV_H_

