#ifndef	_DBUS_PARAMETER_LIST_H_
#define	_DBUS_PARAMETER_LIST_H_

#include "DBusParameter.h"
#include <list>
#include <SharedPtr.h>
#include "DBusMessage.h"
#include <StackBufString.h>

using namespace std;
using namespace CppBase;

class CDBusMessage;

typedef list < SharedPtr<CDBusParameter> > DBUS_PARAMETER_LIST;

class CDBusParameterList
{
public:
	CDBusParameterList();
	~CDBusParameterList();
	size_t getSize();
	INT_t ConvertFromDBusMsg(DBusMessage * pDBusMsg);
	CDBusParameterList & operator << (DBusMessage * pDBusMsg);
	INT_t AddParameter(LPCSTR pszParam);
	INT_t AddParameter(INT32_t iParam);
	INT_t AddParameter(UINT32_t uiParam);
	CDBusParameterList & operator << (LPCSTR pszParam);
	CDBusParameterList & operator << (CONST CString & strParam);
	CDBusParameterList & operator << (CONST CStackBufString & strParam);
	CDBusParameterList & operator << (INT32_t iParam);
	CDBusParameterList & operator << (UINT32_t uiParam);
	INT_t AppendToDBusMsg(DBusMessage * pDBusMsg);
	CDBusParameterList & operator >> (DBusMessage * pDBusMsg);
	INT_t operator >> (SharedPtr <CDBusMessage> DbusMsg_sp);
	SharedPtr<CDBusParameter> operator [](int iIndex);
	INT_t addParameter(SharedPtr<CDBusParameter> & DBusParam_sp);
#if 1/*added by lxj 2012-11-23*/
	INT_t addParameter(SharedPtr<CDBusParameter_ByteArray> & DBusParam_sp);
	INT_t addParameter_ByteArray(PBYTE pData, size_t sizeData);
	CDBusParameterList & operator << (SharedPtr <CDBusParameter_ByteArray> & spByteArray);
#endif
private:
	DBUS_PARAMETER_LIST m_DBusParamList;
};

#endif	//DBUS_TYPE_INVALID

