#ifndef	_SYSTEM_SERVICE_APP_IF_H_
#define	_SYSTEM_SERVICE_APP_IF_H_

#include <QSharedPointer>
#include <QDBusConnection>
#include <QSettings>
#include <Mp7xxGlobalStatus.h>
#include <QString>

class CSystemServiceAppIf
{
public:
	CSystemServiceAppIf();
	virtual ~CSystemServiceAppIf();
	virtual QSharedPointer <QDBusConnection> getDBusConnection();
	virtual QSharedPointer <QSettings> getSysSrvSettingsObj() = 0;
	virtual CMp7xxGlobalStatus & getMp7xxGlobalStatusObj() = 0;
};

#endif	//_SYSTEM_SERVICE_APP_IF_H_

