#if	!defined(_MediaSrv_h_)
#define	_MediaSrv_h_

#include <QThread>
#include <BaseTypeDef.h>
#include <ThreadMessageQueue.h>
#include <glib.h>
#include <gmain.h>
#include <ErrChkMutex.h>
#include <gst/gst.h>
#include "NexusPlayer.h"
#include <dbus/dbus.h>
#include "CnkB85Lcm.h"
#include "MediaPlayerApi.h"

class QMediaSrv : public QThread, public CThreadMessageQueue, public CThreadMessageQueue::IEventListener
{
	Q_OBJECT
public:
	struct tagPLAY_PARAMS
	{
		INT_t LoopCount;
#if 1/*added by lxj 2013-1-8*/
		bool bPauseAfterBuffering;
#endif
	};
	typedef tagPLAY_PARAMS PLAY_PARAMS;
	typedef PLAY_PARAMS * P_PLAY_PARAMS;
public:
	static void PlayParams_Init(QMediaSrv::P_PLAY_PARAMS pPlayParams);
public:
	WeakPtr <QMediaSrv> m_this_wp;
    VIDEO_OUTPUT_MODE m_eVoMode;
	
public:
	QMediaSrv();
	virtual ~QMediaSrv();
	INT_t InitInstance();
	INT_t ExitInstance();
	virtual void run();
	virtual void quit();
	INT_t setDataSource(const QString & strUrl);
	INT_t Prepare();
	INT_t Play(quint32 * pPlaybackId, IN QMediaSrv::P_PLAY_PARAMS pPlayParams);
	INT_t setPlayMode(const int iPlayMode);
	INT_t getPlayMode(OUT INT_t & iPlayMode);
	INT_t getShowClosedCaption(OUT qint32 & iCcValue);
	INT_t setShowClosedCaption(qint32 iCcValue);
	INT_t setPlaySpeed(qint32 iPlaySpeed);
	INT_t getPlaySpeed(OUT qint32 & iPlaySpeed);
	INT_t Stop(quint32 * pPlaybackId = NULL, QString * pStrUrl = NULL);
	INT_t ChangeVolume(int VolumePercentToChange);
	INT_t setVideoPosition(int x, int y, int w, int h);
    INT_t setOsdSize(int w, int h);
    INT_t ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,VIDEO_OUTPUT_MODE * peOldVoMode = NULL);
    INT_t ChangeVidOutModeAsync(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,VIDEO_OUTPUT_MODE * peOldVoMode = NULL);
	INT_t DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos);
	INT_t scanI2cDev();
	INT_t setGpioOutputVal(const int iGpioNum, const bool bVal);
	INT_t setHdmi3DTvMode(CONST HDMI_3D_TV_MODE eHdmi3DTvMode);	
	INT_t getMonitorInfo(OUT QString & strMonitorInfo);	
	INT_t MibgetMonitorInfo(OUT qint32 & MonitorType);
	INT_t MibsetMonitorInfo(qint32 MonitorType);
	INT_t setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags);	
	INT_t getVideoDispAspectRatio(OUT qint32 & iVidDispAspectRatio);
	virtual void onNewMessageInQueue();	//called by CThreadMessageQueue
	void OnNewMessage();
	INT_t setStereoscopic3DFormat(int* pStereoscopic3D = NULL);
	INT_t getDisplayParam(const int iDispParam,int &oValue);
	INT_t setDisplayParam(int iDispParam,int iValue);
	INT_t getOutputSpdifMode(int &oSpdifMode);
	INT_t setOutputSpdifMode(int iSpdifMode);
	INT_t setHdmiAudioOutput(int bHdmiAudioOutput);
	INT_t setAnalogAudioMute(int bAnalogAudioMute);
	INT_t setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec,int izone);
	INT_t getHwRtcTime(QString & ConfigDateTime);
	INT_t setHdmiCecCtrl(CONST HDMI_CEC_CTRL_CMD eHdmiCecCmd);
	INT_t getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
	INT_t getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
	INT_t setOsdPosition(int x,int y,int width,int height,int type);
	INT_t getCurrentTime(OUT ULONG & uTimestamp);
	INT_t getLastTime(OUT ULONG & uTimestamp);
	INT_t setSyncTime(const LONG & Offset);
	INT_t setSyncSeek(const LONG & Offset);
	INT_t Seek(const unsigned int & Offset);

#if 1 /*Add by xuweiwei 2015-8-28*/
	INT_t getDisplayOutputMode(qint32 & displayoutputMode);
	INT_t setDisplayOutputMode(qint32 displayoutputMode);
	INT_t getvideoColorSpace(qint32 & colorspace);
	INT_t setvideoColorSpace(qint32 colorspace);
#endif
	

#ifdef ENABLE_DTV	
	INT_t PlayTuner(quint32 * pPlaybackId);
	INT_t TunerScan();
	INT_t getTunerChannelMode(quint32 * TunerChannleMode);
	INT_t setTunerChannelMode(qint32 TunerChannleMode);
	INT_t TunerMode(qint32 iStdMode);
	INT_t GetFirstChNumber(quint32* OMajor,quint32* OMinor);
	INT_t GetTunerChannelStrength(quint32* Strength);
	INT_t ResetDuration();
	INT_t ChannelUp();
	INT_t ChannelDown();
	INT_t GetTunerMode(qint32* iStdMode);
	INT_t GetEntryChannelInfo(IN INT32_t EntryId ,OUT INT32_t *pChannelNumber,OUT INT32_t *pSubChannelCount,OUT CStackBufString &StrProgramPids);
#endif
	
public slots:
	void onNewMsgInThreadMsgQueue();
	void onPlaybackEos();
	void onVidDecStreamChanged();

#ifdef ENABLE_DTV	
	void onNotifyTuPlayerPropmtMsg(int);
	void onMPNotifyScanProgress(int,int);
	void onTuner_StartScan();
	void onTuner_StopScan();
#endif
	
signals:
	void sigPlaybackEos(const quint32 playbackId, const QString & strPlaybackUrl, const bool bIsCompletePlaybackEos);
	void sigNewMsgInThreadMsgQueue();
	void sigVidOutModeChangedEvent(int eCurMode);

#ifdef ENABLE_DTV	
	void sigNotifyTunerPlayerPropmtMsg(int MagTag);
	void sigNotifyScanProgress(int PgmNum,int Progress);
	void sigTuner_StartScan();
	void sigTuner_StopScan();
#endif
	
private:
	VOID ProcessMsg(SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_setDataSource(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_Prepare(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_Play(SharedPtr<CMessageQueueItem> & TransactionMsg, SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setPlayMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setShowClosedCaption(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setPlaySpeedCtrl(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getPlayMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_Stop(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setVideoPosition(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_PlaybackEos(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_VidDecStreamChanged(SharedPtr<CGenericMsgData> & GenMsgData_sp);
    INT_t On_ChangeVideoOutputMode(SharedPtr<CMessageQueueItem> & TransactionMsg, SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_DispOnLedScr(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t DisplayOnLedScreen(LPCSTR strContentToDisp, CONST INT_t StartPosToDisp);
	INT_t EmitVidOutModeChangedEvent(VIDEO_OUTPUT_MODE eCurMode);
	INT_t sendSimpleMessage(CONST UINT_t msgId, CONST UINT_t uParam = 0, OUT UINT_t * puParam = NULL);
	INT_t sendComplexMessage(CONST UINT_t msgId, CONST UINT_t uParam = 0, 
		IN OUT WeakPtr<CObject> ObjParam_wp = WeakPtr<CObject>(), OUT UINT_t * puParam = NULL);
	INT_t On_scanI2cDev(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setGpioOutputVal(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setHdmi3DTvMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getMonitorInfo(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setVidDispAspectRatio(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getVidDispAspectRatio(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getOutputSpdifMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setOutputSpdifMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t applyVideoDispAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio, IN BOOL_t bForce = FALSE);
	INT_t setMruaOutputSpdifMode(INT_t iSnmpOutSpdifMode, BOOL_t bApply = TRUE);
	INT_t changeHdmiDisplaySettings(int iDispParam,int iValue);

#ifdef ENABLE_DTV
	INT_t On_PlayTuner(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_TunerScan(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_GetTunerChannelMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_SetTunerChannelMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_SetTunerStandardMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_NotifyTuPlayerPropmtMsg(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_NotifyScanProgress(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_Tuner_StartScan(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_Tuner_StopScan(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_GetFirstChNum(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_GetSignalStrength(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_ResetDuration(SharedPtr<CGenericMsgData> & GenMsgData_sp, SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_SetChannelUp(SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_SetChannelDown(SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t On_GetTunerStandardMode(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
	INT_t StopTunerPlayer();
	INT_t On_GetEntryChannelInfo(SharedPtr<CGenericMsgData> & GenMsgData_sp,SharedPtr<CMessageQueueItem> & TransactionMsg);
#endif
	INT_t On_setDisplayOutputMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getDisplayOutputMode(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_setvideoColorSpace(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	INT_t On_getvideoColorSpace(SharedPtr<CGenericMsgData> & GenMsgData_sp);
	

private:
	pthread_t m_threadId;
	QString m_sDataSrcUrl;
	Qt::HANDLE m_hThisThreadId;
	BOOL_t m_bShouldExitThread;
	int m_iPlayStatus;
	CErrChkMutex m_PlayStatusMutex;
    CErrChkMutex m_SharedDataMutex;
	CEventEx m_PlayStatChangedEvent;
	UINT32_t m_PlaybackId;
	CErrChkMutex m_PlaybackIdMutex;
	BOOL_t m_bIsCompletePlaybackEos;	//without an user break
	SharedPtr <QNexusPlayer> m_NexusPlayer_sp;
	INT_t m_iRemaingLoopCnt;
	SharedPtr <CCnkB85Lcm> m_CnkB85LcmLedScr_sp;	
	HDMI_3D_TV_MODE m_eHdmi3DTvMode;
	int m_CurRightStereoscopic3D;
	DISPLAY_ASPECT_RATIO m_VidDispAspectRatioAtNextPlay;
	DISPLAY_ASPECT_RATIO m_CurVidDispAspectRatio;
	qint32 mibPlaySpeed;
	qint32 mibCcValue;
	BOOL_t m_bVidDispAspectRatioAtNextPlay_Valid;
};

#endif	//_QMediaSrv_h_

