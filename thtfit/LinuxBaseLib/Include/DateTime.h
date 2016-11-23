#ifndef	_DATE_TIME_H_
#define	_DATE_TIME_H_

#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

UINT64_t GetCurTimeMs(void);
UINT64_t GetSysUpTimeSec(VOID);
UINT64_t GetSysUpTimeMs(VOID);
INT64_t getClkMonotonicTimeUs();

DECLARE_EXTERN_C_BLOCK_END

#endif	//_DATE_TIME_H_

