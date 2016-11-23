#if	!defined(_NexusExt_h_)
#define	_NexusExt_h_

#include <NexusExtApi.h>
#include "TunerSrv.h"


class CNexusExtImpl : public CNexusExt
{
public:
	CNexusExtImpl();
	virtual ~CNexusExtImpl();
	virtual INT_t setEventListener(WeakPtr <CEventListener> evtListener_wp);
	virtual INT_t NexusExt_NxClient_Join(LPCSTR pszClientName = NULL);
	virtual INT_t NexusExt_NxClient_Uninit();	
	virtual INT_t Play(CONST LPCSTR pszUrl, CONST BOOL_t bLoopMode = FALSE);
	virtual INT_t SwitchToPlay();
	virtual INT_t SwitchToPause();
	virtual INT_t SwitchToFastForward();
	virtual INT_t setShowClosedCaption(int iCcValue);
	virtual INT_t mibSwitchToFastForward(int iPlaySpeed);
	virtual INT_t SwitchToRewind();
	virtual INT_t getPlayMode(OUT MediaPlayer::PlayMode & ePlayMode);
	virtual INT_t StopPlayback();
	virtual INT_t ChangeVolume(int VolumePercentToChange);
	virtual INT_t setVideoPosition(int x, int y, int w, int h);
	virtual INT_t ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,
		VIDEO_OUTPUT_MODE * peOldVoMode =NULL);
	static VOID pfnOnEndOfStreamCb(void * pContext);
	VOID OnEndOfStream();
	static VOID vidDecStreamChangedCb(void * pCtx);
	VOID onVidDecStreamChanged();
	virtual INT_t scanI2cDev();
	virtual INT_t setGpioOutputVal(const int iGpioNum, const bool bVal);
	virtual INT_t setStereoscopic3DFormat(int * pStereoscopic3D = NULL);
	virtual INT_t setVidWinAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio);
	virtual INT_t getMonitorInfo(OUT CString &strMonitorType);
	virtual INT_t MibgetMonitorInfo(OUT int & MonitorType);
	virtual INT_t MibsetMonitorInfo(int MonitorType);
	virtual INT_t getDisplayParam(const int iDispParam,OUT int &oValue);
	virtual INT_t setDisplayParam(int iDispParam,int iValue);
	virtual INT_t setOutputSpdifMode(int iSpdifMode);
	virtual INT_t setHdmiAudioOutput(int bHdmiAudioOutput);
	virtual INT_t setAnalogAudioMute(int bAnalogAudioMute);
	virtual INT_t setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec, int izone);
	virtual INT_t getHwRtcTime(OUT CString  & ConfigDateTime);
	virtual INT_t setHdmiCecCtrl(int HdmiCecCmd);
	virtual INT_t getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
	virtual INT_t getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
    virtual INT_t setOsdSize(int w, int h);
	virtual INT_t setOsdPosition(int x,int y,int width,int height,int type);
	virtual INT_t getCurrentTime(OUT ULONG & uTimestamp);
	virtual INT_t getLastTime(OUT ULONG & uTimestamp);
	virtual INT_t setSyncTime(const LONG & Offset);
	virtual INT_t setSyncSeek(const LONG & Offset);
	virtual INT_t Seek(const unsigned int& Offset);
	virtual INT_t setDisplayOutputMode(int displayMode);
	virtual INT_t getDisplayOutputMode(int & displayMode);
	virtual INT_t setvideoColorSpace(int colorspace);
	virtual INT_t getvideoColorSpace(int & colorspace);
	virtual INT_t changeHdmiDisplaySettings(int iDispParam,int iValue);

#ifdef ENABLE_DTV
	virtual INT_t GetEntryChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids);
	virtual INT_t PlayTuner(CONST LPCSTR pszUrl);
	virtual INT_t TunerScan();
	virtual INT_t GetTunerChannelMode(int* iTuChMode);
	virtual INT_t SetTunerChannelMode(int iTuChMode);
	virtual INT_t SetTunerStandardMode(int iTuStMode);
	virtual INT_t GetSignalStrength(unsigned int* Strength);
	virtual INT_t GetFirstChNum(unsigned int* OMajor,unsigned int* OMinor);
	virtual INT_t ResetDuration();
	virtual INT_t SetChannelUp();
	virtual INT_t SetChannelDown();
	virtual INT_t GetTunerStandardMode(int* iStdMode);
	static void CallBackScanProgress(void* pContext,UINT16_t *pRrogramNum,UINT16_t *pScanRrogress);
	void FN_cbScanProgress(UINT16_t *pRrogramNum,UINT16_t *pScanRrogress);
	INT_t PsfPlayback();
#endif
	
private:
	INT_t CreateMediaPlayer();

#ifdef ENABLE_DTV
	INT_t OpenTunerPlayer();
#endif
protected:
    bool m_bFirstOutputModeSet;
    unsigned int m_iYPrPbValue;
	
private:
	media_player_t m_pMediaPlayer;
	WeakPtr <CEventListener> m_EvtListener_wp;
	NEXUS_DisplayAspectRatio m_eNexusDispAspectRatio;
	bool m_bFullVideo;
	int  ccSertype;
	int iMibmonitortype;
	int iCcflag;
#ifdef ENABLE_DTV
	SharedPtr<CTunerSrv> m_TunerSrv_sp;
	CErrChkMutex m_TunerScanMutex;
	bool m_bTunerOpenSucc;
#endif

};

#endif	//_NexusExt_h_
