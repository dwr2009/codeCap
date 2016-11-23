#include "DbusConnection.h"

CDbusConnection::CDbusConnection()
{
}

CDbusConnection::~CDbusConnection()
{
}

SharedPtr <CDBusMessage> CDbusConnection::Call(DBusConnection * pDbusConn, 
	SharedPtr <CDBusMessage> DbusMsg_MethodCall, int iTimeoutMs/* = -1*/)
{
	SharedPtr <CDBusMessage> DbusMsgReply_sp(new CDBusMessage);
	INT_t iOutRet = ERROR_SUCCESS;
	DBusMessage * pDbusMsgReply = NULL;
	DBusError DbusErr;

	dbus_error_init(&DbusErr);

	do
	{
		if(NULL == pDbusConn)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		pDbusMsgReply = dbus_connection_send_with_reply_and_block(pDbusConn, (*DbusMsg_MethodCall),
			iTimeoutMs, &DbusErr);
		if(dbus_error_is_set(&DbusErr))
		{
			dbus_error_free(&DbusErr);
			DbusMsgReply_sp.Clear();
			break;
		}
		if(NULL == pDbusMsgReply)
		{
			DbusMsgReply_sp.Clear();
			break;
		}
		iOutRet = DbusMsgReply_sp->Attach(pDbusMsgReply);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		pDbusMsgReply = NULL;	//detach
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		DbusMsgReply_sp.Clear();
	}

	if(pDbusMsgReply)
	{
		dbus_message_unref(pDbusMsgReply);
		pDbusMsgReply = NULL;
	}

	return DbusMsgReply_sp;
}

