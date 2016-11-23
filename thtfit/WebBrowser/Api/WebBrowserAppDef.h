#ifndef	_WEB_BROWSER_APP_DEF_H_
#define	_WEB_BROWSER_APP_DEF_H_

#define	MEDIA_VIR_CH_LIST_FILE_PATH		"Config/VirtualChannel.xml"

//FileType definition
#define	FileType_Desc_Video				"Video"
#define	FileType_Desc_Audio				"Audio"
#define	FileType_Desc_Picture			"Picture"
#define	FileType_Desc_Playlist			"Playlist"
#define	FileType_Desc_Html				"Html"
#define	FileType_Browser			"Browser"
#define	FileType_Http				"HTTP"
#define	FileType_Music				"Music"
#define	FileType_Udp				"Udp"
#define	FileType_Rtsp				"rtsp"
#define	FileType_Rtp				"rtp"
#define	FileType_Stream				"Stream"
#define	FileType_Tuner				"tuner"
#if 1/*added by lxj 2012-9-11*/
#define FileType_Capture_USB        "usb"
#endif

#if 1/*added by abner 2014-10-31*/
#define FileType_Capture_HDMI             "hdmi"
#endif

#if 1/*added by lxj 2012-10-9*/
#define USB_CAPTURE_URL             "capture://usb"
#endif


//First charater indicates where the destination file stores
#define	DST_LOCATION_ID_FLASH		"F"
#define	DST_LOCATION_ID_RAM			"R"
#define	DST_LOCATION_ID_SDCARD		"S"
#define	DST_LOCATION_ID_UDISK		"U"
#define	DST_LOCATION_ID_HDD			"H"

#define	PRF_FILE_EXTENSION_NAME		"prf"

//activeMedia description
#define	ActiveMedia_Desc_FLASH			"Flash"
#define	ActiveMedia_Desc_SdCard		"SdCard"
#define	ActiveMedia_Desc_USB			"Usb"
#define	ActiveMedia_Desc_HDD			"HDD"
#define	ActiveMedia_Desc_RAN			"RAM"
#define	ActiveMedia_Desc_TUNER			"TUNER"
#define ActiveMedia_Desc_Capture        "capture"

#define	Media_Desc_URL			"Url"

//CeLabs data source device
#define	CeLabs_DATA_SRC_DEV_FLASH		"flash"
#define	CeLabs_DATA_SRC_DEV_USB			"usb"
#define	CeLabs_DATA_SRC_DEV_HDD			"hdd"
#define	CeLabs_DATA_SRC_DEV_SD			"sd"
#define	CeLabs_DATA_SRC_DEV_UDP			"udp"
#define	CeLabs_DATA_SRC_DEV_RTP			"rtp"
#define	CeLabs_DATA_SRC_DEV_TUNER		"tuner"
#define	CeLabs_DATA_SRC_DEV_WEBPAGE	"http"
#define	CeLabs_DATA_SRC_DEV_Url			"url"
#define	CeLabs_DATA_SRC_DEV_RTSP			"rtsp"
#define	CeLabs_DATA_SRC_DEV_IDE			"ide"
#if 1/*added by lxj 2012-9-11*/
#define CeLabs_DATA_SRC_DEV_CAPTURE     "capture"
#endif


#define	INI_KEY_GpioPlayFilePath_1			"GPIO/GpioPlayFilePath_1"
#define	INI_KEY_GpioPlayFilePath_2			"GPIO/GpioPlayFilePath_2"
#define	INI_KEY_GpioPlayFilePath_3			"GPIO/GpioPlayFilePath_3"
#define	INI_KEY_GpioPlayFilePath_4			"GPIO/GpioPlayFilePath_4"
#define	INI_KEY_GpioPlayFilePath_5			"GPIO/GpioPlayFilePath_5"
#define	INI_KEY_GpioPlayFilePath_6			"GPIO/GpioPlayFilePath_6"
#define	INI_KEY_GpioPlayFilePath_7			"GPIO/GpioPlayFilePath_7"
#define	INI_KEY_GpioPlayFilePath_8			"GPIO/GpioPlayFilePath_8"

#define	INI_KEY_GpioPlayFilePath			"GPIO/GpioPlayFilePath"		

#define	SysProp_KEY_AutoAnnounceIntervalSec		"AutoAnnounceIntervalSec"
#define	SysProp_KEY_XappVersion					"XappVer"
#define 	SysProp_KEY_CurOsdResolutionId 			"Display.Osd.ResolutionId"
#define 	SysProp_KEY_TunerScanStatus 	"TunerCurrScanStatus"
#define	SysProp_KEY_TunerChannelMode	"TunerChannelMode"

 
#define    SHIFTMINSIZE 	10
#define	GPIOBUTTONSIZE			8	

#define  MENUOSDTIMEOUT_MS   	(5*60*1000)

#define TUNER_HAS_SCAN 1
#define TUNER_NO_SCAN 0


#define	PARAM_KEY_CommitType		"CommitType"
#define	PARAM_KEY_DefEthDevName				"DefEthDevName"
#define	PARAM_KEY_WifiEthDhcp				"WifiDhcp"
#define	PARAM_KEY_WifiEthIP				"WifiIP"
#define	PARAM_KEY_WifiEthSubnetMask				"WifiSubnetMask"
#define	PARAM_KEY_WifiGateway				"WifiGateway"
#define	PARAM_KEY_WifiDns1				"Dns1"
#define	PARAM_KEY_WifiDns2				"Dns2"
#define	PARAM_KEY_WifiDomainName				"DomainName"

#define	PARAM_KEY_WifiSsid				"ssid"
#define	PARAM_KEY_WifiSecurity				"security"
#define	PARAM_KEY_WifiChannel				"channel"
#define	PARAM_KEY_WifiKey				"key"



typedef enum
{
	AMedia_TYPE_VIDEO,
	AMedia_TYPE_AUDIO,
	AMedia_TYPE_PICTURE,
	AMedia_TYPE_PLAYLIST,
	AMedia_TYPE_HTML,
	AMedia_TYPE_Tuner,	
	AMedia_TYPE_Url,
	AMedia_TYPE_MAX
}AMedia_File_TYPE;

typedef enum
{
	MediaPlay_Status_NullPlayStatus =0,
	MediaPlay_Status_PlayPlayList,
	MediaPlay_Status_PlaySelectFile,
	MediaPlay_Status_PlayCeLabsFile,
	MediaPlay_Status_PlayIdlePlayList	
}MediaPlay_STATUS;

typedef enum {
    AMedia_TYPE_Unknown = -1,
    AMedia_TYPE_FLASH,
    AMedia_TYPE_USB,
    AMedia_TYPE_SdCard,
    AMedia_TYPE_HDD,
    AMedia_TYPE_RAM,
    AMedia_TYPE_TUNER,
    AMedia_TYPE_URL,
#if 1/*added by lxj 2012-9-11*/
	AMedia_TYPE_CAPTURE,
#endif
    AMedia_TYPE_Invalid
} ActiveMedia_TYPE, *P_ActiveMedia_TYPE;

typedef enum
{
	State_Idle=0,
	State_IdleWithStreamAddress,
	State_Image,
	State_ImageWithStreamAddress,
	State_ImageWhileStreaming,
	State_Streaming,
	State_PlayerError,
	State_StreamServerError,
	State_NetworkError,
	State_UnknownError,
	State_PlayingVideoFile = 20,
	State_PlayingAudioFile =21,	
	State_PlayingPictureFile =23,
	State_PlayingBrowserFile = 24,
	State_PlayingCapture,
	State_PlayingTuner,
	State_FilePlayError=30,
	State_MenuOSD=40,
	State_UserOSD,
	State_PlayingPlayList = 60,
#if 1/*added by lxj 2012-10-9 for usb capture*/
	State_USB_Capture,
#endif
#if 1/*added by abner 2014-11-3 for hdmi capture*/
    State_HDMI_Capture,
#endif    
	State_Unknown = 255
}State_TYPE;

typedef enum
{
	RELATIVE_MODE =0,
	ABSOLUTE_MODE,
}RUA_DISPLAY_MODE;

typedef enum
{
	CONTENT_TRIGGER_SRC_Unknown,
	CONTENT_TRIGGER_SRC_DefaultStartupItem,
	CONTENT_TRIGGER_SRC_DefaultPlaylist,
	CONTENT_TRIGGER_SRC_PlayVirChannleItem,
	CONTENT_TRIGGER_SRC_PlaySelectItem,
}CONTENT_TRIGGER_SRC_TYPE, *P_CONTENT_TRIGGER_SRC_TYPE;

typedef enum
{
	FORMAT_MEDIA_UnKnow = 0,
	FORMAT_MEDIA_InNow,
	REPAIR_MEDIA_InNow,
	FORMAT_MEDIA_Cancel ,
	FORMAT_MEDIA_Error ,
	FORMAT_MEDIA_Success 	
}FORMAT_MEDIA_STATUS, *P_FORMAT_MEDIA_STATUS;


typedef enum
{
	PlayMedia_MODE =0,
	playNextFile_MODE,
	playVideoLayer_MODE,
	playHtmlLayer_MODE
}MediaPlay_Commad_MODE;

typedef enum
{
	ATSC_MODE =0,
	QAM64_MODE,
	QAM256_MODE
}Tuner_Scan_MODE;

typedef enum
{
	SystemUpgrade_Idle=0,
	SystemUpgrade_Start ,
	SystemUpgrade_DownloadFile,
	SystemUpgrade_DownloadFileError,
	SystemUpgrade_Fail,
	SystemUpgrade_Success	
}System_Upgrade_Status, *P_System_Upgrade_Status;

typedef enum
{
	TunerStauts_Idle=0,
	TunerScan_Start ,
	TunerScanning,
	TunerScan_Complete	
}TunerScan_Status, *P_TunerScan;

typedef enum
{
	Stop_TunerScan = 0,
	Start_TunerScan,
	Reset_TunerScan	
}Set_TunerScan_Status, *P_SetTunerScanStatus;

typedef enum {
    Type_EnableDhcp = 0, 
	Type_IPv4Addr, 
	Type_SubnetMask, 
	Type_Gateway,
	Type_DomainName, 
	Type_Dns1,
	Type_Dns2, 
	Type_SSID, 
	Type_Key, 
	Type_Channel,
	Type_Security,
	Type_KeyIndex, 
	Type_Type
} CurrDBusWifiType;

//getTransferControl status code
#define	TranCtrlStat_DownloadIdle				0x00
#define	TranCtrlStat_Download_Image_Successful 	0x01
#define	TranCtrlStat_DownloadCanceled			0x02
#define	TranCtrlStat_DownloadErr_NoServer		0x03
#define	TranCtrlStat_DownloadErr_NetworkErr		0x81
#define	TranCtrlStat_DownloadErr_FileErr		0x82
#define	TranCtrlStat_DownloadErr_LoginErr		0x85
#define	TranCtrlStat_DownloadErr_OtherTransferErr		0x86
#define	TranCtrlStat_DownloadInProgress			0xFF

#define	TranCtrlStat_FirmwareDone			0xF0
#define	TranCtrlStat_FirmwareUploading 		0xF1
#define	TranCtrlStat_FirmwareBurning1 		0xF2
#define	TranCtrlStat_FirmwareBurning2		0xF3
#define	TranCtrlStat_FirmwareBurning3		0xF4
#define	TranCtrlStat_FirmwareFailed			0xF5
#define	TranCtrlStat_FirmwareBadAddress	0xF6
#define	TranCtrlStat_FirmwareConnectionError	0xF7
#define	TranCtrlStat_FirmwareBadFile	0xF8
#define 	TranCtrlStat_FirmwareTooLarge 	0xF9
#define	TranCtrlStat_FirmwarePowerError	0xFA
#define	TranCtrlStat_FirmwareOtherError		0xFB


//setTransferControl command code
#define	TranCtrlCmd_CancelDownload			0x87

#define	MAX_DownloadFile_ConcurrentCnt		20
#define	MAX_UploadFile_ConcurrentCnt		20

// DBus service name
#define	DBUS_SrvName_WebBrowser				"org.thtfit.WebBrowser"

//DBus path
#define	DBUS_PATH_WebBrowser_DatCommSrv		"/org/thtfit/WebBrowser/DataCommSrv"

//DBus interface name
#define	DBUS_IF_WebBrowser_DataCommSrv		"org.thtfit.WebBrowser.DataCommSrv"

// DBus service name
#define	DBUS_SrvName_ControlCenter		"org.thtfit.ControlCenter"
#define	DBUS_PATH_WebBrowser_UiSrv		"/org/thtfit/ControlCenter/UiSrv"
#define	DBUS_IF_ControlCenter_UiSrv		"org.thtfit.ControlCenter.UiSrv"

#define	DBUS_METHOD_getAudioOutputMode	"getAudioOutputMode"
#define	DBUS_METHOD_setAudioOutputMode	"setAudioOutputMode"
#define	DBUS_METHOD_setAutoAnnounce		"setAutoAnnounce"
#define	DBUS_METHOD_getAutoAnnounce		"getAutoAnnounce"
#define	DBUS_METHOD_getCardCurrentStatus	"getCardCurrentStatus"
#define	DBUS_METHOD_getMediaStatus	"getMediaStatus"
#define	DBUS_METHOD_getTransferControl	"getTransferControl"
#define	MP_SIGNAL_PlaybackEos2				"PlaybackEos2"

#define	NewSysCfg_DOWNLOAD_TEMP_FILE_PATH	"/Temp/NewSysCfg.tar"
#define	NewSystemVersion_DOWNLOAD_TEMP_FILE_PATH	"/Temp/NewSystemUpgrade.tgz"
#define	FILE_PATH_NewSystemUpgradePackage_InRam		"/tmp/NewSystemUpgrade.tgz"
#define	NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH	"/Temp/Update.xml"

#define	NewSystemVersion_DECOMPRESSION_TEMP_DIR_PATH  "/Temp/Updgrade"

#define	NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH  "/Temp/Updgrade/SystemUpgrade"


#define	NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH  "/Temp/GuiDef.tgz"
#define	NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH  "/Temp/GuiDef"

#define	DEFAULT_AutoAnnounce_UDP_PORT		4657

#ifdef	__cplusplus
namespace ControlCenter
{
#endif	//__cplusplus

//FormatMedia
typedef enum
{
	FORMAT_MEDIA_ACTION_FORMAT = 0,
	FORMAT_MEDIA_ACTION_REPAIR = 3
}FORMAT_MEDIA_ACTION_TYPE, *P_FORMAT_MEDIA_ACTION_TYPE;

typedef enum
{
	AUD_OUT_MODE_Invalid = -1,
	AUD_OUT_MODE_Normal = 0,
	AUD_OUT_MODE_DisableHdmi = 1,
	AUD_OUT_MODE_DisableAll = 2,
	AUD_OUT_MODE_DisableAnalog = 3
}AUDIO_OUTPUT_MODE, *P_AUDIO_OUTPUT_MODE;

//PlayMode
typedef enum
{
	PlayMode_Normal = 0,
	PlayMode_StopContent = 1,
	PlayMode_FastForward = 2,
	PlayMode_Rewind = 3,
	PlayMode_ChapterPlus = 4,
	PlayMode_ChapterMinus = 5,
	PlayMode_Pause = 6,
	PlayMode_StopAll = 7,
	PlayMode_StopMenu = 8,
	PlayMode_StopVideoLayer =17,
	PlayMode_StopHtmlLayer = 33
}PLAY_MODE, *P_PLAY_MODE;

typedef enum 
{
	AMedia_POSTION_X =0,
	AMedia_POSTION_Y,
	AMedia_POSTION_XSIZE,
	AMedia_POSTION_YSIZE,
	AMedia_POSTION_ShiftX,
	AMedia_POSTION_ShiftY,
	AMedia_POSTION_ShiftXSIZE,
	AMedia_POSTION_ShiftYSIZE	
}PostionInfo;


typedef enum
{
	FsMountEvt_SymLink = 1,
	FsMountEvt_SymUnlink = 2
}FS_MOUNT_EVENT_ID, *P_FS_MOUNT_EVENT_ID;

typedef enum
{
	CE_SNMP_VidMode_Normal = 0,
	CE_SNMP_VidMode_Wide = 1,
	CE_SNMP_VidMode_Anamorphic = 2,
	CE_SNMP_VidMode_FULL = 3
}CE_SNMP_VIDEO_MODE_T, *P_CE_SNMP_VIDEO_MODE_T;

#ifdef	__cplusplus
}//namespace ControlCenter
#endif	//__cplusplus

#endif	//_WEB_BROWSER_APP_DEF_H_

