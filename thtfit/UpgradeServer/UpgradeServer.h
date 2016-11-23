#ifndef _UPGRADE_SERVER_H_
#define _UPGRADE_SERVER_H_s

#include <stdio.h>
#include "nexus_types.h"
#include "nexus_platform.h"
#include "nexus_display.h"
#include "nexus_composite_output.h"
#include "nexus_component_output.h"
#if NEXUS_HAS_HDMI_OUTPUT
#include "nexus_hdmi_output.h"
#endif
#include "nexus_core_utils.h"
#include "bfont.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <unistd.h>
#include <errno.h>
#include "BaseDef.h"

#define FONTNAME							"/lib/arial_18_aa.bwin_font"
#define BORDER 								40
#define LINESPACE 							3


class UpgradeServer
{
public:
	UpgradeServer();
	~UpgradeServer();
	int initInstance();
	int mainLoop();
	int drawFont(const char* text,  unsigned long clrText);
	int openFifoFile(const char* path);
	void closeFifoFile();
	void simple_fill(const NEXUS_SurfaceMemory *compositorBufferMem, const NEXUS_Rect *rect, NEXUS_Pixel pixel);

private:
	NEXUS_DisplayHandle 			m_display;
    NEXUS_SurfaceCreateSettings 	m_surfaceCfg;
	NEXUS_SurfaceHandle 			m_frame;
	NEXUS_SurfaceMemory 			m_mem;
	NEXUS_Rect 						m_rect;
	int 							m_UpgradeServerFd;
	bfont_t 						m_font;
	unsigned 						m_fontheight;
	struct bfont_surface_desc 		m_desc;
	
	
};




#endif

