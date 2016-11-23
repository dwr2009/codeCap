#ifndef	_DBG_LOG_SWITCH_H_
#define	_DBG_LOG_SWITCH_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

DECLARE_EXTERN_C_BLOCK_START

INT_t DbgLogSwitch_Init(void);
BOOL_t * DbgLogSwitch_RegSwitch(LPCSTR pszSwitchDesc);
INT_t DbgLogSwitch_getSwitchCount();
INT_t DbgLogSwitch_getSwitchInfo(INT_t iSwitchId, BOOL_t * pbEnable, LPCSTR * ppszDesc);
INT_t DbgLogSwitch_setSwitch(INT_t iSwitchId, BOOL_t bEnable);
INT_t DbgLogSwitch_Uninit(void);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_DBG_LOG_SWITCH_H_

