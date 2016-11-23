#ifndef	_SNMP_CMD_CONV_H_
#define	_SNMP_CMD_CONV_H_

#include <MediaPlayerAppDef.h>
#include <BaseTypeDef.h>
#include <ArrayOpsMacro.h>
#include <WebBrowserAppDef.h>
#if defined(__mips__)
#include <rmdef/rmdef.h>
#include <emhwlib/include/emhwlib_globaltypes.h>
#endif

#define	SysProp_KEY_MpVideoSyncMode				"MediaPlayer.VideoSync.Mode"
#define	SysProp_KEY_MpVideoSyncMaster			"MediaPlayer.VideoSync.Master"

#ifdef	__cplusplus
namespace Mp7xxCommon
{
#endif	//__cplusplus
	
typedef enum
{
	WIFI_STATUS_DISABLED = 0,
	WIFI_STATUS_ENABLED = 1,
    WIFI_STATUS_Connected = 2,
    WIFI_STATUS_NotConnected = 3,
	WIFI_STATUS_NotInstalled = 4
}WIFI_WORK_STATUS, *P_WIFI_WORK_STATUS;

typedef enum
{
	VidSyncMode_DISABLED = 0,
	VidSyncMode_MASTER = 1,
	VidSyncMode_SLAVE = 2
}VIDEO_SYNC_MODE_t, *P_VIDEO_SYNC_MODE_t;

typedef enum {
       SnmpOutSpdifMode_Disabled=0,
	SnmpOutSpdifMode_Compressed = 1,
	SnmpOutSpdifMode_Uncompressed = 2
}SNMP_OUTPUT_SPDIF_MODE,*P_SNMP_OUTPUT_SPDIF_MODE;

typedef enum {
       VIDEO_ROTATION_0=0,
	VIDEO_ROTATION_90 = 1,
	VIDEO_ROTATION_180 = 2,
	VIDEO_ROTATION_270=3
}VIDEO_ROTATION_t,*P_VIDEO_ROTATION_t;


#ifdef	__cplusplus
}	//Mp7xxCommon
#endif	//__cplusplus

int getSnmpResolutionValFromMpVoMode(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode);
MediaPlayer::VIDEO_OUTPUT_MODE getMpVoMode_From_SnmpDispResVal(int iSnmpDispResVal, OUT BOOL_t & bFound);
MediaPlayer::DISPLAY_ASPECT_RATIO getDispAspectRatio_From_SnmpVideoMode(ControlCenter::CE_SNMP_VIDEO_MODE_T eCeSnmpVidMode);
#if defined(__mips__)
enum OutputSpdif_type getMruaOutSpdifTypeFromSnmpMode(CONST Mp7xxCommon::SNMP_OUTPUT_SPDIF_MODE eSnmpOutSpdifMode);
enum EMhwlibPictureOrientation getMruaVideoRotationTypeFromSnmpMode(CONST Mp7xxCommon::VIDEO_ROTATION_t eSnmpVideoRotationMode);
#endif
	
#endif	//_SNMP_CMD_CONV_H_
