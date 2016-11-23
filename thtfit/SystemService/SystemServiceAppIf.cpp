#include "SystemServiceAppIf.h"
#include <ErrPrintHelper.h>

CSystemServiceAppIf::CSystemServiceAppIf()
{
}

CSystemServiceAppIf::~CSystemServiceAppIf()
{
}

QSharedPointer <QDBusConnection> CSystemServiceAppIf::getDBusConnection()
{
	QSharedPointer <QDBusConnection> DBusConn_sp;
		
	return DBusConn_sp;
}

