#include	"SignalInfo.h"
#include	<stdio.h>

LPCSTR CSignalInfo::m_ppSignalDesc[] =
{
	"",
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGBUS",
	"SIGFPE",
	"SIGKILL",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGSTKFLT",
	"SIGCHLD",
	"SIGCONT",
	"SIGSTOP",
	"SIGTSTP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGURG",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGIO",
	"SIGPWR",
	"SIGSYS",
	NULL
};

LPCSTR CSignalInfo::GetSingalDescFromSigNo(int iSigNo)
{
	//printf("count=%d\n", sizeof(m_ppSignalDesc)/sizeof(m_ppSignalDesc[0]));
	
	if(sizeof(m_ppSignalDesc)/sizeof(m_ppSignalDesc[0]) > iSigNo && 0 < iSigNo)
	{
		return m_ppSignalDesc[iSigNo];
	}
	else
	{
		return m_ppSignalDesc[0];
	}
}

