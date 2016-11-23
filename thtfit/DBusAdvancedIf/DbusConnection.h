#ifndef	_DBUS_CONNECTION_H_
#define	_DBUS_CONNECTION_H_

#include "DBusMessage.h"

class CDbusConnection
{
public:
	CDbusConnection();
	~CDbusConnection();
	static SharedPtr <CDBusMessage> Call(DBusConnection * pDbusConn, 
		SharedPtr <CDBusMessage> DbusMsg_MethodCall, int iTimeoutMs = -1);
};

#endif	//_DBUS_CONNECTION_H_
