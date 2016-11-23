
#include "NexusDecoder.h"
#include <nxclient.h>
#include <string.h>
//#include <ErrPrintHelper.h>
#include <nexus_i2c.h>
#include "nexus_platform.h"
#include "nexus_cec.h"
#include <DateTime.h>
//#include <ProcessRunningInfo.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h> 
//#include <SharedPtr.h>


//static pthread_mutex_t g_mutexDecoder = PTHREAD_MUTEX_INITIALIZER;

NexusDecoderImpl::NexusDecoderImpl()
    :m_bNexusInited(0),m_bNexusConfiged(0),
    m_surfaceHandle(0),m_decoderHandle(0),m_gfxHandle(0)
{
    joinNxClient();
    configNxClient();
}


NexusDecoderImpl::~NexusDecoderImpl()
{
    if(NULL!=m_surfaceHandle)
    {
        NEXUS_Surface_Destroy(m_surfaceHandle);
        m_surfaceHandle=NULL;
    }
    unconfigNxClient();
    unjoinNxClient();
    
}

int NexusDecoderImpl::decoder(unsigned char *pDataBuffer,int iDataBufferSize,int iPictureType)
{
    int iRetValue=0;
    picdecoder_t handle;
    NEXUS_SurfaceHandle pictureSurface=NULL;
    int iCurPicType;
    
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    if(!pDataBuffer)
    {
        return -1;
    }
    switch(iPictureType)
    {
    case NexusDecoder_PictureMpeg:
        iCurPicType=NEXUS_PictureFormat_eMpeg;
        break;
    case NexusDecoder_PictureJpeg:
        iCurPicType=NEXUS_PictureFormat_eJpeg;
        break;
    case NexusDecoder_PictureGif:
        iCurPicType=NEXUS_PictureFormat_eGif;
        break;
    case NexusDecoder_PicturePng:
        iCurPicType=NEXUS_PictureFormat_ePng;
        break;
    default:
        return -1;
    }
    
    iRetValue=configNxClient();
    if(iRetValue!=0)
    {
        return -1;
    }
    
    //pthread_mutex_lock(&g_mutexDecoder);
    printf("%s->%s(%d) picdecoder start \n",__FILE__,__FUNCTION__,__LINE__);
    handle = picdecoder_open();
    if (!handle) 
    {
        printf("%s->%s(%d) picdecoder open failed \n",__FILE__,__FUNCTION__,__LINE__);
        //pthread_mutex_unlock(&g_mutexDecoder);
        return -1;
    }
    iRetValue=-1;
    printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    pictureSurface=picdecoder_decode_source_buffer(handle,pDataBuffer,iDataBufferSize,iCurPicType);
    if(pictureSurface!=NULL)
    {
        printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
        if(m_surfaceHandle!=NULL)
        {
            NEXUS_Surface_Destroy(m_surfaceHandle);
            m_surfaceHandle=NULL;
        }
        m_surfaceHandle=pictureSurface;
        NEXUS_Surface_GetMemory(pictureSurface, &m_surfaceMemory);
        iRetValue=0;     
    }
    picdecoder_close(handle);
    printf("%s->%s(%d) picdecoder end \n",__FILE__,__FUNCTION__,__LINE__);
    //pthread_mutex_unlock(&g_mutexDecoder);
    return iRetValue;
}


int NexusDecoderImpl::decoder(char *strFile)
{
    int iRetValue=0;
    picdecoder_t handle;
    NEXUS_SurfaceHandle pictureSurface;
    
    if(!strFile)
    {
        return -1;
    }
    
    iRetValue=configNxClient();
    if(iRetValue!=0)
    {
        return -1;
    }
    handle = picdecoder_open();
    if (!handle) 
    {
        //BDBG_ERR(("not supported on this platform"));
        return -1;
    }
    pictureSurface=picdecoder_decode_source(handle,strFile);
    if(pictureSurface!=NULL)
    {
        if(m_surfaceHandle!=NULL)
        {
            NEXUS_Surface_Destroy(m_surfaceHandle);
            m_surfaceHandle=NULL;
        }
        m_surfaceHandle=pictureSurface;
        NEXUS_Surface_GetMemory(pictureSurface, &m_surfaceMemory);        
    }
    picdecoder_close(handle);
    return 0;
}

int  NexusDecoderImpl::getPictureInfo(int &iWidth,int &iHeight,int &iLineSize,int &iPixelFormat)
{
    NEXUS_SurfaceCreateSettings surfaceSettings;
    
    if(NULL==m_surfaceHandle)
    {
        return -1;
    }
    NEXUS_Surface_GetCreateSettings(m_surfaceHandle,&surfaceSettings);
    
    iPixelFormat=surfaceSettings.pixelFormat;
    iWidth=surfaceSettings.width;
    iHeight=surfaceSettings.height;
    iLineSize=surfaceSettings.pitch;    
    return 0;
}

int NexusDecoderImpl::joinNxClient(char * pszClientName)
{
    int iOutRet = 0;
    NxClient_JoinSettings oNxJoinSettings;
    NEXUS_Error nexusErr;

    NxClient_GetDefaultJoinSettings(&oNxJoinSettings);
    if(NULL != pszClientName)
    {    
        snprintf(oNxJoinSettings.name, sizeof(oNxJoinSettings.name), "%s", pszClientName);
        oNxJoinSettings.name[sizeof(oNxJoinSettings.name)-1] = '\0';
    }
    oNxJoinSettings.tunnelCapable = FALSE;
    oNxJoinSettings.timeout = 0;
    oNxJoinSettings.session = 0;

    UINT64_t uSysUpTimeMs, uSysUpStartTimeMs;
    uSysUpStartTimeMs = GetSysUpTimeMs();
RETRY_NX_JOIN:
    nexusErr = NxClient_Join(&oNxJoinSettings);
    if(NEXUS_SUCCESS != nexusErr)
    {
        uSysUpTimeMs = GetSysUpTimeMs();
        if((5*1000) > (uSysUpTimeMs - uSysUpStartTimeMs))
        {
            sleep(1);
            goto RETRY_NX_JOIN;
           
        }
        iOutRet = -1;
        goto EXIT_PROC;
    }
    else
    {
        m_bNexusInited=1;
    }

EXIT_PROC:
    return iOutRet;
    
}

int NexusDecoderImpl::unjoinNxClient()
{
    if(m_bNexusInited>0)
    {
        NxClient_Uninit();
        m_bNexusInited=0;
    }
    return 0;
}

int NexusDecoderImpl::configNxClient()
{
    NEXUS_Error rc;
    NEXUS_Graphics2DBlitSettings blitSettings;
    NEXUS_Graphics2DOpenSettings openSettings;
    NEXUS_Graphics2DSettings gfxSettings;
    NEXUS_Graphics2DHandle gfx;
    
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    if(m_bNexusConfiged>0)
    {
        return 0;
    }
    NEXUS_Platform_GetClientConfiguration(&m_clientConfig);
    BKNI_CreateEvent(&m_bkniEventHandle);
    BKNI_CreateEvent(&m_checkpointEvent);

    NxClient_GetDefaultAllocSettings(&m_allocSettings);
    m_allocSettings.surfaceClient = 1;
    rc = NxClient_Alloc(&m_allocSettings, &m_allocResults);
    if (rc)
    {
        printf("%s->%s(%d) NxClient_Alloc FAILED\n",__FILE__,__FUNCTION__,__LINE__);
        goto out_err1;
    }

    /* No NxClient_Connect needed for SurfaceClient */

    m_surfaceClientHandle = NEXUS_SurfaceClient_Acquire(m_allocResults.surfaceClient[0].id);
    if (!m_surfaceClientHandle)
    {
        //BDBG_ERR(("NEXUS_SurfaceClient_Acquire failed"));
        printf("%s->%s(%d) NEXUS_SurfaceClient_Acquire FAILED\n",__FILE__,__FUNCTION__,__LINE__);
        goto out_err2;
    }
    
   
    
    NEXUS_Graphics2D_GetDefaultOpenSettings(&openSettings);
    openSettings.packetFifoSize = 1024; /* only single destripe blit queued */
    gfx = NEXUS_Graphics2D_Open(NEXUS_ANY_ID, &openSettings);
    if (!gfx) 
    {
        BERR_TRACE(NEXUS_OUT_OF_SYSTEM_MEMORY);
        printf("%s->%s(%d) NEXUS_Graphics2D_Open FAILED\n",__FILE__,__FUNCTION__,__LINE__);
        goto out_err2;
    }
    
    NEXUS_Graphics2D_GetSettings(gfx, &gfxSettings);
    gfxSettings.checkpointCallback.callback = complete;
    gfxSettings.checkpointCallback.context = m_checkpointEvent;
    rc = NEXUS_Graphics2D_SetSettings(gfx, &gfxSettings);
    if (rc)
    {
        //BERR_TRACE(rc);
        printf("%s->%s(%d) NEXUS_Graphics2D_SetSettings FAILED\n",__FILE__,__FUNCTION__,__LINE__);
        goto out_err3;
    }
    m_gfxHandle=gfx;
    m_gfxSettings=gfxSettings;
    m_bNexusConfiged=1;
    //printf("%s->%s(%d) CONFIG SUCCESS\n",__FILE__,__FUNCTION__,__LINE__);

    return 0;   
out_err3:
    NEXUS_Graphics2D_Close(gfx);
out_err2:
    NxClient_Free(&m_allocResults);
out_err1:
    BKNI_DestroyEvent(m_bkniEventHandle);
    BKNI_DestroyEvent(m_checkpointEvent);
    m_bkniEventHandle=NULL;
    printf("%s->%s(%d) FAILED\n",__FILE__,__FUNCTION__,__LINE__);
    return -1;    
    
}

int NexusDecoderImpl::unconfigNxClient()
{
    if(m_bNexusConfiged<=0)
    {
        return 0;
    }
    if(m_gfxHandle!=NULL)
    {
        NEXUS_Graphics2D_Close(m_gfxHandle);
        m_gfxHandle=NULL;
    }
    NxClient_Free(&m_allocResults);
    BKNI_DestroyEvent(m_bkniEventHandle);
    BKNI_DestroyEvent(m_checkpointEvent);    
    m_bNexusConfiged=0;
}

int NexusDecoderImpl::getPictureData(char *pDataBuffer,int iBufferSize,int iX,int iY,int iWidth,int iHeight,NexusDecoder_PixelFormat iPixelFormat)
{
    NEXUS_Error rc;
    int iDepth=32,iActualPictureSize=0;
    NEXUS_SurfaceCreateSettings createSettings;    
    NEXUS_Graphics2DBlitSettings blitSettings;
    NEXUS_SurfaceCreateSettings srcSurfaceSettings;
    NEXUS_SurfaceHandle destSurfaceHandle;
    NEXUS_SurfaceMemory mem;
    int iCopySize=0;
    if(NULL==m_surfaceHandle)
    {
        printf("%s->%s(%d) m_surfaceHandle is null \n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    if(NULL==pDataBuffer)
    {
        printf("%s->%s(%d) pDataBuffer is null\n",__FILE__,__FUNCTION__,__LINE__);
        return -1;
    }
    NEXUS_Surface_GetCreateSettings(m_surfaceHandle,&srcSurfaceSettings);
    
    if((iX>=srcSurfaceSettings.width)||(iY>=srcSurfaceSettings.height))
    {
        return -1;
    }
    
    if(((iX+iWidth)>srcSurfaceSettings.width)||((iY+iHeight)>srcSurfaceSettings.height))
    {
        return -1;
    }
    NEXUS_Surface_GetDefaultCreateSettings(&createSettings);
    switch(iPixelFormat)
    {
    case NexusDecoder_PixselFormat_A8_R8_G8_B8:
        createSettings.pixelFormat=NEXUS_PixelFormat_eA8_R8_G8_B8;
        iDepth=32;
        break;
    case NexusDecoder_PixselFormat_R5_G6_B5:
        createSettings.pixelFormat=NEXUS_PixelFormat_eR5_G6_B5;
        iDepth=16;
        break;
    case NexusDecoder_PixselFormat_B5_G6_R5:
        createSettings.pixelFormat=NEXUS_PixelFormat_eB5_G6_R5;
        iDepth=16;
        break;
    default:
        return -1;
    };
    iActualPictureSize=iWidth*iHeight*iDepth/8;
    
    if(iActualPictureSize>iBufferSize)
    {
        return -1;
    }
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    
    createSettings.width = iWidth;
    createSettings.height = iHeight;
    
    destSurfaceHandle = NEXUS_Surface_Create(&createSettings);    
       
    
    
    NEXUS_Graphics2D_GetDefaultBlitSettings(&blitSettings);
    blitSettings.source.surface = m_surfaceHandle;
    blitSettings.source.rect.x = iX;
    blitSettings.source.rect.y = iY;
    blitSettings.source.rect.width = srcSurfaceSettings.width;
    blitSettings.source.rect.height= srcSurfaceSettings.height;
    
    if(NEXUS_PIXEL_FORMAT_IS_YCRCB(srcSurfaceSettings.pixelFormat))
    {
        printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
        blitSettings.source.rect.width += blitSettings.source.rect.width%2; /* YCrCb single pixel has width of 2 */
    }    
    blitSettings.colorOp = NEXUS_BlitColorOp_eCopySource;
    blitSettings.alphaOp = NEXUS_BlitAlphaOp_eCopySource;
    
    blitSettings.output.surface = destSurfaceHandle;     
    blitSettings.output.rect.x = 0;
    blitSettings.output.rect.y = 0; 
    blitSettings.output.rect.width = createSettings.width;
    blitSettings.output.rect.height =createSettings.height;            
   
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    NEXUS_Graphics2D_Blit(m_gfxHandle, &blitSettings);
    
    rc = NEXUS_Graphics2D_Checkpoint(m_gfxHandle, NULL); /* require to execute queue */
    if (rc == NEXUS_GRAPHICS2D_QUEUED)
    {
        rc = BKNI_WaitForEvent(m_checkpointEvent, BKNI_INFINITE);
    }
    //printf("%s->%s(%d) rc:%d \n",__FILE__,__FUNCTION__,__LINE__,rc);
    NEXUS_Surface_GetMemory(destSurfaceHandle, &mem);
    iCopySize=iActualPictureSize;
    if(iCopySize>iBufferSize)
    {
        iCopySize=iBufferSize;
    }
    //printf("%s->%s(%d) iActualPictureSize:%d  iBufferSize:%d\n",__FILE__,__FUNCTION__,__LINE__,iActualPictureSize,iBufferSize);
    if(mem.buffer!=NULL)
    {
        memcpy(pDataBuffer,mem.buffer,iCopySize);
    }
    //printf("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
    NEXUS_Surface_Destroy(destSurfaceHandle);
    
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static void *createNexusDecoderHandle()
{
    NexusDecoderImpl *pNexusDecoderImpl=new NexusDecoderImpl();
    return pNexusDecoderImpl;
}

static void destroyNexusDecoderHandle(void *hHandle)
{
    NexusDecoder *pNexusDecoder=reinterpret_cast<NexusDecoder*>(hHandle);
    delete pNexusDecoder;
}
static int nexusDecoderFromFile(void *hHandle,char *strFile)
{
    NexusDecoder *pNexusDecoder=reinterpret_cast<NexusDecoder*>(hHandle);
    return pNexusDecoder->decoder(strFile);    
}

static int nexusDecoderFromBuffer(void *hHandle,unsigned char *pDataBuffer,int iDataBufferSize,int iPictureType)
{
    NexusDecoder *pNexusDecoder=reinterpret_cast<NexusDecoder*>(hHandle);
    return pNexusDecoder->decoder(pDataBuffer,iDataBufferSize,iPictureType);
}

static int nexusGetPictureInfo(void *hHandle,int *iRetWidth,int *iRetHeight,int *iRetLineSize,int *iRetPixelFormat)
{
    NexusDecoder *pNexusDecoder=reinterpret_cast<NexusDecoder*>(hHandle);
    return pNexusDecoder->getPictureInfo(*iRetWidth,*iRetHeight,*iRetLineSize,*iRetPixelFormat);
}

static int nexusGetPictureData(void *hHandle,char *pDataBuffer,int iBufferSize,int iX,int iY,int iWidth,int iHeight,int iPixelFormat)
{
    NexusDecoder *pNexusDecoder=reinterpret_cast<NexusDecoder*>(hHandle);
    return pNexusDecoder->getPictureData(pDataBuffer,iBufferSize,iX,iY,iWidth,iHeight,(NexusDecoder_PixelFormat)iPixelFormat);
}

static unsigned int getNexusDecoderVersion()
{
    return NEXES_DECODER_VERSION(1,0);
}

static bool supportPictureFormat(int iPictureFormat)
{
    switch(iPictureFormat)
    {
    case NexusDecoder_PictureJpeg:
    case NexusDecoder_PictureGif:
    case NexusDecoder_PicturePng:
        return true;
    default:
        break;
    }    
    return false;
}

    
NexusDecoderHandle g_sNexusDecoderHandle=
{
    m_iMagic    :sizeof(NexusDecoderHandle),
    getNexusDecoderVersion      :&getNexusDecoderVersion,
    supportPictureFormat        :&supportPictureFormat,
    createNexusDecoderHandle    :&createNexusDecoderHandle,
    destroyNexusDecoderHandle   :&destroyNexusDecoderHandle,
    nexusDecoderFromBuffer      :&nexusDecoderFromBuffer,
    nexusGetPictureInfo         :&nexusGetPictureInfo,
    nexusGetPictureData         :&nexusGetPictureData,
};

#ifdef __cplusplus
}
#endif

