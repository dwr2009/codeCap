#ifndef	_SOFT_FLOAT_BRIDGE_H_
#define	_SOFT_FLOAT_BRIDGE_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

DECLARE_EXTERN_C_BLOCK_START

UINT32_t SoftFloatBridge_Add2(UINT32_t uiA, UINT32_t uiB);
UINT32_t SoftFloatBridge_ToInt(UINT32_t uiA);
UINT32_t SoftFloatBridge_IntToFloat(UINT32_t uiA);
VOID SoftFloatBridge_FloatToDouble(UINT32_t uiA, UINT64_t * fDest);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_SOFT_FLOAT_BRIDGE_H_

