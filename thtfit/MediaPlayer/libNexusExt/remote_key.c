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
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: $
 *
 *****************************************************************************/
#include "bstd.h"
#include "remote_key.h"
#include "bkni_multi.h"

BDBG_MODULE(remote_key);

struct keymap {
    unsigned ir_input_a;
    unsigned ir_cir_nec;
    unsigned keyboard;
} g_input_keymap[] = {
                       /* ir_a, cir_nec     keyboard */
    /* unknown */      {0,      0,          0},
    /* play */         {0x5038, 0xe21dff00, 25  /* P */},
    /* pause */        {0x1f,   0xe31cff00, 119 /* PAUSE/BREAK */},
    /* fast_forward */ {0x201d, 0xa659ff00, 33  /* F */},
    /* rewind */       {0x101e, 0xe619ff00, 19  /* R */},
    /* stop */         {0x4039, 0xa35cff00, 57  /* SPACE */},
    /* clear */        {0xd012, 0xb24dff00, 111 /* DELETE */},
    /* back */         {0x303a, 0xf906ff00, 14  /* BACKSPACE */},
    /* up */           {0x9034, 0xb14eff00, 103},
    /* down */         {0x8035, 0xf30cff00, 108},
    /* right */        {0x6037, 0xb649ff00, 106},
    /* left */         {0x7036, 0xf40bff00, 105},
    /* select */       {0xe011, 0xf708ff00, 28  /* RETURN */},
    /* power */        {0x600A, 0xf50aff00, 142 /* SLEEP */},
    /* chan_up */      {0, 0xf609ff00, 0},
    /* chan_down */    {0, 0xf20dff00, 0}
};

unsigned b_get_remote_code(NEXUS_IrInputMode irInput, b_remote_key key)
{
    if (key >= b_remote_key_max) {
        BERR_TRACE(NEXUS_INVALID_PARAMETER);
        return 0xFFFFFFFF;
    }
    switch (irInput) {
    case NEXUS_IrInputMode_eCirNec:
        return g_input_keymap[key].ir_cir_nec;
    case NEXUS_IrInputMode_eRemoteA:
        return g_input_keymap[key].ir_input_a;
    default:
        BERR_TRACE(NEXUS_INVALID_PARAMETER);
        return 0xFFFFFFFF;
    }
}

b_remote_key b_get_remote_key(NEXUS_IrInputMode irInput, unsigned code)
{
    b_remote_key key;

    for (key=1;key<b_remote_key_max;key++) {
        switch (irInput) {
        case NEXUS_IrInputMode_eCirNec:
            if (g_input_keymap[key].ir_cir_nec == code) return key;
            break;
        case NEXUS_IrInputMode_eRemoteA:
            if (g_input_keymap[key].ir_input_a == code) return key;
            break;
        default:
            BERR_TRACE(NEXUS_INVALID_PARAMETER);
            return b_remote_key_unknown;
        }
    }

    BDBG_MSG(("unknown key %#x", code));
    return b_remote_key_unknown;
}

b_remote_key b_get_evdev_key(unsigned code)
{
    b_remote_key key;

    for (key=1;key<b_remote_key_max;key++) {
        if (g_input_keymap[key].keyboard == code) return key;
    }

    BERR_TRACE(NEXUS_INVALID_PARAMETER);
    return b_remote_key_unknown;
}

int b_read_remote_key(NEXUS_InputClientHandle inputClient, b_remote_key *key)
{
    NEXUS_InputRouterCode code;
    unsigned num;
    int rc;
    
    rc = NEXUS_InputClient_GetCodes(inputClient, &code, 1, &num);
    if (rc || !num) {
        return -1;
    }
    switch (code.deviceType) {
    case NEXUS_InputRouterDevice_eIrInput:
        if (!code.data.irInput.repeat) {
            *key = b_get_remote_key(code.data.irInput.mode, code.data.irInput.code);
            return 0;
        }
        break;
    case NEXUS_InputRouterDevice_eEvdev:
        /* evdev sends key up and key down status in the value, so ignore one of them to avoid double keypresses */
        if (code.data.evdev.type == 1 /* EV_KEY */ && code.data.evdev.value) {
            *key = b_get_evdev_key(code.data.evdev.code);
            return 0;
        }
        break;
    default:
        break;    
    }
    *key = b_remote_key_unknown;
    return 0;
}
