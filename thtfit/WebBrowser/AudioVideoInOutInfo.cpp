#include "AudioVideoInOutInfo.h"
#include <ErrPrintHelper.h>

namespace MP7XX
{

VOID QAudioVideoInOutInfo::getVoDesc(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode, QString & strVoDesc)
{
	strVoDesc.clear();
	//LOG_LINE("getVoDesc(),eVoMode=%d\n",eVoMode);

	switch(eVoMode)
	{
	case MediaPlayer::VO_MODE_640x480p59:
		strVoDesc = "HDMI_640x480p59";
		break;
	case MediaPlayer::VO_MODE_640x480p60:
		strVoDesc = "HDMI_640x480p60";
		break;
	case MediaPlayer::VO_MODE_HDMI_480i60:
		strVoDesc = "HDMI_480i60";
		break;
	case MediaPlayer::VO_MODE_HDMI_480p60:
		strVoDesc = "HDMI_480p60";
		break;
	case MediaPlayer::VO_MODE_HDMI_576p50:
		strVoDesc = "HDMI_576p50";
		break;
	case MediaPlayer::VO_MODE_HDMI_CAV_720p60:
		strVoDesc = "H_CAV_720P60";
		break;
	case MediaPlayer::VO_MODE_HDMI_CAV_1080p60:
		strVoDesc = "H_CAV_1080P60";
		break;
	case MediaPlayer::VO_MODE_HDMI_1080i59:
		strVoDesc = "HDMI_1080i59";
		break;
	case MediaPlayer::VO_MODE_HDMI_CAV_1080i60:
		strVoDesc = "HDMI_CAV_1080i60";
		break;
	case MediaPlayer::VO_MODE_HDMI_720p50:
		strVoDesc = "HDMI_720p50";
		break;
	case MediaPlayer::VO_MODE_HDMI_1080i50:
		strVoDesc = "HDMI_1080i50";
		break;
	case MediaPlayer::VO_MODE_HDMI_1080p50:
		strVoDesc = "HDMI_1080p50";
		break;	
	case MediaPlayer::VO_MODE_HDMI_1080p59:
		strVoDesc = "HDMI_1080p59";
		break;			
	case MediaPlayer::VO_MODE_VESA_800x600x60:
		strVoDesc = "VGA 800x600@60";	
		break;
	case MediaPlayer::VO_MODE_VESA_1024x768x60:
		strVoDesc = "VGA_1024x768x60";	
		break;
	case MediaPlayer::VO_MODE_VESA_1366x768x60:
		strVoDesc = "VGA_1366x768@60";
		break;
	case MediaPlayer::VO_MODE_VESA_1600x1200x60:
		strVoDesc = "1600x1200x60";
		break;
	case MediaPlayer::VO_MODE_CVBS:
		strVoDesc = "CVBS_NTSC";
		break;
	case MediaPlayer::VO_MODE_RGB_1080i50:
		strVoDesc = "RGB_1080i50";
		break;
	case MediaPlayer::VO_MODE_SPLIT_HdmiVcr_VgaMain:
		strVoDesc = "H720p60_V1024x768@60";
		break;
	case MediaPlayer::VO_MODE_SPLIT_HdmiVcr_1080p60_VgaMain:
		strVoDesc = "H1080p60_V1024x768@60";
		break;
	case MediaPlayer::VO_MODE_HDMI_2160p24:
		//strVoDesc = "HDMI_3840x2160P24";
        strVoDesc = "HDMI_2160P24";
		break;
	case MediaPlayer::VO_MODE_HDMI_2160p25:
		//strVoDesc = "HDMI_3840x2160P25";
		strVoDesc = "HDMI_2160P25";
		break;
	case MediaPlayer::VO_MODE_HDMI_2160p30:
		//strVoDesc = "HDMI_3840x2160P30";
		strVoDesc = "HDMI_2160P30";
		break;
	case MediaPlayer::VO_MODE_HDMI_2160p50:
		//strVoDesc = "HDMI_3840x2160P50";
		strVoDesc = "HDMI_2160P50";
		break;
	case MediaPlayer::VO_MODE_HDMI_2160p60:
		//strVoDesc = "HDMI_3840x2160P60";
		strVoDesc = "HDMI_2160P60";
		break;
	case MediaPlayer::VO_MODE_HDMI_4K2Kp24:
		strVoDesc = "HDMI_4096x2160P24";
		break;
	case MediaPlayer::VO_MODE_HDMI_4K2Kp25:
		strVoDesc = "HDMI_4096x2160P25";
		break;
	case MediaPlayer::VO_MODE_HDMI_4K2Kp30:
		strVoDesc = "HDMI_4096x2160P30";
		break;
	case MediaPlayer::VO_MODE_HDMI_4K2Kp50:
		strVoDesc = "HDMI_4096x2160P50";
		break;
	case MediaPlayer::VO_MODE_HDMI_4K2Kp60:
		strVoDesc = "HDMI_4096x2160P60";
		break;
	case MediaPlayer::VO_MODE_HDMI_720p59:
		strVoDesc = "HDMI_720p59";
		break;
	default:
		strVoDesc.clear();
		break;
	}	
}

VOID QAudioVideoInOutInfo::getVoDescForLedDisp(CONST MediaPlayer::VIDEO_OUTPUT_MODE eVoMode, QString & strVoDesc)
{
	strVoDesc.clear();
	//LOG_LINE("getVoDescForLedDisp(),eVoMode=%d\n",eVoMode);

	switch(eVoMode)
	{
		case MediaPlayer::VO_MODE_640x480p59:
			strVoDesc = "HDMI640x480p59";
			break;
		case MediaPlayer::VO_MODE_640x480p60:
			strVoDesc = "HDMI640x480p60";
			break;
		case MediaPlayer::VO_MODE_HDMI_480i60:
			strVoDesc = "HDMI_480i60";
			break;
		case MediaPlayer::VO_MODE_HDMI_480p60:
			strVoDesc = "HDMI_480p60";
			break;
		case MediaPlayer::VO_MODE_HDMI_576p50:
			strVoDesc = "HDMI_576p50";
			break;
		case MediaPlayer::VO_MODE_HDMI_CAV_720p60:
			strVoDesc = "H_CAV_720P60";
			break;
		case MediaPlayer::VO_MODE_HDMI_CAV_1080p60:
			strVoDesc = "H_CAV_1080P60";
			break;
		case MediaPlayer::VO_MODE_RGB_1080i50:
			strVoDesc = "RGB_1080i50";
			break;
		case MediaPlayer::VO_MODE_HDMI_1080i59:
			strVoDesc = "HDMI_1080i59";
			break;
		case MediaPlayer::VO_MODE_HDMI_CAV_1080i60:
			strVoDesc = "H_CAV_1080i60";
			break;
		case MediaPlayer::VO_MODE_HDMI_720p50:
			strVoDesc = "HDMI_720p50";
			break;
		case MediaPlayer::VO_MODE_HDMI_1080i50:
			strVoDesc = "HDMI_1080i50";
			break;
		case MediaPlayer::VO_MODE_HDMI_1080p50:
			strVoDesc = "HDMI_1080p50";
			break;	
		case MediaPlayer::VO_MODE_HDMI_1080p59:
			strVoDesc = "HDMI_1080P59";
			break;			
		case MediaPlayer::VO_MODE_VESA_800x600x60:
			strVoDesc = "VGA 800x600@60";	
		    	break;
		case MediaPlayer::VO_MODE_VESA_1024x768x60:
			strVoDesc = "V_1024x768x60";	
		   	 break;
		case MediaPlayer::VO_MODE_VESA_1366x768x60:
			strVoDesc = "V_1366x768x60";
		   	 break;
		case MediaPlayer::VO_MODE_VESA_1600x1200x60:
			strVoDesc = "1600x1200x60";
		   	 break;
		case MediaPlayer::VO_MODE_CVBS:
			strVoDesc = "CVBS_NTSC";
			break;
		case MediaPlayer::VO_MODE_SPLIT_HdmiVcr_VgaMain:
			strVoDesc = "H720p60_VGA";
			break;
		case MediaPlayer::VO_MODE_SPLIT_HdmiVcr_1080p60_VgaMain:
			strVoDesc = "H1080p60_VGA";
			break;
		case MediaPlayer::VO_MODE_HDMI_2160p24:
		//	strVoDesc = "HDMI_3840x2160P24";
			strVoDesc = "HDMI_2160P24";
			break;
		case MediaPlayer::VO_MODE_HDMI_2160p25:
		//	strVoDesc = "HDMI_3840x2160P25";
			strVoDesc = "HDMI_2160P25";
			break;
		case MediaPlayer::VO_MODE_HDMI_2160p30:
		//	strVoDesc = "HDMI_3840x2160P30";
			strVoDesc = "HDMI_2160P30";
			break;
		case MediaPlayer::VO_MODE_HDMI_2160p50:
		//	strVoDesc = "HDMI_3840x2160P50";
			strVoDesc = "HDMI_2160P50";
			break;
		case MediaPlayer::VO_MODE_HDMI_2160p60:
		//	strVoDesc = "HDMI_3840x2160P60";
			strVoDesc = "HDMI_2160P60";
			break;
		case MediaPlayer::VO_MODE_HDMI_4K2Kp24:
			strVoDesc = "HDMI_4096x2160P24";
			break;
		case MediaPlayer::VO_MODE_HDMI_4K2Kp25:
			strVoDesc = "HDMI_4096x2160P25";
			break;
		case MediaPlayer::VO_MODE_HDMI_4K2Kp30:
			strVoDesc = "HDMI_4096x2160P30";
			break;
		case MediaPlayer::VO_MODE_HDMI_4K2Kp50:
			strVoDesc = "HDMI_4096x2160P50";
			break;
		case MediaPlayer::VO_MODE_HDMI_4K2Kp60:
			strVoDesc = "HDMI_4096x2160P60";
			break;
		case MediaPlayer::VO_MODE_HDMI_720p59:
			strVoDesc = "HDMI_720p59";
			break;
		default:
			strVoDesc.clear();
			break;
	}	
}

}

