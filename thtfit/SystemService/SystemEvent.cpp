#include "SystemEvent.h"
#include "SystemServiceAppIf.h"
#include <QtWidgets/QApplication>
#include "SystemSrvApi.h"
#include <QDBusError>
#include <unistd.h>

QDBusIf_SystemEvent::QDBusIf_SystemEvent(QSystemEvent * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pSystemEvent = pParentObj;	
	setAutoRelaySignals(true);
}

void QDBusIf_SystemEvent::SyncCacheToPersistentStorage(int & iOutRet)
{
	if(m_pSystemEvent)
	{
		m_pSystemEvent->SyncCacheToPersistentStorage(iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

QSystemEvent::QSystemEvent()
{

}

QSystemEvent::~QSystemEvent()
{
	m_DbusIf_SysEvt_sp.clear();
}

void QSystemEvent::SyncCacheToPersistentStorage(int & iOutRet)
{
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

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
		emit Evt_SyncCacheToStorage();
		SysSrvSettingsObj_sp->sync();
		sync();
	}while(FALSE);
}

INT_t QSystemEvent::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	BOOL_t bRet;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		m_DbusIf_SysEvt_sp = QSharedPointer <QDBusIf_SystemEvent>(new QDBusIf_SystemEvent(this));
		if(m_DbusIf_SysEvt_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = DBusConn_sp->registerObject(DBUS_PATH_SysSrv_SysEvt, this);
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
				LOG_BLINE("Failed to reg dbus obj %s\n", DBUS_PATH_SysSrv_SysEvt);
			}
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QSystemEvent::ExitInstance()
{	
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_SysSrv_SysEvt); 		
		}
	}while(FALSE);
	
	return iOutRet;
}

