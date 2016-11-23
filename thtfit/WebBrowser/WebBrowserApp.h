#ifndef	_WEB_BROWSER_APP_H_
#define	_WEB_BROWSER_APP_H_

#include <QtWidgets/QApplication>
#include <BaseTypeDef.h>
#include "browser.h"
#include "PlaylistData.h"
#include <QTimer>

#ifdef	QT_NO_DBUS
#undef	QT_NO_DBUS
#endif	//QT_NO_DBUS
#include <QtDBus>
#include <QDBusConnection>
#include <QObject>
#include <QtScript>
#include "QPowerMateClient.h"
#include "MediaPlayerCtrl.h"
#include "qplayerwindow.h"
#include "configuration.h"
#include "qvolumewin.h"
#include "NetworkManagerClient.h"
#include "NetMgrEthernetClient.h"
#include <QByteArray>
#include "DataCommService.h"
#include "WebBrowserSrv.h"
#include "Api/WebBrowserAppDef.h"
#include "WebBrowserAppIf.h"
#include <QMutex>
#include <QInputDialog>
#include <QMessageBox>
#include "MediaVirtualChannel.h"
#include "qplaypic_html_win.h"
#include "GpioFilePath.h"
#include "qshowlogo.h"
#include "SysBaseSrvApi.h"
#include <MediaPlayerAppDef.h>
#include <StatusMsgBox.h>
#include <QUdpSocket>
#include <QtNetwork>
#include <QAutoLock.h>
#include "BackendTransactionThread.h"
#include <EventLog.h>
#include <map>
#include <String.h>
#include <StackBufString.h>
#include <File.h>
#include <QProcess>

#include "QWbStatusBar.h"
#include <EventLogExt.h>
#include "ControlCenterApp.h"
#include <DevConnectionStatus.h>
#include "QScanProgressBar.h"
#include "QInputcontentdlg.h"
#include <QVariant>
#include <GuiOsd2Ctrl.h>
#include <AppFilePathManage.h>


using namespace ControlCenter;
using namespace MediaPlayer;

#define	FOLDER_NAME_VIDEO			"Video"
#define	FOLDER_NAME_PICTURE			"Picture"
#define	FOLDER_NAME_MUSIC			"Music"
#define	FOLDER_NAME_HTML				"Html"
#define	FOLDER_NAME_PLAYLIST			"Playlist"
#if 1/*added by lxj 2012-10-9*/
#define FOLDER_NAME_USB_CAPTURE     "Capture"
#endif

#define FOLDER_NAME_TUNER     "Tuner"

#define	DEFAULT_PLAYLIST_DIR			"LocalMedia/Playlist/h0"
#define	DEFAULT_CONFIG_PATH		"./Config/SystemConfig.ini"
#define	APP_JavaScript_FILE_PATH	"./Script/WebBrowser.js"
#define	APP_INFO_CONFIG_PATH			"AppInfo.ini"

#define    PLAY_STATUS_PAUSE		"PAUSE"
#define    PLAY_STATUS_PLAY		"PLAY"
#define    PLAY_STATUS_IDLE		"IDLE"
#define    PLAY_STATUS_FAST		"FAST"
#define    PLAY_STATUS_REW			"FAST REW"

#define    PLAY_STATUS_TRANSFER		"TRANSFER"


#define 	CONFIG_NAME	"Config"
#define 	IMAGE_PATH_NAME  "Image"


#define 	GPIO_BTN_0_DOWN	 10
#define 	GPIO_BTN_7_DOWN	 17
#define 	GPIO_BTN_0_UP		 20
#define 	GPIO_BTN_7_UP		 27

#define 	VideoSurface_WIDTH			4096
#define 	VideoSurface_HEIGHT			4096
#define	EXCE_FAIL_ERROR	-1

#define	APP_JavaScript_FILE_PATH	"./Script/WebBrowser.js"

//Ini config keys
#define	INI_KEY_CurPlaylistAFilePath			"CurrPlayList"
#define	INI_KEY_CurPlaylistBFilePath			"CurrPlayListB"
#define	INI_KEY_CurActiveMediaType			"CurrActiveMedia"
#define	INI_KEY_CurIdlePlaylistFilePath			"SNMP/CurrIdlePlayList"
#define	INI_KEY_IdleDetectTime			"SNMP/IdleDetectTime"
#define	INI_KEY_FlippingIntervalTime			"SNMP/FlippingIntervalTime"
#define	INI_KEY_FlippingIntervaImageId			"SNMP/FlippingIntervaImageId"
#define	INI_KEY_DefaultStartupItem			"SNMP/DefaultStartupItem"
#define	INI_KEY_AutoRebootInterVal		"SNMP/AutoRebootInterValue"
#define	INI_KEY_TrapEnable		"SNMP/TrapEnable"
#define	INI_KEY_AutoRebootEnable		"SNMP/AutoRebootEnable"
#define	INI_KEY_SystemUpgradeUrl		"SNMP/SystemUpgradeUrl"
#define	INI_KEY_LoggingEnable           "SNMP/LoggingEnable"
#define	INI_KEY_UserOSDAddressUrl		"SNMP/UserOSDAddressUrl"
#define	INI_KEY_ResetDurationTime		"SNMP/ResetDurationTime"
#define	INI_KEY_BootSplashPlayUrl		"SNMP/BootSplashPlayUrl"
#define	INI_KEY_CurrPlayTunerUrl		"SNMP/CurrPlayTunerUrl"
//#define	INI_KEY_CurrTunerMode		"TUNER/TunerMode"
#define	INI_KEY_CurrAudioOutputMode		"SNMP/AudioOutputMode"

/*add by carl to  get appinfo*/
#define	INI_KEY_XappVersion					"VersionInfo/XAppVersion"
#define	INI_KEY_HardWareVersion				"VersionInfo/HardWareVersion"
#define	INI_KEY_FirmVersion				    "VersionInfo/FirmVersion"
#define	INI_KEY_BrowserVersion				"VersionInfo/BrowserVersion"



#if 1/*added by lxj 2012-8-2 for deviceKey*/
#define INI_KEY_DEVICE_KEY              "DeviceKey/EnabledFeatures"
#endif

#if 1/*by_su for Language index key*/
#define KEY_Language_Type              "Language/CurLanguageIndex"
#endif

#define	KEY_NetMgrEthernet_DNSAddr1			"NetMgrEthernet/DnsAddr1"
#define	KEY_NetMgrEthernet_DNSAddr2			"NetMgrEthernet/DnsAddr2"
#define	KEY_NetMgrEthernet_GatewayAddr			"NetMgrEthernet/GatewayAddr"
#define	KEY_NetMgrEthernet_IPv4Addr			"NetMgrEthernet/IPv4Addr"
#define	KEY_NetMgrEthernet_SubnetMask			"NetMgrEthernet/SubnetMask"

#define	INTERNALIMAGEID                                       "InternalImage%1.jpg"


#define	Playlist_NAME_DELIMETER_CHAR			"%"

#define	INI_KEY_CoordinateMode			"CurrCoordinateMode"
#define	INI_KEY_Xpos			"xleft"
#define	INI_KEY_Ypos			"yupper"
#define	INI_KEY_XSize			"xsize"
#define	INI_KEY_YSize			"ysize"
#define	INI_KEY_AlphaValue			"alpha"
#define	INI_KEY_LayerNumber			"LayerNumber"

#define	VOLUMEDOWN	0
#define	VOLUMEUP	1

#if 1/*add by lxj 2012-7-24 save*/
#define MAX_MEDIATYPE_LIST  50
typedef struct{
    UINT PlaybackId;
	State_TYPE iStateType;
}tMediaType,*pMediaType;
#endif

#if 1/*added by lxj 2012-7-31 for features enabled en/decrypt*/
#define ENCRYPT_FIRST_KEY             "tEchnoVARE$"
#define ENCRYPT_ASCII_LOW             33
#define ENCRYPT_ASCII_HIGH            122
#define MAX_ENABLED_FEATURES_LENGTH   10

#define LANG_NO_LICENSE               "No license to play"
#endif

#if 1/*added by lxj 2012-12-11*/
#define TS_CALIBRATION_PROGRAM_NAME   "TouchScreenCalibrate.sh"
#define TS_CALIBRATION_PROGRAM_PATH   "/usr/bin"
#define TS_CALIBRATION_TIMEOUT_MS     (2*1000)
#endif

class QWebBrowserApp;


class QDBusIf_UiSrv : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.ControlCenter.UiSrv")
public:
	QDBusIf_UiSrv(QWebBrowserApp * pParentObj);
	void setParentObj(QWebBrowserApp * pParentObj);
public slots:
	void setPlayMode(int PlayMode, int & iOutRet);
	void startPlaylist(QString strPlaylist, int & iOutRet);
	void playMedia(QString strFilePath,int  & iOutRet);
	//void playMedia(int eDeviceType, QString strFileType, QString strFilename, int iLoopTimes  ,int iDurationTime,int & iOutRet );
	void playNextFile(QString strFilePath,int  & iOutRet);
	//int eDeviceType, QString strFileType, QString strFilename, int iLoopTimes  ,int iDurationTime, int & iOutRet);
	void getCurrentPlayItem(QString & strFileName,int & iOutRet);
	void setPlayVolume(int iVolumeValue,int & iOutRet);
	void getPlayState(int & iOutRet,int &iState);
	void getPlayVolume(int & iOutRet,int &iVolumeValue);
	void setDefaultIdlePlaylist(QString strIdlePlaylist, int & iOutRet);
	void setIdleDetect(int iIdleSeconds,int & iOutRet);
	void setVideoPosition(int iPosType,int iPosValue,int & iOutRet);
	void playBrowser(QString strFilePath,int  & iOutRet);
	void setPlayerMute(int iMuted,int &iOutRet);
	void getPlayerMute(int & iOutRet, int & bMuted);
	void setDisplayParam(int eDispParamType, int iValue,int bSave,int &iRet );
	void getDisplayParam(int eDispParamType,int & iCurrValue,int &iRet );
	void setCoordinateMode(int iMode,int & iRet);
	void setShitfPosition(int eShitfType,int iShiftValue,int & iOutRet);
	void setFlippingInterval(int iIntervalVaule,int &iOutRet);
	void getFlippingInterval(int &iOutRet,int  & iIntervalVaule);
	void setPlayInternalImage(int iIntervalImageId,int &iOutRet);
	void setDefaultStartupItem(const QString & strFileInfo,int & iOutRet);
	void setSpeedControl (int iSpeedValue,int & iOutRet);
	void setPlayVirtualChannel(int iChannelId,int &iOutRet);
	void setVirtualChannelId(int VirChid, int & iOutRet);
	void getVirtualChannelId(int & iOutRet, int & VirChid);
	void setVirChMedia(QString strVirChMedia, int & iOutRet);
	void getVirChMedia(int & iOutRet, QString & strVirChMedia);
	void setHorizontalSize(int iSize,int & iOutRet);
	void getHorizontalSize(int & iOutRet, int & iSize);
	void setHorizontalShift(int iSize,int & iOutRet);
	void getHorizontalShift(int & iOutRet, int & iSize);
	void setVerticalSize(int iSize,int & iOutRet);
	void getSpeedControl (int & iOutRet,int & iSpeedValue);
	void getShitfPosition(int eShitfType,int &  iShiftValue,int & iOutRet);
	//void getPlayMedia(int & iOutRet,QString & strUrl);
	void setVirtualChannelAlias(QString strVirChAlias, int & iOutRet);
	void getVirtualChannelAlias(int & iOutRet, QString & strVirChAlias);
	void setVirChLabelDispPos(int DispPos,  int & iOutRet);
	void getVirChLabelDispPos(int & iOutRet,  int & DispPos);
	void setVirChEnable(int bEnable, int & iOutRet);
	void getVirChEnable(int & iOutRet, int & bEnable);
	void setVirChDispDuration(int DispDuration, int & iOutRet);
	void getVirChDispDuration(int & iOutRet, int & DispDuration);
	void setVirChMin(int MinVal, int & iOutRet);
	void getVirChMin(int & iOutRet, int & MinVal);
	void setVirChMax(int MaxVal, int & iOutRet);
	void getVirChMax(int & iOutRet, int & MaxVal);
	void getPlayMode(int & iOutRet, int & iCurrMode);
	void getDefaultIdlePlaylist( int & iOutRet,QString &strIdlePlaylist);
	void getPlayInternalImage(int &iOutRet,int & iIntervalImageId);
	void getplayPlaylist(int &iOutRet,QString & strPlaylist);
	void getDefaultStartupItem(int & iOutRet,QString & strFileInfo);
	void setrebootSystem(int iId, int & iOutRet);
	void getIdleDetect(int & iOutRet,int & iIdleSeconds);
	void setPullConfig(QString strUrl, int & iOutRet);
 	void setPushConfig(QString strUrl, int & iOutRet);
 	void setAutoReboot(int iEnabled,int &iOutRet);
	void setAutoRebootInterval(int iTime,int & iOutRet);
	void getAutoRebootInterval(int & iOutRet,int &iTime);
	void setTrapEnable(int iEnable,int & iOutRet);
	void getTrapEnable(int & iOutRet,int &iEnable);
	void getCoordinateMode(int & iOutRet,int &iMode);
	void getAutoReboot(int & iOutRet,int &iEnable);
	void setAlphaLayer(int iAlphaValue,int & iOutRet);
	void setActiveLayer(int iLayerId,int & iOutRet);
	void getCardCurrentStatus(int & iOutRet,QString &strCardCurStatus);
	void setFormatMedia(int iAction, int & iOutRet);
	void getFormatMedia(int & iOutRet, int & DummyValue);
	void getAudioOutputMode(int & iOutRet, int & iAudioOutputMode);
	void setAudioOutputMode(int iAudioOutputMode, int & iOutRet);
	void setAutoAnnounce(int iAutoAnnounceIntervalSec, int & iOutRet);
	void getAutoAnnounce(int & iOutRet, int & iAutoAnnounceIntervalSec);
	void getMediaStatus(int & iOutRet, QString & strMediaStatus);
	void getXappVersion(int & iOutRet, QString & strXappVersion);
	void setSystemUpgradeAddress(const QString & strUrl,int & iOutRet);
	void setMaintainLog(int eCurrActiveMedia,int & iOutRet);
	void setTransferLog(QString strLogUrl,int & iOutRet);
	void setUserOSDAddress (const QString & strUrl,int & iOutRet);
	void setPlayTuner(QString strTunerUrl,int  & iOutRet);
	void getTunerChannelStrength(int & iOutRet, int & iStrength);
	void setTunerScan (int iAction,int  & iOutRet);
	void setTunerMode(int iSetMode,int & iOutRet);
	void setChannelUp(int iAction,int  & iOutRet);
	void setChannelDown(int iAction,int  & iOutRet);
	void setResetDuration (int iDurationTime,int & iOutRet);
	void getResetDuration(int & iOutRet, int & iDurationTime);
	void getTransferControl(int & iOutRet, int & TransferStatusCode);
#if 1/*added by lxj 2012-7-31 for features enabled*/
	void setDeviceKey(QString strFromEnabledFeatures, int  & iOutRet);
	void getEnabledFeatures(int & iOutRet, int & iEnabledFeatures);
#endif
	void getPlayTuner(int & iOutRet, QString & strCurrTunerChannel);
	void getTunerScan(int & iOutRet, int & iScanStatus);
	void getForceTxMode(int & iOutRet, int & iTxMode);
	void getSubChannelCount(int & iOutRet, int & iCount);
	void getChannelTable( int & iOutRet, QStringList  & strInfoList);	
	void get_tnetVersion(int & iOutRet, qint32 & iNumber);
	void MPlayerPlugin_Play(QString strUrl,int & iOutRet);
	void setBootSplashPlayUrl(QString strBootSplashUrl, int & iOutRet);
	void getBootSplashPlayUrl(int & iOutRet, QString & strBootSplashUrl);
	void getTunerMode(int & iOutRet, int & iCurrTunerMode);
#if 1/*added by lxj 2012-12-11 for touch screen calibrate from SNMP command*/
	void SetTouchScreenCalibrateAsync(int iAction, int & iOutRet);
#endif
#if 1/*added by lxj 2012-12-17 for overlay by javascript dynamic run*/
	void getHtmlEvalJavascript( int & iOutRet, QString & strJavascript );
	void setHtmlEvalJavascript( QString strJavascript, int & iOutRet );
#endif
	void setVideoLayerPlayMedia(const QString & strUrl, int & iOutRet);
	void setTunerChannelMode(int iTunerChMode, int & iOutRet);
	void getTunerChannelMode(int & iOutRet, int & iTunerChMode);
	void setVideoLayerPlayMode(int iVideoLayerMode,int & iOutRet);
	void setHtmlLayerPlay(const QString & strUrl, int & iOutRet);
	void setPlayNext(int & iOutRet);

private:
	QWebBrowserApp * m_pRealObj;
};

class QNextFileInfoToPlay
{
public:
	QString m_strFilename;
	QString m_strFileType;
	int m_iLoopTimes;
	int m_iDuration;
	int m_iPosition;
	int m_iDeviceType;
	bool m_bIsInitZero;
	bool m_bIsInitOne;
	bool m_bIsVideoLayer;
};

class QWebBrowserApp : public QControlCenterApp, public CWebBrowserAppIf
{
	Q_OBJECT
	friend class QDBusIf_UiSrv;
public:
	static int m_SwitchableVidOutMode_Table[];
public:
	QWebBrowserApp(int argc, char *argv[]);
	virtual ~QWebBrowserApp();
	static VOID SigHandler(VOID * pCbContext, int SigNo);
	int exec();
	INT_t InitInstance();
	INT_t ExitInstance();
	virtual bool notify(QObject *receiver, QEvent *event);
	virtual CMutex2 * getPlaylistAccessLocker();
	virtual CMp7xxGlobalStatus & getMp7xxGlobalStatusObj();
	INT_t ChangeVolume(int VolumeChnageVal,bool isVideoSet = false);
	virtual INT_t getDstFilePath(const QUrl & SrcFileUrl, const QString & strFileType,
		QString & strDstFileName, QString & strDstFilePath);
	virtual INT_t getFullFilePath(const QString & strFilename, const QString & strFileType, QString & strFullFilePath);
	virtual INT_t getPlaylistFullPath(const QString & strPlaylistName, QString & strFullPath);
	virtual INT_t setActiveMedia(CONST ActiveMedia_TYPE eActiveMediaType);
	virtual INT_t getActiveMedia(QString & strActiveMedia, QString & strActiveMediaDirName);
	virtual INT_t getMediaFreeSpace(qulonglong & MediaFreeSpaceInBytes);
	void startPlaylist(CONST QString & strPlaylist, int & iOutRet);
	void playMedia(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType, CONST QString & strFilename,
					int & iOutRet, int iLoopTimes ,int iDurationTime, int iPositionTime, const QString & strOriginUrl = QString(),bool isPlayNextFile = false,bool bIsVideoLayer =false);
	void playNextFile(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType, CONST QString & strFilename,
						int & iOutRet ,int iLoopTimes = -1 ,int iDurationTime =-1,int iPositionTime = 0,bool bIsVideoLayer =false );
	void getCurrentPlayItem(QString & strFileName,int & iOutRet);
	void palySpecificTime(int & SpecificTime);
#if 1/*added by lxj 2012-10-10*/
	void getCurrentPlayItemInfo(State_TYPE & iStateType,QString & strUrl);
#endif
	void getPlayState(int &iState,int & iOutRet);
	void setPlayVolume(int iVolumeValue,int & iOutRet);
	void getCurrentVolume(int &iCurVolume,int & iOutRet);
	void setDefaultIdlePlaylist(QString strIdlePlaylist, int & iOutRet);
	void setIdleDetect(int iIdleSeconds,int & iOutRet);
	void setPlayerMute(int iMuted,int &iOutRet);
	void getPlayerMute(int & iOutRet, int & bMuted);
	void setVideoPosition(int iPosType,int iPosValue,int & iOutRet);
	void setDisplayParam(int eDispParamType, INT_t iValue,int &iRet ,int bIsSave = 1);
	INT_t getDisplayParam(int eDispParamType,int & iCurrValue);
	INT_t switchToNextVideoOutputMode();
	INT_t  getDisplayPosition(int eRuaDispType,int & pCurrXpos,
		int & pCurrYpos,int & pCurrWidth,int & pCurrHeight);
	INT_t getDefaultDisplayParam(int& dWidth,int& dHeight);
	void setCoordinateMode(int iMode,int & iRet);
	void setShitfPosition(int eShitfType,int iShiftValue,int & iOutRet);
	void setFlippingInterval(int iIntervalVaule,int &iOutRet);
	void getFlippingInterval(int &iOutRet,int  & iIntervalVaule);
	void setPlayInternalImage(int iIntervalImageId,int &iOutRet);
	void setDefaultStartupItem(const QString & strFileInfo,int & iOutRet);
	void  ResolveStr(QString strResolve,QString & filePath);
	void setPlayVirtualChannel(int iChannelId,int &iOutRet);
	void setVirtualChannelId(int VirChid, int & iOutRet);
	void getVirtualChannelId(int & iOutRet, int & VirChid);
	void setVirChMedia(CONST QString & strVirChMedia, int & iOutRet);
	void getVirChMedia(int & iOutRet, QString & strVirChMedia);
	void setVirtualChannelAlias(CONST QString & strVirChAlias, int & iOutRet);
	void getVirtualChannelAlias(int & iOutRet, QString & strVirChAlias);
	void setVirChLabelDispPos(int DispPos,  int & iOutRet);
	void getVirChLabelDispPos(int & iOutRet,  int & DispPos);
	void setVirChEnable(int bEnable, int & iOutRet);
	void getVirChEnable(int & iOutRet, int & bEnable);
	void setVirChDispDuration(int DispDuration, int & iOutRet);
	void getVirChDispDuration(int & iOutRet, int & DispDuration);
	void setVirChMin(int MinVal, int & iOutRet);
	void getVirChMin(int & iOutRet, int & MinVal);
	void setVirChMax(int MaxVal, int & iOutRet);
	void getVirChMax(int & iOutRet, int & MaxVal);
	void setSpeedControl(int iSpeedValue,int & iOutRet);
	void getShitfPosition(int eShitfType,int & iOutRet,int & iShiftValue);
	void getSpeedControl(int & iOutRet, int &iCurrSpeed);
	void getPlayMode(int & iOutRet, int & iCurrMode);
	void getDefaultIdlePlaylist( int & iOutRet,QString &strIdlePlaylist);
	void getPlayInternalImage(int &iOutRet,int & iIntervalImageId);
	void getplayPlaylist(int &iOutRet, QString & strPlaylist);
	void getDefaultStartupItem(int &iOutRet,QString & strPlaylist);
	void getIdleDetect(int & iOutRet,int & iIdleSeconds);
	void setPullConfig(CONST QString & strUrl, int & iOutRet);
	void setAutoReboot(int iEnabled,int &iOutRet);
	void setAutoRebootInterval(int iTime,int & iOutRet);
	void getAutoRebootInterval(int & iOutRet,int &iTime);
 	void setPushConfig(CONST QString & strUrl, int & iOutRet);
	void setTrapEnable(int iEnable,int & iOutRet);
	void getTrapEnable(int & iOutRet,int & iEnable);
	void getCoordinateMode(int & iOutRet,int &iMode);
	void getAutoReboot(int & iOutRet,int &iEnable);
	void SystemReboot();
	void setAlphaLayer(int iAlphaValue,int & iOutRet);
	void setActiveLayer(int iLayerId,int & iOutRet);
	void getCardCurrentStatus(int & iOutRet, QString &strCardCurStatus, BOOL_t bOnlyGetRawData = FALSE,
		OUT quint8 * pRawDataBuf = NULL, size_t sizeRawDataBuf = 0, size_t * pActualRawDataSize = NULL);
	void setFormatMedia(int iAction, int & iOutRet);
	void setFormatMediaAsync(int iAction, int & iOutRet);
	int parsePlayMediaCommad(const QString & strPlayFilePath,int  & iOutRet,int iCommadType);
	void getAudioOutputMode(int & iOutRet, int & iAudioOutputMode);
	void setAudioOutputMode(int iAudioOutputMode, int & iOutRet);
	void setAutoAnnounce(int iAutoAnnounceIntervalSec, int & iOutRet);
	void getAutoAnnounce(int & iOutRet, int & iAutoAnnounceIntervalSec);
	void getMediaStatus(int & iOutRet, QString & strMediaStatus);
	void getXappVersion(int & iOutRet, QString & strXappVersion);
	void setSystemUpgradeAddress(const QString & strUrl,int & iOutRet);
	void setMaintainLog(int eCurrActiveMedia,int & iOutRet);
	void setTransferLog(QString strLogUrl,int & iOutRet);
	void setUserOSDAddress (const QString & strUrl,int & iOutRet);
	void setPlayTuner(QString strTunerUrl,int  & iOutRet);
	void getTunerChannelStrength(int & iOutRet, int & iStrength);
	void setTunerScan(int iAction,int  & iOutRet);
	void setResetDuration (int iDurationTime,int & iOutRet);
	void setTunerMode(int iSetMode,int & iOutRet);
	void setChannelUp(int iAction,int  & iOutRet);
	void setChannelDown(int iAction,int  & iOutRet);
	void getResetDuration(int & iOutRet, int & iDurationTime);
	void getTransferControl(int & iOutRet, int & TransferStatusCode);
	void getTunerScan(int & iOutRet, int & iScanStatus);
	virtual int setVideoMode(const int eCeSnmpVidMode);
	void getForceTxMode(int & iOutRet, int & iTxMode);
	void getChannelTable( int & iOutRet, QStringList  & strInfoList);
	void get_tnetVersion(int & iOutRet, qint32 & iNumber);
	void setBootSplashPlayUrl(QString strBootSplashUrl, int & iOutRet);
	void getBootSplashPlayUrl(int & iOutRet, QString & strBootSplashUrl);
	void getFirstChNumber(int & iOutRet, int & majorNo,int & minorNo);
	void getPlayTuner(int & iOutRet, QString & strCurrTunerChannel);
	void getTunerMode(int & iOutRet, int & iCurrTunerMode);
	QString  ResolveCommadToUrl(const QString & strCommad,QString & strFileType,QString & strOtherPar);
	INT_t  PlayUrl(const QString & strUrl, const QString & strFileType,QString strOtherPar = "",int iEntryIndex = 0);
	void playVideoLayer(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType, CONST QString & strFilename,
					int & iOutRet, int iLoopTimes ,int iDurationTime, int iPositionTime,const QString & strOriginUrl = QString(),bool isPlayNextFile = false);
	void setTunerChannelMode(int iTunerChMode, int & iOutRet);
	void getTunerChannelMode(int & iOutRet, int & iTunerChMode);
	virtual int setHdmi3DTvModeSlot(int Hdmi3DTvMode);
	void setVideoLayerPlayMode(int iVideoLayerMode,int & iOutRet);
	QString getCurrMainHtmlUrl();
#if 1 /*Add by xuweiwei 2014-2-21*/
	bool getCurrPlaybackEosFlag();
#endif

#if 1 /*Add by xuweiwei 2014-3-7*/
	int WD_SetParam(int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs);
#endif

	void setItemTemporary(QString strFilePath);
	void PlayItemTemporary();
	void setMediaPlayType(AMedia_File_TYPE type);

protected:
    virtual bool event(QEvent * pEvent);
public slots:
	void OnTimer_ConnectDBus();
	void OnTimer_ConnectMPSignal();
	void On_DbusConnectedToMediaPlayer();
	void On_MP_PlaybackEos2(const QString CurPlayUrl, const unsigned int PlaybackId, const int bIsCompletePlaybackEos);
	void On_DefKey_Down(int keyValue);
	void setVideoPosition(int x,int y,int width,int height, bool bFullScreen = false);
	void setPlayListActionPosition(int x,int y,int width,int height);
	void UpdateVideoPosition(bool bFullScreen = false);
	void getPlayListFiles(int media_type,QStringList & list,QStringList &filePathInfoList);
	void PlayCurrentFile(const QString fileName,int fileType);
	void OnTimer_checkVolumeWin();
	void OnTimer_checkPlayDurationTimeOut();
	void OnTimer_checkPlaylistDurationTimeOut();
	void On_Timer_checkHtmlDisPlayTimeOut();
	void SwitchToPlayWin();
	void GetDhcpInfoByDbus(QStringList & dhcpInfoList);
	void SetDhcpInfoByDbus(QStringList  dhcpInfoList,bool isAutoGetIp);
	void SetVideoParInfoByDbus(QString paraName ,int paraValue );
	void GetNetWorkInfo(QStringList & dhcpInfoList);
	void SetSysTimeInfo(QStringList timeInfoList);
	int OnTimer_OpenGpioBtnInputFile();
	void On_GpioBtnInputEvent(int SocketFd);
	static VOID LinuxSigHandlerCb(VOID * pCbContext, int SigNo);
	void QSigHandler(int SigNo);
	virtual void SetGpioLedStatus(unsigned int status);
	void  GetMonitorTypeInfoByDbus(QStringList & monitorTypeList);
	void  GetVersionInfo(QStringList & versionInfoList);
	bool ResetConfiguration();
	void   ReStart();
	void   PlayIdlePlayListFile();
	void setPlayMode(const int PlayMode, int & iOutRet);
	void setPlayModeNormal(int iPlayMode);
	int GetSysTimeZone (int & currTimeZone);
	int Load_JsFile();
	int  OpenPlaylist(const QString & playListName);
	int setOsdPosition(int x, int y, int Width, int Height,int type);
	int   ResetSnmpPassword();
	int SaveMediaVirChInfoCfg();
	int SyncCacheToStorage();
	/*  add by cuitt 2015-07-02 */
	void BringBrowserToFront();

	
#if 1/*added by lxj 2012-12-7*/
	int On_TouchScreenCalibrationDone();
#endif
#if 1/*added by su 2013-06-18*/
	int On_ChangeLanguageDone(int data);
#endif
	int On_DataCommSrv_DonwloadFinished(int iDownloadErrNo, quint32 DonwloadId, const QString & strSrcUrl, const QString & strDestUrl);
	int On_NewSysCfgIsReady(const QString & SysCfgDirPath);
	int On_GpioBtnInput_Started();
	virtual void DbgLog(const QString & strDbgInfo);
	int EnableDisableInput(int iEnableFlag);
	QStringList getChannelStreamList();
	int UpdateVirChMedia(const QString strVirMedia , int  ChannleId);
	int DeleteVirChMedia(const QString strVirMedia , int  ChannleId);
	QStringList getChannelIdList();
	QStringList  getInputEnabled();
	int UploadCfg();
	void On_VidOutModeChanged(int iMode);
	int On_tnetReady_Sig();
 #if 1 /*Add by xuweiwei 2013-12-23*/
	int On_HtmlPlaybackEos_Sig(const QString CurPlayUrl, const unsigned int PlaybackId, const int bIsCompletePlaybackEos);
 #endif
	QString getDefEthDevName();
	void On_DhcpDone2(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
		const QString & strGatewayAddr,const QString & strDns);
	void OnTimer_checkPlayPluginEntryDurationTimeOut();
	void OnTimer_checkPlayPluginHtmlEntryDurationTimeOut();
	QString getVirChannelStreamInfo(int iChannelId);
	int Send_AutoAnnounce();
	void TarUpgradeZipFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void On_AsyncTransactionFinished(const MessageItem_SharedPointer_t & MsgItem_sp);
	void On_FormatMedia_Finished(const MessageItem_SharedPointer_t & MsgItem_sp);
	void onSnmpSetFinished(const MessageItem_SharedPointer_t & MsgItem_sp);
	void onSnmpGetFinished(const MessageItem_SharedPointer_t & MsgItem_sp);
	void On_StartTransferSlot(QString strSrcUrl,QString strDestUrl,int iAction);
	void TarUpgradeOsdFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void  TarUpgradeZipError(QProcess::ProcessError exitCode) ; //added by Rocky
	int  tunerScan(int iMode);
	void OnTimer_checkMenuOsdTimeOut();
	void OnNotifyScanProgress(int iPgmNum,int iProgress);
	void OnShowProgressBarForScan(int iType);
	void OnHideProgressBar(int iType);
	int PlayMedia(const QString & strUrl, int iLoopCount = 1);
	int StopMedia();
	virtual int MediaServer_PlayAudio(const QString & strUrl);
	virtual int MediaServer_StopAudio();
	void setSnmpPassFinishSlot(int iResult);
	int MPlayerPlugin_Play(QString strUrl);
	QStringList GetWirelessStatus();
	QStringList GetWirelessDhcpInfo();
	INT_t SaveWifiCfg(QString & strParam,int iCurStatus);
	void OnTimer_checkBootSplashTimeOut();
	int SaveWirelessNetworkAddr(QString tempNetworkAddrInfo);
	int getWifiSignalLevel();
	int getTunerChannelStrength();
	int Exec_PlayTunerChannel_Number(int iKeyCode);	
	void OnNotifyTunerPlayerPropmtMsg(int iType);
	void Play_InputChannel(QString strChannelNumber);
	int MP_Play(const QString & strUrl, int iLoopCount = 1);
#if 1/*added by lxj 2012-12-11*/
	int setTouchScreenCalibrateAsync(void);
	int closeTouchScreenCalibrate(void);
#endif
	int getOutputSpdifMode();
	int setOutputSpdifMode(int iMode); 
#if 1/*added by lxj 2012-12-24*/
	void Set_CursorVisible(int b);
#endif
	int adjustVolume(int adjustVolumePercent);
	int SnmpCall_Set(const QString strOid, const QVariant varArgs);
	QString SnmpCall_Get(const QString strOid);
	int getTunerChannelMode();
	int SaveTunerChannelMode(int iSaveMode);
	int setFormatMediaAsync(int iAction);
#if 1/*by_su*/
	int SaveLanguageIndex(int );
	int GetLanguageIndex();
	int GuiOsd2VideolistPlay(const QString strUrl);
#endif
	QVariantList GuiOsd2_getScreenSize();
	int GuiOsd2_ClearScreen();
	int GuiOsd2_ClearRect(int x, int y, int w, int h);
	int GuiOsd2_drawTextAlignCenter(int x, int y, int w, int h, const QString & text);
	int GuiOsd2_setFontSize(int pixelSize);
	int GuiOsd2_setTextColor(int r, int g, int b, int alpha);
	int GuiOsd2_fillRect(int x, int y, int w, int h, int r, int g, int b, int a);	
	int GuiOsd2_DrawImage(const QString & strFilePath, int scrX, int scrY);
       int GuiOsd2_DrawTiledImage(const QString & strFilePath, int scrX, int scrY, int scrW, int scrH);
       int GuiOsd2_setFontName(const QString& strFontName);

signals:
	void PostAsyncTransaction(const MessageItem_SharedPointer_t & MsgItem_sp);
	void onPlayPluginListBackEnd();
private:
	VOID OnLinuxSignal(int SigNo);
	bool LoadPlaylist(QString strPlaylistUrl = "");
	INT_t PlayUrl(CONST QString & strUrl,qint32 iLoopTimes =1 );
	INT_t StopPlayback();
	VOID PlayNextEntry(bool bFirstEntry = false,bool bPreEntry = false);
	VOID PlayFirstEntry();
	virtual void customEvent(QEvent * event);
	//virtual bool event(QEvent * pEvent);
	VOID ShowBrowserWindow();
	VOID ShowPlayerWindow();
	VOID setForegroundWindow(QWidget * pActiveWindow);
	VOID BringWindowToFront(QWidget * pActiveWindow);
	QString LookForActualFilePath(QPlaylistEntry::ACTION_TYPE eAction, QString & strFileName,QString strFloderName="");
	VOID CloseGpioBtnInputFile();
	virtual VOID On_GpioBtnKey(BYTE BtnKeyCode);
	VOID StopPlayDurationTimer();
	QString getDirNameFromFileType(const QString & strFileType);
	QString getStorageDirNameFromDstFileName(const QString & strDstFileName, QString * pstrActualPlaylistFilename = NULL);
	QString getStorageDirNameFromCeLabsDataSrcDev(const QString & strCeLabsDataSrvDev);
	virtual QString getActiveMediaDescFromType(ActiveMedia_TYPE eActiveMediaType);
	INT_t SavePlaylistAsPersistent(const QString & strPlaylistAFilePath, const QString & strPlaylistBFilePath = "");
	INT_t SaveIdlePlaylist(const QString & strIdlePlaylistFilePath);
	QString getDevMediaDirNameByType(CONST ActiveMedia_TYPE eDeviceType);
	INT_t QueueNextFileToPlay(const QString & strFileFullPath, const QString & strFileType,int eDeviceType,int  iLoopTimes,int iDuration,int iPosition,bool bIsVideoLayer =false);
	void getCurrentPlayEntryInfo(QString & strEntryType,QString & strEntryFileName);
	void getLastPlayEntryInfo(QString & strEntryType,unsigned int PlaybackId);/*add by lxj 2012-7-20 for get last status type string */
	QString getFolderNameByFileType(int iFileType);
	//QString getDeviceNameFromFlashType(const QString & strFileType);
	INT_t PlayCeLabsUrl(const QString & strCeLabsUrl,bool isPlayVirtualChannle =false);
	QString getFileFullPathByCeLabsUrl(const QString & strDataSrcDevice, const QString & strFileType, const QString & strFilename);
	QString getStorageDirNameFromActiveMediaType(CONST ActiveMedia_TYPE eActiveMediaType);
	VOID ShowIpInfoOnLed();
	VOID ResetPlayListEntryPostionInfo();
	int CallPlayWebpage(QString pageUrl,int iDurationTime = -1,bool isPlaySelectFile = false);
	VOID CallPlayPicturepage(QString pageUrl,int iDurationTime = -1);
 	INT_t OnNewSysCfgDownloadFinished(quint32 DownloadId);
 	VOID ShowPlayPicOrHtmlWindow();
	QString  GetDefaultStartupItemUrlByItemFullPath(QString strItemFullPath);
	QString   getDevMediaDirNameByDirName(CONST QString strDirName);
	void ShowLogo();
	QString getGpioPlayFilePath(int iGpioKey);
	void setGpioPlayFilePath(QString storageMedium = USB_STORAGE_DIR_NAME);
	void checkAutoReboot();
	void Show_StatusMsgBox(const QString & strTitle, const QString & strContent, BOOL_t bPaintImmediately = FALSE, INT_t iTimeoutMs = -1);
	void  getFileInfoByStr(const QString & strPlayFileUrl,QString & getStrDataSrcDevice,QString &  getStrFileType,QString & getStrFileName );
	ActiveMedia_TYPE getDevTypeByStr(QString strActiveMedia);
	int formatFlashLocalMedia(const QString strDirPath);
	INT_t OnNewSystemVersionDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl);
	INT_t CreateSckForAutoAnnounceIfNeed();
	INT_t RecreateSckForAutoAnnounce();
	bool parseUpdateXml(QString strXmlUrl,QString & strUpgradeXml,QString & strVersion);
	INT_t OnNewSysVersionCfgDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl);
	INT_t CompareVersion(const QString & strVersionA, const QString & strVersionB);
	INT_t OnNewOsdDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl);
	void startIdelDetectTimer();
	void StopAllPlayDurationTimer();
	void setVideoLayerWndSize();
	void playHtmlLayer(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType, CONST QString & strFilename,
		int & iOutRet, int iLoopTimes ,int iDurationTime, int iPositionTime,const QString & strOriginUrl = QString(),bool isPlayNextFile = false);
	INT_t stopMPlayerWithEventLog();
	VOID rememberEndedPlaybackId(const quint32 endedPlaybackId);
	BOOL_t playbackIdIsEnded(const quint32 playbackId);
	void setGuiOsd2AccordingToCurVideoMode();
public:
	INT_t setPluginListDataSource(const QSharedPointer <QPlaylistData> & PlaylistData_sp,const QString strPlayListUrl);
	INT_t PlayPluginListNextEntry(bool bFirstEntry = false,bool bPreEntry = false);
	INT_t StopPlayPluginList();
	QString getPluginPlayListUrl();
	QString ParsePlayFilePathToPlayUrl(const QString & strPlayFilePath,QString & fileType);
#if 1/*added by lxj 2012-7-31 for features enabled en/decrypt*/
	void setDeviceKey(QString strFromEnabledFeatures,int  & iOutRet);
	INT_t CheckPlayEnable(BOOL_t isShowError=TRUE);
    QString GetMacAddress(void);
    QString ScrambleMac(QString mac);
    QString StrEncrypt(QString source,QString key);
    QString StrDecrypt(QString secret,QString key);
#endif
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
	INT_t MediaServerPlayAudio(const QString & strUrl);
	INT_t MediaServerStopAudio(void);
#endif
#if 1/*added by lxj 2012-12-17 for overlay by javascript dynamic run*/
	int getHtmlEvalJavascript( int & iOutRet, QString & strJavascript );
	int setHtmlEvalJavascript( QString strJavascript, int & iOutRet );
#endif
	int setVideoLayerPlayMedia(const QString & strUrl);
	bool getIsPlayVideoLayer();
	int setHtmlLayerPlay(const QString & strUrl);
	int setPlayNext();
private:
	Qt::HANDLE m_MainThreadId;
	browser m_Browser;
	QPlayerWindow m_playWin;
	QPlaylistData m_PlaylistDataA;
	QSharedPointer <QPlaylistData> m_PluginPlaylistData_sp;
	QSharedPointer <QDBusConnection> m_DBusConn_sp;
	QSharedPointer <QMediaPlayerCtrl> m_MediaPlayerCtrl_sp;
	QSharedPointer <QDBusInterface> m_QDbus_MpDefaultIf_sp;
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
	QSharedPointer <QDBusInterface> m_QDbus_MediaServer_sp;
#endif
	QSharedPointer <QNetworkManagerClient> m_NetMgrClient_sp;
	QSharedPointer <QNetMgrEthernetClient> m_NetMgrEthernetClient_sp;
#if 1 /*Add by xuweiwei 2014-3-4*/
       QSharedPointer <QPowerMateClient> m_QPowerMateClient_sp;
#endif
	QSharedPointer <QBackendTransactionThread> m_BackendTransactionThread_sp;
	int m_CalcVideoPosTryTimes;
	QTimer m_CalcVideoPosTimer;
	QString m_strPlayListDir;
	QString m_StrUsbListDir;
	QString m_StrUsbListDir_2;
	QString m_strCurPlayUrl;
	QString m_StrSdListDir;
	QSharedPointer<configuration> mp_Config;
	QSharedPointer<configuration> mp_AppInfo_Config;
	QVolumeWin m_TurnVolumeWin;
	QTimer m_HideVolumeBarTimer;
	unsigned int m_PlaybackId;
	QTimer m_PlayDurationTimer;
	QTimer m_PlaylistDurationTimer;
	BOOL_t m_bShouldQuitApp;
	int m_GpioBtnInputFd;
	QSharedPointer <QSocketNotifier> m_GpioBtnInputNotifier_sp;
	QSharedPointer <QScriptEngine> m_ScriptEng_sp;
	QByteArray m_GpioBtnInputData;
	QTimer m_HtmlDurationTimer;
	bool mb_IsFullSrceen;
	QSharedPointer <CWebBrowserSrv> mWebBrowserSrv_sp;
	CMutex2 m_PlaylistAccessLocker;
	QMutex m_SharedDataMutex;
	ActiveMedia_TYPE m_eActiveMediaType;
	QString m_strActiveStorageDirName;
	QSharedPointer <QDBusIf_UiSrv> m_QDbusIfUiSrv_sp;
	QNextFileInfoToPlay m_NextFileInfoToPlay;
	BOOL m_bIsNeedLoop;
	State_TYPE m_iStateType;
#if 1/*add by lxj 2012-7-20 for record last status type*/
	State_TYPE m_iStateTypeLast;
	tMediaType m_StateTypeList[MAX_MEDIATYPE_LIST];
#endif
#if 1/*add by lxj 2012-10-9 for usb video switch*/
	State_TYPE m_iStateTypePrevious;
	QString m_strPlayUrlPrevious;
#endif
	VIDEO_OUTPUT_MODE m_iVideoOutMode;
#if 1 /*Add by xuweiwei 2014-1-14*/
       VIDEO_OUTPUT_MODE m_iOldVideoOutMode;
#endif
#if 1 /*Add by clyde 2016-01-11*/
	BOOL isNeedLoop;
#endif
       BOOL m_FristEntryIsHtml;
	MediaPlay_STATUS m_eMediaPlayCurrStatus;
	bool m_hasPlayedStatrUpItem;
	QSharedPointer <QMediaVirtualChannel> mMediaVirChannel_sp;
	int m_iSnmpTempVirChId;
	QString m_strSnmpTempVirChMedia;
	QString m_strSnmpTempVirChAlias;
	int m_eSnmpTempVirChLabelDispPos;
	QTimer m_SaveMediaVirCh_Timer;
	QTimer m_PlayIdlePlayList_Timer;
	int m_iIdleTime;
	BOOL m_bIsPlayingIdlePlayList;
	int m_CurPlayerVolume;
	BOOL_t m_bVolumeMuted;
	QString m_strDefaultStartupItem;
	CONTENT_TRIGGER_SRC_TYPE m_eContentTrigSrcType;
	AMedia_File_TYPE m_eCurPlayMediaFileType;
	ControlCenter::PLAY_MODE m_eCurPlayMode;
	QSharedPointer <QPlayPic_Html_Win> m_PlayPicAndHtmlWin;
	QTimer m_UploadCfgTimer;
	QString m_strSysCfgUploadUrl;
	quint32 m_NewSysCfg_DownloadId;
	BOOL m_bIsFirstPlayUrl;
	QMap < int, QSharedPointer <QGpioFilePath> > m_GpioFilePath_Map;
	QGpioFilePath m_GpioFilePath;
	bool m_bHasPlayFirst;
	bool m_bIsPlayWin;
	QString m_strIPv4Addr;
#if 1/*added by lxj 2012-81-1 for get host mac address*/
    QString m_strMacAddress;
#endif
	QShowLogo m_ShowlogoWin;
	QTimer m_CheckRebootTimer;
	bool m_bInValidDefaultStatrUpItem;
	bool m_bIsNeedLoopPlaylistBEntry;
	QTimer m_PlayPluginListEntryDurationTimer;
	QTimer m_PlayPluginListHtmlEntryDurationTimer;
	FORMAT_MEDIA_STATUS m_eFormatMediaStatus;
	bool m_bIsPlayPluginList;
	AUDIO_OUTPUT_MODE m_eAudioOutputMode;
	QSharedPointer <QStatusMsgBox> m_StatusMsgBox_sp;
	quint32 m_NewSystem_DownloadId;
	int m_iAutoAnnounceIntervalSec;
	QTimer m_timerAutoAnnounce;
	QSharedPointer <QUdpSocket> m_sckAutoAnnounce_sp;
	QString m_strXappVersion;
	QSharedPointer <QProcess> m_TarUpgradeProcess_sp;
	System_Upgrade_Status m_eCurUpgradeStatus;
	quint32 m_NewSysVerCfg_DownloadId;
	QSharedPointer <QWbStatusBar> m_WbStatusBar_sp;
	QSharedPointer <QWbStatusBarItem> m_UpgradeStatusBarItem_sp;
#if 1/*added by lxj 2012-8-2 for tips message on OSD*/
	QSharedPointer <QWbStatusBarItem> m_MsgStatusBarItem_sp;
#endif
	QString m_strCurrPlayListName;
	QString m_strTempSysUpgradeUrl;
	int m_iEnableLog;
    INT_t PlayPluginListflag ;
	UINT32_t m_uiCurEthIpV4Addr;
	quint32 m_NewOsd_DownloadId;
	QSharedPointer <QProcess> m_TarUpgradeOsdProcess_sp;
	QString m_strCurrPlayListUrl;
	QSharedPointer<QScanProgressBar>  m_pScangressBar;
	QTimer m_OsdTimer;
	TunerScan_Status m_TunerScanStatus;
	QMessageBox Upgrade_MsgBox;
	QSharedPointer<QInputcontentdlg> m_PwdInputDlg_sp;
	QString m_strBootSplashPlayUrl;
	QTimer m_BootSplashTimer;
	bool m_BIsPlaySplash;
	QString m_ChannelNumber;
	QSharedPointer <QInputChannelWin> m_TunerChannelBar_sp;
	int m_iTunerScanStatus;
	QSharedPointer <QWbStatusBarItem> m_TunerStatusBarItem_sp;
	bool m_IsPlayHtmlStatus;
	QPlaylistData m_PlaylistDataB;
#if 1/*added by lxj 2012-12-11*/
	QSharedPointer <QProcess> m_TouchScreenCalibrationProcess_sp;
#endif
#if 1/*added by lxj 2012-12-17*/
	QString m_strRetJavascriptLast;
#endif
	//smp set/get op flags
	bool m_bWaitingForSnmpResponse;
	bool m_bMustWaitForSnmpResponse;
	int m_iCurSnmpOpTransationId;
	QString m_strCurrPluginListName;
	bool m_bIsPlayVideoLayer;
	QString m_strCurrPluginListUrl;
	QString m_strCurrPluginListControlName;
	QString m_StrHddListDir;
	QList <quint32> mEndedPlaybackIdList;
	QGuiOsd2Ctrl m_GuiOsd2Ctrl;

 #if 1  /*Add by xuweiwei 2013-12-23*/
	BOOL_t m_bIsCompletePlaybackEos;
#endif
    QList<QString> m_PlayItemlist;
	bool m_bIsHavePlayItemTemp;
};

#endif	//_WEB_BROWSER_APP_H_

