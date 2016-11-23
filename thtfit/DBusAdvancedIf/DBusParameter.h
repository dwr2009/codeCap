#ifndef	_DBUS_PARAMETER_H_
#define	_DBUS_PARAMETER_H_

#include <dbus/dbus.h>
#include <Object.h>
#include <String.h>
#include <SharedPtr.h>
#include <ByteArray2.h>

using namespace CppBase;

class CDBusParameter
{
	DECLARE_RUNTIME_CLASS(CDBusParameter);
public:
	CDBusParameter();
	virtual ~CDBusParameter();
	virtual int getArgType();
	virtual INT32_t toInt32();
	virtual UINT32_t toUint32();
	virtual CString toString();
#if 1/*added by 2012-11-23*/
	virtual SharedPtr <CByteArray2> toByteArray();
#endif
protected:
	int m_ArgType;
};

class CDBusParameter_INT32 : public CDBusParameter
{
	DECLARE_RUNTIME_CLASS(CDBusParameter_INT32);
public:
	CDBusParameter_INT32();
	virtual ~CDBusParameter_INT32();
	virtual int getArgType();
	virtual INT32_t toInt32();
	virtual UINT32_t toUint32();
	virtual CString toString();
	virtual VOID setValue(INT32_t * pValue);
	virtual INT32_t getValue();
	virtual operator INT32_t() CONST;
private:
	INT32_t m_Value;
};

class CDBusParameter_UINT32 : public CDBusParameter
{
	DECLARE_RUNTIME_CLASS(CDBusParameter_UINT32);
public:
	CDBusParameter_UINT32();
	virtual ~CDBusParameter_UINT32();
	virtual int getArgType();
	virtual INT32_t toInt32();
	virtual UINT32_t toUint32();
	virtual CString toString();
	virtual VOID setValue(UINT32_t * pValue);
	virtual UINT32_t getValue();
	virtual operator UINT32_t() CONST;
private:
	UINT32_t m_Value;
};

class CDBusParameter_STRING : public CDBusParameter
{
	DECLARE_RUNTIME_CLASS(CDBusParameter_STRING);
public:
	CDBusParameter_STRING();
	virtual ~CDBusParameter_STRING();
	virtual int getArgType();
	virtual INT32_t toInt32();
	virtual UINT32_t toUint32();
	virtual CString toString();
	virtual VOID setValue(const char * pszValue);
	virtual CString getValue();
	virtual operator LPCSTR() CONST;
private:
	CString m_strValue;
};

#if 1/*added by lxj 2012-11-23*/
class CDBusParameter_ByteArray : public CDBusParameter
{
	DECLARE_RUNTIME_CLASS(CDBusParameter_ByteArray);
public:
	CDBusParameter_ByteArray();
	virtual ~CDBusParameter_ByteArray();
	virtual int getArgType();
	virtual SharedPtr <CByteArray2> toByteArray();
private:
	SharedPtr <CByteArray2> m_spByteArray;
};
#endif
#endif	//_DBUS_PARAMETER_H_

