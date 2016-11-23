
#ifndef __CI2CINTERFACE_H__
#define __CI2CINTERFACE_H__
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#include "nexus_i2c.h"

class CI2cInterface
{
public:
	CI2cInterface(int iBusId)
	:m_bInited(false),m_iBusId(iBusId)
	{
	
	};
	CI2cInterface():m_bInited(false),m_iBusId(0)
	{
	
	};	
	void setBusId(int iBusId){
		m_iBusId=iBusId;
	};
	int getBusId(){
		return m_iBusId;
	}
	bool isInited()
	{ 
		return m_bInited;
	}
	virtual ~CI2cInterface(){};
	virtual int readDataFromDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData=NULL,int iDataLen=0)=0;
	virtual int writeDataToDevice(unsigned char iChipId,unsigned int iAddr,unsigned char *pData=NULL,int iDataLen=0)=0;
protected:
	bool m_bInited;
	int m_iBusId;

};

#endif

