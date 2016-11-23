#include "NetMgrWifi.h"
#include <BaseErrDef.h>
#include <QDBusError>
#include <ErrPrintHelper.h>
#include <NetIF.h>
#include <QSettings>
#include <SystemServiceAppIf.h>
#include <QtWidgets/QApplication>
#include <inetaddr.h>
#include <QDebug>
#include <SysBaseSrvApi.h>
#include <MediaPlayerAppDef.h>
#include "SystemSrvApi.h"

#include <signal.h>
//#include <LinuxSignalHandlerEx.h>

#include <QThread>
#include <QDBusInterface>

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QResolveConf.h>

 #include <net/route.h>
 #include <net/if.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define	KEY_NetMgrWifi_DefaultDeviceName	"NetMgrWifi/DefaultDeviceName"
#define	KEY_NetMgrWifi_bEnable  			"NetMgrWifi/bEnable"
#define	KEY_NetMgrWifi_AutoGetIP			"NetMgrWifi/AutoGetIP"
#define	KEY_NetMgrWifi_IPv4Addr				"NetMgrWifi/IPv4Addr"
#define	KEY_NetMgrWifi_SubnetMask			"NetMgrWifi/SubnetMask"
#define	KEY_NetMgrWifi_Gateway			    "NetMgrWifi/Gateway"

#define	KEY_NetMgrWifi_SSID			        "NetMgrWifi/SSID"
#define	KEY_NetMgrWifi_Key			        "NetMgrWifi/Key"
#define	KEY_NetMgrWifi_Security			    "NetMgrWifi/Security"
#define	KEY_NetMgrWifi_WepKeyIndex			"NetMgrWifi/WepKeyIndex"
#define	KEY_NetMgrWifi_Channel			    "NetMgrWifi/Channel"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include <iwlib.h>
#include <NetIF.h>
#include <DateTime.h>
#include "DbgLogSwitchDef.h"
#include <ProcessList.h>
#include <ThreadSleep.h>
#include <SnmpCmdConv.h>

#define PROC_NET_WIRELESS 			"/proc/net/wireless"

#define DBUS_SystemService_NetworkManagerSrv_DEST   "org.thtfit.SystemService"
#define DBUS_SystemService_NetworkManagerSrv_PATH   "/org/thtfit/SystemService/NetworkManagerSrv"
#define DBUS_SystemService_NetworkManagerSrv_IFC    "org.thtfit.SystemService.NetworkManagerSrv"

#define WIFI_MODULES_NAME "rt5572sta"
#define ERR_INVALID_CONFIG -2

enum {
    AUTH_NONE_OPEN,
    AUTH_NONE_WEP,
    AUTH_NONE_WEP_SHARED,
    AUTH_IEEE8021X,
    AUTH_WPA_PSK,
    AUTH_WPA_EAP,
    AUTH_WPA2_PSK,
    AUTH_WPA2_EAP
};

static int ifc_ctl_sock = -1;

QDBusIf_NetMgrWifi::QDBusIf_NetMgrWifi(QNetMgrWifi * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
    m_pNetMgrWifi = pParentObj;
}

#if 0
void QDBusIf_NetMgrWifi::getDefaultDeviceInfo(int & iOutRet, DBusWifiType type, QString & value)
{
    m_pNetMgrWifi->getDefaultDeviceInfo(type, value, iOutRet);
}
#endif

// class QNetMgrWifi

QNetMgrWifi::QNetMgrWifi()
{   
    m_strDefaultDeviceName = "";
    m_bExitThread = FALSE;
    m_bDevActive = FALSE;
	m_eCurWpaState = SystemSrv::WpaState_DISCONNECTED;
}

void QNetMgrWifi::getDefaultDeviceInfo(DBusWifiType type, QString & value, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;

    switch (type) {
    case EnableDhcp:
        value.sprintf("%d", m_cfgNow.bDhcp ? 1 : 0);
        break;
    case IPv4Addr:
        value = m_cfgNow.strIPv4Addr;
        break;
    case SubnetMask:
        value = m_cfgNow.strSubnetMask;
        break;
    case Gateway:
        value = m_cfgNow.strGateway;
        break;
    case DomainName:
        value = m_cfgNow.strDomainName;
        break;
    case Dns1:
        value = m_cfgNow.strDns1;
        break;
    case Dns2:
        value = m_cfgNow.strDns2;
        break;
    case SSID:
        value = m_cfgNow.strSSID;
        break;
    case Key:
        value = m_cfgNow.strKey;
        break;
    case Channel:
        value = m_cfgNow.iChannel;
        break;
    case Security:
        value = m_cfgNow.strSecurity;
        break;
    case KeyIndex:
        value = m_cfgNow.iWepKeyIndex;
        break;
    case Type:
        value = m_cfgNow.strType;
        break;
    default:
        iOutRet = -1;
        break;
    }
}

INT_t QNetMgrWifi::setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp)
{
    INT_t iOutRet = ERROR_SUCCESS;
    m_DBusConn_wp = DBusConn_sp;
    return iOutRet;
}

static int ifc_init(void)
{
    if (ifc_ctl_sock == -1) {
        ifc_ctl_sock = socket(AF_INET, SOCK_DGRAM, 0);    
        if (ifc_ctl_sock < 0) {
            LOGE("socket() failed: %s\n", strerror(errno));
        }
    }
    return ifc_ctl_sock < 0 ? -1 : 0;
}

static void ifc_close(void)
{
    if (ifc_ctl_sock != -1) {
        (void)close(ifc_ctl_sock);
        ifc_ctl_sock = -1;
    }
}

static int ifc_get_default_route(const char *ifname)
{
    char name[64];
    in_addr_t dest, gway, mask;
    int flags, refcnt, use, metric, mtu, win, irtt;
    int result;
    FILE *fp;

    fp = fopen("/proc/net/route", "r");
    if (fp == NULL)
        return 0;
    /* Skip the header line */
    if (fscanf(fp, "%*[^\n]\n") < 0) {
        fclose(fp);
        return 0;
    }
    ifc_init();
    result = 0;
    for (;;) {
        int nread = fscanf(fp, "%63s%X%X%X%d%d%d%X%d%d%d\n",
                           name, &dest, &gway, &flags, &refcnt, &use, &metric, &mask,
                           &mtu, &win, &irtt);
        if (nread != 11) {
            break;
        }
        if ((flags & (RTF_UP|RTF_GATEWAY)) == (RTF_UP|RTF_GATEWAY)
                && dest == 0
                && strcmp(ifname, name) == 0) {
            result = htonl(gway);
            break;
        }
    }
    fclose(fp);
    ifc_close();
    return result;
}

static const char *ipaddr_to_string(uint32_t addr)
{
    struct in_addr in_addr;

    in_addr.s_addr = addr;
    return inet_ntoa(in_addr);
}

int QNetMgrWifi::getCurDeviceName()
{
    char buff[1024];
    char name[32];
    FILE* fh;
    int ret = -1;

    fh = fopen(PROC_NET_WIRELESS, "r");

    if (fh != NULL) {
        // Eat 2 lines of header
        fgets(buff, sizeof(buff), fh);
        fgets(buff, sizeof(buff), fh);

        // Read each device line
        while (fgets(buff, sizeof(buff), fh)) {
			buff[sizeof(buff)-1] = '\0';
            const char *p, *end;
            if ((buff[0] == '\0') || (buff[1] == '\0'))
                continue;

            p = buff;
            while (isspace(*p))
                p++;

            end = strstr(p, ": ");
            if (end) {
                memcpy(name, p, (end - p));
                name[end - p] = '\0';
                m_strDefaultDeviceName = name;
                ret = 0;
                break;
            }
        }
        fclose(fh);
    }

    return ret;
}

INT_t QNetMgrWifi::InitInstance()
{
    INT_t iOutRet = ERROR_SUCCESS, iRet = 0;
    QSharedPointer <QDBusConnection> DBusConn_sp;
    bool bRet;

    do {
        // load driver
        QString strCmd;
        strCmd.sprintf("modprobe %s", WIFI_MODULES_NAME);
		iRet = system(strCmd.toLatin1());
		if(iRet != 0) {
            LOGE("Failed to load wifi module(%s), ret=%d\n", WIFI_MODULES_NAME, iRet);
        }

        // Read settings
        if (getCurSettings() != 0) {
            LOGE("Failed to get current wifi settings.\n");
            return -1;
        }

        // Wifi DBus
        DBusConn_sp = m_DBusConn_wp.toStrongRef();
        if (DBusConn_sp.isNull()) {
            PRINT_BFILE_LINENO_IRET_STR;
            iOutRet = ERROR_INVALID_STATE;
            break;
        }
        m_DBusIfAdaptor_NetMgrWifi_sp = QSharedPointer <QDBusIf_NetMgrWifi> (new QDBusIf_NetMgrWifi(this));
        if (m_DBusIfAdaptor_NetMgrWifi_sp.isNull()) {
            iOutRet = ERROR_OUT_OF_MEMORY;
            break;
        }
        bRet = DBusConn_sp->registerObject(DBUS_PATH_NetworkManagerSrv_Wifi, this);
        if (false == bRet) {
            iOutRet = ERR_REGISTER_FAIL;
            QDBusError QDBusErr = DBusConn_sp->lastError();
            if (QDBusErr.isValid()) {
                LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
            } else {
                LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_PATH_NetworkManagerSrv_Wifi);
            }
            break;
        }

		m_timerChkWifiExistence.setSingleShot(FALSE);
		m_timerChkWifiExistence.setInterval(2*1000/*ms*/);
		bRet = connect(&m_timerChkWifiExistence, SIGNAL(timeout()), this, SLOT(onChkWifiExistence()));
		if(bRet)
		{
			m_timerChkWifiExistence.start();
		}
		else
		{
			LOG_BLINE("ChkWiFiExist.ConnSig failed\n");
		}

		onChkWifiExistence();
    } while (FALSE);

    return iOutRet;
}

int QNetMgrWifi::IsEnable()
{
    return m_cfgNow.bEnable;
}

INT_t QNetMgrWifi::ExitInstance()
{
    INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
    QSharedPointer <QDBusConnection> DBusConn_sp;

    do {
        DBusConn_sp = m_DBusConn_wp.toStrongRef();
        if (FALSE == DBusConn_sp.isNull()) {
	        if (DBusConn_sp->isConnected()) {
	            DBusConn_sp->unregisterObject(DBUS_PATH_NetworkManagerSrv_Wifi);            
	        }
        }
		else
		{
			PRINT_BFILE_LINENO_BUG_STR;
		}		

		if(FALSE == m_WpaCtrlEventFdNotifier_sp.isNull())
		{
			m_WpaCtrlEventFdNotifier_sp->setEnabled(FALSE);
			bRet = QObject::disconnect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
			if(FALSE == bRet)
			{
				LOG_BLINE("WpaCtrlEvtFdNotifier.disconnect failed\n");
			}
		}
		
        // stop the dhcp prpocess
        iRet = stopDhcpcProcess();
        if (ERROR_SUCCESS != iRet) {
            PRINT_BFILE_LINENO_IRET_STR;
        }

        // stop the wpa_supplicant process
        if ((iRet = stopWpaSupplicant()) != 0) {
            LOGE("stopping wpa_supplicant process failed!\n");
            PRINT_BFILE_LINENO_IRET_STR;
        }
    } while (FALSE);

    return iOutRet;
}

int QNetMgrWifi::startWpaSupplicant()
{
    if (m_strDefaultDeviceName.isEmpty()) {
		return ERR_NO_DEV;
    }

    if (m_WpaProcess_sp.isNull()) {
        m_WpaProcess_sp = QSharedPointer <QProcess> (new QProcess(this));
        if (m_WpaProcess_sp.isNull()) {
            LOGE("error: init WpaProcess failed!\n");
            return -1;
        }
    }

	if(QProcess::NotRunning == m_WpaProcess_sp->state())
	{
	    QStringList CmdLineParams;
	    // qDebug() << "start...";
	    CmdLineParams << "-Dwext" << "-c/etc/wpa_supplicant.conf" << "-i" + m_strDefaultDeviceName;
	    // qDebug() << CmdLineParams;
	    m_WpaProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
	    m_WpaProcess_sp->start(PROG_NAME_WpaSupplicant, CmdLineParams, QIODevice::NotOpen);
	    // qDebug() << "end...";
	}
    
    return 0;
}

int QNetMgrWifi::stopWpaSupplicant()
{
    int ret = 0, TimeoutMs = (5 * 1000);

    if (!m_WpaProcess_sp.isNull()) {
        if (m_WpaProcess_sp->state() != QProcess::NotRunning) {
            m_WpaProcess_sp->terminate();
            if (!m_WpaProcess_sp->waitForFinished(TimeoutMs)) {
                LOGE("WpaProcess temination timeout\n");
                ret = -1;
            }
			m_eCurWpaState = SystemSrv::WpaState_DISCONNECTED;
        }
    }

    return ret;
}

INT_t QNetMgrWifi::onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
	const QString & strGatewayAddr,const QString & strDns)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
    UINT32_t uiCurIPv4Addr, uiCurSubnetMask, uiCurGateway;
	Q_UNUSED(strNetDevName);
	Q_UNUSED(strIPv4Addr);
	Q_UNUSED(strSubnetMask);
	Q_UNUSED(strGatewayAddr);
	Q_UNUSED(strDns);

    if (strNetDevName != m_strDefaultDeviceName) {
        return iOutRet;
    }

    if (Sw_LogWifiConnection) {
        LOG_BLINE("NetDev=%s,Ip=%s,SubnetMask=%s,Gateway=%s,Dns=\"%s\"\n", qPrintable(strNetDevName), qPrintable(strIPv4Addr), 
                  qPrintable(strSubnetMask), qPrintable(strGatewayAddr), qPrintable(strDns));
    }

	do
	{
		// ipaddr
		iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
		if (iRet != ERROR_SUCCESS) {
			LOG_BLINE("Failed GetIfAddrIPV4, iRet=%d\n", iRet);
			iOutRet = -1;
            break;
		}

		// subnetmask
		iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiCurSubnetMask);
		if (iRet != ERROR_SUCCESS) {
			LOG_BLINE("Failed getNetIfSubnetMask, iRet=%d\n", iRet);
			iOutRet = -1;
            break;
		}

		// gateway
		uiCurGateway = ifc_get_default_route(m_strDefaultDeviceName.toLatin1().constData());
		if (uiCurGateway == 0) {
			// LOG_BLINE("Failed ifc_get_default_route\n");
			// return -1;
		}
		m_cfgNow.strIPv4Addr	= ipaddr_to_string(htonl(uiCurIPv4Addr));
		m_cfgNow.strSubnetMask	= ipaddr_to_string(htonl(uiCurSubnetMask));
		m_cfgNow.strGateway 	= ipaddr_to_string(htonl(uiCurGateway));

		// dns
		getResolveConf(m_cfgNow.strDomainName, m_cfgNow.strDns1, m_cfgNow.strDns2);
	}while(FALSE);

    SendFinishedState(iOutRet, "WifiDhcpFinished");

	return iOutRet;
}

void QNetMgrWifi::onChkWifiExistence()
{
	int iRet;
	bool bRet, bDevActive;

	do
	{
        bDevActive = (getCurDeviceName() == 0) ? 1: 0;
        if (bDevActive != m_bDevActive) {

            m_bDevActive = bDevActive;

            if (!bDevActive) {
				if(Sw_LogWifiConnection)
				{
                	LOGD("Wifi-Removed!\n");
				}
                m_strDefaultDeviceName = "";

                // cancel fd event listening
                if (FALSE == m_WpaCtrlEventFdNotifier_sp.isNull()) {
                    m_WpaCtrlEventFdNotifier_sp->setEnabled(FALSE);
                    bRet = QObject::disconnect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
                    if (FALSE == bRet) {
                        LOG_BLINE("WpaCtrlEvtFdNotifier.disconnect failed\n");
                    }
                }

                iRet = CloseWpaCtrl();
                if (ERROR_SUCCESS != iRet) {
                    PRINT_BFILE_LINENO_IRET_STR;
                }

                // stop the dhcp prpocess
                iRet = stopDhcpcProcess();
                if (ERROR_SUCCESS != iRet) {
                    PRINT_BFILE_LINENO_IRET_STR;
                }

                // stop the wpa_supplicant process
                if ((iRet = stopWpaSupplicant()) != 0) {
                    LOGE("stopping wpa_supplicant process failed!\n");
                    PRINT_BFILE_LINENO_IRET_STR;
                }

             } else {

				if(Sw_LogWifiConnection)
				{
                	LOGD("Wifi-Active!\n");
				}

                if(FALSE == IsEnable())
                {
                    break;
                }

                BOOL_t bWpaSupplicantIsRunning = FALSE;
                iRet = ChkWpaSupplicantProcess(OUT bWpaSupplicantIsRunning);
                if(ERROR_SUCCESS != iRet)
                {
                    PRINT_BFILE_LINENO_IRET_STR;
                    break;
                }

                if(FALSE == bWpaSupplicantIsRunning)
                {
                    //up the network interface
					DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
					iRet = strShellCmd.Format("ifconfig %s up", qPrintable(m_strDefaultDeviceName));
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
						break;
					}
					iRet = system(strShellCmd);
					if(ERROR_SUCCESS != iRet)
					{
						LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
						iRet = SetIfStatus(m_strDefaultDeviceName.toLocal8Bit(), STATUS_NETIF_UP);
						if(ERROR_SUCCESS != iRet)
						{
							PRINT_BFILE_LINENO_IRET_STR;
						}
					}
					//run wpa_supplicant
                    iRet = startWpaSupplicant();
                    if(0 != iRet)
                    {
                        PRINT_BFILE_LINENO_IRET_STR;
                        break;
                    }
                    int iLeftTimeoutMs = (3*1000);
                    UINT64_t StartTimeMs = GetSysUpTimeMs();
                    bRet = m_WpaProcess_sp->waitForStarted(iLeftTimeoutMs);
                    if(false == bRet)
                    {
                        LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
                        break;
                    }
                    iLeftTimeoutMs = iLeftTimeoutMs - (GetSysUpTimeMs() - StartTimeMs);
                    if(0 >= iLeftTimeoutMs)
                    {
                        LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
                        break;
                    }
                    StartTimeMs = GetSysUpTimeMs();
                    BOOL_t bFoundWpaIfDir = FALSE;
                    while(TRUE)
                    {
                        iRet = access(WpaSupplicant_INTERFACE_DIR_PATH, F_OK);
                        if(0 == iRet)
                        {
                            bFoundWpaIfDir = TRUE;
                            break;
                        }
                        iLeftTimeoutMs = iLeftTimeoutMs - (GetSysUpTimeMs() - StartTimeMs);
                        if(0 >= iLeftTimeoutMs)
                        {
                            LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
                            break;
                        }
                        StartTimeMs = GetSysUpTimeMs();
                        iRet = usleep(10*1000);
                        if(0 != iRet && EINTR != errno)
                        {
                            PRINT_BFILE_LINENO_CRT_ERRINFO;
                        }
                    }
                    if(FALSE == bFoundWpaIfDir)
                    {
                        break;
                    }
                }

                iRet = OpenWpaCtrl();
                if(0 != iRet)
                {
                    break;
                }

                int ret = ConnectToAp();
                if (ret != 0) {
                    SendFinishedState(ret, "WifiConnectToApERR");
                }

                int iWpaCtrlFd = m_wpaCtrl.getEventNotifyFd();
                if(0 <= iWpaCtrlFd)
                {
                    m_WpaCtrlEventFdNotifier_sp = QSharedPointer <QSocketNotifier> (new QSocketNotifier(iWpaCtrlFd, QSocketNotifier::Read, this));
                    if(FALSE == m_WpaCtrlEventFdNotifier_sp.isNull())
                    {
                        bRet = connect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
                        if(false == bRet)
                        {
                            LOG_BLINE("ConnWpaCtrlEventNotifier failed\n");
                        }
                    }
                    else
                    {
                        LOG_BLINE("out of memory\n");
                    }
                }
                else
                {
                    LOG_BLINE("Invalid wpa ctrl fd(%d)\n", iWpaCtrlFd);
                }
            }
        }
    } while(FALSE);
}

void QNetMgrWifi::onWpaCtrlEvent(int iFd)
{
	int iRet;
	bool bRet;
    char szEvent[256];

	Q_UNUSED(iFd);

	do
	{
		if(FALSE == HasWpaCtrlEvent())
		{
			break;
		}

		iRet = RecvWpaCtrlEvent(szEvent, sizeof(szEvent));
		if(0 > iRet)
		{
			break;
		}
		
		if(FALSE == IsEnable())
		{
			break;
		}
		
		if (strncmp(szEvent, WPA_EVENT_TERMINATING, strlen(WPA_EVENT_TERMINATING)) == 0) {
			if (m_bDevActive || (getCurDeviceName() != 0)) {
				// terminating
				CloseWpaCtrl();
				
				if(FALSE == m_WpaCtrlEventFdNotifier_sp.isNull())
				{
					m_WpaCtrlEventFdNotifier_sp->setEnabled(FALSE);
					bRet = QObject::disconnect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
					if(FALSE == bRet)
					{
						LOG_BLINE("WpaCtrlEvtFdNotifier.disconnect failed\n");
					}
				}
				
				// stop the dhcp prpocess
				iRet = stopDhcpcProcess();
				if (ERROR_SUCCESS != iRet) {
					PRINT_BFILE_LINENO_IRET_STR;
				}
				
				// stop the wpa_supplicant process
				if ((iRet = stopWpaSupplicant()) != 0) {
					LOGE("stopping wpa_supplicant process failed!\n");
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		} else if (strncmp(szEvent, WPA_EVENT_CONNECTED, strlen(WPA_EVENT_CONNECTED)) == 0) {
			SystemSrv::WPA_STATE eOldWpaState = m_eCurWpaState;
			m_eCurWpaState = SystemSrv::WpaState_CONNECTED;
			if(m_eCurWpaState != eOldWpaState)
			{
				iRet = doNetworkConfig();
                if (!m_cfgNow.bDhcp) {
                    SendFinishedState(iRet, "WifiManuallyFinished");
                }
			}

		} else if (strncmp(szEvent, WPA_EVENT_DISCONNECTED, strlen(WPA_EVENT_DISCONNECTED)) == 0) {
			m_eCurWpaState = SystemSrv::WpaState_DISCONNECTED;
		}
	}while(FALSE);
}

int QNetMgrWifi::OpenWpaCtrl()
{
    const char* ifc = m_strDefaultDeviceName.toLatin1().constData();
    if (m_wpaCtrl.openCtrlConnection(ifc) < 0) {
        LOGE("Failed to open control connection to wpa_supplicant: '%s'.\n",
             m_strDefaultDeviceName.toLatin1().constData());
        return -1;
    }
    return 0;
}

int QNetMgrWifi::CloseWpaCtrl()
{
    m_wpaCtrl.closeCtrlConnection();
    return 0;
}

int QNetMgrWifi::ConnectToAp()
{
    int netId = 0;

    if (getScanResults() != 0) {
        LOG_BLINE("Failed to getScanResults!\n");
        return -1;
    }

    if (listConfigured() != 0) {
        LOG_BLINE("Failed to listConfigured!\n");
        return -1;
    }

    removeNetworkAll();
    netId = addNetwork();
    if (netId < 0) {
        LOG_BLINE("Failed to add a network!\n");
        return -1;
    }

    if (m_cfgNow.strSSID.isEmpty()) {
		if(Sw_LogWifiConnection)
		{
        	LOG_BLINE("SSID is empty!\n");
		}
		return ERR_INVALID_CONFIG;
    }
    setNetworkParam(netId, "ssid", m_cfgNow.strSSID.toLatin1().constData(), true);

    // Security type
    int auth;
    if (m_cfgNow.strSecurity == "0") {
        // none (0)
        auth = AUTH_NONE_OPEN;
    } else if (m_cfgNow.strSecurity == "1") {
        // wep (1)
        auth = AUTH_NONE_WEP_SHARED;
    } else if (m_cfgNow.strSecurity == "2") {
        // wpa (2)
        auth = AUTH_WPA_PSK;
    } else if (m_cfgNow.strSecurity == "3") {
        // wpa2 (3)
        auth = AUTH_WPA2_PSK;
    } else {
        LOG_BLINE("wifi: Does not support encryption mode!\n");
        return ERR_INVALID_CONFIG;
    }
    if (auth != AUTH_NONE_OPEN) {
        // encrypted AP
        if (m_cfgNow.strKey.isEmpty()) {
            LOG_BLINE("Key is empty!\n");
            return ERR_INVALID_CONFIG;
        }
    }

    if (auth == AUTH_WPA_PSK || auth == AUTH_WPA2_PSK) {
        // wpa
        if (m_cfgNow.strKey.length() < 8 || m_cfgNow.strKey.length() > 64) {
            LOG_BLINE("WPA-PSK requires a passphrase of 8 to 63 characters or 64 hex digit PSK\n");
            return ERR_INVALID_CONFIG;
        }
    } else if (auth == AUTH_NONE_WEP_SHARED) {
        // wep
        int len = m_cfgNow.strKey.length();
        bool bHex = false, bValid = false;

        if (len == 5 || len == 13 || len == 16)
            bValid = true;
        if (len == 10 || len == 26 || len == 32) {
            bValid = true;
            bHex = true;
        }
        if (!bValid) {
            LOG_BLINE("invalid Key: '%s'\n", m_cfgNow.strKey.toLatin1().constData());
            return ERR_INVALID_CONFIG;
        }

        QString keyIndex;
        keyIndex.sprintf("wep_key%d", m_cfgNow.iWepKeyIndex);
        setNetworkParam(0, keyIndex.toLatin1().constData(), m_cfgNow.strKey.toLatin1().constData(), !bHex);

        keyIndex.sprintf("%d", m_cfgNow.iWepKeyIndex);
        setNetworkParam(0, "wep_tx_keyidx", keyIndex.toLatin1().constData(), false);
    }

    const char *key_mgmt = NULL, *proto = NULL;
    switch (auth) {
    case AUTH_NONE_OPEN:
    case AUTH_NONE_WEP:
    case AUTH_NONE_WEP_SHARED:
        key_mgmt = "NONE";
        break;
    case AUTH_IEEE8021X:
        key_mgmt = "IEEE8021X";
        break;
    case AUTH_WPA_PSK:
        key_mgmt = "WPA-PSK";
        proto = "WPA";
        break;
    case AUTH_WPA_EAP:
        key_mgmt = "WPA-EAP";
        proto = "WPA";
        break;
    case AUTH_WPA2_PSK:
        key_mgmt = "WPA-PSK";
        proto = "WPA2";
        break;
    case AUTH_WPA2_EAP:
        key_mgmt = "WPA-EAP";
        proto = "WPA2";
        break;
    }
    if (proto)
        setNetworkParam(netId, "proto", proto, false);
    if (key_mgmt)
        setNetworkParam(netId, "key_mgmt", key_mgmt, false);

    if (auth == AUTH_NONE_WEP_SHARED)
        setNetworkParam(netId, "auth_alg", "SHARED", false);
    else
        setNetworkParam(netId, "auth_alg", "OPEN", false);

    if (auth == AUTH_WPA_PSK || auth == AUTH_WPA_EAP ||
        auth == AUTH_WPA2_PSK || auth == AUTH_WPA2_EAP) {
        //setNetworkParam(netId, "pairwise", "CCMP TKIP", false);
        //setNetworkParam(netId, "group", "TKIP CCMP WEP104 WEP40", false);
        setNetworkParam(netId, "psk", m_cfgNow.strKey.toLatin1().constData(), true);
    }

    enableNetwork(netId, TRUE);
    //reconnect();
    reassociate();
    return 0;
}

void QNetMgrWifi::ShowCurSettings()
{
    qDebug() << "m_strDefaultDeviceName:" << m_strDefaultDeviceName;
    qDebug() << "m_cfgNow.bEnable:" << m_cfgNow.bEnable;

    qDebug() << "m_cfgNow.bDhcp:" << m_cfgNow.bDhcp;
    qDebug() << "m_cfgNow.strIPv4Addr:" << m_cfgNow.strIPv4Addr;
    qDebug() << "m_cfgNow.strSubnetMask:" << m_cfgNow.strSubnetMask;
    qDebug() << "m_cfgNow.strGateway:" << m_cfgNow.strGateway;

    qDebug() << "m_cfgNow.strSSID: " << m_cfgNow.strSSID;
    qDebug() << "m_cfgNow.strKey:" << m_cfgNow.strKey;
    qDebug() << "m_cfgNow.strSecurity:" << m_cfgNow.strSecurity;
    qDebug() << "m_cfgNow.iWepKeyIndex:" << m_cfgNow.iWepKeyIndex;
    qDebug() << "m_cfgNow.iChannel:" << m_cfgNow.iChannel;

    qDebug() << "";
}

INT_t QNetMgrWifi::saveCurSettings()
{
    INT_t iOutRet = ERROR_SUCCESS;
    CSystemServiceAppIf * pSystemServiceAppIf = NULL;
    QSharedPointer <QSettings> SysSrvSettingsObj_sp;

    pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
    if (NULL == pSystemServiceAppIf) {
        iOutRet = ERROR_INVALID_STATE;
        return iOutRet;
    }
    SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
    if (SysSrvSettingsObj_sp.isNull()) {
        iOutRet = ERROR_INVALID_STATE;
        return iOutRet;
    }

    // SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_DefaultDeviceName, m_strDefaultDeviceName);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_bEnable, m_cfgNow.bEnable);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_AutoGetIP, m_cfgNow.bDhcp);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_IPv4Addr, m_cfgNow.strIPv4Addr);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_SubnetMask, m_cfgNow.strSubnetMask);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_Gateway, m_cfgNow.strGateway);

    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_SSID, m_cfgNow.strSSID);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_Key, m_cfgNow.strKey);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_Security, m_cfgNow.strSecurity);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_WepKeyIndex, m_cfgNow.iWepKeyIndex);
    SysSrvSettingsObj_sp->setValue(KEY_NetMgrWifi_Channel, m_cfgNow.iChannel);

    // qDebug() << "---QNetMgrWifi::saveCurSettings---";
    // ShowCurSettings();
    SysSrvSettingsObj_sp->sync();
    return iOutRet;
}

INT_t QNetMgrWifi::getCurSettings()
{
    INT_t iOutRet = ERROR_SUCCESS;
    CSystemServiceAppIf * pSystemServiceAppIf = NULL;
    QSharedPointer <QSettings> SysSrvSettingsObj_sp;

    pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
    if (NULL == pSystemServiceAppIf) {
        iOutRet = ERROR_INVALID_STATE;
        return iOutRet;
    }
    SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
    if (SysSrvSettingsObj_sp.isNull()) {
        iOutRet = ERROR_INVALID_STATE;
        return iOutRet;
    }

    m_cfgNow.bEnable        = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_bEnable, true).toBool();
    m_cfgNow.bDhcp          = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_AutoGetIP, true).toBool();

    m_cfgNow.strIPv4Addr    = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_IPv4Addr).toString();
    m_cfgNow.strSubnetMask  = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_SubnetMask).toString();
    m_cfgNow.strGateway     = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_Gateway).toString();

    m_cfgNow.strSSID        = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_SSID).toString();
    m_cfgNow.strKey         = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_Key).toString();
    m_cfgNow.strSecurity    = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_Security).toString();
    m_cfgNow.iWepKeyIndex   = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_WepKeyIndex, 0).toInt();
    m_cfgNow.iChannel       = SysSrvSettingsObj_sp->value(KEY_NetMgrWifi_Channel, 0).toInt();

    // qDebug() << "---QNetMgrWifi::getCurSettings---";
    // ShowCurSettings();
    SysSrvSettingsObj_sp->sync();
    return iOutRet;
}

INT_t QNetMgrWifi::startDhcpcProcess()
{
    INT_t iOutRet = ERROR_SUCCESS;

    QString strNetIfDevName = m_strDefaultDeviceName;
    if (strNetIfDevName.isEmpty()) {
        return -1;
    }

    if (m_DhcpcProcess_sp.isNull()) {
        m_DhcpcProcess_sp = QSharedPointer <QProcess> (new QProcess(this));
        if (m_DhcpcProcess_sp.isNull()) {
            iOutRet = ERROR_OUT_OF_MEMORY;
            return iOutRet;
        }
    }

    QStringList CmdLineParams;
    CmdLineParams << ((QString)"--interface=" + strNetIfDevName) << "--foreground";
    m_DhcpcProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
    m_DhcpcProcess_sp->start("udhcpc", CmdLineParams, QIODevice::NotOpen);

    return iOutRet;
}

INT_t QNetMgrWifi::stopDhcpcProcess()
{
    INT_t iOutRet = ERROR_SUCCESS;
    int TimeoutMs = (5 * 1000);

    if (!m_DhcpcProcess_sp.isNull()) {
        if (m_DhcpcProcess_sp->state() != QProcess::NotRunning) {
            m_DhcpcProcess_sp->terminate();
            if (m_DhcpcProcess_sp->waitForFinished(TimeoutMs) == false) {
                LOG_BLINE("DhcpcProcess temination timeout\n");
                iOutRet = ERR_TIMEOUT;
            }
        }
    }

    return iOutRet;
}

INT_t QNetMgrWifi::ChkWpaSupplicantProcess(OUT BOOL_t & bIsRunning)
{
    INT_t iOutRet = ERROR_SUCCESS, iRet, iId;
	QSharedPointer <CProcessList> m_ProcessList_sp(new CProcessList);
	POSITION pPos;
	P_PROCESS_ITEM_INFO pProcessItemInfo;

	bIsRunning = FALSE;

	do
	{
		if(m_ProcessList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_ProcessList_sp->ScanSysProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		CONST PROCESS_ITEM_INFO_LIST & ProcessInfoList = m_ProcessList_sp->getProcessList();
		pPos = ProcessInfoList.GetHeadPosition();
		while(pPos)
		{
			pProcessItemInfo = ProcessInfoList.GetNext(OUT pPos);
			do
			{
				if(NULL == pProcessItemInfo)
				{
					break;				
				}
				QString strCmdLine = (LPCSTR)(pProcessItemInfo->strCmdLine);
				QStringList CmdFieldList = strCmdLine.split(' ');
				if(2 > CmdFieldList.size())
				{
					break;
				}
				if(PROG_NAME_WpaSupplicant != CmdFieldList[0])
				{
					break;
				}
				for(iId = 0; iId < CmdFieldList.size(); iId++)
				{
					if(((QString)"-i")+m_strDefaultDeviceName == CmdFieldList[iId])
					{
						bIsRunning = TRUE;
						break;
					}
				}
				if(FALSE == bIsRunning)
				{
					break;
				}
				bIsRunning = TRUE;
				break;
			}while(FALSE);
			if(bIsRunning)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrWifi::ChkDhcpcProcess(OUT BOOL_t & bIsRunning, OUT pid_t & pidDhcpc)
{
    INT_t iOutRet = ERROR_SUCCESS, iRet, iId;
	QSharedPointer <CProcessList> m_ProcessList_sp(new CProcessList);
	POSITION pPos;
	P_PROCESS_ITEM_INFO pProcessItemInfo;

	bIsRunning = FALSE;

	do
	{
		if(m_ProcessList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_ProcessList_sp->ScanSysProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		CONST PROCESS_ITEM_INFO_LIST & ProcessInfoList = m_ProcessList_sp->getProcessList();
		pPos = ProcessInfoList.GetHeadPosition();
		while(pPos)
		{
			pProcessItemInfo = ProcessInfoList.GetNext(OUT pPos);
			do
			{
				if(NULL == pProcessItemInfo)
				{
					break;				
				}
				QString strCmdLine = (LPCSTR)(pProcessItemInfo->strCmdLine);
				QStringList CmdFieldList = strCmdLine.split(' ');
				if(2 > CmdFieldList.size())
				{
					break;
				}
				if(PROG_NAME_dhcpc != CmdFieldList[0])
				{
					break;
				}
				for(iId = 0; iId < CmdFieldList.size(); iId++)
				{
					if(((QString)"--interface=")+m_strDefaultDeviceName == CmdFieldList[iId])
					{
						bIsRunning = TRUE;
						pidDhcpc = pProcessItemInfo->uiProcessId;
						break;
					}
				}
				if(FALSE == bIsRunning)
				{
					break;
				}
				bIsRunning = TRUE;
				break;
			}while(FALSE);
			if(bIsRunning)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

int QNetMgrWifi::getResolveConf(QString & strDomainName, QString & strDns1, QString & strDns2)
{
    INT_t iOutRet = ERROR_SUCCESS;
    bool bRet;
    QSharedPointer <QFile> ResolveFile_sp(new QFile(DNS_RESOLV_CONF_FILE_PATH));
    QSharedPointer <QTextStream> ResolveStream_sp;

    QStringList DnsList;

    do {
        if (ResolveFile_sp.isNull()) {
            iOutRet = ERROR_OUT_OF_MEMORY;
            break;
        }

        bRet = ResolveFile_sp->open(QIODevice::ReadOnly);
        if (false == bRet) {
            iOutRet = ERROR_FILE_OPEN_FAIL;
            break;
        }

        ResolveStream_sp = QSharedPointer <QTextStream> (new QTextStream(&(*ResolveFile_sp)));
        if (ResolveStream_sp.isNull()) {
            iOutRet = ERROR_OUT_OF_MEMORY;
            break;
        }

        QString OneLineContent;
        while (TRUE) {
            OneLineContent = ResolveStream_sp->readLine();
            if (OneLineContent.isNull())
                break;

            QStringList WordList = OneLineContent.split(' ');
            if (1 <= WordList.count()) {
                if ("domain" == WordList[0]) {
                    if (2 <= WordList.count()) {
                        strDomainName = WordList[1];
                    }
                } else if ("nameserver" == WordList[0]) {
                    if (2 <= WordList.count()) {
                        DnsList << WordList[1];
                    }
                }
            }
        }
    } while(FALSE);

    ResolveStream_sp.clear();
    if (FALSE == ResolveFile_sp.isNull()) {
        ResolveFile_sp->close();
    }
    ResolveFile_sp.clear();

    strDns1 = "";
    strDns2 = "";
    if (DnsList.size() >= 1)
        strDns1 = DnsList[0];
    if (DnsList.size() >= 2)
        strDns2 = DnsList[1];

    return iOutRet;
}

int QNetMgrWifi::SendFinishedState(int status, const char *err)
{
	int iOutRet = ERROR_SUCCESS;
    QDBusMessage replay;
    QSharedPointer <QDBusConnection> DBusConn_sp;

    DBusConn_sp = m_DBusConn_wp.toStrongRef();
    if (DBusConn_sp.isNull()) {
        iOutRet = ERROR_INVALID_STATE;
        return iOutRet;
    }

    replay = QDBusMessage::createSignal(DBUS_SystemService_NetworkManagerSrv_PATH, 
                                        DBUS_SystemService_NetworkManagerSrv_IFC,
                                        "WifiStatus");
    replay << status << err;
    if (!DBusConn_sp->send(replay))
        iOutRet = ERR_DBUS_CALL_ERR;

	return iOutRet;
}

int QNetMgrWifi::doNetworkConfig()
{
    INT_t  iRet;
    UINT32_t uiCurIPv4Addr, uiCurSubnetMask, uiCurGateway;
    UINT32_t uiIPv4AddrToSet, uiSubnetMaskToSet, uiGatewayToSet;

    // first: stop dhcpc process
    iRet = stopDhcpcProcess();
    if (iRet != ERROR_SUCCESS) {
        LOG_BLINE("Failed to stop the dhcpc process.\n");
        return -1;
    }

    if (m_cfgNow.bDhcp) {
		if(Sw_LogWifiConnection)
		{
        	LOGD("Wifi: dhcp\n");
		}

		//Stop current dhcpc process if found
		BOOL_t bIsRunning = FALSE;
		pid_t pidDhcpc = 0;
		iRet = ChkDhcpcProcess(OUT bIsRunning, OUT pidDhcpc);
		if(ERROR_SUCCESS == iRet)
		{
			if(bIsRunning)
			{
				iRet = kill(pidDhcpc, SIGTERM);
				if(0 == iRet)
				{
					//because we are not sure that pidDhcpc is own child process, waitpid can not be used here, have to use usleep instead of it.
					msleep_exactly(200);
				}
				else
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
#if 1//	Added by jaylon for ip
		// ipaddr
	  iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
	   if (iRet != ERROR_SUCCESS) {
		   uiCurIPv4Addr = 0;
	   }

	   // subnetmask
	   iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiCurSubnetMask);
	   if (iRet != ERROR_SUCCESS) {
		   uiCurSubnetMask = 0;
	   }

	   // gateway
	   uiCurGateway = ifc_get_default_route(m_strDefaultDeviceName.toLatin1().constData());
	   // dns
	 	getResolveConf(m_cfgNow.strDomainName, m_cfgNow.strDns1, m_cfgNow.strDns2);

		m_cfgNow.strIPv4Addr	= ipaddr_to_string(htonl(uiCurIPv4Addr));
		m_cfgNow.strSubnetMask	= ipaddr_to_string(htonl(uiCurSubnetMask));
		m_cfgNow.strGateway 	= ipaddr_to_string(htonl(uiCurGateway));
		 saveCurSettings();
		

#endif
        // Start a new dhcpc process
        iRet = startDhcpcProcess();
        if (iRet != ERROR_SUCCESS) {
            LOG_BLINE("Failed to start the dhcpc process.\n");
            return -1;
        }
    } else {
        // manually
		if(Sw_LogWifiConnection)
		{
        	LOGD("Wifi: manual\n");
		}

        // ipaddr
        iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
        if (iRet != ERROR_SUCCESS) {
            uiCurIPv4Addr = 0;
        }

        // subnetmask
        iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiCurSubnetMask);
        if (iRet != ERROR_SUCCESS) {
            uiCurSubnetMask = 0;
        }

        // gateway
        uiCurGateway = ifc_get_default_route(m_strDefaultDeviceName.toLatin1().constData());

        // config: ipaddr, subnetmask and gateway
        iRet = getIPv4AddrFromStr(m_cfgNow.strIPv4Addr.toLatin1().constData(), &uiIPv4AddrToSet);
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed to getIPv4AddrFromStr strIPv4Addr\n");
            return -1;
        }
        iRet = getIPv4AddrFromStr(m_cfgNow.strSubnetMask.toLatin1().constData(), &uiSubnetMaskToSet);
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed to getIPv4AddrFromStr strSubnetMask\n");
            return -1;
        }
        iRet = getIPv4AddrFromStr(m_cfgNow.strGateway.toLatin1().constData(), &uiGatewayToSet);
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed to getIPv4AddrFromStr strGateway\n");
            return -1;
        }

        // if different, to set
        if (uiCurIPv4Addr != uiIPv4AddrToSet) {
            iRet = setNetIfIPv4Addr(m_strDefaultDeviceName.toLatin1().constData(), uiIPv4AddrToSet);
            if (ERROR_SUCCESS != iRet) {
                LOG_BLINE("Failed to set wifi ip\n");
                return -1;
            }
        }
        if (uiCurSubnetMask != uiSubnetMaskToSet) {
            iRet = SetIfNetmask(m_strDefaultDeviceName.toLatin1().constData(), uiSubnetMaskToSet);
            if (ERROR_SUCCESS != iRet) {
                LOG_BLINE("Failed to set wifi subnetmask(0x%08x)\n", uiSubnetMaskToSet);
                return -1;
            }
        }

        if (uiCurGateway != uiGatewayToSet) {
            if (DelDefaultGatewayIfc(m_strDefaultDeviceName.toLatin1().constData()) != 0) {
                // LOG_BLINE("Failed to remove wifi default gateway\n");
            }
            iRet = AddDefaultGateway(m_cfgNow.strGateway.toLatin1().constData(),
                                     m_strDefaultDeviceName.toLatin1().constData());
            if (ERROR_SUCCESS != iRet) {
                //LOG_BLINE("Failed to add wifi default gateway\n");
                //return -1;
            }
        }

        // DNS & DomainName
        //
        QResolveConf ResolveConf;
        iRet = ResolveConf.ReloadConf();
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed ResolveConf, ret=%d\n", iRet);
            return -1;
        }

        if (m_cfgNow.strDns1 != "" && m_cfgNow.strDns1.isNull() != TRUE) {
            iRet = ResolveConf.setDomainName(m_cfgNow.strDomainName);
            if (ERROR_SUCCESS != iRet) {
                LOG_BLINE("Failed ResolveConf setDomainName, ret=%d\n", iRet);
                return -1;
            }
        }

        QStringList DnsList;
        if (m_cfgNow.strDns1 != "" && m_cfgNow.strDns1.isNull() != TRUE)
            DnsList << m_cfgNow.strDns1;
        if (m_cfgNow.strDns2 != "" && m_cfgNow.strDns2.isNull() != TRUE)
            DnsList << m_cfgNow.strDns2;
        iRet = ResolveConf.setNameServer(DnsList);
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed ResolveConf setNameServer, ret=%d\n", iRet);
            return -1;
        }

        iRet = ResolveConf.SaveConf();
        if (ERROR_SUCCESS != iRet) {
            LOG_BLINE("Failed ResolveConf SaveConf, ret=%d\n", iRet);
            return -1;
        }
    }

    return 0;
}

int QNetMgrWifi::UpdateNextCfg(WifiCfg & cfg)
{
    m_cfgNow = cfg;
    return 0;
}

/*
    disabled (0)
    enabled (1)
    connected (2)
    notConnected (3)
    notInstalled (4)
*/
void QNetMgrWifi::setStatus(int bEnable, int & iOutRet)
{
    int iRet = 0;
	bool bRet;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_cfgNow.bEnable == bEnable)	//no change
		{
			break;
		}
		
		//save settings	
		m_cfgNow.bEnable = bEnable;
		iRet = saveCurSettings();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
	    if (m_strDefaultDeviceName.isEmpty()) {	//no actual device
			break;
	    }

		if(Mp7xxCommon::WIFI_STATUS_ENABLED == bEnable)
		{
			BOOL_t bWpaSupplicantIsRunning = FALSE;
			iRet = ChkWpaSupplicantProcess(OUT bWpaSupplicantIsRunning);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}

			if(FALSE == bWpaSupplicantIsRunning)
			{
				//up the network interface
				iRet = SetIfStatus(m_strDefaultDeviceName.toLocal8Bit(), STATUS_NETIF_UP);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				iRet = startWpaSupplicant();
				if(0 != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
				int iLeftTimeoutMs = (3*1000);
				UINT64_t StartTimeMs = GetSysUpTimeMs();
				bRet = m_WpaProcess_sp->waitForStarted(iLeftTimeoutMs);
				if(false == bRet)
				{
					LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
					break;
				}
				iLeftTimeoutMs = iLeftTimeoutMs - (GetSysUpTimeMs() - StartTimeMs);
				if(0 >= iLeftTimeoutMs)
				{
					LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
					break;
				}
				StartTimeMs = GetSysUpTimeMs();
				BOOL_t bFoundWpaIfDir = FALSE;
				while(TRUE)
				{
					iRet = access(WpaSupplicant_INTERFACE_DIR_PATH, F_OK);
					if(0 == iRet)
					{
						bFoundWpaIfDir = TRUE;
						break;
					}
					iLeftTimeoutMs = iLeftTimeoutMs - (GetSysUpTimeMs() - StartTimeMs);
					if(0 >= iLeftTimeoutMs)
					{
						LOG_BLINE("WaitFor_WpaSupplicant_Start failed\n");
						break;
					}
					StartTimeMs = GetSysUpTimeMs();
					iRet = usleep(10*1000);
					if(0 != iRet && EINTR != errno)
					{
						PRINT_BFILE_LINENO_CRT_ERRINFO;
					}
				}
				if(FALSE == bFoundWpaIfDir)
				{
					break;
				}
				
				iRet = OpenWpaCtrl();
				if(0 != iRet)
				{
					break;
				}
				
				int ret = ConnectToAp();
				if (ret != 0) {
                    SendFinishedState(ret, "WifiConnectToApERR");
				}
				
				int iWpaCtrlFd = m_wpaCtrl.getEventNotifyFd();
				if(0 <= iWpaCtrlFd)
				{
					m_WpaCtrlEventFdNotifier_sp = QSharedPointer <QSocketNotifier> (new QSocketNotifier(iWpaCtrlFd, QSocketNotifier::Read, this));
					if(FALSE == m_WpaCtrlEventFdNotifier_sp.isNull())
					{
						bRet = connect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
						if(false == bRet)
						{
							LOG_BLINE("ConnWpaCtrlEventNotifier failed\n");
						}
					}
					else
					{
						LOG_BLINE("out of memory\n");
					}
				}
				else
				{
					LOG_BLINE("Invalid wpa ctrl fd(%d)\n", iWpaCtrlFd);
				}
			}
		}
		else if(Mp7xxCommon::WIFI_STATUS_DISABLED == bEnable)
		{
			//cancel fd event listening
			if(FALSE == m_WpaCtrlEventFdNotifier_sp.isNull())
			{
				m_WpaCtrlEventFdNotifier_sp->setEnabled(FALSE);
				bRet = QObject::disconnect(&(*m_WpaCtrlEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(onWpaCtrlEvent(int)));
				if(FALSE == bRet)
				{
					LOG_BLINE("WpaCtrlEvtFdNotifier.disconnect failed\n");
				}
			}

			iRet = CloseWpaCtrl();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
						
			// stop the dhcp prpocess
			iRet = stopDhcpcProcess();
			if (ERROR_SUCCESS != iRet) {
				PRINT_BFILE_LINENO_IRET_STR;
			}
			
			// stop the wpa_supplicant process
			if ((iRet = stopWpaSupplicant()) != 0) {
				LOGE("stopping wpa_supplicant process failed!\n");
				PRINT_BFILE_LINENO_IRET_STR;
			}
			//down the network interface
			iRet = SetIfStatus(m_strDefaultDeviceName.toLocal8Bit(), STATUS_NETIF_DOWN);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}	
	}while(FALSE);	
}

void QNetMgrWifi::getStatus(int & iOutRet, int &iStatus)
{
	iOutRet = ERROR_SUCCESS;

    if (m_strDefaultDeviceName.isEmpty()) {
		iStatus = Mp7xxCommon::WIFI_STATUS_NotInstalled;
        return;
    }

	if(!m_cfgNow.bEnable) {
        iStatus = Mp7xxCommon::WIFI_STATUS_DISABLED;
    } else {
        if (m_eCurWpaState == SystemSrv::WpaState_CONNECTED) {
            iStatus = Mp7xxCommon::WIFI_STATUS_Connected;
        } else if (m_eCurWpaState == SystemSrv::WpaState_DISCONNECTED) {
            iStatus = Mp7xxCommon::WIFI_STATUS_NotConnected;
        } else {
            iStatus = Mp7xxCommon::WIFI_STATUS_ENABLED;
        }
    }
 }

void QNetMgrWifi::getSignalLevel(int & iOutRet, int &level)
{
    level = 0;

    if (!m_bDevActive) {
        //LOGE("wifi device is not active.\n");
        return;
    }

    if (getScanResults() != 0) {
		if(Sw_LogWifiConnection)
		{
        	LOG_BLINE("Failed to getScanResults!\n");
		}
        return;
    }
#if 1
	INT_t  iRet;
	  UINT32_t uiCurIPv4Addr, uiCurSubnetMask, uiCurGateway;

	iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
	   if (iRet != ERROR_SUCCESS) {
		   uiCurIPv4Addr = 0;
	   }

	   // subnetmask
	   iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiCurSubnetMask);
	   if (iRet != ERROR_SUCCESS) {
		   uiCurSubnetMask = 0;
	   }

	   // gateway
	   uiCurGateway = ifc_get_default_route(m_strDefaultDeviceName.toLatin1().constData());
	   // dns
	 	getResolveConf(m_cfgNow.strDomainName, m_cfgNow.strDns1, m_cfgNow.strDns2);

		m_cfgNow.strIPv4Addr	= ipaddr_to_string(htonl(uiCurIPv4Addr));
		m_cfgNow.strSubnetMask	= ipaddr_to_string(htonl(uiCurSubnetMask));
		m_cfgNow.strGateway 	= ipaddr_to_string(htonl(uiCurGateway));

	#endif
    QList<ScanResult>::const_iterator it;
    int iDbmMax = -59, iDbmMin = -100;
    for (it = m_ScanList.begin(); it != m_ScanList.end(); it++) {
        if (it->SSID == m_cfgNow.strSSID) {
            level = (it->level - iDbmMin) * 100 / (iDbmMax - iDbmMin);
            break;
        }
    }

    iOutRet = 0;
}

int QNetMgrWifi::getInterfaces()
{
    const char *cmd = "INTERFACES";
    int ret = m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
    if (ret != 0) {
        qDebug() << "QNetMgrWifi::getInterfaces() ctrlRequest error!!";
        return -1;
    }
    m_strDefaultDeviceName = m_szReply;
    qDebug() << "Default Wifi Device Name:" << m_strDefaultDeviceName;
    return ret;
}

int QNetMgrWifi::getScanResults()
{
    const char *cmd = "SCAN_RESULTS";
    if (m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply)) != 0) {
        return -1;
    }

    QString res(m_szReply);
    QStringList lines = res.split(QRegExp("\\n"));
    bool first = true;

    m_ScanList.clear();

    for (QStringList::Iterator it = lines.begin(); it != lines.end(); it++) {
        if (first) {
            first = false;
            continue;
        }
        QStringList cols = (*it).split(QRegExp("\\t"));

        ScanResult r;
        r.BSSID = cols.count() > 0 ? cols[0] : "";
        r.frequency = cols.count() > 1 ? cols[1].toInt() : 0;
        r.level = cols.count() > 2 ? cols[2].toInt() : 0;
        if (r.level > 0)
            r.level -= 256;
        r.capabilities = cols.count() > 3 ? cols[3] : "";
        r.SSID = cols.count() > 4 ? cols[4] : "";

        m_ScanList.append(r);
    }

    return 0;
}

int QNetMgrWifi::getCurrentBSSID(QString & strBSSID)
{
    const char *cmd = "SCAN_RESULTS";
    if (m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply)) != 0) {
        return -1;
    }

    QString res(m_szReply);
    QStringList lines = res.split(QRegExp("\\n"));
    bool first = true;

    strBSSID = "";

    for (QStringList::Iterator it = lines.begin(); it != lines.end(); it++) {
        if (first) {
            first = false;
            continue;
        }
        QStringList cols = (*it).split(QRegExp("\\t"));

        ScanResult r;
        r.BSSID = cols.count() > 0 ? cols[0] : "";
        r.SSID  = cols.count() > 4 ? cols[4] : "";
        if (r.SSID == m_cfgNow.strSSID) {
            strBSSID = r.BSSID;
            break;
        }
    }

    return 0;
}

int QNetMgrWifi::scanRequest()
{
    const char *cmd = "SCAN";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::disconnect()
{
    const char *cmd = "DISCONNECT";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::reconnect()
{
    const char *cmd = "RECONNECT";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::listConfigured()
{
    const char *cmd = "LIST_NETWORKS";
    int ret;

    ret = m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));

    if (ret == 0) {
        QString res(m_szReply);
        QStringList lines = res.split(QRegExp("\\n"));
        bool first = true;

        m_listNetworks.clear();

        // network id / ssid / bssid / flags
        // 0       meeting any     [DISABLED]

        for (QStringList::Iterator it = lines.begin(); it != lines.end(); it++) {
            if (first) {
                first = false;
                continue;
            }
            QStringList cols = (*it).split(QRegExp("\\t"));

            ListNetworks r;
            r.netId = cols.count() > 0 ? cols[0].toInt() : 0;
            r.ssid = cols.count() > 1 ? cols[1] : "";
            r.bssid = cols.count() > 2 ? cols[2] : "";
            r.flags = cols.count() > 3 ? cols[3] : "";

			if(Sw_LogWifiConnection)
			{
	            LOGD("NetworkCfg: '%d' '%s' '%s' '%s'\n", r.netId,
	                 r.ssid.toLatin1().constData(), 
	                 r.bssid.toLatin1().constData(),
	                 r.flags.toLatin1().constData());
			}

            m_listNetworks.append(r);
        }
    }

    return ret;
}

int QNetMgrWifi::addNetwork()
{
    const char *cmd = "ADD_NETWORK";
    int netId = -1;
    if (m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply)) == 0)
        netId = atoi(m_szReply);
    return netId;
}

int QNetMgrWifi::removeNetwork(int netId)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "REMOVE_NETWORK %d", netId);
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::removeNetworkAll()
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "REMOVE_NETWORK all");
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::enableNetwork(int netId, int disableOthers)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "%s %d", disableOthers ? "SELECT_NETWORK" : "ENABLE_NETWORK", netId);
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::disableNetwork(int netId)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "DISABLE_NETWORK %d", netId);
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::setNetworkParam(int netId, const char *name, const char *value,
                                 bool quote)
{
    char cmdstr[256];

    snprintf(cmdstr, sizeof(cmdstr), "SET_NETWORK %d %s %s%s%s",
             netId, name, quote ? "\"" : "", value, quote ? "\"" : "");

    m_wpaCtrl.ctrlRequest(cmdstr, m_szReply, sizeof(m_szReply));
    return strncmp(m_szReply, "OK", 2) == 0 ? 0 : -1;
}

int QNetMgrWifi::setNetworkVariable(int netId, const char *name, const char *value)
{
    if (name == NULL || value == NULL) {
        return -1;
    }

    char cmdstr[256];
    if (snprintf(cmdstr, sizeof(cmdstr), "SET_NETWORK %d %s %s", 
                 netId, name, value) >= (int)sizeof(cmdstr)) {
        qDebug() << "QNetMgrWifi::setNetworkVariable() cmdTooLong!!";
        return -1;
    }

    return m_wpaCtrl.ctrlRequest(cmdstr, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::ping()
{
    const char *cmd = "PING";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::setScanMode(int setActive)
{
    const char *cmd = setActive ? "DRIVER SCAN-ACTIVE" : "DRIVER SCAN-PASSIVE";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

int QNetMgrWifi::reassociate()
{
    const char *cmd = "REASSOCIATE";
    return m_wpaCtrl.ctrlRequest(cmd, m_szReply, sizeof(m_szReply));
}

BOOL_t QNetMgrWifi::HasWpaCtrlEvent()
{
    return m_wpaCtrl.HasWpaCtrlEvent();
}

int QNetMgrWifi::RecvWpaCtrlEvent(char *buf, size_t buflen)
{
    return m_wpaCtrl.RecvEvent(buf, buflen);
}

