/* **************************************************** */
/*!
   @file	MN_DMD_I2C_WIN.c
   @brief	USB I2C communication wrapper (for WINDOWS)
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#include <windows.h>
#include "MN_DMD_driver.h"
#include "MN_DMD_common.h"
#include "MN_I2C.h"
#include "fx2i2c.h"
#include <stdio.h>

#ifdef DMD_I2C_DEBUG
DMD_u32_t	dmd_i2c_debug_flag = 0;
#endif
/* **************************************************** */
/* **************************************************** */
#ifdef PC_CONTROL_FE
#define DMD_I2C_MAXSIZE	127
/*! I2C Initialize Function*/
DMD_ERROR_t DMD_API DMD_I2C_open(void *pI2cMode)
{
	//TODO:	Please call I2C initialize API here
	//this function is called by DMD_API_open

	usbi2c_open();

	return DMD_E_OK;
}
/*! Write 1byte */
DMD_ERROR_t DMD_I2C_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
	//TODO:	Please call I2C 1byte Write API
	s32	wdata;

	wdata = (long) data;
	usbi2c_send( slvadr, adr , 1,&wdata, 1, 0, 0, WAIT_SEND, OFF_400K, LAST_WAIT);
#ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag )
		printf("W:%02x %02x %02x\n",slvadr,adr,data);
#endif
	return DMD_E_OK;
}

/*! Read 1byte */
DMD_ERROR_t DMD_I2C_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *data )
{
	//TODO:	Please call I2C 1byte Write API
	s32	rdata;

	usbi2c_send( slvadr, adr, 1,0, 0, &rdata, 1, WAIT_SEND, OFF_400K, LAST_WAIT);

	*data = (DMD_u8_t) rdata;
#ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag )
		printf("R:%02x,%02x,%02x\n",slvadr,adr,*data);
#endif
	return DMD_E_OK;
}

/*! Write/Read any Length*/
DMD_ERROR_t DMD_I2C_WriteRead(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	//TODO:	Please call I2C Read/Write API here
	int wd[DMD_REGISTER_MAX];
	int rd[DMD_REGISTER_MAX];
	unsigned int	wl;
	unsigned int i;
	for(i=0;i<wlen;i++) wd[i] = (long) wdata[i];
	if( rlen >= DMD_I2C_MAXSIZE )		//do not support large size read
		return DMD_E_ERROR;
	if( wlen >= DMD_I2C_MAXSIZE )
	{
		//large size write
		wl = wlen;
		i=0;
		while( wl >= DMD_I2C_MAXSIZE ){
			usbi2c_send( slvadr, adr, 1,wd + i*DMD_I2C_MAXSIZE ,DMD_I2C_MAXSIZE, rd, 0, WAIT_SEND, OFF_400K, LAST_WAIT);
			wl -= DMD_I2C_MAXSIZE;
			i++;
		}
		usbi2c_send( slvadr, adr, 1,wd + i*DMD_I2C_MAXSIZE , (u8)wl, rd, (u8)rlen, WAIT_SEND, OFF_400K, LAST_WAIT);
	}
	else
		usbi2c_send( slvadr, adr, 1,wd  ,(u8)wlen, rd, (u8)rlen, WAIT_SEND, OFF_400K, LAST_WAIT);

#ifdef DMD_I2C_DEBUG
	if( dmd_i2c_debug_flag ){
		printf("W:%02x %02x:",slvadr,adr);
		for(i=0;i<wlen;i++) printf("%02x ",wd[i] );
		printf("\n");
		printf("R:%02x %02x:",slvadr,adr);
		for(i=0;i<rlen;i++) printf("%02x ",rd[i] );
		printf("\n");
	}
#endif
	for(i=0;i<rlen;i++) rdata[i] = (DMD_u8_t) rd[i];
	return DMD_E_OK;
}

/*! Timer wait */
DMD_ERROR_t DMD_wait( DMD_u32_t msecond ){

	Sleep( msecond );

	return DMD_E_OK;
}

/*! Get System Time (ms) */
DMD_ERROR_t DMD_timer( DMD_u32_t* tim )
{
	*tim = GetTickCount();
	return DMD_E_OK;
}

#endif //#ifdef PC_CONTROL_FE

