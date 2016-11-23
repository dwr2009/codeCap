#include "display.h"
#include <ArrayOpsMacro.h>
#include <ErrPrintHelper.h>

static NEXUS_VideoFormat g_sLastSetVideoFormat=NEXUS_VideoFormat_eUnknown;
typedef struct VideoFormat {
	VIDEO_OUTPUT_MODE mode;
	NEXUS_VideoFormat format;
}VideoFormat_t;

VideoFormat_t video_format_pool[]={
	{VO_MODE_NotSet,								NEXUS_VideoFormat_eUnknown},
	{VO_MODE_HDMI_720p50,							NEXUS_VideoFormat_e720p50hz},
	{VO_MODE_HDMI_1080i50,							NEXUS_VideoFormat_e1080i50hz},
	{VO_MODE_HDMI_1080p50,							NEXUS_VideoFormat_e1080p50hz},
	{VO_MODE_HDMI_480p60,							NEXUS_VideoFormat_e480p},
	{VO_MODE_HDMI_480i60,							NEXUS_VideoFormat_eNtsc},
	{VO_MODE_HDMI_576p50,							NEXUS_VideoFormat_e576p},
	{VO_MODE_640x480p59,							NEXUS_VideoFormat_eVesa640x480p60hz},	/*not match*/
	{VO_MODE_640x480p60,							NEXUS_VideoFormat_eVesa640x480p60hz},
	{VO_MODE_HDMI_480p59,							NEXUS_VideoFormat_e480p},								/*not match   10*/
	{VO_MODE_HDMI_720p59,							NEXUS_VideoFormat_e720p},								/*not match*/
	{VO_MODE_HDMI_1080i59,							NEXUS_VideoFormat_e1080i},								/*not match*/
	{VO_MODE_2880x240,								NEXUS_VideoFormat_eCustomer1440x240p60hz},
	{VO_MODE_1440x480p59,							NEXUS_VideoFormat_e1440x480p60hz},			/*not match*/
	{VO_MODE_1440x480p60,							NEXUS_VideoFormat_e1440x480p60hz},
	{VO_MODE_HDMI_1080p59,							NEXUS_VideoFormat_e1080p},							/*not match*/
	{VO_MODE_720x288p,								NEXUS_VideoFormat_e288p50hz},
	{VO_MODE_2880x576i50,							NEXUS_VideoFormat_e1440x576p50hz},
	{VO_MODE_HDMI_2880x288p50,						NEXUS_VideoFormat_eCustomer1440x288p50hz},
	{VO_MODE_1440x576i50,							NEXUS_VideoFormat_e1440x576p50hz},			/*not match    20*/
	{VO_MODE_HDMI_1080p23,							NEXUS_VideoFormat_e1080p24hz},					/*not match*/
	{VO_MODE_1080p24,								NEXUS_VideoFormat_e1080p24hz},
	{VO_MODE_1080p25,								NEXUS_VideoFormat_e1080p25hz},
	{VO_MODE_1080p29,								NEXUS_VideoFormat_e1080p30hz},
	{VO_MODE_1080p30,								NEXUS_VideoFormat_e1080p30hz},
	{VO_MODE_2880x480p59,							NEXUS_VideoFormat_e1440x480p60hz},
	{VO_MODE_2880x480p,								NEXUS_VideoFormat_e1440x480p60hz},
	{VO_MODE_2880x576p50,							NEXUS_VideoFormat_e1440x576p50hz},
	{VO_MODE_1080i50_1250,							NEXUS_VideoFormat_e1080i50hz},
	{VO_MODE_1080i100,								NEXUS_VideoFormat_e1080i},
	{VO_MODE_720p100,								NEXUS_VideoFormat_e720p},
	{VO_MODE_576p100,								NEXUS_VideoFormat_e576p},
	{VO_MODE_576i100,								NEXUS_VideoFormat_e576p},
	{VO_MODE_1080i119,								NEXUS_VideoFormat_e1080i},
	{VO_MODE_1080i120,								NEXUS_VideoFormat_e1080i},
	{VO_MODE_HDMI_CAV_1080p60,						NEXUS_VideoFormat_e1080p60hz},
	{VO_MODE_HDMI_CAV_1080i60,						NEXUS_VideoFormat_e1080i},
	{VO_MODE_HDMI_CAV_720p60,						NEXUS_VideoFormat_e720p},
	{VO_MODE_COMPONENT_480p60,						NEXUS_VideoFormat_e480p},
	{VO_MODE_COMPONENT_1080p50,						NEXUS_VideoFormat_e1080p50hz},
	{VO_MODE_COMPONENT_1080p60,						NEXUS_VideoFormat_e1080p60hz},
	{VO_MODE_COMPONENT_1080i60,						NEXUS_VideoFormat_e1080i},
	{VO_MODE_COMPONENT_720p50,						NEXUS_VideoFormat_e720p50hz},
	{VO_MODE_COMPONENT_720p60,						NEXUS_VideoFormat_e720p},
	{VO_MODE_COMPONENT_1080i50,						NEXUS_VideoFormat_e1080i50hz},
	{VO_MODE_COMPONENT_576p50,						NEXUS_VideoFormat_e576p},
	{VO_MODE_VESA_1280x1024x60,						NEXUS_VideoFormat_eVesa1280x1024p60hz},
	{VO_MODE_VESA_720x400x70,						NEXUS_VideoFormat_eVesa720x400p70hz},
	{VO_MODE_VESA_800x600x60,						NEXUS_VideoFormat_eVesa800x600p60hz},
	{VO_MODE_VESA_1024x768x60,						NEXUS_VideoFormat_eVesa1024x768p60hz},   /* 50 */
	{VO_MODE_VESA_1280x768x60,						NEXUS_VideoFormat_eVesa1280x768p60hz},
	{VO_MODE_VESA_1360x768x60,	 					NEXUS_VideoFormat_eVesa1360x768p60hz},
	{VO_MODE_VESA_1366x768x60,						NEXUS_VideoFormat_eVesa1366x768p60hz},
	{VO_MODE_VESA_1600x1200x60,						NEXUS_VideoFormat_eVesa1600x1200p60hz},
	{VO_MODE_RGB_1080i50,							NEXUS_VideoFormat_e1080i50hz},						/*not match*/
	{VO_MODE_CVBS,									NEXUS_VideoFormat_eNtsc},

	/*4K*/
	{VO_MODE_HDMI_2160p24,							NEXUS_VideoFormat_e3840x2160p24hz},
	{VO_MODE_HDMI_2160p25,							NEXUS_VideoFormat_e3840x2160p25hz},			
	{VO_MODE_HDMI_2160p30,							NEXUS_VideoFormat_e3840x2160p30hz},
	{VO_MODE_HDMI_2160p50,							NEXUS_VideoFormat_e3840x2160p50hz},
	{VO_MODE_HDMI_2160p60,							NEXUS_VideoFormat_e3840x2160p60hz},
	{VO_MODE_HDMI_4K2Kp24,							NEXUS_VideoFormat_e4096x2160p24hz},
	{VO_MODE_HDMI_4K2Kp25,							NEXUS_VideoFormat_e4096x2160p25hz},
	{VO_MODE_HDMI_4K2Kp30,							NEXUS_VideoFormat_e4096x2160p30hz},
	{VO_MODE_HDMI_4K2Kp50,							NEXUS_VideoFormat_e4096x2160p50hz},
	{VO_MODE_HDMI_4K2Kp60,							NEXUS_VideoFormat_e4096x2160p60hz},
	{VO_MODE_SPLIT_HdmiVcr_VgaMain,					NEXUS_VideoFormat_eUnknown},							/*unknown*/
	{VO_MODE_SPLIT_HdmiVcr_1080p60_VgaMain,	 		NEXUS_VideoFormat_eUnknown},							/*unknown*/
	{VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr,	 		NEXUS_VideoFormat_eUnknown},							/*unknown*/
};

//Nexus_Format => VIDEO_OUTPUT_MODE
int VideoFormatConvert(VIDEO_OUTPUT_MODE *mode, NEXUS_VideoFormat *format){
	int i, iOutRet = ERR_SUCCESS;
	int count = sizeof(video_format_pool)/sizeof(VideoFormat_t);

	if(mode == NULL || format == NULL){
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}
	
	if(*mode!=VO_MODE_NotSet){
		for(i=0;i<count;i++){
			if(video_format_pool[i].mode==*mode){
				*format=video_format_pool[i].format;
				if(*format == NEXUS_VideoFormat_eUnknown){
					iOutRet = ERROR_NOT_SUPPORTED;
				}
				break;
			}
		}
	} else {	//VO_MODE_NotSet
		if(*format==NEXUS_VideoFormat_eUnknown)
		{
			iOutRet = ERROR_INVALID_STATE;
			goto EXIT_PROC;
		}

		for(i=0;i<count;i++){
			if(video_format_pool[i].format == *format){
				*mode=video_format_pool[i].mode;
				break;
			}
		}
	}

EXIT_PROC:

	return iOutRet;
}

INT_t NexusVidFmt_to_VidOutMode(CONST NEXUS_VideoFormat nexusVidFmt, VIDEO_OUTPUT_MODE * pVidOutMode)
{
	INT_t iOutRet = ERR_NOT_FOUND;
	int iId;

	for(iId=0; iId<ARRAY_COUNT(video_format_pool); iId++)
	{
		if(nexusVidFmt == video_format_pool[iId].format)
		{
			if(pVidOutMode)
			{
				*pVidOutMode = video_format_pool[iId].mode;
			}
			iOutRet = ERR_SUCCESS;
			break;
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t VidOutMode_to_NexusVidFmt(CONST VIDEO_OUTPUT_MODE eVidOutMode, NEXUS_VideoFormat * pNexusVidFmt)
{
	INT_t iOutRet = ERR_NOT_FOUND;
	int iId;

	for(iId=0; iId<ARRAY_COUNT(video_format_pool); iId++)
	{
		if(eVidOutMode == video_format_pool[iId].mode)
		{
			if(pNexusVidFmt)
			{
				*pNexusVidFmt = video_format_pool[iId].format;
			}
			iOutRet = ERR_SUCCESS;
			break;
		}
	}

EXIT_PROC:
	return iOutRet;
}

int ChangeVidOutputMode(NEXUS_VideoFormat newFormat){
    int iOutRet = ERR_SUCCESS, iRet;
	NEXUS_Error nexusErr;
    NxClient_DisplaySettings nxDisplaySettings;
    
    NxClient_GetDisplaySettings( &nxDisplaySettings );

	if(newFormat <= NEXUS_VideoFormat_eUnknown || newFormat >= NEXUS_VideoFormat_eMax)
	{
        iOutRet = ERR_INV_PARAM;
        goto EXIT_PROC;
    }
    
    if(newFormat==g_sLastSetVideoFormat)
    {
        LOG_BLINE("Last VideoFormat is %d not change\n", newFormat);
        goto EXIT_PROC;
    }
    g_sLastSetVideoFormat=newFormat;
    /*
    if(nxDisplaySettings.format == newFormat)
    {
        goto EXIT_PROC;
    }
    */
    

    nxDisplaySettings.format = newFormat;
	nxDisplaySettings.hdmiPreferences.preventUnsupportedFormat = false;
	//LOG_BLINE("Fmt=%d\n", nxDisplaySettings.format);
    nexusErr = NxClient_SetDisplaySettings( &nxDisplaySettings );
    if(NEXUS_SUCCESS != nexusErr){
        iOutRet = ERR_Nexus_SetDispSetting_Err;
        goto EXIT_PROC;
    }

EXIT_PROC:

	return iOutRet;
}

int ChangeVidColorSpace(NEXUS_ColorSpace newcolspace)
{
    int iOutRet = ERR_SUCCESS, iRet;
	NEXUS_Error nexusErr;
    NxClient_DisplaySettings nxDisplaySettings;
    NxClient_GetDisplaySettings( &nxDisplaySettings );
    if(nxDisplaySettings.hdmiPreferences.colorSpace == newcolspace){
        goto EXIT_PROC;
    }

    nxDisplaySettings.hdmiPreferences.colorSpace = newcolspace;
	//nxDisplaySettings.hdmiPreferences.preventUnsupportedFormat = false;
    nexusErr = NxClient_SetDisplaySettings( &nxDisplaySettings );
    if(NEXUS_SUCCESS != nexusErr){
        iOutRet = ERR_Nexus_SetDispSetting_Err;
        goto EXIT_PROC;
    }

EXIT_PROC:

	return iOutRet;
}

int ChangeVidColorDepth(unsigned colorDepth)
{
    int iOutRet = ERR_SUCCESS, iRet;
	NEXUS_Error nexusErr;
    NxClient_DisplaySettings nxDisplaySettings;
    NxClient_GetDisplaySettings( &nxDisplaySettings );
    if(nxDisplaySettings.hdmiPreferences.colorDepth == colorDepth){
        goto EXIT_PROC;
    }

    nxDisplaySettings.hdmiPreferences.colorDepth = colorDepth;
	//nxDisplaySettings.hdmiPreferences.preventUnsupportedFormat = false;
    nexusErr = NxClient_SetDisplaySettings( &nxDisplaySettings );
    if(NEXUS_SUCCESS != nexusErr){
        iOutRet = ERR_Nexus_SetDispSetting_Err;
        goto EXIT_PROC;
    }

EXIT_PROC:

	return iOutRet;
}



NEXUS_VideoFormat getCurNexusDispVidFmt(){
	NxClient_DisplaySettings nxDisplaySettings;
	NxClient_GetDisplaySettings( &nxDisplaySettings );
	return nxDisplaySettings.format;
}

