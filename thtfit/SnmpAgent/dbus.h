#ifndef SNMP_DBUS_H
#define SNMP_DBUS_H

#include "snmp_support.h"
#include <dbus/dbus.h>
#include <BaseTypeDef.h>

#define DBUS_WebBrowser_DataCommSrv_DEST    "org.thtfit.WebBrowser"
#define DBUS_WebBrowser_DataCommSrv_PATH    "/org/thtfit/WebBrowser/DataCommSrv"
#define DBUS_WebBrowser_DataCommSrv_IFC     "org.thtfit.WebBrowser.DataCommSrv"

#define DBUS_ControlCenter_UiSrv_DEST       "org.thtfit.ControlCenter"
#define DBUS_ControlCenter_UiSrv_PATH       "/org/thtfit/ControlCenter/UiSrv"
#define DBUS_ControlCenter_UiSrv_IFC        "org.thtfit.ControlCenter.UiSrv"

#define DBUS_SystemService_NetworkManagerSrv_DEST   "org.thtfit.SystemService"
#define DBUS_SystemService_NetworkManagerSrv_PATH   "/org/thtfit/SystemService/NetworkManagerSrv"
#define DBUS_SystemService_NetworkManagerSrv_IFC    "org.thtfit.SystemService.NetworkManagerSrv"

#define DBUS_SystemService_IrInputCtrl_DEST     "org.thtfit.SystemService"
#define DBUS_SystemService_IrInputCtrl_PATH     "/org/thtfit/SystemService/IrInputCtrl"
#define DBUS_SystemService_IrInputCtrl_IFC      "org.thtfit.SystemService.IrInputCtrl"

#define DBUS_MPlayer_DEST   "org.thtfit.MPlayer"
#define DBUS_MPlayer_PATH   "/org/thtfit/MPlayer"
#define DBUS_MPlayer_IFC    "org.thtfit.MPlayer"

#define DBUS_GpioBtnInput_DEST   "org.thtfit.GpioBtnInput"
#define DBUS_GpioBtnInput_PATH   "/org/thtfit/GpioBtnInput"
#define DBUS_GpioBtnInput_IFC    "org.thtfit.GpioBtnInput"

#define LOG_AND_FREE_DBUS_ERROR_WITH_MSG(err, msg) \
    {   DEBUGMSGTL(("DBus", "%s: D-Bus error in %s: %s (%s)\n", __FUNCTION__, \
        dbus_message_get_member((msg)), (err)->name, (err)->message)); \
        dbus_error_free((err)); }
#define LOG_AND_FREE_DBUS_ERROR(err) \
    {   DEBUGMSGTL(("DBus", "%s: D-Bus error: %s (%s)\n", __FUNCTION__, \
        (err)->name, (err)->message)); \
        dbus_error_free((err)); }

#define	DBUS_OBJ_PATH_SNMP_AGENT	"/org/thtfit/SnmpAgent"

#define	DBUS_OBJ_IF_SNMP_AGENT		"org.thtfit.SnmpAgent"

#define	DBUS_SIG_SnmpAgent_WakeupThread	"WakeupThread"

#define	DBUS_CALL_DEFAULT_TIMEOUT_MS	(5*1000)

#ifdef __cplusplus
extern "C" {
#endif

DBusConnection* connectToDBus();
INT_t MainDbusConn_Cleanup();
INT_t SendDbusSig_WakeupThread(void);
VOID DbusConn_ClearInputMsgQueue(DBusConnection * pDbusConn);

DBusMessage * DBusMethodCall(DBusConnection *conn, int timeout_ms, const char *dist,
                             const char *path, const char *ifc, const char *func,
                             int first_arg_type, ...);

dbus_bool_t DBusGetArgs(DBusMessage *message, int first_arg_type, ...);

DBusMessage * DBusMethodCallV(DBusConnection *conn, int timeout_ms, const char *dest,
                              const char *path, const char *ifc, const char *func,
                              int first_arg_type, va_list lst);

dbus_bool_t DBusGetArgsV(DBusMessage *message, int first_arg_type, va_list lst);

#ifdef __cplusplus
}
#endif

#endif /* SNMP_DBUS_H */
