#ifndef	_DEV_CONNECTION_STATUS_H_
#define	_DEV_CONNECTION_STATUS_H_

#include <BaseTypeDef.h>

#define	SysProp_KEY_DevConn_UsbMounted		"DevConnectionStatus.UsbMounted"
#define	SysProp_KEY_DevConn_SdMounted		"DevConnectionStatus.SdMounted"
#define	SysProp_KEY_DevConn_HddMounted		"DevConnectionStatus.HddMounted"

INT_t getDevMountStatus_Usb(BOOL_t * CONST pbMounted);
INT_t getDevMountStatus_Sd(BOOL_t * CONST pbMounted);
INT_t getDevMountStatus_Hdd(BOOL_t * CONST pbMounted);
INT_t UpdateDeviceMountStatusFromSysProp();

typedef enum
{
	DISP_OSD_RES_DEFAULT=0,
	DISP_OSD_RES_1366x768 = 1,
	DISP_OSD_RES_1920x1080 = 2,
	DISP_OSD_RES_3840x2160 = 3,
	DISP_OSD_RES_MAX
}DISP_OSD_RESOLUTION_ID,*P_DISP_OSD_RESOLUTION_ID;

#endif	//_DEV_CONNECTION_STATUS_H_

