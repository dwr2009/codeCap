#ifndef	_BACKEND_TRANSACTION_THREAD_H_
#define	_BACKEND_TRANSACTION_THREAD_H_

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <QTimer>

class QBackendTransactionThread : public QThread
{
	Q_OBJECT
public:
	QBackendTransactionThread();
	virtual ~QBackendTransactionThread();
public slots:
	void OnTimer_Sync();
private:
	virtual void run();
private:
	Qt::HANDLE m_BackendTransThreadId;
	QTimer m_SyncTimer;
};

#endif	//_BACKEND_TRANSACTION_THREAD_H_

