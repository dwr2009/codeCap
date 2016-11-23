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
#include "live_source.h"
#include "bstd.h"
#include "bdbg.h"
#include "nexus_platform.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

BDBG_MODULE(live);

#if NEXUS_HAS_FRONTEND
static void lock_callback(void *context, int param)
{
    NEXUS_FrontendHandle frontend = (NEXUS_FrontendHandle)context;
    NEXUS_FrontendFastStatus status;
    BSTD_UNUSED(param);
    NEXUS_Frontend_GetFastStatus(frontend, &status);
    BDBG_MSG(("frontend %#x %s", (unsigned)frontend, status.lockStatus?"locked":"unlocked"));
}
#endif

void get_default_tune_settings(tune_settings *psettings)
{
    memset(psettings, 0, sizeof(*psettings));
    psettings->qam.mode = 64;
#if NEXUS_HAS_FRONTEND
    psettings->ofdm.mode = NEXUS_FrontendOfdmMode_eDvbt;
    psettings->sat.mode = NEXUS_FrontendSatelliteMode_eDvb;
#endif
}

#if NEXUS_HAS_FRONTEND
static int connect_frontend_to_transport(NEXUS_FrontendHandle frontend, NEXUS_ParserBand parserBand)
{
    NEXUS_ParserBandSettings parserBandSettings;
    NEXUS_FrontendUserParameters userParams;
    int rc;

    NEXUS_Frontend_GetUserParameters(frontend, &userParams);

    NEXUS_ParserBand_GetSettings(parserBand, &parserBandSettings);
    if (userParams.isMtsif) {
        parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eMtsif;
        parserBandSettings.sourceTypeSettings.mtsif = NEXUS_Frontend_GetConnector(frontend); /* NEXUS_Frontend_TuneXyz() will connect this frontend to this parser band */
    }
    else {
        parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
        parserBandSettings.sourceTypeSettings.inputBand = userParams.param1;  /* Platform initializes this to input band */
    }
    parserBandSettings.transportType = NEXUS_TransportType_eTs;
    rc = NEXUS_ParserBand_SetSettings(parserBand, &parserBandSettings);
    if (rc) return BERR_TRACE(rc);

    return 0;
}
#endif

int tune(NEXUS_ParserBand parserBand, NEXUS_FrontendHandle frontend, const tune_settings *psettings)
{
    int rc;

#if NEXUS_HAS_FRONTEND
    if (psettings->qam.freq) {
        NEXUS_FrontendQamSettings qamSettings;

        NEXUS_Frontend_GetDefaultQamSettings(&qamSettings);
        qamSettings.frequency = psettings->qam.freq * 1000000;
        switch (psettings->qam.mode) {
        case 64:
            qamSettings.mode = NEXUS_FrontendQamMode_e64;
            qamSettings.symbolRate = 5056900;
            break;
        case 256:
            qamSettings.mode = NEXUS_FrontendQamMode_e256;
            qamSettings.symbolRate = 5360537;
            break;
        case 1024:
            qamSettings.mode = NEXUS_FrontendQamMode_e1024;
            qamSettings.symbolRate = 0; /* TODO */
            break;
        default:
            BDBG_ERR(("unknown qam mode %d", psettings->qam.mode));
            rc = -1;
            goto error;
        }
        qamSettings.annex = NEXUS_FrontendQamAnnex_eB;
        qamSettings.bandwidth = NEXUS_FrontendQamBandwidth_e6Mhz;
        qamSettings.lockCallback.callback = lock_callback;
        qamSettings.lockCallback.context = frontend;

        rc = connect_frontend_to_transport(frontend, parserBand);
        if (rc) {rc = BERR_TRACE(rc); goto error;}

        if (psettings->alreadyTuned) {
            rc = NEXUS_Frontend_ReapplyTransportSettings(frontend);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }
        else {
            rc = NEXUS_Frontend_TuneQam(frontend, &qamSettings);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }

        BDBG_MSG(("Tuning QAM%d and %d MHz...", psettings->qam.mode, psettings->qam.freq));

        /* TODO: wait for lock. for now, just start scanning and let it lock during scan */
    }
    else if (psettings->ofdm.freq) {
        NEXUS_FrontendOfdmSettings ofdmSettings;

        NEXUS_Frontend_GetDefaultOfdmSettings(&ofdmSettings);
        ofdmSettings.frequency = psettings->ofdm.freq * 1000000;
        ofdmSettings.acquisitionMode = NEXUS_FrontendOfdmAcquisitionMode_eAuto;
        ofdmSettings.terrestrial = true;
        ofdmSettings.spectrum = NEXUS_FrontendOfdmSpectrum_eAuto;
        ofdmSettings.mode = psettings->ofdm.mode;
        ofdmSettings.lockCallback.callback = lock_callback;
        ofdmSettings.lockCallback.context = frontend;

        rc = connect_frontend_to_transport(frontend, parserBand);
        if (rc) {rc = BERR_TRACE(rc); goto error;}

        if (psettings->alreadyTuned) {
            rc = NEXUS_Frontend_ReapplyTransportSettings(frontend);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }
        else {
            rc = NEXUS_Frontend_TuneOfdm(frontend, &ofdmSettings);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }

        BDBG_MSG(("Tuning OFDM %d MHz...", psettings->ofdm.freq));

        /* TODO: wait for lock. for now, just start scanning and let it lock during scan */
    }
    else if (psettings->sat.freq) {
        NEXUS_FrontendSatelliteSettings satSettings;

        NEXUS_Frontend_GetDefaultSatelliteSettings(&satSettings);
        satSettings.frequency = psettings->sat.freq * 1000000;
        satSettings.mode = psettings->sat.mode;
        satSettings.lockCallback.callback = lock_callback;
        satSettings.lockCallback.context = frontend;

        rc = connect_frontend_to_transport(frontend, parserBand);
        if (rc) {rc = BERR_TRACE(rc); goto error;}

        if (psettings->alreadyTuned) {
            rc = NEXUS_Frontend_ReapplyTransportSettings(frontend);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }
        else {
            rc = NEXUS_Frontend_TuneSatellite(frontend, &satSettings);
            if (rc) {rc = BERR_TRACE(rc); goto error;}
        }

        BDBG_MSG(("Tuning SAT %d MHz...", psettings->sat.freq));

        /* TODO: wait for lock. for now, just start scanning and let it lock during scan */
    }
    else
#else
    BSTD_UNUSED(frontend);
#endif
    {
        NEXUS_ParserBandSettings parserBandSettings;
        NEXUS_InputBand inputBand;

        BDBG_MSG(("Scanning streamer %d...", psettings->streamer.index));
        NEXUS_Platform_GetStreamerInputBand(psettings->streamer.index, &inputBand);
        NEXUS_ParserBand_GetSettings(parserBand, &parserBandSettings);
        parserBandSettings.sourceType = NEXUS_ParserBandSourceType_eInputBand;
        parserBandSettings.sourceTypeSettings.inputBand = inputBand;
        parserBandSettings.transportType = NEXUS_TransportType_eTs;
        rc = NEXUS_ParserBand_SetSettings(parserBand, &parserBandSettings);
        if (rc) {rc = BERR_TRACE(rc); goto error;}
    }
    return 0;

error:
    return rc;
}

const char *channel_source_str[channel_source_max] = {"auto", "qam", "ofdm", "sat", "streamer"};

/* Broadcom Irvine lab */
const char *qam_freq_list = "765,777,789";
const char *ofdm_freq_list = "578"; /* 602 for DVB-T2 */
const char *sat_freq_list = "1119";

void get_default_channels(enum channel_source *psource, const char **freq_list)
{
#if NEXUS_HAS_FRONTEND
    /* auto-detect format */
    if (*psource == channel_source_auto) {
        NEXUS_FrontendAcquireSettings settings;
        NEXUS_FrontendHandle frontend;
        NEXUS_Frontend_GetDefaultAcquireSettings(&settings);
        settings.mode = NEXUS_FrontendAcquireMode_eByIndex;
        settings.index = NEXUS_ANY_ID;
        frontend = NEXUS_Frontend_Acquire(&settings);
        if (frontend) {
            NEXUS_FrontendCapabilities cap;
            NEXUS_Frontend_GetCapabilities(frontend, &cap);
            if (cap.qam) {
                *psource = channel_source_qam;
            }
            else if (cap.satellite) {
                *psource = channel_source_sat;
            }
            else if (cap.ofdm) {
                *psource = channel_source_ofdm;
            }
            else {
                *psource = channel_source_streamer;
            }
            NEXUS_Frontend_Release(frontend);
        }
        else
        {
            *psource = channel_source_streamer;
        }
    }

    if (!*freq_list) {
        switch (*psource) {
        case channel_source_qam:  *freq_list = qam_freq_list; break;
        case channel_source_ofdm: *freq_list = ofdm_freq_list; break;
        case channel_source_sat:  *freq_list = sat_freq_list; break;
        default: break;
        }
    }
#else
    *psource = channel_source_streamer;
    *freq_list = NULL;
#endif
}

#include "tshdrbuilder.h"
#include "nexus_packetsub.h"

struct bpsi_injection {
    NEXUS_PacketSubHandle packetSub;
    NEXUS_PidChannelHandle pidChannel;
};

/* TSHDRBUILDER has one extra byte at the beginning to describe the variable length TS header buffer */
#define BTST_TS_HEADER_BUF_LENGTH 189
#define PAT_PMT_PAIR_SIZE (188*2)

static int build_psi(unsigned pmtPid, void *buffer, unsigned size, unsigned *pSizeUsed, const tspsimgr_scan_results *pscan_results, unsigned program)
{
    uint8_t pat_pl_buf[BTST_TS_HEADER_BUF_LENGTH], pmt_pl_buf[BTST_TS_HEADER_BUF_LENGTH];
    size_t pat_pl_size, pmt_pl_size;
    unsigned streamNum;
    TS_PAT_state patState;
    TS_PSI_header psi_header;
    TS_PMT_state pmtState;
    TS_PAT_program pat_program;
    TS_PMT_stream pmt_stream;
    uint8_t *pat, *pmt;

    if (size < PAT_PMT_PAIR_SIZE) {
        return BERR_TRACE(NEXUS_INVALID_PARAMETER);
    }

    /* == CREATE PSI TABLES == */
    TS_PSI_header_Init(&psi_header);
    TS_PAT_Init(&patState, &psi_header, pat_pl_buf, BTST_TS_HEADER_BUF_LENGTH);

    TS_PAT_program_Init(&pat_program, 1, pmtPid);
    TS_PAT_addProgram(&patState, &pmtState, &pat_program, pmt_pl_buf, BTST_TS_HEADER_BUF_LENGTH);

    TS_PMT_setPcrPid(&pmtState, 0); /* unused */

    if (pscan_results->program_info[program].num_video_pids) {
        unsigned vidStreamType;
        switch(pscan_results->program_info[program].video_pids[0].codec) {
        case NEXUS_VideoCodec_eMpeg2:         vidStreamType = 0x2; break;
        case NEXUS_VideoCodec_eMpeg4Part2:    vidStreamType = 0x10; break;
        case NEXUS_VideoCodec_eH264:          vidStreamType = 0x1b; break;
        case NEXUS_VideoCodec_eVc1SimpleMain: vidStreamType = 0xea; break;
        default: return BERR_TRACE(NEXUS_NOT_SUPPORTED);
        }
        TS_PMT_stream_Init(&pmt_stream, vidStreamType, pscan_results->program_info[program].video_pids[0].pid);
        TS_PMT_addStream(&pmtState, &pmt_stream, &streamNum);
    }

    if (pscan_results->program_info[program].num_audio_pids) {
        unsigned audStreamType;
        switch(pscan_results->program_info[program].audio_pids[0].codec) {
        case NEXUS_AudioCodec_eMpeg:         audStreamType = 0x4; break;
        case NEXUS_AudioCodec_eMp3:          audStreamType = 0x4; break;
        case NEXUS_AudioCodec_eAac:          audStreamType = 0xf; break; /* ADTS */
        case NEXUS_AudioCodec_eAacPlus:      audStreamType = 0x11; break;/* LOAS */
        /* MP2TS doesn't allow 14496-3 AAC+ADTS; here is placeholder to test AAC-HE before LOAS encode is supported; */
        case NEXUS_AudioCodec_eAacPlusAdts:  audStreamType = 0x11; break;
        case NEXUS_AudioCodec_eAc3:          audStreamType = 0x81; break;
        case NEXUS_AudioCodec_eLpcm1394:     audStreamType = 0x83; break;
        default: return BERR_TRACE(NEXUS_NOT_SUPPORTED);
        }

        TS_PMT_stream_Init(&pmt_stream, audStreamType, pscan_results->program_info[program].audio_pids[0].pid);
        TS_PMT_addStream(&pmtState, &pmt_stream, &streamNum);
    }

    TS_PAT_finalize(&patState, &pat_pl_size);
    TS_PMT_finalize(&pmtState, &pmt_pl_size);

    pat = buffer;
    pmt = pat+188;

    pat[0] = 0x47;
    pat[1] = 0x40; /* TEI = 0, Payload Unit Start = 1, Transport Priority = 0, 13 bit-pid# = 0 */
    pat[2] = 0x00;
    pat[3] = 0x10; /* scrambling = 0, adaptation field = 0, continuity counter = 0 */
    pat[4] = 0x00; /* pointer = 0 */
    BKNI_Memcpy(pat + 5, pat_pl_buf, pat_pl_size); /* PAT table */
    BKNI_Memset(pat + 5 + pat_pl_size, 0xff, 188 - 5 - pat_pl_size); /* stuffing bytes */

    pmt[0] = 0x47;
    pmt[1] = 0x40 | ((pmtPid >> 8) & 0x1f); /* TEI = 0, PUSI= 1, TP=0, 13-bit pid# */
    pmt[2] = pmtPid & 0xff;
    pmt[3] = 0x11; /* scrambling = 0, adaptation field = 0, continuity counter = 1 */
    pmt[4] = 0x00; /* pointer = 0 */
    BKNI_Memcpy(pmt + 5, pmt_pl_buf, pmt_pl_size); /* PMT table */
    BKNI_Memset(pmt + 5 + pmt_pl_size, 0xff, 188 - 5 - pmt_pl_size); /* stuffing bytes */

    *pSizeUsed = PAT_PMT_PAIR_SIZE;

    return 0;
}

NEXUS_PidChannelHandle bpsi_injection_get_pid_channel(bpsi_injection_t handle)
{
    return handle->pidChannel;
}

bpsi_injection_t bpsi_injection_open(NEXUS_ParserBand parserBand, const tspsimgr_scan_results *pscan_results, unsigned program)
{
    bpsi_injection_t handle;
    int rc;
    uint8_t *pkt;
    unsigned size;
    NEXUS_PacketSubOpenSettings packetSubOpenSettings;
    NEXUS_PacketSubSettings packetSubSettings;
    NEXUS_PidChannelSettings pidChannelSettings;

    handle = BKNI_Malloc(sizeof(*handle));
    if (!handle) return NULL;
    BKNI_Memset(handle, 0, sizeof(*handle));

    NEXUS_PidChannel_GetDefaultSettings(&pidChannelSettings);
    handle->pidChannel = NEXUS_PidChannel_Open(parserBand, 0x1ffe, &pidChannelSettings);

    NEXUS_PacketSub_GetDefaultOpenSettings(&packetSubOpenSettings);
    packetSubOpenSettings.fifoSize = 1024;
    handle->packetSub = NEXUS_PacketSub_Open(NEXUS_ANY_ID, &packetSubOpenSettings);

    NEXUS_PacketSub_GetSettings(handle->packetSub, &packetSubSettings);
    packetSubSettings.pidChannel = handle->pidChannel;
    packetSubSettings.loop = false;
    rc = NEXUS_PacketSub_SetSettings(handle->packetSub, &packetSubSettings);
    if (rc) {rc = BERR_TRACE(rc); goto error;}

    rc = NEXUS_PacketSub_GetBuffer(handle->packetSub, (void **)&pkt, &size);
    if (rc) {rc = BERR_TRACE(rc); goto error;}

    rc = build_psi(0x55, pkt, size, &size, pscan_results, program);
    if (rc) {rc = BERR_TRACE(rc); goto error;}

    rc = NEXUS_PacketSub_WriteComplete(handle->packetSub, size);
    if (rc) {rc = BERR_TRACE(rc); goto error;}

    rc = NEXUS_PacketSub_Start(handle->packetSub);
    if (rc) {rc = BERR_TRACE(rc); goto error;}

    return handle;

error:
    bpsi_injection_close(handle);
    return NULL;
}

void bpsi_injection_close(bpsi_injection_t handle)
{
    if (handle->packetSub) {
        NEXUS_PacketSub_Close(handle->packetSub);
    }
    if (handle->pidChannel) {
        NEXUS_PidChannel_Close(handle->pidChannel);
    }
    BKNI_Free(handle);
}

void *bdevice_mem_alloc(unsigned size, bool full)
{
    NEXUS_ClientConfiguration clientConfig;
    NEXUS_MemoryAllocationSettings settings;
    unsigned i;
    NEXUS_Platform_GetClientConfiguration(&clientConfig);
    NEXUS_Memory_GetDefaultAllocationSettings(&settings);
    for (i=0;i<NEXUS_MAX_HEAPS;i++) {
        void *buffer;
        int rc;
        if (!clientConfig.heap[i]) continue;
        if (full) {
            NEXUS_MemoryStatus status;
            rc = NEXUS_Heap_GetStatus(clientConfig.heap[i], &status);
            if (rc || status.memoryType != NEXUS_MemoryType_eFull) continue;
        }
        settings.heap = clientConfig.heap[i];
        rc = NEXUS_Memory_Allocate(size, &settings, &buffer);
        if (!rc) return buffer;
    }
    return NULL;
}

void bdevice_mem_free(void *ptr)
{
    NEXUS_Memory_Free(ptr);
}

/*****************************************************/

struct bchannel_scan
{
    NEXUS_FrontendHandle frontend;
    NEXUS_ParserBand parserBand;
    tspsimgr_t tspsimgr;
    bchannel_scan_start_settings settings;
};

void bchannel_scan_get_default_start_settings(struct bchannel_scan_start_settings *psettings)
{
    memset(psettings, 0, sizeof(*psettings));
}

bchannel_scan_t bchannel_scan_start(const struct bchannel_scan_start_settings *psettings)
{
    bchannel_scan_t scan;
    tspsimgr_scan_settings scan_settings;
    tune_settings settings;
    int rc;

    scan = malloc(sizeof(*scan));
    if (!scan) return NULL;
    memset(scan, 0, sizeof(*scan));
    scan->settings = *psettings;

    if (psettings->source != channel_source_streamer) {
#if NEXUS_HAS_FRONTEND
        NEXUS_FrontendAcquireSettings settings;
        NEXUS_Frontend_GetDefaultAcquireSettings(&settings);
        settings.capabilities.qam = (psettings->source == channel_source_qam);
        settings.capabilities.ofdm = (psettings->source == channel_source_ofdm);
        settings.capabilities.satellite = (psettings->source == channel_source_sat);
        scan->frontend = NEXUS_Frontend_Acquire(&settings);
        if (!scan->frontend) {
            BDBG_WRN(("Unable to find capable frontend"));
            goto error;
        }
#endif
    }

    scan->parserBand = NEXUS_ParserBand_Open(NEXUS_ANY_ID);
    if (!scan->parserBand) {
        goto error;
    }

    scan->tspsimgr = tspsimgr_create();
    if (!scan->tspsimgr) {
        goto error;
    }

    get_default_tune_settings(&settings);
    if (psettings->source == channel_source_qam) {
        settings.qam.freq = psettings->freq;
        settings.qam.mode = 64;
    }
    else if (psettings->source == channel_source_ofdm) {
        settings.ofdm.freq = psettings->freq;
    }
    else {
        settings.streamer.index = psettings->freq;
    }
    rc = tune(scan->parserBand, scan->frontend, &settings);
    if (rc) goto error;

    /* TODO: also fire scan_done if tune cannot acquire */
    tspsimgr_get_default_start_scan_settings(&scan_settings);
    scan_settings.parserBand = scan->parserBand;
    scan_settings.scan_done = psettings->scan_done;
    scan_settings.context = psettings->context;
    rc = tspsimgr_start_scan(scan->tspsimgr, &scan_settings);
    BDBG_ASSERT(!rc);

    return scan;

error:
    bchannel_scan_stop(scan);
    return NULL;
}

int bchannel_scan_get_results(bchannel_scan_t scan, tspsimgr_scan_results *presults)
{
    return tspsimgr_get_scan_results(scan->tspsimgr, presults);
}

void bchannel_scan_get_resources(bchannel_scan_t scan, NEXUS_FrontendHandle *pFrontend, NEXUS_ParserBand *pParserBand)
{
    *pFrontend = scan->frontend;
    *pParserBand = scan->parserBand;
}

void bchannel_scan_stop(bchannel_scan_t scan)
{
    if (scan->tspsimgr) {
        tspsimgr_destroy(scan->tspsimgr);
    }
    if (scan->parserBand) {
        NEXUS_ParserBand_Close(scan->parserBand);
    }
#if NEXUS_HAS_FRONTEND
    if (scan->frontend) {
        NEXUS_Frontend_Release(scan->frontend);
    }
#endif
    free(scan);
}

void bchannel_source_print(char *str, unsigned n, enum channel_source source, unsigned freq)
{
#if NEXUS_HAS_FRONTEND
    if (freq) {
        snprintf(str, n, "%s %dMhz", channel_source_str[source], freq);
    }
    else
#endif
    {
        snprintf(str, n, "Streamer %d", freq);
    }
}
