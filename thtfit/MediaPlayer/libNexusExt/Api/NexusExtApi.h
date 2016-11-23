#if	!defined(_NexusExtApi_h_)
#define	_NexusExtApi_h_

#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <WeakPtr.h>
#include "MediaPlayerAppDef.h"
#include <String.h>
#include <SharedPtr.h>

using namespace CppBase;
using namespace MediaPlayer;

class CNexusExt
{
public:
	class CEventListener
	{
	public:
		virtual ~CEventListener() {};
		virtual void OnEndOfStream() {};
		virtual void onVidDecStreamChanged() {};
		virtual void NotifyScanProgress(UINT32_t PgmNum,UINT32_t Progress){};
		virtual void NotifyTuPlayerPropmtMsg(UINT32_t MsgTag){};
		virtual void Tuner_StartScan(){};
		virtual void Tuner_StopScan(){};
	};
public:
	virtual INT_t setEventListener(WeakPtr <CEventListener> evtListener_wp) = 0;
	virtual INT_t NexusExt_NxClient_Join(LPCSTR pszClientName = NULL) = 0;
	virtual INT_t NexusExt_NxClient_Uninit() = 0;	
	virtual INT_t Play(CONST LPCSTR pszUrl, CONST BOOL_t bLoopMode = FALSE) = 0;
	virtual INT_t SwitchToPlay() = 0;
	virtual INT_t SwitchToPause() = 0;
	virtual INT_t SwitchToFastForward() = 0;
	virtual INT_t setShowClosedCaption(int iCcValue)= 0;
	virtual INT_t mibSwitchToFastForward(int iPlaySpeed) = 0;
	virtual INT_t SwitchToRewind() = 0;
	virtual INT_t getPlayMode(OUT MediaPlayer::PlayMode & ePlayMode) = 0;
	virtual INT_t StopPlayback() = 0;
	virtual INT_t ChangeVolume(int VolumePercentToChange) = 0;
	virtual INT_t setVideoPosition(int x, int y, int w, int h) = 0;
    virtual INT_t setOsdSize(int w, int h)=0;
	virtual INT_t ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode,
		VIDEO_OUTPUT_MODE * peNewVoMode=NULL , VIDEO_OUTPUT_MODE * peOldVoMode=NULL) = 0;
	virtual INT_t scanI2cDev() = 0;
	virtual INT_t setGpioOutputVal(const int iGpioNum, const bool bVal) = 0;
	virtual INT_t setStereoscopic3DFormat(int* pStereoscopic3D = NULL) = 0;
	virtual INT_t setVidWinAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio) = 0;
	virtual INT_t changeHdmiDisplaySettings(int iDispParam,int iValue) = 0;
	virtual	INT_t getMonitorInfo(OUT CString & strMonitorType) = 0;
	virtual INT_t MibgetMonitorInfo(OUT int & MonitorType) = 0;
	virtual INT_t MibsetMonitorInfo(int MonitorType) = 0;
	virtual INT_t getDisplayParam(const int iDispParam,OUT int &oValue)=0;
	virtual INT_t setDisplayParam(int iDispParam,int iValue)=0;
	virtual INT_t setOutputSpdifMode(int iSpdifMode)=0;
	virtual INT_t setHdmiAudioOutput(int bHdmiAudioOutput)=0;
	virtual INT_t setAnalogAudioMute(int bAnalogAudioMute)=0;
	virtual INT_t setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec, int izone)=0;
	virtual INT_t getHwRtcTime(OUT CString & ConfigDateTime)=0;
	virtual INT_t setHdmiCecCtrl(int HdmiCecCmd)=0;
	virtual INT_t getDisplayPosition(int  eRuaDispType,int &x,int &y,int &width,int &height)=0;
	virtual INT_t getVideoPosition(int  eRuaDispType,int &x,int &y,int &width,int &height)=0;
	virtual INT_t setOsdPosition(int x,int y,int width,int height,int type)=0;
	virtual INT_t getCurrentTime(OUT ULONG & uTimestamp)=0;
	virtual INT_t getLastTime(OUT ULONG & uTimestamp)=0;
	virtual INT_t setSyncTime(const LONG & Offset)=0;
	virtual INT_t setSyncSeek(const LONG & Offset)=0;
	virtual INT_t Seek(const unsigned int& Offset)=0;
	virtual INT_t setDisplayOutputMode(int displayMode)=0;
	virtual INT_t getDisplayOutputMode(int & displayMode)=0;
	virtual INT_t setvideoColorSpace(int colorspace)=0;
	virtual INT_t getvideoColorSpace(int & colorspace)=0;

#ifdef ENABLE_DTV
	virtual INT_t PlayTuner(CONST LPCSTR pszUrl)=0;
	virtual INT_t TunerScan()=0;
	virtual INT_t GetTunerChannelMode(int* iTuChMode)=0;
	virtual INT_t SetTunerChannelMode(int iTuChMode)=0;
	virtual INT_t SetTunerStandardMode(int iTuStMode)=0;
	virtual INT_t GetFirstChNum(unsigned int* OMajor,unsigned int* OMinor)=0;
	virtual INT_t GetSignalStrength(unsigned int* Strength) = 0;
	virtual INT_t ResetDuration() = 0;
	virtual INT_t SetChannelUp() =0;
	virtual INT_t SetChannelDown() = 0;
	virtual INT_t GetTunerStandardMode(int* iStdMode)=0;
	virtual INT_t GetEntryChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids)=0;
#endif
};

CNexusExt * CNexusExtImpl_new();

#endif	//_NexusExtApi_h_

