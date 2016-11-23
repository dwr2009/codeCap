#include "CustomAsyncMsg.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <QAutoLock.h>

//QMsg_FormatMedia

QMsg_FormatMedia::QMsg_FormatMedia()
{
	m_eFmtMediaActionType = ControlCenter::FORMAT_MEDIA_ACTION_FORMAT;
}

QMsg_FormatMedia::~QMsg_FormatMedia()
{
}

INT_t QMsg_FormatMedia::setParameters(CONST ControlCenter::FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType,
	CONST ActiveMedia_TYPE eActiveMediaType)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_eFmtMediaActionType = eFmtMediaActionType;
		m_eActiveMediaType = eActiveMediaType;
	}while(FALSE);

	return iOutRet;
}

INT_t QMsg_FormatMedia::getParameters(ControlCenter::FORMAT_MEDIA_ACTION_TYPE & eFmtMediaActionType,
	ActiveMedia_TYPE & eActiveMediaType)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		eFmtMediaActionType = m_eFmtMediaActionType;
		eActiveMediaType = m_eActiveMediaType;
	}while(FALSE);

	return iOutRet;
}

QErrorInfoList & QMsg_FormatMedia::getErrInfoList()
{
	return m_lstErrInfo;
}

//QMsg_SnmpOp
int QMsg_SnmpOp::g_iTransactionId = 0;
QMutex QMsg_SnmpOp::g_mutexGlobal(QMutex::Recursive);

QMsg_SnmpOp::QMsg_SnmpOp()
{
	{
		QAutoLock autoRangeLocker(&g_mutexGlobal);
		g_iTransactionId++;
		if(0 >= g_iTransactionId)
		{
			g_iTransactionId = 1;
		}
		m_iCurTransactionId = g_iTransactionId;
	}
}

QMsg_SnmpOp::~QMsg_SnmpOp()
{
}

int QMsg_SnmpOp::getTransactionId()
{
	return m_iCurTransactionId;
}

//QMsg_SnmpSet

QMsg_SnmpSet::QMsg_SnmpSet()
{
	INT_t iRet;
	iRet = setMsgId(ControlCenter::AsyncMsgId_SnmpSet);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QMsg_SnmpSet::~QMsg_SnmpSet()
{
}

INT_t QMsg_SnmpSet::setParameters(const QString strOid, const QVariant varArgs)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_strOid = strOid;
		m_varArgs = varArgs;
	}while(FALSE);

	return iOutRet;
}

INT_t QMsg_SnmpSet::getParameters(QString & strOid, QVariant & varArgs)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		strOid = m_strOid;
		varArgs = m_varArgs;
	}while(FALSE);

	return iOutRet;
}

//QMsg_SnmpGet

QMsg_SnmpGet::QMsg_SnmpGet()
{
	INT_t iRet;
	iRet = setMsgId(ControlCenter::AsyncMsgId_SnmpGet);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QMsg_SnmpGet::~QMsg_SnmpGet()
{
}

INT_t QMsg_SnmpGet::setParameters(const QString strOid)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_strOid = strOid;
	}while(FALSE);
	
	return iOutRet;
}

INT_t QMsg_SnmpGet::getParameters(QString & strOid)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		strOid = m_strOid;
	}while(FALSE);

	return iOutRet;
}

INT_t QMsg_SnmpGet::setResult(const char * pszResult)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_strResult = pszResult;
	}while(FALSE);

	return iOutRet;
}

QString QMsg_SnmpGet::getResult()
{
	return m_strResult;
}

const QString & QMsg_SnmpGet::getResult() const
{
	return m_strResult;
}

