#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#include "nexus_i2c.h"
#include "CRtcIsl1208Device.h"
#include "CNexusI2cBus.h"
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#define	TIMEZONE_ENV_VAR_NAME		"TZ"
#define DEFAULT_RTC_I2C_BUS_ID	4

static unsigned int g_iLocalParameter1=0;
typedef int (*CmdFunc)(CRtcInterface *);

struct CmdMap{
	const char *pCmdString;
	const char *pCmdDescription;
	const CmdFunc pCmdFunc;
};
static int usage(char *strCmd);

static int rtcToSystem(CRtcInterface *pRtcInterface)
{
	struct tm tmUtcTimeToSet;
	time_t timeToSetSystem;
	int iRet=-1;
	char * pszRet = NULL;
	bool bNeedRestoreTZ;
	char szOldVarTZ[256];

	iRet=pRtcInterface->getTimerFromRtcDevice();
	if(iRet!=0){
		printf("%s(%d) get rtc  Failed\n",__func__,__LINE__);
		return iRet;
	}
	tmUtcTimeToSet.tm_year = pRtcInterface->getYear()-1900;
	tmUtcTimeToSet.tm_mon = pRtcInterface->getMonth() - 1;
	tmUtcTimeToSet.tm_mday = pRtcInterface->getDay();
	tmUtcTimeToSet.tm_hour = pRtcInterface->getHour();
	tmUtcTimeToSet.tm_min = pRtcInterface->getMinute();
	tmUtcTimeToSet.tm_sec = pRtcInterface->getSecond();

	pszRet = getenv(TIMEZONE_ENV_VAR_NAME);
	if(pszRet){
		snprintf(szOldVarTZ, sizeof(szOldVarTZ), "%s", pszRet);
		szOldVarTZ[sizeof(szOldVarTZ)-1] = '\0';			
		bNeedRestoreTZ = true;
	}
	iRet = setenv(TIMEZONE_ENV_VAR_NAME, "UTC0", 1);
	if(0 != iRet)
	{
		printf("%s(%d) setenv %s UTC0 Failed\n",__func__,__LINE__,TIMEZONE_ENV_VAR_NAME);
	}
	do{
		timeToSetSystem = mktime(&tmUtcTimeToSet);
		iRet = stime(&timeToSetSystem);
		if(0 != iRet)
		{
			printf("%s(%d) sTime  Failed\n",__func__,__LINE__);
			break;
		}
		printf("%d/%d/%d %02d:%02d:%02d\n",
		pRtcInterface->getYear(),
		pRtcInterface->getMonth(),
		pRtcInterface->getDay(),
		pRtcInterface->getHour(),
		pRtcInterface->getMinute(),
		pRtcInterface->getSecond());
	}while(false);
	if(bNeedRestoreTZ){
		iRet = setenv(TIMEZONE_ENV_VAR_NAME, szOldVarTZ, 1);
		if(0 != iRet)
		{
			printf("%s(%d) setenv %s  %s Failed\n",
			__func__,__LINE__,TIMEZONE_ENV_VAR_NAME,szOldVarTZ);
		}
	}
	return iRet;	
}

static int systemToRtc(CRtcInterface *pRtcInterface)
{
	struct tm *ptmToRtc=NULL;
	time_t timeToRtc;
	int iRet=-1;
	char * pszRet = NULL;
	bool bNeedRestoreTZ;
	char szOldVarTZ[256];
	

	
	pszRet = getenv(TIMEZONE_ENV_VAR_NAME);
	if(pszRet){
		snprintf(szOldVarTZ, sizeof(szOldVarTZ), "%s", pszRet);
		szOldVarTZ[sizeof(szOldVarTZ)-1] = '\0';			
		bNeedRestoreTZ = true;
	}
	iRet = setenv(TIMEZONE_ENV_VAR_NAME, "UTC0", 1);
	if(0 != iRet)
	{
		printf("%s(%d) setenv %s UTC0 Failed\n",__func__,__LINE__,TIMEZONE_ENV_VAR_NAME);
	}
	timeToRtc=time(NULL);
	ptmToRtc=localtime(&timeToRtc);	
	pRtcInterface->setYear(ptmToRtc->tm_year);
	pRtcInterface->setMonth(ptmToRtc->tm_mon+1);
	pRtcInterface->setDay(ptmToRtc->tm_mday);
	pRtcInterface->setHour(ptmToRtc->tm_hour);
	pRtcInterface->setMinute(ptmToRtc->tm_min);
	pRtcInterface->setSecond(ptmToRtc->tm_sec);

	//printf("year %d\n",ptmToRtc->tm_year);
	printf("%d/%d/%d %02d:%02d:%02d\n",
		pRtcInterface->getYear(),
		pRtcInterface->getMonth(),
		pRtcInterface->getDay(),
		pRtcInterface->getHour(),
		pRtcInterface->getMinute(),
		pRtcInterface->getSecond());

	iRet=pRtcInterface->setTimerToRtcDevice();
	if(bNeedRestoreTZ){
		iRet = setenv(TIMEZONE_ENV_VAR_NAME, szOldVarTZ, 1);
		if(0 != iRet)
		{
			printf("%s(%d) setenv %s  %s Failed\n",
			__func__,__LINE__,TIMEZONE_ENV_VAR_NAME,szOldVarTZ);
		}
	}
	return iRet;
}

static int readRtcFromDevice(CRtcInterface *pRtcInterface)
{
	int iRet=0;
	iRet=pRtcInterface->getTimerFromRtcDevice();
	if(iRet){
		printf("read rtc failed\n");
		return iRet;
	}
	printf("%d/%d/%d %02d:%02d:%02d\n",
		pRtcInterface->getYear(),
		pRtcInterface->getMonth(),
		pRtcInterface->getDay(),
		pRtcInterface->getHour(),
		pRtcInterface->getMinute(),
		pRtcInterface->getSecond());
	return iRet;	
}

static int enableXal(CRtcInterface *pRtcInterface)
{
    return pRtcInterface->enableXal();
}

static int disableXal(CRtcInterface *pRtcInterface)
{
    return pRtcInterface->disableXal();
}

static int enableLowPower(CRtcInterface *pRtcInterface)
{
    return pRtcInterface->enableLowPower();
}

static int disableLowPower(CRtcInterface *pRtcInterface)
{
    return pRtcInterface->disableLowPower();
}

static int resetRtc(CRtcInterface *pRtcInterface)
{
    return pRtcInterface->resetRtc();
}

static int setCapacity(CRtcInterface *pRtcInterface)
{
    pRtcInterface->setCapacity(g_iLocalParameter1);
    return 0;
}
static const struct CmdMap g_CmdMapTable[]={
{"--hwtosystem","sync rtc to system",rtcToSystem},
{"--systemtohw","sync system to rtc",systemToRtc},
{"--read","show current rtc",readRtcFromDevice},
{"--disableXal","use extern xal",disableXal},
{"--enableXal","don't use extern xal",enableXal},
{"--enableLowPower","enable low power mode",enableLowPower},
{"--disableLowPower","disable low power mode",disableLowPower},
{"--resetRtc","reset rtc",resetRtc},
{"--setCapacity","set capacity value to rtc chip ,--setCapacity 8",setCapacity},
{"--help","show help info",NULL},
}; 

static int usage(char *strCmd)
{
	unsigned int iIndex=0;
	const struct CmdMap *pCmdMap=NULL;
	printf("Usage:\n");
	printf("%s  CMD [ --bus 0|1|2|3 ]\n",strCmd);
	printf("CMD is :\n");
	for(iIndex=0,pCmdMap=g_CmdMapTable;iIndex<sizeof(g_CmdMapTable)/sizeof(g_CmdMapTable[0]);iIndex++,pCmdMap++){
		printf("%s  %s\n",pCmdMap->pCmdString,pCmdMap->pCmdDescription);
	}	
	return 0;
}


int main(int argc,char **argv)
{

	int iRet=0;
	CmdFunc pCmdFunc=NULL;
	CNexusI2cBus *pNexusI2cBus=NULL;	
	CRtcIsl1208Device *pRtcIsl1208Device=NULL;	
	unsigned int iIndex=0;
	int iI2cBus=DEFAULT_RTC_I2C_BUS_ID;
	const struct CmdMap *pCmdMap=NULL;
	if(argc<2){
		usage(argv[0]);
		return iRet;
	}
	pCmdFunc=NULL;

    for(iIndex=0,pCmdMap=g_CmdMapTable;iIndex<sizeof(g_CmdMapTable)/sizeof(g_CmdMapTable[0]);iIndex++,pCmdMap++)
    {
        if(strcmp(argv[1],pCmdMap->pCmdString)==0)
        {
            pCmdFunc=pCmdMap->pCmdFunc;
            break;
        }
    }
    if(NULL==pCmdFunc)
    {
        usage(argv[0]);
        return iRet;
    }
    if((argc>2)&&(strcmp(argv[1],"--setCapacity")==0))
    {
        g_iLocalParameter1=atoi(argv[2]);
    }
    
    if(argc>=4)
    {
        if(strcmp(argv[2],"--bus")==0)
        {
            int iBusId=DEFAULT_RTC_I2C_BUS_ID;
            iBusId=atoi(argv[3]);
            if((iBusId>=0)&&(iBusId<=3))
            {
                iI2cBus=iBusId;
            }
        }
    }
    pNexusI2cBus=new CNexusI2cBus(iI2cBus);
    pRtcIsl1208Device=new CRtcIsl1208Device(pNexusI2cBus);
    
    iRet=pCmdFunc(pRtcIsl1208Device);


	delete pRtcIsl1208Device;
	delete pNexusI2cBus;
	
	return iRet;
}

