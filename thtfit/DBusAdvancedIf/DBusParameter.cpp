#include "DBusParameter.h"
#include <stdlib.h>

//CDBusParameter
BEGIN_IMPL_RUNTIME_CLASS(CDBusParameter);
END_IMPL_RUNTIME_CLASS();

CDBusParameter::CDBusParameter()
{
	m_ArgType = DBUS_TYPE_INVALID;
}

CDBusParameter::~CDBusParameter()
{

}

int CDBusParameter::getArgType()
{
	return m_ArgType;
}

INT32_t CDBusParameter::toInt32()
{
	return 0;
}

UINT32_t CDBusParameter::toUint32()
{
	return 0;
}

CString CDBusParameter::toString()
{
	return CString();
}

#if 1/*added by lxj 2012-11-23*/
SharedPtr <CByteArray2> CDBusParameter::toByteArray()
{
	return (SharedPtr <CByteArray2>());
}
#endif

//CDBusParameter_INT32
BEGIN_IMPL_RUNTIME_CLASS(CDBusParameter_INT32);
BASE_IMPL_RUNTIME_CLASS(CDBusParameter);
END_IMPL_RUNTIME_CLASS();

CDBusParameter_INT32::CDBusParameter_INT32()
{
	m_ArgType = DBUS_TYPE_INT32;
}

CDBusParameter_INT32::~CDBusParameter_INT32()
{
}

int CDBusParameter_INT32::getArgType()
{
	return m_ArgType;
}

INT32_t CDBusParameter_INT32::toInt32()
{
	return m_Value;
}

UINT32_t CDBusParameter_INT32::toUint32()
{
	return m_Value;
}

CString CDBusParameter_INT32::toString()
{
	INT_t iRet;
	char szBuf[32];

	szBuf[0] = '\0';
	iRet = snprintf(szBuf, sizeof(szBuf), "%d", m_Value);
	szBuf[sizeof(szBuf)-1] = '\0';

	return szBuf;
}

VOID CDBusParameter_INT32::setValue(INT32_t * pValue)
{
	if(pValue)
	{
		m_Value = *pValue;
	}
	else
	{
		m_Value = 0;
	}
}

INT32_t CDBusParameter_INT32::getValue()
{
	return m_Value;
}

CDBusParameter_INT32::operator INT32_t() CONST
{
	return m_Value;
}


//CDBusParameter_UINT32
BEGIN_IMPL_RUNTIME_CLASS(CDBusParameter_UINT32);
BASE_IMPL_RUNTIME_CLASS(CDBusParameter);
END_IMPL_RUNTIME_CLASS();

CDBusParameter_UINT32::CDBusParameter_UINT32()
{
	m_ArgType = DBUS_TYPE_UINT32;
}

CDBusParameter_UINT32::~CDBusParameter_UINT32()
{
}

int CDBusParameter_UINT32::getArgType()
{
	return m_ArgType;
}

INT32_t CDBusParameter_UINT32::toInt32()
{
	return m_Value;
}

UINT32_t CDBusParameter_UINT32::toUint32()
{
	return m_Value;
}

CString CDBusParameter_UINT32::toString()
{
	INT_t iRet;
	char szBuf[32];

	szBuf[0] = '\0';
	iRet = snprintf(szBuf, sizeof(szBuf), "%u", m_Value);
	szBuf[sizeof(szBuf)-1] = '\0';

	return szBuf;
}

VOID CDBusParameter_UINT32::setValue(UINT32_t * pValue)
{
	if(pValue)
	{
		m_Value = *pValue;
	}
	else
	{
		m_Value = 0;
	}
}

UINT32_t CDBusParameter_UINT32::getValue()
{
	return m_Value;
}

CDBusParameter_UINT32::operator UINT32_t() CONST
{
	return m_Value;
}


//CDBusParameter_STRING
BEGIN_IMPL_RUNTIME_CLASS(CDBusParameter_STRING);
BASE_IMPL_RUNTIME_CLASS(CDBusParameter);
END_IMPL_RUNTIME_CLASS();

CDBusParameter_STRING::CDBusParameter_STRING()
{
	m_ArgType = DBUS_TYPE_STRING;
}

CDBusParameter_STRING::~CDBusParameter_STRING()
{
}

int CDBusParameter_STRING::getArgType()
{
	return m_ArgType;
}

INT32_t CDBusParameter_STRING::toInt32()
{
	return atoi(m_strValue);
}

UINT32_t CDBusParameter_STRING::toUint32()
{
	return atoi(m_strValue);
}

CString CDBusParameter_STRING::toString()
{
	return m_strValue;
}

VOID CDBusParameter_STRING::setValue(const char * pszValue)
{
	m_strValue = pszValue;
}

CString CDBusParameter_STRING::getValue()
{
	return m_strValue;
}

CDBusParameter_STRING::operator LPCSTR() CONST
{
	return (LPCSTR)m_strValue;
}


#if 1/*added by lxj 2012-11-23*/
//CDBusParameter_ByteArray
BEGIN_IMPL_RUNTIME_CLASS(CDBusParameter_ByteArray);
BASE_IMPL_RUNTIME_CLASS(CDBusParameter);
END_IMPL_RUNTIME_CLASS();

CDBusParameter_ByteArray::CDBusParameter_ByteArray()
{
	m_ArgType = DBUS_TYPE_ARRAY;
}

CDBusParameter_ByteArray::~CDBusParameter_ByteArray()
{
}

int CDBusParameter_ByteArray::getArgType()
{
	return m_ArgType;
}

SharedPtr <CByteArray2> CDBusParameter_ByteArray::toByteArray()
{
	if( m_spByteArray.isNull() ){
		m_spByteArray = SharedPtr <CByteArray2> (new CByteArray2);
	}
	return m_spByteArray;
}
#endif