#include	"Read.h"
/* According to POSIX 1003.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include <termios.h>

#include	<BaseTypeDef.h>

int main(int argc, char * argv[])
{
	int iOutRet = 0, iRet = 0, iFdToRead1 = 0;
	fd_set fdsRead, fdsExcept; struct timeval tvTimeout;
	unsigned int uiKey; struct termios tioSave, tioToSet;
	BOOL bTIO_Op_Success = TRUE;

	iFdToRead1 = STDIN_FILENO;
	FD_ZERO(&fdsRead); FD_ZERO(&fdsExcept);
	FD_SET(iFdToRead1, &fdsRead); FD_SET(STDIN_FILENO, &fdsExcept);
	tvTimeout.tv_sec = 0; tvTimeout.tv_usec = 150*1000;
		
	do{
		iRet = tcgetattr(iFdToRead1, &tioSave);
		if(-1 == iRet)
		{
			printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			bTIO_Op_Success = FALSE;
		}

		if(bTIO_Op_Success)
		{
			tioToSet = tioSave;
			tioToSet.c_lflag |= (ISIG);
			tioToSet.c_lflag &= ~(ECHO | ICANON);
			tioToSet.c_iflag |= (BRKINT);
			tioToSet.c_iflag &= ~(IGNBRK);
			/* 
			   arrows or F1 characters generate a sequence of 3 characters in a row:
			   like ESC [ A (up arrow).
			   
			   To avoid reading [ or A as an individual character, we set
			   an inter character interval of 1/10 second. If characters
			   are received in less of 1/10 second then it is treated as
			   an individual character and is discard.
			*/
			
			tioToSet.c_cc[VMIN] = 3;   /* up to 3 chars at a time */
			tioToSet.c_cc[VTIME] = 1;  /* after receiving 1 char and after a delay of 1/10 second, 'read' returns */
			iRet = tcsetattr(iFdToRead1, TCSAFLUSH, &tioToSet);
			if(-1 == iRet)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				bTIO_Op_Success = FALSE;
			}
		}	
		
		iRet = select(iFdToRead1 + 1, &fdsRead, NULL, &fdsExcept, &tvTimeout);
		if(-1 == iRet)
		{
			printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}

		if(0 == iRet) {break;}
		
		if(FD_ISSET(iFdToRead1, &fdsExcept))
		{
			break;
		}
		
		if(FD_ISSET(iFdToRead1, &fdsRead))
		{
			iRet = read(iFdToRead1, &uiKey, sizeof(uiKey));
			if(-1 == iRet)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				break;
			}
			else if(0 == iRet)
			{
				printf("[%s:%d]EOF\n", __FILE__, __LINE__);
				break;
			}
			
			iOutRet = uiKey;
		}
	}while(FALSE);

	if(bTIO_Op_Success)
	{
		iRet = tcsetattr(iFdToRead1, TCSAFLUSH, &tioSave);
		if(-1 == iRet)
		{
			printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			bTIO_Op_Success = FALSE;
		}
	}

	return iOutRet;
}

