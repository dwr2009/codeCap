#ifndef	_URL_INFO_H_
#define	_URL_INFO_H_

#include <BaseTypeDef.h>
#include <String.h>
#include <StackBufString.h>

class CUrlInfo
{
public:
	CUrlInfo();
	CUrlInfo(LPCSTR pszUrl);
	virtual ~CUrlInfo();
	virtual INT_t ParseUrl(LPCSTR pszUrl);
	virtual LPCSTR getExtensionName();
	virtual CString & Filename();
private:
	CString m_strOrigUrl;
	CString m_strExtensionName;
	CString m_strFilename;
};

#endif	//_URL_INFO_H_

