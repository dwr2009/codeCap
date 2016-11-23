#ifndef	_MEDIA_PLAYER_APP_DEF_H_
#define	_MEDIA_PLAYER_APP_DEF_H_

#define	MP_SETTING_XML_DOC_PATH			"Config/MediaPlayer.xml"

#define DBUS_MPLAYER_SERVICE_NAME  		"org.thtfit.MPlayer"
//Not necessary to use OBJECT_PATH, because we have only one MPlayer instance.
#define DBUS_MPLAYER_OBJECT_PATH  		"/org/thtfit/MPlayer"
#define DBUS_MPLAYER_OBJECT_INTERFACE  	"org.thtfit.MPlayer"

#define	METHOD_setDisplayParam			"setDisplayParam"
#define	METHOD_getDisplayParam			"getDisplayParam"
#define	METHOD_getDisplayPosition		"getDisplayPosition"
#define	METHOD_getVideoPosition		"getVideoPosition"
#define	METHOD_setMonitorType			"setMonitorType"
#define	METHOD_getMonitorType			"getMonitorType"
#define	METHOD_setPlaySpeedCtrlAsync	"setPlaySpeedCtrlAsync"
#define	METHOD_getPlaySpeedCtrl			"getPlaySpeedCtrl"
#define	METHOD_getCurClkTicks			"getCurClkTicks"
#define	METHOD_setCurClkTicks			"setCurClkTicks"
#define	METHOD_getConfigDateTime		"getConfigDateTime"
#define	METHOD_setConfigDateTime		"setConfigDateTime"
#define	METHOD_setVideoDispAspectRatio	"setVideoDispAspectRatio"
#define	METHOD_setOutputSpdifMode		"setOutputSpdifMode"
#define	METHOD_getOutputSpdifMode		"getOutputSpdifMode"
#define	METHOD_setHdmiAudioOutput		"setHdmiAudioOutput"
#define	METHOD_setAnalogAudioMute		"setAnalogAudioMute"
#define	DBUS_METHOD_setSerialParameters	"setSerialParameters"
#define	DBUS_METHOD_getSerialParameters	"getSerialParameters"
#define	DBUS_METHOD_getSerialControlData	"getSerialControlData"
#define	DBUS_METHOD_setSerialControlData	"setSerialControlData"
#define	METHOD_ChangeVideoOutputMode	"ChangeVideoOutputMode"
#define	METHOD_setVideoOutputModeAsync	"setVideoOutputModeAsync"

#if 1 /*added by bert 2016/11/12*/
#define	METHOD_setHDRAsync				"setHDRAsync"
#define	METHOD_getHDRAsync				"getHDRAsync"

#endif

#define	METHOD_getCpuSerialNo			"getCpuSerialNo"
#define	METHOD_setHdmi3DTvMode			"setHdmi3DTvMode"
#if 1/*added by lxj 2012-9-28*/
#define METHOD_getVideoDispAspectRatio  "getVideoDispAspectRatio"
#endif
#if 1/*added by lxj 2012-11-22*/
#define METHOD_getVideoSyncMode         "getVideoSyncMode"
#define METHOD_setVideoSyncMode         "setVideoSyncMode"
#define METHOD_getVideoSyncMaster       "getVideoSyncMaster"
#define METHOD_setVideoSyncMaster       "setVideoSyncMaster"
#endif
#define	METHOD_setHdmiCecCtrl			"setHdmiCecCtrl"
#define	METHOD_setShowClosedCaption		"setShowClosedCaption"
#define	METHOD_getShowClosedCaption    "getShowClosedCaption"

#if 1 /*Add by xuweiwei 2014-2-18*/
#define	METHOD_setVideoRotation		"setVideoRotation"
#define	METHOD_getVideoRotation             "getVideoRotation"
#endif

#if 1 /*Add by xuweiwei 2014-3-17*/
#define	METHOD_setEnableAudio6Ch          "setEnableAudio6Ch"
#define	METHOD_getEnableAudio6Ch          "getEnableAudio6Ch"
#endif

#if 1/*Add by xuweiwei 2015-8-28*/
#define METHOD_getDisplayOutputMode		  "getDisplayOutputMode"
#define METHOD_setDisplayOutputMode		  "setDisplayOutputMode"
#define	METHOD_getvideoColorSpace 	  	  "getvideoColorSpace"
#define	METHOD_setvideoColorSpace 	  	  "setvideoColorSpace"
#endif

#define	XML_DispParam_Ele_PATH			"Display"
#define	XML_OsdPositionParam_Ele_PATH	"OsdPosition"
#define	XML_Monitor_Ele_PATH			"Monitor"

#define	PropDesc_Brightness				"Brightness"
#define	PropDesc_Contrast				"Contrast"
#define	PropDesc_Saturation				"Saturation"
#define	PropDesc_Hue					"Hue"
#define	PropDesc_ColorSpace				"ColorSpace"
#define	PropDesc_ColorDepth				"ColorDepth"
#define	PropDesc_Matrix					"Matrix"
#define	PropDesc_VoMode					"VoMode"
#define	PropDesc_Type					"Type"

#define	PropDesc_ShiftX			"ShiftHorizontal"
#define	PropDesc_shiftXSIZE		"WidthAdjustment"
#define	PropDesc_ShiftY			"ShiftVertical"
#define	PropDesc_ShiftYSIZE		"HeightAdjustment"

#if 1/*added by lxj 2012-9-28*/
#define PropDesc_VideoMode              			"VideoMode"
#endif

//SysProp keys
#define	SysProp_KEY_PrevEthernetIP		   "PrevEthernetIP"
#define	SysProp_KEY_CurEthernetIP		   "CurEthernetIP"
#define	SysProp_KEY_SerialParameters	   "SerialParameters"
#define	SysProp_KEY_OutputSpdifMode			"OutputSpdifMode"
#define	SysProp_KEY_NeedInitOsdForVcr      "NeedInitOsdForVcr"
#define	SysProp_KEY_ShowClosedCaption	   "ShowClosedCaption"
#define	SysProp_KEY_VideoRotationMode     "VideoRotationMode"
#define	SysProp_KEY_EnAudio6ChFlag          "EnAudio6ChFlag"
                                                 


#ifdef ENABLE_DTV
#define	SysProp_KEY_TunerStdMode		"TunerStdMode"
#define	SysProp_KEY_TunerScanStatus		"TunerCurrScanStatus"
#define	SysProp_KEY_TunerChannelMode	"TunerChannelMode"
#endif

#define	DispParam_LowLimit_Percent		25

#ifdef	__cplusplus
namespace MediaPlayer
{
#endif	//__cplusplus
	
typedef enum
{
	PlayMode_Normal = 0,
	PlayMode_FastForward = 2,
	PlayMode_Rewind = 3,
	PlayMode_Pause = 6,
	PlayMode_Stopped = 7
}PlayMode, *P_PlayMode;

typedef enum
{
	DispPARAM_Brightness = 1,
	DispPARAM_Contrast,
	DispPARAM_Saturation,
	DispPARAM_Hue,
	DispPARAM_ColorSpace,
	DispPARAM_ColorDepth,
	DispPARAM_matrixcoefficients,
	DispPARAM_Max
}DISP_PARAM_TYPE, *P_DISP_PARAM_TYPE;

typedef enum
{
	Matrixcoefficients_Auto=0,
	Matrixcoefficients_Bt709,
	Matrixcoefficients_Smpte_170M,
	Matrixcoefficients_BT_470_2_BG,
	Matrixcoefficients_2020ncl,
	Matrixcoefficients_2020cl,
	Matrixcoefficients_Max
}MATRIXCOEFFICIENTS_TYPE, *P_MATRIXCOEFFICIENTS_TYPE;

typedef enum
{
	ColorSpace_Auto=0,
	ColorSpace_Rgb,
	ColorSpace_YCbCr422,
	ColorSpace_YCbCr444,
	ColorSpace_YCbCr420,
	ColorSpace_Max
}ColorSpace_TYPE, *P_ColorSpace_TYPE;

typedef enum
{
	ColorDepth_Auto=0,
	ColorDepth_8bit,
	ColorDepth_10bit,
	ColorDepth_12bit,
	ColorDepth_Max
}ColorDepth_TYPE, *P_ColorDepths_TYPE;


typedef enum
{
	QMedia_POSTION_X =0,
	QMedia_POSTION_Y,
	QMedia_POSTION_XSIZE,
	QMedia_POSTION_YSIZE,
	QMedia_POSTION_ShiftX,
	QMedia_POSTION_ShiftY,
	QMedia_POSTION_ShiftXSIZE,
	QMedia_POSTION_ShiftYSIZE	
}Postion_Info;

typedef enum
{
	VO_MODE_NotSet,		//for read
	VO_MODE_START_VALUE,	//available start
	VO_MODE_HDMI_720p50,	// 2
	VO_MODE_HDMI_1080i50,	// 3
	VO_MODE_HDMI_1080p50,	// 4
	VO_MODE_HDMI_480p60,	// 5
	VO_MODE_HDMI_480i60,	// 6
	VO_MODE_HDMI_576p50,	// 7
	VO_MODE_640x480p59,//8
	VO_MODE_640x480p60,//9
	VO_MODE_HDMI_480p59,//10
	VO_MODE_HDMI_720p59,//11
	VO_MODE_HDMI_1080i59,//12
	VO_MODE_2880x240,//13
	VO_MODE_1440x480p59,//14
	VO_MODE_1440x480p60,//15
	VO_MODE_HDMI_1080p59,//16
	VO_MODE_720x288p,//17
	VO_MODE_2880x576i50,//18
	VO_MODE_HDMI_2880x288p50,//19
	VO_MODE_1440x576i50,//20
	VO_MODE_HDMI_1080p23,//21
	VO_MODE_1080p24,//22
	VO_MODE_1080p25,//23
	VO_MODE_1080p29,//24
	VO_MODE_1080p30,//25
	VO_MODE_2880x480p59,//26
	VO_MODE_2880x480p,//27
	VO_MODE_2880x576p50,//28
	VO_MODE_1080i50_1250,//29
	VO_MODE_1080i100,//30
	VO_MODE_720p100,//31
	VO_MODE_576p100,//32
	VO_MODE_576i100,//33
	VO_MODE_1080i119,//34
	VO_MODE_1080i120,//35
	VO_MODE_HDMI_CAV_1080p60,//36
	VO_MODE_HDMI_CAV_1080i60,//37
	VO_MODE_HDMI_CAV_720p60,//38
	VO_MODE_COMPONENT_480p60,//39
	VO_MODE_COMPONENT_1080p50,//40
	VO_MODE_COMPONENT_1080p60,//41
	VO_MODE_COMPONENT_1080i60,//42
	VO_MODE_COMPONENT_720p50,//43
	VO_MODE_COMPONENT_720p60,//44
	VO_MODE_COMPONENT_1080i50,//45
	VO_MODE_COMPONENT_576p50,//46
	VO_MODE_VESA_1280x1024x60,//47
	VO_MODE_VESA_720x400x70,//48
	VO_MODE_VESA_800x600x60,//49
	VO_MODE_VESA_1024x768x60,//50
	VO_MODE_VESA_1280x768x60,//51
	VO_MODE_VESA_1360x768x60,	//52
	VO_MODE_VESA_1366x768x60,//53
	VO_MODE_VESA_1600x1200x60,//54
	VO_MODE_RGB_1080i50,//55
	VO_MODE_CVBS,//56
	VO_MODE_END_VALUE,		//available start
	VO_MODE_HDMI_2160p24,  //4k 58
	VO_MODE_HDMI_2160p25,//59
	VO_MODE_HDMI_2160p30,//60
	VO_MODE_HDMI_2160p50,//61
	VO_MODE_HDMI_2160p60,//62
	VO_MODE_HDMI_4K2Kp24,//63
	VO_MODE_HDMI_4K2Kp25,//64
	VO_MODE_HDMI_4K2Kp30,//65
	VO_MODE_HDMI_4K2Kp50,//66
	VO_MODE_HDMI_4K2Kp60,//67
	VO_MODE_SPLIT_MODE_MIN_BOUNDARY,	//SPLIT start boundary
	VO_MODE_SPLIT_HdmiVcr_VgaMain,//69
	VO_MODE_SPLIT_HdmiVcr_1080p60_VgaMain,	//70
	VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr,	//71
	VO_MODE_SPLIT_MODE_MAX_BOUNDARY //SPLIT end boundary
}VIDEO_OUTPUT_MODE, *P_VIDEO_OUTPUT_MODE;

typedef enum
{
	RUA_DISP_UNKNOWN,
	RUA_DISP_VIDEO,
	RUA_DISP_OSD
}RUA_DISPLAY_TYPE, *P_RUA_DISPLAY_TYPE;

typedef enum
{
	MONITOR_MANUFACTURER_ID_SONY = 1,
	MONITOR_MANUFACTURER_ID_LG = 2,
	MONITOR_MANUFACTURER_ID_NEC = 3,
	MONITOR_MANUFACTURER_ID_PANASONIC = 4,
	MONITOR_MANUFACTURER_ID_PIONEER = 5,
	MONITOR_MANUFACTURER_ID_SAMSUNG = 6,
	MONITOR_MANUFACTURER_ID_UNDEFINED = 7,
	MONITOR_MANUFACTURER_ID_PASSTHRU = 8
}MONITOR_MANUFACTURER_ID, *P_MONITOR_MANUFACTURER_ID;

typedef enum
{
	DispAspectRatio_DEFAULT = 0,
	DispAspectRatio_16_9 = 1,
	DispAspectRatio_4_3 = 2,
	DispAspectRatio_FULL = 3
}DISPLAY_ASPECT_RATIO, *P_DISPLAY_ASPECT_RATIO;

typedef enum
{
	H3DTvMode_MinValue,
	H3DTvMode_Normal = H3DTvMode_MinValue,
	H3DTvMode_Force2D,	
	H3DTvMode_MaxValue = H3DTvMode_Force2D
}HDMI_3D_TV_MODE, *P_HDMI_3D_TV_MODE;

typedef enum {
	CC_SEL_MIN_VALUE = 0,
	CC_SEL_Disabled = CC_SEL_MIN_VALUE,
	CC_SEL_CC1 = 1,
	CC_SEL_CC2 = 2,
	CC_SEL_CC3 = 3,
	CC_SEL_CC4 = 4,
	CC_SEL_EIA708 = 5,
	CC_SEL_MAX_VALUE = CC_SEL_EIA708
}ClosedCaption_SELECTION, *P_ClosedCaption_SELECTION;

//OptFlag for DISPLAY_ASPECT_RATIO
#define	DAR_ONLY_APPLY_ONCE_AT_NEXT_PLAY			0x00000001

#define	SerialComPort_DevPath			"/dev/ttyS1"

#ifdef	__cplusplus
}	//namespace MediaPlayer
#endif	//__cplusplus

#endif	//_MEDIA_PLAYER_APP_DEF_H_

