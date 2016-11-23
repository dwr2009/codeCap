
#include "picdecoder.h"
#include "NexusDecoderApi.h"
#include <nxclient.h>
#include <string.h>
#include <nexus_picture_decoder.h>
#include <nexus_i2c.h>
#include "nexus_platform.h"
#include "nexus_platform_client.h"
#include "nexus_surface.h"
#include "nexus_surface_client.h"



#ifndef __NEXUSDECODER_H__
#define __NEXUSDECODER_H__




class NexusDecoderImpl : public NexusDecoder
{
public:
    NexusDecoderImpl();
    virtual ~NexusDecoderImpl();
    virtual int decoder(char *strFile);
    virtual int decoder(unsigned char *pDataBuffer,int iDataBufferSize,int iPictureType);
    virtual int getPictureInfo(int &iWidth,int &iHeight,int &iLineSize,int &iPixelFormat);
    virtual int getPictureData(char *pDataBuffer,int iBufferSize,int iX,int iY,int iWidth,int iHeight,NexusDecoder_PixelFormat iPixelFormat=NexusDecoder_PixselFormat_A8_R8_G8_B8);
protected:
    virtual int joinNxClient(char * pszClientName = NULL);
    virtual int unjoinNxClient();
    virtual int configNxClient();
    virtual int unconfigNxClient();
        
    static void complete(void *data, int unused)
    {
        BSTD_UNUSED(unused);
        BKNI_SetEvent(reinterpret_cast<BKNI_EventHandle>(data));
    };

    
protected:
    int m_bNexusInited;
    int m_bNexusConfiged;
    NEXUS_ClientConfiguration m_clientConfig;
    NxClient_AllocSettings m_allocSettings;
    NxClient_AllocResults m_allocResults;
    NEXUS_SurfaceHandle m_surfaceHandle;
    NEXUS_SurfaceMemory m_surfaceMemory;
    NEXUS_SurfaceCreateSettings m_surfaceCreateSettings;    
    NEXUS_SurfaceClientHandle m_surfaceClientHandle;
    NEXUS_SurfaceClientSettings m_surfaceClientSettings;
    BKNI_EventHandle m_bkniEventHandle;
    
    picdecoder_t m_decoderHandle;
    BKNI_EventHandle m_checkpointEvent;
    
    NEXUS_Graphics2DSettings m_gfxSettings;
    NEXUS_Graphics2DHandle m_gfxHandle;
    
};






#endif


