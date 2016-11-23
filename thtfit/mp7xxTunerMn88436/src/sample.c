/* **************************************************** */
/*!
   @file	MN88436_sample.c
   @brief	Sample Application for MN88436 multiple DMD(Panasonic ATSB/QAM-B Demodulator)
   @author	Simon
   @date	2012/4/9
   @param
		(c)	Panasonic
   */
/* **************************************************** */

//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/MN_DMD_driver.h"
#include "../include/MN_DMD_device.h"
#include "../include/MN_DMD_console.h"
#include "../include/MN_I2C.h"
#include "../include/MN_DMD_common.h"
#include "../include/MN88436_reg.h"
#if (TUNER_TYPE == MXL603)    
#include "../include/MxL603_TunerApi.h"
extern MXL603_TUNER_MODE_CFG_T tunerModeCfg;
extern MXL603_CHAN_TUNE_CFG_T chanTuneCfg;
#endif

#ifdef PC_CONTROL_FE
int select_menu( DMD_PARAMETER_t *param );
void select_id_system( DMD_PARAMETER_t *param);
#endif


/* **************************************************** */
//	MN88436 Control application
/* **************************************************** */
int main(void) //this is for DMD init 
{
	DMD_PARAMETER_t param;

	//Open API
	printf("Loading SIGMA I2C Driver(API Open) ...");
	if( DMD_open(&param) == DMD_E_OK )
		printf("OK\n");
	else
		printf("NG\n");

	//Select device ID
	param.devid = 0;

	//Initialize LSI
	printf("Initializing LSI .. ");
	if( DMD_init(&param) == DMD_E_OK )
		printf("OK\n");
	else
		printf("NG\n");
	
#ifdef PC_CONTROL_FE
	select_menu( &param ) ;
#endif

	//Close API
	DMD_close(&param);
	
	return 0;
}

/* **************************************************** */
/*!	User interface  */
/*
Description :
purpose : 1. To find info. in one RF channel; 2. to jump to one service based on stored info. 
input  : //you can also add IF_out_freq, or AGC control, if you want. Normally, not necessary
  freq_khz :  user-set RF freq. 
  set_dmd_sytem_mode : DTV system, DVB-T or DVB-T2
  //bandwidth : default 6Mhz 
return : ret -> 1 : signal unlocked, or parameter set error; 0. signal locked
Author :  Troy.wangyx, 20120801 
*/
/* **************************************************** */

DMD_ERROR_t Scan_one_RF_sample(DMD_u32_t freq_khz,DMD_SYSTEM_t set_dmd_sytem_mode)
{	
    DMD_ERROR_t ret = DMD_E_ERROR;
	
     DMD_PARAMETER_t param; //Demodulator Parameter    
    // bw -- default bandwidth is 6Mhz 
    
     //Tuner and DMD call relative register array, toward different System mode and Bandwidth.
    param.system = set_dmd_sytem_mode; //default DMD_E_ATSC
    
      #if (TUNER_TYPE == MXL603)
		    switch(param.system)
		    {
		      	case DMD_E_ATSC:
				case DMD_E_QAMB_64QAM:
				case DMD_E_QAMB_256QAM:     
						tunerModeCfg.signalMode = MXL603_DIG_ISDBT_ATSC;
				     break;  	
				default:  
					      printf("--- UNSUPPORT SYSTEM!!---\n");
						  return DMD_E_ERROR;
		    }
		     chanTuneCfg.signalMode = tunerModeCfg.signalMode;
			 	
			  chanTuneCfg.freqInHz  = freq_khz*1000;
		#endif
						
	     //DMD_set_system(&param);
	
        //Tuner tuning -> DMD scan
      // DMD_scan(&param);	  
	   
	   if (param.info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)
	   	{  
	   	     /*  [If do channel search]
	   	     Afte signal locked
	   	     1. store channel info. afte signal locked  : RF freq <-> program stream 
	   	     2. Get channel modulation info, GI, CR, FFT etc.  Func. DMD_get_info( DMD_PARAMETER_t* param , DMD_u32_t id )
             3. if Auto search, jump out and set next freq. 
	   	     */
           ret = DMD_E_OK;			 
              printf("--- VALID SIGNAL found !! ---\n");

			;//you may add interface to get programe stream info.(eg. EPG, pid)
		     return ret;
	   	}
	   else 
	   	{
	   	       printf("--- NO VALID SIGNAL FOUND !!---\n");
            ret = DMD_E_ERROR;
	   	}
   	
	return ret;
}


// ************************************
/*
The functions below are using PC communication protocal, 
Normally, the code below is just for refercence of system flow: Set system-> tuning -> scan ->  lock signal 

After you get the source code, if you want to compile and connect PC with FE ,
you can open the define as below. 
After compile, it will generate CMD box, try to communicate with FE. Don't forget to run with folder "i2c_fx2i2c_for_PC"
*/
#ifdef PC_CONTROL_FE 
//	Main Menu
// ************************************
int select_menu( DMD_PARAMETER_t *param )
{
	char	buf[256], rd=0;
	DMD_u32_t	id=0, ts=0;

	do {
		printf("\n*** MN88436 Sample Menu ***\n" );
		printf(" 1.   iic direct read/write\n");
		printf(" 2.   select Device ID\n" );
		printf(" 3.   select broadcast system( ATSC/QAM-B )\n" );
		printf(" 4.   select TS output\n" );
		printf(" 5.   tune\n" );
		printf(" s.   show status\n" );
		printf(" m.   signal monitor\n" );
		printf(" c.   channel search\n" );
#ifdef DMD_I2C_DEBUG
		printf(" z.   i2c debug display\n" );
#endif
		printf(" Q.   Quit\n" );
		printf("\n" );
		printf("Input Number >> " );
		fgets( buf, 256, stdin);
		switch ( buf[0] ) {
			case '1':
				DMD_con_i2c_test(param);
		  		break;
			case '2':
				printf("Select Device ID >");
				fgets( buf , 256 , stdin );
				param->devid = atoi(buf);

				//DMD_init
				printf("DMD_init .. ");
				if( DMD_init(param) == DMD_E_OK )
					printf("OK\n");
				else
					printf("NG\n");
				break;
			case '3':
				DMD_con_select_system( param );
				DMD_set_system( param );
				break;
			case '4':
				printf("0_Serial  1_Parallel > " );
				fgets( buf , 256 , stdin );
				ts = atoi(buf);
				if( ts == 0 )
					{
					DMD_I2C_Write( DMD_BANK_MAIN(param->devid) , DMD_MAIN_CPOSET2 , 0xc0 );
					DMD_I2C_Write( DMD_BANK_MAIN(param->devid) , DMD_MAIN_GPSET1 , 0x00 );
					}
				else
					{
					DMD_I2C_Write( DMD_BANK_MAIN(param->devid) , DMD_MAIN_CPOSET2 , 0xc1 );
                                   DMD_I2C_Write( DMD_BANK_MAIN(param->devid) , DMD_MAIN_GPSET1 , 0xff );//troy.wang, 121010, for parallel data setting
					}
				break;
			case '5':
				DMD_con_input_freq(param);
				DMD_tune(param);
				break;
			case 's':
				DMD_get_info(param,DMD_E_INFO_ALL);
				DMD_con_show_status(param);
		  		break;
			case 'm':
				printf( "Press any key to exit\n");
				while( !_kbhit() )
				{
					DMD_get_info(param,DMD_E_INFO_ALL);
					DMD_con_show_monitor(param);
					DMD_wait(500);
				}
				break;
			case 'c':
				DMD_con_channel_search(param);
				break;
#ifdef DMD_I2C_DEBUG
			case 'z':
				dmd_i2c_debug_flag ^= 1;
				break;
#endif

		}
	}while ( buf[0] != 'q' && buf[0] != 'Q' );

	return 0;
}
#endif //#ifdef PC_CONTROL_FE

