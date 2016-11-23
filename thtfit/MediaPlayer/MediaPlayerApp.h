#if	!defined(_MediaPlayerApp_h_)
#define	_MediaPlayerApp_h_

#include <QApplication>
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <SignalInfo.h>
#include <signal.h>
#include <ErrPrintHelper.h>
#include <LinuxSignalHandlerEx.h>
#include <QDBusConnection>
#include <QSharedPointer>
#include <QDBusAbstractAdaptor>
#include <MediaSrv.h>
#include "MediaPlayerAppIf.h"
#include "SerialComPort.h"
#include <TiXmlDoc2.h>
#include <QTimer>
#include <QByteArray>
#include "VideoSync.h"


// ProcessExitCodes
#define	SS_PEC_OUT_OF_MEMORY			1
#define	SS_PEC_INIT_INSTANCE_FAIL		2
#define	SS_PEC_EXIT_INSTANCE_FAIL		3

#define	QDBUS_CONN_NAME_MediaPlayerSrv			"MediaPlayerSrv"
#define	XML_VOLUME_ELEMENT_PATH					"Volume"
#define	AUD_ENG0_MASTER_NAME					"AudEng0Master"

using namespace VideoSync;//abner

class QMediaPlayerApp;

class QDBusIf_MediaPlayer : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.MPlayer")	
public:
	explicit QDBusIf_MediaPlayer(QMediaPlayerApp * pRealObj);
public slots:
	void setDataSource(const QString & strUrl, int & iOutRet);
	void Prepare(int & iOutRet);
	void Play(int & iOutRet, quint32 & playbackId);
	void Seek(const unsigned int &Offset,int & iOutRet);
	void Play2(qint32 iLoopCnt, int & iOutRet, quint32 & playbackId);
	void Stop(int & iOutRet, quint32 & playbackId, QString & strUrl);
	void ChangeVolume(int VolumePercentToChange,int & iOutRet,bool & pbVolChanged);
    void setVideoOutputModeAsync(int mode,int & iOutRet, qint32 & newVoMode);
#if 1 /*added by bert 2016/11/12*/
void setHDRAsync(int mode,int & iOutRet, qint32 & smoothMode);
void getHDRAsync(int mode,int & iOutRet, qint32 & smoothMode);
#endif
    void ChangeVideoOutputMode(int mode,int & iOutRet, qint32 & newVoMode);
	void setVideoPosition(int x, int y, int w, int h, int & iOutRet);
    void setOsdSize(int w, int h, int & iOutRet);
	void DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos, int & iOutRet);
	void scanI2cDev(int & iOutRet);
	void setGpioLedStatus(const quint32 nGpioLedStatus, int & iOutRet, quint32 & nActualLedStatus);
	void setGpioOutputVal(const int iGpioNum, const int iVal, int & iOutRet);
	void setPlayMode(const qint32 iPlayMode, qint32 & iOutRet);
	void getPlayMode(qint32 & iOutRet, qint32 & iPlayMode);
	void setHdmi3DTvMode(const qint32 iHdmi3DTvMode, qint32 & iOutRet);
	void getMonitorInfo(qint32 & iOutRet, QString & strMonitorInfo);
	void setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags, qint32 & iOutRet);
	void getVideoDispAspectRatio(qint32 & iOutRet, qint32 & iVidDispAspectRatio);
	void getShowClosedCaption(qint32 & iOutRet, qint32 & iCcValue);
	void setShowClosedCaption(qint32 iCcValue, qint32 & iOutRet);
	void setPlaySpeed(qint32 iPlaySpeed, qint32 & iOutRet);
	void getPlaySpeed(qint32 & iOutRet, OUT qint32 & iPlaySpeed);
	void MibsetMonitorInfo(qint32 MonitorInfo, qint32 & iOutRet);
	void MibgetMonitorInfo(qint32 & iOutRet, qint32 & MonitorInfo);
	void getSerialControlData(qint32 & iOutRet, QString &strSerialControlData);
	void setSerialControlData(const QString & strSerialControlData,qint32 & iOutRet);
	void getSerialParameters(qint32 & iOutRet,QString & strSerialParameters);
	void setSerialParameters(const QString & strSerialControlData,qint32 & iOutRet);
	void getDisplayParam(const qint32 iDispParam,qint32 & iOutRet,qint32 &oValue);
	void setDisplayParam(qint32 iDispParam,qint32 iValue,qint32 bSave,qint32 & iOutRet);
	void getOutputSpdifMode(qint32 & iOutRet,qint32 &oSpdifMode);
	void setOutputSpdifMode(qint32 iSpdifMode,qint32 & iOutRet);
	void getTimeZoneSetting(qint32 & iOutRet,OUT qint32 & TimezoneHourOff);
	void setTimeZoneSetting(qint32 TimezoneHourOff,qint32 & iOutRet);
	void setHdmiAudioOutput(qint32 bHdmiAudioOutput,qint32 & iOutRet);
	void setAnalogAudioMute(qint32 bAnalogAudioMute,qint32 & iOutRet);
	void setConfigDateTime(qint32 DateTime,qint32 & iOutRet);
	void getConfigDateTime(qint32 & iOutRet,QString & ConfigDateTime);
	void setCurClkTicks(qint32 ClkTicks,qint32 & iOutRet);
	void getCurClkTicks(qint32 & iOutRet,OUT qint32& ClkTicks);
	void setHdmiCecCtrl(qint32 HdmiCecCtrl,qint32 & iOutRet);
	void setHwRtcTime(qint32  Year, qint32  Month, qint32  Date, qint32  Hour, qint32  Minute, qint32  Second, int pszTimeZone,qint32 & iOutRet);
	void getDisplayPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height,qint32 & iOutRet);
	void getVideoPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height,qint32 & iOutRet);
	void setOsdPosition(qint32 x,qint32 y,qint32 width,qint32 height,qint32 type,qint32 & iOutRet);
	void getCurrentVolume(qint32 & iOutRet,qint32 &oValue);
	void OnSaveVolume(qint32 iVolume,qint32 & iOutRet);
	void getVideoSyncMode(qint32 & iOutRet,qint32 &SyncMode);
	void setVideoSyncMode(qint32 SyncMode,qint32 & iOutRet);
	void getVideoSyncMaster(qint32 & iOutRet, QString &strMasterAddr);
	void setVideoSyncMaster(const QString &strMasterAddr,qint32 & iOutRet);
	#if 1 //added by jaylon for getcpunum
	void getCpuSerialNo(qint32 & iOutRet, QString & CpuSerialNo);
	#endif
#if 1 //add by bert 2016/08/27	
	void gethardwareCapability(qint32 & iOutRet, qint32 & hardwareCapability);
#endif	
#if 1 /*Add by xuweiwei 2015-8-28 */
	void getDisplayOutputMode(int & iOutRet, qint32 & displayoutputMode);
	void setDisplayOutputMode(qint32 displayoutputMode,int & iOutRet);
	void getvideoColorSpace(int & iOutRet, qint32 & colorspace);
	void setvideoColorSpace(qint32 colorspace,int & iOutRet);
#endif

#ifdef ENABLE_DTV
	void PlayTuner(qint32 & iOutRet, quint32 & playbackId);
	void TunerScan(qint32 & iOutRet);
	void getTunerChannelMode(quint32 & iOutRet, quint32 & TunerChannleMode);
	void setTunerChannelMode(qint32 TunerChannleMode,qint32 & iOutRet);
	void TunerMode(qint32 iStdMode,qint32 & iOutRet);
	void GetFirstChNumber(qint32 & iOutRet,quint32 & OMajor,quint32 & OMinor);
	void GetTunerChannelStrength(qint32 & iOutRet,quint32& Strength);
	void ResetDuration(qint32 & iOutRet);
	void ChannelUp(qint32 & iOutRet);
	void ChannelDown(qint32 & iOutRet);
	void GetTunerMode(qint32 & iOutRet,qint32& iStdMode);
	void Get_EntryChannelInfo(qint32 EntryId,qint32 & iOutRet,qint32& pChannelNumber,qint32& pSubChannelCount,QString& StrProgramPids);
	void GetForceTxMode(qint32 & iOutRet,qint32& iTxMode);
#endif
    

signals:
	void PlaybackEos(const QString & strPlaybackUrl, const quint32 playbackId);
	void PlaybackEos2(const QString & strPlaybackUrl, const quint32 playbackId, const qint32 bIsCompletePlaybackEos);
	void VidOutModeChanged(const qint32 iCurVoMode);

#ifdef ENABLE_DTV
	void NotifyTuPlayerPropmtMsg(int MagTag);
	void NotifyScanProgress(qint32 PgmNum,qint32 Progress);
	void Tuner_StartScan(qint32 startid);
	void Tuner_StopScan(qint32 stopid);
#endif
	
private:
	QMediaPlayerApp * m_pRealObj;
};

class QMediaPlayerApp : public QApplication, public CGeneralApp
{
	Q_OBJECT
public:
	QMediaPlayerApp(int argc, char * argv[]);
	virtual ~QMediaPlayerApp();
	static VOID SigHandler(VOID * pCbContext, int SigNo);
	INT_t InitInstance();
	INT_t ExitInstance();	
	virtual SharedPtr <CTinyXmlDoc3> getXmlCfgDoc();
	int exec();
	int setDataSource(const QString & strUrl);
	int Prepare();
	int Play(qint32 iLoopCnt, quint32 & playbackId);
	int Stop(quint32 & playbackId, QString & strUrl);
	int ChangeVolume(int VolumePercentToChange);
	INT_t setVideoPosition(int x, int y, int w, int h);
    INT_t setOsdSize(int w, int h);
    int ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,
		VIDEO_OUTPUT_MODE * peOldVoMode = NULL);
    int ChangeVidOutModeAsync(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode = NULL,
		VIDEO_OUTPUT_MODE * peOldVoMode = NULL);
	INT_t DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos);
	INT_t scanI2cDev();
	INT_t setGpioLedStatus(const quint32 nGpioLedStatus, quint32 & nActualLedStatus);
	INT_t setGpioOutputVal(const int iGpioNum, const bool bVal);
	INT_t setPlayMode(const int iPlayMode);
	INT_t getPlayMode(OUT INT_t & iPlayMode);
	INT_t setHdmi3DTvMode(const int iHdmi3DTvMode);
	INT_t getMonitorInfo(OUT QString & strMonitorInfo);
	INT_t setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags);	
	INT_t getVideoDispAspectRatio(OUT qint32 & iVidDispAspectRatio);
	INT_t getShowClosedCaption(OUT qint32 & iCcValue);
	INT_t setShowClosedCaption(qint32 iCcValue);
	INT_t setPlaySpeed(qint32 iPlaySpeed);
	INT_t getPlaySpeed(OUT qint32 & iPlaySpeed);
	INT_t MibgetMonitorInfo(OUT qint32 & MonitorType);
	INT_t MibsetMonitorInfo(qint32 MonitorType);
	INT_t getSerialControlData(QString& strSerialControlData);
	INT_t setSerialControlData(const QString & strSerialControlData);	
	INT_t getSerialParameters(QString & strSerialParameters);
	INT_t setSerialParameters(const QString & strSerialControlData);
	INT_t getDisplayParam(const qint32 iDispParam,OUT qint32 &oValue);
	INT_t setDisplayParam(qint32 iDispParam,qint32 iValue,qint32 bSave);
	INT_t getOutputSpdifMode(qint32 &oSpdifMode);
	INT_t setOutputSpdifMode(qint32 iSpdifMode);
	INT_t getTimeZoneSetting(OUT qint32 & TimezoneHourOff);
	INT_t setTimeZoneSetting(qint32 TimezoneHourOff);
	INT_t setHdmiAudioOutput(qint32 bHdmiAudioOutput);
	INT_t setAnalogAudioMute(qint32 bAnalogAudioMute);
	INT_t setConfigDateTime(qint32 DateTime);
	INT_t getConfigDateTime(QString & ConfigDateTime);
	INT_t setCurClkTicks(qint32 ClkTicks);
	INT_t getCurClkTicks(OUT qint32& ClkTicks);
	INT_t setHdmiCecCtrl(qint32 HdmiCecCtrl);
	INT_t setHwRtcTime(qint32 Year, qint32 Month, qint32 Date, qint32 Hour, qint32 Minute, qint32 Second, int pszTimeZone);
	INT_t getDisplayPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height);
	INT_t getVideoPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height);
	INT_t setOsdPosition(qint32 x,qint32 y,qint32 width,qint32 height,qint32 type);
	INT_t getCurrentVolume(qint32 & CurVolume);


#if 1/*added by lxj 2012-11-22*/
	INT_t getVideoSyncMode(OUT qint32 &SyncMode);
	INT_t setVideoSyncMode(VIDEO_SYNC_MODE_t SyncMode);
	INT_t getVideoSyncMaster(OUT QString &strMasterAddr);
	INT_t setVideoSyncMaster(const QString &strMasterAddr);

	INT_t getCurrentTime(OUT ULONG & uTimestamp);
	INT_t getLastTime(OUT ULONG & uTimestamp);
	INT_t setSyncTime(const LONG & Offset);
	INT_t setSyncSeek(const LONG & Offset);
	INT_t Seek(const unsigned int & Offset);
	
#endif

#if 1		//added by jaylon for getcpunum
	INT_t getCpuSerialNo( QString & CpuSerialNo);
#endif
#if 1 //add by bert 2016/08/27
INT_t gethardwareCapability( qint32 & hardwareCapability);
#endif

#if 1 /*Add by xuweiwei 2015-8-28*/
	INT_t getDisplayOutputMode(qint32 & displayoutputMode);
	INT_t setDisplayOutputMode(qint32 displayoutputMode);
	INT_t getvideoColorSpace(qint32 & colorspace);
	INT_t setvideoColorSpace(qint32 colorspace);
#endif
	
#ifdef ENABLE_DTV
	INT_t GetEntryChannelInfo(qint32 EntryId,qint32& pChannelNumber,qint32& pSubChannelCount,QString& StrProgramPids);
	INT_t PlayTuner(quint32 & playbackId);
	INT_t TunerScan();
	INT_t GetTunerChannelStrength(quint32& Strength);
	INT_t ResetDuration();
	INT_t ChannelUp();
	INT_t ChannelDown();	
	INT_t TunerMode(qint32 iStdMode);
	INT_t GetTunerMode(qint32& iStdMode);
	INT_t getTunerChannelMode(quint32 & TunerChannleMode);
	INT_t setTunerChannelMode(qint32 TunerChannleMode);
	INT_t GetFirstChNumber(quint32 &OMajor,quint32 &OMinor);
	INT_t GetForceTxMode(qint32 & iTxMode);
#endif	
	
signals:
	void PlaybackEos(const QString & strPlaybackUrl, const quint32 playbackId);
	void PlaybackEos2(const QString & strPlaybackUrl, const quint32 playbackId, const qint32 bIsCompletePlaybackEos);
	void VidOutModeChanged(const qint32 iCurVoMode);

#ifdef ENABLE_DTV	
	void NotifyTuPlayerPropmtMsg(int MagTag);
	void NotifyScanProgress(qint32 PgmNum,qint32 Progress);
	void Tuner_StartScan(qint32 startid);
	void Tuner_StopScan(qint32 stopid);
#endif
	
public slots:
	void onMediaSrvPlaybackEos(const quint32 playbackId, const QString & strPlaybackUrl, const bool);
	void onVidOutModeChanged(const int iCurVoMode);
	void onSaveMode(int eNewVoMode);	
	INT_t OnSaveVolume(qint32 iVolume);
	
#ifdef ENABLE_DTV
	void onNotifyTunerPlayerPropmtMsg(int MsgTag);
	void onNotifyScanProgress(qint32 PgmNum,qint32 Progress);
	void onTuner_StartScan();
	void onTuner_StopScan();
#endif

protected:

	
private:
	VOID OnLinuxSignal(int SigNo);
	INT_t setVolume(int VolumePercent);
	INT_t SaveVoMode(VIDEO_OUTPUT_MODE eNewVoMode);
	INT_t SaveDisplayParam(DISP_PARAM_TYPE eDispParamType, INT_t iValue);
	INT_t SaveOsdPositionParam(DISP_PARAM_TYPE eDispParamType, INT_t iValue);
	LPCSTR getPropNameDescFromDispParamType(DISP_PARAM_TYPE eDispParamType);
	LPCSTR getPropNameDescFromOsdPositionParamType(DISP_PARAM_TYPE eOsdPosParamType);
	INT_t applyDisplayParamsFromCfg(BOOL_t bContinueIfErr = TRUE);
	INT_t applyOsdPositionParamsFromCfg(BOOL_t bContinueIfErr = TRUE);
	
private:
	Qt::HANDLE m_hThread;
	SharedPtr <QDBusConnection> m_DBusConn_sp;
	SharedPtr <QDBusIf_MediaPlayer> m_DbusIf_MediaPlayer_sp;
	SharedPtr <QMediaSrv> m_MediaSrv_sp;
	BOOL_t m_bMPlayerDbusServIsReged;
	int m_VolumePercent;
	QTimer	m_VolumeSaveTimer;
	SharedPtr <CTinyXmlDoc3> m_SettingXmlDoc_sp;
	CErrChkMutex m_SettingXmlDocSpMutex;
	SharedPtr <QSerialComPort> m_SerialComPort_sp;
	SharedPtr <QVideoSync> m_QVideoSync_sp;
};

#endif	//_QMediaPlayerApp_h_

