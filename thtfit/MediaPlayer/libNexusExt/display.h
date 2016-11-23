#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <nxclient.h>
#include "MediaPlayerAppDef.h"
#include "BaseErrDef.h"
#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

DECLARE_EXTERN_C_BLOCK_START

int VideoFormatConvert(VIDEO_OUTPUT_MODE  * mode, NEXUS_VideoFormat * format);
INT_t NexusVidFmt_to_VidOutMode(CONST NEXUS_VideoFormat nexusVidFmt, VIDEO_OUTPUT_MODE * pVidOutMode);
INT_t VidOutMode_to_NexusVidFmt(CONST VIDEO_OUTPUT_MODE eVidOutMode, NEXUS_VideoFormat * pNexusVidFmt);

int ChangeVidOutputMode(NEXUS_VideoFormat newFormat);
int ChangeVidColorSpace(NEXUS_ColorSpace newcolspace);
int ChangeVidColorDepth(unsigned colorDepth);


NEXUS_VideoFormat getCurNexusDispVidFmt();

DECLARE_EXTERN_C_BLOCK_END

#endif /* DISPLAY_H_ */

