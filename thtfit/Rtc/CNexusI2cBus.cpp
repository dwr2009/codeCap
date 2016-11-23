
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#include "nexus_i2c.h"

#include "CNexusI2cBus.h"
#include "CI2cInterface.h"
#include <DateTime.h>

CNexusI2cBus::CNexusI2cBus(int iI2cBusId)
		:CI2cInterface(iI2cBusId)	
{
	initI2cBus();
}

CNexusI2cBus::~CNexusI2cBus()
{
	exitI2cBus();
}


int CNexusI2cBus::initI2cBus()
{
	int iRet=-1;
	NxClient_JoinSettings joinSettings;
	NEXUS_I2cHandle hI2cHandle=NULL;
	long long lOldTimer=0,lCurTimer=0,lDiffTimer=0;
	const long long lWaitMaxTimer=30000;

	if(m_bInited)
		return 0;
	NxClient_GetDefaultJoinSettings(&joinSettings);

	lOldTimer=GetSysUpTimeMs();
	lCurTimer=GetSysUpTimeMs();

	lDiffTimer=lCurTimer-lOldTimer;	
	do{
		iRet = NxClient_Join(&joinSettings);
		if(0==iRet){
			//printf("%s(%d) NxClient_Join is Ok\n",__func__,__LINE__); 		
			break;
		}		
		sleep(1);
		lCurTimer=GetSysUpTimeMs();
		lDiffTimer=lCurTimer-lOldTimer;
	}while(lDiffTimer<lWaitMaxTimer);	
	if( iRet)
	{
		printf("%s(%d) failed.\n",__func__,__LINE__);
		return iRet;
	}
	NEXUS_I2c_GetDefaultSettings(&m_i2cSetting);
	m_i2cSetting.clockRate=NEXUS_I2cClockRate_e100Khz;
	m_i2cSetting.interruptMode = true;
	m_i2cSetting.burstMode = false;
	m_i2cSetting.timeout = 0;	//use defalt
	m_i2cSetting.softI2c = false;
	m_i2cSetting.fourByteXferMode = false;
	m_i2cSetting.use5v = false;
	m_i2cSetting.clockGpio = NULL;
	m_i2cSetting.dataGpio = NULL;
	
	hI2cHandle=NEXUS_I2c_Open(m_iBusId,&m_i2cSetting);
	if(NULL==hI2cHandle){
		printf("%s(%d) failed.\n",__func__,__LINE__);
		return iRet;
	}
	m_hI2cHandle=hI2cHandle;
	m_bInited=true;

}

int CNexusI2cBus::exitI2cBus()
{

	int iRet=-1;

	if(!m_bInited)
		return 0;
	if(m_hI2cHandle!=NULL)
		NEXUS_I2c_Close(m_hI2cHandle);
	m_hI2cHandle=NULL;
	NxClient_Uninit(); 
	m_bInited=false;
	return iRet;
}

int CNexusI2cBus::readDataFromDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData,int iDataLen)
{
	int iRet=-1;
	if((NULL==m_hI2cHandle)||(NULL==pData)){
		printf("%s(%d) pData or m_hI2cHandle is null\n",__func__,__LINE__);
		return -1;
	}
	iRet=NEXUS_I2c_Read(m_hI2cHandle,iChipId,iAddr,pData,iDataLen);
	return iRet;

}

int CNexusI2cBus::writeDataToDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData,int iDataLen)
{
	int iRet=-1;	
	if((NULL==m_hI2cHandle)||(NULL==pData)){
		printf("%s(%d) pData or m_hI2cHandle is null\n",__func__,__LINE__);
		return -1;
	}
	iRet=NEXUS_I2c_Write(m_hI2cHandle,iChipId,iAddr,pData,iDataLen);
	return iRet;

}




