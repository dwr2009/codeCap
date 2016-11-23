#include "UpgradeServer.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <memory.h>

UpgradeServer::UpgradeServer()
{
	m_UpgradeServerFd=-1;
	m_display=NULL;
	m_frame=NULL;
	m_font=NULL;
	m_fontheight=0;
	memset(&m_mem,0,sizeof(NEXUS_SurfaceMemory));
	memset(&m_desc,0,sizeof(bfont_surface_desc));
	memset(&m_rect,0,sizeof(m_rect));
}

UpgradeServer::~UpgradeServer()
{
	if(m_display)
	{
		NEXUS_StopCallbacks(m_display);
    	NEXUS_Display_RemoveAllOutputs(m_display);
    	NEXUS_Display_Close(m_display);
		m_display=NULL;
	}

	if(m_frame)
	{
    	NEXUS_Surface_Destroy(m_frame);
		m_frame=NULL;
	}
	
	if(m_font)
	{
		bfont_close(m_font);
		m_font=NULL;
	}
	
	closeFifoFile();
    NEXUS_Platform_Uninit();
}

int UpgradeServer::initInstance()
{
	int iOutRet=0,iRet;
	NEXUS_Error 					errCode;
	NEXUS_PlatformSettings 			platformSettings;
	NEXUS_PlatformConfiguration 	platformConfig;
	NEXUS_DisplaySettings 			displaySettings;	
    NEXUS_GraphicsSettings 			graphicsCfg;
	
#if NEXUS_NUM_HDMI_OUTPUTS	
	NEXUS_HdmiOutputStatus 			hdmiStatus;
#endif

	NEXUS_Platform_GetDefaultSettings(&platformSettings);
    platformSettings.openFrontend = false;
	errCode = NEXUS_Platform_Init(&platformSettings);
    if (errCode) 
	{
	
        fprintf(stderr, "Unable to initialize nexus, err %d\n", errCode);
		iOutRet=errCode;
		goto PROC_EXIT;
	}

	NEXUS_Platform_GetConfiguration(&platformConfig);
	NEXUS_Display_GetDefaultSettings(&displaySettings);
    displaySettings.displayType = NEXUS_DisplayType_eAuto;
    displaySettings.format = NEXUS_VideoFormat_e720p;
    m_display = NEXUS_Display_Open(0, &displaySettings);

#if NEXUS_NUM_COMPONENT_OUTPUTS
    NEXUS_Display_AddOutput(m_display, NEXUS_ComponentOutput_GetConnector(platformConfig.outputs.component[0]));
#endif

#if NEXUS_NUM_COMPOSITE_OUTPUTS
    NEXUS_Display_AddOutput(m_display, NEXUS_CompositeOutput_GetConnector(platformConfig.outputs.composite[0]));
#endif

#if NEXUS_NUM_HDMI_OUTPUTS
    NEXUS_Display_AddOutput(m_display, NEXUS_HdmiOutput_GetVideoConnector(platformConfig.outputs.hdmi[0]));
    errCode = NEXUS_HdmiOutput_GetStatus(platformConfig.outputs.hdmi[0], &hdmiStatus);
    if ( !errCode && hdmiStatus.connected )
    {
        NEXUS_Display_GetSettings(m_display, &displaySettings);
        if ( !hdmiStatus.videoFormatSupported[displaySettings.format] ) 
		{
            displaySettings.format = hdmiStatus.preferredVideoFormat;
            NEXUS_Display_SetSettings(m_display, &displaySettings);
		}
    }
#endif

	NEXUS_Display_GetGraphicsSettings(m_display, &graphicsCfg);
    graphicsCfg.frameBufferCallback.callback = NULL;
    graphicsCfg.frameBufferCallback.context = NULL;
    errCode = NEXUS_Display_SetGraphicsSettings(m_display, &graphicsCfg);
    if(errCode)
    {
    	iOutRet=errCode;
		goto PROC_EXIT;
    }

	iRet=openFifoFile(BTN_UPGRADESERVER_PIPE_PATH);
	if(iRet)
	{
		iOutRet=iRet;
		goto PROC_EXIT;
	}

	NEXUS_Surface_GetDefaultCreateSettings(&m_surfaceCfg);
	
    m_surfaceCfg.width 	= 1280;
    m_surfaceCfg.height = 720;
	m_surfaceCfg.pixelFormat = NEXUS_PixelFormat_eA8_R8_G8_B8;
    m_surfaceCfg.heap 	= NEXUS_Platform_GetFramebufferHeap(0);
    m_frame 			= NEXUS_Surface_Create(&m_surfaceCfg);
    if(!m_frame) 
	{
        fprintf(stderr, "Can't create frame buffer\n");
		iOutRet=ERR_CREATE_SURFACE;
        goto PROC_EXIT;
    }
	
    NEXUS_Surface_GetMemory(m_frame, &m_mem);
    simple_fill(&m_mem, &graphicsCfg.clip, 0);

	m_font = bfont_open(FONTNAME);
	if(!m_font)
	{
		iOutRet=ERR_OPEN_FONT;
        goto PROC_EXIT;
	}
	
	bfont_get_height(m_font, &m_fontheight);

	bfont_get_surface_desc(m_frame, &m_desc);

	return iOutRet;
	
PROC_EXIT:
	
	if(m_display)
	{
		NEXUS_Display_Close(m_display);
		m_display=NULL;
	}

	if(m_font)
	{
		bfont_close(m_font);
		m_font=NULL;
	}

	closeFifoFile();
	
	return iOutRet;
	
}

int UpgradeServer::mainLoop()
{
	int iOutRet=0,iRet;
	struct timeval tvTimeout; 
	tvTimeout.tv_sec = 0; 
	tvTimeout.tv_usec = 150*1000;

	while(true)
	{
		fd_set fdsRead;
		FD_ZERO(&fdsRead); 
		FD_SET(m_UpgradeServerFd, &fdsRead);
		iRet = select(m_UpgradeServerFd + 1, &fdsRead, NULL, NULL, &tvTimeout);
		if(iRet<0)
		{
			if (errno == EINTR) 
			{
                printf("%s: select System Call interrupted, retrying\n", __FUNCTION__);
                continue;
            }
            printf("%s: ERROR: select(): errno = %d", __FUNCTION__, errno);
            return -1;
		}
		
		if(FD_ISSET(m_UpgradeServerFd, &fdsRead))
		{
			char buf[1024]={0};
			iRet = read(m_UpgradeServerFd, buf, sizeof(buf));
			
			if(-1 == iRet)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			}
			else if(0<iRet)
			{	
				buf[iRet]=0;
				iRet=drawFont(buf,0xFFCCCCCC);
				if(iRet)
				{
					printf("[%s:%d] draw Font error\n", __FILE__, __LINE__);
				}
			}
		}
		
	}
	

	return iOutRet;
}


int UpgradeServer::drawFont(const char* text,  unsigned long clrText)
{
	int iOutRet=0,iRet;
	
	if (!m_rect.x) 
	{
		m_rect.x=BORDER;
		m_rect.y=BORDER;
		m_rect.height=m_fontheight;	
		m_rect.width=m_surfaceCfg.width-2*BORDER;
		memset(m_mem.buffer, 0, m_surfaceCfg.height * m_mem.pitch);
	}

	m_rect.y += m_rect.height + LINESPACE;

	if (m_rect.y >= m_surfaceCfg.height - BORDER*2) 
	{
		m_rect.x = 0;
		iOutRet=ERR_OUT_OF_SURFACE;
		goto PROC_EXIT;
	}

	bfont_draw_aligned_text(&m_desc, m_font, &m_rect,text, \
		-1, clrText, bfont_valign_top, bfont_halign_left);

	NEXUS_Surface_Flush(m_frame);
	iRet = NEXUS_Display_SetGraphicsFramebuffer(m_display, m_frame);
	if(iRet)
	{
		printf("errCode:%s\n",iRet);
		iOutRet=iRet;
		goto PROC_EXIT;
	}

PROC_EXIT:
	
	return iOutRet;
}


int UpgradeServer::openFifoFile(const char* path)
{	
	int iOutRet=0,iRet;

	iRet = remove(path);
	if(0 != iRet)
	{
		if(ENOENT != errno)
		{
			iOutRet = ERR_SYS_remove_FAIL;
			goto PROC_EXIT;
		}
	}
	
	iRet = mkfifo(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if(-1 == iRet)
	{
		iOutRet = -2;
		goto PROC_EXIT;
	}

	m_UpgradeServerFd = open(path, O_RDWR | O_NONBLOCK);
	if(0 > m_UpgradeServerFd)
	{
		iOutRet = -3;
		goto PROC_EXIT;
	}
	
PROC_EXIT:
	
	return iOutRet;
}

void UpgradeServer::closeFifoFile()
{
	if(-1 != m_UpgradeServerFd)
	{
		close(m_UpgradeServerFd);
		m_UpgradeServerFd = -1;
		remove(BTN_UPGRADESERVER_PIPE_PATH);
	}
}


void UpgradeServer::simple_fill(const NEXUS_SurfaceMemory *compositorBufferMem, const NEXUS_Rect *rect, NEXUS_Pixel pixel)
{
    unsigned x,y;
    unsigned width = rect->width;
    for(y=0;y<rect->height;y++) 
	{
        uint32_t *buf = (uint32_t *)((uint8_t *)compositorBufferMem->buffer + compositorBufferMem->pitch*(y+rect->y))+rect->x;
        for(x=0;x<width;x++) 
		{
            buf[x] = pixel;
        }
    }
}


UpgradeServer* g_UpgradeServer=new UpgradeServer; 

int main(int argc,char*argv[])
{
	int iOutRet=ERR_SUCCESS,iRet;
	if(!g_UpgradeServer)
	{
		iOutRet=ERROR_OUT_OF_MEMORY;
		goto PROC_EXIT;
	}
	
	iRet=g_UpgradeServer->initInstance();
	if(iRet)
	{
		iOutRet=iRet;
		goto PROC_EXIT;
	}
	
	
	iRet=g_UpgradeServer->mainLoop();
	if(iRet)
	{
		iOutRet=iRet;
		goto PROC_EXIT;
	}

PROC_EXIT:

	if(!g_UpgradeServer)
	{
		delete g_UpgradeServer;
	}
	
	return iOutRet;
}

