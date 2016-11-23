#ifndef	_DBG_LOG_SWITCH_DEF_H_
#define	_DBG_LOG_SWITCH_DEF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	Sw_LogDbusCmd				(*g_pLogDbusCmdSw)
#define	Sw_LogMediaFileInfo			(*g_pLogMediaFileInfoSw)
#define	Sw_LogPlaybackStatus		(*g_pLogPlaybackStatus)
#define	Sw_LogVideoOutputInfo		(*g_pLogVideoOutputInfo)
#define	Sw_LogVideoSync				(*g_pLogVideoSync)
#define	Sw_LogTuner					(*g_pLogTuner)

INT_t RegDbgLogSwitch();
INT_t UnregDbgLogSwitch();

extern BOOL_t * g_pLogDbusCmdSw;
extern BOOL_t * g_pLogMediaFileInfoSw;
extern BOOL_t * g_pLogPlaybackStatus;
extern BOOL_t * g_pLogVideoOutputInfo;
extern BOOL_t * g_pLogVideoSync;
extern BOOL_t * g_pLogTuner;

#endif	//_DBG_LOG_SWITCH_DEF_H_

