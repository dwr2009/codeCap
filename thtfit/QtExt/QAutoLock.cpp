#include "QAutoLock.h"

QAutoLock::QAutoLock(QMutex * pQMutex)
{
	m_pQMutex = pQMutex;
	if(m_pQMutex)
	{
		m_pQMutex->lock();
	}
}

QAutoLock::~QAutoLock()
{
	if(m_pQMutex)
	{
		m_pQMutex->unlock();
	}
}


