#ifndef CBASETUNERIFACE_H__
#define CBASETUNERIFACE_H__

#include <BaseTypeDef.h>
#include <DtvType.h>

class CBaseTunerIface
{
public:
	CBaseTunerIface(){};
	virtual ~CBaseTunerIface(){};
public:
	virtual INT_t InitVal()=0;
	virtual INT_t OpenTunerModule()=0;
	virtual INT_t SetScanParam()=0;
	virtual INT_t SetNextFreqChannel(UINT32_t *pLock,UINT16_t *pScanProgress)=0;
	virtual INT_t SetStandard(IN StandardChannle iStandard,IN INT32_t iConstellation)=0;
	virtual INT_t GetStandard(OUT StandardChannle *pOStandard)=0;
	virtual INT_t GetChannleInfo(INT32_t *pFreq,UINT8 *pBw)=0;
	virtual INT_t GetChannelStrength(UINT16_t *pOutStrength)=0;
	virtual INT_t SetChannelInfo(INT32_t Freq)=0;
	virtual INT_t WaitForLock(UINT32_t *pLock)=0;
	virtual INT_t GetCurrConstellation(OUT UINT32_t *pConstellation)=0;
	virtual INT_t ReInitModules()=0;
};
#endif	//CBASETUNERIFACE_H__

