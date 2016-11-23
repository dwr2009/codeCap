#ifndef	_SYSTEM_SERVICE_APP_H_
#define	_SYSTEM_SERVICE_APP_H_

#include <QtWidgets/QApplication>
#include <QObject>
#include <BaseTypeDef.h>
#include <QSharedPointer>
#include <QDBusConnection>
#include "NetworkManagerSrv.h"
#include "DBusInterfaceDef.h"
#include "SystemServiceAppIf.h"
#include <QTimer>
#include "SystemEvent.h"
#include "IrInputCtrl.h"
#include <BackendTransactionThread.h>
#include <HwWatchDog.h>

#include "PowerMate.h" //added by lxj 2014-3-4

// ProcessExitCodes
#define	SS_PEC_OUT_OF_MEMORY			1
#define	SS_PEC_INIT_INSTANCE_FAIL		2
#define	SS_PEC_EXIT_INSTANCE_FAIL		3

#define	SysSrv_SETTINGS_REL_PATH		"Config/SystemService.ini"

class QSystemService : public QApplication, public CSystemServiceAppIf
{
	Q_OBJECT
		
public:
	QSystemService(int argc, char * argv[]);
	virtual ~QSystemService();
	static VOID SigHandler(VOID * pCbContext, int SigNo);
	INT_t InitInstance();
	INT_t ExitInstance();
	int exec();
	virtual QSharedPointer <QDBusConnection> getDBusConnection();
	virtual QSharedPointer <QSettings> getSysSrvSettingsObj();
	virtual CMp7xxGlobalStatus & getMp7xxGlobalStatusObj();
public slots:
	int On_NewSysCfgIsReady(const QString & SysCfgDirPath);
	void On_DhcpDone2(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
		const QString & strGatewayAddr,const QString & strDns);
private:
	VOID OnLinuxSignal(int SigNo);
private:
	Qt::HANDLE m_MainThreadId;
	QSharedPointer <QDBusConnection> m_DBusConn_sp;
	QSharedPointer <QNetworkManagerSrv> m_NetworkManagerSrv_sp;
	QSharedPointer <QSettings> m_SysSrvSettings_sp;
	QSharedPointer <QStringList> m_CmdLineParamList_sp;
	QSharedPointer <QSystemEvent> m_SystemEvent_sp;
	QSharedPointer <QIrInputCtrl> m_IrInputCtrl_sp;
	QSharedPointer <QHwWatchDog> m_HwWatchDog_sp;
	QSharedPointer <QBackendTransactionThread> m_BackendTransactionThread_sp;
	QSharedPointer <QPowerMate> m_PowerMate_sp;//added by lxj 2014-3-4
	CMp7xxGlobalStatus m_Mp7xxGlobalStatusData;
	QString m_sDefaultEthDevName;
};

#endif	//_SYSTEM_SERVICE_APP_H_

