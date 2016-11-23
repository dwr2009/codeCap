#ifndef	_MEDIA_PLAYER_CTRL_H_
#define	_MEDIA_PLAYER_CTRL_H_

#include <QObject>
#include <BaseTypeDef.h>
#include <QSharedPointer>
#ifdef	QT_NO_DBUS
#undef	QT_NO_DBUS
#endif	//QT_NO_DBUS
#include <QtDBus>
#include <QDBusConnection>
#include <QDBusInterface>

#define DBUS_MPLAYER_SERVICE_NAME  		"org.thtfit.MPlayer"
//Not necessary to use OBJECT_PATH, because we have only one MPlayer instance.
#define DBUS_MPLAYER_OBJECT_PATH  		"/org/thtfit/MPlayer"
#define DBUS_MPLAYER_OBJECT_INTERFACE  	"org.thtfit.MPlayer"
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
#define DBUS_MEDIASERVER_SERVICE_NAME     "org.thtfit.MediaServer"
#define DBUS_MEDIASERVER_OBJECT_PATH      "/org/thtfit/MediaServer/AudioPlayer"
#define DBUS_MEDIASERVER_OBJECT_INTERFACE "org.thtfit.MediaServer.AudioPlayer"
#endif

//Method names
#define	METHOD_setDataSource			"setDataSource"
#define	METHOD_Prepare					"Prepare"
#define	METHOD_Play2						"Play2"
#define	METHOD_Play						"Play"
#define	METHOD_Stop						"Stop"
#define	METHOD_setVideoPosition			"setVideoPosition"
#define	METHOD_ChangeVolume				"ChangeVolume"
#define	METHOD_Seek				        "Seek"
#define	METHOD_getCurrentVolume				"getCurrentVolume"
#define	METHOD_OnSaveVolume                  "OnSaveVolume" 
#define	METHOD_setHwRtcTime				"setHwRtcTime"
#define	METHOD_getTimeZoneSetting		"getTimeZoneSetting"
#define	METHOD_setGpioLedStatus			"setGpioLedStatus"
#define	METHOD_getMonitorInfo			"getMonitorInfo"
#define	METHOD_setPlayMode				"setPlayMode"
#define	METHOD_displayOnLedScreen		"DispOnLedScrAsync"
#define	METHOD_setDisplayParam		"setDisplayParam"
#define	METHOD_getDisplayParam		"getDisplayParam"
#define	METHOD_setOsdPosition			"setOsdPosition"
#define	METHOD_ChangeVideoOutputMode			"ChangeVideoOutputMode"
#define	METHOD_getDisplayPosition			"getDisplayPosition"
#define	METHOD_getVideoPosition			"getVideoPosition"
#define	METHOD_setPlaySpeedCtrlAsync			"setPlaySpeedCtrlAsync"
#define	METHOD_getPlaySpeedCtrl			"getPlaySpeedCtrl"
#define	METHOD_getPlayMode			"getPlayMode"
#define	METHOD_PlayTuner						"PlayTuner"
#define	METHOD_getTunerChannelStrength						"GetTunerChannelStrength"
#define	METHOD_resetDuration						"ResetDuration"
#define	METHOD_ChannelUp						"ChannelUp"
#define	METHOD_ChannelDown						"ChannelDown"
#define	METHOD_TunerScan						"TunerScan"
#define	METHOD_TunerMode						"TunerMode"
#define	METHOD_StopTunerScan						"StopTunerScan"
#define	METHOD_TunerMode						"TunerMode"
#define	METHOD_SetHdmi3DTvMode						"setHdmi3DTvMode"
#define	METHOD_SetAnalogAudioMute						"setAnalogAudioMute"
#define	METHOD_setOsdSize           "setOsdSize"
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
#define METHOD_MEDIASERVER_PLAY_AUDIO           "Play"
#define METHOD_MEDIASERVER_STOP_AUDIO           "Stop"
#endif

#define  METHOD_GetChannelTable			"Get_EntryChannelInfo"
#define  METHOD_GetFirstChNumber			"GetFirstChNumber"
#define  METHOD_GetForceTxMode			"GetForceTxMode"
#define  METHOD_GetTunerPlayerStatus			"GetLoadProgramTableState"
#define  METHOD_GetTunerMode			"GetTunerMode"

#define  METHOD_GetOutputSpdifMode			"getOutputSpdifMode"
#define  METHOD_SetOutputSpdifMode			"setOutputSpdifMode"
#define  METHOD_SetTunerChannelMode			"setTunerChannelMode"
#define  METHOD_GetTunerChannelMode		"getTunerChannelMode"

class QMediaPlayerCtrl : public QObject
{
	Q_OBJECT
public:
	QMediaPlayerCtrl();
	virtual ~QMediaPlayerCtrl();
	INT_t setDBusConnection(QWeakPointer <QDBusConnection> DBusConn_wp);
	INT_t setQDBusMpDefaultIf(QSharedPointer <QDBusInterface> & QDBusMpDefaultIf_sp);
	INT_t Play(CONST QString & strUrl, unsigned int & PLaybackId,qint32  iLoopCount);
	INT_t Seek(int &iPosition);
	INT_t Stop(UINT32_t * pPlaybackId = NULL, QString * pStrUrl = NULL);
	INT_t setVideoPosition(int x,int y,int width,int height);
	INT_t bakVideoPosition(int x_bak,int y_bak,int width_bak,int height_bak);	
	INT_t changeVolume(INT_t VolChangePercent);
	INT_t setHwRtcTime(int Year, int Month, int Date, int Hour, int Minute, int Second, int strTimeZone);
	INT_t getTimeZoneSetting(OUT int & TimezoneHourOff);
	INT_t setGpioLedStatus(CONST UINT32_t GpioLedStatusToSet, UINT32_t * CONST pActualLedStatus = NULL);
	INT_t getMonitorTypeInfo(OUT QString & TimezoneHourOff);
	INT_t setPlayMode(int PlayMode);
	INT_t DispOnLedScrAsync(QString strContentToDisp, CONST INT_t StartPosToDisp);
	INT_t setDisplayParam(int eDispParamType, INT_t iValue,INT_t isSave);
	INT_t getDisplayParam(int eDispParamType, INT_t * iValue);
	INT_t setOsdPosition(int x,int y,int width,int height,int type);
	INT_t ChangeVideoOutputMode( int  iResolutionType,int * iCurrMode);
	INT_t getDisplayPosition(int eRuaDispType,int * pCurrXpos,
		int * pCurrYpos,int * pCurrWidth,int * pCurrHeight);
	INT_t getVideoPosition(int eRuaDispType,int * pCurrXpos,
		int * pCurrYpos,int * pCurrWidth,int * pCurrHeight);	
	INT_t setPlaySpeedCtrlAsync(int iSpeed);
	INT_t getPlaySpeedCtrl(int *  iSpeedValue);
	INT_t getPlayMode(int *  iPlayMoed);
	INT_t setHdmiAudioOutput(BOOL_t bEnable);
	INT_t setAnalogAudioMute(CONST BOOL_t bMute);
	INT_t PlayTuner(CONST QString & strUrl, unsigned int & PLaybackId);
	INT_t getTunerChannelStrength(int * iStrength);
	INT_t ChannelUp();
	INT_t ChannelDown();
	INT_t ResetDuration(int iDuration);
	INT_t TunerScan();
	INT_t TunerMode(int iMode);
	INT_t  setHdmi3DTvMode(int iTvMode);//MediaPlayer::HDMI_3D_TV_MODE
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
	INT_t setQDBusMediaServerIf(QSharedPointer <QDBusInterface> & QDBusMediaServerIf_sp);
	INT_t MediaServer_PlayAudio(const QString & strUrl);
	INT_t MediaServer_StopAudio(void);
	INT_t getChannelTable(int * iEntryId,int * channelNumer,
		int * subChannelCount, QString & strProgramPids,QStringList & FilenameList);
#endif
	INT_t  getFirstChNumber(int * majorNo,int  *minorNo);
	INT_t  getForceTxMode(int * iTxMode);
	INT_t  getTunerMode(int * iCurrTunerMode);	
	INT_t getOutputSpdifMode(int * iSpdifMode);
	INT_t setOutputSpdifMode(int iMode); 
	INT_t setTunerChannelMode(int iTunerChannelMode);
	INT_t getTunerChannelMode(int * iCurrTunerMode);
	INT_t getCurrentVolume(int *iCurVolume);
	INT_t OnSaveVolume(int iVolume);
    
    INT_t setOsdSize(int iWidth,int iHeight);

signals:
       void onChangeVolume_signal(int& VolChangePercent);

private:
	QWeakPointer <QDBusInterface> m_QDBusMpDefaultIf_wp;
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
	QWeakPointer <QDBusInterface> m_QDBusMediaServerIf_wp;
#endif
	QWeakPointer <QDBusConnection> m_DBusConn_wp;
};

#endif	//_MEDIA_PLAYER_CTRL_H_

