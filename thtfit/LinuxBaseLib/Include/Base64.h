#ifndef	_BASE64_H_
#define	_BASE64_H_

#include	<BaseTypeDef.h>

// 从双字中取单字节
#define BYTE0(a) (a & 0xFF)
#define B1(a) (a >> 8 & 0xFF)
#define B2(a) (a >> 16 & 0xFF)
#define B3(a) (a >> 24 & 0xFF)

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

char GetB64Char(int index);
int GetB64Index(char ch);
INT Base64Encode(CONST LPBYTE pInBuf, INT iInSize, LPBYTE pOutBuf, INT * piOutSize);
INT Base64Decode(CONST LPBYTE pInBuf, INT iInSize, LPBYTE pOutBuf, INT * piOutSize);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_BASE64_H_

