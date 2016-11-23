#ifndef	_BACKEND_TRANSACTION_THREAD_H_
#define	_BACKEND_TRANSACTION_THREAD_H_

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QTimer>
#include "MessageItem.h"
#include <snmp_pp/address.h>
#include <snmp_pp/target.h>
#include <snmp_pp/uxsnmp.h>

class QBackendTransactionThread : public QThread
{
	Q_OBJECT
public:
	QBackendTransactionThread();
	virtual ~QBackendTransactionThread();
public slots:
	virtual void On_AsyncTransaction(const MessageItem_SharedPointer_t & MsgItem_sp);
signals:
	void AsyncTransactionFinished(const MessageItem_SharedPointer_t & MsgItem_sp);
private:
	virtual void run();
	virtual INT_t DoTransaction(const MessageItem_SharedPointer_t & MsgItem_sp);
	INT_t DoFormatMedia(const MessageItem_SharedPointer_t & MsgItem_sp);
	INT_t DoSnmpSet(const MessageItem_SharedPointer_t & MsgItem_sp);
	INT_t DoSnmpGet(const MessageItem_SharedPointer_t & MsgItem_sp);
	INT_t CreateLocalSnmpSrvTarget();
private:
	Qt::HANDLE m_BackendTransThreadId;
	QSharedPointer <Snmp_pp::Snmp> m_LocalSnmpSession_sp;
	QSharedPointer <Snmp_pp::CTarget> m_LocalSnmpSrvTarget_sp;
};

#endif	//_BACKEND_TRANSACTION_THREAD_H_

