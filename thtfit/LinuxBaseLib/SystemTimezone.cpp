#include "SystemTimezone.h"
#include <BaseErrDef.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <ErrPrintHelper.h>
#include <time.h>
#include "StackBufString.h"
INT_t CSystemTimezone::setSystemGlobalTimeZone(int TimeZoneHourOff)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszLocalZoneinfoFile = NULL;
	LPCSTR pszLocalTimeFile = "/etc/localtime";

	do
	{
		if(12 < TimeZoneHourOff || -12 > TimeZoneHourOff)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(12 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-12";
		}
		else if(11 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-11";
		}
		else if(10 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-10";
		}
		else if(9 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-9";
		}
		else if(8 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-8";
		}
		else if(7 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-7";
		}
		else if(6 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-6";
		}
		else if(5 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-5";
		}
		else if(4 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-4";
		}
		else if(3 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-3";
		}
		else if(2 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-2";
		}
		else if(1 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT-1";
		}
		else if(0 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/GMT+0";
		}
		else if(-1 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+1";
		}
		else if(-2 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+2";
		}
		else if(-3 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+3";
		}
		else if(-4 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+4";
		}
		else if(-5 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+5";
		}
		else if(-6 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+6";
		}
		else if(-7 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+7";
		}
		else if(-8 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+8";
		}
		else if(-9 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+9";
		}
		else if(-10 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+10";
		}
		else if(-11 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+11";
		}
		else if(-12 == TimeZoneHourOff)
		{
			pszLocalZoneinfoFile = "/usr/share/zoneinfo/Etc/GMT+12";
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
			
		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			strShellCmd.Format("mount -o rw,remount /");
			system(strShellCmd);
			
			iRet = remove(pszLocalTimeFile);
			if(0 != iRet)
			{
				if(ENOENT != errno)
				{
					iOutRet = ERR_SYMLINK_FAIL;
					break;
				}
			}
			iRet = symlink(pszLocalZoneinfoFile, pszLocalTimeFile);
			if(0 != iRet)
			{
				iOutRet = ERR_SYMLINK_FAIL;
				break;
			}

			strShellCmd.Format("mount -o ro,remount /");
			system(strShellCmd);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSystemTimezone::getSysUtcTime(OUT time_t & timeUtc)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	char * pszOldTzEnvVal = NULL;
	char szOldVarTZ[256];
	BOOL_t bNeedRestoreTZ = FALSE;

	do
	{
		//save the env var TZ
		pszOldTzEnvVal = getenv(TIMEZONE_ENV_VAR_NAME);
		if(pszOldTzEnvVal)
		{
			snprintf(szOldVarTZ, sizeof(szOldVarTZ), "%s", pszOldTzEnvVal);
			szOldVarTZ[sizeof(szOldVarTZ)-1] = '\0';			
		}

		//set TZ to UTC time
		iRet = setenv(TIMEZONE_ENV_VAR_NAME, "UTC0", 1);
		if(0 != iRet)
		{
			iOutRet = ERR_SYS_SET_ENV_FAIL;
			break;
		}
		bNeedRestoreTZ = TRUE;

		timeUtc = time(NULL);
	}while(FALSE);

	//restore the env var TZ
	if(bNeedRestoreTZ)
	{
		bNeedRestoreTZ = false;
		if(pszOldTzEnvVal)
		{
			iRet = setenv(TIMEZONE_ENV_VAR_NAME, szOldVarTZ, 1);
			if(0 != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		else	//unset this env
		{
			iRet = unsetenv(TIMEZONE_ENV_VAR_NAME);
			if(0 != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
	}

	return iOutRet;
}

