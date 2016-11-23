#include "Api/SnmpCmdConv.h"
#include "ErrPrintHelper.h"

typedef struct
{
	MediaPlayer::VIDEO_OUTPUT_MODE eVoMode;
	int iSnmpResVal;
}MpVoMode_SnmpResVal_PAIR, *P_MpVoMode_SnmpResVal_PAIR;

static MpVoMode_SnmpResVal_PAIR g_MpVoMode_SnmpResVal_Table[] = 
{
	{MediaPlayer::VO_MODE_HDMI_720p50, 13},
	{MediaPlayer::VO_MODE_RGB_1080i50, 6},
	{MediaPlayer::VO_MODE_HDMI_1080i50, 7},
	{MediaPlayer::VO_MODE_HDMI_1080p50, 34},
	{MediaPlayer::VO_MODE_HDMI_480p60, 11},
	{MediaPlayer::VO_MODE_HDMI_480i60, 12},
	{MediaPlayer::VO_MODE_HDMI_576p50, 14},
	{MediaPlayer::VO_MODE_640x480p59, 15},
	{MediaPlayer::VO_MODE_640x480p60, 16},
	{MediaPlayer::VO_MODE_HDMI_480p59, 17},
	{MediaPlayer::VO_MODE_HDMI_720p59, 18},
	{MediaPlayer::VO_MODE_HDMI_1080i59, 19},
	{MediaPlayer::VO_MODE_2880x240, 26},
	{MediaPlayer::VO_MODE_1440x480p59, 27},
	{MediaPlayer::VO_MODE_1440x480p60, 28},
	{MediaPlayer::VO_MODE_HDMI_1080p59, 29},
	{MediaPlayer::VO_MODE_720x288p, 30},
	{MediaPlayer::VO_MODE_2880x576i50, 31},
	{MediaPlayer::VO_MODE_HDMI_2880x288p50, 32},
	{MediaPlayer::VO_MODE_1440x576i50, 33},
	{MediaPlayer::VO_MODE_HDMI_1080p23, 35},
	{MediaPlayer::VO_MODE_1080p24, 36},
	{MediaPlayer::VO_MODE_1080p25, 37},
	{MediaPlayer::VO_MODE_1080p29, 38},
	{MediaPlayer::VO_MODE_1080p30, 39},
	{MediaPlayer::VO_MODE_2880x480p59, 40},
	{MediaPlayer::VO_MODE_2880x480p, 41},
	{MediaPlayer::VO_MODE_2880x576p50, 42},
	{MediaPlayer::VO_MODE_1080i50_1250, 43},
	{MediaPlayer::VO_MODE_1080i100, 44},
	{MediaPlayer::VO_MODE_720p100, 45},
	{MediaPlayer::VO_MODE_576p100, 46},
	{MediaPlayer::VO_MODE_576i100, 47},
	{MediaPlayer::VO_MODE_1080i119, 48},
	{MediaPlayer::VO_MODE_1080i120, 49},
	{MediaPlayer::VO_MODE_HDMI_CAV_1080p60, 8},
	{MediaPlayer::VO_MODE_HDMI_CAV_1080i60, 9},
	{MediaPlayer::VO_MODE_HDMI_CAV_720p60, 10},
	{MediaPlayer::VO_MODE_COMPONENT_480p60, 53},
	{MediaPlayer::VO_MODE_COMPONENT_1080p60, 50},
	{MediaPlayer::VO_MODE_COMPONENT_1080i60, 51},
	{MediaPlayer::VO_MODE_COMPONENT_720p60, 52},
	{MediaPlayer::VO_MODE_COMPONENT_1080p50, 60},
	{MediaPlayer::VO_MODE_COMPONENT_1080i50, 61},
	{MediaPlayer::VO_MODE_COMPONENT_720p50, 62},
	{MediaPlayer::VO_MODE_COMPONENT_576p50, 63},
	{MediaPlayer::VO_MODE_VESA_1280x768x60, 3},
	//{MediaPlayer::VO_MODE_VESA_1280x768x60, 22},
	{MediaPlayer::VO_MODE_VESA_1280x1024x60, 2},
	//{MediaPlayer::VO_MODE_VESA_1280x1024x60, 23},
	{MediaPlayer::VO_MODE_VESA_800x600x60, 5},
	//{MediaPlayer::VO_MODE_VESA_800x600x60, 20},
	{MediaPlayer::VO_MODE_VESA_1024x768x60, 4},
	//{MediaPlayer::VO_MODE_VESA_1024x768x60, 21},
	{MediaPlayer::VO_MODE_VESA_1360x768x60, 1},
	//{MediaPlayer::VO_MODE_VESA_1360x768x60, 24},
	{MediaPlayer::VO_MODE_VESA_1366x768x60, 80},
	{MediaPlayer::VO_MODE_VESA_1600x1200x60, 25},
	{MediaPlayer::VO_MODE_CVBS, 68},	
	//split dual display
	{MediaPlayer::VO_MODE_SPLIT_HdmiVcr_VgaMain, 90},	
	{MediaPlayer::VO_MODE_SPLIT_HdmiVcr_1080p60_VgaMain, 91},
	{MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr, 92},

	/*4K*/
	{MediaPlayer::VO_MODE_HDMI_2160p24, 100},
	{MediaPlayer::VO_MODE_HDMI_2160p25, 101},
	{MediaPlayer::VO_MODE_HDMI_2160p30, 102},
	{MediaPlayer::VO_MODE_HDMI_2160p50, 103},
	{MediaPlayer::VO_MODE_HDMI_2160p60, 104},

	
	{MediaPlayer::VO_MODE_HDMI_4K2Kp24, 110},
	{MediaPlayer::VO_MODE_HDMI_4K2Kp25, 111},
	{MediaPlayer::VO_MODE_HDMI_4K2Kp30, 112},
	{MediaPlayer::VO_MODE_HDMI_4K2Kp50, 113},
	{MediaPlayer::VO_MODE_HDMI_4K2Kp60, 114},	

	/*add by clyde 2015-5-14 In order to make the number set will not go wrong*/
	#if 1
	{MediaPlayer::VO_MODE_NotSet, 0},
	{MediaPlayer::VO_MODE_NotSet, 20},
	{MediaPlayer::VO_MODE_NotSet, 21},
	{MediaPlayer::VO_MODE_NotSet, 22},
	{MediaPlayer::VO_MODE_NotSet, 23},
	{MediaPlayer::VO_MODE_NotSet, 24},

	{MediaPlayer::VO_MODE_NotSet, 54},
	{MediaPlayer::VO_MODE_NotSet, 55},
	{MediaPlayer::VO_MODE_NotSet, 56},
	{MediaPlayer::VO_MODE_NotSet, 57},
	{MediaPlayer::VO_MODE_NotSet, 58},
	{MediaPlayer::VO_MODE_NotSet, 59},
	
	{MediaPlayer::VO_MODE_NotSet, 64},
	{MediaPlayer::VO_MODE_NotSet, 65},
	{MediaPlayer::VO_MODE_NotSet, 66},
	{MediaPlayer::VO_MODE_NotSet, 67},
	{MediaPlayer::VO_MODE_NotSet, 69},
	{MediaPlayer::VO_MODE_NotSet, 70},
	{MediaPlayer::VO_MODE_NotSet, 71},
	{MediaPlayer::VO_MODE_NotSet, 72},
	{MediaPlayer::VO_MODE_NotSet, 73},
	{MediaPlayer::VO_MODE_NotSet, 74},
	{MediaPlayer::VO_MODE_NotSet, 75},
	{MediaPlayer::VO_MODE_NotSet, 76},
	{MediaPlayer::VO_MODE_NotSet, 77},
	{MediaPlayer::VO_MODE_NotSet, 78},
	{MediaPlayer::VO_MODE_NotSet, 79},
	
	{MediaPlayer::VO_MODE_NotSet, 81},
	{MediaPlayer::VO_MODE_NotSet, 82},
	{MediaPlayer::VO_MODE_NotSet, 83},
	{MediaPlayer::VO_MODE_NotSet, 84},
	{MediaPlayer::VO_MODE_NotSet, 85},
	{MediaPlayer::VO_MODE_NotSet, 86},
	{MediaPlayer::VO_MODE_NotSet, 87},
	{MediaPlayer::VO_MODE_NotSet, 88},
	{MediaPlayer::VO_MODE_NotSet, 89},

	{MediaPlayer::VO_MODE_NotSet, 93},
	{MediaPlayer::VO_MODE_NotSet, 94},
	{MediaPlayer::VO_MODE_NotSet, 95},
	{MediaPlayer::VO_MODE_NotSet, 96},
	{MediaPlayer::VO_MODE_NotSet, 97},
	{MediaPlayer::VO_MODE_NotSet, 98},
	{MediaPlayer::VO_MODE_NotSet, 99},
	#endif
};

int getSnmpResolutionValFromMpVoMode(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode)
{
	int iSnmpResVal = 0, iId;

	for(iId=0; iId<(typeof(iId))ARRAY_COUNT(g_MpVoMode_SnmpResVal_Table); iId++)
	{
		if(g_MpVoMode_SnmpResVal_Table[iId].eVoMode == eVoMode)
		{
			iSnmpResVal = g_MpVoMode_SnmpResVal_Table[iId].iSnmpResVal;
			break;
		}
	}

	return iSnmpResVal;
}

MediaPlayer::VIDEO_OUTPUT_MODE getMpVoMode_From_SnmpDispResVal(int iSnmpDispResVal, OUT BOOL_t & bFound)
{
	int iId;
	MediaPlayer::VIDEO_OUTPUT_MODE eOutVoMode = MediaPlayer::VO_MODE_NotSet;
//	LOG_LINE("iSnmpDispResVal(we set value):%d\n",iSnmpDispResVal);

	bFound = FALSE;

	for(iId=0; iId<(typeof(iId))ARRAY_COUNT(g_MpVoMode_SnmpResVal_Table); iId++)
	{
	//	LOG_LINE("===========iid %d\n",iId);
		if(g_MpVoMode_SnmpResVal_Table[iId].iSnmpResVal == iSnmpDispResVal)
		{
			eOutVoMode = g_MpVoMode_SnmpResVal_Table[iId].eVoMode;
			bFound = TRUE;
			break;
		}
	}

	return eOutVoMode;
}

MediaPlayer::DISPLAY_ASPECT_RATIO getDispAspectRatio_From_SnmpVideoMode(ControlCenter::CE_SNMP_VIDEO_MODE_T eCeSnmpVidMode)
{
	MediaPlayer::DISPLAY_ASPECT_RATIO eDispAspectRatio = MediaPlayer::DispAspectRatio_DEFAULT;

	if(ControlCenter::CE_SNMP_VidMode_Normal == eCeSnmpVidMode)
	{
		eDispAspectRatio = MediaPlayer::DispAspectRatio_DEFAULT;
	}
	else if(ControlCenter::CE_SNMP_VidMode_Wide == eCeSnmpVidMode)
	{
		eDispAspectRatio = MediaPlayer::DispAspectRatio_16_9;
	}
	else if(ControlCenter::CE_SNMP_VidMode_Anamorphic == eCeSnmpVidMode)
	{
		eDispAspectRatio = MediaPlayer::DispAspectRatio_4_3;
	}
	else if(ControlCenter::CE_SNMP_VidMode_FULL == eCeSnmpVidMode)
	{
		eDispAspectRatio = MediaPlayer::DispAspectRatio_FULL;
	}

	return eDispAspectRatio;
}
#if defined(__mips__)
enum OutputSpdif_type getMruaOutSpdifTypeFromSnmpMode(CONST Mp7xxCommon::SNMP_OUTPUT_SPDIF_MODE eSnmpOutSpdifMode)
{
	enum OutputSpdif_type eOutputSpdifType = (typeof(eOutputSpdifType))((AUDIO_CODEC_OUTPUTMODE_pcmToPP<<4) | OutputSpdif_Uncompressed);
	if(Mp7xxCommon::SnmpOutSpdifMode_Compressed == eSnmpOutSpdifMode)
	{
		eOutputSpdifType = (typeof(eOutputSpdifType))((AUDIO_CODEC_OUTPUTMODE_cmprsdToSPDIF_pcmToPP<<4) | OutputSpdif_Compressed);
	}
	else if(Mp7xxCommon::SnmpOutSpdifMode_Disabled == eSnmpOutSpdifMode)
	{
		eOutputSpdifType = (typeof(eOutputSpdifType))(OutputSpdif_Disable);
	}
		
	return eOutputSpdifType;
}

enum EMhwlibPictureOrientation getMruaVideoRotationTypeFromSnmpMode(CONST Mp7xxCommon::VIDEO_ROTATION_t eSnmpVideoRotationMode)
{
        enum EMhwlibPictureOrientation eVideoRotationype = (typeof(eVideoRotationype))(EMhwlibPictureOrientation_0);
        if(Mp7xxCommon::VIDEO_ROTATION_90==eSnmpVideoRotationMode)
        {
                eVideoRotationype=(typeof(eVideoRotationype))(EMhwlibPictureOrientation_90);
        }
        else if(Mp7xxCommon::VIDEO_ROTATION_180==eSnmpVideoRotationMode)
        {
                eVideoRotationype=(typeof(eVideoRotationype))(EMhwlibPictureOrientation_180);
        }
        else if(Mp7xxCommon::VIDEO_ROTATION_270==eSnmpVideoRotationMode)
        {
                eVideoRotationype=(typeof(eVideoRotationype))(EMhwlibPictureOrientation_270);
        }

        return eVideoRotationype;
    
}
#endif 

