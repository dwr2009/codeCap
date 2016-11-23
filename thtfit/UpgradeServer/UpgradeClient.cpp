#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <memory.h>
#include "BaseDef.h"

int main(int argc,char*argv[])
{
	int iOutRet=ERR_SUCCESS;
	int upgradeInputFd=-1;
	int sizeWritten=0,buflen=0;

	upgradeInputFd = open(BTN_UPGRADESERVER_PIPE_PATH, O_RDWR | O_NONBLOCK);
	if(0 > upgradeInputFd)
	{
		iOutRet = ERROR_FILE_OPEN_FAIL;
		goto PROC_EXIT;
	}

	if(NULL==argv[1])
	{
		printf("[%s:%d] input message is NULL\n", __FILE__, __LINE__);
		iOutRet = ERROR_INVALID_PARAMETER;
		goto PROC_EXIT;
	}

	buflen=strlen(argv[1]);
	
	if(0==buflen)
	{
		printf("[%s:%d] input message length is zero\n", __FILE__, __LINE__);
		iOutRet = ERROR_INVALID_PARAMETER;
		goto PROC_EXIT;
	}
	
	sizeWritten = write(upgradeInputFd,argv[1],buflen);
	if(0 < sizeWritten)
	{
	}
	else if(0 == sizeWritten)
	{
		printf("[%s:%d] no message send\n", __FILE__, __LINE__);
	}
	else
	{
		if(EAGAIN == errno)
		{
			printf("%s: write System Call interrupted, retrying\n", __FUNCTION__);
		}
		else
		{
			printf("%s: ERROR: write(): errno = %d", __FUNCTION__, errno);
		}
	}
	

PROC_EXIT:

	if(-1!=upgradeInputFd)
	{
		close(upgradeInputFd);
		upgradeInputFd=-1;
	}
	
	return iOutRet;
	
}




