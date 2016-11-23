#ifndef	_ENC_DECRYPT_RC4_H_
#define	_ENC_DECRYPT_RC4_H_

#include	<BaseTypeDef.h>

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

INT EncDecrypt_RC4(LPBYTE pInData, INT iInSize, LPBYTE pOutData, INT iOutSize, INT * piOutWritten, 
	const char * pszKey);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_ENC_DECRYPT_RC4_H_

