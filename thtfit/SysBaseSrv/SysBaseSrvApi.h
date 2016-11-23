#ifndef	_SYS_BASE_SRV_API_H_
#define	_SYS_BASE_SRV_API_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>
#include <StackBufString.h>

#define	SBS_SOCKET_PATH			"/tmp/SysBaseSrvSck"

#define	SYS_DATA_DIR				"/data"
#define	SYS_PROP_DATA_FILE			SYS_DATA_DIR "/SysProp.bin"
#define	SYS_PROP_DATA_BACKUP_FILE	SYS_PROP_DATA_FILE ".bak"

//Message Ids
#define	MSG_ID_SysProp_SetPersistent		1
#define	MSG_ID_SysProp_SetTemp				2
#define	MSG_ID_SysProp_Get					3
#define	MSG_ID_SysProp_Del					4
#define	MSG_ID_SysProp_getCount				5
#define	MSG_ID_SysProp_getById				6

//For C language

DECLARE_EXTERN_C_BLOCK_START

INT_t SysProp_setPersistent(LPCSTR pszKey, LPCSTR pszValue);
INT_t SysProp_setTemp(LPCSTR pszKey, LPCSTR pszValue);
INT_t SysProp_get(LPCSTR pszKey, char * pValueBuf, CONST size_t ValBufSize);
INT_t SysProp_del(LPCSTR pszKey);
INT_t SysProp_getCount(INT_t * CONST piCount);

DECLARE_EXTERN_C_BLOCK_END

//For C++ language

#ifdef	__cplusplus

INT_t SysProp_setPersistent(LPCSTR pszKey, CONST INT_t iVal);
INT_t SysProp_get(LPCSTR pszKey, OUT CStackBufString & strValue);
INT_t SysProp_get(LPCSTR pszKey, OUT CString & strValue);
INT_t SysProp_get(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strValue);

#endif	//__cplusplus

#endif	//_SYS_BASE_SRV_API_H_

