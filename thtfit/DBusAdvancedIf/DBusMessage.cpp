#include "DBusMessage.h"
#include <BaseErrDef.h>
#include <InvArgException.h>
#include <BaseErrException.h>

CDBusMessage::CDBusMessage()
{
	m_pDbusMsg = NULL;
}

CDBusMessage::~CDBusMessage()
{
	if(m_pDbusMsg)
	{
		dbus_message_unref(m_pDbusMsg);
		m_pDbusMsg = NULL;
	}
}

INT_t CDBusMessage::SendErrReply(DBusConnection * pDBusConn, DBusMessage * pInDBusMsg,
	LPCSTR pszErrName, LPCSTR pszErrMsg/* NULL*/, BOOL_t bFlush/* = FALSE*/)
{
	INT_t iOutRet = ERROR_SUCCESS;
	DBusMessage * pReplyMsg = NULL;
	dbus_bool_t bDBusRet;

	do
	{
		pReplyMsg = dbus_message_new_error(pInDBusMsg, pszErrName, pszErrMsg);
		if(NULL == pReplyMsg)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bDBusRet = dbus_connection_send(pDBusConn, pReplyMsg, NULL);
		if(TRUE != bDBusRet)
		{
			iOutRet = ERR_DBUS_SEND_ERR;
			break;
		}

		if(bFlush)
		{
			dbus_connection_flush(pDBusConn);
		}
	}while(FALSE);

	if(pReplyMsg)
	{
		dbus_message_unref(pReplyMsg);
		pReplyMsg = NULL;
	}

	return iOutRet;
}

SharedPtr <CDBusMessage> CDBusMessage::createMethodCall(LPCSTR pszService, LPCSTR pszPath,
	LPCSTR pszInterface, LPCSTR pszMethod)
{
	SharedPtr <CDBusMessage> DbusMsg_sp(new CDBusMessage);

	do
	{
		if(DbusMsg_sp.isNull())
		{
			break;
		}
		DbusMsg_sp->m_pDbusMsg = dbus_message_new_method_call(pszService, pszPath, pszInterface, pszMethod);
		if(NULL == DbusMsg_sp->m_pDbusMsg)
		{
			DbusMsg_sp.Clear();
			break;
		}
		
	}while(FALSE);

	return DbusMsg_sp;
}

INT_t CDBusMessage::AppendArgs(CDBusParameterList & DbusParamList)
{	
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(NULL == m_pDbusMsg)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		try
		{
			DbusParamList >> m_pDbusMsg;
		}
		catch(CInvArgException & InvArgEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		catch(CBaseErrException & BaseErrEx)
		{
			iOutRet = BaseErrEx.getBaseErr();
			break;
		}
	}while(FALSE);

	return iOutRet;
}

CDBusMessage::operator DBusMessage *() const
{
	return m_pDbusMsg;
}

INT_t CDBusMessage::Attach(DBusMessage * pDbusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_pDbusMsg)
		{
			dbus_message_unref(m_pDbusMsg);
			m_pDbusMsg = NULL;
		}
		if(NULL == pDbusMsg)
		{
			break;
		}
		m_pDbusMsg = pDbusMsg;
	}while(FALSE);

	return iOutRet;
}

