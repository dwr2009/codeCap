#include <stdio.h>
#include "SystemServiceApp.h"
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <SignalInfo.h>
#include <signal.h>
#include <ErrPrintHelper.h>
#include <LinuxSignalHandlerEx.h>
#include <QDBusError>
#include "DbgLogSwitchDef.h"
#include <StackBufString.h>
#include <stdlib.h>
#include <MediaPlayerAppDef.h>
#include <SysBaseSrvApi.h>
#include <QFile>
#include <FileCreator.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define	QDBUS_CONN_NAME_SS				"QDBusConnection_SS"

QSharedPointer <QSystemService> g_SystemService_sp;

QSystemService::QSystemService(int argc, char * argv[]) : QApplication(argc, argv/*, QApplication::Tty*/)
{
	INT_t iRet;

	iRet = CLinuxSignalHandlerEx::HookSignal(SIGINT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGTERM, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGQUIT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	m_CmdLineParamList_sp = QSharedPointer <QStringList> (new QStringList);
	if(FALSE == m_CmdLineParamList_sp.isNull())
	{
		*m_CmdLineParamList_sp = arguments();
	}

	m_MainThreadId = QThread::currentThreadId ();
	//LOG_BLINE("MainThrdId=%d\n", m_MainThreadId);

	iRet = RegDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QSystemService::~QSystemService()
{
	INT_t iRet;

	m_IrInputCtrl_sp.clear();
	m_HwWatchDog_sp.clear();
	m_PowerMate_sp.clear();//added by lxj 2014-3-4
	
	iRet = UnregDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

VOID QSystemService::SigHandler(VOID * pCbContext, int SigNo)
{
	QSystemService * pApp = (typeof(pApp))pCbContext;
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		if(pApp)
		{
			pApp->OnLinuxSignal(SigNo);
		}
	}
}

VOID QSystemService::OnLinuxSignal(int SigNo)
{	
	LOG("(%d)Signal:%s\n", getpid(), CSignalInfo::GetSingalDescFromSigNo(SigNo));
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		quit();
	}
}

INT_t QSystemService::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iId;
	bool bRet;

	do
	{
		if(m_CmdLineParamList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_SysSrvSettings_sp = QSharedPointer <QSettings> (
			new QSettings(applicationDirPath()+"/"+SysSrv_SETTINGS_REL_PATH, QSettings::IniFormat));
		if(m_SysSrvSettings_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		m_DBusConn_sp = QSharedPointer <QDBusConnection> (new QDBusConnection(QDBUS_CONN_NAME_SS));
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}		
		
		m_NetworkManagerSrv_sp = QSharedPointer <QNetworkManagerSrv> (new QNetworkManagerSrv());
		if(m_NetworkManagerSrv_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_SystemEvent_sp = QSharedPointer <QSystemEvent> (new QSystemEvent());
		if(m_SystemEvent_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_IrInputCtrl_sp = QSharedPointer <QIrInputCtrl> (new QIrInputCtrl());
		if(m_IrInputCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		m_HwWatchDog_sp = QSharedPointer <QHwWatchDog> (new QHwWatchDog());
		if(m_HwWatchDog_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_PowerMate_sp = QSharedPointer <QPowerMate> (new QPowerMate());//added by lxj 2014-3-4
		if(m_PowerMate_sp.isNull()){
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_BackendTransactionThread_sp = QSharedPointer <QBackendTransactionThread> (new QBackendTransactionThread);
		if(m_BackendTransactionThread_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
			
		*m_DBusConn_sp = QDBusConnection::systemBus();
		if(m_DBusConn_sp->isConnected())
		{
		}
		else
		{
			iOutRet = ERR_CONNECTION_FAIL;
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to connect dbus\n");
			}
			break;
		}

		bRet = m_DBusConn_sp->registerService(SS_DBUS_SERVICE_NAME);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", SS_DBUS_SERVICE_NAME);
			}
			break;
		}

		iRet = m_NetworkManagerSrv_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_SystemEvent_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = m_HwWatchDog_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		iRet = m_IrInputCtrl_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = m_PowerMate_sp->InitInstance();//added by lxj 2014-3-4
		if(ERROR_SUCCESS != iRet ){
			//Maybe USB PowerMate not PlugIn
		}

		iId = (*m_CmdLineParamList_sp).indexOf("-applyInitNetCfg");
		if(0 < iId)	//found
		{
			iRet = m_NetworkManagerSrv_sp->applyCurSettings();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;				
			}
		}

		//Connect the dbus signal WebBrowser.DataCommSrv.DownloadFinished
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_CfgMgr, DbusIf_SysSrv_CfgMgr,
			"NewSysCfgIsReady", this, SLOT(On_NewSysCfgIsReady(const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.SyncCacheToStorage) err\n");
		}

		//DhcpDone2 signal hook
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_SysEvt,
			DbusIf_SysSrv_SysEvt, SIGNAL_Evt_DhcpDone2, this,
			SLOT(On_DhcpDone2(const QString &,const QString &,const QString &,const QString &,const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.DhcpDone2) err\n");
		}

		m_BackendTransactionThread_sp->start();
	}while(FALSE);

	return iOutRet;
}

INT_t QSystemService::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;

	do
	{
		if(FALSE == m_BackendTransactionThread_sp.isNull())
		{
			if(m_BackendTransactionThread_sp->isRunning())
			{
				m_BackendTransactionThread_sp->quit();
				bRet = m_BackendTransactionThread_sp->wait(15*1000);
				if(false == bRet)
				{
					LOG_BLINE("Waiting BackendTransactionThread to exit timeout\n");
				}
			}
		}
		
		if(FALSE == m_IrInputCtrl_sp.isNull())
		{
			iRet = m_IrInputCtrl_sp->ExitInstance();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		if(FALSE == m_HwWatchDog_sp.isNull())
		{
			iRet = m_HwWatchDog_sp->ExitInstance();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		if(FALSE == m_SystemEvent_sp.isNull())
		{
			iRet = m_SystemEvent_sp->ExitInstance();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		if(FALSE == m_NetworkManagerSrv_sp.isNull())
		{
			iRet = m_NetworkManagerSrv_sp->ExitInstance();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		if(FALSE == m_PowerMate_sp.isNull())//added by lxj 2014-3-4
		{
			iRet = m_PowerMate_sp->ExitInstance();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		m_IrInputCtrl_sp.clear();
		m_HwWatchDog_sp.clear();
		m_SystemEvent_sp.clear();
		m_NetworkManagerSrv_sp.clear();
		m_PowerMate_sp.clear();//added by lxj 2014-3-4
	
		if(m_DBusConn_sp->isConnected())
		{
			bRet = m_DBusConn_sp->unregisterService(SS_DBUS_SERVICE_NAME);
			if(false == bRet)
			{
				//iOutRet = ERR_REGISTER_FAIL;
				LOG_BLINE("Failed to unreg dbus srv %s\n", SS_DBUS_SERVICE_NAME);
				//break;
			}
			QDBusConnection::disconnectFromBus(QDBUS_CONN_NAME_SS);
		}
	}while(FALSE);

	return iOutRet;
}

int QSystemService::exec()
{
	int iProcessExitCode = ERROR_SUCCESS;
	INT_t iRet;

	do
	{
		iRet = InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iProcessExitCode = SS_PEC_INIT_INSTANCE_FAIL;
			break;
		}
		
		iProcessExitCode = QApplication::exec();
		
		iRet = ExitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iProcessExitCode = SS_PEC_EXIT_INSTANCE_FAIL;
			break;
		}
	}while(FALSE);

	return iProcessExitCode;
}

QSharedPointer <QDBusConnection> QSystemService::getDBusConnection()
{
	//PRINT_BFILE_LINENO_NEWLINE;
	return m_DBusConn_sp;
}

QSharedPointer <QSettings> QSystemService::getSysSrvSettingsObj()
{
	return m_SysSrvSettings_sp;
}

CMp7xxGlobalStatus & QSystemService::getMp7xxGlobalStatusObj()
{
	return m_Mp7xxGlobalStatusData;
}

int QSystemService::On_NewSysCfgIsReady(const QString & SysCfgDirPath)
{
	int iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("SysCfgDirPath=%s\n", qPrintable(SysCfgDirPath));
	}

	do
	{
		// /etc/snmp/snmpd.conf
		iRet = strShellCmd.Format("cp -fa \"%s/etc/snmp/snmpd.conf\" /etc/snmp/snmpd.conf", SysCfgDirPath.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
		// /MP7XX/Config/SystemService.ini
		iRet = strShellCmd.Format("cp -fa \"%s/MP7XX/Config/SystemService.ini\" /MP7XX/Config/SystemService.ini", SysCfgDirPath.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}		
		// /MP7XX/Config/Uinput.xml
		{
			QString strSrcFilePath = SysCfgDirPath + "/MP7XX/Config/Uinput.xml";
			if(QFile::exists(strSrcFilePath))
			{
				iRet = strShellCmd.Format("cp -fa \"%s/MP7XX/Config/Uinput.xml\" /MP7XX/Config/Uinput.xml", SysCfgDirPath.toUtf8().constData());
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				}
			}
		}
		// /data/SysProp.bin
		{
			QString strSrcFilePath = SysCfgDirPath + "/data/SysProp.bin";
			if(QFile::exists(strSrcFilePath))
			{
				iRet = strShellCmd.Format("cp -fa \"%s/data/SysProp.bin\" /data/SysProp.bin", SysCfgDirPath.toUtf8().constData());
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				}
			}
		}
		// Save current network settings
		iRet = m_NetworkManagerSrv_sp->saveCurSettings();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

void QSystemService::On_DhcpDone2(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
	const QString & strGatewayAddr,const QString & strDns)
{
	int iOutRet = ERROR_SUCCESS, iRet;

	Q_UNUSED(iOutRet);
	Q_UNUSED(strSubnetMask);
	Q_UNUSED(strGatewayAddr);
	Q_UNUSED(strDns);

	/*
	LOG_BLINE("Dev=%s,IP=%s,SubNetMask=%s,Gateway=%s,Dns=%s\n", 
		qPrintable(strNetDevName), qPrintable(strIPv4Addr), qPrintable(strSubnetMask), qPrintable(strGatewayAddr), qPrintable(strDns));
	*/
	
	if(m_NetworkManagerSrv_sp->getDefaultEthDevName() == strNetDevName)
	{
		iRet = SysProp_setPersistent(SysProp_KEY_CurEthernetIP, strIPv4Addr.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;	
		}
	}
	m_NetworkManagerSrv_sp->onDhcpDone(strNetDevName, strIPv4Addr, strSubnetMask, strGatewayAddr, strDns);
}

int main(int argc, char * argv[])
{
	int ProcessExitCode = 0;

    pid_t iPid=-1;
    
        
    printf("start service %s ...\n",argv[0]);
    iPid=fork();
    if(iPid>0)
    {
        printf("parent exit\n");
        exit(0);
    }
    if(iPid<0)
    {
        printf("%s fork failed\n",argv[0]);
        exit(-1);
    }
    printf("service %s init\n",argv[0]);
    setsid();
#if 0
    do
    {
        int iNullFd=-1;
        int iRetFd=-1;
        const int iStdInFd=0;
        const int iStdOutFd=1;
        const int iStdErrFd=2;
        iNullFd=open("/dev/null", O_RDWR);
        if(iNullFd<0)
        {
            printf("%s open /dev/null failed\n",argv[0]);
            break;
        }
        iRetFd=dup2(iNullFd, iStdInFd);
        if(iRetFd<0)
        {
            printf("%s dup2 stdin failed\n",argv[0]);
            break;
        }
        iRetFd=dup2(iNullFd, iStdOutFd);
        iRetFd=dup2(iNullFd, iStdErrFd);
        if(iNullFd>2)
        {
            close(iNullFd);
        }   
        
    }while(false);
#endif

	do
	{
		g_SystemService_sp = QSharedPointer <QSystemService> (new QSystemService(argc, argv));
		if(g_SystemService_sp.isNull())
		{
			ProcessExitCode = SS_PEC_OUT_OF_MEMORY;
			break;
		}
		ProcessExitCode = g_SystemService_sp->exec();
	}while(FALSE);

	if(FALSE == g_SystemService_sp.isNull())
	{
		g_SystemService_sp.clear();
	}

	return ProcessExitCode;
}

