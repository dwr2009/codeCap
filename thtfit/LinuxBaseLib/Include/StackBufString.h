#ifndef	_STACK_BUF_STRING_H_
#define	_STACK_BUF_STRING_H_

#include <BaseTypeDef.h>
#include <stdarg.h>
#include <list>

using namespace std;

#include <String.h>

class CStackBufString
{
public:
	typedef struct
	{
		int iFieldsScanned;
	}SCANF_PARAM, *P_SCANF_PARAM;
	typedef enum
	{
		IgnoreEmptyParts,
		KeepEmptyParts,
	}SplitOptions;
public:
	CStackBufString(char * pBuf, INT iBufSize);
	virtual ~CStackBufString();
	virtual CStackBufString & operator =(LPCSTR pSrc);
	virtual CStackBufString & operator= (CString & strSrc);
	virtual INT setContent(const char cChar, const int iCharCnt);
	virtual INT_t setContent(LPCSTR pszSrc, size_t SizeToFetch);
	virtual INT Format(LPCSTR pszFmt, ...);
	virtual operator LPCSTR() const;
	virtual operator INT_t() const;
	virtual BOOL_t Equal(LPCSTR pszStr1);
	BOOL_t Equal(LPCSTR pszStr1, size_t SizeToCmp);
	virtual BOOL_t NotEqual(LPCSTR pszStr1);
	virtual LPSTR get_BufPtr();
	virtual LPCSTR getConstData() CONST;
	virtual INT get_BufSize();
	virtual INT get_LastErrNo();
	UINT getStringLength();
	const CStackBufString & operator +=(LPCSTR pszAdded);
	virtual void SafeEndNullChar(INT iDataLen = -1);
	static VOID ScanfParam_Init(P_SCANF_PARAM pScanfParam);
	virtual INT Scanf(OUT SCANF_PARAM & oScanfParam, LPCSTR pszFmt, ...);
	CString::STD_STRING_LIST Split(char DeliminatedChar, CStackBufString::SplitOptions eSplitOptions = KeepEmptyParts);
	virtual BOOL_t toBool();
protected:
	char * m_pszBuf;
	INT m_iBufSize;
	INT m_iLastErrNo;
	BOOL_t m_bStrLenMaybeChanged;
	INT m_iCachedStrLen;
};

#define	DECLARE_CLS_STACK_BUF_STRING(ClsName,BufSize)	char sz##ClsName[BufSize]; CStackBufString ClsName(sz##ClsName, BufSize);

//bool operator == (CStackBufString & str1, LPCSTR pszStr2);

#endif	//_STACK_BUF_STRING_H_

