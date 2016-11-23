#ifndef	_FILE_CREATOR_H_
#define	_FILE_CREATOR_H_

#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

INT BufferDataToFile(LPCSTR pszFilePath, PBYTE pBufData, UINT uiDataLen);
INT_t FileDataToBuffer(LPCSTR pszFilePath, PBYTE pBuf, size_t bufSize, size_t * pBytesRead);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_FILE_CREATOR_H_
