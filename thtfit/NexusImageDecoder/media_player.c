/******************************************************************************
 *    (c)2010-2014 Broadcom Corporation
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
#include "media_player.h"
#include "nexus_simple_video_decoder.h"
#include "nexus_simple_audio_decoder.h"
#include "nexus_simple_stc_channel.h"
#include "nexus_surface_client.h"
#include "nexus_playback.h"
#include "nexus_file.h"
#include "nexus_core_utils.h"
#include "namevalue.h"
#include "nxclient.h"

#include "blst_queue.h"

/* media probe */
#include "bmedia_probe.h"
#include "bmpeg2ts_probe.h"
#include "bfile_stdio.h"
#if B_HAS_ASF
#include "basf_probe.h"
#endif
#if B_HAS_AVI
#include "bavi_probe.h"
#endif
#include "bhevc_video_probe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "bstd.h"
#include "bkni.h"

#include <ErrPrintHelper.h>
#include <MediaPlayerApi.h>
#include <BaseTypeDef.h>
#include <nexus_video_window.h>
#include <ErrChkMutex.h>
#include <BaseErrDef.h>
#include <Compiler.h>
#include <picdecoder.h>
#include <DateTime.h>
#include <ProcessRunningInfo.h>
#include <tspsimgr3.h>
#include <nexus_parser_band.h>
#include "b_dcc_lib.h"
#include "parse_userdata.h"
#include "media_player_priv.h"

BDBG_MODULE(media_player);

LPCSTR getVidWinContentModeDesc(NEXUS_VideoWindowContentMode eVidWnContentMode);

BDBG_OBJECT_ID(media_player);


struct hdmi_edid 
{
	const void *data;
	unsigned size;
	bool allocated;
};

void media_player_get_default_create_settings( media_player_create_settings *psettings )
{
    memset(psettings, 0, sizeof(*psettings));
    psettings->decodeVideo = true;
    psettings->decodeAudio = true;
}

void media_player_get_default_start_settings( media_player_start_settings *psettings )
{
    memset(psettings, 0, sizeof(*psettings));
    psettings->loopMode = NEXUS_PlaybackLoopMode_eLoop;
    psettings->decrypt.algo = NEXUS_SecurityAlgorithm_eMax; /* none */
    psettings->audio.dolbyDrcMode = NEXUS_AudioDecoderDolbyDrcMode_eMax; /* none */
	psettings->stcTrick = true;
    psettings->video.pid = 0x1fff;
    psettings->audio.pid = 0x1fff;
    psettings->transportType = NEXUS_TransportType_eMax; /* no override */
}

static void endOfStreamCallback(void *context, int param)
{
    media_player_t player = context;
	INT_t iRet;

    BSTD_UNUSED(param);

	{
		bool bLocked = false;
		/* lock */
		if(CC_LIKELY(false == bLocked))
		{
			if(CC_LIKELY(player->hMutex))
			{
				iRet = CErrChkMutex_Lock(player->hMutex);
				if(CC_LIKELY(ERR_SUCCESS == iRet))
				{
					bLocked = true;
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		/* save */
		player->sourceStreamWidth = 0;	/*invalid*/
		player->sourceStreamHeight = 0;	/*invalid*/
		/* unlock */
		if(true == bLocked)
		{
			iRet = CErrChkMutex_Unlock(player->hMutex);
			if(ERR_SUCCESS == iRet)
			{
				bLocked = false;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

	if (player->start_settings.eof) {
        (player->start_settings.eof)(player->start_settings.context);
    }
}

static void vidDecoderStreamChangeCb(void * pContext, int param)
{
	INT_t iRet;
	media_player_t player = (typeof(player))(pContext);
    BSTD_UNUSED(param);

	/*LOG("vidDecoderStreamChangeCb\n");*/

	if(CC_LIKELY(player->videoDecoder))
	{
		NEXUS_VideoDecoderStreamInformation vidDecoderStreamInfo;
		NEXUS_SimpleVideoDecoder_GetStreamInformation(player->videoDecoder, &vidDecoderStreamInfo);
		if(CC_LIKELY(true == vidDecoderStreamInfo.valid))
		{
			bool bLocked = false;
			/*
			LOG("video stream changed:\n");
			LOG("  sourceHorizontalSize: %u\n", vidDecoderStreamInfo.sourceHorizontalSize);
			LOG("  sourceVerticalSize: %u\n", vidDecoderStreamInfo.sourceVerticalSize);
			LOG("  codedSourceHorizontalSize: %u\n", vidDecoderStreamInfo.codedSourceHorizontalSize);
			LOG("  codedSourceVerticalSize: %u\n", vidDecoderStreamInfo.codedSourceVerticalSize);
			LOG("  displayHorizontalSize: %u\n", vidDecoderStreamInfo.displayHorizontalSize);
			LOG("  displayVerticalSize: %u\n", vidDecoderStreamInfo.displayVerticalSize);
			LOG("  aspectRatio: %d\n", vidDecoderStreamInfo.aspectRatio);
			LOG("  sampleAspectRatioX: %hu\n", vidDecoderStreamInfo.sampleAspectRatioX);
			LOG("  sampleAspectRatioY: %hu\n", vidDecoderStreamInfo.sampleAspectRatioY);
			LOG("  frameRate: %d\n", vidDecoderStreamInfo.frameRate);
			LOG("  colorPrimaries: %d\n", vidDecoderStreamInfo.colorPrimaries);
			LOG("  transferCharacteristics: %d\n", vidDecoderStreamInfo.transferCharacteristics);
			LOG("  matrixCoefficients: %d\n", vidDecoderStreamInfo.matrixCoefficients);
			LOG("  frameProgressive: %d\n", vidDecoderStreamInfo.frameProgressive);
			LOG("  streamProgressive: %d\n", vidDecoderStreamInfo.streamProgressive);
			LOG("  horizontalPanScan: %d\n", vidDecoderStreamInfo.horizontalPanScan);
			LOG("  verticalPanScan: %d\n", vidDecoderStreamInfo.verticalPanScan);
			LOG("  lowDelayFlag: %d\n", vidDecoderStreamInfo.lowDelayFlag);
			LOG("  fixedFrameRateFlag: %d\n", vidDecoderStreamInfo.fixedFrameRateFlag);
			*/
			/* lock */
			if(CC_LIKELY(false == bLocked))
			{
				if(CC_LIKELY(player->hMutex))
				{
					iRet = CErrChkMutex_Lock(player->hMutex);
					if(CC_LIKELY(ERR_SUCCESS == iRet))
					{
						bLocked = true;
					}
					else
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
			}
			/* save */
			player->sourceStreamWidth = vidDecoderStreamInfo.sourceHorizontalSize;
			player->sourceStreamHeight = vidDecoderStreamInfo.sourceVerticalSize;
			/* unlock */
			if(true == bLocked)
			{
				iRet = CErrChkMutex_Unlock(player->hMutex);
				if(ERR_SUCCESS == iRet)
				{
					bLocked = false;
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
	}

	if(CC_LIKELY(NULL != player->start_settings.pfnVidDecStreamChangedCb))
	{
		player->start_settings.pfnVidDecStreamChangedCb(player->start_settings.context);
	}
}

static media_player_t media_player_p_create(const NxClient_AllocResults *pAllocResults, unsigned allocIndex, 
	const media_player_create_settings *psettings, bool mosaic)
{
    NEXUS_PlaypumpOpenSettings playpumpOpenSettings;
    NEXUS_PlaybackSettings playbackSettings;
    media_player_t player;
    int rc;

    player = malloc(sizeof(*player));
    if (!player) {
        return NULL;
    }
    memset(player, 0, sizeof(*player));
    BDBG_OBJECT_SET(player, media_player);
    player->create_settings = *psettings;
	player->bNxConnectSettingsIsAvailable = false;
    player->allocResults = *pAllocResults;
    player->allocIndex = allocIndex;
	/* obj's mutex */
	player->hMutex = CErrChkMutex_new();
	player->ePlaySrcType = PLAY_SRC_UNKNOWN;
	player->parserBand=-1;
		
    NEXUS_Playpump_GetDefaultOpenSettings(&playpumpOpenSettings);
    if (psettings->maxHeight <= 576) {
        playpumpOpenSettings.fifoSize = 1024*1024;
    }
    player->playpump = NEXUS_Playpump_Open(NEXUS_ANY_ID, &playpumpOpenSettings);
    if (!player->playpump) {
        free(player);
        BDBG_WRN(("no more playpumps available"));
        return NULL;
    }
    player->playback = NEXUS_Playback_Create();
    BDBG_ASSERT(player->playback);

    player->stcChannel = NEXUS_SimpleStcChannel_Create(NULL);
    if (!player->stcChannel) {
        BDBG_WRN(("stc channel not available"));
    }

    NEXUS_Playback_GetSettings(player->playback, &playbackSettings);
    playbackSettings.playpump = player->playpump;
    playbackSettings.simpleStcChannel = player->stcChannel;
    playbackSettings.stcTrick = !mosaic && player->stcChannel != NULL;
    playbackSettings.endOfStreamCallback.callback = endOfStreamCallback;
    playbackSettings.endOfStreamCallback.context = player;
    rc = NEXUS_Playback_SetSettings(player->playback, &playbackSettings);
    BDBG_ASSERT(!rc);

    if (player->allocResults.simpleVideoDecoder[player->allocIndex].id) 
	{
        player->videoDecoder = NEXUS_SimpleVideoDecoder_Acquire(player->allocResults.simpleVideoDecoder[player->allocIndex].id);
        NEXUS_SimpleVideoDecoder_GetSettings(player->videoDecoder, &(player->videoDecoderSettings));
    }
	
    if (psettings->decodeVideo && !player->videoDecoder) 
	{
        BDBG_WRN(("video decoder not available"));
    }

    if (player->allocIndex == 0) 
	{
        if (player->allocResults.simpleAudioDecoder.id) 
		{
            player->audioDecoder = NEXUS_SimpleAudioDecoder_Acquire(player->allocResults.simpleAudioDecoder.id);
        }
		
        if (psettings->decodeAudio && !player->audioDecoder) 
		{
            BDBG_WRN(("audio decoder not available"));
        }
    }

	player->ip = media_player_ip_create(player);


    return player;
}

/*
If need do disconnect, return true.
*/
static bool media_player_p_test_disconnect(media_player_t player, NEXUS_SimpleVideoDecoderStartSettings *pVideoProgram)
{
	bool bNeedDisconect = false;
	if(PLAY_SRC_HdmiIn == player->ePlaySrcType)
	{
		if(0 == player->connectId || false == player->bNxConnectSettingsIsAvailable)
		{
			bNeedDisconect = false;
			goto EXIT_PROC;
		}
		if(player->bNxConnectSettingsIsAvailable)
		{
			if(player->allocResults.simpleVideoDecoder[0].id != player->nxConnectSettings.simpleVideoDecoder[0].id ||
				player->create_settings.window.surfaceClientId != player->nxConnectSettings.simpleVideoDecoder[0].surfaceClientId ||
				player->create_settings.window.id != player->nxConnectSettings.simpleVideoDecoder[0].windowId ||
				player->allocResults.simpleAudioDecoder.id != player->nxConnectSettings.simpleAudioDecoder.id ||
				0 != player->nxConnectSettings.simpleVideoDecoder[0].decoderCapabilities.maxWidth ||
				0 != player->nxConnectSettings.simpleVideoDecoder[0].decoderCapabilities.maxHeight
				)
			{
				bNeedDisconect = true;
				goto EXIT_PROC;
			}
		}
	}
	else	/*other src*/
	{
	    bNeedDisconect = (player->connectId && player->videoDecoder &&
	        (player->videoProgram.displayEnabled != pVideoProgram->displayEnabled ||
	         !player->videoDecoderSettings.supportedCodecs[pVideoProgram->settings.codec] ||
	         player->colorDepth != player->videoDecoderSettings.colorDepth));
		
		if(true == bNeedDisconect)
		{
			goto EXIT_PROC;
		}
	}
EXIT_PROC:	
	return bNeedDisconect;
}

/* connect client resources to server's resources */
static int media_player_p_connect(media_player_t player)
{
    NxClient_ConnectSettings connectSettings;
    int rc;

    BDBG_ASSERT(!player->master || player->master == player);

    if (player->videoDecoder && player->videoDecoderSettings.colorDepth != player->colorDepth) {
		/* no media_player API for colorDepth. instead, we force an NxClient_Connect based on a probe-detected
		colorDepth change from current settings. if we get dynamic changes from 8->10 in the future, we'll need a
		user setting to start with 10 bit. */
        NEXUS_SimpleVideoDecoder_GetSettings(player->videoDecoder, &player->videoDecoderSettings);
        player->videoDecoderSettings.colorDepth = player->colorDepth;
		/* set */
        rc = NEXUS_SimpleVideoDecoder_SetSettings(player->videoDecoder, &player->videoDecoderSettings);
        if (rc) return BERR_TRACE(rc);
    }

    NxClient_GetDefaultConnectSettings(&connectSettings);
    if (player->master) {
        unsigned i;
        media_player_t p;

        if (player->mosaic_start_count) {
            /* already connected */
            player->mosaic_start_count++;
            return 0;
        }
        for (i=0,p=BLST_Q_FIRST(&player->master->players);p;i++,p=BLST_Q_NEXT(p,link)) {
            const media_player_create_settings *psettings = &p->create_settings;
            connectSettings.simpleVideoDecoder[i].id = player->master->allocResults.simpleVideoDecoder[i].id;
            connectSettings.simpleVideoDecoder[i].surfaceClientId = psettings->window.surfaceClientId;
            connectSettings.simpleVideoDecoder[i].windowId = i;
            connectSettings.simpleVideoDecoder[i].decoderCapabilities.maxWidth = player->videoProgram.maxWidth;
            connectSettings.simpleVideoDecoder[i].decoderCapabilities.maxHeight = player->videoProgram.maxHeight;
            connectSettings.simpleVideoDecoder[i].decoderCapabilities.colorDepth = player->colorDepth;
            connectSettings.simpleVideoDecoder[i].windowCapabilities.type = player->start_settings.videoWindowType;
            /* connectSettings.simpleVideoDecoder[i].decoderCapabilities.supportedCodecs[player->videoProgram.settings.codec] = true; */
        }
        connectSettings.simpleAudioDecoder.id = player->master->allocResults.simpleAudioDecoder.id;
    } else {	/*not master*/
        const media_player_create_settings *psettings = &player->create_settings;
        if (player->videoProgram.settings.pidChannel) {
            connectSettings.simpleVideoDecoder[0].id = player->allocResults.simpleVideoDecoder[0].id;
            connectSettings.simpleVideoDecoder[0].surfaceClientId = psettings->window.surfaceClientId;
            connectSettings.simpleVideoDecoder[0].windowId = psettings->window.id;
			if(player->start_settings.maxFormat)
			{
            	connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxFormat = player->start_settings.maxFormat;
			}
			else
			{
				connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxWidth = player->videoProgram.maxWidth;
            	connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxHeight = player->videoProgram.maxHeight;
			}
			connectSettings.simpleVideoDecoder[0].decoderCapabilities.supportedCodecs[player->videoProgram.settings.codec] = true;
            connectSettings.simpleVideoDecoder[0].decoderCapabilities.colorDepth = player->colorDepth;
			connectSettings.simpleVideoDecoder[0].decoderCapabilities.fifoSize = player->start_settings.video.fifoSize;
			connectSettings.simpleVideoDecoder[0].windowCapabilities.type = player->start_settings.videoWindowType;
        }
        if (player->audioProgram.primary.pidChannel) {
            connectSettings.simpleAudioDecoder.id = player->allocResults.simpleAudioDecoder.id;
			connectSettings.simpleAudioDecoder.primer = (player->start_settings.audio_primers == media_player_audio_primers_immediate);
        }
		/*hdmi in*/
		if(PLAY_SRC_HdmiIn == player->ePlaySrcType)
		{
            connectSettings.simpleVideoDecoder[0].id = player->allocResults.simpleVideoDecoder[0].id;
            connectSettings.simpleVideoDecoder[0].surfaceClientId = psettings->window.surfaceClientId;
            connectSettings.simpleVideoDecoder[0].windowId = psettings->window.id;
			connectSettings.simpleAudioDecoder.id = player->allocResults.simpleAudioDecoder.id;
			connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxWidth = 0; /* no decoder needed, just a window */
			connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxHeight = 0;
		}
    }
    rc = NxClient_Connect(&connectSettings, &player->connectId);
    if (rc) return BERR_TRACE(rc);
	player->nxConnectSettings = connectSettings;

    if (player->master) {
        player->mosaic_start_count++;
    }

    return 0;
}

static void media_player_p_disconnect(media_player_t player)
{
    if (player->master) 
	{
        if (player->audio.connectId) 
		{
            NxClient_Disconnect(player->audio.connectId);
            player->audio.connectId = 0;
        }

        if (!player->master->mosaic_start_count || --player->master->mosaic_start_count) return;

		if (player->master->connectId) 
		{
            NxClient_Disconnect(player->master->connectId);
            player->master->connectId = 0;
        }
    }
    else 
	{
        if (player->connectId) 
		{
            NxClient_Disconnect(player->connectId);
            player->connectId = 0;
        }
    }

}

media_player_t media_player_create( media_player_create_settings *psettings )
{
    NxClient_JoinSettings joinSettings;
    NxClient_AllocSettings allocSettings;
    NxClient_AllocResults allocResults;
    media_player_t player = NULL;
    media_player_create_settings default_settings;
	NEXUS_SurfaceClientHandle hSurfaceClient = NULL;
    int rc;

    if (!psettings) {
        media_player_get_default_create_settings(&default_settings);
        psettings = &default_settings;
    }

    /* connect to server and nexus */
    NxClient_GetDefaultJoinSettings(&joinSettings);
    snprintf(joinSettings.name, NXCLIENT_MAX_NAME, "%s", MP_NexusNxClientName);
	{
		UINT64_t uSysUpTimeMs, uSysUpStartTimeMs;
		uSysUpStartTimeMs = GetSysUpTimeMs();
RETRY_NX_JOIN:
	    rc = NxClient_Join(&joinSettings);
		if(NEXUS_SUCCESS != rc)
		{
			uSysUpTimeMs = GetSysUpTimeMs();
			if((5*1000) > (uSysUpTimeMs - uSysUpStartTimeMs))
			{
				if(TRUE == NxserverIsRunning())
				{
					/*LOG_BLINE("nxserver is running, retry.\n");*/
					sleep(1);
					goto RETRY_NX_JOIN;
				}
			}			
		}
	    if (rc) {
	        return NULL;
	    }
    }

    NxClient_GetDefaultAllocSettings(&allocSettings);
    allocSettings.simpleVideoDecoder = psettings->decodeVideo?1:0;
    allocSettings.simpleAudioDecoder = psettings->decodeAudio?1:0;
	allocSettings.surfaceClient = 1;	/* surface client required for video window */
    rc = NxClient_Alloc(&allocSettings, &allocResults);
    if (rc) { rc = BERR_TRACE(rc); goto error_alloc;}

	/*LOG_BLINE("AllocRes:\n");*/
	LOG_BLINE("  surfaceClient[0].id: %u\n", allocResults.surfaceClient[0].id);
	if(0 < allocResults.surfaceClient[0].id)
	{
		hSurfaceClient = NEXUS_SurfaceClient_Acquire(allocResults.surfaceClient[0].id);
		if(NULL != hSurfaceClient)
		{
			
		}
		else
		{
			LOG_BLINE("Err,SurfaceClient_Acquire(%u)\n", allocResults.surfaceClient[0].id);
		}
		psettings->window.surfaceClientId = allocResults.surfaceClient[0].id;
		psettings->window.id = MianVideoWindow_Id;
	}	

#if	0
	{
		NEXUS_SurfaceComposition surfaceComposition;
		NEXUS_SurfaceClientSettings surfaceClientSettings;
		
		NxClient_GetSurfaceClientComposition(allocResults.surfaceClient[0].id, &surfaceComposition);
		/*
		LOG_BLINE("%u surfaceComposition:\n", allocResults.surfaceClient[0].id);
		LOG_BLINE("  virtualDisplay:\n");
		LOG_BLINE("    Width: %u\n", surfaceComposition.virtualDisplay.width);
		LOG_BLINE("    Height: %u\n", surfaceComposition.virtualDisplay.height);
		LOG_BLINE("  position:\n"); 
		LOG_BLINE("    x: %d\n", surfaceComposition.position.x);	
		LOG_BLINE("    y: %d\n", surfaceComposition.position.y);	
		LOG_BLINE("    w: %d\n", surfaceComposition.position.width);	
		LOG_BLINE("    h: %d\n", surfaceComposition.position.height);	
		LOG_BLINE("  clipRect:\n"); 
		LOG_BLINE("    x: %d\n", surfaceComposition.clipRect.x);	
		LOG_BLINE("    y: %d\n", surfaceComposition.clipRect.y);	
		LOG_BLINE("    w: %d\n", surfaceComposition.clipRect.width);	
		LOG_BLINE("    h: %d\n", surfaceComposition.clipRect.height);	
		LOG_BLINE("  zorder: %u\n", surfaceComposition.zorder); 
		LOG_BLINE("  contentMode: %s\n", getVidWinContentModeDesc(surfaceComposition.contentMode)); 
		*/
		surfaceComposition.virtualDisplay.width = HwScaler_Relative_MAX_SIZE_W;
		surfaceComposition.virtualDisplay.height = HwScaler_Relative_MAX_SIZE_H;
		surfaceComposition.position.x = 0;
		surfaceComposition.position.y = 0;
		surfaceComposition.position.width = 2048;
		surfaceComposition.position.height = 4096;
	
		LOG_BLINE("%u surfaceComposition:\n", allocResults.surfaceClient[0].id);
		LOG_BLINE("  virtualDisplay:\n");
		LOG_BLINE("    Width: %u\n", surfaceComposition.virtualDisplay.width);
		LOG_BLINE("    Height: %u\n", surfaceComposition.virtualDisplay.height);
		LOG_BLINE("  position:\n"); 
		LOG_BLINE("    x: %d\n", surfaceComposition.position.x);	
		LOG_BLINE("    y: %d\n", surfaceComposition.position.y);	
		LOG_BLINE("    w: %d\n", surfaceComposition.position.width);	
		LOG_BLINE("    h: %d\n", surfaceComposition.position.height);	
		LOG_BLINE("  clipRect:\n"); 
		LOG_BLINE("    x: %d\n", surfaceComposition.clipRect.x);	
		LOG_BLINE("    y: %d\n", surfaceComposition.clipRect.y);	
		LOG_BLINE("    w: %d\n", surfaceComposition.clipRect.width);	
		LOG_BLINE("    h: %d\n", surfaceComposition.clipRect.height);	
		LOG_BLINE("  zorder: %u\n", surfaceComposition.zorder); 
		LOG_BLINE("  contentMode: %s\n", getVidWinContentModeDesc(surfaceComposition.contentMode)); 
	
		iNexusErr = NxClient_SetSurfaceClientComposition(allocResults.surfaceClient[0].id, &surfaceComposition);
		if(NEXUS_SUCCESS != iNexusErr)
		{
			iRet = iNexusErr;
			PRINT_BFILE_LINENO_IRET_STR;
		}	

		if(hSurfaceClient)
		{
			NEXUS_SurfaceClient_GetSettings(hSurfaceClient, &surfaceClientSettings);
			iNexusErr = NEXUS_SurfaceClient_SetSettings(hSurfaceClient, &surfaceClientSettings);
			if(NEXUS_SUCCESS != iNexusErr)
			{
				iRet = iNexusErr;
				PRINT_BFILE_LINENO_IRET_STR;
			}	
		}
	}
#endif

    player = media_player_p_create(&allocResults, 0, psettings, false);
    if (!player) goto error_create_player;

	player->hSurfaceClient = hSurfaceClient;
	hSurfaceClient = NULL;	/*detached*/

	if(NULL != player->hSurfaceClient)
	{
		player->hVideoSurfaceClient = NEXUS_SurfaceClient_AcquireVideoWindow(player->hSurfaceClient, psettings->window.id);
		if(NULL != player->hVideoSurfaceClient)
		{
			
		}
		else
		{
			LOG_BLINE("Err,AcquireVideoWindow(%u)\n", psettings->window.id);
		}
	}
	/*
	iNexusErr = MediaPlayer_setVidPos(player, 0, 0, 2048, 2048);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		iRet = iNexusErr;
		PRINT_BFILE_LINENO_IRET_STR;
	}
	*/

    return player;

error_create_player:
    NxClient_Free(&allocResults);
error_alloc:
    NxClient_Uninit();

	if(hSurfaceClient)
	{
		NEXUS_SurfaceClient_Release(hSurfaceClient);		
		hSurfaceClient = NULL;
	}
	
    return NULL;
}

int media_player_create_mosaics(media_player_t *players, unsigned num_mosaics, const media_player_create_settings *psettings )
{
    NxClient_JoinSettings joinSettings;
    NxClient_AllocSettings allocSettings;
    NxClient_AllocResults allocResults;
    int rc;
    media_player_create_settings default_settings;
    unsigned i;

    if (!psettings) {
        media_player_get_default_create_settings(&default_settings);
        psettings = &default_settings;
    }

    /* connect to server and nexus */
    NxClient_GetDefaultJoinSettings(&joinSettings);
    snprintf(joinSettings.name, NXCLIENT_MAX_NAME, "%s", MP_NexusNxClientName);
    rc = NxClient_Join(&joinSettings);
    if (rc) return BERR_TRACE(rc);

    NxClient_GetDefaultAllocSettings(&allocSettings);
    allocSettings.simpleVideoDecoder = psettings->decodeVideo?num_mosaics:0;
    allocSettings.simpleAudioDecoder = psettings->decodeAudio?1:0;
    rc = NxClient_Alloc(&allocSettings, &allocResults);
    if (rc) { rc = BERR_TRACE(rc); goto error_alloc;}

    BKNI_Memset(players, 0, sizeof(players[0])*num_mosaics);
    for (i=0;i<num_mosaics;i++) {
        players[i] = media_player_p_create(&allocResults, i, psettings, true);
        if (!players[i]) {
            if (i == 0) {
                rc = BERR_TRACE(NEXUS_NOT_AVAILABLE);
                goto error_create_player;
            }
            /* if we can create at least one, go with it */
            break;
        }

        players[i]->master = players[0];
        BLST_Q_INSERT_TAIL(&players[0]->players, players[i], link);
    }

    return 0;

error_create_player:
    media_player_destroy_mosaics(players, num_mosaics);
    NxClient_Free(&allocResults);
error_alloc:
    NxClient_Uninit();
    BDBG_ASSERT(rc);
    return rc;
}

void media_player_destroy_mosaics( media_player_t *players, unsigned num_mosaics )
{
    int i;
    for (i=num_mosaics-1;i>=0;i--) {
        if (players[i]) {
            BLST_Q_REMOVE(&players[0]->players, players[i], link);
            media_player_destroy(players[i]);
        }
    }
}

static void b_print_media_string(const bmedia_probe_stream *stream)
{
    char stream_info[512];
    bmedia_stream_to_string(stream, stream_info, sizeof(stream_info));
    printf( "Media Probe:\n" "%s\n\n", stream_info);
}

#undef min
#define min(A,B) ((A)<(B)?(A):(B))

/* parse_url()

example: http://player.vimeo.com:80/play_redirect?quality=hd&codecs=h264&clip_id=638324

    scheme=http
    domain=player.vimeo.com
    port=80
    path=/play_redirect?quality=hd&codecs=h264&clip_id=638324

example: file://videos/cnnticker.mpg or videos/cnnticker.mpg

    scheme=file
    domain=
    port=0
    path=videos/cnnticker.mpg

example: udp://192.168.1.10:1234 || udp://224.1.1.10:1234 || udp://@:1234

    scheme=udp
    domain=live channel is streamed to this STB's IP address 192.168.1.10 || server is streaming to a multicast address 224.1.1.10 || this STB but user doesn't need to explicitly specify its IP address
    port=1234
    path=N/A as server is just streaming out a live channel, client doesn't get to pick a file

*/
static void parse_url(const char *s, struct url *url)
{
    const char *server, *file;

    memset(url, 0, sizeof(*url));

    server = strstr(s, "://");
    if (!server) {
        strcpy(url->scheme, "file");
        server = s;
    }
    else {
        strncpy(url->scheme, s, server-s);
        server += strlen("://"); /* points to the start of server name */
    }

    if (!strcmp(url->scheme, "file")) {
        strncpy(url->path, server, sizeof(url->path)-1);
    }
    else {
        char *port;
        file = strstr(server, "/"); /* should point to start of file name */
        if (file) {
            strncpy(url->domain, server, min(sizeof(url->domain)-1, (unsigned)(file-server)));
            strncpy(url->path, file, sizeof(url->path)-1);
        }
        else {
            strncpy(url->domain, server, sizeof(url->domain)-1);
        }

        /* now server string is null terminated, look for explicit port number in the format of server:port */
        port = strstr(url->domain, ":");
        if (port) {
            *port = 0;
            url->port = atoi(port+1);
        }
        else {
            url->port = 80; /* default port */
        }
    }
}

/* Hdmi input was added  by abner on november 3th,2014 */
static void LoadHdmiInEdid(struct hdmi_edid *edid)
{
    static const uint8_t sampleEdid[] =
    {
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x08, 0x6D, 0x74, 0x22, 0x05, 0x01, 0x11, 0x20,
        0x00, 0x14, 0x01, 0x03, 0x80, 0x00, 0x00, 0x78, 0x0A, 0xDA, 0xFF, 0xA3, 0x58, 0x4A, 0xA2, 0x29,
        0x17, 0x49, 0x4B, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
        0x45, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16, 0x20,
        0x58, 0x2C, 0x25, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x9E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x42,
        0x43, 0x4D, 0x37, 0x34, 0x32, 0x32, 0x2F, 0x37, 0x34, 0x32, 0x35, 0x0A, 0x00, 0x00, 0x00, 0xFD,
        0x00, 0x17, 0x3D, 0x0F, 0x44, 0x0F, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0x89,

        0x02, 0x03, 0x3C, 0x71, 0x7F, 0x03, 0x0C, 0x00, 0x40, 0x00, 0xB8, 0x2D, 0x2F, 0x80, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xE3, 0x05, 0x1F, 0x01, 0x49, 0x90, 0x05, 0x20, 0x04, 0x03, 0x02, 0x07,
        0x06, 0x01, 0x29, 0x09, 0x07, 0x01, 0x11, 0x07, 0x00, 0x15, 0x07, 0x00, 0x01, 0x1D, 0x00, 0x72,
        0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28, 0x55, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x1E, 0x8C, 0x0A,
        0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x18,
        0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x0B, 0x88, 0x21, 0x00,
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9D
    };

    memset(edid, 0, sizeof(*edid));

    /* default to sample edid if not connect or there's any failure */
    edid->data = sampleEdid;
    edid->size = sizeof(sampleEdid);
}

static void FreeHdmiInEdid(struct hdmi_edid *edid)
{
    if (edid->allocated && edid->data) {
        BKNI_Free((void*)edid->data);
        edid->data = NULL;
    }
}

static int MP_ConnectToNx(media_player_t player)
{
	int nNexusErr = NEXUS_SUCCESS;
	if(0 == player->connectId)
	{
		nNexusErr = media_player_p_connect(player);		
		if (NEXUS_SUCCESS != nNexusErr) {
			goto EXIT_PROC;
		}
		
		if(NULL != player->videoDecoder)
		{
			NEXUS_SimpleVideoDecoder_GetSettings(player->videoDecoder, &player->videoDecoderSettings);
			if(player->videoDecoderSettings.streamChanged.callback != vidDecoderStreamChangeCb)
			{
				/* vid stream change callback */
				player->videoDecoderSettings.streamChanged.callback = vidDecoderStreamChangeCb;
				player->videoDecoderSettings.streamChanged.context = player;
				player->videoDecoderSettings.streamChanged.param = 0;
				/* set */
				nNexusErr = NEXUS_SimpleVideoDecoder_SetSettings(player->videoDecoder, &player->videoDecoderSettings);
				if (NEXUS_SUCCESS != nNexusErr) { goto EXIT_PROC; }
			}
		}
	}
EXIT_PROC:
	return nNexusErr;
}

static int hdmi_input_capture_player_start(media_player_t player)
{
	NEXUS_HdmiInputSettings hdmiInputSettings;
	unsigned nHdmiInIndex = 0;
	struct hdmi_edid edid;
	int rc = NEXUS_SUCCESS;	
	
	/*BDBG_LOG(("hdmi_input_capture_player_start\n"));*/

	LoadHdmiInEdid(&edid);
	NEXUS_HdmiInput_GetDefaultSettings(&hdmiInputSettings);
	hdmiInputSettings.timebase = NEXUS_Timebase_e0;
	hdmiInputSettings.frontend.hpdDisconnected = false;
	hdmiInputSettings.useInternalEdid = true;
	player->hdmiInput = NEXUS_HdmiInput_OpenWithEdid(nHdmiInIndex, &hdmiInputSettings, edid.data, edid.size);
	if (!player->hdmiInput) 
	{
		BDBG_ERR(("HdmiInput %d not available", nHdmiInIndex));
		rc = NEXUS_NOT_AVAILABLE;
		goto EXIT_PROC;
	}	
	FreeHdmiInEdid(&edid);

	if(true == media_player_p_test_disconnect(player, NULL))
	{
		media_player_p_disconnect(player);
	}
	rc = MP_ConnectToNx(player);
	if (NEXUS_SUCCESS != rc) {
		goto EXIT_PROC;
	}

	if(player->videoDecoder)
	{
		rc = NEXUS_SimpleVideoDecoder_StartHdmiInput(player->videoDecoder, player->hdmiInput, NULL);
		if(NEXUS_SUCCESS != rc)
		{
			goto EXIT_PROC;
		}
	}

	if(player->audioDecoder)
	{
	
		rc = NEXUS_SimpleAudioDecoder_StartHdmiInput(player->audioDecoder, player->hdmiInput, NULL);
		if(NEXUS_SUCCESS != rc)
		{
			goto EXIT_PROC;
		}


		
	}

EXIT_PROC:	
	return rc;
}


static void complete(void *data, int unused)
{
    BSTD_UNUSED(unused);
    BKNI_SetEvent((BKNI_EventHandle)data);
}

static int show_image_start(media_player_t player, char *string_path)
{
	NEXUS_ClientConfiguration clientConfig;
	NEXUS_SurfaceCreateSettings createSettings;
	NEXUS_SurfaceClientSettings client_settings;
	NEXUS_Error rc;

	NEXUS_Platform_GetClientConfiguration(&clientConfig);
	BKNI_CreateEvent(&(player->picDisplayedEvent));

	NEXUS_SurfaceClient_GetSettings(player->hSurfaceClient, &client_settings);
	client_settings.displayed.callback = complete;
	client_settings.displayed.context = player->picDisplayedEvent;
	rc = NEXUS_SurfaceClient_SetSettings(player->hSurfaceClient, &client_settings);
	if(NEXUS_SUCCESS != rc)
	{
		goto EXIT_PROC;
	}

	NEXUS_Surface_GetDefaultCreateSettings(&createSettings);
	createSettings.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
	#if 0
	createSettings.width = 1280;
	createSettings.height = 720;
	#else
	createSettings.width = 3840;
	createSettings.height = 2160;
	#endif
	createSettings.heap = clientConfig.heap[NXCLIENT_SECONDARY_GRAPHICS_HEAP]; /* if NULL, will use NXCLIENT_DEFAULT_HEAP */
	player->picSurface = NEXUS_Surface_Create(&createSettings);

	player->picHandle = picdecoder_open();
	if (!player->picHandle) 
	{
		BDBG_ERR(("not supported on this platform\n"));
		rc = -1;
		goto EXIT_PROC;
	}

	if (picdecoder_decode(player->picHandle, string_path, player->picSurface, picdecoder_aspect_ratio_box)) 
	{
		BDBG_ERR(("unable to display\n"));
		rc = -1;
		goto EXIT_PROC;
	}
	else 
	{
		player->isShowImage = true;
		rc = NEXUS_SurfaceClient_SetSurface(player->hSurfaceClient, player->picSurface);
		if(NEXUS_SUCCESS != rc)
		{
			goto EXIT_PROC;
		}
		
		rc = BKNI_WaitForEvent(player->picDisplayedEvent, 5000);
		if(NEXUS_SUCCESS != rc)
		{
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:	
	return rc;
}

static void show_image_stop(media_player_t player)
{
	picdecoder_close(player->picHandle);
	NEXUS_Surface_Destroy(player->picSurface);
	BKNI_DestroyEvent(player->picDisplayedEvent);
	NEXUS_SurfaceClient_Clear(player->hSurfaceClient);
	player->picHandle = NULL;
	player->isShowImage = false;

}	


int media_player_start( media_player_t player, const media_player_start_settings *psettings )
{
    NEXUS_PlaybackPidChannelSettings playbackPidSettings;
	NEXUS_PlaybackSettings playbackSettings;
    NxClient_DisplaySettings displaySettings;
	struct url url;

	NEXUS_PlaybackStartSettings playbackStartSettings;
	NEXUS_Error rc;

	
	parse_url(psettings->stream_url, &url);

    BDBG_OBJECT_ASSERT(player, media_player);
    if (player->started) 
	{
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }
	
    if (!psettings) 
	{
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }
	
    if (!psettings->stream_url) 
	{
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }

    player->start_settings = *psettings;

    NEXUS_SimpleAudioDecoder_GetDefaultStartSettings(&player->audioProgram);

    NxClient_GetDisplaySettings(&displaySettings);
    if (displaySettings.hdmiPreferences.hdcp != psettings->hdcp) {
        displaySettings.hdmiPreferences.hdcp = psettings->hdcp;
        NxClient_SetDisplaySettings(&displaySettings);
    }

	NEXUS_Playback_GetDefaultStartSettings(&playbackStartSettings);
		
    if (!strcasecmp(url.scheme, "http") || !strcasecmp(url.scheme, "https") \
		|| !strcasecmp(url.scheme, "udp") || !strcasecmp(url.scheme, "rtp")) 
	{
        if (!player->ip) 
		{
            BDBG_ERR(("%s: not suppported. Rebuild with PLAYBACK_IP_SUPPORT=y", url.scheme));
            return NEXUS_NOT_SUPPORTED;
        }
        rc = media_player_ip_start(player->ip, psettings, &url);
        if (rc) return BERR_TRACE(rc);
        player->ipActive = true;
    }
	else
    {
		if(!strcasecmp(url.scheme, "capture") && !strncasecmp(url.domain,"hdmi",4))
		{
			player->ePlaySrcType = PLAY_SRC_HdmiIn;
			rc = hdmi_input_capture_player_start(player);
			if (rc) { BERR_TRACE(rc); return rc; }
			else {player->started = true; return 0;}
		}
		else	/*IMAGE & FILE*/
		{
			if (strcasestr(url.path, ".jpg") || strcasestr(url.path, ".es") || 
				strcasestr(url.path, ".png") || strcasestr(url.path, ".gif") || 
				strcasestr(url.path, ".bmp")) 
			{
				player->ePlaySrcType = PLAY_SRC_Picture;
				rc = show_image_start(player,url.path);
				if(NEXUS_SUCCESS != rc) 
				{
					show_image_stop(player);
					return rc;
				}else return 0;
			}
			else
			{
				NEXUS_SimpleVideoDecoderStartSettings videoProgram;
				NEXUS_TransportType transportType = NEXUS_TransportType_eTs;
				NEXUS_TransportTimestampType timestampType = NEXUS_TransportTimestampType_eNone;
				const char *index_url = NULL;
				const char *probe_file;
				bfile_io_read_t fd = NULL;
    			FILE *fin;
    			bmedia_probe_config probe_config;
    			const bmedia_probe_track *track;
				NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&videoProgram);

				probe_file = url.path;
				player->ePlaySrcType = PLAY_SRC_FILE;
				player->probe = bmedia_probe_create();

				fin = fopen64(probe_file, "rb");
				if (!fin) 
				{
					BDBG_ERR(("can't open '%s'", url.path));
					rc = -1;
					goto error;
				}

				fd = bfile_stdio_read_attach(fin);

				bmedia_probe_default_cfg(&probe_config);
				probe_config.file_name = probe_file;
				probe_config.type = bstream_mpeg_type_unknown;
				player->stream = bmedia_probe_parse(player->probe, fd, &probe_config);

				/* now stream is either NULL, or stream descriptor with linked list of audio/video tracks */
				bfile_stdio_read_detach(fd);

				fclose(fin);
				if(!player->stream) 
				{
					BDBG_ERR(("media probe can't parse '%s'", url.path));
					rc = -1;
					goto error;
				}

				index_url = psettings->index_url;
				if (!index_url) 
				{
					if (player->stream->index == bmedia_probe_index_available || player->stream->index == bmedia_probe_index_required) 
					{
						index_url = url.path;
					}
				}

				if (player->stream->type == bstream_mpeg_type_es) 
				{
					track = BLST_SQ_FIRST(&player->stream->tracks);
					if (track && track->type == bmedia_track_type_audio) 
					{
						index_url = url.path;
						NEXUS_Playback_GetSettings(player->playback, &playbackSettings);
						playbackSettings.enableStreamProcessing = true;
						(void)NEXUS_Playback_SetSettings(player->playback, &playbackSettings);
					}
				}

				player->file = NEXUS_FilePlay_OpenPosix3(url.path, index_url, false);
				if (!player->file) 
				{
					BDBG_ERR(("can't open '%s' and '%s'", url.path, index_url));
					rc = -1;
					goto error;
				}

				if (player->stream) 
				{
		            if (!psettings->quiet) b_print_media_string(player->stream);
		            transportType = b_mpegtype2nexus(player->stream->type);
		            if (player->stream->type == bstream_mpeg_type_ts) 
					{
		                if ((((bmpeg2ts_probe_stream*)player->stream)->pkt_len) == 192) 
						{
		                    timestampType = NEXUS_TransportTimestampType_eMod300;
		                }
		            }
		        }
				
				player->started = true;
				player->colorDepth = 8;
				
				{	
		            unsigned cur_program_offset = 0, prev_program = 0;
		            bool prev_program_set = false;
		            if (player->stcChannel) 
					{
		                NEXUS_SimpleStcChannelSettings stcSettings;
						NEXUS_SimpleStcChannel_GetSettings(player->stcChannel, &stcSettings);
            			stcSettings.mode = NEXUS_StcChannelMode_eAuto;
            			stcSettings.modeSettings.pcr.pidChannel = NULL;
					    stcSettings.modeSettings.Auto.transportType = transportType;
						stcSettings.modeSettings.Auto.behavior = NEXUS_StcChannelAutoModeBehavior_eAudioMaster;
		                rc = NEXUS_SimpleStcChannel_SetSettings(player->stcChannel, &stcSettings);
		                if (rc) { rc = BERR_TRACE(rc); goto error; }
		            }

		            NEXUS_Playback_GetSettings(player->playback, &playbackSettings);
		            playbackSettings.playpumpSettings.transportType = transportType;
		            playbackSettings.playpumpSettings.timestamp.type = timestampType;
		            playbackSettings.endOfStreamAction = psettings->loopMode;
					playbackSettings.timeshifting = (psettings->loopMode == NEXUS_PlaybackLoopMode_ePlay);
					playbackSettings.stcTrick = psettings->stcTrick && player->stcChannel;
			        playbackSettings.startPaused = psettings->startPaused;
    				playbackSettings.frameReverse.gopDepth = psettings->dqtFrameReverse;
					rc = NEXUS_Playback_SetSettings(player->playback, &playbackSettings);
		            if (rc) { rc = BERR_TRACE(rc); goto error; }

		            for(track=BLST_SQ_FIRST(&player->stream->tracks);track;track=BLST_SQ_NEXT(track, link)) 
					{
		                if (track->type == bmedia_track_type_audio || track->type == bmedia_track_type_video) 
						{
		                    if (prev_program_set) 
							{
		                        if (track->program != prev_program) 
								{
		                            cur_program_offset++;
		                        }
		                    }
		                    prev_program = track->program;
		                    prev_program_set = true;
		                }
						
		                if (cur_program_offset != psettings->program) continue;

						switch(track->type) 
						{
		                    case bmedia_track_type_audio:
		                        if(track->info.audio.codec != baudio_format_unknown && player->audioDecoder) 
								{
		                            if (!player->audioProgram.primary.pidChannel) 
									{
		                                NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		                                playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
		                                playbackPidSettings.pidSettings.pidTypeSettings.audio.codec = b_audiocodec2nexus(track->info.audio.codec);
		                                playbackPidSettings.pidTypeSettings.audio.simpleDecoder = player->audioDecoder;
		                                player->audioProgram.primary.pidChannel = NEXUS_Playback_OpenPidChannel(player->playback, track->number, &playbackPidSettings);
		                                player->audioProgram.primary.codec = playbackPidSettings.pidSettings.pidTypeSettings.audio.codec;
		                            }
		                        }
		                        break;
		                    case bmedia_track_type_video:
		                        if (player->videoDecoder) 
								{
		                            if (track->info.video.codec == bvideo_codec_h264_svc || track->info.video.codec == bvideo_codec_h264_mvc) 
									{
		                                if (videoProgram.settings.pidChannel && !videoProgram.settings.enhancementPidChannel) 
										{
		                                    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		                                    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eVideo;
		                                    playbackPidSettings.pidTypeSettings.video.codec = b_videocodec2nexus(track->info.video.codec);
		                                    playbackPidSettings.pidTypeSettings.video.index = true;
		                                    playbackPidSettings.pidTypeSettings.video.simpleDecoder = player->videoDecoder;
		                                    videoProgram.settings.enhancementPidChannel = NEXUS_Playback_OpenPidChannel(player->playback, track->number, &playbackPidSettings);
		                                    videoProgram.settings.codec = b_videocodec2nexus(track->info.video.codec); /* overwrite */
		                                }
		                                break;
		                            }
		                            else if (track->info.video.codec != bvideo_codec_unknown) 
									{
		                                if (!videoProgram.settings.pidChannel) 
										{
		                                    NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		                                    playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eVideo;
		                                    playbackPidSettings.pidTypeSettings.video.codec = b_videocodec2nexus(track->info.video.codec);
		                                    playbackPidSettings.pidTypeSettings.video.index = true;
		                                    playbackPidSettings.pidTypeSettings.video.simpleDecoder = player->videoDecoder;
		                                    videoProgram.settings.pidChannel = NEXUS_Playback_OpenPidChannel(player->playback, track->number, &playbackPidSettings);
		                                    videoProgram.settings.codec = b_videocodec2nexus(track->info.video.codec);
		                                    videoProgram.settings.timestampMode = track->info.video.timestamp_order;
		                                    if (player->create_settings.maxWidth && player->create_settings.maxHeight) 
											{
		                                        videoProgram.maxWidth = player->create_settings.maxWidth;
		                                        videoProgram.maxHeight = player->create_settings.maxHeight;
		                                    }
											
		                                    if (track->info.video.width > videoProgram.maxWidth) 
											{
		                                        videoProgram.maxWidth = track->info.video.width;
		                                    }
											
		                                    if (track->info.video.height > videoProgram.maxHeight) 
											{
		                                        videoProgram.maxHeight = track->info.video.height;
		                                    }
											
		                                }
		                            }
									
		                            if (track->info.video.codec == bvideo_codec_h265) 
									{
		                                player->colorDepth = ((bmedia_probe_h265_video*)&track->info.video.codec_specific)->sps.bit_depth_luma;
									}
		                        }
		                        break;
						#if 0
		                        /* TODO: need playback to handle duplicate w/ different settings in the case of eOther */
		                    case bmedia_track_type_pcr:
		                        NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		                        playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eOther;
		                        player->pcrPidChannel = NEXUS_Playback_OpenPidChannel(player->playback, track->number, &playbackPidSettings);
		                        break;
						#endif
		                    default:
		                        break;
		                }
		            }

					#if B_HAS_AVI
		            if (player->stream->type == bstream_mpeg_type_avi && ((bavi_probe_stream *)player->stream)->video_framerate) 
					{
		                NEXUS_LookupFrameRate(((bavi_probe_stream *)player->stream)->video_framerate, &videoProgram.settings.frameRate);
		            }
					#endif

				    if (!videoProgram.settings.pidChannel && !player->audioProgram.primary.pidChannel) 
					{
				        BDBG_WRN(("no content found for program %d", psettings->program));
				        goto error;
				    }
					player->videoProgram = videoProgram;
    			}
			}
		}
	}
    
	player->videoProgram.displayEnabled = psettings->videoWindowType != NxClient_VideoWindowType_eNone;
    if (media_player_p_test_disconnect(player->master?player->master:player, &player->videoProgram)) 
	{
        media_player_p_disconnect(player);
    }

 	rc = MP_ConnectToNx(player);
	if (rc) {
		goto error;
	}

    /* crypto */
    if (player->start_settings.decrypt.algo < NEXUS_SecurityAlgorithm_eMax) 
	{
        struct dvr_crypto_settings settings;
        unsigned total = 0;
        dvr_crypto_get_default_settings(&settings);
        settings.algo = player->start_settings.decrypt.algo;
        if (player->videoProgram.settings.pidChannel) {
            settings.pid[total++] = player->videoProgram.settings.pidChannel;
        }
        if (player->audioProgram.primary.pidChannel) {
            settings.pid[total++] = player->audioProgram.primary.pidChannel;
        }
        player->crypto = dvr_crypto_create(&settings);
        /* if it fails, keep going */
    }

    /* apply codec settings */
    if (player->start_settings.audio.dolbyDrcMode < NEXUS_AudioDecoderDolbyDrcMode_eMax) {
        NEXUS_AudioDecoderCodecSettings settings;
        NEXUS_SimpleAudioDecoder_GetCodecSettings(player->audioDecoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, player->audioProgram.primary.codec, &settings);
        switch (player->audioProgram.primary.codec) {
        case NEXUS_AudioCodec_eAc3: settings.codecSettings.ac3.drcMode = player->start_settings.audio.dolbyDrcMode; break;
        case NEXUS_AudioCodec_eAc3Plus: settings.codecSettings.ac3Plus.drcMode = player->start_settings.audio.dolbyDrcMode; break;
        /* only line and rf applie for aac/aacplus, but nexus can validate params */
        case NEXUS_AudioCodec_eAac: settings.codecSettings.aac.drcMode = (NEXUS_AudioDecoderDolbyPulseDrcMode)player->start_settings.audio.dolbyDrcMode; break;
        case NEXUS_AudioCodec_eAacPlus: settings.codecSettings.aacPlus.drcMode = (NEXUS_AudioDecoderDolbyPulseDrcMode)player->start_settings.audio.dolbyDrcMode; break;
        default: /* just ignore */ break;
        }
        NEXUS_SimpleAudioDecoder_SetCodecSettings(player->audioDecoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &settings);
    }

    /* set StcChannel to all decoders before starting any */
    if (player->videoProgram.settings.pidChannel && player->stcChannel) {
        rc = NEXUS_SimpleVideoDecoder_SetStcChannel(player->videoDecoder, player->stcChannel);
        if (rc) { rc = BERR_TRACE(rc);goto error; }
    }
    if (player->audioProgram.primary.pidChannel) {
        rc = NEXUS_SimpleAudioDecoder_SetStcChannel(player->audioDecoder, player->stcChannel);
        if (rc) { rc = BERR_TRACE(rc); goto error; }
    }

    if (player->videoProgram.settings.pidChannel) {
        rc = NEXUS_SimpleVideoDecoder_Start(player->videoDecoder, &player->videoProgram);
        if (rc) { rc = BERR_TRACE(rc); goto error; }
    }
    if (player->audioProgram.primary.pidChannel) {
        rc = NEXUS_SimpleAudioDecoder_Start(player->audioDecoder, &player->audioProgram);
        if (rc) { rc = BERR_TRACE(rc); goto error; }
        /* decode may fail if audio codec not supported */
    }

	if (player->ipActive) 
	{
		media_player_ip_start_playback(player->ip);
		if (rc) { rc = BERR_TRACE(rc); goto error; }
	}
	else
    {	
        rc = NEXUS_Playback_Start(player->playback, player->file, NULL);
		if (rc) 
		{
	        if (psettings->index_url) 
			{
	            NEXUS_FilePlay_Close(player->file);
	            player->file = NEXUS_FilePlay_OpenPosix(url.path, NULL);
	            if (player->file) 
				{
	                rc = NEXUS_Playback_Start(player->playback, player->file, NULL);
	            }
	        }
        }
    }
    if (rc) { rc = BERR_TRACE(rc); goto error; }
	

    player->started = true;
	player->ccstatus = CCPREPARE;

	return 0;

error:
    if (player->probe) {
        if (player->stream) {
            bmedia_probe_stream_free(player->probe, player->stream);
            player->stream = NULL;
        }
        bmedia_probe_destroy(player->probe);
        player->probe = NULL;
    }
	
    media_player_stop(player);
    return -1;
}


int media_player_tuner_prepare(media_player_t handle,NEXUS_ParserBand* oparserBand)
{
	int iOutRet=ERROR_SUCCESS,iRet;
	NEXUS_ParserBand iparserBand;
	NEXUS_ParserBandSettings parserBandSettings;

	if(NULL == handle)
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	if(-1 != handle->parserBand)
	{
		if(oparserBand)
		{
			*oparserBand=handle->parserBand;
		}
		return 0;
	}
	
	iparserBand = NEXUS_ParserBand_Open(NEXUS_ANY_ID);
	if( NULL == iparserBand)
	{
		iOutRet = ERR_Nexus_OpenParserBand_Err;
		goto EXIT_PROC;	
	}
	
	NEXUS_ParserBand_GetSettings(iparserBand, &parserBandSettings);
	parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
	parserBandSettings.sourceTypeSettings.inputBand=NEXUS_InputBand_e10;
	parserBandSettings.transportType = NEXUS_TransportType_eTs;
	iRet = NEXUS_ParserBand_SetSettings(iparserBand, &parserBandSettings);
	if( ERROR_SUCCESS != iRet )
	{
		iOutRet= iRet;
		goto EXIT_PROC;
	}
	
	handle->parserBand=iparserBand;
	if(oparserBand)
	{
		*oparserBand=iparserBand;
	}
	
	return 0;

EXIT_PROC:

	if(handle->parserBand)
	{
		NEXUS_ParserBand_Close(iparserBand);
	}

	return iOutRet; 
}


int media_player_tuner_start( media_player_t player, const media_player_start_settings *psettings,tspsimgr_program *pprogram )
{
	int iOutRet=ERROR_SUCCESS,iRet;
	NEXUS_PlaybackPidChannelSettings playbackPidSettings;
	NEXUS_SimpleVideoDecoderStartSettings videoProgram;
	NEXUS_TransportType transportType = NEXUS_TransportType_eTs;
	NEXUS_TransportTimestampType timestampType = NEXUS_TransportTimestampType_eNone;
	NxClient_DisplaySettings displaySettings;
	TsScan tsinfo;
	
	BDBG_OBJECT_ASSERT(player, media_player);
    if (player->started) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }
	
    player->colorDepth = 8;
	player->ePlaySrcType=PLAY_SRC_Tuner;
	if(psettings)
	{
		player->start_settings = *psettings;
	}
	NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&videoProgram);
	videoProgram.displayEnabled = psettings->videoWindowType != NxClient_VideoWindowType_eNone;

	NEXUS_SimpleAudioDecoder_GetDefaultStartSettings(&player->audioProgram);
	NxClient_GetDisplaySettings(&displaySettings);

    if (displaySettings.hdmiPreferences.hdcp != psettings->hdcp) {
        displaySettings.hdmiPreferences.hdcp = psettings->hdcp;
        NxClient_SetDisplaySettings(&displaySettings);
    }

	player->videoProgram = videoProgram;

	iRet=media_player_tuner_prepare(player,NULL);
	if(iRet || -1 == player->parserBand)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	if( NULL == pprogram)
	{	
		int ipmtIndex=psettings->pmtIndex;
		int i=0,j=0,k=0;
		static int tid=-1,totalpg=0;
		
		{
			tsinfo.pid=0x0;
			tsinfo.parserBand=player->parserBand;
			tsinfo.tstype=TS_PAT;
			iRet=tspsimgr_scan_program(&tsinfo);
			if(ERROR_SUCCESS !=iRet)
			{
				iOutRet=ERR_Tuner_Scan_Program;
				goto EXIT_PROC;
			}
		}
		
		if(ipmtIndex <0 || ipmtIndex >= tsinfo.pRgProgram.num_programs)
		{
			ipmtIndex=0;
		}

		if(tid!=tsinfo.pRgProgram.transport_stream_id)
		{
			tid=tsinfo.pRgProgram.transport_stream_id;
			totalpg=0;
			memset(player->program_info,0,sizeof(player->program_info));
		}

		if(player->program_info[ipmtIndex].map_pid)
		{
			pprogram=&player->program_info[ipmtIndex];
			goto FOUND;
		}

		for(i=0;i<tsinfo.pRgProgram.num_programs;++i)
		{
			int issame=0;
			for(j=0;j<totalpg;++j)
			{
				if(player->program_info[j].program_number > tsinfo.pRgProgram.program_info[i].program_number)
				{
					for(k=totalpg-1;k>=j;k--)
					{
						player->program_info[k+1] = player->program_info[k];
					}
					break;
				}
				else if(player->program_info[j].program_number == tsinfo.pRgProgram.program_info[i].program_number)
				{
					issame=1;
					break;
				}
			}

			if(issame) continue;
			player->program_info[j] = tsinfo.pRgProgram.program_info[i];
			totalpg+=1;
		}

		if(totalpg<0)
		{
			iOutRet=ERR_Tuner_Scan_Program;
			goto EXIT_PROC;
		}

		if(ipmtIndex>totalpg-1)
		{
			ipmtIndex=0;
		}

		pprogram=&player->program_info[ipmtIndex];
		
	}

FOUND:

	if(!pprogram)
	{
		iOutRet=ERR_Tuner_Scan_Program;
		goto EXIT_PROC;
	}

	//BDBG_WRN(("pprogram->video_pids[0].pid:%d,pprogram->audio_pids[0].pid:%d\n",pprogram->video_pids[0].pid,pprogram->audio_pids[0].pid));
	if(pprogram->video_pids[0].pid)
	{
		NEXUS_VideoCodec icode=pprogram->video_pids[0].codec;
		if ( icode == NEXUS_VideoCodec_eH264_Svc || icode == NEXUS_VideoCodec_eH264_Mvc)
		{
			NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
            playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eVideo;
            playbackPidSettings.pidTypeSettings.video.codec = icode;
            playbackPidSettings.pidTypeSettings.video.index = true;
            playbackPidSettings.pidTypeSettings.video.simpleDecoder = player->videoDecoder;
            player->videoProgram.settings.enhancementPidChannel = NEXUS_PidChannel_Open(player->parserBand,pprogram->video_pids[0].pid,&playbackPidSettings);	
		}
		
		NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eVideo;
		playbackPidSettings.pidTypeSettings.video.codec = icode;
		playbackPidSettings.pidTypeSettings.video.index = true;
		playbackPidSettings.pidTypeSettings.video.simpleDecoder = player->videoDecoder;
		player->videoProgram.settings.pidChannel = NEXUS_PidChannel_Open(player->parserBand,pprogram->video_pids[0].pid,&playbackPidSettings);
		player->videoProgram.settings.codec = icode;
		player->videoProgram.settings.prerollRate = 2;
		player->videoProgram.smoothResolutionChange =true;
		player->videoProgram.lowDelayImageInput =true;
		player->videoProgram.settings.timestampMode=NEXUS_VideoDecoderTimestampMode_eDisplay;
		if (player->create_settings.maxWidth && player->create_settings.maxHeight) 
		{
			player->videoProgram.maxWidth = player->create_settings.maxWidth;
		 	player->videoProgram.maxHeight = player->create_settings.maxHeight;
		}
		if(!player->videoProgram.settings.pidChannel){iRet = BERR_TRACE(iRet);iOutRet = iRet;goto EXIT_PROC;}
	}

	if(pprogram->audio_pids[0].pid)
	{
		NEXUS_Playback_GetDefaultPidChannelSettings(&playbackPidSettings);
		playbackPidSettings.pidSettings.pidType = NEXUS_PidType_eAudio;
        playbackPidSettings.pidSettings.pidTypeSettings.audio.codec = pprogram->audio_pids[0].codec;
		player->audioProgram.primary.pidChannel = NEXUS_PidChannel_Open(player->parserBand,pprogram->audio_pids[0].pid,&playbackPidSettings);
		player->audioProgram.primary.codec = pprogram->audio_pids[0].codec;
		if(!player->audioProgram.primary.pidChannel){iRet = BERR_TRACE(iRet);iOutRet = iRet;goto EXIT_PROC;}
	}

	if(pprogram->pcr_pid){player->pcrPidChannel=NEXUS_PidChannel_Open(player->parserBand, pprogram->pcr_pid, NULL);}

	if (player->pcrPidChannel && player->stcChannel) 
	{
        NEXUS_SimpleStcChannelSettings stcSettings;
        NEXUS_SimpleStcChannel_GetSettings(player->stcChannel, &stcSettings);
        stcSettings.mode = NEXUS_StcChannelMode_ePcr;
        stcSettings.modeSettings.pcr.pidChannel = player->pcrPidChannel;
        NEXUS_SimpleStcChannel_SetSettings(player->stcChannel, &stcSettings);
    }

	player->videoProgram.displayEnabled = psettings->videoWindowType != NxClient_VideoWindowType_eNone;
    if (media_player_p_test_disconnect(player->master?player->master:player, &player->videoProgram)) 
	{
        media_player_p_disconnect(player);
    }

	iRet = MP_ConnectToNx(player);if (iRet) {iOutRet = iRet;goto EXIT_PROC;}

	if (player->start_settings.decrypt.algo < NEXUS_SecurityAlgorithm_eMax) {
        struct dvr_crypto_settings settings;
        unsigned total = 0;
        dvr_crypto_get_default_settings(&settings);
        settings.algo = player->start_settings.decrypt.algo;
        if (player->videoProgram.settings.pidChannel) {
            settings.pid[total++] = player->videoProgram.settings.pidChannel;
        }
        if (player->audioProgram.primary.pidChannel) {
            settings.pid[total++] = player->audioProgram.primary.pidChannel;
        }
        player->crypto = dvr_crypto_create(&settings);
 	}
	
	if (player->start_settings.audio.dolbyDrcMode < NEXUS_AudioDecoderDolbyDrcMode_eMax) 
	{
	    NEXUS_AudioDecoderCodecSettings settings;
	    NEXUS_SimpleAudioDecoder_GetCodecSettings(player->audioDecoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, player->audioProgram.primary.codec, &settings);
	    switch (player->audioProgram.primary.codec) {
	    case NEXUS_AudioCodec_eAc3: settings.codecSettings.ac3.drcMode = player->start_settings.audio.dolbyDrcMode; break;
	    case NEXUS_AudioCodec_eAc3Plus: settings.codecSettings.ac3Plus.drcMode = player->start_settings.audio.dolbyDrcMode; break;
	    case NEXUS_AudioCodec_eAac: settings.codecSettings.aac.drcMode = (NEXUS_AudioDecoderDolbyPulseDrcMode)player->start_settings.audio.dolbyDrcMode; break;
	    case NEXUS_AudioCodec_eAacPlus: settings.codecSettings.aacPlus.drcMode = (NEXUS_AudioDecoderDolbyPulseDrcMode)player->start_settings.audio.dolbyDrcMode; break;
	    default:break;
	    }
	    NEXUS_SimpleAudioDecoder_SetCodecSettings(player->audioDecoder, NEXUS_SimpleAudioDecoderSelector_ePrimary, &settings);
	}

	if (player->videoProgram.settings.pidChannel) {
        iRet = NEXUS_SimpleVideoDecoder_SetStcChannel(player->videoDecoder, player->stcChannel);
        if (iRet) { iRet = BERR_TRACE(iRet); iOutRet = iRet;goto EXIT_PROC; }
		iRet = NEXUS_SimpleVideoDecoder_Start(player->videoDecoder,&player->videoProgram);
		if (iRet) { iRet = BERR_TRACE(iRet);iOutRet = iRet;goto EXIT_PROC; }
    }
	
    if (player->audioProgram.primary.pidChannel) {
        iRet = NEXUS_SimpleAudioDecoder_SetStcChannel(player->audioDecoder, player->stcChannel);
        if (iRet) { iRet = BERR_TRACE(iRet); iOutRet = iRet;goto EXIT_PROC; }
		iRet = NEXUS_SimpleAudioDecoder_Start(player->audioDecoder,&player->audioProgram);
	    if (iRet) { iRet = BERR_TRACE(iRet);iOutRet = iRet;goto EXIT_PROC;}
	}
	
	player->started = true;
	
    return 0;

EXIT_PROC:

	if (player->parserBand != -1)
	{
        NEXUS_ParserBand_Close(player->parserBand);
		player->parserBand=-1;
	}
	
	media_player_stop(player);
	
	return iOutRet;
	
}

int media_get_status(media_player_t player)
{
	int rc = -1;
	if(!player)
		return;
	rc = player->ccstatus;	
	return rc;
}

void media_player_stop_cc(media_player_t player)
{
	if(!player)
		return;
	player->ccstatus=CCSTOP;
	if(player->playbackcc)
	{
		pthread_join(player->playbackcc, NULL);
	}
	player->playbackcc=NULL;
}

void media_player_stop(media_player_t player)
{
    BDBG_OBJECT_ASSERT(player, media_player);
	
	if(player->isShowImage){show_image_stop(player);} /*abner*/
	
	if(NULL != player->hdmiInput)	/*HDMI input*/
	{
	    if (!player->started) return;
		
   	    if (player->videoDecoder) {
	        NEXUS_SimpleVideoDecoder_Stop(player->videoDecoder);
	        NEXUS_SimpleVideoDecoder_SetStcChannel(player->videoDecoder, NULL);
	    }
	    if (player->audioDecoder) {
	        NEXUS_SimpleAudioDecoder_Stop(player->audioDecoder);
	        NEXUS_SimpleAudioDecoder_SetStcChannel(player->audioDecoder, NULL);
	    }
		NEXUS_HdmiInput_Close(player->hdmiInput);
		player->hdmiInput=NULL;
		player->started = false;
	}
	else if(PLAY_SRC_Tuner == player->ePlaySrcType )
	{
		if (!player->started) return;
		
		if (player->videoDecoder) 
		{
			NEXUS_SimpleVideoDecoder_Stop(player->videoDecoder);
			NEXUS_SimpleVideoDecoder_SetStcChannel(player->videoDecoder, NULL);
		}

		if (player->audioDecoder) 
		{
			NEXUS_SimpleAudioDecoder_Stop(player->audioDecoder);
			NEXUS_SimpleAudioDecoder_SetStcChannel(player->audioDecoder, NULL);
		}
		
		if (player->crypto) 
		{
			dvr_crypto_destroy(player->crypto);
		}
	
		if(player->videoProgram.settings.pidChannel)
		{
			NEXUS_PidChannel_Close(player->videoProgram.settings.pidChannel);
			player->videoProgram.settings.pidChannel = NULL;
		}
		
		
		if(player->audioProgram.primary.pidChannel)
		{
			NEXUS_PidChannel_Close(player->audioProgram.primary.pidChannel);
			player->audioProgram.primary.pidChannel = NULL;
		}
		
		if (player->stcChannel) 
		{
			//NEXUS_SimpleStcChannel_Destroy(player->stcChannel);
			NEXUS_SimpleStcChannelSettings stcSettings;
            NEXUS_SimpleStcChannel_GetSettings(player->stcChannel, &stcSettings);
            stcSettings.mode = NEXUS_StcChannelMode_eAuto;
            NEXUS_SimpleStcChannel_SetSettings(player->stcChannel, &stcSettings);	
			//player->stcChannel = NULL;
		}
		
		if (player->pcrPidChannel) 
		{
			NEXUS_PidChannel_Close(player->pcrPidChannel);
			player->pcrPidChannel = NULL;
		}
		
		if(-1 != player->parserBand)
		{
			NEXUS_ParserBand_Close(player->parserBand);
			player->parserBand=-1;
		}
		player->started = false;
	}
	else	/*media stream*/
	{
		if (!player->started) return;
		
		if (player->videoDecoder) {
			NEXUS_SimpleVideoDecoder_Stop(player->videoDecoder);
			NEXUS_SimpleVideoDecoder_SetStcChannel(player->videoDecoder, NULL);
		}
		
		if (player->audioDecoder) {
			NEXUS_SimpleAudioDecoder_Stop(player->audioDecoder);
			NEXUS_SimpleAudioDecoder_SetStcChannel(player->audioDecoder, NULL);
		}
		
		if (player->crypto) {
			dvr_crypto_destroy(player->crypto);
		}
		
		if (player->ipActive) 
		{
				media_player_ip_stop(player->ip);
				player->ipActive = false;
		}
		else
		{
			/* regular file playback case */
			if (player->playback) {
				NEXUS_Playback_Stop(player->playback);
				if (player->videoProgram.settings.pidChannel) {
					NEXUS_Playback_ClosePidChannel(player->playback, player->videoProgram.settings.pidChannel);
					player->videoProgram.settings.pidChannel = NULL;
				}
				if (player->audioProgram.primary.pidChannel) {
					NEXUS_Playback_ClosePidChannel(player->playback, player->audioProgram.primary.pidChannel);
					player->audioProgram.primary.pidChannel = NULL;
				}
				if (player->pcrPidChannel) {
					NEXUS_Playback_ClosePidChannel(player->playback, player->pcrPidChannel);
					player->pcrPidChannel = NULL;
				}
			}
			
			if (player->file) {
				NEXUS_FilePlay_Close(player->file);
			}

			if (player->probe) {
				if (player->stream) {
					bmedia_probe_stream_free(player->probe, player->stream);
					player->stream = NULL;
				}
				bmedia_probe_destroy(player->probe);
			}
		}
		player->started = false;
	}
	player->ePlaySrcType = PLAY_SRC_UNKNOWN;
}

void media_player_destroy(media_player_t player)
{
    bool uninit;
    BDBG_OBJECT_ASSERT(player, media_player);

    uninit = !player->master || !BLST_Q_FIRST(&player->master->players);
    if (player->started) {
        media_player_stop(player);
        if (player->connectId) {
            media_player_p_disconnect(player->master?player->master:player);
        }
    }
    if (player->playback) {
        NEXUS_Playback_Destroy(player->playback);
    }
    if (player->playpump) {
        NEXUS_Playpump_Close(player->playpump);
    }
    if (player->videoDecoder) {
        NEXUS_SimpleVideoDecoder_Release(player->videoDecoder);
		player->videoDecoder = NULL;
    }
    if (player->audioDecoder) {
        NEXUS_SimpleAudioDecoder_Release(player->audioDecoder);
    }
    if (player->stcChannel) {
        NEXUS_SimpleStcChannel_Destroy(player->stcChannel);
		player->stcChannel = NULL;
    }

    if (player->ip) 
	{
    	media_player_ip_destroy(player->ip);
    }


	if(NULL != player->hVideoSurfaceClient)
	{
		NEXUS_SurfaceClient_Release(player->hVideoSurfaceClient);
		player->hVideoSurfaceClient = NULL;
	}

	if(NULL != player->hSurfaceClient)
	{
		NEXUS_SurfaceClient_Release(player->hSurfaceClient);
		player->hSurfaceClient = NULL;
	}
	/* obj mutex */
	if(player->hMutex)
	{
		CErrChkMutex_delete(player->hMutex);
		player->hMutex = NULL;
	}
	/* */
    if (uninit) {
        NxClient_Free(&player->allocResults);
    }
    BDBG_OBJECT_DESTROY(player, media_player);
    free(player);
    if (uninit) {
        NxClient_Uninit();
    }
}

int media_player_trick( media_player_t player, int rate)
{
    BDBG_OBJECT_ASSERT(player, media_player);
    if (!player->started) {
        return BERR_TRACE(NEXUS_NOT_AVAILABLE);
    }
	
    if (player->ipActive) 
	{
    	return media_ip_player_trick(player->ip, rate);
    }

    if (rate == NEXUS_NORMAL_DECODE_RATE) {
        return NEXUS_Playback_Play(player->playback);
    }
    else if (rate == 0) {
        return NEXUS_Playback_Pause(player->playback);
    }
    else {
        NEXUS_PlaybackTrickModeSettings settings;
        NEXUS_Playback_GetDefaultTrickModeSettings(&settings);
        settings.rate = rate;
        return NEXUS_Playback_TrickMode(player->playback, &settings);
    }
}

int media_player_get_playback_status( media_player_t player, NEXUS_PlaybackStatus *pstatus )
{
    if (player->ipActive) 
	{
    	return media_player_ip_get_playback_status(player->ip, pstatus);
    }
    return NEXUS_Playback_GetStatus(player->playback, pstatus);
}

NEXUS_SimpleVideoDecoderHandle media_player_get_video_decoder(media_player_t player)
{
    return player->videoDecoder;
}

/*=================add cc=======================*/

typedef struct CCTest_Caption_Triplets
{
    uint8_t ccType;
    uint8_t data[2];
} CCTest_Caption_Triplets;

struct cccontext {
    NEXUS_SurfaceHandle framebuffer1;
    NEXUS_SurfaceClientHandle surfaceClient;
    BKNI_EventHandle displayedEvent;
};


enum CCTest_Display_Modes
{
    CCTest_Display_480i,
    CCTest_Display_480p,
    CCTest_Display_720p,
    CCTest_Display_1080i,
    CCTest_Display_Max_Size
};

uint32_t g_DisplayMode = CCTest_Display_480i;
static const uint32_t gDisplayWidth[4]   = {720, 720, 1280, 1920};
static const uint32_t gDisplayHeight[4]  = {480, 480, 720, 1080};
static const uint32_t gCaptionColumns[4] = {32, 32, 42, 42};


static NEXUS_SurfaceHandle client_flip(void *context)
{
    struct cccontext *pContext = context;
    NEXUS_SurfaceClient_SetSurface(pContext->surfaceClient, pContext->framebuffer1);
    return pContext->framebuffer1;
}

static void client_get_framebuffer_dimensions(void *context, NEXUS_VideoFormat *format, unsigned *width, unsigned *height)
{
    NEXUS_SurfaceCreateSettings surfaceCreateSettings;
    struct cccontext *pContext = context;
    NEXUS_Surface_GetCreateSettings(pContext->framebuffer1, &surfaceCreateSettings);
    *width = surfaceCreateSettings.width;
    *height = surfaceCreateSettings.height;
    *format = NEXUS_VideoFormat_eNtsc;
}

static void loadFonts( BDCC_WINLIB_Handle winlib, uint32_t *scaleFactor )
{
	int i, j, iErr;
	BDCC_FONT_DESCRIPTOR *pFont=NULL;

	BDCC_FONT_DESCRIPTOR standardFonts[ BDCC_FontStyle_Max_Value ][BDCC_PenSize_Max_Size];
	BDCC_FONT_DESCRIPTOR italicsFonts [ BDCC_FontStyle_Max_Value ][BDCC_PenSize_Max_Size];
	char *pszStandardFontFilenames[BDCC_FontStyle_Max_Value][ CCTest_Display_Max_Size ][ BDCC_PenSize_Max_Size ]=
	{
	  /* non-italic */
	  /* 480i S M L                                                                          480p S M L                                                           720p S M L                                   1080i S M L */

	  {{ "cc_fonts/cinecavB14_type.bwin_font",   "cc_fonts/cinecavB18_type.bwin_font",   "cc_fonts/cinecavB22_type.bwin_font"   }, { "cc_fonts/cinecavB14_type.bwin_font",   "cc_fonts/cinecavB18_type.bwin_font",   "cc_fonts/cinecavB22_type.bwin_font"   }, { "cc_fonts/cinecavB21_type.bwin_font",   "cc_fonts/cinecavB28_type.bwin_font",   "cc_fonts/cinecavB33_type.bwin_font"   }, { "cc_fonts/cinecavB32_type.bwin_font",   "cc_fonts/cinecavB41_type.bwin_font",   "cc_fonts/cinecavB49_type.bwin_font"   }}, /* BDCC_FontStyle_Default */
	  {{ "cc_fonts/cinecavB14_type.bwin_font",   "cc_fonts/cinecavB18_type.bwin_font",   "cc_fonts/cinecavB22_type.bwin_font"   }, { "cc_fonts/cinecavB14_type.bwin_font",   "cc_fonts/cinecavB18_type.bwin_font",   "cc_fonts/cinecavB22_type.bwin_font"   }, { "cc_fonts/cinecavB21_type.bwin_font",   "cc_fonts/cinecavB28_type.bwin_font",   "cc_fonts/cinecavB33_type.bwin_font"   }, { "cc_fonts/cinecavB32_type.bwin_font",   "cc_fonts/cinecavB41_type.bwin_font",   "cc_fonts/cinecavB49_type.bwin_font"   }}, /* BDCC_FontStyle_MonoSerifs */
	  {{ "cc_fonts/cinecavB12_serif.bwin_font",  "cc_fonts/cinecavB15_serif.bwin_font",  "cc_fonts/cinecavB19_serif.bwin_font"  }, { "cc_fonts/cinecavB12_serif.bwin_font",  "cc_fonts/cinecavB15_serif.bwin_font",  "cc_fonts/cinecavB19_serif.bwin_font"  }, { "cc_fonts/cinecavB18_serif.bwin_font",  "cc_fonts/cinecavB23_serif.bwin_font",  "cc_fonts/cinecavB29_serif.bwin_font"  }, { "cc_fonts/cinecavB28_serif.bwin_font",  "cc_fonts/cinecavB35_serif.bwin_font",  "cc_fonts/cinecavB44_serif.bwin_font"  }}, /* BDCC_FontStyle_PropSerifs */
	  {{ "cc_fonts/cinecavB14_mono.bwin_font",   "cc_fonts/cinecavB18_mono.bwin_font",   "cc_fonts/cinecavB22_mono.bwin_font"   }, { "cc_fonts/cinecavB14_mono.bwin_font",   "cc_fonts/cinecavB18_mono.bwin_font",   "cc_fonts/cinecavB22_mono.bwin_font"   }, { "cc_fonts/cinecavB21_mono.bwin_font",   "cc_fonts/cinecavB29_mono.bwin_font",   "cc_fonts/cinecavB34_mono.bwin_font"   }, { "cc_fonts/cinecavB34_mono.bwin_font",   "cc_fonts/cinecavB43_mono.bwin_font",   "cc_fonts/cinecavB52_mono.bwin_font"   }}, /* BDCC_FontStyle_Mono  */
	  {{ "cc_fonts/cinecavB12_sans.bwin_font",   "cc_fonts/cinecavB15_sans.bwin_font",   "cc_fonts/cinecavB19_sans.bwin_font"   }, { "cc_fonts/cinecavB12_sans.bwin_font",   "cc_fonts/cinecavB15_sans.bwin_font",   "cc_fonts/cinecavB19_sans.bwin_font"   }, { "cc_fonts/cinecavB18_sans.bwin_font",   "cc_fonts/cinecavB23_sans.bwin_font",   "cc_fonts/cinecavB29_sans.bwin_font"   }, { "cc_fonts/cinecavB28_sans.bwin_font",   "cc_fonts/cinecavB35_sans.bwin_font",   "cc_fonts/cinecavB44_sans.bwin_font"   }}, /* BDCC_FontStyle_Prop  */
	  {{ "cc_fonts/cinecavB13_casual.bwin_font", "cc_fonts/cinecavB16_casual.bwin_font", "cc_fonts/cinecavB20_casual.bwin_font" }, { "cc_fonts/cinecavB13_casual.bwin_font", "cc_fonts/cinecavB16_casual.bwin_font", "cc_fonts/cinecavB20_casual.bwin_font" }, { "cc_fonts/cinecavB19_casual.bwin_font", "cc_fonts/cinecavB24_casual.bwin_font", "cc_fonts/cinecavB30_casual.bwin_font" }, { "cc_fonts/cinecavB30_casual.bwin_font", "cc_fonts/cinecavB36_casual.bwin_font", "cc_fonts/cinecavB45_casual.bwin_font" }}, /* BDCC_FontStyle_Casual  */
	  {{ "cc_fonts/cinecavB12_script.bwin_font", "cc_fonts/cinecavB14_script.bwin_font", "cc_fonts/cinecavB18_script.bwin_font" }, { "cc_fonts/cinecavB12_script.bwin_font", "cc_fonts/cinecavB14_script.bwin_font", "cc_fonts/cinecavB18_script.bwin_font" }, { "cc_fonts/cinecavB16_script.bwin_font", "cc_fonts/cinecavB18_script.bwin_font", "cc_fonts/cinecavB25_script.bwin_font" }, { "cc_fonts/cinecavB25_script.bwin_font", "cc_fonts/cinecavB28_script.bwin_font", "cc_fonts/cinecavB38_script.bwin_font" }}, /* BDCC_FontStyle_Cursive */
	  {{ "cc_fonts/cinecavB13_sc.bwin_font",     "cc_fonts/cinecavB15_sc.bwin_font",     "cc_fonts/cinecavB19_sc.bwin_font"     }, { "cc_fonts/cinecavB13_sc.bwin_font",     "cc_fonts/cinecavB15_sc.bwin_font",     "cc_fonts/cinecavB19_sc.bwin_font"     }, { "cc_fonts/cinecavB18_sc.bwin_font",     "cc_fonts/cinecavB23_sc.bwin_font",     "cc_fonts/cinecavB30_sc.bwin_font"     }, { "cc_fonts/cinecavB28_sc.bwin_font",     "cc_fonts/cinecavB35_sc.bwin_font",     "cc_fonts/cinecavB45_sc.bwin_font"     }}  /* BDCC_FontStyle_SmallCaps */
	};


	char *pszItalicFontFilenames[BDCC_FontStyle_Max_Value][ CCTest_Display_Max_Size ][ BDCC_PenSize_Max_Size ]=
	{
	  /* italic */

	  {{ "cc_fonts/cinecavB14i_type.bwin_font",   "cc_fonts/cinecavB18i_type.bwin_font",   "cc_fonts/cinecavB22i_type.bwin_font"   }, { "cc_fonts/cinecavB14i_type.bwin_font",   "cc_fonts/cinecavB18i_type.bwin_font",   "cc_fonts/cinecavB22i_type.bwin_font"   }, { "cc_fonts/cinecavB21i_type.bwin_font",   "cc_fonts/cinecavB28i_type.bwin_font",   "cc_fonts/cinecavB33i_type.bwin_font"   }, { "cc_fonts/cinecavB32i_type.bwin_font",   "cc_fonts/cinecavB41i_type.bwin_font",   "cc_fonts/cinecavB49i_type.bwin_font"   }}, /* BDCC_FontStyle_Default */
	  {{ "cc_fonts/cinecavB14i_type.bwin_font",   "cc_fonts/cinecavB18i_type.bwin_font",   "cc_fonts/cinecavB22i_type.bwin_font"   }, { "cc_fonts/cinecavB14i_type.bwin_font",   "cc_fonts/cinecavB18i_type.bwin_font",   "cc_fonts/cinecavB22i_type.bwin_font"   }, { "cc_fonts/cinecavB21i_type.bwin_font",   "cc_fonts/cinecavB28i_type.bwin_font",   "cc_fonts/cinecavB33i_type.bwin_font"   }, { "cc_fonts/cinecavB32i_type.bwin_font",   "cc_fonts/cinecavB41i_type.bwin_font",   "cc_fonts/cinecavB49i_type.bwin_font"   }}, /* BDCC_FontStyle_MonoSerifs */
	  {{ "cc_fonts/cinecavB12i_serif.bwin_font",  "cc_fonts/cinecavB15i_serif.bwin_font",  "cc_fonts/cinecavB19i_serif.bwin_font"  }, { "cc_fonts/cinecavB12i_serif.bwin_font",  "cc_fonts/cinecavB15i_serif.bwin_font",  "cc_fonts/cinecavB19i_serif.bwin_font"  }, { "cc_fonts/cinecavB18i_serif.bwin_font",  "cc_fonts/cinecavB23i_serif.bwin_font",  "cc_fonts/cinecavB29i_serif.bwin_font"  }, { "cc_fonts/cinecavB28i_serif.bwin_font",  "cc_fonts/cinecavB35i_serif.bwin_font",  "cc_fonts/cinecavB44i_serif.bwin_font"  }}, /* BDCC_FontStyle_PropSerifs */
	  {{ "cc_fonts/cinecavB14i_mono.bwin_font",   "cc_fonts/cinecavB18i_mono.bwin_font",   "cc_fonts/cinecavB22i_mono.bwin_font"   }, { "cc_fonts/cinecavB14i_mono.bwin_font",   "cc_fonts/cinecavB18i_mono.bwin_font",   "cc_fonts/cinecavB22i_mono.bwin_font"   }, { "cc_fonts/cinecavB21i_mono.bwin_font",   "cc_fonts/cinecavB29i_mono.bwin_font",   "cc_fonts/cinecavB34i_mono.bwin_font"   }, { "cc_fonts/cinecavB34i_mono.bwin_font",   "cc_fonts/cinecavB43i_mono.bwin_font",   "cc_fonts/cinecavB52i_mono.bwin_font"   }}, /* BDCC_FontStyle_Mono  */
	  {{ "cc_fonts/cinecavB12i_sans.bwin_font",   "cc_fonts/cinecavB15i_sans.bwin_font",   "cc_fonts/cinecavB19i_sans.bwin_font"   }, { "cc_fonts/cinecavB12i_sans.bwin_font",   "cc_fonts/cinecavB15i_sans.bwin_font",   "cc_fonts/cinecavB19i_sans.bwin_font"   }, { "cc_fonts/cinecavB18i_sans.bwin_font",   "cc_fonts/cinecavB23i_sans.bwin_font",   "cc_fonts/cinecavB29i_sans.bwin_font"   }, { "cc_fonts/cinecavB28i_sans.bwin_font",   "cc_fonts/cinecavB35i_sans.bwin_font",   "cc_fonts/cinecavB44i_sans.bwin_font"   }}, /* BDCC_FontStyle_Prop  */
	  {{ "cc_fonts/cinecavB13i_casual.bwin_font", "cc_fonts/cinecavB16i_casual.bwin_font", "cc_fonts/cinecavB20i_casual.bwin_font" }, { "cc_fonts/cinecavB13i_casual.bwin_font", "cc_fonts/cinecavB16i_casual.bwin_font", "cc_fonts/cinecavB20i_casual.bwin_font" }, { "cc_fonts/cinecavB19i_casual.bwin_font", "cc_fonts/cinecavB24i_casual.bwin_font", "cc_fonts/cinecavB30i_casual.bwin_font" }, { "cc_fonts/cinecavB30i_casual.bwin_font", "cc_fonts/cinecavB36i_casual.bwin_font", "cc_fonts/cinecavB45i_casual.bwin_font" }}, /* BDCC_FontStyle_Casual  */
	  {{ "cc_fonts/cinecavB12i_script.bwin_font", "cc_fonts/cinecavB14i_script.bwin_font", "cc_fonts/cinecavB18i_script.bwin_font" }, { "cc_fonts/cinecavB12i_script.bwin_font", "cc_fonts/cinecavB14i_script.bwin_font", "cc_fonts/cinecavB18i_script.bwin_font" }, { "cc_fonts/cinecavB16i_script.bwin_font", "cc_fonts/cinecavB18i_script.bwin_font", "cc_fonts/cinecavB25i_script.bwin_font" }, { "cc_fonts/cinecavB25i_script.bwin_font", "cc_fonts/cinecavB28i_script.bwin_font", "cc_fonts/cinecavB38i_script.bwin_font" }}, /* BDCC_FontStyle_Cursive */
	  {{ "cc_fonts/cinecavB13i_sc.bwin_font",     "cc_fonts/cinecavB15i_sc.bwin_font",     "cc_fonts/cinecavB19i_sc.bwin_font"     }, { "cc_fonts/cinecavB13i_sc.bwin_font",     "cc_fonts/cinecavB15i_sc.bwin_font",     "cc_fonts/cinecavB19i_sc.bwin_font"     }, { "cc_fonts/cinecavB18i_sc.bwin_font",     "cc_fonts/cinecavB23i_sc.bwin_font",     "cc_fonts/cinecavB30i_sc.bwin_font"     }, { "cc_fonts/cinecavB28i_sc.bwin_font",     "cc_fonts/cinecavB35i_sc.bwin_font",     "cc_fonts/cinecavB45i_sc.bwin_font"     }}  /* BDCC_FontStyle_SmallCaps */
	};

	/* we do not need to scale these fonts using the m2mc since we have pre-rendered a complete set of 126!! */
	*scaleFactor = 100;  /* scale factor in hundredths i.e. scale factor is 1.0 */

	BKNI_Memset( standardFonts, 0, ( sizeof( BDCC_FONT_DESCRIPTOR ) * BDCC_FontStyle_Max_Value ));
	BKNI_Memset( italicsFonts , 0, ( sizeof( BDCC_FONT_DESCRIPTOR ) * BDCC_FontStyle_Max_Value ));

	for ( i = 0; i < BDCC_FontStyle_Max_Value; i++){
	    for ( j = 0; j < BDCC_PenSize_Max_Size; j++){

	        standardFonts[ i ][ j ].pszFontFile = pszStandardFontFilenames[ i ][ g_DisplayMode ][ j ];
	        standardFonts[ i ][ j ].iFontSize = -1;

	        italicsFonts[ i ][ j ].pszFontFile = pszItalicFontFilenames[ i ][ g_DisplayMode ][ j ];
	        italicsFonts[ i ][ j ].iFontSize = -1;
	    }
	}

	for ( i=0; i < BDCC_FontStyle_Max_Value; i++ )
	{
	    for(j=0; j < BDCC_PenSize_Max_Size; j++)
	    {
	        pFont = &standardFonts[ i ][ j ];

	        if (pFont && pFont->pszFontFile)
	        {
	            iErr = BDCC_WINLIB_LoadFont( winlib, pFont->pszFontFile, pFont->iFontSize, i, j, BDCC_PenStyle_Standard );
	            printf("%s %s\n", iErr ? "\tFAILED to load font!!!:" : "\tloaded font:", pFont->pszFontFile );
	        }

	        pFont = &italicsFonts[ i ][ j ];

	        if(pFont && pFont->pszFontFile) /* need not specify all fonts, minimum is the DEFAULT font */
	        {
	            iErr = BDCC_WINLIB_LoadFont( winlib, pFont->pszFontFile, pFont->iFontSize, i, j, BDCC_PenStyle_Italics );
				printf("%s %s\n", iErr ? "\tFAILED to load font!!!:" : "\tloaded font:", pFont->pszFontFile );
	        }
	    }
	}
	printf("\n");
}

int ColseCapture(media_player_t player, const media_player_start_settings *psettings,int ccSertype)
{
	int iRet=ERR_SUCCESS;
	player->ccSertype=ccSertype;
	iRet=pthread_create(&player->playbackcc, NULL, colseCaptureThread, player);
	
	return iRet;
}

int colseCaptureThread(void *pParam)
{
	int rc;
	NEXUS_SurfaceCreateSettings 	surfaceCreateSettings;
	NEXUS_SurfaceMemory	 			surfmem1;
	BDCC_WINLIB_Interface 			winlibInterface;
	B_Dcc_Settings 					ccEngineSettings;
	uint32_t 						scaleFactor;
	BDCC_WINLIB_OpenSettings 		openSettings;
	B_Dcc_Type 						ccMode 				=B_Dcc_Type_e708;
	B_Dcc_Handle 					ccEngine			=NULL;
	buserdata_t  					userdata			=NULL;
	BDCC_WINLIB_Handle 				winlib				=NULL;
	int 							iccSertype			=0;
	int 							icount   			=0;
	media_player_t 					player				=pParam;
	NEXUS_SurfaceClientSettings 	surfaceClientSettings;
	
	struct cccontext iContext,*pContext = &iContext;	
	if(!player&&!player->hSurfaceClient)
	{
		return -1;
	}

	player->ccstatus = CCSTART;	
	pContext->surfaceClient = player->hSurfaceClient;
	NEXUS_SurfaceClient_GetSettings(pContext->surfaceClient, &surfaceClientSettings);
    surfaceClientSettings.displayed.callback = NULL;
    surfaceClientSettings.displayed.context = NULL;
    rc = NEXUS_SurfaceClient_SetSettings(pContext->surfaceClient, &surfaceClientSettings);
	
	iccSertype = player->ccSertype;
	switch(iccSertype)
	{
	case 0:
		player->ccstatus=CCSTOP;	
		return 0;
	case 5:
		ccMode = B_Dcc_Type_e708;
		iccSertype = 1;
		break;
	
	default:
		ccMode = B_Dcc_Type_e608;
		break;	
	}

	NEXUS_Surface_GetDefaultCreateSettings( &surfaceCreateSettings );
	surfaceCreateSettings.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
	surfaceCreateSettings.width = 720;
	surfaceCreateSettings.height = 480;
	pContext->framebuffer1 =  NEXUS_Surface_Create( &surfaceCreateSettings );
	NEXUS_Surface_GetMemory(pContext->framebuffer1, &surfmem1);
	BKNI_Memset(surfmem1.buffer, 0x00, surfaceCreateSettings.height * surfmem1.pitch);
	NEXUS_Surface_Flush(pContext->framebuffer1);

	BDCC_WINLIB_GetDefaultOpenSettings(&openSettings);
	openSettings.flip = client_flip;
	openSettings.get_framebuffer_dimensions = client_get_framebuffer_dimensions;
	openSettings.context = pContext;
	openSettings.framebufferWidth = surfaceCreateSettings.width;
	openSettings.framebufferHeight = surfaceCreateSettings.height;
	rc = BDCC_WINLIB_Open(&winlib, &openSettings);
	if (rc) { rc = BERR_TRACE(rc); goto PROC_EXIT; }

	BDCC_WINLIB_GetInterface( &winlibInterface );
	loadFonts( winlib, &scaleFactor );
	rc = B_Dcc_Open( &ccEngine, winlib, &winlibInterface );
	if (rc) { rc = BERR_TRACE(rc); goto PROC_EXIT; }

	B_Dcc_GetDefaultSettings( &ccEngineSettings );
	ccEngineSettings.iSafeTitleX = gDisplayWidth[g_DisplayMode] / 10; /* 20% of width / 2 according to CEB-10 */;
	ccEngineSettings.iSafeTitleY = gDisplayHeight[g_DisplayMode] / 10; /* 20% of height / 2 according to CEB-10 */;
	ccEngineSettings.ScaleFactor = scaleFactor;
	ccEngineSettings.Columns	 = gCaptionColumns[g_DisplayMode]; /* 32 for 4:3 42 for 16:9 */;
	B_Dcc_SetSettings( ccEngine, &ccEngineSettings );

	rc = B_Dcc_Init( ccEngine, iccSertype, ccMode);
	if (rc) { rc = BERR_TRACE(rc); goto PROC_EXIT; }

	userdata = buserdata_create(NULL);
	
	while (1)
	{
		#define CC_DATA_BUF_SIZE 128
		NEXUS_ClosedCaptionData captionData[ CC_DATA_BUF_SIZE ];
		CCTest_Caption_Triplets cc708UserData[ CC_DATA_BUF_SIZE ];
		uint32_t numEntries, numValidEntries, i;

		if(CCSTOP == player->ccstatus)
		{
			break;
		}
		rc = buserdata_parse(userdata, player->videoDecoder, captionData, CC_DATA_BUF_SIZE, &numEntries);
		if (rc || !numEntries) 
		{
			if (rc) BERR_TRACE(rc);
			BKNI_Sleep(30);
			icount++;
			if(icount >= 20) 
				break;
			continue;
		}

		icount = 0;
		
		for ( i=0, numValidEntries = 0; i < numEntries; i++ )
		{
			/* bit 1 of 'field' should be set for 708 data, clear for 608 data */
			if(((( captionData[ i ].field & 0x2) == 2 ) ^ ( B_Dcc_Type_e608 == ccMode )) && !captionData[i].noData)
			{
				cc708UserData[ numValidEntries ].ccType  = (uint8_t)(captionData[ i ].field);
				cc708UserData[ numValidEntries ].data[0] = captionData[ i ].data[0];
				cc708UserData[ numValidEntries ].data[1] = captionData[ i ].data[1];
				numValidEntries++;
			}
		}

		if( B_Dcc_Process(ccEngine, (unsigned char *)cc708UserData, numValidEntries)) 
		{
			printf("B_Dcc_Process returned with an Error\n");
			break;
		}
	}

PROC_EXIT:

	BKNI_Memset(surfmem1.buffer, 0x00, surfaceCreateSettings.height * surfmem1.pitch);
	NEXUS_Surface_Flush(pContext->framebuffer1);
	NEXUS_SurfaceClient_SetSurface(pContext->surfaceClient, pContext->framebuffer1);

	if(userdata)
	{
		buserdata_destroy(userdata);
		userdata=NULL;
	}

	if(ccEngine)
	{	
		B_Dcc_Close(ccEngine);
		ccEngine=NULL;
	}

	if(winlib)
	{
		BDCC_WINLIB_UnloadFonts(winlib);
		BDCC_WINLIB_Close( winlib );
	}

	if(pContext->framebuffer1)
	{
		NEXUS_Surface_Destroy( pContext->framebuffer1 );
		pContext->framebuffer1=NULL;
	}
	
	player->ccstatus=CCSTOP;
	
	return rc;


}



/*===============end cc=====================*/

int media_player_seek( media_player_t player, long offset, int whence )
{
	NEXUS_PlaybackStatus status;
	int rc;
	if (player->ipActive) 
	{
		return media_ip_player_seek(player->ip, offset, whence);
	}
	
	rc = NEXUS_Playback_GetStatus(player->playback, &status);
	if (rc) return BERR_TRACE(rc);
	switch (whence) {
	case SEEK_SET:
	default:
		break;
	case SEEK_CUR:
		offset += status.position;
		break;
	case SEEK_END:
		offset += status.last;
		break;
	}
#define TIMESHIFT_GAP 5000
	if (offset < 0 || (unsigned)offset < status.first) {
		offset = status.first;
	}
#if 0
	else if (player->start_settings.loopMode == NEXUS_PlaybackLoopMode_ePlay && status.last > TIMESHIFT_GAP && (unsigned)offset > status.last - TIMESHIFT_GAP) {
		offset = status.last - TIMESHIFT_GAP;
	}
#endif
	else if ((unsigned)offset > status.last) {
		offset = status.last;
	}
	
	{
		char timestr[16];
		unsigned hr = offset / 1000 / 60 / 60;
		unsigned min = (offset / 1000 / 60) % 60;
		unsigned sec = (offset / 1000) % 60;
		if (hr) 
			BKNI_Snprintf(timestr, sizeof(timestr), "%d:%02d:%02d", hr, min, sec);
		else 
			BKNI_Snprintf(timestr, sizeof(timestr), "%d:%02d", min, sec);
		BDBG_WRN(("seek %s", timestr));
	}
	return NEXUS_Playback_Seek(player->playback, offset);

}

#if B_REFSW_TR69C_SUPPORT
int media_player_get_set_tr69c_info(void *context, enum b_tr69c_type type, union b_tr69c_info *info)
{
    media_player_t player = context;
    int rc;
    switch (type)
    {
		case b_tr69c_type_get_playback_ip_status:
			if (player->ipActive) {
				rc = media_player_ip_get_set_tr69c_info(player->ip, type, info);
				if (rc) return BERR_TRACE(rc);
			}
			break;

        case b_tr69c_type_get_playback_status:
            rc = NEXUS_Playback_GetStatus(player->playback, &info->playback_status);
            if (rc) return BERR_TRACE(rc);
            break;
        case b_tr69c_type_get_video_decoder_status:
            rc = NEXUS_SimpleVideoDecoder_GetStatus(player->videoDecoder, &info->video_decoder_status);
            if (rc) return BERR_TRACE(rc);
            break;
        case b_tr69c_type_get_audio_decoder_status:
            rc = NEXUS_SimpleAudioDecoder_GetStatus(player->audioDecoder, &info->audio_decoder_status);
            if (rc) return BERR_TRACE(rc);
            break;
		case b_tr69c_type_set_video_decoder_mute:
		{
			NEXUS_VideoDecoderSettings settings;
			NEXUS_SimpleVideoDecoder_GetSettings(player->videoDecoder, &settings);
			settings.mute = info->video_decoder_mute;
			rc = NEXUS_SimpleVideoDecoder_SetSettings(player->videoDecoder, &settings);
            if (rc) return BERR_TRACE(rc);
			break;
		}
        default:
            return BERR_TRACE(NEXUS_NOT_SUPPORTED);
    }
    return 0;
}
#endif

LPCSTR getVidWinContentModeDesc(NEXUS_VideoWindowContentMode eVidWnContentMode)
{
	LPCSTR pszDesc = "";
	switch(eVidWnContentMode)
	{
		case NEXUS_VideoWindowContentMode_eZoom:
			pszDesc = "NEXUS_VideoWindowContentMode_eZoom";
			break;
		case NEXUS_VideoWindowContentMode_eBox:
			pszDesc = "NEXUS_VideoWindowContentMode_eBox";
			break;
		case NEXUS_VideoWindowContentMode_ePanScan:
			pszDesc = "NEXUS_VideoWindowContentMode_ePanScan";
			break;
		case NEXUS_VideoWindowContentMode_eFull:
			pszDesc = "NEXUS_VideoWindowContentMode_eFull";
			break;
		case NEXUS_VideoWindowContentMode_eFullNonLinear:
			pszDesc = "NEXUS_VideoWindowContentMode_eFullNonLinear";
			break;
		case NEXUS_VideoWindowContentMode_ePanScanWithoutCorrection:
			pszDesc = "NEXUS_VideoWindowContentMode_ePanScanWithoutCorrection";
			break;
		default:
			break;
	}
	return pszDesc;
}

int MediaPlayer_getVidPos(media_player_t pMediaPlayer, int *x, int *y, int *width, int *height)
{
	int iNexusErr = NEXUS_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;

	if(0 >= pMediaPlayer->allocResults.surfaceClient[0].id)
	{
		iNexusErr = NEXUS_NOT_AVAILABLE;
		goto EXIT_PROC;
	}

	NxClient_GetSurfaceClientComposition(pMediaPlayer->allocResults.surfaceClient[0].id, &surfaceComposition);
	
	*x = surfaceComposition.position.x;
	*y = surfaceComposition.position.y;
	*width = surfaceComposition.position.width;
	*height = surfaceComposition.position.height;

EXIT_PROC:
	return iNexusErr;
}

int MediaPlayer_setOsdPos(media_player_t pMediaPlayer, int x, int y, int w, int h)
{
	int iNexusErr = NEXUS_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;
	int iwebsurfaceid;
	if(0 >= pMediaPlayer->allocResults.surfaceClient[0].id)
	{
		iNexusErr = NEXUS_NOT_AVAILABLE;
		goto EXIT_PROC;
	}

	iwebsurfaceid=pMediaPlayer->allocResults.surfaceClient[0].id+1;

	NxClient_GetSurfaceClientComposition(iwebsurfaceid, &surfaceComposition);
	surfaceComposition.position.x = x;
	surfaceComposition.position.y = y;
	surfaceComposition.position.width = w;
	surfaceComposition.position.height = h;
	iNexusErr = NxClient_SetSurfaceClientComposition(iwebsurfaceid, &surfaceComposition);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iNexusErr;
}


int MediaPlayer_getOsdPos(media_player_t pMediaPlayer, int *x, int *y, int *width, int *height)
{
	int iNexusErr = NEXUS_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;
	int iwebsurfaceid;
	if(0 >= pMediaPlayer->allocResults.surfaceClient[0].id)
	{
		iNexusErr = NEXUS_NOT_AVAILABLE;
		goto EXIT_PROC;
	}

	iwebsurfaceid=pMediaPlayer->allocResults.surfaceClient[0].id+1;
	NxClient_GetSurfaceClientComposition(iwebsurfaceid, &surfaceComposition);
	
	*x = surfaceComposition.position.x;
	*y = surfaceComposition.position.y;
	*width = surfaceComposition.position.width;
	*height = surfaceComposition.position.height;

EXIT_PROC:
	return iNexusErr;
}

int MediaPlayer_setVidPos(media_player_t pMediaPlayer, int x, int y, int w, int h)
{
	int iNexusErr = NEXUS_SUCCESS;
	NEXUS_SurfaceComposition surfaceComposition;

	if(0 >= pMediaPlayer->allocResults.surfaceClient[0].id)
	{
		iNexusErr = NEXUS_NOT_AVAILABLE;
		goto EXIT_PROC;
	}

	NxClient_GetSurfaceClientComposition(pMediaPlayer->allocResults.surfaceClient[0].id, &surfaceComposition);
	/*
	LOG_BLINE("%u surfaceComposition:\n", pMediaPlayer->allocResults.surfaceClient[0].id);
	LOG_BLINE("  virtualDisplay:\n");
	LOG_BLINE("    Width: %u\n", surfaceComposition.virtualDisplay.width);
	LOG_BLINE("    Height: %u\n", surfaceComposition.virtualDisplay.height);
	LOG_BLINE("  position:\n");	
	LOG_BLINE("    x: %d\n", surfaceComposition.position.x);	
	LOG_BLINE("    y: %d\n", surfaceComposition.position.y);	
	LOG_BLINE("    w: %d\n", surfaceComposition.position.width);	
	LOG_BLINE("    h: %d\n", surfaceComposition.position.height);
	LOG_BLINE("  clipRect:\n");	
	LOG_BLINE("    x: %d\n", surfaceComposition.clipRect.x);	
	LOG_BLINE("    y: %d\n", surfaceComposition.clipRect.y);	
	LOG_BLINE("    w: %d\n", surfaceComposition.clipRect.width);	
	LOG_BLINE("    h: %d\n", surfaceComposition.clipRect.height);	
	LOG_BLINE("  zorder: %u\n", surfaceComposition.zorder);	
	LOG_BLINE("  contentMode: %s\n", getVidWinContentModeDesc(surfaceComposition.contentMode));	
	*/
	surfaceComposition.virtualDisplay.width = HwScaler_Relative_MAX_SIZE_W;
	surfaceComposition.virtualDisplay.height = HwScaler_Relative_MAX_SIZE_H;
	surfaceComposition.position.x = x;
	surfaceComposition.position.y = y;
	surfaceComposition.position.width = w;
	surfaceComposition.position.height = h;

	/*
	LOG_BLINE("%u surfaceComposition:\n", pMediaPlayer->allocResults.surfaceClient[0].id);
	LOG_BLINE("  virtualDisplay:\n");
	LOG_BLINE("    Width: %u\n", surfaceComposition.virtualDisplay.width);
	LOG_BLINE("    Height: %u\n", surfaceComposition.virtualDisplay.height);
	LOG_BLINE("  position:\n");	
	LOG_BLINE("    x: %d\n", surfaceComposition.position.x);	
	LOG_BLINE("    y: %d\n", surfaceComposition.position.y);	
	LOG_BLINE("    w: %d\n", surfaceComposition.position.width);	
	LOG_BLINE("    h: %d\n", surfaceComposition.position.height);	
	LOG_BLINE("  clipRect:\n");	
	LOG_BLINE("    x: %d\n", surfaceComposition.clipRect.x);	
	LOG_BLINE("    y: %d\n", surfaceComposition.clipRect.y);	
	LOG_BLINE("    w: %d\n", surfaceComposition.clipRect.width);	
	LOG_BLINE("    h: %d\n", surfaceComposition.clipRect.height);	
	LOG_BLINE("  zorder: %u\n", surfaceComposition.zorder);	
	LOG_BLINE("  contentMode: %s\n", getVidWinContentModeDesc(surfaceComposition.contentMode));
	*/
	surfaceComposition.zorder=1; /*abner*/
	iNexusErr = NxClient_SetSurfaceClientComposition(pMediaPlayer->allocResults.surfaceClient[0].id, &surfaceComposition);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iNexusErr;
}

int MediaPlayer_setVidWinAspectRatio(media_player_t pMediaPlayer, NEXUS_DisplayAspectRatio eDispAspectRatio, bool bFullVideo)
{
	int iNexusErr = NEXUS_SUCCESS;
	int iRet;
	media_player_t player;
	NEXUS_SurfaceClientSettings videoSurfaceSettings;
	NEXUS_SurfaceComposition * pVideoSurfaceComposition;
	NEXUS_Rect vidWinPos;
	uint32_t sourceStreamWidth;	
	uint32_t sourceStreamHeight;
	int32_t monitorAS_numerator, monitorAS_denominator;

	player = pMediaPlayer;
	NEXUS_SurfaceClient_GetSettings(pMediaPlayer->hVideoSurfaceClient, &videoSurfaceSettings);
	pVideoSurfaceComposition = &(videoSurfaceSettings.composition);

	/*
	LOG("virtualDisplay\n");
	LOG("  width: %hu\n", pVideoSurfaceComposition->virtualDisplay.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->virtualDisplay.height);
	LOG("position\n");
	LOG("  x: %hd\n", pVideoSurfaceComposition->position.x);
	LOG("  y: %hd\n", pVideoSurfaceComposition->position.y);
	LOG("  width: %hu\n", pVideoSurfaceComposition->position.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->position.height);
	LOG("zorder: %d\n", pVideoSurfaceComposition->zorder);
	LOG("visible: %d\n", pVideoSurfaceComposition->visible);
	LOG("contentMode: %d\n", pVideoSurfaceComposition->contentMode);	
	LOG("clipRect\n");
	LOG("  x: %hd\n", pVideoSurfaceComposition->clipRect.x);
	LOG("  y: %hd\n", pVideoSurfaceComposition->clipRect.y);
	LOG("  width: %hu\n", pVideoSurfaceComposition->clipRect.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->clipRect.height);	
	*/
	if(bFullVideo)	/*full*/
	{
		vidWinPos.x = 0;
		vidWinPos.y = 0;
		vidWinPos.width = pVideoSurfaceComposition->virtualDisplay.width;
		vidWinPos.height = pVideoSurfaceComposition->virtualDisplay.height;
	}
	else	/*not full*/
	{
		int32_t sourceAsPercent, monitorAsPercent;
		if(NEXUS_DisplayAspectRatio_eAuto == eDispAspectRatio)
		{
			monitorAS_numerator = 16;
			monitorAS_denominator = 9;
		}
		else if(NEXUS_DisplayAspectRatio_e4x3 == eDispAspectRatio)
		{
			monitorAS_numerator = 16;
			monitorAS_denominator = 9;
		}
		else if(NEXUS_DisplayAspectRatio_e16x9 == eDispAspectRatio)
		{
			monitorAS_numerator = 4;
			monitorAS_denominator = 3;
		}
		else
		{
			monitorAS_numerator = 16;
			monitorAS_denominator = 9;
		}

		{
			bool bLocked = false;
			/* lock */
			if(false == bLocked)
			{
				if(player->hMutex)
				{
					iRet = CErrChkMutex_Lock(player->hMutex);
					if(ERR_SUCCESS == iRet)
					{
						bLocked = true;
					}
					else
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
			}
			/* get */
			sourceStreamWidth = player->sourceStreamWidth;
			sourceStreamHeight = player->sourceStreamHeight;
			/* unlock */
			if(true == bLocked)
			{
				iRet = CErrChkMutex_Unlock(player->hMutex);
				if(ERR_SUCCESS == iRet)
				{
					bLocked = false;
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			
			if(0 == sourceStreamWidth || 0 == sourceStreamHeight)
			{
				goto EXIT_PROC;
			}
		}

		
		sourceAsPercent = sourceStreamWidth*100/sourceStreamHeight;
		monitorAsPercent = monitorAS_numerator*100/monitorAS_denominator;
		/*LOG("SourceAS=%d,MonitorAS=%d\n", sourceAsPercent, monitorAsPercent);*/
		if(monitorAsPercent >= sourceAsPercent)	/*keep height*/
		{
			vidWinPos.height = pVideoSurfaceComposition->virtualDisplay.height;
			vidWinPos.y = 0;
			vidWinPos.width = ((uint32_t)pVideoSurfaceComposition->virtualDisplay.width*sourceAsPercent/monitorAsPercent);
			vidWinPos.x = (pVideoSurfaceComposition->virtualDisplay.width-vidWinPos.width)/2;
		}
		else	/*keep width*/
		{
			vidWinPos.width = pVideoSurfaceComposition->virtualDisplay.width;
			vidWinPos.x = 0;
			vidWinPos.height = ((uint32_t)pVideoSurfaceComposition->virtualDisplay.height*monitorAsPercent/sourceAsPercent);
			vidWinPos.y = (pVideoSurfaceComposition->virtualDisplay.height-vidWinPos.height)/2;
		}
		/*LOG("vidWinPos: %d,%d,%d,%d\n", vidWinPos.x, vidWinPos.y, vidWinPos.width, vidWinPos.height);*/
	}

	pVideoSurfaceComposition->contentMode = NEXUS_VideoWindowContentMode_eFull;
	pVideoSurfaceComposition->position = vidWinPos;
	iNexusErr = NEXUS_SurfaceClient_SetSettings(pMediaPlayer->hVideoSurfaceClient, &videoSurfaceSettings);
	if(NEXUS_SUCCESS != iNexusErr)
	{
		LOG_BLINE("iNexusErr=%d\n", iNexusErr);
		goto EXIT_PROC;
	}

	/*	
	LOG("\n");
	NEXUS_SurfaceClient_GetSettings(pMediaPlayer->hVideoSurfaceClient, &videoSurfaceSettings);
	pVideoSurfaceComposition = &(videoSurfaceSettings.composition);
	LOG("virtualDisplay\n");
	LOG("  width: %hu\n", pVideoSurfaceComposition->virtualDisplay.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->virtualDisplay.height);
	LOG("position\n");
	LOG("  x: %hd\n", pVideoSurfaceComposition->position.x);
	LOG("  y: %hd\n", pVideoSurfaceComposition->position.y);
	LOG("  width: %hu\n", pVideoSurfaceComposition->position.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->position.height);
	LOG("zorder: %d\n", pVideoSurfaceComposition->zorder);
	LOG("visible: %d\n", pVideoSurfaceComposition->visible);
	LOG("contentMode: %d\n", pVideoSurfaceComposition->contentMode);	
	LOG("clipRect\n");
	LOG("  x: %hd\n", pVideoSurfaceComposition->clipRect.x);
	LOG("  y: %hd\n", pVideoSurfaceComposition->clipRect.y);
	LOG("  width: %hu\n", pVideoSurfaceComposition->clipRect.width);
	LOG("  height: %hu\n", pVideoSurfaceComposition->clipRect.height);
	*/
	
EXIT_PROC:
	return iNexusErr;
}

