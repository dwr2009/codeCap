#ifndef	_DBG_LOG_SWITCH_DEF_H_
#define	_DBG_LOG_SWITCH_DEF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	Sw_LogDbusCmd			(*g_pLogDbusCmdSw)
#define	Sw_LogSysIoEvt			(*g_pLogSysIoEvtSw)
#define	Sw_LogWifiConnection	(*g_pLogWifiConnection)

INT_t RegDbgLogSwitch();
INT_t UnregDbgLogSwitch();

extern BOOL_t * g_pLogDbusCmdSw;
extern BOOL_t * g_pLogSysIoEvtSw;
extern BOOL_t * g_pLogWifiConnection;

#endif	//_DBG_LOG_SWITCH_DEF_H_

