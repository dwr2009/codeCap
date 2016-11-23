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
#include "media_probe.h"
#include "bstd.h"
#include "namevalue.h"
#include "nexus_file.h"
#include <string.h>

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

BDBG_MODULE(media_probe);

int probe_media(const char *streamname, struct probe_results *results)
{
    bmedia_probe_t probe;
    bfile_io_read_t fd = NULL;
    FILE *fin;
    bmedia_probe_config probe_config;
    const bmedia_probe_track *track;
    const bmedia_probe_stream *stream;
    NEXUS_FilePlayHandle file;
    int rc = 0;
    const char *indexname = NULL;

    memset(results, 0, sizeof(*results));

    probe = bmedia_probe_create();
    if (!probe) {
        return BERR_TRACE(-1);
    }

    fin = fopen64(streamname, "rb");
    if (!fin) {
        BDBG_ERR(("can't open '%s'", streamname));
        return -1;
    }

    fd = bfile_stdio_read_attach(fin);

    bmedia_probe_default_cfg(&probe_config);
    probe_config.file_name = streamname;
    probe_config.type = bstream_mpeg_type_unknown;
    stream = bmedia_probe_parse(probe, fd, &probe_config);

    /* now stream is either NULL, or stream descriptor with linked list of audio/video tracks */
    bfile_stdio_read_detach(fd);

    fclose(fin);
    if(!stream) {
        BDBG_ERR(("media probe can't parse '%s'", streamname));
        bmedia_probe_destroy(probe);
        probe = NULL;
        return -1;
    }

    if (stream->index == bmedia_probe_index_available || stream->index == bmedia_probe_index_required) {
        /* if user didn't specify an index, use the file as index if probe indicates */
        indexname = streamname;
        results->useStreamAsIndex = true;
    }
    file = NEXUS_FilePlay_OpenPosix3(streamname, indexname, false);
    if (!file) {
        BDBG_ERR(("can't open '%s'", streamname));
        rc = -1;
        goto error;
    }

    results->transportType = b_mpegtype2nexus(stream->type);
    if (stream->type == bstream_mpeg_type_ts) {
        if ((((bmpeg2ts_probe_stream*)stream)->pkt_len) == 192) {
            results->timestampType = NEXUS_TransportTimestampType_eMod300;
        }
    }

    for(track=BLST_SQ_FIRST(&stream->tracks);track;track=BLST_SQ_NEXT(track, link)) {
        switch(track->type) {
            case bmedia_track_type_audio:
                if(track->info.audio.codec != baudio_format_unknown && results->num_audio < MAX_PIDS) {
                    results->audio[results->num_audio].pid = track->number;
                    results->audio[results->num_audio].codec = b_audiocodec2nexus(track->info.audio.codec);
                    results->num_audio++;
                }
                break;
            case bmedia_track_type_video:
                if (track->info.video.codec == bvideo_codec_h264_svc || track->info.video.codec == bvideo_codec_h264_mvc) {
                    /* TODO */
                }
                else if (track->info.video.codec != bvideo_codec_unknown && results->num_video < MAX_PIDS) {
                    results->video[results->num_video].pid = track->number;
                    results->video[results->num_video].codec = b_videocodec2nexus(track->info.video.codec);
                    results->num_video++;
                }
                break;
            default:
                break;
        }
    }

error:
    if (file) {
        NEXUS_FilePlay_Close(file);
    }
    if (probe) {
        if (stream) {
            bmedia_probe_stream_free(probe, stream);
        }
        bmedia_probe_destroy(probe);
    }

    return rc;
}
