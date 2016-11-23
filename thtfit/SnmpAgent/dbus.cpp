#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "dbus.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

static DBusConnection* g_pDbusConn = NULL;

DBusConnection* connectToDBus()
{
    if (g_pDbusConn == NULL) {
        DBusError err;
        dbus_error_init(&err);
        g_pDbusConn = dbus_bus_get_private(DBUS_BUS_SYSTEM, &err);
        if (dbus_error_is_set(&err)) {
            DEBUGMSGTL(("DBus", "Could not get onto the system bus!\n"));
            dbus_error_free(&err);
            return NULL;
        }

        dbus_connection_set_exit_on_disconnect(g_pDbusConn, FALSE);
    }

    return g_pDbusConn;
}

/*
ThreadContext: must be MainThread
*/
INT_t MainDbusConn_Cleanup()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == g_pDbusConn)
		{
			break;
		}
		dbus_connection_close(g_pDbusConn);
		dbus_connection_unref(g_pDbusConn);
		g_pDbusConn = NULL;
	}while(FALSE);

	return iOutRet;
}

/*
ThreadContext: must be MainThread
*/
INT_t SendDbusSig_WakeupThread(void)
{
	INT_t iOutRet = ERROR_SUCCESS;
	DBusConnection* pDbusConn_Main = connectToDBus();
	DBusMessage * pDBusMsg = NULL;
	dbus_bool_t bDBusRet;

	do
	{
		if(NULL == pDbusConn_Main)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		pDBusMsg = dbus_message_new_signal(DBUS_OBJ_PATH_SNMP_AGENT, DBUS_OBJ_IF_SNMP_AGENT, 
			DBUS_SIG_SnmpAgent_WakeupThread);
		if(NULL == pDBusMsg)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bDBusRet = dbus_connection_send(pDbusConn_Main, pDBusMsg, NULL);
		if(FALSE == bDBusRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		dbus_connection_flush(pDbusConn_Main);
	}while(FALSE);

	if(pDBusMsg)
	{
		dbus_message_unref(pDBusMsg);
		pDBusMsg = NULL;
	}

	return iOutRet;
}

/*
ThreadContext: must be the one the pDbusConn stays in, main thead typically.
*/
VOID DbusConn_ClearInputMsgQueue(DBusConnection * pDbusConn)
{
 	dbus_bool_t bDBusRet;
	DBusMessage * pDBusMsg = NULL;

	do
	{
		if(NULL == pDbusConn)
		{
			break;
		}
		
		bDBusRet = dbus_connection_read_write(pDbusConn, 0);
		if(FALSE == bDBusRet)	//the connection is closed
		{
			LOG_BLINE("DBusConn disconnect\n");
			break;
		}

		while(TRUE)
		{
			pDBusMsg = dbus_connection_pop_message(pDbusConn);
			if(pDBusMsg)
			{
				/*
				LOG_BLINE("dbus_message,type:%d,path:%s,interface:%s,member:%s,errorname:%s,destination:%s,sender:%s\n", 
					dbus_message_get_type(pDBusMsg), dbus_message_get_path(pDBusMsg),
					dbus_message_get_interface(pDBusMsg), dbus_message_get_member(pDBusMsg),
					dbus_message_get_error_name(pDBusMsg), dbus_message_get_destination(pDBusMsg),
					dbus_message_get_sender(pDBusMsg));
				/*/
 				dbus_message_unref(pDBusMsg);
				pDBusMsg = NULL;
			}
			else	//no more data
			{
				break;
			}
		}
	}while(FALSE);
}

DBusMessage * DBusMethodCall(DBusConnection *conn, int timeout_ms, const char *dest,
                             const char *path, const char *ifc, const char *func,
                             int first_arg_type, ...)
{
    DBusMessage *msg, *reply = NULL;

    msg = dbus_message_new_method_call(dest, path, ifc, func);
    if (msg) {
        DBusError err;
        dbus_error_init(&err);

        va_list lst;
        va_start(lst, first_arg_type);

        if (!dbus_message_append_args_valist(msg, first_arg_type, lst))
            DEBUGMSGTL(("DBus", "Couldn't append argument to method call: '%s'\n", func));
        else
            reply = dbus_connection_send_with_reply_and_block(conn, msg, timeout_ms, &err);

        va_end(lst);
        dbus_message_unref(msg);

        if (!reply) {
            if (dbus_error_is_set(&err)) {
                DEBUGMSGTL(("DBus", "%s: D-Bus error: %s (%s)\n", __FUNCTION__,
                            err.name, err.message));
                dbus_error_free(&err);
            } else
                DEBUGMSGTL(("DBus", "DBus reply is NULL in function %s\n", __FUNCTION__));
        }
    } else {
        DEBUGMSGTL(("DBus", "Couldn't create msg with method: '%s' to service: '%s'\n", func, dest));
    }

    return reply;
}

dbus_bool_t DBusGetArgs(DBusMessage *message, int first_arg_type, ...)
{
    dbus_bool_t retval;
    DBusError err;
    dbus_error_init(&err);

    va_list var_args;
    va_start (var_args, first_arg_type);
    retval = dbus_message_get_args_valist(message, &err, first_arg_type, var_args);
    va_end (var_args);

    if (!retval) {
        if (dbus_error_is_set(&err)) {
            DEBUGMSGTL(("DBus", "%s: D-Bus error: %s (%s)\n", __FUNCTION__, err.name, err.message));
            dbus_error_free(&err);
        } else {
            DEBUGMSGTL(("DBus", "D-Bus error in %s unable to get args.\n", __FUNCTION__));
        }
    }
    dbus_message_unref(message);
    return retval;
}

DBusMessage * DBusMethodCallV(DBusConnection *conn, int timeout_ms, const char *dest,
                              const char *path, const char *ifc, const char *func,
                              int first_arg_type, va_list lst)
{
    DBusMessage *msg, *reply = NULL;

    msg = dbus_message_new_method_call(dest, path, ifc, func);
    if (msg) {
        DBusError err;
        dbus_error_init(&err);

        if (!dbus_message_append_args_valist(msg, first_arg_type, lst))
            DEBUGMSGTL(("DBus", "Couldn't append argument to method call: '%s'\n", func));
        else 
            reply = dbus_connection_send_with_reply_and_block(conn, msg, timeout_ms, &err);

        dbus_message_unref(msg);

        if (!reply) {
            if (dbus_error_is_set(&err)) {
                DEBUGMSGTL(("DBus", "%s: D-Bus error: %s (%s)\n", __FUNCTION__,
                            err.name, err.message));
                dbus_error_free(&err);
            } else
                DEBUGMSGTL(("DBus", "DBus reply is NULL in function %s\n", __FUNCTION__));
        }
    } else {
        DEBUGMSGTL(("DBus", "Couldn't create msg with method: '%s' to service: '%s'\n", func, dest));
    }

    return reply;
}

dbus_bool_t DBusGetArgsV(DBusMessage *message, int first_arg_type, va_list lst)
{
    dbus_bool_t retval;
    DBusError err;
    dbus_error_init(&err);

    retval = dbus_message_get_args_valist(message, &err, first_arg_type, lst);

    if (!retval) {
        if (dbus_error_is_set(&err)) {
            DEBUGMSGTL(("DBus", "%s: D-Bus error: %s (%s)\n", __FUNCTION__, err.name, err.message));
            dbus_error_free(&err);
        } else {
            DEBUGMSGTL(("DBus", "D-Bus error in %s unable to get args.\n", __FUNCTION__));
        }
    }
    dbus_message_unref(message);
    return retval;
}

