#ifndef	_UNIQUE_ID_H_
#define	_UNIQUE_ID_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

DECLARE_EXTERN_C_BLOCK_START

INT UniqueId_Init();
INT UniqueId_getNextId(P_INT pUniqueId);
INT UniqueId_ReleaseId(const INT iUniqueId);
INT UniqueId_Uninit();

DECLARE_EXTERN_C_BLOCK_END

#endif	//_UNIQUE_ID_H_

