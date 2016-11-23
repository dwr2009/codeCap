#ifndef	_DBG_LOG_SWITCH_DEF_H_
#define	_DBG_LOG_SWITCH_DEF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	Sw_LogDbusCmd				(*g_pLogDbusCmdSw)
#define	Sw_LogPlaybackStatus		(*g_pLogPlaybackStatus)
#define	Sw_LogJavascript			(*g_pLogJavascript)
#define	Sw_LogGpioButton			(*g_pLogGpioButton)

INT_t RegDbgLogSwitch();
INT_t UnregDbgLogSwitch();

extern BOOL_t * g_pLogDbusCmdSw;
extern BOOL_t * g_pLogPlaybackStatus;
extern BOOL_t * g_pLogJavascript;
extern BOOL_t * g_pLogGpioButton;

#endif	//_DBG_LOG_SWITCH_DEF_H_

