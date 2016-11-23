#ifndef	_STRING_H_
#define	_STRING_H_

#include <BaseTypeDef.h>
#include <Array.h>
#include <list>

class CStackBufString;
class CString;
using namespace std;

CString operator+(const CString & str1, LPCSTR pszStr2);

class CString
{
public:
	typedef enum
	{		
		CaseInsensitive,
		CaseSensitive,
	}CaseSensitivity;
	typedef enum
	{
		IgnoreEmptyParts,
		KeepEmptyParts,
	}SplitOptions;
	typedef list <CString> STD_STRING_LIST;
public:
	CString();
	CString(const char * pszSrc);
	CString(const char * & pszSrc);
	CString(const CString & oStringSrc);
	CString(LPCSTR pszSrc, size_t SrcSizeToFetch);
	CString(INT_t iSrc);
	VOID CommonConstruct();
	virtual ~CString();
	void FreeMem();
	int SetStrLength(int iLen);
	int GetStrLength() const;
	int GetBufLength();
	const CString & operator =(LPCSTR pSrc);
	const CString & operator =(char * pSrc);
	const CString & operator =(const CString & oStringSrc);
	const CString & operator = (const CStackBufString & strSrc);
	const CString & operator =(const char cChar);
	const CString & operator +=(const char cChar);
	friend CString operator+(const CString & str1, LPCSTR pszStr2);
	bool operator ==(const CString & str1);
	bool operator !=(const CString & str1);
	bool operator ==(const char * pszStr2);
	bool operator !=(const char * pszStr2);
	bool Less(CONST CString & str2) CONST;
	operator const char *() const;
	int SetContent(const char cChar, const int iCharCnt);
	INT_t setContent(LPCSTR pszSrc, size_t SrcSizeToFetch);
	virtual INT SetStr(LPCSTR pszSrc, BOOL_t bRemaingMemIfPossible = FALSE);
	// return pointer to const string
	char * GetBufPtr();
	void Attach(char * pszSrc);
	char * Detach();
	virtual int InsertAt(int iIndex, char cInChar);
	virtual int RemoveAt(int iIndex);
	virtual int Replace(int iIndex, char cInChar);
	int GetLastErrNo();
	virtual BOOL_t Equal(LPCSTR pszStr1, CString::CaseSensitivity eCaseSensitivity = CString::CaseSensitive);
	virtual BOOL_t Equal(LPCSTR pszStr1, UINT uiBytesToCmp, CString::CaseSensitivity eCaseSensitivity = CString::CaseSensitive);
	virtual BOOL_t Equal(CString & Str1);
	virtual LPCSTR SearchStr(LPCSTR pStr1, UINT uiStartOff);
	CString::STD_STRING_LIST Split(char DeliminatedChar, CString::SplitOptions eSplitOptions = KeepEmptyParts);
	INT_t toInt();
	char operator[](int iIndex);
private:
	char * m_pBuf;
	int m_iLastErrNo;
	int m_iBufLength;
	BOOL_t m_bStrLenChanged;
	INT m_iCachedStrLen;
};

typedef CArray <CString, CString &> CStringArray;

bool operator < (CONST CString & str1, CONST CString & str2);

#endif	//_STRING_H_

