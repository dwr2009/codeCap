#include "Include/ThreadSleep.h"
#include <unistd.h>
#include "Include/DateTime.h"
#include <errno.h>
#include "Include/ErrPrintHelper.h"

void msleep_exactly(signed long int iDurationMs)
{
	int iRet;
	UINT64_t StartTimeMs = GetSysUpTimeMs();
	while(TRUE)
	{
		if(0 > iDurationMs)
		{
			iDurationMs = 0;
			break;
		}
		iRet = usleep(iDurationMs*1000);
		if(0 != iRet)
		{
			if(EINTR == errno)
			{
				iDurationMs -= (GetSysUpTimeMs() - StartTimeMs);
				StartTimeMs = GetSysUpTimeMs();
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
			
		}else{
			break;
		}
	}
}

