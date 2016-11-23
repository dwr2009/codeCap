#ifndef	_Q_AUTO_LOCK_H_
#define	_Q_AUTO_LOCK_H_

#include <QMutex>

class QAutoLock
{
public:
	explicit QAutoLock(QMutex * pQMutex);
	virtual ~QAutoLock();
private:
	QMutex * m_pQMutex;
};

#endif	//_Q_AUTO_LOCK_H_
