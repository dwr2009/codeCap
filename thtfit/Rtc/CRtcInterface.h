
#ifndef __CRTCINTERFACE_H__
#define __CRTCINTERFACE_H__
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#include "nexus_i2c.h"

class CRtcInterface
{
public:
    CRtcInterface():m_iYear(1970),m_iMonth(1),m_iDay(1),m_iHour(0), m_iMinute(0),m_iSecond(0)
    {
    };
    virtual ~CRtcInterface(){};
    virtual int getTimerFromRtcDevice()=0;
    virtual int setTimerToRtcDevice()=0;
    virtual int initRtcDevice()=0;
    virtual int exitRtcDevice()=0;

    virtual int getYear(){return m_iYear;};
    virtual int getMonth(){return m_iMonth;};
    virtual int getDay(){return m_iDay;};
    virtual int getHour(){return m_iHour;};
    virtual int getMinute(){return m_iMinute;};
    virtual int getSecond(){return m_iSecond;};
    virtual int getWDay(){return m_iWDay;};


    virtual void setYear(unsigned int iYear=2000)
    {
        m_iYear=iYear;
    };
    virtual void setMonth(unsigned int iMonth=1){m_iMonth=iMonth;};
    virtual void setDay(unsigned int iDay=1){m_iDay=iDay;};
    virtual void setHour(unsigned int iHour=0){m_iHour=iHour;};
    virtual void setMinute(unsigned int iMinute=0){m_iMinute=iMinute;};
    virtual void setSecond(unsigned int iSecond=0){m_iSecond=iSecond;};
    virtual void setWDay(unsigned int iWDay){m_iWDay=iWDay;};
    virtual int enableXal(){ return 0;};
    virtual int disableXal(){ return 0;};
    virtual int enableLowPower(){ return 0;};
    virtual int disableLowPower(){ return 0;};
    virtual int resetRtc(){ return 0;};
    virtual int setCapacity(unsigned char iValue)
    {
        int iTmp=iValue;
        iValue=iTmp;
        return 0;
    };
protected:
    unsigned int m_iYear;
    unsigned int m_iMonth;
    unsigned int m_iDay;
    unsigned int m_iHour;
    unsigned int m_iMinute;
    unsigned int m_iSecond;
    unsigned int m_iWDay;
};


#endif


