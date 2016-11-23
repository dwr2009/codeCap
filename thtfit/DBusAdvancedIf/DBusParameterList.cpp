#include <InvArgException.h>
#include "DBusParameterList.h"
#include <BaseErrException.h>

CDBusParameterList::CDBusParameterList()
{

}

CDBusParameterList::~CDBusParameterList()
{

}

size_t CDBusParameterList::getSize()
{
	return m_DBusParamList.size();
}

INT_t CDBusParameterList::ConvertFromDBusMsg(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS;
	dbus_bool_t bDBusRet;
	DBusMessageIter DBusMsgIter;
	int DBusArgType;

	do
	{
		if(NULL == pDBusMsg)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		m_DBusParamList.clear();

		bDBusRet = dbus_message_iter_init(pDBusMsg, &DBusMsgIter);
		if(FALSE == bDBusRet)	//no arguments
		{
			break;
		}

		while(TRUE)
		{
			DBusArgType = dbus_message_iter_get_arg_type(&DBusMsgIter);
			if(DBUS_TYPE_INVALID == DBusArgType)
			{
				break;
			}

			if(DBUS_TYPE_INT32 == DBusArgType)
			{
 				SharedPtr <CDBusParameter_INT32> DBusParamInt32_sp;
 				DBusParamInt32_sp = SharedPtr <CDBusParameter_INT32> (new CDBusParameter_INT32);
				if(NULL == DBusParamInt32_sp.get())
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				INT32_t Value = 0;
				dbus_message_iter_get_basic(&DBusMsgIter, &Value);
				DBusParamInt32_sp->setValue(&Value);
				m_DBusParamList.push_back(DBusParamInt32_sp);
			}
			else if(DBUS_TYPE_UINT32 == DBusArgType)
			{
 				SharedPtr <CDBusParameter_UINT32> DBusParamUint32_sp;
 				DBusParamUint32_sp = SharedPtr <CDBusParameter_UINT32> (new CDBusParameter_UINT32);
				if(NULL == DBusParamUint32_sp.get())
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				UINT32_t Value = 0;
				dbus_message_iter_get_basic(&DBusMsgIter, &Value);
				DBusParamUint32_sp->setValue(&Value);
				m_DBusParamList.push_back(DBusParamUint32_sp);
			}
			else if(DBUS_TYPE_STRING == DBusArgType)
			{
 				SharedPtr <CDBusParameter_STRING> DBusParamString_sp;
 				DBusParamString_sp = SharedPtr <CDBusParameter_STRING> (new CDBusParameter_STRING);
				if(NULL == DBusParamString_sp.get())
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				const char *pValue = NULL;
				dbus_message_iter_get_basic(&DBusMsgIter, &pValue);
				DBusParamString_sp->setValue(pValue);
				m_DBusParamList.push_back(DBusParamString_sp);
			}
#if 1/*added by lxj 2012-11-23*/
			else if(DBUS_TYPE_ARRAY == DBusArgType){
				int element_type = dbus_message_iter_get_element_type(&DBusMsgIter);
 				SharedPtr <CDBusParameter_ByteArray> DBusParamByteArray_sp;
 				DBusParamByteArray_sp = SharedPtr <CDBusParameter_ByteArray> (new CDBusParameter_ByteArray);
				if(NULL == DBusParamByteArray_sp.get()){
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}

				if (dbus_type_is_fixed (element_type) &&
              	     element_type != DBUS_TYPE_UNIX_FD) {
					DBusMessageIter subiter;
					const BYTE *pData = NULL;
					INT_t iDataLen=0;

					dbus_message_iter_recurse (&DBusMsgIter, &subiter);
					dbus_message_iter_get_fixed_array(&subiter, &pData, &iDataLen);
					DBusParamByteArray_sp->toByteArray()->Append((BYTE*)pData,iDataLen);
					m_DBusParamList.push_back(DBusParamByteArray_sp);
				}else{
					iOutRet = ERROR_INVALID_PARAMETER;
					LOG_BLINE("Ignore: element type=%d!",element_type);
					break;
				}
			}
#endif

			bDBusRet = dbus_message_iter_next(&DBusMsgIter);
			if(FALSE == bDBusRet)	//no more fileds
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CDBusParameterList::AppendToDBusMsg(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS;
	dbus_bool_t bDBusRet;
	DBUS_PARAMETER_LIST::iterator itDBusParam;

	do
	{
		if(NULL == pDBusMsg)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		for(itDBusParam = m_DBusParamList.begin(); itDBusParam != m_DBusParamList.end(); itDBusParam++)
		{
			SharedPtr <CDBusParameter> & DBusParam_sp = (*itDBusParam);
			if(NULL == DBusParam_sp.get())
			{
				continue;
			}
			if(DBUS_TYPE_INT32 == DBusParam_sp->getArgType())
			{
				SharedPtr <CDBusParameter_INT32> DBusParam_INT32_sp = DBusParam_sp.DynamicCast<CDBusParameter_INT32>();
				if(NULL == DBusParam_INT32_sp.get())
				{
					continue;
				}
				INT32_t Value = (INT32_t)(*DBusParam_INT32_sp);
				bDBusRet = dbus_message_append_args(pDBusMsg, DBUS_TYPE_INT32, &Value, DBUS_TYPE_INVALID);
				if(TRUE != bDBusRet)
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
			}
			else if(DBUS_TYPE_UINT32 == DBusParam_sp->getArgType())
			{
				SharedPtr <CDBusParameter_UINT32> DBusParam_UINT32_sp = 
				DBusParam_sp.DynamicCast<CDBusParameter_UINT32>();
				if(NULL == DBusParam_UINT32_sp.get())
				{
					continue;
				}
				UINT32_t Value = (UINT32_t)(*DBusParam_UINT32_sp);
				bDBusRet = dbus_message_append_args(pDBusMsg, DBUS_TYPE_UINT32, &Value, DBUS_TYPE_INVALID);
				if(TRUE != bDBusRet)
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
			}
			else if(DBUS_TYPE_STRING == DBusParam_sp->getArgType())
			{
				SharedPtr <CDBusParameter_STRING> DBusParam_String_sp = 
				DBusParam_sp.DynamicCast<CDBusParameter_STRING>();
				if(NULL == DBusParam_String_sp.get())
				{
					continue;
				}
				LPCSTR Value = (LPCSTR)(*DBusParam_String_sp);
				//printf("%s\n", Value);
				bDBusRet = dbus_message_append_args(pDBusMsg, DBUS_TYPE_STRING, &Value, DBUS_TYPE_INVALID);
				if(TRUE != bDBusRet)
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
			}
#if 1/*added by lxj 2012-11-23*/
			else if(DBUS_TYPE_ARRAY == DBusParam_sp->getArgType()){
				SharedPtr <CByteArray2> ByteArray_sp;
				SharedPtr <CDBusParameter_ByteArray> DBusParam_ByteArray_sp = 
				DBusParam_sp.DynamicCast<CDBusParameter_ByteArray>();

				if(NULL == DBusParam_ByteArray_sp.get()){
					continue;
				}

				ByteArray_sp = DBusParam_ByteArray_sp->toByteArray();
				if( ( ByteArray_sp.isNull() )    ||
					( !ByteArray_sp->GetSize() ) ||
					( !ByteArray_sp->GetData() ) ){
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}

				const BYTE *v_ARRAY = ByteArray_sp->GetData();
				bDBusRet = dbus_message_append_args ( pDBusMsg,
				                                      DBUS_TYPE_ARRAY,
				                                      DBUS_TYPE_BYTE,
				                                      &v_ARRAY,
				                                      ByteArray_sp->GetSize(),
				                                      DBUS_TYPE_INVALID);
				if(TRUE != bDBusRet){
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
			}
#endif
		}
	}while(FALSE);

	return iOutRet;
}

CDBusParameterList & CDBusParameterList::operator >>(DBusMessage * pDBusMsg)
{
	INT_t iRet;

	do
	{
		if(NULL == pDBusMsg)
		{
#ifdef	__EXCEPTIONS
			throw CInvArgException();
#endif	//__EXCEPTIONS
			break;
		}
		iRet = AppendToDBusMsg(pDBusMsg);
		if(NULL == pDBusMsg)
		{
#ifdef	__EXCEPTIONS
			throw CBaseErrException(iRet);
#endif	//__EXCEPTIONS
			break;
		}
	}while(FALSE);

	return (*this);
}

INT_t CDBusParameterList::operator >> (SharedPtr <CDBusMessage> DbusMsg_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(DbusMsg_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = DbusMsg_sp->AppendArgs(*this);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CDBusParameterList::addParameter(SharedPtr<CDBusParameter> & DBusParam_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_DBusParamList.push_back(DBusParam_sp);
	}while(FALSE);

	return iOutRet;
}

CDBusParameterList & CDBusParameterList::operator << (DBusMessage * pDBusMsg)
{
	INT_t iRet;

	do
	{
		if(NULL == pDBusMsg)
		{
#ifdef	__EXCEPTIONS
			throw CInvArgException();
#endif	//__EXCEPTIONS
			break;
		}
		iRet = ConvertFromDBusMsg(pDBusMsg);
		if(NULL == pDBusMsg)
		{
#ifdef	__EXCEPTIONS
			throw CBaseErrException(iRet);
#endif	//__EXCEPTIONS
			break;
		}
	}while(FALSE);

	return (*this);
}

INT_t CDBusParameterList::AddParameter(LPCSTR pszParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr<CDBusParameter> DBusParam_sp;
	SharedPtr<CDBusParameter_STRING> DBusParam_String_sp(new CDBusParameter_STRING);

	do
	{
		if(DBusParam_String_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		DBusParam_String_sp->setValue(pszParam);
		DBusParam_sp = DBusParam_String_sp;
		iOutRet = addParameter(DBusParam_sp);
	}while(FALSE);

	return iOutRet;
}

INT_t CDBusParameterList::AddParameter(INT32_t iParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr<CDBusParameter> DBusParam_sp;
	SharedPtr<CDBusParameter_INT32> DBusParam_Int32_sp(new CDBusParameter_INT32);

	do
	{
		if(DBusParam_Int32_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		DBusParam_Int32_sp->setValue(&iParam);
		DBusParam_sp = DBusParam_Int32_sp;
		iOutRet = addParameter(DBusParam_sp);
	}while(FALSE);

	return iOutRet;
}

INT_t CDBusParameterList::AddParameter(UINT32_t uiParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr<CDBusParameter> DBusParam_sp;
	SharedPtr<CDBusParameter_UINT32> DBusParam_Uint32_sp(new CDBusParameter_UINT32);

	do
	{
		if(DBusParam_Uint32_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		DBusParam_Uint32_sp->setValue(&uiParam);
		DBusParam_sp = DBusParam_Uint32_sp;
		iOutRet = addParameter(DBusParam_sp);
	}while(FALSE);

	return iOutRet;
}

CDBusParameterList & CDBusParameterList::operator << (LPCSTR pszParam)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (LPCSTR pszParam)\n");

	do
	{
		iRet = AddParameter(pszParam);
	}while(FALSE);

	return (*this);
}

CDBusParameterList & CDBusParameterList::operator << (CONST CString & strParam)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (CONST CString & strParam)\n");

	do
	{
		iRet = AddParameter((LPCSTR)strParam);
	}while(FALSE);

	return (*this);
}

CDBusParameterList & CDBusParameterList::operator << (CONST CStackBufString & strParam)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (CONST CStackBufString & strParam)\n");

	do
	{
		iRet = AddParameter((LPCSTR)strParam);
	}while(FALSE);

	return (*this);
}

CDBusParameterList & CDBusParameterList::operator << (INT32_t iParam)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (INT32_t iParam)\n");

	do
	{
		iRet = AddParameter(iParam);
	}while(FALSE);

	return (*this);
}

CDBusParameterList & CDBusParameterList::operator << (UINT32_t uiParam)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (UINT32_t uiParam)\n");

	do
	{
		iRet = AddParameter(uiParam);
	}while(FALSE);

	return (*this);
}

SharedPtr<CDBusParameter> CDBusParameterList::operator [](int iIndex)
{
	SharedPtr<CDBusParameter> DBusParam_sp;
	DBUS_PARAMETER_LIST::iterator itDBusParam;

	do
	{
		if(0 > iIndex || ((typeof(iIndex))(m_DBusParamList.size())) <= iIndex)
		{
			DBusParam_sp = SharedPtr<CDBusParameter> (new CDBusParameter);
			break;
		}
		itDBusParam = m_DBusParamList.begin();
		int StepIndex = iIndex;
		while(StepIndex--)
		{
			itDBusParam++;
		}
		DBusParam_sp = *itDBusParam;
	}while(FALSE);

	return DBusParam_sp;
}

#if 1/*added by lxj 2012-11-23*/
INT_t CDBusParameterList::addParameter(SharedPtr<CDBusParameter_ByteArray> & DBusParam_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_DBusParamList.push_back(DBusParam_sp);
	}while(FALSE);

	return iOutRet;
}

INT_t CDBusParameterList::addParameter_ByteArray(PBYTE pData, size_t sizeData)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr <CDBusParameter_ByteArray> DBusParam_ByteArray_sp(new CDBusParameter_ByteArray);

	do
	{
		if(DBusParam_ByteArray_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		DBusParam_ByteArray_sp->toByteArray()->Append(pData,sizeData);
		iOutRet = addParameter(DBusParam_ByteArray_sp);
	}while(FALSE);

	return iOutRet;
}

CDBusParameterList & CDBusParameterList::operator << (SharedPtr <CDBusParameter_ByteArray> & spByteArray)
{
	INT_t iRet;

	//LOG_BLINE("CDBusParameterList::operator << (SharedPtr <CDBusParameter_ByteArray> spByteArray)\n");

	do
	{
		iRet = addParameter(spByteArray);
	}while(FALSE);

	return (*this);
}
#endif
