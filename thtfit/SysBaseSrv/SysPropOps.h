#ifndef	_SYS_PROP_OPS_H_
#define	_SYS_PROP_OPS_H_

#include <SharedPtr.h>
#include <map>
#include <String.h>
#include <StackBufString.h>
#include <File.h>
#include <BaseTypeDef.h>

using namespace CppBase;

class CSysPropOps
{
public:
	class CSysPropValue
	{
	public:
		CString m_strValue;
		BOOL_t m_bPersistent;
	public:
		CSysPropValue();
		~CSysPropValue();
	};
	typedef map <CString, SharedPtr <CSysPropValue> > SYS_PROP_KEY_VAL_MAP;
public:
	CSysPropOps();
	~CSysPropOps();
	INT_t Load(BOOL_t bClearCurVal = TRUE);
	INT_t Save();
	INT_t setKeyValPair(LPCSTR pszKey, size_t KeyLen, LPCSTR pszVal, size_t ValueLen, BOOL_t bPersistent);
	INT_t getKeyValPair(LPCSTR pszKey, OUT CStackBufString & strValue);
	INT_t getById(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strPropVal);
	INT_t getCount(OUT INT_t & iPropCount);
	INT_t DelKey(LPCSTR pszKey, size_t KeyLen);
private:
	INT_t Load(LPCSTR pszSysPropDataFilePath, BOOL_t bClearCurVal = TRUE);	
	INT_t BackupSysPropDataFile();
	INT_t RecoverySysPropDataFile();
private:
	SYS_PROP_KEY_VAL_MAP m_SysPropKeyValMap;
	SharedPtr <CFile> m_SysPropDataFile_sp;
};

#endif	//_SYS_PROP_OPS_H_

