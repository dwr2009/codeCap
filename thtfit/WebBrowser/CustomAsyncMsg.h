#ifndef	_CUSTOM_ASYNC_MSG_H_
#define	_CUSTOM_ASYNC_MSG_H_

#include "MessageItem.h"
#include "Api/WebBrowserAppDef.h"
#include <QErrorInfoList.h>
#include <QVariant>
#include <QMutex>

namespace ControlCenter
{
	
//MsgIds
typedef enum
{
	AsyncMsgId_NONE,
	AsyncMsgId_FormatMedia,
	AsyncMsgId_SnmpSet,
	AsyncMsgId_SnmpGet,
}AsyncMsgId_TYPE, *P_AsyncMsgId_TYPE;

}

class QMsg_FormatMedia : public QMessageItem
{
public:
	QMsg_FormatMedia();
	virtual ~QMsg_FormatMedia();
	virtual INT_t setParameters(CONST ControlCenter::FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType,
		CONST ActiveMedia_TYPE eActiveMediaType);
	virtual INT_t getParameters(ControlCenter::FORMAT_MEDIA_ACTION_TYPE & eFmtMediaActionType,
		ActiveMedia_TYPE & eActiveMediaType);
	virtual QErrorInfoList & getErrInfoList();
private:
	ControlCenter::FORMAT_MEDIA_ACTION_TYPE m_eFmtMediaActionType;
	ActiveMedia_TYPE m_eActiveMediaType;
	QErrorInfoList m_lstErrInfo;
};

class QMsg_SnmpOp : public QMessageItem
{
public:
	QMsg_SnmpOp();
	virtual ~QMsg_SnmpOp();
	virtual int getTransactionId();
private:
	static int g_iTransactionId;
	static QMutex g_mutexGlobal;
	int m_iCurTransactionId;
};

class QMsg_SnmpSet : public QMsg_SnmpOp
{
public:
	QMsg_SnmpSet();
	virtual ~QMsg_SnmpSet();
	virtual INT_t setParameters(const QString strOid, const QVariant varArgs);
	virtual INT_t getParameters(QString & strOid, QVariant & varArgs);
private:
	QString m_strOid;
	QVariant m_varArgs;
};

class QMsg_SnmpGet : public QMsg_SnmpOp
{
public:
	QMsg_SnmpGet();
	virtual ~QMsg_SnmpGet();
	virtual INT_t setParameters(const QString strOid);
	virtual INT_t getParameters(QString & strOid);
	virtual INT_t setResult(const char * pszResult);
	virtual QString getResult();
	virtual const QString & getResult() const;
private:
	QString m_strOid;
	QString m_strResult;
};

#endif	//_CUSTOM_ASYNC_MSG_H_

