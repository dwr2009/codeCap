#include <sys/time.h>
#include "DateTime.h"
#include <ErrPrintHelper.h>
#include <sys/sysinfo.h>
#include <BaseErrDef.h>
#include <sys/times.h> 
#include <asm/param.h>
#include <Compiler.h>
#include <pthread.h>

UINT64_t GetCurTimeMs(void)
{
	int iRet = 0;
	UINT64_t uiOutRet = 0;
	struct timeval tmCur;

	do{
		iRet = gettimeofday(&tmCur, NULL);
		if(0 != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		uiOutRet = (tmCur.tv_sec * 1000 + tmCur.tv_usec / 1000);
	}while(FALSE);

	return uiOutRet;
}

static ULONG g_ulPrevSysUpTimeSec = 0;
static UINT64_t g_uiAccumSysUpTimeSec = 0;

#ifdef	ANDROID
#else	//Not ANDROID
UINT64_t GetSysUpTimeSec(VOID)
{
	int iRet;
	struct sysinfo oSysInfo;

	do{
		oSysInfo.uptime = 0;
		iRet = sysinfo(&oSysInfo);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		
		if(0 == g_uiAccumSysUpTimeSec)	//uninitialized
		{
			g_uiAccumSysUpTimeSec = (ULONG)(oSysInfo.uptime);
			break;
		}
		
		g_uiAccumSysUpTimeSec += ((ULONG)(oSysInfo.uptime) - g_ulPrevSysUpTimeSec);
	}while(FALSE);
	
	g_ulPrevSysUpTimeSec = (ULONG)(oSysInfo.uptime);

	return g_uiAccumSysUpTimeSec;
}
#endif

#ifdef	ANDROID
static pthread_mutex_t g_SysUpTimeMutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
#else	//!ANDROID
static pthread_mutex_t g_SysUpTimeMutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
#endif	//ANDROID
static ULONG g_ulPrevSysJiffies = 0;
static UINT64_t g_uiAccumSysUpTimeMs = 0;

UINT64_t GetSysUpTimeMs(VOID)
{
	int iRet;
	UINT64_t uiSysUpTimeMs;
	clock_t Jiffies;
	BOOL_t bLocked = FALSE;

	do
	{
		if(CC_LIKELY(FALSE == bLocked))
		{
			iRet = pthread_mutex_lock(&g_SysUpTimeMutex);
			if(CC_UNLIKELY(0 != iRet))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				uiSysUpTimeMs = -1;
				break;
			}
			bLocked = TRUE;
		}
		
		Jiffies = times(NULL);
		//LOG_BLINE("Jiffies:%lu\n", Jiffies);
		
		if(CC_UNLIKELY(0 == g_uiAccumSysUpTimeMs))	//uninitialized
		{
			g_uiAccumSysUpTimeMs = (UINT64_t)Jiffies*1000/HZ;
			g_ulPrevSysJiffies = Jiffies;
			uiSysUpTimeMs = g_uiAccumSysUpTimeMs;
			break;
		}
		
		g_uiAccumSysUpTimeMs += ( (UINT64_t)((ULONG)Jiffies - g_ulPrevSysJiffies)*1000/HZ );
		g_ulPrevSysJiffies = Jiffies;
		uiSysUpTimeMs = g_uiAccumSysUpTimeMs;
	}while(FALSE);

	if(CC_LIKELY(bLocked))
	{
		iRet = pthread_mutex_unlock(&g_SysUpTimeMutex);
		if(CC_UNLIKELY(0 != iRet))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			uiSysUpTimeMs = -2;
		}
		bLocked = FALSE;
	}
	
	//LOG_BLINE("UpTime:%llu\n", uiSysUpTimeMs);

	return uiSysUpTimeMs;
}

INT64_t getClkMonotonicTimeUs()
{
	INT64_t clkMonotonicTimeUs;
	int iRet;
	struct timespec monotonicTime;

	iRet = clock_gettime(CLOCK_MONOTONIC, &monotonicTime);
	if(-1 == iRet)
	{
		clkMonotonicTimeUs = 0;
		PRINT_BFILE_LINENO_CRT_ERRINFO;
		goto EXIT_PROC;
	}

	clkMonotonicTimeUs = (monotonicTime.tv_sec * 1000 * 1000) + (monotonicTime.tv_nsec / 1000);

EXIT_PROC:

	return clkMonotonicTimeUs;
}

