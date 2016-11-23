#if	!defined(_NexusPlayer_h_)
#define	_NexusPlayer_h_

#include <QObject>
#include <BaseTypeDef.h>
#include <QString>
#include <NexusExtApi.h>
#include <SharedPtr.h>

using namespace CppBase;

class QNexusPlayer : public QObject, public CNexusExt::CEventListener
{
	Q_OBJECT
public:
	QNexusPlayer();
	virtual ~QNexusPlayer();
	INT_t setThisWp(WeakPtr <QNexusPlayer> this_wp);
	INT_t Play(const QString & strUrl, CONST BOOL_t bLoopMode = FALSE);
	INT_t SwitchToPlay();
	INT_t SwitchToPause();
	INT_t SwitchToFastForward();
	INT_t setShowClosedCaption(qint32 iCcValue);
	INT_t mibSwitchToFastForward(qint32 iPlaySpeed);
	INT_t SwitchToRewind();
	INT_t getPlayMode(OUT MediaPlayer::PlayMode & ePlayMode);
	INT_t Stop();
	INT_t ChangeVolume(int VolumePercentToChange);
	INT_t setVideoPosition(int x, int y, int w, int h);
    INT_t ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,
		VIDEO_OUTPUT_MODE * peOldVoMode = NULL);
	virtual void OnEndOfStream();
	virtual void onVidDecStreamChanged();
	INT_t scanI2cDev();
	INT_t setGpioOutputVal(const int iGpioNum, const bool bVal);
	INT_t setStereoscopic3DFormat(int* pStereoscopic3D = NULL);
	INT_t setVidWinAspectRatio(IN DISPLAY_ASPECT_RATIO eDispAspectRatio);
	INT_t changeHdmiDisplaySettings(int iDispParam,int iValue);
	INT_t getMonitorInfo(OUT CString & strMonitorType);
	INT_t MibgetMonitorInfo(OUT qint32 & MonitorType);
	INT_t MibsetMonitorInfo(qint32 MonitorType);
	INT_t getDisplayParam(const int iDispParam,OUT int &oValue);
	INT_t setDisplayParam(int iDispParam,int iValue);
	INT_t setOutputSpdifMode(int iSpdifMode);
	INT_t setHdmiAudioOutput(int bHdmiAudioOutput);
	INT_t setAnalogAudioMute(int bAnalogAudioMute);
	INT_t setHwRtcTime(unsigned int iyear,unsigned int imonth,unsigned int imday,unsigned int ihour,unsigned int imin,unsigned int isec,int izone);
	INT_t getHwRtcTime(OUT CString & ConfigDateTime);
	INT_t setHdmiCecCtrl(int HdmiCecCmd);
	INT_t getDisplayPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
	INT_t getVideoPosition(int eRuaDispType,int &x,int &y,int &width,int &height);
	INT_t setOsdPosition(int x,int y,int width,int height,int type);
    INT_t setOsdSize(int width,int height);
	INT_t getCurrentTime(OUT ULONG & uTimestamp);
	INT_t getLastTime(OUT ULONG & uTimestamp);
	INT_t setSyncTime(const LONG & Offset);	
	INT_t setSyncSeek(const LONG & Offset);
	INT_t Seek(const unsigned int & Offset);

#ifdef ENABLE_DTV
	INT_t PlayTuner(const QString & strUrl);
	INT_t TunerScan();
	INT_t GetTunerChannelMode(int* iTuChMode);
	INT_t SetTunerChannelMode(int iTuChMode);
	INT_t SetTunerStandardMode(int iTuStMode);
	void  NotifyTuPlayerPropmtMsg(int MsgTag);
	void  NotifyScanProgress(UINT32_t PgmNum,UINT32_t Progress);
	void  Tuner_StartScan();
	void  Tuner_StopScan();
	INT_t GetFirstChNum(unsigned int* OMajor,unsigned int* OMinor);
	INT_t GetSignalStrength(unsigned int* Strength);
	INT_t ResetDuration();
	INT_t SetChannelUp();
	INT_t SetChannelDown();
	INT_t GetTunerStandardMode(int* iStdMode);
	INT_t GetEntryChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids);
#endif
	INT_t setDisplayOutputMode(INT32_t displayMode);
	INT_t getDisplayOutputMode(INT32_t& displayMode);
	INT_t setvideoColorSpace(INT32_t colorspace);
	INT_t getvideoColorSpace(INT32_t& colorspace);


signals:
	void sigOnEndOfStream(void);
	void sigOnVidDecStreamChanged(void);

#ifdef ENABLE_DTV
	void sigNotifyTuPlayerPropmtMsg(int);
	void sigMPNotifyPlayerScanProgress(int,int);
	void sigTuner_StartScan();
	void sigTuner_StopScan();
#endif
	
private:
	void ConnectToNxServer();
	void DisconnectFromNxServer();
	void ReconnectToNxServer();
private:
	BOOL_t m_bNxServerIsConnected;
	SharedPtr <CNexusExt> m_NexusExt_sp;
	WeakPtr <QNexusPlayer> m_this_wp;
};

#endif	//_QNexusPlayer_h_

