#ifndef	_SYSTEM_EVENT_H_
#define	_SYSTEM_EVENT_H_

#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QtCore/qobjectdefs.h>
#include <BaseTypeDef.h>
#include "NetMgrEthernet.h"
#include "NetMgrWifi.h"
#include <NetRouteTable.h>
#include <NetIF.h>
#include <QSettings>
#include <QDBusVariant>
#include <QTimer>
#include "SystemSrvApi.h"

class QSystemEvent;

class QDBusIf_SystemEvent : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.SystemEvent")	
public:
	QDBusIf_SystemEvent(QSystemEvent * pParentObj);
public slots:
	void SyncCacheToPersistentStorage(int & iOutRet);
signals:
	void Evt_SyncCacheToStorage();
private:
	QSystemEvent * m_pSystemEvent;
};

class QSystemEvent : public QObject
{
	Q_OBJECT		
	friend class QDBusIf_SystemEvent;
public:
	QSystemEvent();
	~QSystemEvent();
	void SyncCacheToPersistentStorage(int & iOutRet);
	INT_t InitInstance();
	INT_t ExitInstance();
signals:
	void Evt_SyncCacheToStorage();
private:
	QSharedPointer <QDBusIf_SystemEvent> m_DbusIf_SysEvt_sp;
};

#endif	//_SYSTEM_EVENT_H_

