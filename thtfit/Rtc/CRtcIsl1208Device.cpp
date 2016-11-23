

#include "CRtcIsl1208Device.h"
#include "CRtcInterface.h"
#include "CI2cInterface.h"

CRtcIsl1208Device::CRtcIsl1208Device(CI2cInterface *pI2cInterface)
	:m_pI2cInterface(pI2cInterface),m_iChipId(ISL1208_CHIP_ID)
{

}

CRtcIsl1208Device::~CRtcIsl1208Device()
{

}
int CRtcIsl1208Device::enableWRTC(bool bEnable)
{
	int iRet=-1;
	const int iBufferCount=6;
	unsigned char byteBuffer[iBufferCount];
	bool bIs24Hours=false;

	if(NULL==m_pI2cInterface){
		printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
		return iRet;
	}
	memset(byteBuffer,0x00,sizeof(byteBuffer));
	iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
	if(iRet){
		printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
		return iRet;
	}
	byteBuffer[0]&= ~(1 << WRTC_BIT_SHIFT);
	if(bEnable){
		byteBuffer[0]|=(1 << WRTC_BIT_SHIFT);
	}
    byteBuffer[0] &= ~(1<<BAT_BIT_SHIFT);
	iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
	if(iRet){
		printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
		return iRet;
	}
	return iRet;
}
int CRtcIsl1208Device::getTimerFromRtcDevice()
{
	int iRet=-1;
	const int iBufferCount=6;
	unsigned char byteBuffer[iBufferCount];
	bool bIs24Hours=false;

	if(NULL==m_pI2cInterface){
		printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
		return iRet;
	}
	memset(byteBuffer,0x00,sizeof(byteBuffer));

	iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,0x00,byteBuffer,sizeof(byteBuffer));
	if(iRet){
		printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
		return iRet;
	}
	m_iSecond=byteBuffer[ISL1208_SECOND_INDEX]>>4;
	m_iSecond&=0x07;
	m_iSecond*=10;
	m_iSecond+=byteBuffer[ISL1208_SECOND_INDEX]&0x0f;


	m_iMinute=((byteBuffer[ISL1208_MINUTE_INDEX] & 0x70) >> 4) * 10 + (byteBuffer[ISL1208_MINUTE_INDEX]& 0x0F);

	bIs24Hours=(byteBuffer[ISL1208_HOUR_INDEX]&0x80)?true:false;
	if(bIs24Hours){
		m_iHour=((byteBuffer[ISL1208_HOUR_INDEX] & 0x30) >> 4) * 10 + (byteBuffer[ISL1208_HOUR_INDEX] & 0x0F);
	}else{
		bool bHR21_bit = (byteBuffer[ISL1208_HOUR_INDEX] & 0x20) >> 5;
		m_iHour = ((byteBuffer[ISL1208_HOUR_INDEX] & 0x10) >> 4) * 10 + (byteBuffer[ISL1208_HOUR_INDEX] & 0x0F);
		if(bHR21_bit)	//PM
		{
			m_iHour+= 12;
		}
	}
		
	m_iDay= ((byteBuffer[ISL1208_DAY_INDEX] & 0x30) >> 4) * 10 + (byteBuffer[ISL1208_DAY_INDEX] & 0x0F);
	m_iMonth= ((byteBuffer[ISL1208_MONTH_INDEX] & 0x10) >> 4) * 10 + (byteBuffer[ISL1208_MONTH_INDEX] & 0x0F);
	m_iYear= ((byteBuffer[ISL1208_YEAR_INDEX] & 0xf0) >> 4) * 10 + (byteBuffer[ISL1208_YEAR_INDEX] & 0x0F);
	
	if(m_iYear<100)
		m_iYear+=2000;
	/*	
	printf("%s(%d) %d/%d/%d %02d:%02d:%02d\n",__func__,__LINE__,
		m_iYear,m_iMonth,m_iDay,m_iHour,m_iMinute,m_iSecond);
	*/
	return iRet;
}

int CRtcIsl1208Device::setTimerToRtcDevice()
{
	int iRet=-1;
	const int  iBufferCount=6;
	unsigned char byteBuffer[iBufferCount];
	

	if(NULL==m_pI2cInterface){
		printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
		return iRet;
	}
	byteBuffer[ISL1208_YEAR_INDEX]=((((m_iYear)%100)/10)<<4)|(((m_iYear)%100)%10);

	byteBuffer[ISL1208_MONTH_INDEX]=((m_iMonth/10)<<4)|(m_iMonth%10);
	byteBuffer[ISL1208_DAY_INDEX]=((m_iDay/10)<<4)|(m_iDay%10);

	byteBuffer[ISL1208_HOUR_INDEX]=((m_iHour/ 10) << 4) | (m_iHour% 10);
	byteBuffer[ISL1208_HOUR_INDEX]|=0x80;
	
	byteBuffer[ISL1208_MINUTE_INDEX]=((m_iMinute / 10) << 4) | (m_iMinute % 10);
	byteBuffer[ISL1208_SECOND_INDEX]=((m_iSecond/ 10) << 4) | (m_iSecond % 10);
#if defined(LIB_CONFIG_CAPACITY) && (LIB_CONFIG_CAPACITY==1)
    setCapacity(32);
#else
#error failed LIB_CONFIG_CAPACITY config
#endif
    //disable internal Xal,use external Xal
    disableXal();
	enableWRTC(true);
	iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,0x00,byteBuffer,sizeof(byteBuffer));
	if(iRet){
		printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);		
	}
	enableWRTC(false);
    
	return iRet;
}

int CRtcIsl1208Device::initRtcDevice()
{
	int iRet=-1;
	return iRet;
}

int CRtcIsl1208Device::exitRtcDevice()
{
	int iRet=-1;
	return iRet;
}
int CRtcIsl1208Device::enableXal()
{
    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
    unsigned char iCtrlState=0;
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    printf("CTRL_STAT old value is %02x\n",byteBuffer[0]);
    byteBuffer[0]|=(1 << XTOSCB_BIT_SHIFT);       
    byteBuffer[0] &= ~(1<<BAT_BIT_SHIFT);
    iCtrlState=byteBuffer[0];
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    if(iCtrlState!=byteBuffer[0])
    {
        printf("%s(%d) write value failed \n",__func__,__LINE__);
        return -1;
    }
    return 0;
}
int CRtcIsl1208Device::disableXal()
{
    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
    unsigned char iCtrlState=0;
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    printf("CTRL_STAT old value is %02x\n",byteBuffer[0]);
    byteBuffer[0] &= ~(1 << XTOSCB_BIT_SHIFT);       
    byteBuffer[0] &= ~(1<<BAT_BIT_SHIFT);
    iCtrlState=byteBuffer[0];
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    if(iCtrlState!=byteBuffer[0])
    {
        printf("%s(%d) write value failed \n",__func__,__LINE__);
        return -1;
    }
    return 0;
}

int CRtcIsl1208Device::enableLowPower()
{
    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
    unsigned char iCtrlState=0;
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    printf("CTRL_STAT old value is %02x\n",byteBuffer[0]);
    byteBuffer[0]|=(1 << LPMODE_BIT_SHIFT);    
    iCtrlState=byteBuffer[0];
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    if(iCtrlState!=byteBuffer[0])
    {
        printf("%s(%d) write value failed \n",__func__,__LINE__);
        return -1;
    }
    return 0;
}
int CRtcIsl1208Device::disableLowPower()
{

    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
    unsigned char iCtrlState=0;
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    printf("CTRL_STAT old value is %02x\n",byteBuffer[0]);
    byteBuffer[0] &= ~(1 << LPMODE_BIT_SHIFT);       
    iCtrlState=byteBuffer[0];
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_INT_STAT_08,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    if(iCtrlState!=byteBuffer[0])
    {
        printf("%s(%d) write value failed \n",__func__,__LINE__);
        return -1;
    }
    return 0;
}
int CRtcIsl1208Device::resetRtc()
{
    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
    unsigned char iCtrlState=0;
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
   
    byteBuffer[0] |= ~(1 << ARST_BIT_SHIFT);       
    
    iCtrlState=byteBuffer[0];
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_CTRL_STAT_07,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    
    return 0;
}

int CRtcIsl1208Device::setCapacity(unsigned char iValue)
{
    int iRet=-1;
    const int iBufferCount=6;
    unsigned char byteBuffer[iBufferCount];
   
    
    if(NULL==m_pI2cInterface)
    {
        printf("%s(%d) m_pI2cInterface is null\n",__func__,__LINE__);
        return iRet;
    }
    memset(byteBuffer,0x00,sizeof(byteBuffer));
    iRet=m_pI2cInterface->readDataFromDevice(m_iChipId,REG_ADDR_CAPACITY_0A,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus read err \n",__func__,__LINE__);
        return iRet;
    }
    iValue&=CAPACITY_MASK;
    byteBuffer[0]&=~CAPACITY_MASK;
    byteBuffer[0]|=iValue;       
    
    iRet=m_pI2cInterface->writeDataToDevice(m_iChipId,REG_ADDR_CAPACITY_0A,byteBuffer,0x01);
    if(iRet)
    {
        printf("%s(%d) i2c Bus write err \n",__func__,__LINE__);
        return iRet;
    }
    
    return 0;
    
}

