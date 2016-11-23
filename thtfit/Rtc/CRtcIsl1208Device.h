

#ifndef __CRTCISL1208_H__
#define  __CRTCISL1208_H__
#include "CRtcInterface.h"
#include "CI2cInterface.h"

enum{
	ISL1208_SECOND_INDEX=0,
	ISL1208_MINUTE_INDEX,
	ISL1208_HOUR_INDEX,
	ISL1208_DAY_INDEX,
	ISL1208_MONTH_INDEX,
	ISL1208_YEAR_INDEX	
};
#define	REG_ADDR_CTRL_STAT_07	0x07

#define	BAT_BIT_SHIFT      1
#define	WRTC_BIT_SHIFT      4

#define XTOSCB_BIT_SHIFT        6
#define ARST_BIT_SHIFT      7

#define	REG_ADDR_INT_STAT_08       0x8
#define LPMODE_BIT_SHIFT        5

#define	REG_ADDR_CAPACITY_0A       0x0a
#define CAPACITY_MASK   0x3f

#define ISL1208_CHIP_ID     0x6f

class CRtcIsl1208Device:virtual public CRtcInterface
{
public:
	CRtcIsl1208Device(CI2cInterface *pI2cInterface=NULL);
	virtual ~CRtcIsl1208Device();
	void setI2cBusInterface(CI2cInterface *pI2cInterface=NULL){m_pI2cInterface=pI2cInterface;};
	virtual int getTimerFromRtcDevice();
	virtual int setTimerToRtcDevice();
	virtual int initRtcDevice();
	virtual int exitRtcDevice();
    virtual int enableXal();
    virtual int disableXal();
    virtual int enableLowPower();
    virtual int disableLowPower();
    virtual int resetRtc();
    virtual int setCapacity(unsigned char iValue);
protected:
	CI2cInterface *m_pI2cInterface;
private:
	const int m_iChipId;
	int enableWRTC(bool bEnable=true);
};


#endif

