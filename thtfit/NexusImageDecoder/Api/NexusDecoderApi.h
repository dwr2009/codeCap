
#include "picdecoder.h"

#ifndef __NEXUSDECODERAPI_H__
#define __NEXUSDECODERAPI_H__

#define NEXES_DECODER_VERSION(major,minor)    (((major&0xffff)<<16)|(minor&0xffff))

enum NexusDecoder_PixelFormat
{
     NexusDecoder_PixselFormat_Unknow=0,
     NexusDecoder_PixselFormat_A8_R8_G8_B8,
     NexusDecoder_PixselFormat_R5_G6_B5,
     NexusDecoder_PixselFormat_B5_G6_R5
};

enum NexusDecoder_PictureType
{
    NexusDecoder_PictureUnknow=0,
    NexusDecoder_PictureMpeg,
    NexusDecoder_PictureJpeg,
    NexusDecoder_PictureGif,
    NexusDecoder_PicturePng
};

class NexusDecoder
{

public:
    NexusDecoder(){};
    virtual ~NexusDecoder(){};
    
    virtual int decoder(char *strFile)=0;
    virtual int decoder(unsigned char *pDataBuffer,int iDataBufferSize,int iPictureType)=0;
    virtual int getPictureInfo(int &iWidth,int &iHeight,int &iLineSize,int &iPixelFormat)=0;
    virtual int getPictureData(char *pDataBuffer,int iBufferSize,int iX,int iY,int iWidth,int iHeight,NexusDecoder_PixelFormat iPixelFormat=NexusDecoder_PixselFormat_A8_R8_G8_B8)=0;
public:
    
};


#ifdef __cplusplus
extern "C"{
#endif
/*
 * Don't change the struct ,there is used by 
 * qtwebkit/Source/WebCore/platform/image-decoders/NexusImageDecoder.h 
 */
struct NexusDecoderHandle
{
    unsigned int m_iMagic;
    unsigned int (*getNexusDecoderVersion)();
    bool (*supportPictureFormat)(int iPictureFormat);
    void *(*createNexusDecoderHandle)();
    void (*destroyNexusDecoderHandle)(void *hHandle);
    int  (*nexusDecoderFromBuffer)(void *hHandle,unsigned char *pDataBuffer,int iDataBufferSize,int iPictureType);
    int (*nexusGetPictureInfo)(void *hHandle,int *iRetWidth,int *iRetHeight,int *iRetLineSize,int *iRetPixelFormat);
    int (*nexusGetPictureData)(void *hHandle,char *pDataBuffer,int iBufferSize,int iX,int iY,int iWidth,int iHeight,int iPixelFormat);
};

extern NexusDecoderHandle g_sNexusDecoderHandle;

#ifdef __cplusplus
}
#endif






#endif


