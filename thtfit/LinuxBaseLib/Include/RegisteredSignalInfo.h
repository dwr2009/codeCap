#ifndef	_REGISTERED_SIGNAL_INFO_H_
#define	_REGISTERED_SIGNAL_INFO_H_

#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

class CRegisteredSignalInfo
{
public:
	CRegisteredSignalInfo();
	CRegisteredSignalInfo(int SigNo);
public:
	int m_SigNo;
	sighandler_t m_pOldSigHandler;
};

#endif	//_REGISTERED_SIGNAL_INFO_H_

