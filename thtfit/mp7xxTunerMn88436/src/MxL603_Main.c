#include <stdio.h>
#include "../include/MaxLinearDataTypes.h"
#include "../include/MxL603_TunerApi.h"
#include "../include/MxL603_OEM_Drv.h"

#define EXAMPLE_DEV_MAX 2
MXL603_TUNER_MODE_CFG_T tunerModeCfg; //global parameter 
MXL603_CHAN_TUNE_CFG_T chanTuneCfg;//set to be global value

/* Example of OEM Data, customers should have
below data structure declared at their appropriate 
places as per their software design 

typedef struct
{
  UINT8   i2c_address;
  UINT8   i2c_bus;
  sem_type_t sem;
  UINT16  i2c_cnt;
} user_data_t;  

user_data_t device_context[EXAMPLE_DEV_MAX];

*/

MXL_STATUS MXL603_INIT(void)
{
	MXL_STATUS status; 
	UINT8 devId;
	MXL_BOOL singleSupply_3_3V;
	MXL603_XTAL_SET_CFG_T xtalCfg;
	MXL603_IF_OUT_CFG_T ifOutCfg;
	MXL603_AGC_CFG_T agcCfg;
	//MXL603_TUNER_MODE_CFG_T tunerModeCfg; //set to be global value
	//MXL603_CHAN_TUNE_CFG_T chanTuneCfg;//set to be global value
	MXL_BOOL refLockPtr = 0;
	MXL_BOOL rfLockPtr = 0;
	/* If OEM data is implemented, customer needs to use OEM data structure  
	related operation. Following code should be used as a reference. 
	For more information refer to sections 2.5 & 2.6 of 
	MxL603_mxLWare_API_UserGuide document.

	for (devId = 0; devId < EXAMPLE_DEV_MAX; devId++)
	{
	// assigning i2c address for  the device
	device_context[devId].i2c_address = GET_FROM_FILE_I2C_ADDRESS(devId);     

	// assigning i2c bus for  the device
	device_context[devId].i2c_bus = GET_FROM_FILE_I2C_BUS(devId);                      

	// create semaphore if necessary
	device_context[devId].sem = CREATE_SEM();                                                           

	// sample stat counter
	device_context[devId].i2c_cnt = 0;                                                                               

	status = MxLWare603_API_CfgDrvInit(devId, (void *) &device_context[devId]);  

	// if you don’t want to pass any oem data, just use NULL as a parameter:
	// status = MxLWare603_API_CfgDrvInit(devId, NULL);  
	}

	*/

	/* If OEM data is not required, customer should treat devId as 
	I2C slave Address */
	devId = MXL603_I2C_ADDR;

	//Step 1 : Soft Reset MxL603
	status = MxLWare603_API_CfgDevSoftReset(devId);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgDevSoftReset\n");    
	}

	//Step 2 : Overwrite Default
	singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE;
	status = MxLWare603_API_CfgDevOverwriteDefaults(devId, singleSupply_3_3V);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgDevOverwriteDefaults\n");    
	}

	//Step 3 : XTAL Setting 
#if 0
	/* Dual XTAL for tuner and demod separately*/
	xtalCfg.xtalFreqSel = MXL603_XTAL_16MHz;
	xtalCfg.xtalCap = 12;
	xtalCfg.clkOutEnable = MXL_DISABLE;
#else
	/* Single XTAL for tuner and demod sharing*/
	xtalCfg.xtalFreqSel = MXL603_XTAL_24MHz;
	xtalCfg.xtalCap = 25;
	xtalCfg.clkOutEnable = MXL_ENABLE;
#endif 
	xtalCfg.clkOutDiv = MXL_DISABLE;
	xtalCfg.clkOutExt = MXL_DISABLE;

	/*Parameter : singleSupply_3_3V
	- Enable :  Single power supply to Tuner (3.3v only;  3.3v -> 1.8V tuner internally inverts ) 
	- Disable : Dual power supply to Tuner (3.3v+1.8v; internal regulator be bypassed) 
	¡ï If set wrongly toward hardware layout, Tuner will loose control of AGC(at least) 
	*/
	xtalCfg.singleSupply_3_3V = MXL_ENABLE;//MXL_DISABLE

	xtalCfg.XtalSharingMode = MXL_DISABLE;
	status = MxLWare603_API_CfgDevXtal(devId, xtalCfg);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgDevXtal\n");    
	}

	//Step 4 : IF Out setting
	//IF freq set, should match Demod request 
	ifOutCfg.ifOutFreq = MXL603_IF_5MHz;

	ifOutCfg.ifInversion = MXL_DISABLE;
	ifOutCfg.gainLevel = 11;
	ifOutCfg.manualFreqSet = MXL_DISABLE;
	ifOutCfg.manualIFOutFreqInKHz = 0;
	status = MxLWare603_API_CfgTunerIFOutParam(devId, ifOutCfg);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgTunerIFOutParam\n");    
	}

	//Step 5 : AGC Setting
	agcCfg.agcType = MXL603_AGC_EXTERNAL;
	//agcCfg.agcType = MXL603_AGC_SELF; //if you doubt DMD IF-AGC part, pls. use Tuner self AGC instead.

	agcCfg.setPoint = 66;
	agcCfg.agcPolarityInverstion = MXL_DISABLE;
	status = MxLWare603_API_CfgTunerAGC(devId, agcCfg);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgTunerAGC\n");    
	}

	//Step 6 : Application Mode setting
	tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;//MXL603_DIG_DVB_T_DTMB;

	//IF freq set, should match Demod request 
	tunerModeCfg.ifOutFreqinKHz = 5000;

#if 0
	/* Dual XTAL for tuner and demod separately*/
	tunerModeCfg.xtalFreqSel = MXL603_XTAL_16MHz;
#else
	/* Single XTAL for tuner and demod sharing*/
	tunerModeCfg.xtalFreqSel = MXL603_XTAL_24MHz;
#endif
	tunerModeCfg.ifOutGainLevel = 11;
	status = MxLWare603_API_CfgTunerMode(devId, &tunerModeCfg);
	if (status != MXL_SUCCESS)
	{
		printf("##### Error! pls. make sure return value no problem, otherwise, it will cause Tuner unable to unlock signal #####\n");   
		printf("Error! MxLWare603_API_CfgTunerMode\n");    
	}

	//Step 7 : Channel frequency & bandwidth setting
	chanTuneCfg.bandWidth =  MXL603_TERR_BW_6MHz;//MXL603_TERR_BW_8MHz;
	chanTuneCfg.freqInHz = 666000000;
	chanTuneCfg.signalMode = MXL603_DIG_ISDBT_ATSC;//MXL603_DIG_DVB_T_DTMB;
#if 0
	/* Dual XTAL for tuner and demod separately*/
	chanTuneCfg.xtalFreqSel = MXL603_XTAL_16MHz;
#else
	/* Single XTAL for tuner and demod sharing*/
	chanTuneCfg.xtalFreqSel = MXL603_XTAL_24MHz;
#endif 

	chanTuneCfg.startTune = MXL_START_TUNE;
	status = MxLWare603_API_CfgTunerChanTune(devId, &chanTuneCfg);
	if (status != MXL_SUCCESS)
	{
		printf("Error! MxLWare603_API_CfgTunerChanTune\n");    
	}

	// Wait 15 ms 
	MxLWare603_OEM_Sleep(15);

	// Read back Tuner lock status
	status = MxLWare603_API_ReqTunerLockStatus(devId, &rfLockPtr, &refLockPtr);
	if (status == MXL_TRUE)
	{
		if (MXL_LOCKED == rfLockPtr && MXL_LOCKED == refLockPtr)
		{
			printf("Tuner locked\n");
		}
		else
			printf("Tuner unlocked\n");
	}

	// To Change Channel, GOTO Step #7

	// To change Application mode settings, GOTO Step #6

	return status;
}


MXL_STATUS Mxl603SetSystemMode(PMXL603_TUNER_MODE_CFG_T pTunerModeCfg)
{
	MXL_STATUS status = MXL_SUCCESS; 
	UINT8 devId = MXL603_I2C_ADDR;

	do{
		if(NULL == pTunerModeCfg)
		{
			status = MXL_FAILED;
			break;
		}
		
		status = MxLWare603_API_CfgTunerMode(devId, pTunerModeCfg);
		if (MXL_SUCCESS != status)
		{
			printf("Error! MxLWare603_API_CfgTunerMode\n");    
		}
		
	}while(FALSE);

	return status;

}  

MXL_STATUS Mxl603SetFreqBw(PMXL603_CHAN_TUNE_CFG_T pChannelInfoCfg)
{
	MXL_STATUS status = MXL_SUCCESS; 
	UINT8 devId = MXL603_I2C_ADDR;
	MXL_BOOL refLockPtr = MXL_UNLOCKED;
	MXL_BOOL rfLockPtr = MXL_UNLOCKED;

	do{
		if(NULL == pChannelInfoCfg)
		{
			status = MXL_INVALID_PARAMETER;
			break;
		}
		
		status = MxLWare603_API_CfgTunerChanTune(devId, pChannelInfoCfg);
		if (MXL_SUCCESS != status )
		{
			printf("Error! MxLWare603_API_CfgTunerChanTune\n");
			break;
		}

		// Wait 30 ms 
		MxLWare603_OEM_Sleep(30);

		// Read back Tuner lock status
		status = MxLWare603_API_ReqTunerLockStatus(devId, &rfLockPtr, &refLockPtr);
		if (MXL_TRUE == status)
		{
			;//printf("[rfLockPtr : %d],[refLockPtr : %d]",rfLockPtr,refLockPtr);
		}
	}while(FALSE);
	
	return status; 
}

