#ifndef	_STRING2_H_
#define	_STRING2_H_

#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

void TruncTailingNewline(char * pszSrc);
LPSTR GetNextPosForSscanf(LPCSTR pszSrc);
void CharToUpper(char * pcSrc);
void CharToLower(char * pcSrc);
void StrToUpper(char * pszSrc);
void StrToLower(char * pszSrc);
int StrIsDigit(const char * pszStr);
BOOL_t IsDigitString(LPCSTR pszStr);
int strcmp_safe(const char *s1, const char *s2);
int strcasecmp_safe(const char *s1, const char *s2);
int safe_strncmp(const char *s1, const char *s2, size_t n);
int safe_strncasecmp(const char *s1, const char *s2, size_t n);
size_t safe_strlen(LPCSTR pszSrc);
const char * StrChr(const char * pszSearchIn, const char cToBeSearched, int nSrchCnt);
char * safe_strchr(const char *s, int c);
void TrimRight(char * pszSrc);
int TrimLeft(char * pszSrc);
PBYTE memchrWithCnt(const PBYTE pSrc, BYTE ucCharToSearch, UINT uiMaxBytesCnt, P_INT piStatCnt);
PBYTE MemSearchMem(const PBYTE pSrcBuf, const INT iSrcBufLen, const PBYTE pToSearchBuf, const INT iToSearchBufLen);

DECLARE_EXTERN_C_BLOCK_END
	
#endif	//_STRING2_H_

