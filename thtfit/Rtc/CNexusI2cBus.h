#ifndef __CNEXUSI2CBUS_H__
#define __CNEXUSI2CBUS_H__

#include "CI2cInterface.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#include "nexus_i2c.h"

#define DEFAULT_RTC_I2C_BUS_ID	4

class CNexusI2cBus:public CI2cInterface
{
public:
	CNexusI2cBus(int iI2cBusId=0);
	virtual ~CNexusI2cBus();
	virtual int readDataFromDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData=NULL,int iDataLen=0);
	virtual int writeDataToDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData=NULL,int iDataLen=0);
protected:
	NEXUS_I2cSettings m_i2cSetting;
	NEXUS_I2cHandle m_hI2cHandle;
private:
	int initI2cBus();
	int exitI2cBus();
};

#endif

