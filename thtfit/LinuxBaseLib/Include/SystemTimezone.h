#ifndef	_SYSTEM_TIME_ZONE_H_
#define	_SYSTEM_TIME_ZONE_H_

#include <BaseTypeDef.h>

#define	TIMEZONE_ENV_VAR_NAME		"TZ"

class CSystemTimezone
{
public:
	static INT_t setSystemGlobalTimeZone(int TimeZoneHourOff);
	static INT_t getSysUtcTime(OUT time_t & timeUtc);
};

#endif	//_SYSTEM_TIME_ZONE_H_

