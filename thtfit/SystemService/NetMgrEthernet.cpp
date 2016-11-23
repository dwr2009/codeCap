#include "NetMgrEthernet.h"
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
#include "QNetDhcp.h"
#include <sys/types.h>
#include <signal.h>
#include <ThreadSleep.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <NetDevListReader.h>
#include <FileCreator.h>
#include <unistd.h>

#define	KEY_NetMgrEthernet_DefaultDeviceName	"NetMgrEthernet/DefaultDeviceName"
#define	KEY_NetMgrEthernet_AutoGetIP			"NetMgrEthernet/AutoGetIP"
#define	KEY_NetMgrEthernet_IPv4Addr				"NetMgrEthernet/IPv4Addr"
#define	KEY_NetMgrEthernet_SubnetMask			"NetMgrEthernet/SubnetMask"

QDBusIf_NetMgrEthernet::QDBusIf_NetMgrEthernet(QNetMgrEthernet * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pNetMgrEthernet = pParentObj;
}

void QDBusIf_NetMgrEthernet::getDefaultDeviceInfo(int & iOutRet, QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask)
{
	m_pNetMgrEthernet->getDefaultDeviceInfo(iOutRet, strDeviceName, strIPv4Addr, strSubnetMask);
}

void QDBusIf_NetMgrEthernet::readSettings(int & iOutRet, QString & strDefaultDeviceName, bool & bAutoGetIP, QString & strIPv4Addr, QString & SubnetMask)
{
	m_pNetMgrEthernet->readSettings(iOutRet, strDefaultDeviceName, bAutoGetIP, strIPv4Addr, SubnetMask);
}

void QDBusIf_NetMgrEthernet::applySettingsForDefaultDevice(bool bAutoGetIP, QString strIPv4Addr, QString strSubnetMask, int & iOutRet)
{
	m_pNetMgrEthernet->applySettingsForDefaultDevice(bAutoGetIP, strIPv4Addr, strSubnetMask, iOutRet);
}

void QDBusIf_NetMgrEthernet::getDefEthDevName(int & iOutRet, QString & strEthDevName)
{
	if(m_pNetMgrEthernet)
	{
		strEthDevName = m_pNetMgrEthernet->getDefaultEthDevName();
		iOutRet = ERR_SUCCESS;
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

// class QNetMgrEthernet

QNetMgrEthernet::QNetMgrEthernet()
{	
	INT_t iRet;
	
	m_strDefaultDeviceName = PREFERRED_ETH_DEV_gphy;
	m_bAutoGetIP = TRUE;

	iRet = UpdateMp7xxGlobalStatusData();
	if(ERROR_SUCCESS != iRet)
	{
		if(m_strIPv4Addr.isEmpty() || m_strSubnetMask.isEmpty())
		{
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

INT_t QNetMgrEthernet::setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	m_DBusConn_wp = DBusConn_sp;

	return iOutRet;
}

const QString QNetMgrEthernet::getDefaultEthDevName()
{
	return m_strDefaultDeviceName;
}

INT_t QNetMgrEthernet::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		//Read settings
		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		m_strDefaultDeviceName = SysSrvSettingsObj_sp->value(KEY_NetMgrEthernet_DefaultDeviceName, 
			getDefaultEthDevName()).toString();
		//automatically detect
		{
			QNetDevListReader oNetDevListReader;
			iRet = oNetDevListReader.Scan();
			if(ERR_SUCCESS == iRet)
			{
				/*
				UINT32_t productId;
				iRet = FileDataToBuffer("/proc/device-tree/bolt/product-id", (PBYTE)(&productId), sizeof(productId), NULL);
				if(ERR_SUCCESS == iRet)
				{
				}
				else
				{
					productId = 0x000000;
					PRINT_BFILE_LINENO_IRET_STR;
				}
				//
				if(0x30005272 == productId)
				{
					m_strDefaultDeviceName = PREFERRED_ETH_DEV_eth0;
				}
				else 
				*/
				if(TRUE == oNetDevListReader.HasDev(PREFERRED_ETH_DEV_gphy))
				{
					m_strDefaultDeviceName = PREFERRED_ETH_DEV_gphy;
				}
				else if(TRUE == oNetDevListReader.HasDev(PREFERRED_ETH_DEV_eth0))
				{
					m_strDefaultDeviceName = PREFERRED_ETH_DEV_eth0;
				}
				else
				{
					m_strDefaultDeviceName = PREFERRED_ETH_DEV_eth0;
				}
			}
			else	//err
			{
				LOG_BLINE("scanning net dev list err,%d\n", iRet);
			}
		}
		qDebug() << "DefEthDevName=" << m_strDefaultDeviceName;
		//qDebug() << "DefDevName:" << m_strDefaultDeviceName;
		m_bAutoGetIP = SysSrvSettingsObj_sp->value(KEY_NetMgrEthernet_AutoGetIP, true).toBool();
		m_strIPv4Addr = SysSrvSettingsObj_sp->value(KEY_NetMgrEthernet_IPv4Addr).toString();
		//qDebug() << "IP:" << m_strIPv4Addr;
		m_strSubnetMask = SysSrvSettingsObj_sp->value(KEY_NetMgrEthernet_SubnetMask).toString();

		iRet = UpdateMp7xxGlobalStatusData();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		DBusConn_sp = m_DBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		m_DBusIfAdaptor_NetMgrEthernet_sp = QSharedPointer <QDBusIf_NetMgrEthernet> (new QDBusIf_NetMgrEthernet(this));
		if(m_DBusIfAdaptor_NetMgrEthernet_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = DBusConn_sp->registerObject(DBUS_PATH_NetworkManagerSrv_Ethernet, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_PATH_NetworkManagerSrv_Ethernet);
			}
			break;
		}

		//stop current dhcpc process
		BOOL_t bIsRunning = FALSE;
		pid_t pidDhcpc = 0;
		iRet = QNetDhcp::ChkDhcpcProcess(m_strDefaultDeviceName.toLocal8Bit(), OUT bIsRunning, OUT pidDhcpc);
		if(ERROR_SUCCESS == iRet)
		{
			if(bIsRunning)
			{
				//LOG_BLINE("stop %d dhcpc(%s)\n", pidDhcpc, qPrintable(m_strDefaultDeviceName))
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
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		DBusConn_sp = m_DBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_NetworkManagerSrv_Ethernet);			
		}

		iRet = StopDhcpcProcess();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iOutRet;
}

void QNetMgrEthernet::getDefaultDeviceInfo(int & iOutRet, QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask,
	BOOL_t * CONST pbUseDhcp/* = NULL*/, quint32 * pIpV4Addr/* = NULL*/, quint32 * pSubnetMaskV4Addr/* = NULL*/)
{
	INT_t iRet;
	UINT32_t uiIPv4Addr;
	UINT32_t uiSubnetMask;

	iOutRet = ERROR_SUCCESS;
	
	do
	{
		strDeviceName = m_strDefaultDeviceName;

		//UseDhcp?
		if(pbUseDhcp)
		{
			*pbUseDhcp = m_bAutoGetIP;
		}
		
		iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiIPv4Addr);
		if(ERROR_SUCCESS == iRet)
		{
			char szBuf[64];
			iRet = convertIPv4Addr_BinToStr(uiIPv4Addr, szBuf, sizeof(szBuf));
			if(ERROR_SUCCESS == iRet)
			{
				strIPv4Addr = szBuf;
			}
			else
			{
				strIPv4Addr.clear();
			}
			if(pIpV4Addr)
			{
				*pIpV4Addr = uiIPv4Addr;
			}
		}
		else
		{
			if(pIpV4Addr)
			{
				*pIpV4Addr = 0x00000000;
			}
			strIPv4Addr.clear();
		}

		iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiSubnetMask);
		if(ERROR_SUCCESS == iRet)
		{
			char szBuf[64];
			iRet = convertIPv4Addr_BinToStr(uiSubnetMask, szBuf, sizeof(szBuf));
			if(ERROR_SUCCESS == iRet)
			{
				strSubnetMask = szBuf;
			}
			else
			{
				strSubnetMask.clear();
			}
			if(pSubnetMaskV4Addr)
			{
				*pSubnetMaskV4Addr = uiSubnetMask;
			}
		}
		else
		{
			strSubnetMask.clear();
			if(pSubnetMaskV4Addr)
			{
				*pSubnetMaskV4Addr = 0x00000000;
			}
		}
	}while(FALSE);
}

INT_t QNetMgrEthernet::getSavedNetworkCfgInfo(QString & strDefDevName, BOOL_t * CONST pbUseDhcp, quint32 * CONST pIpV4Addr, 
	quint32 * CONST pSubnetMaskV4Addr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		strDefDevName = m_strDefaultDeviceName;
		
		if(pbUseDhcp)
		{
			*pbUseDhcp = m_bAutoGetIP;
		}

		if(pIpV4Addr)
		{
			iRet = getIPv4AddrFromStr(m_strIPv4Addr.toLatin1(), pIpV4Addr);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}

		if(pSubnetMaskV4Addr)
		{
			iRet = getIPv4AddrFromStr(m_strSubnetMask.toLatin1(), pSubnetMaskV4Addr);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

void QNetMgrEthernet::readSettings(int & iOutRet, QString & strDefaultDeviceName, bool & bAutoGetIP, 
	QString & strIPv4Addr, QString & SubnetMask)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		strDefaultDeviceName = m_strDefaultDeviceName;
		bAutoGetIP = m_bAutoGetIP;
		strIPv4Addr = m_strIPv4Addr;
		SubnetMask = m_strSubnetMask;
	}while(FALSE);
}

void QNetMgrEthernet::applySettingsForDefaultDevice(bool bAutoGetIP, QString strIPv4Addr, QString strSubnetMask, int & iOutRet,
	BOOL_t bSaveCfg/* = TRUE*/)
{
	INT_t iRet;
	BOOL_t bRet;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	BOOL_t bNeedUpdateGlobalStatusData = FALSE;

	iOutRet = ERROR_SUCCESS;
	
	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		//bNeedUpdateGlobalStatusData?
		if(m_bAutoGetIP != bAutoGetIP || m_strIPv4Addr != strIPv4Addr || m_strSubnetMask != strSubnetMask)
		{
			bNeedUpdateGlobalStatusData = TRUE;
		}

		if(bAutoGetIP)
		{
			if(bSaveCfg)
			{
				//Cache the settings
				m_bAutoGetIP = bAutoGetIP;
				//Save the settings
				SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_AutoGetIP, m_bAutoGetIP);
			}
			
			if(FALSE == strIPv4Addr.isEmpty() && IsValidIpAddr(strIPv4Addr.toLatin1().constData(), NULL))
			{
				if(bSaveCfg)
				{
					//Cache the settings
					m_strIPv4Addr = strIPv4Addr;
					//Save the settings
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_IPv4Addr, m_strIPv4Addr);
				}
			}
			if(FALSE == strSubnetMask.isEmpty() && IsValidIpAddr(strSubnetMask.toLatin1().constData(), NULL))
			{
				if(bSaveCfg)
				{
					//Cache the settings
					m_strSubnetMask = strSubnetMask;
					//Save the settings
					SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_SubnetMask, m_strSubnetMask);
				}
			}
		}
		else	//manually set
		{
			//Parameters checking
			bRet = IsValidIpAddr(strIPv4Addr.toLatin1().constData(), NULL);
			if(FALSE == bRet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
			bRet = IsValidIpAddr(strSubnetMask.toLatin1().constData(), NULL);
			if(FALSE == bRet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
			if(bSaveCfg)
			{
				//Cache the settings
				m_bAutoGetIP = bAutoGetIP;
				m_strIPv4Addr = strIPv4Addr;
				m_strSubnetMask = strSubnetMask;
				//Save the settings
				SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_AutoGetIP, m_bAutoGetIP);
				SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_IPv4Addr, m_strIPv4Addr);
				SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_SubnetMask, m_strSubnetMask);
				}
		}while(FALSE);
		if(bSaveCfg)
		{
			SysSrvSettingsObj_sp->sync();
			sync();
		}
		//Implement the actual action
		iOutRet = doNetworkConfig(bAutoGetIP, strIPv4Addr, strSubnetMask);
	}while(FALSE);

	if(bNeedUpdateGlobalStatusData)
	{
		iRet = UpdateMp7xxGlobalStatusData();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}

INT_t QNetMgrEthernet::applyCurSettings()
{
	INT_t iOutRet = ERROR_SUCCESS;
	BOOL_t bSaveCfg = FALSE;
	
	applySettingsForDefaultDevice(m_bAutoGetIP, m_strIPv4Addr, m_strSubnetMask, OUT iOutRet, bSaveCfg);

	return iOutRet;
}

BOOL_t QNetMgrEthernet::HaveAutoIpSet()
{
	return m_bAutoGetIP;
}

INT_t QNetMgrEthernet::setIpV4Addr(const QString & strIpV4Addr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszDefaultNetIfName = m_strDefaultDeviceName.toLatin1().constData();
	UINT32_t uiCurIPv4Addr, uiIPv4AddrToSet;

	do
	{
		//LOG_BLINE("DefaultDeviceName=%s\n", qPrintable(m_strDefaultDeviceName));
		//ifconfig up
		iRet = SetIfStatus(pszDefaultNetIfName, STATUS_NETIF_UP);			
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to SetIfStatus(%s,up)\n", pszDefaultNetIfName);
		}
		//IP
		iRet = GetIfAddrIPV4(pszDefaultNetIfName, &uiCurIPv4Addr);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to GetIfAddrIPV4\n");
			uiCurIPv4Addr = 0;
		}
		iRet = getIPv4AddrFromStr(strIpV4Addr.toLatin1().constData(), &uiIPv4AddrToSet);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//LOG_BLINE("Dev:%s,CurIP:0x%08x,IPtoSet:0x%08x\n", pszDefaultNetIfName, uiCurIPv4Addr, uiIPv4AddrToSet);
		if(uiCurIPv4Addr != uiIPv4AddrToSet)
		{
			iRet = setNetIfIPv4Addr(pszDefaultNetIfName, uiIPv4AddrToSet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			//Cache the settings
			m_strIPv4Addr = strIpV4Addr;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::saveIpV4Addr(const QString & strIpV4Addr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_IPv4Addr, strIpV4Addr);
		SysSrvSettingsObj_sp->sync();

		//Also save it to SysProp database
		iRet = SysProp_setPersistent(SysProp_KEY_PrevEthernetIP, strIpV4Addr.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;	
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::setSubnetMask(const QString & strSubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszDefaultNetIfName = m_strDefaultDeviceName.toLatin1().constData();
	UINT32_t uiCurSubnetMask, uiSubnetMaskToSet;

	do
	{
		//ifconfig up
		iRet = SetIfStatus(pszDefaultNetIfName, STATUS_NETIF_UP);			
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to SetIfStatus(%s,up)\n", pszDefaultNetIfName);
		}
		//Subnet mask
		iRet = getNetIfSubnetMask(pszDefaultNetIfName, &uiCurSubnetMask);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = getIPv4AddrFromStr(strSubnetMask.toLatin1().constData(), &uiSubnetMaskToSet);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(uiCurSubnetMask != uiSubnetMaskToSet)
		{
			iRet = SetIfNetmask(pszDefaultNetIfName, uiSubnetMaskToSet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			//Cache the settings
			m_strSubnetMask = strSubnetMask;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::saveSubnetMask(const QString & strSubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_SubnetMask, strSubnetMask);
		SysSrvSettingsObj_sp->sync();
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::setDhcp(bool bUseDhcp, BOOL_t bApply/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszDefaultNetIfName = m_strDefaultDeviceName.toLatin1().constData();
	BOOL_t bNeedUpdateGlobalStatusData = FALSE;

	do
	{
		//bNeedUpdateGlobalStatusData?
		if(m_bAutoGetIP != bUseDhcp)
		{
			bNeedUpdateGlobalStatusData = TRUE;
		}

		//Cache the settings
		m_bAutoGetIP = bUseDhcp;

		if(bApply)
		{
			if(m_bAutoGetIP)
			{
				//ifconfig up
				iRet = SetIfStatus(pszDefaultNetIfName, STATUS_NETIF_UP);			
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Failed to SetIfStatus(%s,up)\n", pszDefaultNetIfName);
				}
				//Stop the dhcpc process
				iRet = StopDhcpcProcess();
				if(ERR_TIMEOUT == iRet)
				{
				}
				else if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				//Start a new dhcpc process
				iRet = startDhcpcProcess();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else	//manually
			{
				//Stop the dhcpc process
				iRet = StopDhcpcProcess();
				if(ERR_TIMEOUT == iRet)
				{
				}
				else if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
		}
	}while(FALSE);

	if(bNeedUpdateGlobalStatusData)
	{
		iRet = UpdateMp7xxGlobalStatusData();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	return iOutRet;
}

INT_t QNetMgrEthernet::saveDhcp(bool bUseDhcp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		//Save the settings
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_AutoGetIP, (int)bUseDhcp);
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::saveCurSettings()
{
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_IPv4Addr, m_strIPv4Addr);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_SubnetMask, m_strSubnetMask);
		SysSrvSettingsObj_sp->setValue(KEY_NetMgrEthernet_AutoGetIP, m_bAutoGetIP);

		SysSrvSettingsObj_sp->sync();
	}while(FALSE);

	return iOutRet;
}

void QNetMgrEthernet::onDhcpDone(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
	const QString & strGatewayAddr,const QString & strDns)
{
	Q_UNUSED(strNetDevName);
	Q_UNUSED(strIPv4Addr);
	Q_UNUSED(strSubnetMask);
	Q_UNUSED(strGatewayAddr);
	Q_UNUSED(strDns);
}

INT_t QNetMgrEthernet::startDhcpcProcess()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strNetIfDevName = m_strDefaultDeviceName;

	do
	{
		if(m_DhcpcProcess_sp.isNull())
		{
			m_DhcpcProcess_sp = QSharedPointer <QProcess> (new QProcess(this));
			if(m_DhcpcProcess_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
		QStringList CmdLineParams;
		if(strNetIfDevName.isEmpty())
		{
			strNetIfDevName = getDefaultEthDevName();
		}
		CmdLineParams << ((QString)"--interface=" + strNetIfDevName) << "--foreground";
		//qDebug() << CmdLineParams;
		m_DhcpcProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
		m_DhcpcProcess_sp->start("udhcpc", CmdLineParams, QIODevice::NotOpen);
	}while(FALSE);
	
	return iOutRet;
}

INT_t QNetMgrEthernet::StopDhcpcProcess()
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	int TimeoutMs;

	do
	{
		if(m_DhcpcProcess_sp.isNull())
		{
			break;
		}
		if(QProcess::NotRunning == m_DhcpcProcess_sp->state())
		{
			break;
		}
		m_DhcpcProcess_sp->terminate();
		TimeoutMs = 5*1000;
		bRet = m_DhcpcProcess_sp->waitForFinished(TimeoutMs);
		if(false == bRet)
		{
			LOG_BLINE("DhcpcProcess temination timeout\n");
			iOutRet = ERR_TIMEOUT;
		}
	}while(FALSE);

	return iOutRet;
}

int getNetworkDeviceHwAddr(const char *name, void *ptr)
{
	int sock, result = -1;
	struct ifreq ifr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock) {
		memset(&ifr, 0, sizeof(struct ifreq));
		strncpy(ifr.ifr_name, name, IFNAMSIZ);
		ifr.ifr_name[IFNAMSIZ - 1] = 0;

		result = ioctl(sock, SIOCGIFHWADDR, &ifr);
		if (result >= 0) {
			memcpy(ptr, &ifr.ifr_hwaddr.sa_data, ETH_ALEN);
			result = 0;
		}
		close(sock);
	}

	return result;
}

INT_t QNetMgrEthernet::doNetworkConfig(bool bAutoGetIP, CONST QString & strIPv4Addr, CONST QString & strSubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t uiCurIPv4Addr, uiIPv4AddrToSet, uiCurSubnetMas, uiSubnetMaskToSet;
	/*Add by bert 2016.7.18*/
	bool bRet;
	int TimeoutMs = 10*1000;

	do
	{
		if(bAutoGetIP)
		{
			//ifconfig up
			iRet = SetIfStatus(m_strDefaultDeviceName.toLatin1().constData(), STATUS_NETIF_UP);
			if(ERR_IOCTL_FAILED == iRet && 0 == m_strDefaultDeviceName.compare(PREFERRED_ETH_DEV_gphy))
			{
				iRet = SetIfStatus(PREFERRED_ETH_DEV_eth0, STATUS_NETIF_UP);
				if(ERR_SUCCESS != iRet)
				{
					LOG_BLINE("Failed(%d) to SetIfStatus(%s,up)\n", iRet, PREFERRED_ETH_DEV_eth0);
				}
				iRet = SetIfStatus(m_strDefaultDeviceName.toLatin1().constData(), STATUS_NETIF_UP);
			}
			//chk
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Failed(%d) to SetIfStatus(%s,up)\n", iRet, m_strDefaultDeviceName.toLatin1().constData());
			}
			//Stop the dhcpc process
			iRet = StopDhcpcProcess();
			if(ERR_TIMEOUT == iRet)
			{
			}
			else if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			// get ethernet hwaddr
			unsigned char hwaddr[6] = {0, };
			iRet = getNetworkDeviceHwAddr(m_strDefaultDeviceName.toLatin1().constData(), &hwaddr);
			if(ERROR_SUCCESS != iRet) {
				iOutRet = iRet;
				LOG_BLINE("Failed to getNetworkDeviceHwAddr: dev=%s\n", m_strDefaultDeviceName.toLatin1().constData());
				break;
			}

			// get ethernet IP
			uiCurIPv4Addr = 0;
			iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
			if(ERROR_SUCCESS != iRet)
			{
				if(ERR_IOCTL_FAILED == iRet && EADDRNOTAVAIL == errno)
				{
				}
				else {
					LOG_BLINE("Failed to GetIfAddrIPV4,ret %d,errno %d(%s)\n", iRet, errno, strerror(errno));
				}
			}

			if (uiCurIPv4Addr == 0) {
				// first: set IpAddr to 169.254.x.x
				QString strIPv4Mac;
				strIPv4Mac.sprintf("169.254.%d.%d", hwaddr[4], hwaddr[5]);
				iRet = getIPv4AddrFromStr(strIPv4Mac.toLatin1().constData(), &uiIPv4AddrToSet);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = setNetIfIPv4Addr(m_strDefaultDeviceName.toLatin1().constData(), uiIPv4AddrToSet);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Failed to setNetIfIPv4Addr: ipaddr=%s\n", qPrintable(strIPv4Mac));
					iOutRet = iRet;
					break;
				}
			}

			//Start a new dhcpc process
			iRet = startDhcpcProcess();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			/*Add by bert 2007.7.18*/
			bRet = m_DhcpcProcess_sp->waitForFinished(TimeoutMs);
			if(FALSE == bRet)
			{
				LOG_BLINE("DhcpcProcess temination timeout\n");
			}
		}
		else	//manually
		{
			//Stop the dhcpc process
			iRet = StopDhcpcProcess();
			if(ERR_TIMEOUT == iRet)
			{
			}
			else if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			//ifconfig up
			iRet = SetIfStatus(m_strDefaultDeviceName.toLatin1().constData(), STATUS_NETIF_UP);			
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Failed to SetIfStatus(%s,up)\n", m_strDefaultDeviceName.toLatin1().constData());
			}
			//IP
			iRet = GetIfAddrIPV4(m_strDefaultDeviceName.toLatin1().constData(), &uiCurIPv4Addr);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Failed to GetIfAddrIPV4\n");
			}
			iRet = getIPv4AddrFromStr(strIPv4Addr.toLatin1().constData(), &uiIPv4AddrToSet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(uiCurIPv4Addr != uiIPv4AddrToSet)
			{
				iRet = setNetIfIPv4Addr(m_strDefaultDeviceName.toLatin1().constData(), uiIPv4AddrToSet);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			//Subnet mask
			iRet = getNetIfSubnetMask(m_strDefaultDeviceName.toLatin1().constData(), &uiCurSubnetMas);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = getIPv4AddrFromStr(strSubnetMask.toLatin1().constData(), &uiSubnetMaskToSet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(uiCurSubnetMas != uiSubnetMaskToSet)
			{
				iRet = SetIfNetmask(m_strDefaultDeviceName.toLatin1().constData(), uiSubnetMaskToSet);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernet::UpdateMp7xxGlobalStatusData()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	UINT32_t uiEthIpV4Addr = 0, uiEthSubnetMaskV4 = 0;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);	
		if(NULL == pSystemServiceAppIf)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		iRet = getIPv4AddrFromStr(m_strIPv4Addr.toLatin1(), &uiEthIpV4Addr);
		if(ERR_INVALID_IPv4ADDR == iRet)
		{
			//treat this case as normal
			uiEthIpV4Addr = 0;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = getIPv4AddrFromStr(m_strSubnetMask.toLatin1(), &uiEthSubnetMaskV4);
		if(ERR_INVALID_IPv4ADDR == iRet)
		{
			//treat this case as normal
			uiEthSubnetMaskV4 = 0;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		CMp7xxGlobalStatus & Mp7xxGlobalStatus = pSystemServiceAppIf->getMp7xxGlobalStatusObj();
		iRet = Mp7xxGlobalStatus.setCurEthSettings(m_bAutoGetIP, uiEthIpV4Addr, uiEthSubnetMaskV4);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		iOutRet = iRet;
 	}while(FALSE);

	return iOutRet;
}

