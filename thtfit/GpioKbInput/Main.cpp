#include <stdio.h>
#include "Keypad.h"
#include <signal.h>
#include "UserInputDevice.h"
#include "ClientKeypad.h"
#include "IndependentKeypad.h"
#include <unistd.h>
#include <fcntl.h>

static Keypad *g_pKeypad=NULL;
static void terminate(const int iSignalNo)
{
	BSTD_UNUSED(iSignalNo);
	Keypad *pKeypad=g_pKeypad;
	printf("%s(%d) iSignalNo:%d\n",__func__,__LINE__,iSignalNo);	
	switch(iSignalNo){
	case SIGHUP:
	case SIGQUIT:
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
	case SIGILL:
	case SIGSTOP:
	case SIGSEGV:
		pKeypad->signalExit(iSignalNo);
		break;
	default:
		printf("%s(%d) iSignalNo:%d miss process \n",__func__,__LINE__,iSignalNo);
		break;
	}
}

static void installSignalHandler(void)
{
	signal(SIGBUS, terminate);
	signal(SIGFPE, terminate);
	signal(SIGHUP, terminate);
	signal(SIGILL, terminate);
	signal(SIGINT, terminate);
	signal(SIGIOT, terminate);
	signal(SIGPIPE, terminate);
	signal(SIGQUIT, terminate);
	signal(SIGSEGV, terminate);
	signal(SIGSYS, terminate);
	signal(SIGTERM, terminate);
	signal(SIGTRAP, terminate);
	signal(SIGUSR1, terminate);
	signal(SIGUSR2, terminate);
}

int main(int argc,char **argv)
{
	
	BSTD_UNUSED(argc);
	BSTD_UNUSED(argv);
	
	Keypad *pKeypad=NULL;
    pid_t iPid=-1;
    
        
    printf("start service %s ...\n",argv[0]);
    iPid=fork();
    if(iPid>0)
    {
        printf("parent exit\n");
        exit(0);
    }
    if(iPid<0)
    {
        printf("%s fork failed\n",argv[0]);
        exit(-1);
    }
    printf("service %s init\n",argv[0]);
    setsid();
    
#if 0
    do
    {
        int iNullFd=-1;
        int iRetFd=-1;
        const int iStdInFd=0;
        const int iStdOutFd=1;
        const int iStdErrFd=2;
        iNullFd=open("/dev/null", O_RDWR);
        if(iNullFd<0)
        {
            printf("%s open /dev/null failed\n",argv[0]);
            break;
        }
        iRetFd=dup2(iNullFd, iStdInFd);
        if(iRetFd<0)
        {
            printf("%s dup2 stdin failed\n",argv[0]);
            break;
        }
        iRetFd=dup2(iNullFd, iStdOutFd);
        iRetFd=dup2(iNullFd, iStdErrFd);
        if(iNullFd>2)
        {
            close(iNullFd);
        }   
        
    }while(false);
#endif

    {
        UserInputDevice sUserInputDevice;
        pKeypad=ClientKeypad::getKeypadInstance();
        g_pKeypad=pKeypad;
        pKeypad->setUserInputDevice(&sUserInputDevice);
        pKeypad->initKeypad();
        installSignalHandler();
        while(pKeypad->isRun())
        {
            pKeypad->waitEvent();
        }
        pKeypad->exitKeypad();	
        sUserInputDevice.destroyInputDevice();
    }
    return 0;
}


