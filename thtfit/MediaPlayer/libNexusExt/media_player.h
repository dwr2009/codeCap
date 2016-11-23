/******************************************************************************
 *    (c)2010-2013 Broadcom Corporation
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
#ifndef MEDIA_PLAYER_H__
#define MEDIA_PLAYER_H__

#include "nexus_types.h"
#include "nexus_playback.h"
#include "dvr_crypto.h"
#include "nxclient.h"
#include "tspsimgr3.h"
#if B_REFSW_TR69C_SUPPORT
#include "tr69clib.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
NOTE: This API is only example code. It is subject to change and
is not supported as a standard reference software deliverable.
**/

#define	MianVideoWindow_Id			(0)

#define PLAYBACK_IP_SUPPORT  (1)


/**
Summary:
settings for media_player_create
**/
typedef struct media_player_create_settings
{
    bool decodeVideo; /* if false, video is ignored */
    bool decodeAudio; /* if false, audio is ignored */
	bool dtcpEnabled; /* if false, dtcp_ip wont be used */
    struct {
        unsigned surfaceClientId;
        unsigned id;
    } window; /* assign window to control position */
    unsigned maxWidth, maxHeight; /* Minimum source resolution from decoder. if stream resolution is detected to be
                                     higher, player will raise. After connecting, resource cannot be raised: disconnect/connect
                                     required to raise resolution. */
} media_player_create_settings;

typedef void (*PFN_VidDecStreamChangedCb)(void * pCtx);

enum tagPLAY_SRC_TYPE {
	PLAY_SRC_UNKNOWN,
	PLAY_SRC_FILE,
	PLAY_SRC_HdmiIn,
	PLAY_SRC_Tuner,
	PLAY_SRC_Picture,
};

typedef enum tagCCSTATUS
{
	CCINVALID,
	CCSTOP,
	CCPREPARE,
	CCSTART,
}tagCCSTATUS;

typedef enum tagPLAY_SRC_TYPE PLAY_SRC_TYPE_t;

enum media_player_audio_primers
{
    media_player_audio_primers_none,
    media_player_audio_primers_later,
    media_player_audio_primers_immediate
};

/**
Summary:
settings for media_player_start
**/
typedef struct media_player_start_settings
{
    const char *stream_url;
    const char *index_url;
    unsigned program;
	unsigned pmtIndex;
    NEXUS_PlaybackLoopMode loopMode;
    void (*eof)(void *context);
	PFN_VidDecStreamChangedCb pfnVidDecStreamChangedCb;
    void *context;
    NxClient_VideoWindowType videoWindowType;

	bool smoothResolutionChange;//add
    bool startPaused;//add
    bool stcTrick;//add
    unsigned dqtFrameReverse;//add

	enum //add
	{
        source3d_none,
        source3d_lr,
        source3d_ou,
        source3d_auto
    } source3d;

	NEXUS_VideoFormat maxFormat;//add

	struct//add 
	{
        unsigned pid;
        NEXUS_VideoCodec codec;
        unsigned fifoSize;
    } video;

	NEXUS_TransportType transportType; /*add*/

    struct 
	{
        NEXUS_SecurityAlgorithm algo; 
    } decrypt;

	struct 
	{
		unsigned pid;/*add*/
        NEXUS_VideoCodec codec;/*add*/
        NEXUS_AudioDecoderDolbyDrcMode dolbyDrcMode;
    } audio;

    bool quiet;
    NxClient_HdcpLevel hdcp;
	enum media_player_audio_primers audio_primers;/*add*/
} media_player_start_settings;

typedef struct media_player *media_player_t;

int ColseCapture(
	media_player_t player, 
	const media_player_start_settings *psettings,
	int ccSertype
	);

int colseCaptureThread(void *pParam);



/**
Summary:
**/
void media_player_get_default_create_settings(
    media_player_create_settings *psettings
    );

/**
Summary:
**/
void media_player_stop_cc(
	media_player_t handle
	);
int media_get_status(
	media_player_t player);


media_player_t media_player_create(
    media_player_create_settings *psettings /* optional */
    );

/**
Summary:
**/
void media_player_get_default_start_settings(
    media_player_start_settings *psettings
    );

/**
Start playback
**/
int media_player_start(
    media_player_t handle,
    const media_player_start_settings *psettings
    );

int media_player_tuner_prepare(
	media_player_t handle,
	NEXUS_ParserBand *oparserBand
	);

int media_player_tuner_start( 
	media_player_t player, 
	const media_player_start_settings *psettings,
	tspsimgr_program *pprogram
	);

/**
Summary:
Perform trick mode
**/
int media_player_trick(
    media_player_t handle,
    int rate /* units are NEXUS_NORMAL_DECODE_RATE. for example:
                0 = pause
                NEXUS_NORMAL_DECODE_RATE = normal play
                2*NEXUS_NORMAL_DECODE_RATE = 2x fast forward
                -3*NEXUS_NORMAL_DECODE_RATE = 3x rewind
                */
    );

/**
Summary:
Seek based on time
**/
int media_player_seek(
    media_player_t handle,
    long offset, /* in milliseconds */
    int whence /* SEEK_SET, SEEK_CUR, SEEK_END */
    );

/**
Summary:
**/
int media_player_get_playback_status( media_player_t handle, NEXUS_PlaybackStatus *pstatus );

/**
Summary:
**/
void media_player_stop( media_player_t handle );
int MediaPlayer_getVidPos(media_player_t pMediaPlayer, int *x, int *y, int *width, int *height);
int MediaPlayer_setVidPos(media_player_t pMediaPlayer, int x, int y, int w, int h);
int MediaPlayer_setVidWinAspectRatio(media_player_t pMediaPlayer, NEXUS_DisplayAspectRatio eDispAspectRatio, bool bFullVideo);
int MediaPlayer_setOsdPos(media_player_t pMediaPlayer, int x, int y, int w, int h);
int MediaPlayer_getOsdPos(media_player_t pMediaPlayer, int *x, int *y, int *width, int *height);
int MediaPlayer_changeHdmiDisplaySettings(media_player_t m_pMediaPlayer,int iDispParam,int iValue);
int MediaPlayer_setOsdSize(media_player_t pMediaPlayer, int w, int h);
/**
Summary:
**/
void media_player_destroy( media_player_t handle );

/**
Summary:
Create a set of media players for mosaic decode

Each can be started and stopped independently. Creating in a bundle
allows the underlying NxClient_Alloc and _Connect calls to be batched as required.
**/
int media_player_create_mosaics(
    media_player_t *players, /* array of size num_mosaics. will return NULL for players that can't be created */
    unsigned num_mosaics, /* may create less than requested number */
    const media_player_create_settings *psettings /* optional */
    );

void media_player_destroy_mosaics(
    media_player_t *players, /* array of size num_mosaics. */
    unsigned num_mosaics
    );

/**
Summary:
**/
NEXUS_SimpleVideoDecoderHandle media_player_get_video_decoder(media_player_t player);

/**
Summary:
**/
#if B_REFSW_TR69C_SUPPORT
int media_player_get_set_tr69c_info(void *context, enum b_tr69c_type type, union b_tr69c_info *info);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MEDIA_PLAYER_H__ */
