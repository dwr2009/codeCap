
/* **************************************************** */
/*!
   @file	MN_DMD_Tuner_template.c
   @brief	Tuner API wrapper (sample)
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */
//---------- this is sample Tuner API  ----------------
#include "../include/MN_DMD_driver.h"
#include "../include/MN_DMD_common.h"
#if (TUNER_TYPE == MXL603)   
#include "../include/MxL603_TunerApi.h"
#include "../include/MaxLinearDataTypes.h"
#endif
/* **************************************************** */
/* **************************************************** */

/*! Tuner Initialize Function*/
DMD_ERROR_t DMD_API DMD_Tuner_init()
{
	//TODO:	Please call tuner initialize API here

	DMD_ERROR_t ret;
	#if (TUNER_TYPE == MXL603)   
	ret = MXL603_INIT();	
	 #endif
     if (ret != DMD_E_OK)
    {
      printf("Error! DMD_Tuner_init\n");    
     }
 
	//this function is called by DMD_init
	
	//DMD_DBG_TRACE("Tuner is not implemeted\n");
	return DMD_E_OK;
}

/*! Tuner Tune Function */
DMD_ERROR_t DMD_API DMD_Tuner_tune(PMXL603_CHAN_TUNE_CFG_T pChannelInfoCfg)
{
	DMD_ERROR_t ret = DMD_E_OK;

	do{
		ret = Mxl603SetFreqBw(pChannelInfoCfg);
		if (DMD_E_OK != ret)
		{
			printf("Error! DMD_Tuner_init()\n");   
			break;
		}
	}while(FALSE);
	 
	return ret;
}

/*! Tuner Termination Function */
DMD_ERROR_t DMD_API DMD_Tuner_term(void)
{
	//TODO:	Please call tune  API here
	//this function is called by DMD_term
	printf("Tuner is not implemeted\n");
	return DMD_E_OK;
}

/*! Tuner Tune Function */
DMD_ERROR_t DMD_API DMD_Tuner_set_system(PMXL603_TUNER_MODE_CFG_T pCfgTuParam)
{
	DMD_ERROR_t ret = DMD_E_OK;

	do{
		if(NULL == pCfgTuParam)
		{
			ret = DMD_E_PARA_ERROR;
			break;
		}
		
		ret = Mxl603SetSystemMode(pCfgTuParam);
		if (DMD_E_OK != ret)
		{
			printf("SetTunerMxl603SystemMode Failed\n");
			break;
		}	
	}while(FALSE);
	
	return ret;
}
