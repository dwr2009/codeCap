#ifndef	_DBG_LOG_SWITCH_DEF_H_
#define	_DBG_LOG_SWITCH_DEF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	Sw_LogDbusCmd			(*g_pLogDbusCmdSw)
#define	Sw_LogUinput			(*g_pLogUinputSw)

INT_t RegDbgLogSwitch();
INT_t UnregDbgLogSwitch();

extern BOOL_t * g_pLogDbusCmdSw;
extern BOOL_t * g_pLogUinputSw;

#endif	//_DBG_LOG_SWITCH_DEF_H_

