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
#include "live_decode.h"
#include "bstd.h"
#include "bkni.h"
#include "bdbg.h"
#include "blst_slist.h"
#include "nexus_simple_video_decoder.h"
#include "nexus_simple_video_decoder_primer.h"
#include "nexus_simple_audio_decoder.h"
#include "nexus_simple_stc_channel.h"
#include "nexus_surface_client.h"
#include "nexus_core_utils.h"
#include "nxclient.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

BDBG_MODULE(live);

struct live_decode;

struct live_decode_channel {
    BLST_S_ENTRY(live_decode_channel) link;
    NxClient_AllocResults allocResults;
    unsigned connectId;
    live_decode_start_settings start_settings;
    unsigned videoDecoderId;
    NEXUS_SimpleVideoDecoderHandle videoDecoder;
    NEXUS_SimpleVideoDecoderStartSettings videoProgram;
    NEXUS_SimpleAudioDecoderHandle audioDecoder;
    NEXUS_SimpleAudioDecoderStartSettings audioProgram;
    NEXUS_PidChannelHandle pcrPidChannel;
    NEXUS_SimpleStcChannelHandle stcChannel;
    struct live_decode *decode;
    bool started;
};

struct live_decode {
    live_decode_create_settings create_settings;
    struct live_decode_channel *current;
    BLST_S_HEAD(channel_list, live_decode_channel) channels;
};

void live_decode_get_default_create_settings( live_decode_create_settings *psettings )
{
    memset(psettings, 0, sizeof(*psettings));
    psettings->primed = true;
}

live_decode_t live_decode_create( const live_decode_create_settings *psettings )
{
    NxClient_JoinSettings joinSettings;
    live_decode_t decode;
    int rc;
    live_decode_create_settings default_settings;

    if (!psettings) {
        live_decode_get_default_create_settings(&default_settings);
        psettings = &default_settings;
    }

    /* connect to server and nexus */
    NxClient_GetDefaultJoinSettings(&joinSettings);
    snprintf(joinSettings.name, NXCLIENT_MAX_NAME, "%s", "live_decode");
    rc = NxClient_Join(&joinSettings);
    if (rc) {
        return NULL;
    }

    /* no DBG until after Join, so defer this */
    decode = malloc(sizeof(*decode));
    if (!decode) {
        NxClient_Uninit();
        return NULL;
    }
    memset(decode, 0, sizeof(*decode));
    decode->create_settings = *psettings;

    return decode;
}

void live_decode_destroy(live_decode_t decode)
{
    live_decode_channel_t channel;
    while ((channel = BLST_S_FIRST(&decode->channels))) {
        live_decode_destroy_channel(channel);
    }
    free(decode);
    NxClient_Uninit();
}

void live_decode_get_default_start_settings( live_decode_start_settings *psettings )
{
    memset(psettings, 0, sizeof(*psettings));
}

static void start_audio(live_decode_channel_t channel)
{
    if (channel->audioProgram.primary.pidChannel) {
        NEXUS_SimpleAudioDecoder_SetStcChannel(channel->audioDecoder, channel->stcChannel);
        NEXUS_SimpleAudioDecoder_Start(channel->audioDecoder, &channel->audioProgram);
    }
}

static void stop_audio(live_decode_channel_t channel)
{
    if (channel->audioProgram.primary.pidChannel) {
        NEXUS_SimpleAudioDecoder_Stop(channel->audioDecoder);
        NEXUS_SimpleAudioDecoder_SetStcChannel(channel->audioDecoder, NULL);
    }
}

static void first_pts_passed(void *context, int param)
{
    BSTD_UNUSED(param);
    BDBG_MSG(("first pts passed: %d", context));
}

live_decode_channel_t live_decode_create_channel( live_decode_t decode )
{
    live_decode_channel_t channel;
    NxClient_AllocSettings allocSettings;
    int rc;
    NEXUS_VideoDecoderSettings settings;
    NEXUS_VideoDecoderPrimerSettings primerSettings;
    
    channel = BKNI_Malloc(sizeof(*channel));
    if (!channel) {
        BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
        return NULL;
    }
    BKNI_Memset(channel, 0, sizeof(*channel));
    channel->decode = decode;
    BLST_S_INSERT_HEAD(&decode->channels, channel, link);
    
    NxClient_GetDefaultAllocSettings(&allocSettings);
    allocSettings.simpleVideoDecoder = 1;
    allocSettings.simpleAudioDecoder = 1;
    rc = NxClient_Alloc(&allocSettings, &channel->allocResults);
    if (rc) { rc = BERR_TRACE(rc); goto error;}
    
    if (channel->allocResults.simpleAudioDecoder.id) {
        channel->audioDecoder = NEXUS_SimpleAudioDecoder_Acquire(channel->allocResults.simpleAudioDecoder.id);
        if (!channel->audioDecoder) {
            BDBG_WRN(("audio decoder not available"));
        }
    }

    channel->videoDecoderId = channel->allocResults.simpleVideoDecoder[0].id;
    
    channel->videoDecoder = NEXUS_SimpleVideoDecoder_Acquire(channel->videoDecoderId);
    if (!channel->videoDecoder) {
        rc = BERR_TRACE(NEXUS_NOT_AVAILABLE);
        goto error;
    }
    
    NEXUS_SimpleVideoDecoder_GetSettings(channel->videoDecoder, &settings);
    settings.firstPtsPassed.callback = first_pts_passed;
    settings.firstPtsPassed.context = channel;
    rc = NEXUS_SimpleVideoDecoder_SetSettings(channel->videoDecoder, &settings);
    BDBG_ASSERT(!rc);

    NEXUS_SimpleVideoDecoderPrimer_GetSettings(channel->videoDecoder, &primerSettings);
    primerSettings.ptsStcDiffCorrectionEnabled = false; /* TODO: enable after fix */
    primerSettings.pastTolerance = 45*500; /* 1/2 second */
    primerSettings.futureTolerance = 0;
    NEXUS_SimpleVideoDecoderPrimer_SetSettings(channel->videoDecoder, &primerSettings);

    channel->stcChannel = NEXUS_SimpleStcChannel_Create(NULL);
    
    NEXUS_SimpleVideoDecoder_SetStcChannel(channel->videoDecoder, channel->stcChannel);
    
    return channel;
    
error:
    live_decode_destroy_channel(channel);
    return NULL;
}
    
void live_decode_destroy_channel( live_decode_channel_t channel )
{
    if (channel->audioDecoder) {
        NEXUS_SimpleAudioDecoder_Release(channel->audioDecoder);
    }
    if (channel->videoDecoder) {
        NEXUS_SimpleVideoDecoder_Release(channel->videoDecoder);
    }
    if (channel->stcChannel) {
        NEXUS_SimpleStcChannel_Destroy(channel->stcChannel);
    }
    NxClient_Free(&channel->allocResults);
    BLST_S_REMOVE(&channel->decode->channels, channel, live_decode_channel, link);
    BKNI_Free(channel);
}

int live_decode_start_channel(live_decode_channel_t channel, const live_decode_start_settings *psettings )
{
    int rc;
    
    if (!psettings) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }
    channel->start_settings = *psettings;
    
    NEXUS_SimpleVideoDecoder_GetDefaultStartSettings(&channel->videoProgram);
    NEXUS_SimpleAudioDecoder_GetDefaultStartSettings(&channel->audioProgram);

    if (psettings->video.pid) {
        channel->videoProgram.settings.pidChannel = NEXUS_PidChannel_Open(psettings->parserBand, psettings->video.pid, NULL);
        channel->videoProgram.settings.codec = psettings->video.codec;
    }
    if (psettings->audio.pid && channel->audioDecoder) {
        channel->audioProgram.primary.pidChannel = NEXUS_PidChannel_Open(psettings->parserBand, psettings->audio.pid, NULL);
        channel->audioProgram.primary.codec = psettings->audio.codec;
    }
    if (psettings->pcr_pid) {
        channel->pcrPidChannel = NEXUS_PidChannel_Open(psettings->parserBand, psettings->pcr_pid, NULL);
    }
    
    if (channel->pcrPidChannel && channel->stcChannel) {
        NEXUS_SimpleStcChannelSettings stcSettings;
        NEXUS_SimpleStcChannel_GetSettings(channel->stcChannel, &stcSettings);
        stcSettings.mode = NEXUS_StcChannelMode_ePcr;
        stcSettings.modeSettings.pcr.pidChannel = channel->pcrPidChannel;
        stcSettings.modeSettings.pcr.offsetThreshold = 0xFF;
        NEXUS_SimpleStcChannel_SetSettings(channel->stcChannel, &stcSettings);
    }
    
    if (!channel->videoProgram.settings.pidChannel && !channel->audioProgram.primary.pidChannel) {
        BDBG_WRN(("no content found"));
        rc = BERR_TRACE(NEXUS_INVALID_PARAMETER);
        goto error;
    }
    
    if (psettings->video.pid && channel->decode->create_settings.primed) {
        rc = NEXUS_SimpleVideoDecoder_StartPrimer(channel->videoDecoder, &channel->videoProgram);
        if (rc) {rc = BERR_TRACE(rc); goto error;}
    }
    
    channel->started = true;
    
    return 0;

error:
    if (channel->videoProgram.settings.pidChannel) {
        NEXUS_PidChannel_Close(channel->videoProgram.settings.pidChannel);
        channel->videoProgram.settings.pidChannel = NULL;
    }
    if (channel->audioProgram.primary.pidChannel) {
        NEXUS_PidChannel_Close(channel->audioProgram.primary.pidChannel);
        channel->audioProgram.primary.pidChannel = NULL;
    }
    if (channel->pcrPidChannel) {
        if (channel->stcChannel) {
            NEXUS_SimpleStcChannelSettings stcSettings;
            NEXUS_SimpleStcChannel_GetSettings(channel->stcChannel, &stcSettings);
            stcSettings.mode = NEXUS_StcChannelMode_eAuto;
            NEXUS_SimpleStcChannel_SetSettings(channel->stcChannel, &stcSettings);
        }
        NEXUS_PidChannel_Close(channel->pcrPidChannel);
        channel->pcrPidChannel = NULL;
    }
    return rc;
}

void live_decode_stop_channel(live_decode_channel_t channel)
{
    if (!channel->started) return;
    
    if (channel == channel->decode->current) {
        NEXUS_SimpleVideoDecoder_Stop(channel->videoDecoder);
        stop_audio(channel);
        if (channel->connectId) {
            NxClient_Disconnect(channel->connectId);
            channel->connectId = 0;
        }
        channel->decode->current = NULL;
    }
    else {
        NEXUS_SimpleVideoDecoder_StopPrimer(channel->videoDecoder);
    }
    
    /* converse of prologue */
    if (channel->videoProgram.settings.pidChannel) {
        NEXUS_PidChannel_Close(channel->videoProgram.settings.pidChannel);
        channel->videoProgram.settings.pidChannel = NULL;
    }
    if (channel->audioProgram.primary.pidChannel) {
        NEXUS_PidChannel_Close(channel->audioProgram.primary.pidChannel);
        channel->audioProgram.primary.pidChannel = NULL;
    }
    if (channel->pcrPidChannel) {
        if (channel->stcChannel) {
            NEXUS_SimpleStcChannelSettings stcSettings;
            NEXUS_SimpleStcChannel_GetSettings(channel->stcChannel, &stcSettings);
            stcSettings.mode = NEXUS_StcChannelMode_eAuto;
            NEXUS_SimpleStcChannel_SetSettings(channel->stcChannel, &stcSettings);
        }
        NEXUS_PidChannel_Close(channel->pcrPidChannel);
        channel->pcrPidChannel = NULL;
    }
    channel->started = false;
}

int live_decode_activate( live_decode_channel_t channel)
{
    int rc;
    NxClient_ConnectSettings connectSettings;
    
    if (!channel->started) return -1;

    if (channel->decode->current) {
        if (channel->decode->current == channel) {
            return 0;
        }
        if (channel->start_settings.video.pid && channel->decode->create_settings.primed) {
            rc = NEXUS_SimpleVideoDecoder_StopDecodeAndStartPrimer(channel->decode->current->videoDecoder);
            if (rc) return BERR_TRACE(rc);
        }
        else {
            NEXUS_SimpleVideoDecoder_Stop(channel->decode->current->videoDecoder);
        }
        stop_audio(channel->decode->current);
        if (channel->decode->current->connectId) {
            NxClient_Disconnect(channel->decode->current->connectId);
            channel->decode->current->connectId = 0;
        }
        channel->decode->current = NULL;
    }
    
    BDBG_ASSERT(!channel->connectId);
    NxClient_GetDefaultConnectSettings(&connectSettings);
    connectSettings.simpleVideoDecoder[0].id = channel->videoDecoderId;
    connectSettings.simpleVideoDecoder[0].surfaceClientId = channel->decode->create_settings.window.surfaceClientId;
    connectSettings.simpleVideoDecoder[0].windowId = channel->decode->create_settings.window.id;
    connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxWidth = 1920;
    connectSettings.simpleVideoDecoder[0].decoderCapabilities.maxHeight = 1080;
    connectSettings.simpleVideoDecoder[0].windowCapabilities.type = channel->start_settings.videoWindowType;
    connectSettings.simpleAudioDecoder.id = channel->allocResults.simpleAudioDecoder.id;
    rc = NxClient_Connect(&connectSettings, &channel->connectId);
    if (rc) return BERR_TRACE(rc);
    
    if (channel->start_settings.video.pid) {
        BDBG_MSG(("start decode: %p", channel->decode));
        if (channel->decode->create_settings.primed) {
            rc = NEXUS_SimpleVideoDecoder_StopPrimerAndStartDecode(channel->videoDecoder);
        }
        else {
            rc = NEXUS_SimpleVideoDecoder_Start(channel->videoDecoder, &channel->videoProgram);
        }
        if (rc) {
            if (channel->connectId) {
                NxClient_Disconnect(channel->connectId);
                channel->connectId = 0;
            }
            return BERR_TRACE(rc);
        }
    }
    start_audio(channel);
    channel->decode->current = channel;
    return 0;
}

NEXUS_SimpleVideoDecoderHandle live_decode_get_video_decoder(live_decode_channel_t channel)
{
    return channel->videoDecoder;
}
