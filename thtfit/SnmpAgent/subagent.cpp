#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <unistd.h>

#include "subagent.h"
#include <BaseTypeDef.h>
#include "DbgLogSwitchDef.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <Compiler.h>

#define TIMEOUT_MS 8000

static int g_InitCallCnt = 0;

const char *ParseUrlFilename(const char *url)
{
    const char *p = strrchr(url, '/');
    if (p) {
        return(p + 1);
    }
    return NULL;
}

const char *stringActieMediaType(ActiveMedia_TYPE type)
{
    const char *string[] = { "Unknown", "Flash", "Usb", "SdCard", "HDD", "RAM", "Tuner", NULL };
    return string[type];
}

int DBus_getProperty(SPBusType busType, const char *pFuncName, SPArgs *pSetArgsExt, int first_arg_type, ...)
{
    int retval = -1;
    DBusMessage *reply = NULL;

    DBusConnection *conn = connectToDBus();
    if (!conn) {
		if(Sw_LogDbusCmd)
		{
			LOG_BLINE("No DbusConn\n");
		}
        goto _error;
    }

	DbusConn_ClearInputMsgQueue(conn);

    const char *dest, *path, *ifc;
    if (busType == SPBus_WebBrowser_DataCommSrv) {
        dest = DBUS_WebBrowser_DataCommSrv_DEST;
        path = DBUS_WebBrowser_DataCommSrv_PATH;
        ifc  = DBUS_WebBrowser_DataCommSrv_IFC;
    } else if (busType == SPBus_ControlCenter_UiSrv) {
        dest = DBUS_ControlCenter_UiSrv_DEST;
        path = DBUS_ControlCenter_UiSrv_PATH;
        ifc  = DBUS_ControlCenter_UiSrv_IFC;
    } else if (busType == SPBus_SystemService_IrInputCtrl) {
        dest = DBUS_SystemService_IrInputCtrl_DEST;
        path = DBUS_SystemService_IrInputCtrl_PATH;
        ifc  = DBUS_SystemService_IrInputCtrl_IFC;
    } else if (busType == SPBus_SystemService_NetworkManager) {
        dest = DBUS_SystemService_NetworkManagerSrv_DEST;
        path = DBUS_SystemService_NetworkManagerSrv_PATH;
        ifc  = DBUS_SystemService_NetworkManagerSrv_IFC;
    } else if (busType == SPBus_MPlayer) {
        dest = DBUS_MPlayer_DEST;
        path = DBUS_MPlayer_PATH;
        ifc  = DBUS_MPlayer_IFC;
    } else if (busType == SPBus_GpioBtnInput) {
        dest = DBUS_GpioBtnInput_DEST;
        path = DBUS_GpioBtnInput_PATH;
        ifc  = DBUS_GpioBtnInput_IFC;
    } else {
        retval = -2;
		//if(Sw_LogDbusCmd)
		{
			LOG_BLINE("Inv BusType(%d)\n", busType);
		}
        goto _error;
    }

    if (!pSetArgsExt) {
        reply = DBusMethodCall(conn, TIMEOUT_MS, dest, path, ifc, pFuncName, DBUS_TYPE_INVALID);
    } else if (pSetArgsExt->eType == SPValue_String) {
        reply = DBusMethodCall(conn, TIMEOUT_MS, dest, path, ifc, pFuncName,
                               DBUS_TYPE_STRING, &pSetArgsExt->pString,
                               DBUS_TYPE_INVALID);
    } else if (pSetArgsExt->eType == SPValue_Integer) {
        reply = DBusMethodCall(conn, TIMEOUT_MS, dest, path, ifc, pFuncName,
                               DBUS_TYPE_INT32, &pSetArgsExt->integer,
                               DBUS_TYPE_INVALID);
    } else {
        retval = -3;
		//if(Sw_LogDbusCmd)
		{
			LOG_BLINE("Inv ArgType(%d)\n", pSetArgsExt->eType);
		}
        goto _error;
    }

    if (reply) {
        va_list lst;
        va_start(lst, first_arg_type);
        if (DBusGetArgsV(reply, first_arg_type, lst)) {
            retval = 0;
        }
        va_end(lst);
    } else {
        retval = -4;
		//if(Sw_LogDbusCmd)
		{
			LOG_BLINE("Reply is NULL\n");
		}
    }

_error:
    DEBUGMSGTL(("DBus", "%s:%s retval=%d\n", __FUNCTION__, pFuncName, retval));
    return retval;
}

int DBus_setProperty(SPBusType busType, const char *pFuncName, SPArgs *pGetArgsExt, int first_arg_type, ...)
{
    int retval = -1;

    DBusConnection *conn = connectToDBus();
    if (!conn) {
        goto _error;
    }

	DbusConn_ClearInputMsgQueue(conn);

    const char *dest, *path, *ifc;
    if (busType == SPBus_WebBrowser_DataCommSrv) {
        dest = DBUS_WebBrowser_DataCommSrv_DEST;
        path = DBUS_WebBrowser_DataCommSrv_PATH;
        ifc  = DBUS_WebBrowser_DataCommSrv_IFC;
    } else if (busType == SPBus_ControlCenter_UiSrv) {
        dest = DBUS_ControlCenter_UiSrv_DEST;
        path = DBUS_ControlCenter_UiSrv_PATH;
        ifc  = DBUS_ControlCenter_UiSrv_IFC;
    } else if (busType == SPBus_SystemService_IrInputCtrl) {
        dest = DBUS_SystemService_IrInputCtrl_DEST;
        path = DBUS_SystemService_IrInputCtrl_PATH;
        ifc  = DBUS_SystemService_IrInputCtrl_IFC;
    } else if (busType == SPBus_SystemService_NetworkManager) {
        dest = DBUS_SystemService_NetworkManagerSrv_DEST;
        path = DBUS_SystemService_NetworkManagerSrv_PATH;
        ifc  = DBUS_SystemService_NetworkManagerSrv_IFC;
    } else if (busType == SPBus_MPlayer) {
        dest = DBUS_MPlayer_DEST;
        path = DBUS_MPlayer_PATH;
        ifc  = DBUS_MPlayer_IFC;
    } else if (busType == SPBus_GpioBtnInput) {
        dest = DBUS_GpioBtnInput_DEST;
        path = DBUS_GpioBtnInput_PATH;
        ifc  = DBUS_GpioBtnInput_IFC;
    } else {
        retval = -2;
        goto _error;
    }

    va_list lst;
    va_start(lst, first_arg_type);
    DBusMessage *reply;
    reply = DBusMethodCallV(conn, TIMEOUT_MS, dest, path, ifc, pFuncName,
                            first_arg_type, lst);
    va_end(lst);

    if (reply) {
        if (!pGetArgsExt) {
            DBusGetArgs(reply, DBUS_TYPE_INT32, &retval, DBUS_TYPE_INVALID);
        } else if (pGetArgsExt->eType == SPValue_Integer) {
            DBusGetArgs(reply, DBUS_TYPE_INT32, &retval, pGetArgsExt->eType, &pGetArgsExt->integer, DBUS_TYPE_INVALID);
        } else if (pGetArgsExt->eType == SPValue_String) {
            DBusGetArgs(reply, DBUS_TYPE_INT32, &retval, pGetArgsExt->eType, &pGetArgsExt->pString, DBUS_TYPE_INVALID);
        } else {
            dbus_message_unref(reply);
            retval = -3;
            goto _error;
        }
    }
_error:
    DEBUGMSGTL(("DBus", "%s:%s retval=%d\n", __FUNCTION__, pFuncName, retval));
    return retval;
}

int init_libSnmpAgent(void)
{
	int iOutRet = 0;
	INT_t iRet = 0;

	C_UNUSED(iRet);
	
	LOG_BLINE("init_libSnmpAgent\n");

	do
	{
		if(0 < g_InitCallCnt)
		{
			g_InitCallCnt++;
			break;
		}
		g_InitCallCnt++;
		
		iRet = RegDbgLogSwitch();
		if(ERROR_SUCCESS == iRet)
		{
		}
		else
		{
			LOG_BLINE("RegDbgLogSwitch err %d\n", iRet);
		}

	    static MP7xxSetting setting;
	    memset(&setting, 0, sizeof(sizeof(MP7xxSetting)));
	    
		init_network();
		init_monitor(&setting.monitor);
	    init_system();
	    init_player();
	    init_gpio();
	    init_media(&setting.media);
	    init_tuner();
	    init_fileListingTable();
        init_channelTable();
	    init_asTrap();
	}while(FALSE);

	return iOutRet;
}

int deinit_libSnmpAgent(void)
{
	int iOutRet = 0;
	INT_t iRet;

	LOG_BLINE("deinit_libSnmpAgent\n");

	do
	{
		g_InitCallCnt--;
		if(0 < g_InitCallCnt)
		{
			break;
		}
		DeinitTrap();
		iRet = MainDbusConn_Cleanup();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		iRet = UnregDbgLogSwitch();
		if(ERROR_SUCCESS == iRet)
		{
		}
		else
		{
			LOG_BLINE("UnregDbgLogSwitch err %d\n", iRet);
		}
	}while(FALSE);
	
	return iOutRet;
}

