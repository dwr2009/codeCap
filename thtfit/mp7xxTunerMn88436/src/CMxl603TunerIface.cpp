#ifndef	ALLOW_OS_CODE
#define	ALLOW_OS_CODE
#endif	//ALLOW_OS_CODE


#include <BaseErrDef.h>
#include <Compiler.h>
#include <memory.h>
#include <ErrPrintHelper.h>
#include <DtvType.h>
#include "../include/CMxl603TunerIface.h"
#include "../include/MN_DMD_device.h"
#include "../include/MN_I2C.h"
#include <unistd.h>

#include <nexus_base_types.h>
#include <nexus_i2c.h>

static ScanFrequencyPlan_t ScanFrequencyPlan_AIR[] = {
	{ 57000,  69000, 6000, 3},
	{ 79000,  85000, 6000, 2},
	{ 177000, 213000, 6000, 7},
	{ 473000, 887000, 6000, 70}
};

static ScanFrequencyPlan_t ScanFrequencyPlan_CABLE[] = {
	{  57000,  69000, 6000, 3}, 
	{  79000,  85000, 6000, 2},
	{  93000, 117000, 6000, 5},
	{ 123000, 171000, 6000, 9},
	{ 177000, 213000, 6000, 7},
	{ 219000, 645000, 6000, 72},
	{ 651000, 999000, 6000, 59}
};

INT_t CMxl603TunerIface::OpenTunerModule()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	DMD_ERROR_t IRetDmd = DMD_E_OK;
	MXL_STATUS status = MXL_SUCCESS;
	UINT8 devId = 0;
	MXL_BOOL singleSupply_3_3V = MXL_DISABLE;
	MXL603_XTAL_SET_CFG_T xtalCfg;
	MXL603_IF_OUT_CFG_T ifOutCfg;
	MXL603_AGC_CFG_T agcCfg;

	do{
		if(FALSE == m_bInited)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		ZeroMemory(&xtalCfg,sizeof(MXL603_XTAL_SET_CFG_T));
		ZeroMemory(&ifOutCfg,sizeof(MXL603_IF_OUT_CFG_T));
		ZeroMemory(&agcCfg,sizeof(MXL603_AGC_CFG_T));

		iRet = ResetTunerHardware();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		//Init nexus i2c 
		iRet = DMD_I2C_open();
		if(DMD_E_OK != iRet)
		{
			LOG_BLINE("I2C OPEN ERROR :%d\n",iRet);
			iOutRet = ERROR_I2C_READ_FAIL;
			break;
		}
		
		//Demodulator LSI Initialize
		IRetDmd = DMD_device_init(&m_DemodPara);
		if(DMD_E_OK != IRetDmd)
		{
			iOutRet = ERR_TU_INITDEMOD_ERROR;
			break;
		}
		
		if( DMD_E_OK != DMD_device_load_pseq( &m_DemodPara ) )
		{
			iOutRet = ERR_TU_INITDEMOD_ERROR;
			break;
		}

		sleep(2);
		/****************INIT TUNER********************************/
		devId = MXL603_I2C_ADDR;
		// 1 Reset
		status = MxLWare603_API_CfgDevSoftReset(devId);
		if (MXL_SUCCESS != status )
		{
			LOG_BLINE("ERROR,SoftReset Mxl603 %d\n",status);
			iOutRet = ERR_TU_RESET_FAIL;
			break;
		}

		// 2 Overwrite Default
		singleSupply_3_3V = MXL_ENABLE;
		status = MxLWare603_API_CfgDevOverwriteDefaults(devId, singleSupply_3_3V);
		if (MXL_SUCCESS != status)
		{
			LOG_BLINE("Error! MxLWare603_API_CfgDevOverwriteDefaults %d\n",status);
			iOutRet = ERR_TUNER_OPEN_FAIL;
			break;
		}

		// 3 XTAL SETTING
		/* Single XTAL for tuner and demod sharing*/
		xtalCfg.xtalFreqSel = MXL603_XTAL_24MHz;
		xtalCfg.xtalCap = 25;
		xtalCfg.clkOutEnable = MXL_ENABLE;
		xtalCfg.singleSupply_3_3V = MXL_ENABLE;
		xtalCfg.clkOutDiv = MXL_DISABLE;
		xtalCfg.clkOutExt = MXL_DISABLE;
		xtalCfg.XtalSharingMode = MXL_DISABLE;
		status = MxLWare603_API_CfgDevXtal(devId, xtalCfg);
		if (MXL_SUCCESS != status)
		{
			LOG_BLINE("Error! MxLWare603_API_CfgDevXtal\n"); 
			iOutRet = ERR_TUNER_OPEN_FAIL;
			break;
		}

		// 4 : IF Out setting
		//IF freq set, should match Demod request 
		ifOutCfg.ifOutFreq = MXL603_IF_5MHz;
		ifOutCfg.ifInversion = MXL_DISABLE;
		ifOutCfg.gainLevel = 11;
		ifOutCfg.manualFreqSet = MXL_DISABLE;
		ifOutCfg.manualIFOutFreqInKHz = 0;
		status = MxLWare603_API_CfgTunerIFOutParam(devId, ifOutCfg);
		if (MXL_SUCCESS != status)
		{
			LOG_BLINE("Error! MxLWare603_API_CfgTunerIFOutParam\n"); 
			iOutRet = ERR_TUNER_OPEN_FAIL;
			break;
		}
		// 5 : AGC Setting
		agcCfg.agcType = MXL603_AGC_EXTERNAL; //if you doubt DMD IF-AGC part, pls. use Tuner self AGC instead.
		agcCfg.setPoint = 66;
		agcCfg.agcPolarityInverstion = MXL_DISABLE;
		status = MxLWare603_API_CfgTunerAGC(devId, agcCfg);
		if (MXL_SUCCESS != status)
		{
			LOG_BLINE("Error! MxLWare603_API_CfgTunerAGC\n");
			iOutRet = ERR_TUNER_OPEN_FAIL;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::SetSystemModeTuner(UINT8 tunerId,PMXL603_TUNER_MODE_CFG_T pCfgTunerMode)
{
	MXL_STATUS status = MXL_SUCCESS;
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pCfgTunerMode
			|| 0 == tunerId)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		
		if(MXL603_DIG_ISDBT_ATSC != pCfgTunerMode->signalMode
			&& MXL603_DIG_J83B != pCfgTunerMode->signalMode)
		{
			pCfgTunerMode->signalMode = MXL603_DIG_ISDBT_ATSC;
		}
		
		status = MxLWare603_API_CfgTunerMode(tunerId, pCfgTunerMode);
		if (MXL_SUCCESS != status)
		{
			LOG_BLINE("Error! MxLWare603_API_CfgTunerMode\n");
			iOutRet = ERR_TUNER_OPEN_FAIL;
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::SetScanParam()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		ZeroMemory(&m_ScanFreqParam,sizeof(m_ScanFreqParam));
		
		if(MXL603_DIG_ISDBT_ATSC == m_CfgTunerMode.signalMode)
		{
			m_ScanFreqParam.pScanFrequencyPlan = ScanFrequencyPlan_AIR;
			m_ScanFreqParam.zoneNum = sizeof(ScanFrequencyPlan_AIR)/sizeof(ScanFrequencyPlan_t);
			m_ScanFreqParam.freqIdx = 0;
			m_ScanFreqParam.zoneIdx = 0;
			m_ScanFreqParam.totalFreqNum = TOTAL_FREQ_NUM_USA_AIR;
		}
		else if(MXL603_DIG_J83B == m_CfgTunerMode.signalMode)
		{
			m_ScanFreqParam.pScanFrequencyPlan = ScanFrequencyPlan_CABLE;
			m_ScanFreqParam.zoneNum = sizeof(ScanFrequencyPlan_CABLE)/sizeof(ScanFrequencyPlan_t);
			m_ScanFreqParam.freqIdx = 0;
			m_ScanFreqParam.zoneIdx = 0;
			m_ScanFreqParam.totalFreqNum = TOTAL_FREQ_NUM_USA_CABLE;
		}
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}


INT_t CMxl603TunerIface::GetNextFreq(INT32_t *pFreq)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT8_t Idx = 0;
	UINT8_t zIdx = 0;
	INT32_t freq = 0;

	do{
		if(NULL == pFreq)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		
		Idx = m_ScanFreqParam.freqIdx;
		zIdx = m_ScanFreqParam.zoneIdx;

		if(zIdx > m_ScanFreqParam.zoneNum
			|| Idx > m_ScanFreqParam.totalFreqNum)
		{
			iOutRet = ERR_TU_FREQ_OUT_OF_ZONE_RANG;
			break;
		}
		else if(zIdx == m_ScanFreqParam.zoneNum || Idx == m_ScanFreqParam.totalFreqNum)
		{
			iOutRet = ERR_TU_SCAN_DONE;
			break;
		}
		
		freq = m_ScanFreqParam.pScanFrequencyPlan[zIdx].startFreqKhz
			+Idx*m_ScanFreqParam.pScanFrequencyPlan[zIdx].stepKhz;
		
		
		if(freq > m_ScanFreqParam.pScanFrequencyPlan[zIdx].endFreqKhz)
		{
			iOutRet = ERR_TU_FREQ_OUT_OF_ZONE_RANG;
			break;
		}
		else if(freq == m_ScanFreqParam.pScanFrequencyPlan[zIdx].endFreqKhz)
		{
			zIdx ++;
			if(zIdx < m_ScanFreqParam.zoneNum)
			{
				Idx = 0;
			}
			else if(zIdx >= m_ScanFreqParam.zoneNum)
			{
				Idx = 0;
				iOutRet = ERR_TU_SCAN_DONE;
				//break;
			}
		}
		else
		{
			Idx ++;
		}

		*pFreq = freq;
		m_ScanFreqParam.zoneIdx  = zIdx;
		m_ScanFreqParam.freqIdx  = Idx;
		m_ScanFreqParam.freqProgress ++;
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::GetCurrentScanProgress(UINT16_t *pProgress)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT16_t Progress = 0;

	do{
		if(NULL == pProgress)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		Progress = (UINT16_t)(m_ScanFreqParam.freqProgress*100)/m_ScanFreqParam.totalFreqNum;

		if(0 == Progress%3)
		{
			iOutRet = ERR_TU_DEVICE_BUSY;
			break;
		}
	}while(FALSE);

	*pProgress = Progress;

	return iOutRet;
}

INT_t CMxl603TunerIface::SetNextFreqChannel(UINT32_t *pLock,UINT16_t *pScanProgress)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	INT32_t freq = 0;
	UINT16_t iProgress = 0;

	do{
		if(NULL == pLock || NULL == pScanProgress)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = GetNextFreq(&freq);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(ERROR_SUCCESS == iRet)
		{
			iRet = GetCurrentScanProgress(&iProgress);
			if(ERR_TU_DEVICE_BUSY == iRet)//update progress
			{
				*pScanProgress = iProgress*10;
				iOutRet = ERR_TU_DEVICE_BUSY;
			}
		}

		iRet = SetChannelInfo(freq*1000);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//Check Lock
		iRet = WaitForLock(pLock);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::GetChannleInfo(INT32_t *pFreq,UINT8 *pBw)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do{
		if(NULL == pFreq || NULL == pBw)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		*pFreq = m_TunerChanInfo.freqInHz;
		*pBw = m_TunerChanInfo.bandWidth;
		
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::GetChannelStrength(UINT16_t *pOutStrength)
{
	INT_t iOutRet = ERROR_SUCCESS;
	DMD_ERROR_t ret = DMD_E_OK;
	UINT16_t outStrength = 0;
	
	do{
		if(NULL == pOutStrength)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		ret = DMD_get_SSI(&m_DemodPara,&outStrength);
		if(DMD_E_OK != ret)
		{
			iOutRet = ERR_TU_GET_STRENGTH_FAIL;
			break;
		}

		*pOutStrength = outStrength;

		
	}while(FALSE);
	
	return iOutRet;
}

INT_t CMxl603TunerIface::ReInitModules()
{
	return ERROR_SUCCESS;
}

INT_t CMxl603TunerIface::InitVal()
{
	INT_t iOutRet = ERROR_SUCCESS;
	DMD_ERROR_t iRet = DMD_E_OK;

	m_Constellation = 0;
	
	//demod
	ZeroMemory(&m_DemodPara,sizeof(m_DemodPara));
	m_DemodPara.devid = 0;
	m_DemodPara.system = DMD_E_ATSC;//default
	m_DemodPara.ts_out = DMD_E_TSOUT_PARALLEL_FIXED_CLOCK;//default
	
	//tuner
	ZeroMemory(&m_CfgTunerMode,sizeof(m_CfgTunerMode));
	m_CfgTunerMode.signalMode = MXL603_DIG_ISDBT_ATSC;
	m_CfgTunerMode.ifOutFreqinKHz = 5000; //IF freq set, should match Demod request 
	m_CfgTunerMode.xtalFreqSel = MXL603_XTAL_24MHz;
	m_CfgTunerMode.ifOutGainLevel = 11;	//DFE_DACIF_GAIN == 0

	//tuner chaninfo
	ZeroMemory(&m_TunerChanInfo,sizeof(m_TunerChanInfo));
	m_TunerChanInfo.bandWidth = MXL603_TERR_BW_6MHz;
	m_TunerChanInfo.freqInHz = 0;
	m_TunerChanInfo.signalMode = m_CfgTunerMode.signalMode;
	m_TunerChanInfo.startTune = MXL_START;
	m_TunerChanInfo.xtalFreqSel = MXL603_XTAL_24MHz;

	ZeroMemory(&m_ScanFreqParam,sizeof(m_ScanFreqParam));
	
	m_bInited = TRUE;

EXIT_PROC:

	return iOutRet;
}

INT_t CMxl603TunerIface::SetStandard(IN StandardChannle iStandard,IN INT32_t iConstellation)
{
	INT_t iOutRet = ERROR_SUCCESS;
	DMD_ERROR_t iRet = DMD_E_OK;
	
	do{
		if(STANDARD_8VSB == iStandard && 0 == iConstellation)
		{
			m_DemodPara.system = DMD_E_ATSC;
			m_CfgTunerMode.signalMode = MXL603_DIG_ISDBT_ATSC;
			m_Constellation = DMD_E_NOT_DEFINED;
		}
		else if(STANDARD_ITU_B == iStandard && 1 == iConstellation)
		{
			m_DemodPara.system = DMD_E_QAMB_64QAM;
			m_CfgTunerMode.signalMode = MXL603_DIG_J83B;
			m_Constellation = DMD_E_QAMB_64QAM;
		}
		else if(STANDARD_ITU_B == iStandard && 2 == iConstellation)
		{
			m_DemodPara.system = DMD_E_QAMB_256QAM;
			m_CfgTunerMode.signalMode = MXL603_DIG_J83B;
			m_Constellation = DMD_E_QAMB_256QAM;
		}
		else
		{
			iOutRet = ERR_TU_SET_STANDARD_FAIL;
			break;
		}
		
		//Set Standard for tuner and demod
		iRet = DMD_set_system(&m_DemodPara,&m_CfgTunerMode);
		if(DMD_E_OK != iRet)
		{
			LOG_BLINE("DMD_SET_SYSTEM ERROR\n");
			iOutRet = ERR_TU_SET_STANDARD_FAIL;
			break;
		}

		iRet = DMD_set_ts_output(&m_DemodPara, m_DemodPara.ts_out);
		if(DMD_E_OK != iRet )
		{
			LOG_BLINE("ERROR:set ts output");
			iOutRet = ERR_TU_SET_STANDARD_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::SetChannelInfo(INT32_t Freq)
{
	INT_t iOutRet = ERROR_SUCCESS;
	DMD_ERROR_t iRet = DMD_E_OK;

	do{
		m_TunerChanInfo.signalMode = m_CfgTunerMode.signalMode;
		m_TunerChanInfo.freqInHz  = Freq;
		m_TunerChanInfo.startTune = MXL_START_TUNE;
		if(MXL603_DIG_J83B == m_TunerChanInfo.signalMode)
		{
			m_TunerChanInfo.bandWidth = MXL603_CABLE_BW_6MHz;
		}
		else
		{
			m_TunerChanInfo.bandWidth = MXL603_TERR_BW_6MHz;
		}
		m_TunerChanInfo.xtalFreqSel = MXL603_XTAL_24MHz;
		iRet = DMD_scan(&m_DemodPara,&m_TunerChanInfo);
		if(DMD_E_OK != iRet)
		{
			iOutRet = ERR_TU_SCAN_ERROR;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::GetStandard(OUT StandardChannle *pOStandard)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pOStandard)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(MXL603_DIG_ISDBT_ATSC == m_CfgTunerMode.signalMode)
		{
			*pOStandard = STANDARD_8VSB;
		}
		else if(MXL603_DIG_J83B == m_CfgTunerMode.signalMode)
		{
			*pOStandard =  STANDARD_ITU_B;
		}
		else
		{
			*pOStandard = STANDARD_UNKNOW;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CMxl603TunerIface::GetCurrConstellation(OUT UINT32_t *pConstellation)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pConstellation)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(DMD_E_QAMB_64QAM == m_Constellation)
		{
			*pConstellation = 1;
		}
		else if(DMD_E_QAMB_256QAM == m_Constellation)
		{
			*pConstellation = 2;
		}
		else
		{
			*pConstellation = 0;
		}

	}while(FALSE);

	return iOutRet;
}


INT_t CMxl603TunerIface::WaitForLock(UINT32_t *pLock)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == pLock)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if (DMD_E_LOCKED == m_DemodPara.info[DMD_E_INFO_LOCK])
		{
			*pLock = TRUE;
		}
		else
		{
			*pLock = FALSE;
		}
	}while(FALSE);

	return iOutRet;
}


void CMxl603TunerIface::ResetDemodParaInfo()
{
	memset(m_DemodPara.info,0x00,sizeof(m_DemodPara.info));
}

INT_t CMxl603TunerIface::UnInit()
{
	DMD_ERROR_t iRet = DMD_E_OK;
	
	do{
		iRet = DMD_close(&m_DemodPara);
		if(DMD_E_OK != iRet)
		{
			LOG_BLINE("DMD_close %d\n",iRet);
		}
		
	}while(FALSE);
	
	return ERROR_SUCCESS;
}

CMxl603TunerIface::CMxl603TunerIface()
{	
	m_Constellation = 0;
	m_bInited = FALSE;	
}

CMxl603TunerIface::~CMxl603TunerIface()
{
	m_Constellation = 0;
	m_bInited = FALSE;
	UnInit();
}

INT_t CMxl603TunerIface::ResetTunerHardware()
{
	INT_t iOutRet = ERROR_SUCCESS;
	NEXUS_Error nexusErr;
	NEXUS_GpioHandle pGpioHandle = NULL;
	NEXUS_GpioHandle pTunerPower=NULL;
	NEXUS_GpioSettings gpioSettings;

	NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eStandard, &gpioSettings);
	gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
	gpioSettings.value = NEXUS_GpioValue_eHigh;
	gpioSettings.interruptMode = NEXUS_GpioInterrupt_eDisabled;
	gpioSettings.interrupt.callback = NULL;
	gpioSettings.interrupt.context = NULL;
	gpioSettings.interrupt.param = 0;
	gpioSettings.maskEdgeInterrupts = FALSE;

	pTunerPower = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, I2cTuner_Power, &gpioSettings);
	if(NULL == pTunerPower)
	{
		iOutRet = DMD_E_PARA_ERROR;
		goto EXIT_PROC;
	}

	usleep(1);
	
	pGpioHandle = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_RESET, &gpioSettings);
	if( NULL == pGpioHandle )
	{
		iOutRet = ERR_OpenNexusGpio_FAIL;
		goto EXIT_PROC;
	}

	//high
	gpioSettings.value = NEXUS_GpioValue_eHigh;
	nexusErr = NEXUS_Gpio_SetSettings(pGpioHandle, &gpioSettings);
	if(NEXUS_SUCCESS != nexusErr)
	{
		iOutRet = ERR_SETUP_FAIL;
		goto EXIT_PROC;
	}
	usleep(8*1000);

	//low
	gpioSettings.value = NEXUS_GpioValue_eLow;
	nexusErr = NEXUS_Gpio_SetSettings(pGpioHandle, &gpioSettings);
	if(NEXUS_SUCCESS != nexusErr)
	{
		iOutRet = ERR_SETUP_FAIL;
		goto EXIT_PROC;
	}
	usleep(8*1000);

	//high
	gpioSettings.value = NEXUS_GpioValue_eHigh;
	nexusErr = NEXUS_Gpio_SetSettings(pGpioHandle, &gpioSettings);
	if(NEXUS_SUCCESS != nexusErr)
	{
		iOutRet = ERR_SETUP_FAIL;
		goto EXIT_PROC;
	}
	//wait for stable
	usleep(0);

EXIT_PROC:

	if(pGpioHandle)
	{
		NEXUS_Gpio_Close(pGpioHandle);
		pGpioHandle = NULL;
	}

	if(pTunerPower)
	{
		NEXUS_Gpio_Close(pTunerPower);
		pTunerPower = NULL;
	}

	return iOutRet;
}

