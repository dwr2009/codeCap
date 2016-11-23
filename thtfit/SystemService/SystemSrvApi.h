#ifndef	_SYSTEM_SRV_API_H_
#define	_SYSTEM_SRV_API_H_

#include <BaseTypeDef.h>

#define	DNS_RESOLV_CONF_FILE_PATH		"/etc/resolv.conf"

#define	PREFERRED_ETH_DEV_gphy		"gphy"
#define	PREFERRED_ETH_DEV_eth0		"eth0"

#define	PARAM_KEY_ActionScope			"ActionScope"

#define	PARAM_VALUE_Current				"Current"
#define	PARAM_VALUE_SavedCfg			"SavedCfg"

#define	SNMPD_CONF_FILE_PATH			"/etc/snmp/snmpd.conf"
#define	SNMPD_DEFAULT_PASSWORD			"CeLabsSnmpd"

// DBus service name
#define	SS_DBUS_SERVICE_NAME			"org.thtfit.SystemService"

// DBus path
#define	DBUS_PATH_SysSrv_SysEvt			  "/org/thtfit/SystemService/SystemEvent"
#define	DBUS_PATH_SysSrv_IrInputCtrl	  "/org/thtfit/SystemService/IrInputCtrl"
#define	DBUS_PATH_SysSrv_CfgMgr			  "/org/thtfit/SystemService/CfgMgr"
#define	DBUS_PATH_NetworkManagerSrv_Wifi  "/org/thtfit/SystemService/NetworkManagerSrv/Wifi"
#define DBUS_PATH_SysSrv_PowerMate        "/org/thtfit/SystemService/PowerMate" //added by lxj 2014-3-4
#define	DBUS_PATH_SysSrv_HwWatchDog		"/org/thtfit/SystemService/HwWatchDog"
#define	DBUS_PATH_NetworkManagerSrv_Ethernet		"/org/thtfit/SystemService/NetworkManagerSrv/Ethernet"
#define	DBUS_PATH_NetworkManagerSrv		"/org/thtfit/SystemService/NetworkManagerSrv"



//DBus interfaces
#define	DbusIf_SysSrv_SysEvt			"org.thtfit.SystemService.SystemEvent"
#define	DbusIf_SysSrv_CfgMgr			"org.thtfit.SystemService.CfgMgr"
#define	DbusIf_SysSrv_HwWatchDog		"org.thtfit.SystemService.HwWatchDog"
#define DbusIf_SysSrv_PowerMate         "org.thtfit.SystemService.PowerMate" //added by lxj 2014-3-4
#define	DbusIf_SysSrv_NetMgrSrv_Ethernet "org.thtfit.SystemService.NetworkManagerSrv.Ethernet"
#define DbusIf_SysSrv_NetMgrSrv         "org.thtfit.SystemService.NetworkManagerSrv"


//Methods
#define	METHOD_SyncCacheToStorage		"SyncCacheToPersistentStorage"
#define	METHOD_FsMountUmount			"FsMountUmount"

#define	FS_ACTION_Mounted				"Mounted"
#define	FS_ACTION_Unmounted				"Unmounted"
#define	FS_ACTION_MountPointRemoved		"MountPointRemoved"

//Signals
#define	SIGNAL_Evt_SyncCacheToStorage	        "Evt_SyncCacheToStorage"
#define	SIGNAL_Evt_DhcpDone				        "DhcpDone"
#define	SIGNAL_Evt_DhcpDone2			        "DhcpDone2"
#define	SIGNAL_Evt_FsMountUmount		        "FsMountUmount"
#define	SigName_FsMountPointSymLink		        "FsMountPointSymLink"
#define	SigName_FsMountPointSymUnlink	        "FsMountPointSymUnlink"
#if 1/*added by lxj 2012-12-7*/
#define SIGNAL_Evt_TouchScreenCalibrationDone   "TouchScreenCalibrationDone"
#endif

//SIGNAL,SLOT
#define SigName_Evt_SyncCacheToStorage	Evt_SyncCacheToStorage

//Program names
#define	PROGRAM_NAME_UINPUT				"uinput"

//UINPUT
#define	KEY_UINPUT_INPUT_ENABLE			"UInput/InputEnalbe"

#define	PROG_NAME_WpaSupplicant			"wpa_supplicant"

#define	WpaSupplicant_INTERFACE_DIR_PATH	"/var/run/wpa_supplicant"
#define	PROG_NAME_dhcpc			"udhcpc"

namespace SystemSrv
{

typedef enum
{
	NetCfg_ActionScope_UNKNOWN,
	NetCfg_ActionScope_CURRENT,
	NetCfg_ActionScope_SavedCfg,
}NetCfg_ActionScope_T, *P_NetCfg_ActionScope_T;

NetCfg_ActionScope_T getNetCfgActionScopeFromDesc(LPCSTR pszNetCfgActionScopeDesc);

typedef enum
{
	WpaState_DISCONNECTED,
	WpaState_CONNECTED,
}WPA_STATE, *P_WPA_STATE;

}

#endif	//_SYSTEM_SRV_API_H_

