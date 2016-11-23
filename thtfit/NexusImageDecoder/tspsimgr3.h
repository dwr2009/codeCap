/******************************************************************************
 *    (c)2013 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF
 * USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF
 * ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: $
 * $brcm_Revision: $
 * $brcm_Date: $
 *
 *****************************************************************************/
#ifndef TSPSIMGR3_H__
#define TSPSIMGR3_H__

#include "nexus_types.h"
#include "psip_vct.h"

/**
Asyncrhonous MPEG2TS PSI scanner
**/

#ifdef __cplusplus
extern "C" {
#endif

/**
NOTE: This API is only example code. It is subject to change and 
is not supported as a standard reference software deliverable.
**/

#define TSPSIMGR_MAX_PIDS     	12
#define TSPSIMGR_MAX_PROGRAMS 	64
#define SHORT_NAME_LENGTH 		14

typedef enum TsScanType
{
	TS_PAT,
	TS_VCT
}TsScanType;

typedef struct tspsimgr_program
{    
    uint8_t     num_video_pids;
    struct {
        uint16_t pid;
        NEXUS_VideoCodec codec;
        uint16_t ca_pid;
    } video_pids[TSPSIMGR_MAX_PIDS];
    
    uint8_t     num_audio_pids;
    struct {
        uint16_t pid;
        NEXUS_AudioCodec codec;
        uint16_t ca_pid;
    } audio_pids[TSPSIMGR_MAX_PIDS];
    
    uint8_t     num_other_pids;
    struct {
        uint16_t pid;
        unsigned stream_type;
        uint16_t ca_pid;
    } other_pids[TSPSIMGR_MAX_PIDS];

	/*Add by xuweiwei 2015-1-5 for VCT INFO*/
	uint8_t	 short_name[SHORT_NAME_LENGTH + 1];
	uint16_t major_number;
	uint16_t minor_number;
	uint16_t program_number;
	uint8_t  free_CA_mode;

	uint16_t    pcr_pid;
	uint16_t 	numProgram;
	uint16_t    map_pid;

	uint32_t 	freq;
	uint8_t     version;
    uint16_t    ca_pid;
	uint32_t programIndex;
} tspsimgr_program;

typedef struct tspsimgr_scan_results
{
    uint8_t     version;
    uint16_t    transport_stream_id;
    uint16_t    num_programs;
    tspsimgr_program program_info[TSPSIMGR_MAX_PROGRAMS];
} tspsimgr_scan_results;

#define _MAX_SUB_VIRTUAL_CHANNEL_NUM 30

typedef struct DCCVCTInfoType{
	uint16_t section_length;
	uint16_t transport_stream_id;
	uint8_t version_number;          
	uint8_t current_next_indicator;
	uint8_t num_channels_in_section;
	PSIP_VCT_channel serivce_item[_MAX_SUB_VIRTUAL_CHANNEL_NUM];
}DCCVCTInfoType;


typedef struct TsScan
{
	TsScanType tstype;
	NEXUS_ParserBand parserBand;
	tspsimgr_scan_results pRgProgram;
	DCCVCTInfoType TmpVctInfo;
	uint16_t pid;
}TsScan,*pTsScan;


typedef struct tspsimgr *tspsimgr_t;


/**
Summary:
**/
tspsimgr_t tspsimgr_create(void);

/**
Summary:
**/
void tspsimgr_destroy(
    tspsimgr_t handle
    );

/**
Summary:
**/
typedef struct tspsimgr_scan_settings
{
    NEXUS_ParserBand parserBand;
    
    /* Callbacks */
    void (*scan_done)(void *context); /* Called when all PMT's are retrieved.
                                       Will be called from another thread. Inside the callback you 
                                       may call get_scan_results, but not stop_scan or destroy. */
    void *context;                    /* User context pass to callback */

    bool exclude_no_video_audio;      /* if no video or audio, exclude */
    bool exclude_ca_pid;              /* if ca_pid, exclude */
} tspsimgr_scan_settings;

/**
Summary:
**/
void tspsimgr_get_default_start_scan_settings(
    tspsimgr_scan_settings *psettings
    );
    
/**
Summary:
**/
int tspsimgr_start_scan(
    tspsimgr_t handle,
    const tspsimgr_scan_settings *psettings
    );

    
/**
Summary:
**/
void tspsimgr_stop_scan(
    tspsimgr_t handle
    );
    
/**
Summary:
Get PSI scan results.
This can be called before scan is complete.
If not all PMT's are retrieved, application must perform its own timeout
and call this function to get partial results.
Returns non-zero if scan is not done.
**/
int tspsimgr_get_scan_results(
    tspsimgr_t handle,
    tspsimgr_scan_results *presults
    );

int tspsimgr_get_vct_results(
	tspsimgr_t handle, 
	DCCVCTInfoType *presults 
	);


int tspsimgr_scan_program(
	pTsScan tsinfo
	);

    
#ifdef __cplusplus
}
#endif

#endif /* TSPSIMGR3_H__ */
