#ifndef	_DBUS_MESSAGE_H_
#define	_DBUS_MESSAGE_H_

#include <BaseTypeDef.h>
#include <dbus/dbus.h>
#include <SharedPtr.h>
#include "DBusParameterList.h"

using namespace CppBase;

class CDBusParameterList;

class CDBusMessage
{
public:
	CDBusMessage();
	virtual ~CDBusMessage();
	static INT_t SendErrReply(DBusConnection * pDBusConn, DBusMessage * pInDBusMsg,
		LPCSTR pszErrName, LPCSTR pszErrMsg = NULL, BOOL_t bFlush = FALSE);
	static SharedPtr <CDBusMessage> createMethodCall(LPCSTR pszService, LPCSTR pszPath,
		LPCSTR pszInterface, LPCSTR pszMethod);
	INT_t AppendArgs(CDBusParameterList & DbusParamList);
	operator DBusMessage *() const;
	INT_t Attach(DBusMessage * pDbusMsg);
private:
	DBusMessage * m_pDbusMsg;
};

#endif	//_DBUS_MESSAGE_H_

