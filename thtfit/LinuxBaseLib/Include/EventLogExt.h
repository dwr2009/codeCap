#ifndef EVENT_LOG_EXT_H__
#define EVENT_LOG_EXT_H__

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>
#include <String.h>

DECLARE_EXTERN_C_BLOCK_START
INT_t InitEventLog();
INT_t WriteEventLog(CString::STD_STRING_LIST & StrList);
INT_t UnInitEventLog();
INT_t StopWriteEventLog();
INT_t ReInitEventLog();
DECLARE_EXTERN_C_BLOCK_END
	
#endif	//EVENT_LOG_EXT_H__

