#include "CmdLineArgument.h"

CCmdLineArgument::CCmdLineArgument()
{

}

CCmdLineArgument::CCmdLineArgument(int argc, char * argv[])
{
	int iId;

	for(iId = 1; iId < argc; iId++)
	{
		(*this) << argv[iId];
	}
}

CCmdLineArgument::~CCmdLineArgument()
{

}

CCmdLineArgument & CCmdLineArgument::operator << (LPCSTR pszOneArg)
{
	try
	{
		m_ArgList.push_back((const char*)pszOneArg);
	}
	catch(std::bad_alloc & AllocEx)
	{
	}
	
	return *this;
}

BOOL_t CCmdLineArgument::ExistArg(LPCSTR pszOneArg)
{
	BOOL_t bOutRet = FALSE;
	ARG_LIST::iterator itOneArg;

	for(itOneArg = m_ArgList.begin(); itOneArg != m_ArgList.end(); itOneArg++)
	{
		if((*itOneArg) == pszOneArg)
		{
			bOutRet = TRUE;
			break;
		}
	}

	return bOutRet;
}

CString & CCmdLineArgument::operator[](int nIndex)
{
	ARG_LIST::iterator itArg;

	do
	{
		if(0 > nIndex || nIndex >= (typeof(nIndex))m_ArgList.size())
		{
			return m_strNull;
		}
		itArg = m_ArgList.begin();
		int iIndex = nIndex;
		while(iIndex--)
		{
			itArg++;
		}
		return *itArg;
	}while(FALSE);
}

size_t CCmdLineArgument::getSize()
{
	return m_ArgList.size();
}

