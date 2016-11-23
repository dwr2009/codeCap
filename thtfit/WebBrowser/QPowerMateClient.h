#ifndef _POWER_MATE_CLIENT_H_
#define _POWER_MATE_CLIENT_H_

#include <QObject>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <BaseTypeDef.h>
#include <QSharedPointer>

#define	SS_DBUS_SERVICE_NAME				    "org.thtfit.SystemService"
#define	DBUS_PATH_PowerMateSrv		    "/org/thtfit/SystemService/PowerMate"
#define	IF_NAME_SysSrv_PowerMateSrv	    "org.thtfit.SystemService.PowerMate"
#define   METHOD_setPowerMateBrightness                   "set_brightness"
#define   METHOD_getPowerMateGlobalControl              "GetEnableGlobalControl"

typedef enum{
	PowerMate_Invalid = 0,
       PowerMate_Open = 1,
       PowerMate_Close = 2,
       PowerMate_End = 3,
}e_PowerMateStatus;


class QPowerMateClient:public QObject
{
    Q_OBJECT
        
public:
    QPowerMateClient();
    virtual ~QPowerMateClient();
    virtual INT_t setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp);
    int getPowerMateGlobalControl(int& pmgbct);

public slots:
    void OnTimer_PrepareDBus();   
    INT_t setPowerMateBrightness(int& volume);
    void onPowerMateOpen();
    void onPowerMateClose();

private:
    
    
private:
    QWeakPointer <QDBusConnection> m_DBusConn_pm;
    QSharedPointer <QDBusInterface> m_DBusIf_PowerMate_sp;
    e_PowerMateStatus m_pmstatus;
    
};

#endif
