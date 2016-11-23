#ifndef	_TRANSACTION_MSG_DATA_H_
#define	_TRANSACTION_MSG_DATA_H_

#include <GenericMsgData.h>
#include <String.h>
#include "MediaPlayerAppDef.h"
#include <ErrChkMutex.h>
#include <QString>

using namespace MediaPlayer;

class CMsgData_setDataSource : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setDataSource);
public:
	QString m_strDataSrcUrl;
};

class CMsgData_Play : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_Play);
public:
	CMsgData_Play();
public:
	//IN
	INT_t LoopCount;
#if 1/*added by lxj 2013-1-8*/
	bool bPauseAfterBuffering;
#endif
	//OUT
	UINT32_t m_PlaybackId;
};

class CMsgData_PlaybackStart : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_PlaybackStart);
public:
};

class CMsgData_PlaybackEos : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_PlaybackEos);
public:
	CString m_strCurPlayUrl;
	UINT32_t m_PlaybackId;
	BOOL_t m_bIsCompletePlaybackEos;
};

class CMsgData_setVideoPosition : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setVideoPosition);
public:
	int x, y, Width, Height;
};

class CMsgData_setVideoInputWindowSize : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setVideoInputWindowSize);
public:
	INT32_t inWinX, inWinY, inWinW, inWinH;
};

class CMsgData_ChangeVidOutMode : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_ChangeVidOutMode);
public:
    VIDEO_OUTPUT_MODE m_eVoModeToSet;
    VIDEO_OUTPUT_MODE m_eOldVoMode, m_eNewVoMode;
};

class CMsgData_setSpdifMode : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setSpdifMode);
public:
	INT32_t m_iSpdifMode;//IN
};

class CMsgData_setPlaySpeedCtrl : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setPlaySpeedCtrl);
public:
	int m_iSpeed;
};

class CMsgData_getPlaySpeedCtrl : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_getPlaySpeedCtrl);
public:
	int m_iSpeed;
};

class CMsgData_DispOnLedScr : public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_DispOnLedScr);
public:
	CString m_strDispContent;
	INT_t m_StartPosToDisp;
};

#ifdef ENABLE_DTV
class CMsgData_GetTunerMode:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_GetTunerMode);
public:
	INT32_t m_channelStdMode;
};

class CMsgData_PropmtMsg:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_ScanProgress);
public:
	UINT32_t m_ProMsgNum;
};

class CMsgData_GetStrength:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_GetStrength);
public:
	UINT32_t m_channelStrength;
};

class CMsgData_GetFirstChNum:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_GetFirstChNum);
public:
	UINT32_t m_major;
	UINT32_t m_minor;
};

class CMsgData_SetTuMode:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_SetTuMode);
public:
	INT32_t m_iStdMode;
};

class CMsgData_ScanProgress:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_ScanProgress);
public:
	UINT16_t m_ProgramNum;
	UINT16_t m_ScanProgress;
};

class CMsgData_GetEntryInfo:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_GetEntryInfo);
public:
	//IN 
	INT32_t m_EntryId;
	//OUT
	INT32_t m_ChannelNum;
	INT32_t m_SubChannlNum;
	char  m_StrProgramPids[64];
};

class CMsgData_SetTuChMode:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_SetTuChMode);
public:
	INT32_t m_iChannelMode;
};

class CMsgData_GetTuChMode:public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_GetTuChMode);
public:
	INT32_t m_OChannelMode;
};

class CMsgData_QuitPlay : public CGenericMsgData
{
public:
	CMsgData_QuitPlay();
public:
	CErrChkMutex mLock;
	//IN
	//OUT
	CString m_strUrl;
	UINT32_t m_PlaybackId;
};

class CMsgData_setVideoRotation: public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setVideoRotation);
public:
	INT32_t m_iVideoRotationMode;//IN
};

class CMsgData_setEnableAudio6Ch: public CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CMsgData_setEnableAudio6Ch);
public:
	BOOL_t m_iEnAudio6ChFlag;//IN
};

#endif

class CMsgData_StopPlayback : public CGenericMsgData
{
public:
	CMsgData_StopPlayback();
public:
	CErrChkMutex mLock;
	//IN
	//OUT
	CString m_strUrl;
	UINT32_t m_PlaybackId;
};

class CMonitorInfoParam : public CObject
{
public:
	//OUT
	CString m_sMonitorInfo;
};

class CVidDispAspectRatioParam : public CObject
{
public:
	//IN
	INT32_t m_iVidDispAspectRatio;
	UINT32_t m_uOptFlags;
};

#endif	//_TRANSACTION_MSG_DATA_H_

