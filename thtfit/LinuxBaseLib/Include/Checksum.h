#ifndef	_CHECKSUM_H_
#define	_CHECKSUM_H_

#include "BaseTypeDef.h"
#include <CCplusplusBridge.h>

#define	ERR_CHKSUM_BASE					1900
#define	ERR_FILE_OPEN_FAIL				(ERR_CHKSUM_BASE+1)
#define	ERR_FILE_READ_FAIL				(ERR_CHKSUM_BASE+2)
#define	ERR_CALC_FILE_CHKSUM_FAIL		(ERR_CHKSUM_BASE+3)

DECLARE_EXTERN_C_BLOCK_START

UINT32 CheckSum32(LPBYTE pBuf, INT iBufSize);
INT CalcFileChecksum32(const char * pszFilePath, UINT32 * puiChecksumVal);
UINT8_t CalcCrc8(CONST UINT8_t uiInitialCrc8Val, PBYTE pData, CONST size_t sizeData);
	
DECLARE_EXTERN_C_BLOCK_END

#endif	//_CHECKSUM_H_

