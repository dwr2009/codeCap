#ifndef	_THREAD_SPECIFIC_H_
#define	_THREAD_SPECIFIC_H_

#include <pthread.h>
#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

typedef struct tagTHREAD_SPECIFIC_DICTIONARY THREAD_SPECIFIC_DICTIONARY;
typedef THREAD_SPECIFIC_DICTIONARY * P_THREAD_SPECIFIC_DICTIONARY;
	
INT ThrdSpecificDict_Init();
INT ThrdSpecificDict_Uninit();
INT ThrdSpecificDict_setKeyValue(const UINT uiKey, const PVOID pData);
INT ThrdSpecificDict_getValueFromKey(const UINT uiKey, PVOID * ppData);
INT ThrdSpecificDict_ReleaseKey(const UINT uiKey);
INT ThrdSpecificDict_DestroySelfThrdData();

DECLARE_EXTERN_C_BLOCK_END

#endif	//_THREAD_SPECIFIC_H_

