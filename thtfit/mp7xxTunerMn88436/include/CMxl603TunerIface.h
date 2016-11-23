#ifndef _CMXL603TUNERIFACE_H__
#define _CMXL603TUNERIFACE_H__

#include "MaxLinearDataTypes.h"
#include "MxL603_TunerApi.h"
#include "MxL603_OEM_Drv.h"
#include "MN_DMD_driver.h"
#include "../baseIfce/CBaseTunerIface.h"

#define TOTAL_FREQ_NUM_USA_AIR 		82
#define TOTAL_FREQ_NUM_USA_CABLE 	157
#define BrcmGpioNo_RESET 59
#define I2cTuner_Power 	 58



class CMxl603TunerIface : public CBaseTunerIface
{
public:
	CMxl603TunerIface();
	~CMxl603TunerIface();
	//INIT OPEN
	virtual INT_t OpenTunerModule();
	virtual INT_t InitVal();
	//SCAN
	virtual INT_t SetScanParam();
	virtual INT_t SetNextFreqChannel(UINT32_t *pLock,UINT16_t *pScanProgress);
	//Standard
	virtual INT_t SetStandard(IN StandardChannle iStandard,IN INT32_t iConstellation);
	virtual INT_t GetStandard(OUT StandardChannle *pOStandard);
	//Channel info
	virtual INT_t GetChannleInfo(INT32_t *pFreq,UINT8 *pBw);
	virtual INT_t SetChannelInfo(INT32_t Freq);

	//Get strength
	virtual INT_t GetChannelStrength(UINT16_t *pOutStrength);
	//LOCK
	virtual INT_t WaitForLock(UINT32_t *pLock);
	//Constellation
	virtual INT_t GetCurrConstellation(OUT UINT32_t *pConstellation);
	//Reinit Tuner Modules
	virtual INT_t ReInitModules();
private:
	INT_t UnInit();
	INT_t SetSystemModeTuner(UINT8 tunerId,PMXL603_TUNER_MODE_CFG_T pCfgTunerMode);
	void ResetDemodParaInfo();
	INT_t GetNextFreq(INT32_t *pFreq);
	INT_t GetCurrentScanProgress(UINT16_t *pProgress);
	INT_t ResetTunerHardware();
private:
	BOOL_t m_bInited;
	
private:
	//demod
	DMD_PARAMETER_t	m_DemodPara;
	MXL603_TUNER_MODE_CFG_T m_CfgTunerMode;
	MXL603_CHAN_TUNE_CFG_T	m_TunerChanInfo;
	ScanFreqParam_t m_ScanFreqParam;
	UINT32_t m_Constellation;
};

#endif	//_CMXL603TUNERIFACE_H__

