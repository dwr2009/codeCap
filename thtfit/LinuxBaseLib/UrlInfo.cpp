#include <UrlInfo.h>

CUrlInfo::CUrlInfo()
{
}

CUrlInfo::CUrlInfo(LPCSTR pszUrl)
{
	m_strOrigUrl = pszUrl;
	ParseUrl(m_strOrigUrl);
}

CUrlInfo::~CUrlInfo()
{
}

INT_t CUrlInfo::ParseUrl(LPCSTR pszUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszRet = NULL;

	do
	{
		pszRet = strrchr(pszUrl, '.');
		if(pszRet)
		{
			m_strExtensionName = (pszRet + 1);
		}
		else
		{
			m_strExtensionName = "";
		}
		//get the filename
		{
			LPCSTR pszSlashPos, pszQuestionMarkPos;
			pszSlashPos = strrchr(pszUrl, '/');
			if(pszSlashPos)
			{
				pszQuestionMarkPos = strchr(pszSlashPos+1, '?');
			}
			else
			{
				pszQuestionMarkPos = strchr(pszUrl, '?');
			}
			if(pszSlashPos)
			{
				if(pszQuestionMarkPos)
				{
					iRet = m_strFilename.setContent((LPCSTR)(pszSlashPos+1), (size_t)(pszQuestionMarkPos-(pszSlashPos+1)));
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
				}
				else	//no '?'
				{
					m_strFilename = pszSlashPos+1;
				}
			}
			else	//no '/'
			{
				if(pszQuestionMarkPos)
				{
					iRet = m_strFilename.setContent(pszUrl, pszQuestionMarkPos-pszUrl);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
				}
				else	//no '?'
				{
					m_strFilename = pszUrl;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

LPCSTR CUrlInfo::getExtensionName()
{
	return m_strExtensionName;
}

CString & CUrlInfo::Filename()
{
	return m_strFilename;
}

