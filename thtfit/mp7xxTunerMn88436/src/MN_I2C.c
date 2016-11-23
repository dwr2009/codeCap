#ifndef	ALLOW_OS_CODE
#define	ALLOW_OS_CODE
#endif	//ALLOW_OS_CODE

#ifndef PC_CONTROL_FE
/* **************************************************** */
/*!
   @file	MN_DMD_I2C_WIN.c
   @brief	I2C communication wrapper
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */

//
#include <ErrPrintHelper.h>
#include <BaseTypeDef.h>
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>

#include "../include/MN_DMD_driver.h"
#include "../include/MN_I2C.h"
#include "../include/MN_DMD_common.h"
#include <nexus_i2c.h>
#include <nexus_gpio.h>



#define	I2C_MODULE_INDEX_SOFTWARE	0
#define I2C_MODULE_INDEX_HARDWARE 	1
#define I2cTuner_I2cChannelNo 	3
static NEXUS_I2cHandle 	m_hTunerI2c=NULL;

DMD_ERROR_t DMD_API DMD_I2C_open()
{
	DMD_ERROR_t iOutRet = DMD_E_OK;
	DMD_u8_t	data = 0;

	NEXUS_I2cSettings nexusI2cSettings;
	NEXUS_I2c_GetDefaultSettings(&nexusI2cSettings);
	nexusI2cSettings.clockRate = NEXUS_I2cClockRate_e100Khz;
	nexusI2cSettings.interruptMode = true;
	nexusI2cSettings.burstMode = false;
	nexusI2cSettings.timeout = 0;	//use defalt
	nexusI2cSettings.softI2c = false;
	nexusI2cSettings.fourByteXferMode = false;
	nexusI2cSettings.use5v = false;
	nexusI2cSettings.clockGpio = NULL;
	nexusI2cSettings.dataGpio = NULL;
	
	m_hTunerI2c = NEXUS_I2c_Open(I2cTuner_I2cChannelNo, &nexusI2cSettings);
	if(NULL == m_hTunerI2c)
	{
		iOutRet = DMD_E_PARA_ERROR;
		goto EXIT_PROC;
	}
	usleep(3000);

    return 0;

EXIT_PROC:

	if(m_hTunerI2c)
	{
		NEXUS_I2c_Close(m_hTunerI2c);
		m_hTunerI2c = NULL;
	}	
	return iOutRet;
}

/*! Write 1byte */
DMD_ERROR_t DMD_I2C_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;
	NEXUS_Error nexusErr;

	nexusErr = NEXUS_I2c_Write(m_hTunerI2c, slvadr, adr, &data, 1);
	if(NEXUS_SUCCESS != nexusErr)
	{
		eOutDMDStatus = DMD_E_ERROR;
		goto EXIT_PROC;
	}

EXIT_PROC:
	
	return eOutDMDStatus;
}

/*! Read 1byte */
DMD_ERROR_t DMD_I2C_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *data )
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;

	NEXUS_Error nexusErr;
	nexusErr = NEXUS_I2c_Read(m_hTunerI2c, slvadr, adr, data, 1);
	if(NEXUS_SUCCESS != nexusErr)
	{
		LOG_BLINE("i2c R err %d\n", nexusErr);
		eOutDMDStatus=DMD_E_ERROR;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return eOutDMDStatus;
}

DMD_ERROR_t DmdI2C_Read_NoSubAddr(DMD_u8_t devAddr8, DMD_u8_t * data)
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;

	NEXUS_Error nexusErr;
	nexusErr = NEXUS_I2c_Read(m_hTunerI2c, devAddr8,NULL,data, 1);
	if(NEXUS_SUCCESS != nexusErr)
	{
		LOG_BLINE("i2c R err %d\n", nexusErr);
		eOutDMDStatus=DMD_E_ERROR;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return eOutDMDStatus;

}

/* 
Write/Read any Length to DMD and Tuner;
DMD_u8_t slvadr : DMD's slave address.   
DMD_u8_t adr    : DMD's REG address; 

For writing CMD to tuner, 
DMD_u8_t slvadr is DMD's slave address;
DMD_u8_t adr is DMD's register TCBCOM(0xF7)
!!!Tuner's slave addr. and REG addr. are as continous data< upper layer : data[x]> 
*/
DMD_ERROR_t DMD_I2C_Write_Anylenth(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen)
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;
	NEXUS_Error nexusErr;

	nexusErr = NEXUS_I2c_Write(m_hTunerI2c, slvadr, adr, wdata, wlen);
	if(NEXUS_SUCCESS != nexusErr)
	{
		eOutDMDStatus = DMD_E_ERROR;
		goto EXIT_PROC;
	}

EXIT_PROC:	
	return eOutDMDStatus;

}

/*
twsbGetData_MN88472_TCB(slvadr, adr, wdata)
This is specific function for MN88472 to read Tuner reg's data.
бя If you use MN88472 to control Tuner, you may need to create new I2C bottom code.

There are two steps to read Tuner, take Mxl603 for example. (See Tuner I2C read protocal as below)
Step 1 : Tell Tuner which Reg to read 
      -> Pls. call function DMD_I2C_Write_Anylenth()
Step 2 : Read Tuner Reg's data
      -> Pls. call function DMD_TCBI2C_Read()
------ Tuner Mxl603 I2C read protocal -------
// 8 bit Register Read Protocol:
  Step1
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr   | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+
  Step2
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // SADDR(I2c slave address), S(Start condition), 
  // A(Slave Ack), MN(Master NACK),  P(Stop condition)

// [ Pls pay attention here ! This is what customer need complete ]
------ Step2 I2C commnication between BE and DMD -------
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |BE |S|SADDR|W| |R-addr| |SADDR-M|R| |S|SADDR|R| |      |MN|P|
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // |DMD|         |A|      |A|         |A|         |A| Data |  | |
  // +------+-+-----+-+-+----+-+----------+-+-++------+-+-----+-+-+----
  // SADDR(DMD's slave address), R-addr(DMD's reg addr. -> TCBCOM ),
  // SADDR-M(Tuner's slave address)
  You can also refer to file "PRODUCT_SPECIFICATIONS_MN88472_ver041_120120.pdf"
  page 34 to see the TCB control flow.
*/
DMD_ERROR_t DMD_TCBI2C_Read(DMD_u8_t slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	//printf("Entry DMD_TCBI2C_Read\n");
	return DMD_E_OK;
}

/*! Write/Read any Length*/
// This is general API for you to communicate with external device which DIRECTLY connect to BE,
// However, to communicate with Tuner through DMD MN88472, you may need to modify Bottom code of I2C sequence. 
DMD_ERROR_t DMD_I2C_WriteRead(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	return DMD_E_OK;
#if 0
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;
	RMstatus eRMstatus = RM_ERROR;

	do{
		if(sizeof(g_I2cWriteReadDataIn.I2cSendData.Data) < wlen)
		{
			eOutDMDStatus = DMD_E_OUT_OF_RANG;
			break;
		}
		
		if(sizeof(g_I2cWriteReadDataOut.I2cReadDataOut.Data) < rlen)
		{
			eOutDMDStatus = DMD_E_OUT_OF_RANG;
			break;
		}

		//input parameters
		g_I2cWriteReadDataIn.I2cSendData.DeviceParameter.NoStop = TRUE;
		g_I2cWriteReadDataIn.I2cSendData.DeviceParameter.DevAddr = slvadr;
		g_I2cWriteReadDataIn.I2cSendData.UseSubAddr = FALSE;
		g_I2cWriteReadDataIn.I2cSendData.DataSize = wlen;
		memcpy(g_I2cWriteReadDataIn.I2cSendData.Data, wdata, wlen);
		g_I2cWriteReadDataIn.I2cReadDataIn.DeviceParameter.NoStop = FALSE;
		g_I2cWriteReadDataIn.I2cReadDataIn.DeviceParameter.DevAddr = slvadr;
		g_I2cWriteReadDataIn.I2cReadDataIn.UseSubAddr = FALSE;
		g_I2cWriteReadDataIn.I2cReadDataIn.DataSize = rlen;
		//call
		eRMstatus = RUAExchangeProperty(m_pRuaInst, m_I2CModuleId, RMI2CPropertyID_WriteReadData, 
			&g_I2cWriteReadDataIn, sizeof(g_I2cWriteReadDataIn),
			&g_I2cWriteReadDataOut, sizeof(g_I2cWriteReadDataOut));
		if(RM_OK != eRMstatus)
		{
			eOutDMDStatus = DMD_E_ERROR;
			PRINT_BFILE_LINENO_RMSTATUS;
			break;
		}
		//output parameters
		memcpy(rdata, g_I2cWriteReadDataOut.I2cReadDataOut.Data, rlen);
		
	}while(FALSE);
	
	return eOutDMDStatus;
#endif
}

/*! Timer wait */
DMD_ERROR_t DMD_wait( DMD_u32_t msecond )
{
	int iRet;

	if(9 <= msecond)	//sleep
	{
		iRet = usleep(msecond*1000);
		if(-1 == iRet)
		{
			if(EINVAL == errno)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else if(EINTR == errno)
			{
				;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
	}
	else	//real delay
	{
		//Smp86xxGpioCtrl_delayUs(msecond*1000);
	}
	
	return DMD_E_OK;
}

/*! Get System Time (ms) */
DMD_ERROR_t DMD_timer( DMD_u32_t* tim )
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;
	UINT64 currTimeMs = 0;
	DMD_u32_t currTimeMsU32 = 0;

	do{
		if(NULL == tim)
		{
			eOutDMDStatus = DMD_E_PARA_ERROR;
			break;
		}

		currTimeMs = GetCurTimeMs();
		currTimeMsU32 = (DMD_u32_t)currTimeMs;
		*tim = currTimeMsU32;
	}while(FALSE);
	
	return eOutDMDStatus;
}

DMD_ERROR_t DMD_Close(void)
{
	DMD_ERROR_t eOutDMDStatus = DMD_E_OK;

	if(m_hTunerI2c)
	{
		NEXUS_I2c_Close(m_hTunerI2c);
	}
	
	return eOutDMDStatus;
}


#endif //#ifndef PC_CONTROL_FE
