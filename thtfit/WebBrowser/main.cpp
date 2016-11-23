#include "browser.h"
#include <LinuxSignalHandlerEx.h>
#include "WebBrowserApp.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
//#include <inputpanelcontext.h>

int main(int argc, char *argv[])
{
    int iRet=-1; 
    pid_t iPid=-1;
    
   
    printf("start service %s ...\n",argv[0]);
    
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
    do
    {
        int iRetPid=0;
        int iState=0;
        //printf("start service %s ...\n",argv[0]);
        iPid=fork();
        
        if(iPid>0)
        {
            printf("wait children process  iPid %d \n",iPid);            
            iRetPid=waitpid(iPid,&iState,0);
            printf("childree process %d died,\n",iPid);
            sleep(5);
            continue;           
        }        
        if(iPid<0)
        {
            printf("%s fork failed\n",argv[0]);
            exit(-1);
        }
        printf("service %s Process %d started \n",argv[0],getpid());
        setsid();
        QWebBrowserApp app(argc, argv);
        iRet =  app.exec();
        break;
    }while(1);
    return iRet;
}

