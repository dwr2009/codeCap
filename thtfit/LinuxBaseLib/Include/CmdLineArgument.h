#ifndef	_CMD_LINE_ARGUMENT_H_
#define	_CMD_LINE_ARGUMENT_H_

#include <BaseTypeDef.h>
#include <String.h>
#include <list>

using namespace std;

class CCmdLineArgument
{
public:
	typedef list <CString> ARG_LIST;
public:
	CCmdLineArgument();
	CCmdLineArgument(int argc, char * argv[]);
	virtual ~CCmdLineArgument();
	virtual CCmdLineArgument & operator << (LPCSTR pszOneArg);
	virtual BOOL_t ExistArg(LPCSTR pszOneArg);
	virtual CString & operator[](int nIndex);
	virtual size_t getSize();
protected:
private:
	ARG_LIST m_ArgList;
	CString m_strNull;
};

#endif	//_CMD_LINE_ARGUMENT_H_

