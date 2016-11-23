#ifndef _DTV_TYPE_H__
#define _DTV_TYPE_H__

#include <BaseTypeDef.h>
#include "../MediaPlayer/libNexusExt/tspsimgr3.h"
#define MAX_ES_NUMBER 			5
#define MAX_EVENT_NUMBER 		16
#define SHORT_NAME_LENGTH		14
#define TITLE_LEN					64
#define MAX_LANGUAGE_NUMBER 	1
#define SHORT_DES_LEN			255
#define EXTEND_DES_LEN			255
#define MAX_COMPONENT_DES		5
#define MAX_EIT_NUMBER			64 //(2+(8*NUM_SEG))	//Eits(prensend+schedule) + Eits(schedule for 7 days)
#define MAX_CHANNEL_NUMBER		100
#define MAX_SERVICE_NUMBER		6
#define MAX_DIGITAL_PROGRAM	200

typedef struct {
	UINT8_t stream_type;
	UINT16_t elementary_pid;
	UINT8_t ISO639_language[3];	
} ESInfo;

struct ComponentDes {
	UINT8_t stream_content;
	UINT8_t component_type;
	UINT8_t ISO_639_language_code[3];
};

//EVENT INFOMATION TABLE CONTEXT
typedef struct {
	UINT16_t	event_id;	
	UINT8_t	start_time[5];
	UINT8_t	duration[3];
	UINT8_t	free_CA_mode;
	UINT8_t	running_status;
	UINT8_t	uiParentalRating;
	UINT8_t	szTitle[TITLE_LEN];
	UINT8_t	ISO_639_language_code_Title[3];
	UINT8_t	szTitle_ex[MAX_LANGUAGE_NUMBER][TITLE_LEN];
	UINT8_t	ISO_639_language_code_Title_ex[MAX_LANGUAGE_NUMBER+1][3];
	UINT8_t	szShoText[SHORT_DES_LEN];	
	UINT8_t	uiShoTextLen;
	UINT8_t	szExtText[EXTEND_DES_LEN];
	UINT8_t	ISO_639_language_code_ExtText[3];
	UINT8_t	uiExtTextLen;
	UINT8_t	szExtText_ex[MAX_LANGUAGE_NUMBER][EXTEND_DES_LEN];
	UINT8_t	ISO_639_language_code_ExtText_ex[MAX_LANGUAGE_NUMBER+1][3];
	//component descriptor
	struct ComponentDes astComponentDes[MAX_COMPONENT_DES];
	UINT8_t	uiNumComponentDes;
} EventInfo;

//Event Infomation Table
typedef struct{
	EventInfo* pEvent;
	UINT8_t	numEvent;
	UINT8_t	table_id;
	UINT8_t	version_number;
	UINT8_t	section_number;
	UINT8_t	last_section_number;
	UINT8_t	segment_last_section_number;
} EITInfo;

typedef struct {
//Video
	ESInfo 	rgVidEs[MAX_ES_NUMBER];
	UINT16_t	numVidEs;
//Audio
	ESInfo 	rgAudEs[MAX_ES_NUMBER];
	UINT16_t 	numAudEs;
//Teletext
	ESInfo 	rgTeletextEs[MAX_ES_NUMBER];
	UINT16_t 	numTeletextEs;
//Subtitle
	ESInfo 	rgSubtitleEs[MAX_ES_NUMBER];
	UINT16_t 	numSubtitleEs;
//Vct Info
	UINT8_t	short_name[SHORT_NAME_LENGTH + 1];
	UINT16_t major_number;
	UINT16_t minor_number;
	UINT8_t   free_CA_mode;
	UINT16_t	program_number;
	
	UINT16_t	audioIdx;
	UINT16_t	videoIdx;
	UINT16_t teletextIdx;
	UINT16_t subtitleIdx;
	
	UINT16_t 	pcr_pid;	
	UINT8_t	pmt_index;
	UINT16_t	program_map_PID;
	UINT8_t 	skip;
	UINT8_t 	lock;

	UINT32_t programIndex;
} ProgramInfo,*pProgramInfo;

typedef enum{
	STANDARD_DVBT    = 0,//Terrestrial DVB-T
        STANDARD_8VSB,		//errestrial 8VSB
   	STANDARD_NTSC,  	// Terrestrial\Cable analog NTSC.
 	STANDARD_PAL_SECAM_BG, //Terrestrial analog PAL/SECAM B/G 
        STANDARD_PAL_SECAM_DK,          /**< Terrestrial analog PAL/SECAM D/K */
  	STANDARD_PAL_SECAM_I,           /**< Terrestrial analog PAL/SECAM I   */
   	STANDARD_PAL_SECAM_L,           /**< Terrestrial analog PAL/SECAM L
                                             with negative modulation        */
   	STANDARD_PAL_SECAM_LP,          /**< Terrestrial analog PAL/SECAM L
                                             with positive modulation        */
   	STANDARD_ITU_A,                 /**< Cable ITU ANNEX A.               */
   	STANDARD_ITU_B,                 /**< Cable ITU ANNEX B.               */
   	STANDARD_ITU_C,                 /**< Cable ITU ANNEX C.               */
   	STANDARD_ITU_D,                 /**< Cable ITU ANNEX D.               */
   	STANDARD_FM,                    /**< Terrestrial\Cable FM radio       */
  	STANDARD_DTMB,                  /**< Terrestrial DTMB standard (China)*/
  	STANDARD_UNKNOW = 254,
  	STANDARD_AUTO,
}StandardChannle,*pStandardChannel;

typedef enum{
	BANDWIDTH_8M = 0,
	BANDWIDTH_7M,
	BANDWIDTH_6M,
	BANDWIDTH_UNKNOW = 254,
	BANDWIDTH_AUTO,
}BandwidthChannel,*pBandwidthChannel;

typedef struct{
	INT32_t freq;
	UINT16_t strength;
	UINT16_t 	numProgram;
	BandwidthChannel chBandwidth;
	StandardChannle chStandard;
	UINT32_t	chConstellation;
	tspsimgr_scan_results pRgProgram;
}ChannelProgramInfo,*pChannelProgramInfo;


typedef struct	
{
	ESInfo 	rgVidEs[MAX_ES_NUMBER];
	UINT16_t 	numVidEs;
	ESInfo 	rgAudEs[MAX_ES_NUMBER];
	UINT16_t 	numAudEs;	
	ESInfo 	rgTeletextEs[MAX_ES_NUMBER];
	UINT16_t 	numTeletextEs;
	ESInfo 	rgSubtitleEs[MAX_ES_NUMBER];
	UINT16_t 	numSubtitleEs;
	
	
	UINT8_t	ext_name[MAX_SERVICE_NUMBER][SHORT_NAME_LENGTH];
	UINT8_t	ISO_639_language_code[MAX_SERVICE_NUMBER][3];
	
	UINT16_t	audioIdx;
	UINT16_t	videoIdx;
	UINT16_t teletextIdx;
	UINT16_t subtitleIdx;

	//Vct Info
	UINT8_t	acName[SHORT_NAME_LENGTH + 1];//½ÚÄ¿Ãû³Æ
	UINT16_t major_number;
	UINT16_t minor_number;
	UINT16_t	program_number;
	UINT8_t	free_CA_mode;
	
	UINT16_t 	pcr_pid;
	UINT16_t	numProgram;
	UINT16_t	program_map_PID;
	UINT8_t	pmt_index;
	INT32_t	uiFreq;
	UINT8_t	uiLock;
	UINT8_t	uiSkip;

	UINT32 uiProgramIndex;
} DigitalChannel;

typedef struct{
	UINT32_t numDChannel;
	BandwidthChannel uiBandwidth;
	StandardChannle uiStandard;
	UINT32_t	uiConstellation; // 1:QAM64 2:QAM256
	//STORE PROGRAM LIST
	//DigitalChannel	astDChannel[MAX_DIGITAL_PROGRAM];
	tspsimgr_program astDChannel[TSPSIMGR_MAX_PROGRAMS];
	UINT32_t crc;
}DtvChannelCfg;

typedef enum {
	TYPEOFDVB_VCT_UNKNOW = 0,
	TYPEOFDVB_VCT_TERRESTRIAL,
	TYPEOFDVB_VCT_CABLE,
}DvbVctType,*pDvbVctType;

typedef struct{
	INT32_t startFreqKhz;
	INT32_t endFreqKhz;
	UINT32_t	stepKhz;
	UINT32_t	chNum;
}ScanFrequencyPlan_t,*pScanFrequencyPlan_t;

typedef struct{
	UINT8_t zoneNum;
	UINT8_t zoneIdx;
	UINT32_t freqProgress; 
	UINT32_t freqIdx;
	UINT32_t totalFreqNum;
	pScanFrequencyPlan_t pScanFrequencyPlan;
}ScanFreqParam_t,*pScanFreqParam_t;

#define	CHANNEL_MODE_FIXED_ATSC 	1
#define	CHANNEL_MODE_FIXED_QAM64	2
#define	CHANNEL_MODE_FIXED_QAM256 	3
#define	CHANNEL_MODE_FIXED_SCANNED 	4
#define	CHANNEL_MODE_DEFAULT		CHANNEL_MODE_FIXED_SCANNED

//TUNER ERR
#define	ERR_TU_ERROR				800
#define	ERR_NOT_DETECTED			801
#define	ERR_TUNER_OPEN_FAIL		802
#define	ERR_TU_MPEG_CFG_FAIL		803
#define	ERR_TU_UIO_CFG_FAIL		804
#define	ERR_TU_GET_VESION_FAIL		805
#define	ERR_TU_UIO_CTRL_WRITE_FAIL	806
#define	ERR_TU_UIO_CTRL_READ_FAIL		807
#define	ERR_TU_I2C_NOT_DEDTROY	808
#define	ERR_TU_SET_STANDARD_FAIL	809
#define	ERR_TU_SET_CHANNEL_FAIL	810
#define	ERR_TU_GET_LOCK_FAIL		811
#define	ERR_TU_INIT_SCANT_FAIL		812
#define	ERR_TU_SCANT_NEVER		813
#define	ERR_TU_GET_STRENGTH_FAIL	814
#define	ERR_TU_USER_STOP_SCAN		815
#define	ERR_TU_SCAN_DONE			816
#define	ERR_TU_GET_STANDARD_FAIL	817
#define	ERR_TU_DEVICE_BUSY			818
#define	ERR_TU_SCAN_ERROR			819
#define	ERR_TU_SCAN_FAIL			820
#define	ERR_TU_SETPMT_FAIL			821
#define	ERR_TU_GET_CHANNELINFO_FAIL 822
#define	ERR_TU_STANDARD_DISAGREEMENT	823
#define	ERR_TU_INITDEMOD_ERROR	824
#define	ERR_TU_RESET_FAIL			825
#define 	ERR_TU_FREQ_OUT_OF_ZONE_RANG 826
#define	ERR_TU_LAST_FREQ			827

#endif	//DTV_TYPE_H__

