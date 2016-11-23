#include <QApplication>
#include <QSharedPointer>
#include "MediaPlayerApp.h"
#include <BaseErrDef.h>
#include <MediaPlayerApi.h>
#include <BaseTypeDef.h>
#include <unistd.h>
#include <fcntl.h>
using namespace CppBase;

static WeakPtr <QMediaPlayerApp> g_MediaPlayer_wp;

int main(int argc, char * argv[])
{
	int ProcessExitCode = 0;
	SharedPtr <QMediaPlayerApp> MediaPlayer_sp;
	SharedPtr <CGeneralApp> GeneralApp_wp;
    
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

//	setApplicationType(APP_TTY);
	MediaPlayer_sp = SharedPtr <QMediaPlayerApp> (new QMediaPlayerApp(argc, argv));
	if(MediaPlayer_sp.isNull())
	{
		ProcessExitCode = MPEC_ERR_OUT_OF_MEM;
		goto EXIT_PROC;
	}
	GeneralApp_wp = MediaPlayer_sp;
	MediaPlayer_sp->setThisWp(GeneralApp_wp);
	g_MediaPlayer_wp = MediaPlayer_sp;
	
	ProcessExitCode = MediaPlayer_sp->exec();

	if(FALSE == MediaPlayer_sp.isNull())
	{
		MediaPlayer_sp.Clear();
	}
	
EXIT_PROC:
	return ProcessExitCode;
}

