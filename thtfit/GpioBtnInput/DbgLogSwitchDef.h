#ifndef	_DBG_LOG_SWITCH_DEF_H_
#define	_DBG_LOG_SWITCH_DEF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	Sw_LogDbusCmd				(*g_pLogDbusCmdSw)
#define	Sw_LogMediaFileInfo			(*g_pLogMediaFileInfoSw)
#define	Sw_LogGpioButton			(*g_pLogGpioButton)

INT_t RegDbgLogSwitch();
INT_t UnregDbgLogSwitch();

extern BOOL_t * g_pLogDbusCmdSw;
extern BOOL_t * g_pLogMediaFileInfoSw;
extern BOOL_t * g_pLogGpioButton;

#endif	//_DBG_LOG_SWITCH_DEF_H_

