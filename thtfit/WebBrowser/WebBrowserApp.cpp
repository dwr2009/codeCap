#define USE_QCOPCHANNEL 0

#include "WebBrowserApp.h"
#include <LinuxSignalHandlerEx.h>
#include <BaseErrDef.h>
#include <SignalInfo.h>
#include <QDBusInterface>
#include "QEventType.h"
#include <Compiler.h>
#include "IrKeyQtKeyMap.h"
#include <QDesktopWidget>
#include <GpioBtnInputEventDef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QFileInfo>
#include <QAutoLock.h>
#include <sys/vfs.h>
#include "WbProcessExitCode.h"
#include <MediaPlayerAppDef.h>
#include <DbgLogSwitchDef.h>
#include <SystemSrvApi.h>
#include <DirectoryManage.h>
#include <StackBufString.h>
#include <UrlHandle.h>
#include <GpioBtnInputApi.h>
#include <storageManage.h>
#include <QByteArray>
#include <Base64.h>
#include <NetIF.h>
#include <QCryptographicHash>
#include "CustomAsyncMsg.h"
#include <ArrayOpsMacro.h>
#include <EventLogExt.h>
#include "AudioVideoInOutInfo.h"
#include <AppFilePathManage.h>
#include <LedStatusIndicator.h>
#include <DateTime.h>
#include <sys/time.h>
#include <SnmpCmdConv.h>
#include <QCryptRC4.h>
#if USE_QCOPCHANNEL
#include <QCopChannel>
#include <QTServerApi.h>
#endif
#include <QTouchScreen.h>
#include <UpgradeRebootConfirmDlg.h>
#include <QScopedPointer>
#include <LinuxSignalHandlerEx.h>
#include "DbgLogSwitchDef.h"


#ifdef	SRC_FILE_NAME
#undef	SRC_FILE_NAME
#endif	//SRC_FILE_NAME
#define	SRC_FILE_NAME			"WebBrowserApp.cpp"

using namespace ControlCenter;
using namespace MP7XX; 

#define	QDBUS_CONN_NAME_WB_UI				"QDBusConnection_WB_UI"
QDBusIf_UiSrv::QDBusIf_UiSrv(QWebBrowserApp * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pRealObj = pParentObj;
}

void QDBusIf_UiSrv::setParentObj(QWebBrowserApp * pParentObj)
{
	m_pRealObj = pParentObj;
}  

void QDBusIf_UiSrv::setPlayMode(int PlayMode, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayMode(PlayMode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::startPlaylist(QString strPlaylist, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->startPlaylist(strPlaylist, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::playMedia(QString strFilePath,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->parsePlayMediaCommad(strFilePath,iOutRet,PlayMedia_MODE);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::playNextFile(QString strFilePath,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setItemTemporary(strFilePath);
		iOutRet = 0;
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getCurrentPlayItem(QString & strFileName,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->getCurrentPlayItem(strFileName,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPlayVolume(int iVolumeValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayVolume(iVolumeValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPlayerMute(int iMuted,int &iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayerMute(iMuted,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getPlayerMute(int & iOutRet, int & bMuted)
{
	if(m_pRealObj)
	{
		m_pRealObj->getPlayerMute(iOutRet, bMuted);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getPlayVolume(int & iOutRet,int &iVolumeValue)
{
	if(m_pRealObj)
	{
		m_pRealObj->getCurrentVolume(iVolumeValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getPlayState(int &iOutRet,int & iState)
{
	if(m_pRealObj)
	{
		m_pRealObj->getPlayState(iOutRet,iState);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setDefaultIdlePlaylist(QString strIdlePlaylist, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setDefaultIdlePlaylist(strIdlePlaylist,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setIdleDetect(int iIdleSeconds,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setIdleDetect(iIdleSeconds,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVideoPosition(int iPosType,int iPosValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVideoPosition(iPosType,iPosValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void  QDBusIf_UiSrv::playBrowser(QString strFilePath,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->parsePlayMediaCommad(strFilePath,iOutRet,PlayMedia_MODE);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setDisplayParam(int eDispParamType, int iValue,int bSave,int &iOutRet )
{
	if(m_pRealObj)
	{
		m_pRealObj->setDisplayParam(eDispParamType,iValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getDisplayParam(int eDispParamType,int & iCurrValue,int &iRet )
{
	if(m_pRealObj)
	{
		iRet = m_pRealObj->getDisplayParam(eDispParamType,iCurrValue);
	}
	else
	{
		iRet = ERROR_INVALID_STATE;
	}
}

void  QDBusIf_UiSrv::setCoordinateMode(int iMode,int & iRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setCoordinateMode(iMode,iRet);
	}
	else
	{
		iRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setShitfPosition(int eShitfType,int iShiftValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setShitfPosition(eShitfType,iShiftValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setFlippingInterval(int iIntervalVaule,int &iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setFlippingInterval(iIntervalVaule,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getFlippingInterval(int &iOutRet,int  & iIntervalVaule)
{
	if(m_pRealObj)
	{
		m_pRealObj->getFlippingInterval(iIntervalVaule,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPlayVirtualChannel(int iChannelId,int &iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayVirtualChannel(iChannelId,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPlayInternalImage(int iIntervalImageId,int &iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayInternalImage(iIntervalImageId,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setDefaultStartupItem(const QString & strFileInfo,int & iOutRet)
{
	if(m_pRealObj)
 	{
		m_pRealObj->setDefaultStartupItem(strFileInfo,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirtualChannelId(int VirChid, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirtualChannelId(VirChid, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirtualChannelId(int & iOutRet, int & VirChid)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirtualChannelId(iOutRet, VirChid);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChMedia(QString strVirChMedia, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChMedia(strVirChMedia, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChMedia(int & iOutRet, QString & strVirChMedia)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChMedia(iOutRet, strVirChMedia);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirtualChannelAlias(QString strVirChAlias, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirtualChannelAlias(strVirChAlias, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirtualChannelAlias(int & iOutRet, QString & strVirChAlias)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirtualChannelAlias(iOutRet, strVirChAlias);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChLabelDispPos(int DispPos,  int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChLabelDispPos(DispPos, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChLabelDispPos(int & iOutRet, int & DispPos)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChLabelDispPos(iOutRet, DispPos);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChEnable(int bEnable, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChEnable(bEnable, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChEnable(int & iOutRet, int & bEnable)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChEnable(iOutRet, bEnable);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChDispDuration(int DispDuration, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChDispDuration(DispDuration, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChDispDuration(int & iOutRet, int & DispDuration)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChDispDuration(iOutRet, DispDuration);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChMin(int MinVal, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChMin(MinVal, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChMin(int & iOutRet, int & MinVal)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChMin(iOutRet, MinVal);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setVirChMax(int MaxVal, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVirChMax(MaxVal, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getVirChMax(int & iOutRet, int & MaxVal)
{
	if(m_pRealObj)
	{
		m_pRealObj->getVirChMax(iOutRet, MaxVal);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setSpeedControl(int iSpeedValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setSpeedControl(iSpeedValue, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setHorizontalSize(int iSize,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setShitfPosition(AMedia_POSTION_ShiftXSIZE,iSize,iOutRet);
	}
	else  
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getHorizontalSize(int & iOutRet, int & iSize)
{
	if(m_pRealObj)
	{
		m_pRealObj->getShitfPosition(AMedia_POSTION_ShiftXSIZE,iOutRet,iSize);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setHorizontalShift(int iSize,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setShitfPosition(AMedia_POSTION_ShiftX,iSize,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getHorizontalShift(int & iOutRet, int & iSize)
{
	if(m_pRealObj)
	{
		m_pRealObj->getShitfPosition(AMedia_POSTION_ShiftX,iSize,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}

}

void QDBusIf_UiSrv::setVerticalSize(int iSize,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->getShitfPosition(AMedia_POSTION_ShiftYSIZE,iSize,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getShitfPosition(int eShitfType,int &  iShiftValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->getShitfPosition(eShitfType,iShiftValue,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getSpeedControl (int & iOutRet,int & iSpeedValue)
{
	if(m_pRealObj)
	{
		m_pRealObj->getSpeedControl(iOutRet,iSpeedValue);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getPlayMode(int & iOutRet, int & iCurrMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getPlayMode(iOutRet, iCurrMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getDefaultIdlePlaylist( int & iOutRet,QString &strIdlePlaylist)
{
	if(m_pRealObj)
	{
		m_pRealObj->getDefaultIdlePlaylist(iOutRet, strIdlePlaylist);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getPlayInternalImage(int &iOutRet,int & iIntervalImageId)
{
	if(m_pRealObj)
	{
		m_pRealObj->getPlayInternalImage(iOutRet, iIntervalImageId);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::getplayPlaylist(int &iOutRet,QString & strPlaylist)
{
	if(m_pRealObj)
	{
		m_pRealObj->getplayPlaylist(iOutRet, strPlaylist);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getDefaultStartupItem(int & iOutRet,QString & strItemInfo)
{
	if(m_pRealObj)
	{
		m_pRealObj->getDefaultStartupItem(iOutRet, strItemInfo);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setrebootSystem(int iId, int & iOutRet)
{
	Q_UNUSED(iId);

 	iOutRet = ERROR_SUCCESS;

	if(m_pRealObj)
	{
		m_pRealObj->SystemReboot();
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getIdleDetect(int & iOutRet,int & iIdleSeconds)
{

	if(m_pRealObj)
	{
		m_pRealObj->getIdleDetect(iOutRet, iIdleSeconds);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPullConfig(QString strUrl, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPullConfig(strUrl, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPushConfig(QString strUrl, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPushConfig(strUrl, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setAutoReboot(int iEnabled,int &iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setAutoReboot(iEnabled, iOutRet);
 	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setAutoRebootInterval(int iTime,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setAutoRebootInterval(iTime, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}

}

void QDBusIf_UiSrv::getAutoRebootInterval(int & iOutRet,int &iTime)
{
	if(m_pRealObj)
	{
		m_pRealObj->getAutoRebootInterval(iOutRet, iTime);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setTrapEnable(int iEnable,int & iOutRet)
{
       if(m_pRealObj)
       {
               m_pRealObj->setTrapEnable(iEnable, iOutRet);
       }
       else
       {
               iOutRet = ERROR_INVALID_STATE;
       }
}

void QDBusIf_UiSrv::getTrapEnable(int & iOutRet,int &iEnable)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTrapEnable(iOutRet, iEnable);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getCoordinateMode(int & iOutRet,int &iMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getCoordinateMode(iOutRet, iMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}

}

void QDBusIf_UiSrv::getAutoReboot(int & iOutRet,int &iEnable)
{
	if(m_pRealObj)
	{
		m_pRealObj->getAutoReboot(iOutRet, iEnable);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setAlphaLayer(int iAlphaValue,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setAlphaLayer(iAlphaValue, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setActiveLayer(int iLayerId,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setActiveLayer(iLayerId, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getCardCurrentStatus(int & iOutRet,QString &strCardCurStatus)
{
	if(m_pRealObj)
	{
		m_pRealObj->getCardCurrentStatus(iOutRet,strCardCurStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setFormatMedia(int iAction, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setFormatMedia(iAction, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getFormatMedia(int & iOutRet, int & DummyValue)
{
	iOutRet = ERROR_SUCCESS;
	DummyValue = 0;
}

void QDBusIf_UiSrv::getAudioOutputMode(int & iOutRet, int & iAudioOutputMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getAudioOutputMode(iOutRet, iAudioOutputMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getMediaStatus(int & iOutRet,QString &strMediaStatus)
{
	if(m_pRealObj)
	{
		m_pRealObj->getMediaStatus(iOutRet, strMediaStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}

}
void QDBusIf_UiSrv::setAudioOutputMode(int iAudioOutputMode, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setAudioOutputMode(iAudioOutputMode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setAutoAnnounce(int iAutoAnnounceIntervalSec, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setAutoAnnounce(iAutoAnnounceIntervalSec, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getAutoAnnounce(int & iOutRet, int & iAutoAnnounceIntervalSec)
{
	if(m_pRealObj)
	{
		m_pRealObj->getAutoAnnounce(iOutRet, iAutoAnnounceIntervalSec);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getXappVersion(int & iOutRet, QString & strXappVersion)
{
	if(m_pRealObj)
	{
		m_pRealObj->getXappVersion(iOutRet, strXappVersion);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setSystemUpgradeAddress(const QString & strUrl,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setSystemUpgradeAddress(strUrl,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::setMaintainLog(int eCurrActiveMedia,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setMaintainLog(eCurrActiveMedia,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::setTransferLog(QString strLogUrl,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setTransferLog(strLogUrl,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::setUserOSDAddress (const QString & strUrl,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setUserOSDAddress (strUrl,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setPlayTuner(QString strTunerUrl,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setPlayTuner (strTunerUrl,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getTunerChannelStrength(int & iOutRet, int & iStrength)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTunerChannelStrength (iOutRet,iStrength);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::setTunerScan (int iAction,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setTunerScan (iAction,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setTunerMode(int iSetMode,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setTunerMode (iSetMode,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setChannelUp(int iAction,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setChannelUp (iAction,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setChannelDown(int iAction,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setChannelDown (iAction,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setResetDuration (int iDurationTime,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setResetDuration (iDurationTime,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getResetDuration(int & iOutRet, int & iDurationTime)
{
	if(m_pRealObj)
	{
		m_pRealObj->getResetDuration (iOutRet,iDurationTime);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getTransferControl(int & iOutRet, int & TransferStatusCode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTransferControl (iOutRet, OUT TransferStatusCode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

#if 1/*added by lxj 2012-7-31 for features enabled setting from snmp*/
void QDBusIf_UiSrv::setDeviceKey(QString strFromEnabledFeatures,int  & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setDeviceKey(strFromEnabledFeatures, OUT iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getEnabledFeatures( int & iOutRet,  int & iEnabledFeatures)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->m_Mp7xxGlobalStatusData.getEnabledFeatures(iEnabledFeatures);
		if(iOutRet != ERROR_SUCCESS )
		{
			iOutRet = ERROR_INVALID_STATE;
		}
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
	//LOG_BLINE("EnabledFeatures=%d(0x%X)\n",iEnabledFeatures,iEnabledFeatures);
}
#endif

#if 1/*added by lxj 2012-12-11 for touch screen calibrate from SNMP command*/
void QDBusIf_UiSrv::SetTouchScreenCalibrateAsync(int iAction, int & iOutRet)
{
	//LOG_BLINE("%s()\n",__func__);	
	if(m_pRealObj){
		if(iAction){
			iOutRet = m_pRealObj->closeTouchScreenCalibrate();
		}else{
			iOutRet = m_pRealObj->setTouchScreenCalibrateAsync();
		}
	}else{
		iOutRet = ERROR_INVALID_STATE;
	}
}
#endif

#if 1/*added by lxj 2012-12-17 for overlay by javascript dynamic run*/
void QDBusIf_UiSrv::getHtmlEvalJavascript( int & iOutRet, QString & strJavascript )
{
	//LOG_BLINE("%s()\n",__func__);	
	if(m_pRealObj){
		m_pRealObj->getHtmlEvalJavascript(OUT iOutRet,OUT strJavascript);
	}else{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::setHtmlEvalJavascript( QString strJavascript, int & iOutRet )
{
	//LOG_BLINE("%s()\n",__func__);	
	if(m_pRealObj){
		m_pRealObj->setHtmlEvalJavascript(strJavascript,OUT iOutRet);
	}else{
		iOutRet = ERROR_INVALID_STATE;
	}

}
#endif

void QDBusIf_UiSrv::getPlayTuner(int & iOutRet, QString & strTunerStatus)
{
	iOutRet = ERROR_SUCCESS;
	if(m_pRealObj)
	{
		m_pRealObj->getPlayTuner (iOutRet, strTunerStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_UiSrv::getTunerScan(int & iOutRet, int & iScanStatus)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTunerScan (iOutRet, OUT iScanStatus);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::getForceTxMode(int & iOutRet, int & iTxMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getForceTxMode (iOutRet, OUT iTxMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}


void QDBusIf_UiSrv::getSubChannelCount(int & iOutRet, int & iCount)
{
	if(m_pRealObj)
	{
		m_pRealObj->getForceTxMode (iOutRet, OUT iCount);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}


void QDBusIf_UiSrv::getChannelTable( int & iOutRet, QStringList  & strInfoList)
{
	if(m_pRealObj)
	{
		m_pRealObj->getChannelTable (iOutRet,strInfoList);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_UiSrv::get_tnetVersion(int & iOutRet, qint32 & iNumber)
{
	if(m_pRealObj)
	{
		m_pRealObj->get_tnetVersion (iOutRet, OUT iNumber);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_UiSrv::MPlayerPlugin_Play(QString strPlayListUrl,int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->MPlayerPlugin_Play(strPlayListUrl);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}		
}


void QDBusIf_UiSrv::setBootSplashPlayUrl(QString strBootSplashUrl, int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setBootSplashPlayUrl(strBootSplashUrl,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}		
}

void QDBusIf_UiSrv::getBootSplashPlayUrl(int & iOutRet, QString & strBootSplashUrl)
{
	if(m_pRealObj)
	{
		m_pRealObj->getBootSplashPlayUrl(iOutRet,strBootSplashUrl);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_UiSrv::getTunerMode(int & iOutRet, int & iCurrTunerMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTunerMode(iOutRet,iCurrTunerMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_UiSrv::setVideoLayerPlayMedia(const QString & strUrl, int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet= m_pRealObj->setVideoLayerPlayMedia(strUrl);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}


void QDBusIf_UiSrv::setTunerChannelMode(int iTunerChMode, int & iOutRet)
{
 	if(m_pRealObj)
	{
		m_pRealObj->setTunerChannelMode(iTunerChMode,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}		
}

void QDBusIf_UiSrv::getTunerChannelMode(int & iOutRet, int & iTunerChMode)
{
	if(m_pRealObj)
	{
		m_pRealObj->getTunerChannelMode(iOutRet,iTunerChMode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void  QDBusIf_UiSrv::setVideoLayerPlayMode(int iVideoLayerMode,int & iOutRet)
{
	if(m_pRealObj)
	{
		m_pRealObj->setVideoLayerPlayMode(iVideoLayerMode,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}


void QDBusIf_UiSrv::setHtmlLayerPlay(const QString & strUrl, int & iOutRet)
{

	if(m_pRealObj)
	{
		iOutRet= m_pRealObj->setHtmlLayerPlay(strUrl);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_UiSrv::setPlayNext(int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet= m_pRealObj->setPlayNext();
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

/*
The implementation of class QWebBrowserApp
*/
int QWebBrowserApp::m_SwitchableVidOutMode_Table[] =
{
	MediaPlayer::VO_MODE_HDMI_CAV_720p60,	//default
	MediaPlayer::VO_MODE_HDMI_720p50,
	MediaPlayer::VO_MODE_HDMI_1080i59,
	MediaPlayer::VO_MODE_HDMI_CAV_1080i60,
	MediaPlayer::VO_MODE_HDMI_1080i50,
	MediaPlayer::VO_MODE_HDMI_1080p59,
	MediaPlayer::VO_MODE_HDMI_CAV_1080p60,
	MediaPlayer::VO_MODE_HDMI_1080p50,
	MediaPlayer::VO_MODE_HDMI_2160p24,
	MediaPlayer::VO_MODE_HDMI_2160p25,
	MediaPlayer::VO_MODE_HDMI_2160p30,
	MediaPlayer::VO_MODE_HDMI_2160p50,
	MediaPlayer::VO_MODE_HDMI_2160p60,
};

QWebBrowserApp::QWebBrowserApp(int argc, char *argv[]) :
	QControlCenterApp(argc, argv),
	m_SharedDataMutex(QMutex::Recursive)
{
	int iRet;
	m_CalcVideoPosTryTimes = 0;
	m_PlaybackId = 0;
	m_bShouldQuitApp = FALSE;
	isNeedLoop = FALSE;
	m_GpioBtnInputFd = INVALID_FILE_DESCRIPTOR;
	mb_IsFullSrceen = FALSE;
	m_eActiveMediaType = AMedia_TYPE_FLASH;
	m_strActiveStorageDirName = LOCAL_STORAGE_DIR_NAME;
	m_iStateType = State_Idle;
#if 1/*add by lxj 2012-7-20 init*/
	m_iStateTypeLast = m_iStateType;
	memset(&m_StateTypeList,0,sizeof(m_StateTypeList));
#endif
#if 1/*add by lxj 2012-10-9*/
	m_iStateTypePrevious = m_iStateType;
	m_strPlayUrlPrevious = "";
#endif
#if 1/**/
	m_strRetJavascriptLast = "";
#endif
	m_bIsNeedLoop = FALSE;
	m_iVideoOutMode = VO_MODE_NotSet;
#if 1 /*Add by xuweiwei 2014-1-14*/
       m_iOldVideoOutMode = VO_MODE_NotSet;
#endif
	m_FristEntryIsHtml = FALSE ;
	m_eMediaPlayCurrStatus = MediaPlay_Status_NullPlayStatus;
	m_hasPlayedStatrUpItem = FALSE;
	m_iSnmpTempVirChId = -1;
	m_iIdleTime = 0;
	m_bIsPlayingIdlePlayList = FALSE;
	m_CurPlayerVolume = -1;
	m_bVolumeMuted = FALSE;
	m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_DefaultPlaylist;
	m_eCurPlayMediaFileType = AMedia_TYPE_VIDEO;
	m_eCurPlayMode = ControlCenter::PlayMode_Normal;
	m_NewSysCfg_DownloadId = 0;
	m_bIsFirstPlayUrl = true;
	m_bHasPlayFirst = true;
	m_bIsPlayWin = true;
	m_bInValidDefaultStatrUpItem = false;
	m_bIsNeedLoopPlaylistBEntry = FALSE;
	m_eFormatMediaStatus  = FORMAT_MEDIA_UnKnow;
	m_bIsPlayPluginList = false ;
	m_eAudioOutputMode = AUD_OUT_MODE_Invalid;
	m_NewSystem_DownloadId = 0;
	m_NewSysVerCfg_DownloadId = 0;
	PlayPluginListflag = 0;
	m_iAutoAnnounceIntervalSec = 0;	//disabled
	m_strXappVersion ="1.0.0";
	m_eCurUpgradeStatus = SystemUpgrade_Idle;
	m_MainThreadId = QThread::currentThreadId ();
	m_strCurrPlayListName = "";
	m_strTempSysUpgradeUrl ="";
	m_uiCurEthIpV4Addr = 0;
	m_iEnableLog = -1;
	m_NewOsd_DownloadId = 0;
	m_strCurrPlayListUrl = "";
	m_TunerScanStatus =TunerStauts_Idle;
	m_BIsPlaySplash = false;
	m_ChannelNumber = "";
	m_iTunerScanStatus = TUNER_NO_SCAN;
	m_IsPlayHtmlStatus = false;
	m_bWaitingForSnmpResponse = false;
	m_bMustWaitForSnmpResponse = false;
	m_iCurSnmpOpTransationId = 0;	//invalid
	m_bIsPlayVideoLayer = false; 
	m_strCurrPluginListName ="";
	m_strCurrPluginListUrl = "";
	m_bIsHavePlayItemTemp = false;
#if 1 /*Add by xuweiwei 2014-2-21*/
	m_bIsCompletePlaybackEos=FALSE;
#endif
	iRet = RegDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
 // m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
	qRegisterMetaType<MessageItem_SharedPointer_t>("MessageItem_SharedPointer_t");
   
}

QWebBrowserApp::~QWebBrowserApp()
{
	INT_t iRet;

	CloseGpioBtnInputFile();
    
    
	iRet = UnregDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
    
}

VOID QWebBrowserApp::SigHandler(VOID * pCbContext, int SigNo)
{
	QWebBrowserApp * pApp = (typeof(pApp))pCbContext;
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		if(pApp)
		{
			pApp->OnLinuxSignal(SigNo);
		}
	}
}

VOID QWebBrowserApp::OnLinuxSignal(int SigNo)
{
	LOG("(%d)Signal:%s\n", getpid(), CSignalInfo::GetSingalDescFromSigNo(SigNo));
	do
	{
		if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
		{
			m_bShouldQuitApp = TRUE;
			//Wakup message
			QEvent * pEvent = new QEvent((enum QEvent::Type)QEVENT_TYPE_Wakeup);
			if(NULL == pEvent)
			{
				break;
			}
			postEvent(this, pEvent);
			//Quit of app
			quit();
		}
	}while(FALSE);
}

/*
Load the default playlist
*/
bool QWebBrowserApp::LoadPlaylist(QString strPlaylistUrl)
{
	bool bOutRet = true;
	INT_t iRet;
	do
	{
		QString playListUrl = strPlaylistUrl;
		if(strPlaylistUrl.trimmed()=="")
		{
			playListUrl = m_strDefaultStartupItem;//mp_Config ->getConfigValue(INI_KEY_DefaultStartupItem);
		}
		if(playListUrl.trimmed()==""  || !QFile::exists(playListUrl))
		{
			QFileInfo playListInfo(playListUrl);
			QString strFileName = playListInfo.fileName();
			playListUrl = applicationDirPath() + "/" + m_strActiveStorageDirName +"/" + FOLDER_NAME_PLAYLIST +"/"+ strFileName;
		}
		QString strUrlType = "";
		if(!QFile::exists(playListUrl))
		{
			playListUrl = ParsePlayFilePathToPlayUrl(strPlaylistUrl,strUrlType);
			if(strPlaylistUrl.trimmed()=="")
			{
				return false;
			}
		}
		iRet = m_PlaylistDataA.LoadFromIniFile(&playListUrl);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			bOutRet = false;
			break;
		}
		else
		{
			m_iStateType = State_PlayingPlayList ;
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlayPlayList;
			QFileInfo fileInfo(playListUrl);
			m_strCurrPlayListUrl = playListUrl;
			m_strCurrPlayListName = fileInfo.fileName();
		}
	}while(FALSE);

	return bOutRet;
}

INT_t QWebBrowserApp::PlayUrl(CONST QString & strUrl,qint32 iLoopTimes)
{
	INT_t iRet= ERROR_SUCCESS;
	unsigned int PlaybackId = 0;

	//LOG_BLINE("strUrl=%s,iLoopTimes=%d,m_iStateType=%d\n",qPrintable(strUrl),iLoopTimes,m_iStateType);

	do
	{
		if(strUrl.trimmed() == "")
		{
			iRet = ERROR_INVALID_PARAMETER;
			break;
		}
		else
		{
			isNeedLoop = TRUE;/*This variable and the configuration file in the "ENTRY = LOOP"
			                            common to decide whether it is necessary to loop playback
			                            (Avoid dead circulation, take up too much CPU resources)*/
		}
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		
		if(!m_bIsPlayPluginList && !m_bIsPlayVideoLayer)
		{
			StopPlayDurationTimer();
		}
		iRet = stopMPlayerWithEventLog();
		if(ERROR_SUCCESS != iRet)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}

		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		 if(m_PlayPicAndHtmlWin->isPlayingHtml())
		{
			if(!m_bIsPlayPluginList && !m_bIsPlayVideoLayer)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
			}
		}

		iRet = m_MediaPlayerCtrl_sp->Play(OUT strUrl, OUT PlaybackId,iLoopTimes);
		if(ERROR_SUCCESS == iRet)	//success, break the loop
		{
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				pActiveWindow = activeWindow();
				if(&m_Browser == pActiveWindow)
				{	
					iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
				}
				else
				{
					iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Normal);
				}
			}
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY, 1);
			if(m_bIsFirstPlayUrl)
			{
				m_bIsFirstPlayUrl = false;
			}
			m_strCurPlayUrl = strUrl;
			m_PlaybackId = PlaybackId;
			m_eCurPlayMode = ControlCenter::PlayMode_Normal;
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY, 1);
			BOOL_t bEnableWriteLoggin = TRUE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			if(bEnableWriteLoggin)
			{
				char strTemp[64];
				snprintf(strTemp, sizeof(strTemp), "%d", TYPE_LOG_StartPlay);
				strTemp[sizeof(strTemp)-1] = '\0';
				CString::STD_STRING_LIST strList ;
				strList.push_back(strTemp);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				sprintf(strTemp,"%d",m_iStateType);
				QString strPlayType,strPlayFileInfo;
				getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
				if(m_strCurrPlayListName.trimmed() == "")
				{
					strList.push_back(strPlayType.toLocal8Bit().constData());
					strList.push_back(strUrl.toLocal8Bit().constData());
				}
				else
				{
					strList.push_back("PlayList");
					strList.push_back(m_strCurrPlayListUrl.toLocal8Bit().constData());
					QFileInfo fileInfo(strUrl);
					strList.push_back(fileInfo.fileName().toLocal8Bit().constData());
				}
				strList.push_back(m_PlaybackId);
				if(ERROR_SUCCESS != WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				else
				{
					strList.clear();
				}
			}
			break;
		}
		else if(ERROR_SUCCESS != iRet)	//any other errors
		{
			break;
		}
	}while(FALSE);
	
	if(ERROR_SUCCESS == iRet)
	{
		m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY, 1);
#if 1/*add by lxj for m_iStateTypeLast2 init*/
		if( strUrl.trimmed() != "" &&
			PlaybackId != 0 &&
			m_iStateType != State_Idle &&
			m_iStateType != State_PlayingBrowserFile)
		{
			/*overlap memory copy*/
			memmove(&m_StateTypeList[1],&m_StateTypeList[0],sizeof(m_StateTypeList)-sizeof(tMediaType));
			m_StateTypeList[0].PlaybackId = PlaybackId;
			m_StateTypeList[0].iStateType = m_iStateType;
		}
		//LOG_BLINE("m_iStateType=%d,PlaybackId=%d\n",m_iStateType,PlaybackId);
#endif
	}

	return iRet;
}

INT_t QWebBrowserApp::StopPlayback()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		//summent up for fix html close when the playlist have a html
		//StopPlayDurationTimer();		
		iRet = stopMPlayerWithEventLog();
		if(ERR_NO_QDBUS_CONN == iRet)
		{
			OnTimer_ConnectDBus();
			if(m_DBusConn_sp->isConnected())
			{
				iRet = stopMPlayerWithEventLog();
				if(ERROR_SUCCESS != iRet)
				{ 
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		else if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
		if(ERROR_SUCCESS == iRet)
		{
#if 1/*add by lxj 2012-7-20 for record last status type*/
			m_iStateTypeLast = m_iStateType;
#endif
			if(m_iStateType != State_PlayingTuner)
			{
				m_bIsPlayPluginList = false ;
			}
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
			m_iStateType = State_Idle;
			m_eCurPlayMode = ControlCenter::PlayMode_StopAll;
			
			startIdelDetectTimer();
		}
	}while(FALSE);
	return iOutRet;
}

VOID QWebBrowserApp::PlayNextEntry(bool bFirstEntry/* = false*/,bool bPreEntry/* = false */)
{
	INT_t iRet;
	QSharedPointer <QPlaylistEntry> PlayEntry_sp;
	PlayPluginListflag=0;
	if(bPreEntry)
	{
		
		PlayEntry_sp = m_PlaylistDataA.MoveToPreHeadPlayEntry();
	}
	else
	{
		if(bFirstEntry)
		{
			PlayEntry_sp = m_PlaylistDataA.MoveToFirstPlayEntry();
		}
		else
		{
			if(!m_bIsNeedLoop)
			{
				PlayEntry_sp = m_PlaylistDataA.MoveToNextPlayEntry();
			}
			else
			{
				PlayEntry_sp = m_PlaylistDataA.MoveToNextPlayEntry(FALSE);
			}
		}
	}

	do
	{		
		if(PlayEntry_sp.isNull())	//end of list
		{
			if(Sw_LogDbusCmd)
			{
				LOG_BLINE("No entry to play\n");
			}
			m_iStateType = State_Idle;
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				break;
			}
			StopPlayDurationTimer();
			iRet = stopMPlayerWithEventLog();
			
			if(m_PlayIdlePlayList_Timer.isActive())
			{
				m_PlayIdlePlayList_Timer.stop();
			}			
			if(m_iStateType == State_Idle)
			{
				if(m_TunerScanStatus == TunerScanning)
				{					
					break;
				}
				startIdelDetectTimer();
			}
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();

			if(&m_playWin == pActiveWindow  || ( pActiveWindow == NULL ))//&& m_FristEntryIsHtml ))
			{	
				//ShowBrowserWindow();
			}		
			m_strCurrPlayListName = "" ;
			m_strCurrPlayListUrl = "";
			break;
		}

		if(Sw_LogDbusCmd)
		{
			LOG_BLINE("PlayNext,ActionType=%d\n", PlayEntry_sp->getActionType());
		}
		m_eMediaPlayCurrStatus = MediaPlay_Status_PlayPlayList;
		if(QPlaylistEntry::ACTION_Video == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Video> PlayEntry_Video_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Video>();
			if(PlayEntry_Video_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Video_sp->getActionType(),
				PlayEntry_Video_sp->m_strFilePath);
			m_iStateType = State_PlayingVideoFile;
			if(m_bIsPlayVideoLayer)
			{
				StopPlayPluginList();
				m_bIsPlayVideoLayer = false;
				m_bIsPlayPluginList = false;
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->lower();
			}
			if(PlayEntry_Video_sp->m_iDuration == DEFAULT_PLAY_DURATION_TIME
				&& PlayEntry_Video_sp->m_iLoopTimes > 1 )
			{
				iRet = PlayUrl(strActualFilePath,PlayEntry_Video_sp->m_iLoopTimes);			
			}
			else
			{
				iRet = PlayUrl(strActualFilePath);
				PlayEntry_Video_sp->m_iHasLoopTimes ++;
				if( PlayEntry_Video_sp->m_iHasLoopTimes  >=  PlayEntry_Video_sp->m_iLoopTimes )
				{
					PlayEntry_Video_sp->m_iHasLoopTimes =0;
					m_bIsNeedLoop = false ;
				}
				else
				{
					m_bIsNeedLoop = true ;
				}
				if(PlayEntry_Video_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
				{
					int iTimerTimeoutMs = (PlayEntry_Video_sp->m_iDuration * 1000);
					if(0 < iTimerTimeoutMs)
					{
						m_PlaylistDurationTimer.start(iTimerTimeoutMs);
					}
				}
			}
			if(iRet != ERROR_SUCCESS)
			{
				PlayNextEntry();
				break;
			}
			else
			{				
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
				{
					setPlayListActionPosition(PlayEntry_Video_sp->m_xLeft,PlayEntry_Video_sp->m_yUpper,
					PlayEntry_Video_sp->m_xSize,PlayEntry_Video_sp->m_ySize);
				}
				else if(&m_Browser== pActiveWindow)
				{
					UpdateVideoPosition();
				}
				else if(pActiveWindow == NULL)
				{
					if(m_bIsPlayWin)
					{
						setPlayListActionPosition(PlayEntry_Video_sp->m_xLeft,PlayEntry_Video_sp->m_yUpper,
						PlayEntry_Video_sp->m_xSize,PlayEntry_Video_sp->m_ySize);
					}
					else
					{
						UpdateVideoPosition();
					}
				}
				else
				{
					if(m_bIsPlayWin)
					{
						setPlayListActionPosition(PlayEntry_Video_sp->m_xLeft,PlayEntry_Video_sp->m_yUpper,
						PlayEntry_Video_sp->m_xSize,PlayEntry_Video_sp->m_ySize);
					}
					else
					{
						//UpdateVideoPosition();
					}
				}
				m_iStateType = State_PlayingVideoFile;
				break;
			}			
			m_iStateType = State_PlayingVideoFile;
			if(!m_bIsPlayPluginList)
			{
				m_IsPlayHtmlStatus = false;
			}
		}
		else if(QPlaylistEntry::ACTION_Picture == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Picture> PlayEntry_Picture_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Picture>();
			if(PlayEntry_Picture_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Picture_sp->getActionType(),
				PlayEntry_Picture_sp->m_strFilePath);
			m_iStateType = State_PlayingPictureFile;
			if(m_bIsPlayVideoLayer)
			{
				StopPlayPluginList();
				m_bIsPlayVideoLayer = false;
				m_bIsPlayPluginList = false;
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->lower();
			}
			iRet = PlayUrl(strActualFilePath);
			if(iRet != ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoop = false ;
#endif
				PlayNextEntry();
				break;
			}			

			 if(ERROR_SUCCESS ==iRet)
			 {
				if(PlayEntry_Picture_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
				{
					int iTimerTimeoutMs = (PlayEntry_Picture_sp->m_iDuration * 1000);
					if(0 == iTimerTimeoutMs)
					{
						iTimerTimeoutMs = 100;
					}
					if(0 < iTimerTimeoutMs)
					{
						if(m_PlaylistDurationTimer.isActive())
						{
							m_PlaylistDurationTimer.stop();
						}
						m_PlaylistDurationTimer.start(iTimerTimeoutMs);
					}
				}
				else
				{
					if(m_PlaylistDurationTimer.isActive())
					{
						m_PlaylistDurationTimer.stop();
					}
					m_PlaylistDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
				}
			 }
			 else
			 {
			 	StopPlayDurationTimer();
			 }
			 QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(&m_playWin == pActiveWindow)//  || (m_bHasPlayFirst  && pActiveWindow == NULL ))//&& m_FristEntryIsHtml ))
			{					
				setPlayListActionPosition(PlayEntry_Picture_sp->m_xLeft,PlayEntry_Picture_sp->m_yUpper,
				PlayEntry_Picture_sp->m_xSize,PlayEntry_Picture_sp->m_ySize);
			}
			else if(&m_Browser== pActiveWindow)
			{
				UpdateVideoPosition();
			}
			else if(pActiveWindow == NULL)
			{
				if(m_bIsPlayWin)
				{
					setPlayListActionPosition(PlayEntry_Picture_sp->m_xLeft,PlayEntry_Picture_sp->m_yUpper,
					PlayEntry_Picture_sp->m_xSize,PlayEntry_Picture_sp->m_ySize);
				}
				else
				{
					UpdateVideoPosition();
				}
			}
			else
			{
				if(m_bIsPlayWin)
				{
					setPlayListActionPosition(PlayEntry_Picture_sp->m_xLeft,PlayEntry_Picture_sp->m_yUpper,
					PlayEntry_Picture_sp->m_xSize,PlayEntry_Picture_sp->m_ySize);
				}
				else
				{
					UpdateVideoPosition();
				}
			}
			m_iStateType = State_PlayingPictureFile;
			if(!m_bIsPlayPluginList)
			{
				m_IsPlayHtmlStatus = false;
			}
		}
		else if(QPlaylistEntry::ACTION_Audio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Audio> PlayEntry_Audio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Audio>();
			if(PlayEntry_Audio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Audio_sp->getActionType(),
				PlayEntry_Audio_sp->m_strFilePath);
			StopPlayDurationTimer();
			m_iStateType = State_PlayingAudioFile;
			iRet = PlayUrl(strActualFilePath);
			if(iRet != ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoop = false ;
#endif
				PlayNextEntry();
				break;
			}
			else
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY, 1);
			}
			if(PlayEntry_Audio_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerTimeoutMs = (PlayEntry_Audio_sp->m_iDuration * 1000);
				if(0 == iTimerTimeoutMs)
				{
					iTimerTimeoutMs = 100;
				}
				if(0 < iTimerTimeoutMs)
				{
					if(m_PlaylistDurationTimer.isActive())
					{
						m_PlaylistDurationTimer.stop();
					}
					m_PlaylistDurationTimer.start(iTimerTimeoutMs);
				}
			}
			 m_iStateType = State_PlayingAudioFile;
			 if(!m_bIsPlayPluginList)
			{
				m_IsPlayHtmlStatus = false;
			}
		}
		else if(QPlaylistEntry::ACTION_Html== PlayEntry_sp->getActionType() ||
			    QPlaylistEntry::ACTION_Browser== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Html> PlayEntry_Html_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Html>();
			if(PlayEntry_Html_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Html_sp->getActionType(),
				PlayEntry_Html_sp->m_strFilePath);
			m_FristEntryIsHtml = true;
			if(strActualFilePath == "")
			{
				strActualFilePath = PlayEntry_Html_sp->m_strFilePath;
			}
            
			iRet = CallPlayWebpage(strActualFilePath,PlayEntry_Html_sp->m_iDuration);
			if(iRet != ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoop = false ;
#endif
				PlayNextEntry();
				break;
			}
			if(!m_bIsPlayVideoLayer)
			{
				 m_iStateType = State_PlayingBrowserFile;		
			}
		}
		else if(QPlaylistEntry::ACTION_PlaySavedPlaylist == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlaySavedPlaylist> PlayEntry_PlaySavedPlaylist_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlaySavedPlaylist>();
			if(PlayEntry_PlaySavedPlaylist_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			StopPlayback();
			LoadPlaylist();
			m_bIsNeedLoop = false;
			PlayNextEntry(true);
		}
		else if(QPlaylistEntry::ACTION_Volume == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlayerVolume> PlaylistEntry_SetPlayVolume_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlayerVolume>();
			if(PlaylistEntry_SetPlayVolume_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			int iOutRet;
			setPlayVolume(PlaylistEntry_SetPlayVolume_sp->m_iVolumeVaule,iOutRet);
			if(iOutRet == ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoop = false ;
#endif				
				PlayNextEntry();
			}
		}
		else if(QPlaylistEntry::ACTION_Reboot == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Reboot> PlayEntry_Reboot_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Reboot>();
			if(PlayEntry_Reboot_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			ReStart();
		}
		else if(QPlaylistEntry::ACTION_MuteVolume == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlayerMuteVolume> PlaylistEntry_SetPlayVolumeMute_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlayerMuteVolume>();
			if(PlaylistEntry_SetPlayVolumeMute_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			int iOutRet;
			setPlayerMute(PlaylistEntry_SetPlayVolumeMute_sp->m_iIsMute,iOutRet);
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoop = false ;
#endif
			PlayNextEntry();
		}
		else if(QPlaylistEntry::ACTION_Loop == PlayEntry_sp->getActionType())
		{
			int PlayEntry_Loop_id = m_PlaylistDataA.getCurPlayEntryId();
			QSharedPointer <QPlaylistEntry_Loop> PlayEntry_Loop_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Loop>();
			if(PlayEntry_Loop_sp.isNull()||(isNeedLoop == FALSE))/*isNeedLoop to avoid the cycle of death
																	add by clyde 2016-01-11*/
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			PlayEntry_Loop_sp->m_iHasLoopTimes ++;
			if(1 >= m_PlaylistDataA.PlayEntryCount() || 0 == PlayEntry_Loop_id)
			{
				break;
			}
			if( PlayEntry_Loop_sp->m_iTimes == -1)
			{

			}
			else if( PlayEntry_Loop_sp->m_iHasLoopTimes  >=  PlayEntry_Loop_sp->m_iTimes )
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				break;
			}
			//Process this message later because we may trap into this infinite loop.
			removePostedEvents(this, QEVENT_TYPE_PlayFirstEntry);
			QEvent * pEvent = new QEvent((enum QEvent::Type)QEVENT_TYPE_PlayFirstEntry);
			if(NULL == pEvent)
			{
				break;
			}
			postEvent(this, pEvent);
		}
		else if(QPlaylistEntry::ACTION_Stream == PlayEntry_sp->getActionType())
		{
			if(m_bIsPlayVideoLayer)
			{
				StopPlayPluginList();
				m_bIsPlayVideoLayer = false;
				m_bIsPlayPluginList = false;
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->lower();
			}
			
			QSharedPointer <QPlaylistEntry_Stream> PlayEntry_Stream_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Stream>();
			if(PlayEntry_Stream_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}

			PlayEntry_Stream_sp->m_iHasLoopTimes ++;
			if( PlayEntry_Stream_sp->m_iLoopTimes == -1)
			{
				m_bIsNeedLoop = false ;
			}
			else  if( PlayEntry_Stream_sp->m_iHasLoopTimes  >=  PlayEntry_Stream_sp->m_iLoopTimes )
			{
				PlayEntry_Stream_sp->m_iHasLoopTimes = -1;
				m_bIsNeedLoop = false ;
				PlayNextEntry();
				break;
			}
			else
			{
				m_bIsNeedLoop = true ;
			}

			QString strActualFilePath = PlayEntry_Stream_sp->m_strFilePath;
			//
			if(strActualFilePath.indexOf("tuner://") == 0)
			{
				QString strTunerUrl = strActualFilePath.mid(8);				
				setPlayTuner(strTunerUrl, iRet);
				m_iStateType = State_PlayingTuner;
			}
			else
			{
				m_iStateType = State_Streaming;
				iRet = PlayUrl(strActualFilePath);
			}
			if(iRet != ERROR_SUCCESS)
			{
				PlayNextEntry();
				break;
			}
			if(PlayEntry_Stream_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerTimeoutMs = (PlayEntry_Stream_sp->m_iDuration * 1000);
				if(0 == iTimerTimeoutMs)
				{
					iTimerTimeoutMs = 100;
				}
				if(0 < iTimerTimeoutMs)
				{
					m_PlaylistDurationTimer.start(iTimerTimeoutMs);
				}
			}

			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();

			if(&m_playWin == pActiveWindow  || (m_bHasPlayFirst  && pActiveWindow == NULL ))//&& m_FristEntryIsHtml ))
			{
					setPlayListActionPosition(PlayEntry_Stream_sp->m_xLeft,PlayEntry_Stream_sp->m_yUpper,
					PlayEntry_Stream_sp->m_xSize,PlayEntry_Stream_sp->m_ySize);
			}
			else
			{
				if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
				{
					setPlayListActionPosition(PlayEntry_Stream_sp->m_xLeft,PlayEntry_Stream_sp->m_yUpper,
					PlayEntry_Stream_sp->m_xSize,PlayEntry_Stream_sp->m_ySize);
				}
				else if(&m_Browser== pActiveWindow)
				{
					UpdateVideoPosition();
				}
				else if(pActiveWindow == NULL)
				{
					if(m_bIsPlayWin)
					{
						setPlayListActionPosition(PlayEntry_Stream_sp->m_xLeft,PlayEntry_Stream_sp->m_yUpper,
						PlayEntry_Stream_sp->m_xSize,PlayEntry_Stream_sp->m_ySize);
					}
					else
					{
						UpdateVideoPosition();
					}
				}
				else
				{
					if(m_bIsPlayWin)
					{
						setPlayListActionPosition(PlayEntry_Stream_sp->m_xLeft,PlayEntry_Stream_sp->m_yUpper,
						PlayEntry_Stream_sp->m_xSize,PlayEntry_Stream_sp->m_ySize);
					}
					else
					{
						//UpdateVideoPosition();
					}
				}
			}
			if(m_iStateType == State_Streaming)
			{
				m_iStateType = State_PlayingVideoFile;
			}
			if(!m_bIsPlayPluginList)
			{
				m_IsPlayHtmlStatus = false;
			}
		}
		else if(QPlaylistEntry::ACTION_PlayPlaylist == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlayPlaylist> PlayEntry_PlayPlaylist_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlayPlaylist>();
			if(PlayEntry_PlayPlaylist_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			StopPlayback();
			if(PlayEntry_PlayPlaylist_sp->m_strFilePath.trimmed()  == "" )
			{
				StopPlayback();
				break;
			}
			if(LoadPlaylist(PlayEntry_PlayPlaylist_sp->m_strFilePath))
			{
				m_bIsNeedLoop = false;
				PlayNextEntry(true);
			}
			else
			{
				StopPlayback();
			}
		}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
		else if(QPlaylistEntry::ACTION_MediaServer_PlayAudio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlayEntry_MediaServerAudio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_MediaServerAudio>();
			if(PlayEntry_MediaServerAudio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_MediaServerAudio_sp->getActionType(),
				PlayEntry_MediaServerAudio_sp->m_strFilePath);

			m_iStateType = State_PlayingAudioFile;
			iRet = MediaServerPlayAudio(strActualFilePath);

			//LOG_BLINE("MediaServerPlayAudio(%s),iRet=%d\n",qPrintable(strActualFilePath),iRet);

#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoop = false ;
#endif
			/*always to next,play in background*/
			PlayNextEntry();
		    m_iStateType = State_PlayingAudioFile;
		}
		else if(QPlaylistEntry::ACTION_MediaServer_StopAudio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlayEntry_MediaServerAudio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_MediaServerAudio>();
			if(PlayEntry_MediaServerAudio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}

			m_iStateType = State_Idle;
			iRet = MediaServerStopAudio();

			//LOG_BLINE("MediaServer_StopAudio(),iRet=%d\n",iRet);

#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoop = false ;
#endif
			/*always to next*/
			PlayNextEntry();
		}
#endif
	}while(FALSE);
}


VOID QWebBrowserApp::PlayFirstEntry()
{
	bool bFirstEntry = true;
	PlayNextEntry(bFirstEntry);
}


void QWebBrowserApp::customEvent(QEvent * event)
{
	do
	{
		if(CC_UNLIKELY(NULL == event))
		{
			break;
		}
		if(QEVENT_TYPE_PlayFirstEntry == event->type())
		{
			PlayFirstEntry();
		}
		else if(QEVENT_TYPE_PlayNextEntry == event->type())
		{
			PlayNextEntry();
		}
		else if(QEVENT_TYPE_PlayPluginListFirstEntry == event->type())
		{
			PlayPluginListNextEntry(true);
		}
	}while(FALSE);
}

VOID QWebBrowserApp::ShowBrowserWindow()
{
    //LOG_BLINE("ShowBrowserWindow\n");
	int iRet = ERROR_SUCCESS;
	do
	{
		m_playWin.hide();
		m_Browser.showMaximized();
		m_CalcVideoPosTryTimes = 0;
		BringWindowToFront(&m_Browser);
		setActiveWindow(&m_Browser);
		UpdateVideoPosition();
		m_bHasPlayFirst = false;	
		m_bIsPlayWin  = false;
		
		if(m_OsdTimer.isActive())
		{
			m_OsdTimer.stop();
		}
		m_OsdTimer.start(MENUOSDTIMEOUT_MS);		
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);
}

VOID QWebBrowserApp::UpdateVideoPosition(bool bFullScreen/* = false*/)
{
	INT_t iRet;
	int VideoPosX = 0, VideoPosY = 0, VideoPosW = 0, VideoPosH = 0;

	do
	{
		if(m_bIsPlayVideoLayer &&  !bFullScreen)
		{
			break;
		}
		if(bFullScreen)
		{
			
			setVideoPosition(0, 0, 0, 0, bFullScreen);
		}
		else
		{
			iRet = m_Browser.CalcVideoPluginPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
			if(VideoPosX == 0 && VideoPosY == 0)
			{
				if(10 > m_CalcVideoPosTryTimes)
				{
					m_CalcVideoPosTimer.start(200);
					m_CalcVideoPosTryTimes++;
				}
			}
			else
			{
				setVideoPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
				m_CalcVideoPosTimer.stop();
			}
		}
	}while(FALSE);
}

VOID QWebBrowserApp::ShowPlayerWindow()
{
    
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("ShowPlayerWindow\n");
	}
	
	do
	{		
		m_CalcVideoPosTimer.stop();
		m_playWin.show();
		BringWindowToFront(&m_playWin);
		setActiveWindow(&m_playWin);	
		if(!m_PwdInputDlg_sp.isNull())
		{
			if(!m_PwdInputDlg_sp->isHidden())
			{
				m_PwdInputDlg_sp->setTextValue("");
				m_PwdInputDlg_sp->hide();
			}
		}
		int iRet = ERROR_SUCCESS;
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Normal);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(false);
}

VOID QWebBrowserApp::setForegroundWindow(QWidget * pActiveWindow)
{
	Qt::WindowStates flags = pActiveWindow->windowState();
	if (qApp->activeWindow() == pActiveWindow)
	{
	}
	else if (!flags.testFlag(Qt::WindowMinimized))
	{
		pActiveWindow->showMinimized();  // 在这里必须先最小化，否则直接调用showMaximized()可能并不能显示到最前面去。
	}
	pActiveWindow->showMaximized();
}

VOID QWebBrowserApp::BringWindowToFront(QWidget * pActiveWindow)
{
	if(pActiveWindow)
	{
		pActiveWindow->raise();
		pActiveWindow->activateWindow();
	}
}
VOID QWebBrowserApp::BringBrowserToFront()
{
	QWidget * pActiveWindow=&m_Browser;
	
	if(pActiveWindow)
	{
		pActiveWindow->raise();
		pActiveWindow->activateWindow();
	}
}

QString QWebBrowserApp::LookForActualFilePath(QPlaylistEntry::ACTION_TYPE eAction, QString & strFileName,QString strFloderName)
{
	QString strActualFilePath;
	QString strTempFolderPath;
	if(QPlaylistEntry::ACTION_Video == eAction)
	{
		strTempFolderPath = FOLDER_NAME_VIDEO;
	}
	else if(QPlaylistEntry::ACTION_Picture == eAction)
	{
		strTempFolderPath = FOLDER_NAME_PICTURE;
	}
	else if(QPlaylistEntry::ACTION_Audio== eAction)
	{
		strTempFolderPath = FOLDER_NAME_MUSIC;
	}
	else if(QPlaylistEntry::ACTION_Html== eAction)
	{
		strTempFolderPath = FOLDER_NAME_HTML;
	}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
	else if(QPlaylistEntry::ACTION_MediaServer_PlayAudio== eAction)
	{
		strTempFolderPath = FOLDER_NAME_MUSIC;
	}
#endif
	else
	{
		strTempFolderPath = FOLDER_NAME_VIDEO;
	}

	do
	{

		if(strFloderName.trimmed()!="")
		{
			strTempFolderPath = strFloderName ;
		}

		QString strActiveMedia = mp_Config ->getConfigValue(INI_KEY_CurActiveMediaType);
		if(strActiveMedia.trimmed() == "")
		{
			strActiveMedia = LOCAL_STORAGE_DIR_NAME;
		}
		else
		{
			strActualFilePath = (applicationDirPath()+"/" + strActiveMedia + "/" + strTempFolderPath + "/" + strFileName);
			if(QFile::exists(strActualFilePath))
			{
				break;
			}
		}

		strActualFilePath = (applicationDirPath()+"/" + LOCAL_STORAGE_DIR_NAME + "/" + strTempFolderPath + "/" + strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath = MOUNTDIR +(applicationDirPath()+"/" + SD_CARD_DIR_NAME + "/" + strTempFolderPath +"/"+strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath = MOUNTDIR +(applicationDirPath()+"/" + USB_STORAGE_DIR_NAME + "/" + strTempFolderPath +"/"+strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath = MOUNTDIR +(applicationDirPath()+"/" + USB2_STORAGE_DIR_NAME + "/" + strTempFolderPath +"/"+strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath = MOUNTDIR +(applicationDirPath()+"/" + StorageDirName_HDD + "/" + strTempFolderPath +"/"+strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath = MOUNTDIR +(applicationDirPath()+"/" + StorageDirName_RAM + "/" + strTempFolderPath +"/"+strFileName);
		if(QFile::exists(strActualFilePath))
		{
			break;
		}
		strActualFilePath.clear();
	}while(FALSE);

	return strActualFilePath;
}

void QWebBrowserApp::OnTimer_ConnectDBus()
{
	INT_t iOutRet, iRet;

	do
	{
		if(m_DBusConn_sp->isConnected())
		{
			break;
		}
		*m_DBusConn_sp = QDBusConnection::systemBus();
		if(m_DBusConn_sp->isConnected())
		{
			iRet = m_MediaPlayerCtrl_sp->setDBusConnection(m_DBusConn_sp);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("MediaPlayerCtrl.setDBusConn err %d\n", iRet);
				iOutRet = iRet;
				break;
			}
			iRet = m_NetMgrClient_sp->setDBusConn(m_DBusConn_sp);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("NetMgrClient.setDBusConn err %d\n", iRet);
				iOutRet = iRet;
				break;
			}
			iRet = m_NetMgrEthernetClient_sp->setDBusConn(m_DBusConn_sp);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("NetMgrEthernetClient.setDBusConn err %d\n", iRet);
				iOutRet = iRet;
				break;
			}
#if 1 /* Add by xuweiwei 2014-3-4*/
                    iRet = m_QPowerMateClient_sp->setDBusConn(m_DBusConn_sp);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("PowerMateClient.setDBusConn err %d\n", iRet);
				iOutRet = iRet;
				break;
			}
#endif 
			OnTimer_ConnectMPSignal();
		}
		else	//not connected
		{
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			QTimer::singleShot(2*1000, this, SLOT(OnTimer_ConnectDBus()));
		}
	}while(FALSE);
}

void QWebBrowserApp::OnTimer_ConnectMPSignal()
{
	bool bRet;
	INT_t iRet,iCurrOutPutMode;

	do
	{
		if(FALSE == m_DBusConn_sp->isConnected())
		{
			break;
		}
		m_QDbus_MpDefaultIf_sp = QSharedPointer <QDBusInterface> (
			new QDBusInterface(DBUS_MPLAYER_SERVICE_NAME, DBUS_MPLAYER_OBJECT_PATH,
			DBUS_MPLAYER_OBJECT_INTERFACE, (*m_DBusConn_sp), this));
		if(m_QDbus_MpDefaultIf_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		if(m_QDbus_MpDefaultIf_sp->isValid())
		{
			iRet = m_MediaPlayerCtrl_sp->setQDBusMpDefaultIf(m_QDbus_MpDefaultIf_sp);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iRet = m_MediaPlayerCtrl_sp->ChangeVideoOutputMode(VO_MODE_NotSet, &iCurrOutPutMode);
			if(ERROR_SUCCESS == iRet)
			{
				m_iVideoOutMode = (VIDEO_OUTPUT_MODE) iCurrOutPutMode;				
				setGuiOsd2AccordingToCurVideoMode();
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			bRet = connect(&(*m_QDbus_MpDefaultIf_sp), SIGNAL(PlaybackEos2(QString, unsigned int, int)), 
				this, SLOT(On_MP_PlaybackEos2(QString, unsigned int, int)));
			if(true == bRet)
			{
			}
			else
			{
				LOG_BLINE("QDBus.ConnectSignal fail\n");
			}
			ShowIpInfoOnLed();
			On_DbusConnectedToMediaPlayer();
            {
                QDesktopWidget * pDesktopWidget = QApplication::desktop();
                if(pDesktopWidget)
                {
                    QRect DesktopRect = pDesktopWidget->availableGeometry();            
                    m_MediaPlayerCtrl_sp->setOsdSize(DesktopRect.width(), DesktopRect.height());
                }       

            }

		}
		else
		{
			LOG_BLINE("QDbus: Can't get the MP default interface.\n");
			QTimer::singleShot(2*1000, this, SLOT(OnTimer_ConnectMPSignal()));
		}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
		m_QDbus_MediaServer_sp = QSharedPointer <QDBusInterface> \
                   ( new QDBusInterface( DBUS_MEDIASERVER_SERVICE_NAME,
                                         DBUS_MEDIASERVER_OBJECT_PATH,
                                         DBUS_MEDIASERVER_OBJECT_INTERFACE,
                                         (*m_DBusConn_sp), this ) );

		if(m_QDbus_MediaServer_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		if(m_QDbus_MediaServer_sp->isValid())
		{
			iRet = m_MediaPlayerCtrl_sp->setQDBusMediaServerIf(m_QDbus_MediaServer_sp);
			if(iRet != ERROR_SUCCESS)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
#endif
	}while(FALSE);
}

void QWebBrowserApp::On_DbusConnectedToMediaPlayer()
{
	INT_t iRet;
	//bool bRet;
	do
	{
		if(FALSE == m_hasPlayedStatrUpItem)
		{
			m_hasPlayedStatrUpItem = TRUE;
			if(m_strDefaultStartupItem != "")
			{
				iRet = PlayCeLabsUrl(m_strDefaultStartupItem);
				if(m_OsdTimer.isActive())
				{
					m_OsdTimer.stop();
				}
				m_OsdTimer.start(MENUOSDTIMEOUT_MS);
				if(ERROR_SUCCESS == iRet)
				{
					m_eMediaPlayCurrStatus = MediaPlay_Status_PlayCeLabsFile;
					m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_DefaultStartupItem;
				}
				else
				{
					m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_Unknown,
					m_bInValidDefaultStatrUpItem = true;
					m_Browser.showMaximized();

					if(m_PlayIdlePlayList_Timer.isActive())
					{
						m_PlayIdlePlayList_Timer.stop();
					}
					m_iIdleTime = 0;
					if(m_iStateType == State_Idle)
					{
						if(m_TunerScanStatus ==TunerScanning)
						{					
							break;
						}
						startIdelDetectTimer();
					}
					break;
				}

			}
			else
			{
				//m_Browser.showMaximized();
				m_strBootSplashPlayUrl =  mp_Config ->getConfigValue(INI_KEY_BootSplashPlayUrl);
				if(m_strBootSplashPlayUrl.trimmed() == "")
				{
					break;
				}
				else
				{			
					iRet = PlayCeLabsUrl(m_strBootSplashPlayUrl);
					if(iRet==ERROR_SUCCESS)
					{
						m_BIsPlaySplash = true;
					}
					if(m_BootSplashTimer.isActive())
					{
						m_BootSplashTimer.stop();
					}
					m_BootSplashTimer.start(MENUOSDTIMEOUT_MS);
				}
				
				if(m_OsdTimer.isActive())
				{
					m_OsdTimer.stop();
				}
				m_OsdTimer.start(MENUOSDTIMEOUT_MS);
				QString IdleTime = mp_Config ->getConfigValue(INI_KEY_IdleDetectTime);
				if(IdleTime.trimmed()!="")
				{
					int idleDetect = -1;
					idleDetect = IdleTime.trimmed().toInt();
					if(0 > idleDetect)
					{
						break;
					}
					if(!m_bIsPlayingIdlePlayList)
					{
						if(m_eMediaPlayCurrStatus != MediaPlay_Status_PlaySelectFile)
						{
							if(m_iStateType == State_Idle )
							{
								if(m_TunerScanStatus ==TunerScanning)
								{					
									break;
								}
								startIdelDetectTimer();
							}
						}
					}
				}
			}
		}
	}while(FALSE);
}

//Not call QtDBus module right now, because we are in dbus handling.
void QWebBrowserApp::On_MP_PlaybackEos2(const QString CurPlayUrl, const unsigned int PlaybackId, const int bIsCompletePlaybackEos)
{
	bool bShouldShowBrowserWindow = false;
	do
	{
#if 1/*add by lxj 2012-7-20, write PlaybackEos event to log */

		if(Sw_LogDbusCmd)
		{
			LOG("PEos,Url=%s,iStateType=%d\n", qPrintable(CurPlayUrl), m_iStateType);
		}

		do
		{
			LOG_BLINE("call when mediaplayer \n");
			BOOL_t bEnableWriteLoggin = TRUE;
			if(playbackIdIsEnded(PlaybackId))
			{
				goto WriteEventLog_DONE;
			}
			rememberEndedPlaybackId(PlaybackId);
			
			INT_t iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}

			if(bEnableWriteLoggin)
			{
				CString::STD_STRING_LIST strList ;
				strList.push_back(TYPE_LOG_PlaybackEos);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				QString strPlayType;
				getLastPlayEntryInfo(OUT strPlayType, PlaybackId);
				strList.push_back(strPlayType.toLocal8Bit().constData());
				strList.push_back(CurPlayUrl.toLocal8Bit().constData());
				strList.push_back(PlaybackId);
				strList.push_back(bIsCompletePlaybackEos);
				if(ERROR_SUCCESS != WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				strList.clear();
			}
		}while(FALSE);
WriteEventLog_DONE:
		;
#endif

		if(m_strCurPlayUrl != CurPlayUrl || m_PlaybackId != PlaybackId)
		{
			if(Sw_LogPlaybackStatus)
			{
				LOG_BLINE("m_strCurPlayUrl=%s,\n", qPrintable(m_strCurPlayUrl));
				LOG_BLINE(" Url=%s,PlaybackId=%d,m_PlaybackId=%d\n",qPrintable(CurPlayUrl),PlaybackId,m_PlaybackId);
			}
			//m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
			break;
		}
		if(CONTENT_TRIGGER_SRC_DefaultStartupItem == m_eContentTrigSrcType ||
			CONTENT_TRIGGER_SRC_PlayVirChannleItem == m_eContentTrigSrcType)
		{
			//LOG_LINE("FileType:%d,PlayEnd:%d\n", m_eCurPlayMediaFileType, m_PlaylistDataA.IsPlayEnd());
			LOG_BLINE("FileType:%d\n", m_eCurPlayMediaFileType);
			if(AMedia_TYPE_VIDEO == m_eCurPlayMediaFileType || AMedia_TYPE_AUDIO == m_eCurPlayMediaFileType ||
				AMedia_TYPE_PICTURE == m_eCurPlayMediaFileType || AMedia_TYPE_Tuner == m_eCurPlayMediaFileType ||
				(AMedia_TYPE_PLAYLIST == m_eCurPlayMediaFileType && m_PlaylistDataA.IsLastEntry()))
			{
				m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_DefaultPlaylist;
				//bShouldShowBrowserWindow = true;
			}
			else if(AMedia_TYPE_HTML == m_eCurPlayMediaFileType)
			{
				m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_DefaultPlaylist;
				{
					//bShouldShowBrowserWindow = true;
				}
			}
		}

		/*each other meida status handle respectively*/
		if(AMedia_TYPE_PLAYLIST == m_eCurPlayMediaFileType)
		{
			if(m_bIsHavePlayItemTemp)
			{
				//LOG_BLINE("play item temporary.\n");
				PlayItemTemporary();
			}
			else
			{
				LOG_BLINE("play next item.\n");
				StopPlayback();
				if(PlayPluginListflag>=1)
					{
						PlayPluginListflag=0;
						PlayPluginListNextEntry();
					}
				else
					{
						PlayNextEntry();
					}
			}
		}
		else
		{
			if(1 < m_NextFileInfoToPlay.m_iLoopTimes)
			{
				int iOutRet;
				m_NextFileInfoToPlay.m_iLoopTimes--;
				playMedia((ActiveMedia_TYPE)m_NextFileInfoToPlay.m_iDeviceType, 
					m_NextFileInfoToPlay.m_strFileType,m_NextFileInfoToPlay.m_strFilename, 
					iOutRet,m_NextFileInfoToPlay.m_iLoopTimes,m_NextFileInfoToPlay.m_iDuration,
					m_NextFileInfoToPlay.m_iPosition,"",true,m_NextFileInfoToPlay.m_bIsVideoLayer);
			}
			else
			{
#if 1 //Added by jaylon for loop 
				LOG_BLINE("iLoopTime =%d\n",m_NextFileInfoToPlay.m_iLoopTimes);
				if(m_NextFileInfoToPlay.m_bIsInitZero && !m_NextFileInfoToPlay.m_bIsInitOne )
				{
					int iOutRet;
					playMedia((ActiveMedia_TYPE)m_NextFileInfoToPlay.m_iDeviceType, 
						m_NextFileInfoToPlay.m_strFileType,m_NextFileInfoToPlay.m_strFilename, 
						iOutRet,m_NextFileInfoToPlay.m_iLoopTimes,m_NextFileInfoToPlay.m_iDuration,
						m_NextFileInfoToPlay.m_iPosition,"",true,m_NextFileInfoToPlay.m_bIsVideoLayer);
				}
#endif 
				if(m_bIsHavePlayItemTemp)
				{
					//LOG_BLINE("play item temporary.\n");
					PlayItemTemporary();
				}
			}
		}


#if 0
		if(m_NextFileInfoToPlay.m_strFilename.trimmed() != "")
		{
		}
		else
		{
			if(m_PluginPlaylistData_sp->IsLastEntry())
			{					
				BOOL_t bOld_IsPlayPluginList = m_bIsPlayPluginList;								
				if(bOld_IsPlayPluginList)
				{
					if(m_bIsNeedLoopPlaylistBEntry)
					{
						PlayPluginListNextEntry(false,false);
					}
					else
					{
						m_PluginPlaylistData_sp->ClearPlayListData();
						m_bIsPlayPluginList = false;
						emit onPlayPluginListBackEnd();
					}
				}
				else
				{
#if 0/*added by lxj 2013-1-10 for debug*/ 
					LOG_BLINE("%s(),m_strFilename=\"%s\",StopPlayback()\n",__func__,qPrintable(m_NextFileInfoToPlay.m_strFilename));
#endif
					StopPlayback();
					PlayNextEntry();	
				}
			}
			else
			{
				PlayPluginListNextEntry();
			}
		}
#endif
	}while(FALSE);

	if(bShouldShowBrowserWindow)
	{
		ShowBrowserWindow();
	}
}

void QWebBrowserApp::On_DefKey_Down(int key_value)
{
	INT_t iRet;

	#if 1/*by_su  for test twice of key event         add this in next version*/
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("key=0x%08x\n", key_value);
	}
	#endif

#if 0/*added by lxj 2012-11-14*/
	DBG_TIME("%s() begin\n",__func__);
#endif

	if(key_value == IR_KEY_MENU)
	{
		QObject * pSender = NULL;
		QWidget * pActiveWindow = NULL;

		pSender = sender();
		pActiveWindow = activeWindow();
		
	#if 1/*by_su*/
		if(pSender == NULL &&  pActiveWindow == &m_playWin)
		{
			UpdateVideoPosition();
			if(m_PlayPicAndHtmlWin->isPlayingPicture())
			{
				StopPlayback();
                m_PlayPicAndHtmlWin->playHtmlFileEnd();
			}
			ShowBrowserWindow();
		}

	#endif
	#if 0
		if(pSender == &m_playWin &&  pActiveWindow == &m_playWin)
		{
			if(m_PlayPicAndHtmlWin->isPlayingPicture())
			{
				StopPlayback();
				m_PlayPicAndHtmlWin->playHtmlFileEnd();				
			}
			ShowBrowserWindow();
		}
		else if(pSender == NULL &&  pActiveWindow == &m_playWin)
		{
			UpdateVideoPosition();
		}
	#endif

		m_ChannelNumber = "";
	}
	else if(IR_KEY_STOP == key_value)
	{
#if 0/*added by lxj 2012-11-14*/
		DBG_TIME("%s() IR_KEY_STOP\n",__func__);
#endif		
		//add for when play html stop,then osd timeout
		m_PlaybackId = 0; //must the same with mediaplay
		StopPlayDurationTimer();
		StopPlayback();
		m_ChannelNumber = "";
		if(m_bIsPlayVideoLayer)
		{
			m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
			m_playWin.getVideoLayerWnd()->hide();
			m_playWin.getVideoLayerWnd()->lower();
		}
		m_bIsPlayVideoLayer = false;		
	}
	else if(IR_KEY_DOWN == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		#if 1/*add 2016-01-06  Two events will be triggered when the button is pressed down*/
		if(&m_playWin == pActiveWindow) 
			m_PlaylistDataA.m_keydownflag = 1;
		#endif
		if(m_iStateType == State_PlayingTuner)
		{
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				int iRet = ERROR_SUCCESS;
				setChannelDown(0,iRet);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}				
			}
			return;
		}
#if 1 // plays next wrong when acts in web window.	
		if(pActiveWindow == &m_playWin && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
		{
			StopAllPlayDurationTimer();		
			StopPlayback();
			PlayNextEntry();
		}
#endif
		m_ChannelNumber = "";
	}
	else if(IR_KEY_UP == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(m_iStateType == State_PlayingTuner)
		{
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				int iRet = ERROR_SUCCESS;
				setChannelUp(0,iRet);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			m_ChannelNumber = "";
			return ;
		}
		if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
		{
			StopAllPlayDurationTimer();
			//if(StopPlayback() == ERROR_SUCCESS)
			 //StopPlayback will have playbackeos will next first
			{
				PlayNextEntry(false,true);
			}
		}		
		m_ChannelNumber = "";
	}
	else if(IR_KEY_RIGHT == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
		{
			m_eCurPlayMode = ControlCenter::PlayMode_FastForward;
			if(m_MediaPlayerCtrl_sp)
			{
				m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_FastForward);
			}
		}
		m_ChannelNumber = "";
	}
	else if(IR_KEY_LEFT == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
		{
			m_eCurPlayMode = ControlCenter::PlayMode_Rewind;
			m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_Rewind);
		}
		m_ChannelNumber = "";
	}
	else if(IR_KEY_ENTER == key_value ||
		IR_KEY_RETURN == key_value )
	{		
		if(FALSE == m_TunerChannelBar_sp.isNull())
		{
			if(FALSE == m_TunerChannelBar_sp->isHidden())
			{
				m_TunerChannelBar_sp->On_TimeoutHide();
			}
		}
		m_ChannelNumber = "";
	}
	else if(IR_KEY_CHANGERESOLUTION == key_value)
	{
		iRet = switchToNextVideoOutputMode();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		else
		{
			do
			{
				BOOL_t bEnableWriteLoggin = TRUE;
				iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
				if(ERROR_SUCCESS != iRet)
				{
					break;
				}
				if(bEnableWriteLoggin)
				{
					CString::STD_STRING_LIST strList ;
					strList.push_back(TYPE_LOG_ResolutionChange);
					strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
					QString strVoModeDesc;
					MP7XX::QAudioVideoInOutInfo::getVoDesc(m_iVideoOutMode, OUT strVoModeDesc);
					strList.push_back(strVoModeDesc.toLocal8Bit().constData());
					if(ERROR_SUCCESS != WriteEventLog(strList))
					{
						PRINT_BFILE_LINENO_CRT_ERRINFO;
					}
					else
					{
						strList.clear();
					}
				}
			}while(FALSE);
		}
		m_ChannelNumber = "";
	}
#if 1/*add by lxj 2012-7-24 for remote control*/
	else if(IR_KEY_PAUSE == key_value)
	{
		//LOG_BLINE("IR_KEY_PAUSE\n");
		int iRet,iCurrentMode;
		getPlayMode(iRet,iCurrentMode);
		if(iRet==ERROR_SUCCESS)
		{
			if( iCurrentMode != ControlCenter::PlayMode_Pause &&
		            iCurrentMode != ControlCenter::PlayMode_StopAll &&
		            iCurrentMode != ControlCenter::PlayMode_StopMenu )
			{
				setPlayMode(ControlCenter::PlayMode_Pause ,iRet);
				if(iRet != ERROR_SUCCESS)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		else
		{
			if(Sw_LogDbusCmd)
			{
				LOG_BLINE("getPlayMode=%d,iCurrentMode=%d\n",(int)(iRet),(int)(iCurrentMode));
			}
		}
		m_ChannelNumber = "";
	}
#endif
	else if(IR_KEY_VOLMUTE ==  key_value )
	{
		do
		{
			INT_t iOutRet = ERROR_SUCCESS,  iCurVolume = 0;
			m_MediaPlayerCtrl_sp->getCurrentVolume(&iCurVolume);
			if(FALSE == m_bVolumeMuted)
			{
				iOutRet = m_MediaPlayerCtrl_sp->changeVolume(-iCurVolume);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}

				m_bVolumeMuted = TRUE;
			}
			else if(m_bVolumeMuted)
			{
				iOutRet = m_MediaPlayerCtrl_sp->changeVolume(iCurVolume);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}

				m_bVolumeMuted = FALSE;
			}
		}while(FALSE);
		m_ChannelNumber = "";
	}
	else if(IR_KEY_TUNERCHANNELUP == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(m_iStateType == State_PlayingTuner)
		{
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				int iRet = ERROR_SUCCESS;
				setChannelUp(0,iRet);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		m_ChannelNumber = "";
	}
	else  if(IR_KEY_TUNERCHANNELDOWN == key_value)
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(m_iStateType == State_PlayingTuner)
		{
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				int iRet = ERROR_SUCCESS;
				setChannelDown(0,iRet);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		m_ChannelNumber = "";
	}
#if 1/*added by lxj 2012-10-8 for usb video switch*/
	else if(IR_KEY_USB_VIDEO == key_value){
		int iOutRet;
		QString strUSBCapture=USB_CAPTURE_URL;
		switch( m_iStateType ){
			case State_Idle:
				m_iStateTypePrevious = State_USB_Capture;
				m_strPlayUrlPrevious = strUSBCapture;
play_usb_capture:
				{
#if 1
					On_DefKey_Down(IR_KEY_STOP);/*like MP500*/
#else
					m_PlaybackId = 0;
					QWidget * pActiveWindow = NULL;
					pActiveWindow = activeWindow();
					if(pActiveWindow == m_PlayPicAndHtmlWin)
					{
						m_PlayPicAndHtmlWin->playHtmlFileEnd();
					}
					{
						StopPlayDurationTimer();
						StopPlayback();
						ShowBrowserWindow();
					}
#endif
				}
				parsePlayMediaCommad(strUSBCapture,iOutRet,PlayMedia_MODE);/*like SNMP play*/
				if(iOutRet != ERROR_SUCCESS ){
					goto replay_media;
				}
				break;
			case State_PlayingBrowserFile:
			case State_PlayingPlayList:
			case State_PlayingVideoFile:
			case State_PlayingAudioFile:
			case State_PlayingPictureFile:
				getCurrentPlayItemInfo(m_iStateTypePrevious,m_strPlayUrlPrevious);
				if(Sw_LogDbusCmd)
				{
					LOG_BLINE("m_iStateTypePrevious=%d,m_strPlayUrlPrevious=%s\n",(int)(m_iStateTypePrevious),qPrintable(m_strPlayUrlPrevious));
				}
				goto play_usb_capture;
			case State_USB_Capture:
replay_media:
				On_DefKey_Down(IR_KEY_STOP);
				switch( m_iStateTypePrevious ){
					case State_PlayingBrowserFile:
						m_Browser.playSelectFile(m_strPlayUrlPrevious,AMedia_TYPE_HTML);		 
						break;
					case State_PlayingPlayList:
						m_Browser.playSelectFile(m_strPlayUrlPrevious,AMedia_TYPE_PLAYLIST); 
						break;
					case State_PlayingVideoFile:
						m_Browser.playSelectFile(m_strPlayUrlPrevious,AMedia_TYPE_VIDEO);
						break;
					case State_PlayingAudioFile:
						m_Browser.playSelectFile(m_strPlayUrlPrevious,AMedia_TYPE_AUDIO);
						break;
					case State_PlayingPictureFile:
						m_Browser.playSelectFile(m_strPlayUrlPrevious,AMedia_TYPE_PICTURE); 
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		m_ChannelNumber = "";
	}
#endif
	else 
	{
		do
		{			
			if(key_value >=IR_KEY_1  && key_value <= IR_KEY_9)
			{

				static UINT64_t m_RepeatableKeyChkStartTimeMs=0;
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();

				if(400 > GetSysUpTimeMs() - m_RepeatableKeyChkStartTimeMs)	//in 400ms
				{
					//ignore
					break;
				}
				m_RepeatableKeyChkStartTimeMs=GetSysUpTimeMs();
				if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
				{				
					iRet = Exec_PlayTunerChannel_Number(key_value) ;
					if(iRet != ERROR_SUCCESS)
					{
						break;
					}
				}	
				else 
				{
					if(FALSE == m_TunerChannelBar_sp.isNull())
					{
						if(FALSE == m_TunerChannelBar_sp->isHidden())
						{
							iRet = Exec_PlayTunerChannel_Number(key_value) ;
							if(iRet != ERROR_SUCCESS)
							{
								break;
							}
						}
					}
				}
			}				
		}while(FALSE);
	}

#if 0/*added by lxj 2012-11-14*/
	DBG_TIME("%s() end\n",__func__);
#endif
}

int QWebBrowserApp::exec()
{
	int iProcessExitCode = ERROR_SUCCESS;
	INT_t iRet;

	do
	{
		/*
		{
			QTextCodec * pTextCodec = QTextCodec::codecForCStrings();
			LOG_BLINE("pTextCodec=%p,%s\n", pTextCodec, (pTextCodec?pTextCodec->name().constData():""));
		}
		{
			QTextCodec * pTextCodec = QTextCodec::codecForLocale();
			LOG_BLINE("pTextCodec=%p,%s\n", pTextCodec, (pTextCodec?pTextCodec->name().constData():""));
		}
		*/
#if	0
		{
			QFont AppFnt = QApplication::font();
			LOG_BLINE("AppFnt:%s\n", qPrintable(AppFnt.toString()));
			//*
			AppFnt.setFamily("unifont");
			setFont(AppFnt);
			LOG_BLINE("AppFnt:%s\n", qPrintable(AppFnt.toString()));
			//*/
			LOG_BLINE("Bold:%d\n", AppFnt.bold());
			QFont::Capitalization FntCapi = AppFnt.capitalization();
			LOG_BLINE("FntCapi:%d\n", FntCapi);
			LOG_BLINE("defaultFamily:%s\n", qPrintable(AppFnt.defaultFamily()));
			LOG_BLINE("exactMatch:%d\n", AppFnt.exactMatch());
			LOG_BLINE("fixedPitch:%d\n", AppFnt.fixedPitch());
			LOG_BLINE("key:%s\n", qPrintable(AppFnt.key()));
			//BUG, abort
			/*
			if(FALSE == AppFnt.lastResortFont().isNull())
			{
				LOG_BLINE("lastResortFont:%s\n", qPrintable(AppFnt.lastResortFont()));
			}
			*/
			LOG_BLINE("rawName:%s\n", qPrintable(AppFnt.rawName()));
			LOG_BLINE("style:%d\n", AppFnt.style());
			LOG_BLINE("styleHint:%d\n", AppFnt.styleHint());
			LOG_BLINE("styleStrategy:%d\n", AppFnt.styleStrategy());
			LOG_BLINE("substitutions:\n");
		}
#endif
		/*
		QFont font;
		font.setFamily(QString::fromUtf8("msyh.ttf"));
		font.setPointSize(18);
		app.setFont(font);
		QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
		QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
		app.addLibraryPath("/plugins");
		*/

        //support chinese character, modifcation of abner on 2014-9-26
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

		iRet = InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iProcessExitCode = WB_PEC_INIT_INST_FAIL;
#if 0 //added by lxj 2014-5-8
			break;
#endif
		}
		iProcessExitCode = QApplication::exec();
		iRet = ExitInstance();
	}while(FALSE);

	//LOG_BLINE("QWebBrowserApp::exec end\n");

	return iProcessExitCode;
}

VOID QWebBrowserApp::LinuxSigHandlerCb(VOID * pCbContext, int SigNo)
{
	QWebBrowserApp * pApp = (typeof(pApp))pCbContext;
	pApp->QSigHandler(SigNo);
}

void QWebBrowserApp::QSigHandler(int SigNo)
{
	LOG("(%d)Signal:%s\n", getpid(), CSignalInfo::GetSingalDescFromSigNo(SigNo));

	do
	{
		if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
		{
			m_bShouldQuitApp = TRUE;
			quit();
		}
	}while(FALSE);
}

INT_t QWebBrowserApp::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;

	DECLARE_WORK_BUSY_SECTION;

	do
	{
#if 1 //added by lxj 2014-5-8
		m_Browser.initSys();
		m_Browser.showMaximized();
		//m_Browser.hide();
#endif

#if 1 /*Add by xuweiwei 2014-8-2 for Set cursor to hidden areas */
		QDesktopWidget * pDesktopWidget = QApplication::desktop();
		if(pDesktopWidget)
		{
			QRect DesktopRect = pDesktopWidget->availableGeometry();
			//LOG_BLINE("DesktopRect.width():%d,DesktopRect.height():%d\n",DesktopRect.width(),DesktopRect.height());
			QCursor::setPos(DesktopRect.width(), DesktopRect.height());
		}	
#endif


#if 0/*added by lxj 2012-8-6 for test memory copy speed*/
		unsigned char *pDest=new unsigned char[1*1024*1024];
		unsigned char *pSource=new unsigned char[1*1024*1024];

		unsigned int copySize=0;
		timeval tick1,tick2;
		long speed=0;

		if(pDest&&pSource)
		{			
#if 1/*method 1*/
			copySize=40*1024;/*GB*/
			gettimeofday(&tick1,NULL);

			while(1)
			{
				memmove(pDest,pSource,1*1024*1024);
				copySize--;
				if(copySize==0){
					break;
				}
			}
			
			gettimeofday(&tick2,NULL);
			
			if(tick2.tv_sec - tick1.tv_sec)/*can't to zero*/
			{
				speed = (40*1024) / (tick2.tv_sec - tick1.tv_sec);
			}
			LOG_BLINE("Method 1 >> Memory copy size=40GB,Elapse:%lds,Test speed=%ldMB/s\n",tick2.tv_sec - tick1.tv_sec,speed);
#endif
#if 1/*method 2*/
			copySize = 0;
			gettimeofday(&tick1,NULL);

			while(1)
			{
				memcpy(pDest,pSource,1*1024*1024);
				gettimeofday(&tick2,NULL);
				copySize++;

				if((tick2.tv_sec - tick1.tv_sec)>=20)
				{
					break;
				}
			}
			
			
			speed = (copySize) / (20);
			LOG_BLINE("Method 2 >> Elapse:20s,Memory copy size=%dMB,Test speed=%ldMB/s\n",copySize,speed);
#endif
			delete pDest;
			pDest = NULL;

			delete pSource;
			pSource = NULL;
		}
#endif
		mp_Config = QSharedPointer <configuration> (new configuration(DEFAULT_CONFIG_PATH));;
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		m_PluginPlaylistData_sp = QSharedPointer <QPlaylistData> (new QPlaylistData);
		if(m_PluginPlaylistData_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_NetMgrClient_sp = QSharedPointer <QNetworkManagerClient> (new QNetworkManagerClient());;
		if(m_NetMgrClient_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_BackendTransactionThread_sp = QSharedPointer <QBackendTransactionThread> (new QBackendTransactionThread);
		if(m_BackendTransactionThread_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(!mp_Config->open_file())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

#if 1/*added by lxj 2012-12-11*/
		m_TouchScreenCalibrationProcess_sp = QSharedPointer <QProcess> (new QProcess);
		if( m_TouchScreenCalibrationProcess_sp.isNull() ){
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
#endif

		//update device connection status
		iRet = UpdateDevMountStatus();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		//Run Xapps if any
		{
			QString strRunXappCmd = APP_DIR_PATH + "RunXapp.sh";
			iRet = system(strRunXappCmd.toUtf8());
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", qPrintable(strRunXappCmd), iRet);
			}
		}

		checkAutoReboot();
		m_strDefaultStartupItem = mp_Config ->getConfigValue(INI_KEY_DefaultStartupItem);
		if(mp_Config ->getConfigValue(INI_KEY_CurActiveMediaType) == "")
		{
		}
		else
		{
			m_eActiveMediaType = (typeof(m_eActiveMediaType))mp_Config ->getConfigValue(INI_KEY_CurActiveMediaType).toInt();
			if(m_eActiveMediaType == AMedia_TYPE_Unknown)
			{
				m_eActiveMediaType = AMedia_TYPE_FLASH;
			}
		}
		m_strActiveStorageDirName = getStorageDirNameFromActiveMediaType(m_eActiveMediaType);
		m_strIPv4Addr = mp_Config->getConfigValue(KEY_NetMgrEthernet_IPv4Addr);
		if(m_strIPv4Addr.trimmed()  =="")
		{
			//read IP
		}	
#if 1/*added by lxj 2012-81-1 for get host mac address*/
		QList<QNetworkInterface> listNetWorkInfo = QNetworkInterface::allInterfaces();
		if(listNetWorkInfo.length() > 1){
			m_strMacAddress = listNetWorkInfo.at(1).hardwareAddress();
		}
		//LOG_LINE("MAC: %s\n",qPrintable(m_strMacAddress));
		/*Read enabled features from SystemConfig.ini*/
		int iEnabledFeatures=DK_DEFAULT_ENABLED;
		QString strEnabledFeatures = mp_Config->getConfigValue(INI_KEY_DEVICE_KEY);
		if(strEnabledFeatures.trimmed() != "")
		{
			bool ok;
			int i = strEnabledFeatures.toInt(&ok,10);
			if(ok)
			{
				iEnabledFeatures= i;
			}
		}
		iRet = m_Mp7xxGlobalStatusData.setEnabledFeatures(iEnabledFeatures);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
#endif
		iRet = InitEventLog();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		//add log
		{
			BOOL_t bEnableWriteLoggin = TRUE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			if(bEnableWriteLoggin)
			{
				char strTemp[64];
				snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_SystemStart);
				strTemp[sizeof(strTemp)-1] = '\0';
				CString::STD_STRING_LIST strList ;
				strList.push_back(strTemp);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				if(ERROR_SUCCESS !=WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				else
				{
					strList.clear();
				}
			}
		}
		iRet = CLinuxSignalHandlerEx::HookSignal(SIGINT, LinuxSigHandlerCb, this);
		if(ERR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to attach SIGINT, err %d.\n", iRet);
		}
		iRet = CLinuxSignalHandlerEx::HookSignal(SIGTERM, LinuxSigHandlerCb, this);
		if(ERR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to attach SIGTERM, err %d.\n", iRet);
		}
		iRet = CLinuxSignalHandlerEx::HookSignal(SIGQUIT, LinuxSigHandlerCb, this);
		if(ERR_SUCCESS != iRet)
		{
			LOG_BLINE("Failed to attach SIGQUIT, err %d.\n", iRet);
		}
		
		m_DBusConn_sp = QSharedPointer <QDBusConnection> (new QDBusConnection(QDBUS_CONN_NAME_WB_UI));
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_MediaPlayerCtrl_sp = QSharedPointer <QMediaPlayerCtrl> (new QMediaPlayerCtrl());
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
        
		m_NetMgrEthernetClient_sp = QSharedPointer <QNetMgrEthernetClient> (new QNetMgrEthernetClient());;
		if(m_NetMgrEthernetClient_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
        
#if 1 /*Add by xuweiwei 2014-3-4*/
		m_QPowerMateClient_sp = QSharedPointer <QPowerMateClient> (new QPowerMateClient());;
		if(m_QPowerMateClient_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}                
#endif
		mWebBrowserSrv_sp = QSharedPointer <CWebBrowserSrv> (new CWebBrowserSrv);
		if(mWebBrowserSrv_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		mMediaVirChannel_sp = QSharedPointer <QMediaVirtualChannel> (new QMediaVirtualChannel);
		if(mMediaVirChannel_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(m_playWin.getPlayHtmlOrPicWin().isNull())
		{
			if(Sw_LogDbusCmd)
			{
				LOG_BLINE("play html or picture window is null\n");
			}
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		m_PlayPicAndHtmlWin = m_playWin.getPlayHtmlOrPicWin();
		bRet = connect(&(*m_PlayPicAndHtmlWin), SIGNAL(getPlayListFiles_signal(int,QStringList &,QStringList &)), this, SLOT(getPlayListFiles(int,QStringList &,QStringList &)));
		bRet = connect(&(*m_PlayPicAndHtmlWin), SIGNAL(playSelectFile_signal(const QString,int)), this, SLOT(PlayCurrentFile(const QString,int)));
		if(false == bRet)
		{
			LOG_BLINE("ConnectSig err\n");
		}
		
		m_WbStatusBar_sp = QSharedPointer <QWbStatusBar> (new QWbStatusBar);
		if(m_WbStatusBar_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_WbStatusBar_sp->m_this_wp = m_WbStatusBar_sp;

		//DBus srv path: UI
		m_QDbusIfUiSrv_sp = QSharedPointer <QDBusIf_UiSrv> (new QDBusIf_UiSrv(this));
		if(m_QDbusIfUiSrv_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = Load_JsFile();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		OnTimer_OpenGpioBtnInputFile();

		QDir config_Dir( qApp->applicationDirPath() + "/"+CONFIG_NAME);
		if(!config_Dir.exists())
		{
			config_Dir.mkdir( qApp->applicationDirPath() + "/"+ CONFIG_NAME);
		}

		mp_AppInfo_Config = QSharedPointer <configuration> (new configuration(APP_INFO_CONFIG_PATH));;
		if(mp_AppInfo_Config.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(!mp_AppInfo_Config->open_file())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
		}

		if(QFile::exists(APP_DIR_PATH + MEDIA_VIR_CH_LIST_FILE_PATH))
		{
			iRet = mMediaVirChannel_sp->LoadCfgFromXmlFile(APP_DIR_PATH + MEDIA_VIR_CH_LIST_FILE_PATH);
			if(ERROR_OUT_OF_MEMORY == iRet)
			{
				iOutRet = iRet;
				break;
			}
			else if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

#if 0 //because of the gpio board of MP800 desiged err.

		for(int i = 1; i<= GPIOBUTTONSIZE; i++)
		{
			QSharedPointer <QGpioFilePath> GpioFilePath_sp(new QGpioFilePath);
			if(GpioFilePath_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			//LOG_BLINE("%s\n", qPrintable(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i)))));
			GpioFilePath_sp->m_strUrl = mp_Config ->getConfigValue(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i))));

			m_GpioFilePath_Map[i] = GpioFilePath_sp;
		}
#else
		for(int i = 1, j = 1, k = 5; i <= GPIOBUTTONSIZE; i++)
		{
			QSharedPointer <QGpioFilePath> GpioFilePath_sp(new QGpioFilePath);
			if(GpioFilePath_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			if(i % 2)
			{
				//LOG_BLINE("%s\n", qPrintable(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i)))));
				GpioFilePath_sp->m_strUrl = mp_Config ->getConfigValue(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i))));
				//LOG_BLINE("num = %d ,%s\n",j,qPrintable(GpioFilePath_sp->m_strUrl));
				m_GpioFilePath_Map[j++] = GpioFilePath_sp;
			}
			else
			{
				//LOG_BLINE("%s\n", qPrintable(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i)))));
				GpioFilePath_sp->m_strUrl = mp_Config ->getConfigValue(QString(INI_KEY_GpioPlayFilePath+QString("_%1").arg(QString::number(i))));
				//LOG_BLINE("num = %d ,%s\n",k,qPrintable(GpioFilePath_sp->m_strUrl));
				m_GpioFilePath_Map[k++] = GpioFilePath_sp;
			}
		}
#endif

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		OnTimer_ConnectDBus();

		//ControlCenter Srv
		bRet = m_DBusConn_sp->registerService(DBUS_SrvName_ControlCenter);
		if(false == bRet)
		{
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_SrvName_ControlCenter);
			}
			break;
		}

		//Reg obj UiSrv
		bRet = m_DBusConn_sp->registerObject(DBUS_PATH_WebBrowser_UiSrv, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("RegDBusObj,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_PATH_WebBrowser_UiSrv);
			}
			break;
		}

#if 0 //added by lxj 2014-5-8
		m_Browser.initSys();
		m_Browser.showMaximized();
		m_Browser.hide();
#endif

		bRet = connect(&m_Browser, SIGNAL(getPlayListFiles_signal(int,QStringList &,QStringList &)), this, SLOT(getPlayListFiles(int,QStringList &,QStringList &)));
		bRet = connect(&m_Browser, SIGNAL(playSelectFile_signal(const QString,int)), this, SLOT(PlayCurrentFile(const QString,int)));
		bRet = connect(&m_Browser, SIGNAL(SwitchWin_signal()), this, SLOT(SwitchToPlayWin()));
		bRet = connect(&m_Browser, SIGNAL(reqDhcpInfo_signal(QStringList &)), this, SLOT(GetDhcpInfoByDbus(QStringList &)));
		bRet = connect(&m_Browser, SIGNAL(saveDhcpInfo_signal(QStringList ,bool)), this, SLOT(SetDhcpInfoByDbus(QStringList,bool )));
		bRet = connect(&m_Browser, SIGNAL(SetVideoPar_signal(QString ,int )), this, SLOT(SetVideoParInfoByDbus(QString ,int )));
		bRet = connect(&m_Browser, SIGNAL(reqNetWorkInfo_signal(QStringList & )), this, SLOT(GetNetWorkInfo(QStringList &)));
		bRet = connect(&m_Browser, SIGNAL(SetSystemCurTime_Sig(QStringList  )), this, SLOT(SetSysTimeInfo(QStringList )));
		bRet = connect(&m_Browser, SIGNAL(GetSysCurrTimeZone_Sig(int &)), this, SLOT(GetSysTimeZone(int &)));
		bRet = connect(&m_Browser, SIGNAL(GetMonitorTypeInfo_Sig(QStringList &)), this, SLOT(GetMonitorTypeInfoByDbus(QStringList &)));
#if 1/*added by su 2013-06-18*/
		bRet = connect(&m_Browser, SIGNAL(SIGNAL_Evt_ChangeLanguage(int)), this, SLOT(On_ChangeLanguageDone(int)));
#endif
		bRet = connect(&m_playWin, SIGNAL(On_KeyMenu_Sig(int)), this, SLOT(On_DefKey_Down(int)));
		bRet = connect(&m_playWin, SIGNAL(On_KeyMenuLongPress_Sig(int)), this, SLOT(setPlayModeNormal(int))); 
#if 1 /*Add by xuweiwei 2014-3-6*/
              bRet =connect(&(*m_MediaPlayerCtrl_sp), SIGNAL(onChangeVolume_signal(int&)), &(*m_QPowerMateClient_sp), SLOT(setPowerMateBrightness(int&)));
#endif
		if(false == bRet)
		{
			LOG_BLINE("ConnectSig err\n");
		}

		{
			m_playWin.showMaximized();
			if( m_bInValidDefaultStatrUpItem)//m_strDefaultStartupItem == "" ||
			{
				ShowBrowserWindow();
			}
		}

		if(m_FristEntryIsHtml)
		{
			m_PlayPicAndHtmlWin = m_playWin.getPlayHtmlOrPicWin();
			//ShowPlayPicOrHtmlWindow();
		}

		bRet = connect(&m_CalcVideoPosTimer, SIGNAL(timeout()), this, SLOT(UpdateVideoPosition()));
		if(true == bRet)
		{
		}
		else
		{
			LOG_BLINE("connect failed\n");
		}

		bRet = connect(&m_HideVolumeBarTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkVolumeWin()));
		if(true == bRet)
		{
		}
		else
		{
			LOG_BLINE("connect failed\n");
		}


		bRet = connect(&m_OsdTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkMenuOsdTimeOut()));
		bRet = connect(&m_PlayDurationTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkPlayDurationTimeOut()));
		bRet = connect(&m_PlaylistDurationTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkPlaylistDurationTimeOut()));
		bRet = connect(&m_HtmlDurationTimer, SIGNAL(timeout()), this, SLOT(On_Timer_checkHtmlDisPlayTimeOut()));
		bRet = connect(&m_PlayIdlePlayList_Timer, SIGNAL(timeout()), this, SLOT(PlayIdlePlayListFile()));
		bRet = connect(&m_PlayPluginListEntryDurationTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkPlayPluginEntryDurationTimeOut()));
		bRet = connect(&m_PlayPluginListHtmlEntryDurationTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkPlayPluginHtmlEntryDurationTimeOut()));
		bRet = connect(&m_BootSplashTimer, SIGNAL(timeout()), this, SLOT(OnTimer_checkBootSplashTimeOut()));

		if(true == bRet)
		{
		}
		else
		{
			LOG_BLINE("connect failed\n");
		}
		bRet = connect(&m_SaveMediaVirCh_Timer, SIGNAL(timeout()), this, SLOT(SaveMediaVirChInfoCfg()));
		if(true == bRet)
		{
		}
		else
		{
			LOG_BLINE("ConnTimerSig(SaveMediaVirCh) err\n");
		}

		mWebBrowserSrv_sp->start();
		//DhcpDone2 signal hook
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_SysEvt,
			DbusIf_SysSrv_SysEvt,SIGNAL_Evt_DhcpDone2, this,
			SLOT(On_DhcpDone2(const QString &,const QString &,const QString &,const QString &,const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.DhcpDone2) err\n");
		}
		//Connect the dbus signal SystemService.SystemEvent.Evt_SyncCacheToStorage
		bRet = m_DBusConn_sp->connect(SS_DBUS_SERVICE_NAME, DBUS_PATH_SysSrv_SysEvt, DbusIf_SysSrv_SysEvt,
			SIGNAL_Evt_SyncCacheToStorage, this, SLOT(SyncCacheToStorage()));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.SyncCacheToStorage) err\n");
		}

#if 1/*added by lxj 2012-12-7 for touch screent calibration done signal*/
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_SysEvt, DbusIf_SysSrv_SysEvt,
			SIGNAL_Evt_TouchScreenCalibrationDone, this, SLOT(On_TouchScreenCalibrationDone()));
		if(false == bRet){
			LOG_BLINE("ConnDbusSig(SysEvt.TouchScreenCalibrationDone) error\n");
		}else{
			//LOG_BLINE("ConnDbusSig(SysEvt.TouchScreenCalibrationDone) ok\n");
		}
#endif

		//Connect the dbus signal WebBrowser.DataCommSrv.DownloadFinished
		bRet = m_DBusConn_sp->connect(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv, DBUS_IF_WebBrowser_DataCommSrv,
			"DownloadFinished", this, SLOT(On_DataCommSrv_DonwloadFinished(int, quint32, const QString &, const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(DBUS_SrvName_WebBrowser) err\n");
		}
		//Connect the dbus signal WebBrowser.DataCommSrv.NewSysCfgIsReady
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_CfgMgr, DbusIf_SysSrv_CfgMgr,
			"NewSysCfgIsReady", this, SLOT(On_NewSysCfgIsReady(const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.SyncCacheToStorage) err\n");
		}
		bRet = m_DBusConn_sp->connect("" , DBUS_MPLAYER_OBJECT_PATH, DBUS_MPLAYER_OBJECT_INTERFACE,
			"VidOutModeChanged", this, SLOT(On_VidOutModeChanged(int)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.SyncCacheToStorage) err\n");
		}

		//Connect the dbus signal org.thtfit.GpioBtnInput.ServiceStarted
		bRet = m_DBusConn_sp->connect(DBUS_GpioBtnInput_ServiceName, DBUS_GpioBtnInput_OBJECT_PATH, DBUS_GpioBtnInput_OBJECT_INTERFACE,
			DBUS_SIG_ServiceStarted, this, SLOT(On_GpioBtnInput_Started()));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(GpioBtnInput.ServiceStarted) err\n");
		}

		bRet = connect(&m_UploadCfgTimer, SIGNAL(timeout()), this, SLOT(UploadCfg()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimerSig(UploadCfg) err\n");
		}
		bRet = connect(&m_CheckRebootTimer, SIGNAL(timeout()), this, SLOT(ReStart()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimerSig(m_CheckRebootTimer) err\n");
		}
		bRet = connect(&m_timerAutoAnnounce, SIGNAL(timeout()), this, SLOT(Send_AutoAnnounce()));
		if(false == bRet)
		{
			LOG_BLINE("ConnTimerSig(timerAutoAnnounce) err\n");
		}
		bRet = m_DBusConn_sp->connect(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv, DBUS_IF_WebBrowser_DataCommSrv,
			"StartTransfer_Sig", this, SLOT(On_StartTransferSlot(QString ,QString ,int )));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.StartTransfer_Sig) err\n");
		}
		//Connect the dbus signal WebBrowser.DataCommSrv.DownloadFinished
		/*
		<signal name=\"NotifyScanProgress\">\n"
		"			<arg name=\"PgmNum\" type=\"u\" direction=\"out\" />\n"
		"			<arg name=\"Progress\" type=\"u\" direction=\"out\" />\n"
		"		</signal>\n"
		"		<signal name=\"ShowBarForScan\">\n"
		"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
		"		</signal>\n"
		"		<signal name=\"HideBarForScan\">\n"
		"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
		"		</signal>\n"
		*/
		bRet = m_DBusConn_sp->connect("",  DBUS_MPLAYER_OBJECT_PATH, DBUS_MPLAYER_OBJECT_INTERFACE,
			"NotifyScanProgress", this, SLOT(OnNotifyScanProgress(int,int)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(Mplayer.NotifyScanProgress) err\n");
		}
		bRet = m_DBusConn_sp->connect("", DBUS_MPLAYER_OBJECT_PATH, DBUS_MPLAYER_OBJECT_INTERFACE,
			"Tuner_StartScan", this, SLOT(OnShowProgressBarForScan(int)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(Mplayer.ShowBarForScan) err\n");
		}
		bRet = m_DBusConn_sp->connect("", DBUS_MPLAYER_OBJECT_PATH, DBUS_MPLAYER_OBJECT_INTERFACE,
			"Tuner_StopScan", this, SLOT(OnHideProgressBar(int)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(Mplayer.HideBarForScan) err\n");
		}
		bRet = m_DBusConn_sp->connect("", DBUS_MPLAYER_OBJECT_PATH, DBUS_MPLAYER_OBJECT_INTERFACE,
			"NotifyTuPlayerPropmtMsg", this, SLOT(OnNotifyTunerPlayerPropmtMsg(int)));

		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(Mplayer.OnNotifyTunerPlayerPropmtMsg) err\n");
		}
#if 1 /*Add by xuweiwei 2014-3-7*/
             bRet=m_DBusConn_sp->connect(SS_DBUS_SERVICE_NAME,DBUS_PATH_PowerMateSrv, IF_NAME_SysSrv_PowerMateSrv,"on_powermate_init",&(*m_QPowerMateClient_sp),SLOT(onPowerMateOpen()));
             if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(PowerMateSrv.on_powermate_init) err\n");
		}
              bRet=m_DBusConn_sp->connect(SS_DBUS_SERVICE_NAME,DBUS_PATH_PowerMateSrv, IF_NAME_SysSrv_PowerMateSrv,"on_powermate_uninit",&(*m_QPowerMateClient_sp),SLOT(onPowerMateClose()));
              if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(PowerMateSrv.on_powermate_uninit) err\n");
		}
#endif

#if 1/*Add by xuweiwei 2014-4-9*/
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_WebBrowser_DatCommSrv, DBUS_IF_WebBrowser_DataCommSrv,"StopMedia_Sig", this, SLOT(StopMedia()));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.StartTransfer_Sig) err\n");
		}
#endif
	
		if(mp_Config->getConfigValue(INI_KEY_TrapEnable).toInt())
		{
			iRet = On_tnetReady_Sig();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		setWindowIcon(QIcon("MP7XX.ico"));
		//load autoAnnounce setting
		{
			DECLARE_CLS_STACK_BUF_STRING(strAutoAnnounceIntervalSec, 32);
			iRet = SysProp_get(SysProp_KEY_AutoAnnounceIntervalSec, OUT strAutoAnnounceIntervalSec);
			if(ERROR_SUCCESS == iRet)
			{
				m_iAutoAnnounceIntervalSec = strAutoAnnounceIntervalSec;
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				m_iAutoAnnounceIntervalSec = 0;	//disabled
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
				m_iAutoAnnounceIntervalSec = 0; //disabled
			}
			//apply
			setAutoAnnounce(m_iAutoAnnounceIntervalSec, OUT iRet);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

		//load XappVersion setting
		{
				DECLARE_CLS_STACK_BUF_STRING(strXappVersion, 32);
				iRet = SysProp_get(SysProp_KEY_XappVersion, OUT strXappVersion);
				if(ERROR_SUCCESS == iRet)
				{
					m_strXappVersion = QString(strXappVersion);
				}
				else if(ERROR_NOT_FOUND == iRet)
				{
					m_strXappVersion = "";
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
		}
		//load AudioOutPutMode
#if 1//modify by lxj 2013-2-27		
		AUDIO_OUTPUT_MODE eAudioOutputMode =(AUDIO_OUTPUT_MODE) mp_Config->getConfigValue(INI_KEY_CurrAudioOutputMode).toInt();
		setAudioOutputMode(eAudioOutputMode,iRet);
		if(iRet !=ERROR_SUCCESS)
		{
			LOG_BLINE("setAudioOutputMode(POST) err\n");
		}
		m_eAudioOutputMode = eAudioOutputMode;
#endif

		//load tuner has scan stauts
		{
			DECLARE_CLS_STACK_BUF_STRING(strTunerScanStatus, 32);
			iRet = SysProp_get(SysProp_KEY_TunerScanStatus, OUT strTunerScanStatus);
			if(ERROR_SUCCESS == iRet)
			{
				m_iTunerScanStatus = QString(strTunerScanStatus).toInt();
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				m_iTunerScanStatus = TUNER_NO_SCAN;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		bRet = connect(this, SIGNAL(PostAsyncTransaction(const MessageItem_SharedPointer_t &)),
			&(*m_BackendTransactionThread_sp), SLOT(On_AsyncTransaction(const MessageItem_SharedPointer_t &)),
			Qt::QueuedConnection);
		if(false == bRet)
		{
			LOG_BLINE("ConnAsyncTransactionSig(POST) err\n");
		}
		bRet = connect(&(*m_BackendTransactionThread_sp), SIGNAL(AsyncTransactionFinished(const MessageItem_SharedPointer_t &)),
			this, SLOT(On_AsyncTransactionFinished(const MessageItem_SharedPointer_t &)),
			Qt::QueuedConnection);
		if(false == bRet)
		{
			LOG_BLINE("ConnAsyncTransactionSig(EVENT) err\n");
		}
		m_BackendTransactionThread_sp->start();

		//Initialize m_uiCurEthIpV4Addr
		{
			DECLARE_CLS_STACK_BUF_STRING(strCurEthIp, 32);
			iRet = SysProp_get(SysProp_KEY_CurEthernetIP, OUT strCurEthIp);
			if(ERROR_SUCCESS == iRet)
			{
				iRet = getIPv4AddrFromStr(strCurEthIp, &m_uiCurEthIpV4Addr);
				if(ERROR_SUCCESS != iRet)
				{
					m_uiCurEthIpV4Addr = 0;
					LOG_BLINE("ip %s err %d\n", qPrintable(m_strIPv4Addr), iRet);
				}
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				m_uiCurEthIpV4Addr = 0;
			}
			else
			{

			}
		}

		//Send load SoftKeyBoard
		{
			BOOL_t bDevIsExists = false;
			iRet = QTouchScreen::ChkDevExistence(OUT bDevIsExists);
			if(ERROR_SUCCESS == iRet)
			{
				//LOG_BLINE("bDevIsExists=%d\n", bDevIsExists);
				if(bDevIsExists)
				{
					#if 1/*by_su for Language index from SystemConfig*/
						QString strLanguageIndex; 
						strLanguageIndex = mp_Config ->getConfigValue(KEY_Language_Type);
						//LOG_BLINE("Current Language index = %s\n", qPrintable(strLanguageIndex));
						if(strLanguageIndex.trimmed() != "")
						{
							On_ChangeLanguageDone(strLanguageIndex.toInt());
						}
					#endif
#if USE_QCOPCHANNEL
					if(!QCopChannel::send(QTSERVER_CopCh_Name, CopChMsg_setImeEnglish))
					{
						LOG_BLINE("QCopChannel send  (CopChMsg_setImeEnglish) err\n");
					}
					else
					{
						//LOG_BLINE("QCopChannel send  (Success)\n");
					}
#endif
				}
				else
				{
					//LOG_BLINE("ChkDevExistence (TouchScreen Dev)  not exists\n");
				}
			}
			else
			{
				LOG_BLINE("ChkDevExistence (TouchScreen Dev) err\n"); 
			}
		}		
	}while(FALSE);
    //ShowBrowserWindow();

	return iOutRet;
}

INT_t QWebBrowserApp::ExitInstance()
{
	INT_t iRet;
	bool bRet;

	if(FALSE == m_BackendTransactionThread_sp.isNull())
	{
		if(m_BackendTransactionThread_sp->isRunning())
		{
			m_BackendTransactionThread_sp->quit();
			bRet = m_BackendTransactionThread_sp->wait(15*1000);
			if(false == bRet)
			{
				LOG_BLINE("Waiting BackendTransactionThread to exit timeout\n");
			}
		}
	}

	m_WbStatusBar_sp.clear();

	bRet = m_DBusConn_sp->unregisterService(DBUS_SrvName_ControlCenter);
	if(false == bRet)
	{
		LOG_BLINE("Failed to unreg dbus srv %s\n", DBUS_SrvName_ControlCenter);
	}

	if(m_QDbusIfUiSrv_sp)
	{
		m_QDbusIfUiSrv_sp->setParentObj(NULL);
		if(m_DBusConn_sp->isConnected())
		{
			m_DBusConn_sp->unregisterObject(DBUS_PATH_WebBrowser_UiSrv);
		}
	}

	if(mWebBrowserSrv_sp->isRunning())
	{
		mWebBrowserSrv_sp->exit();
		mWebBrowserSrv_sp->quit();
		bRet = mWebBrowserSrv_sp->wait(8*1000);
		if(false == bRet)
		{
			LOG_BLINE("Waiting DataCommSrv to exit timeout\n");
		}
	}

	if(m_DBusConn_sp->isConnected())
	{
		QDBusConnection::disconnectFromBus(QDBUS_CONN_NAME_WB_UI);
	}

	if(mp_Config)
	{
		mp_Config->close_file();
	}

	if(mp_AppInfo_Config)
	{
		mp_AppInfo_Config->close_file();
	}

	CloseGpioBtnInputFile();

	mMediaVirChannel_sp.clear();
	m_sckAutoAnnounce_sp.clear();

#if 1/*added by lxj 2012-12-11*/
	if(!m_TouchScreenCalibrationProcess_sp.isNull()){
		closeTouchScreenCalibrate();
		m_TouchScreenCalibrationProcess_sp.clear();
	}
#endif

	iRet = m_GuiOsd2Ctrl.releaseGuiOsd2();
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	return ERROR_SUCCESS;
}

bool QWebBrowserApp::event(QEvent * pEvent)
{
	Q_UNUSED(pEvent);
	//LOG_BLINE("QWebBrowserApp::event\n");
	return QApplication::event(pEvent);
}

bool QWebBrowserApp::notify(QObject *receiver, QEvent *event)
{
	bool bOutRet = false;	//not implemented
	INT_t iRet;
	QWidget * pActiveWindow = NULL;
	static 	INT_t Flag=0;
	//LOG_BLINE("QWebBrowserApp::notify enter\n");
#if 0/*added by lxj 2012-11-14*/
	DBG_TIME("%s() begin\n",__func__);
#endif

	do
	{
		if(QEvent::MouseButtonPress == event->type())
		{
			if(m_OsdTimer.isActive())
			{
				m_OsdTimer.stop();
			}
			m_OsdTimer.start(MENUOSDTIMEOUT_MS);
		}
		else if(QEvent::KeyPress == event->type())
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			//LOG_BLINE("KeyPress,KeyCode=0x%08x\n", keyEvent->key());
			if(m_OsdTimer.isActive())
			{
				m_OsdTimer.stop();
			}
			m_OsdTimer.start(MENUOSDTIMEOUT_MS);
			if(m_PlayIdlePlayList_Timer.isActive())
			{
				m_PlayIdlePlayList_Timer.stop();
				m_iIdleTime = 0;
				if(m_iStateType == State_Idle)
				{
					if(m_TunerScanStatus == TunerScanning)
					{					
						break;
					}
					startIdelDetectTimer();
				}
			}
			if(m_bIsPlayingIdlePlayList)
			{
				m_bIsPlayingIdlePlayList = false;
				m_PlaylistDataA.ClearPlayListData();
				m_iIdleTime = 0;
				m_PlaybackId = 0;
				StopPlayback();
			}
			pActiveWindow = activeWindow();
			if(NULL == pActiveWindow)
			{
				break;
			}			
			if(IR_KEY_STOP == keyEvent->key())
			{
				m_PlaybackId = 0; //must the same to mediaPlay
				if(&m_playWin == pActiveWindow && (m_PlayPicAndHtmlWin->isPlayingHtml()))
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
				}
				m_HtmlDurationTimer.stop();
				ShowBrowserWindow();
				if(m_bIsPlayPluginList)
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
					StopPlayPluginList();
				}	
				iRet = StopPlayback();
				bOutRet = true;	//already processed
				m_PlaylistDataA.ClearPlayListData();
				m_ChannelNumber = "";
				if(m_bIsPlayVideoLayer)
				{
					m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
					m_playWin.getVideoLayerWnd()->hide();
					m_playWin.getVideoLayerWnd()->lower();
				}
				m_bIsPlayVideoLayer = false;	
				m_strCurrPlayListName = "";
				m_strCurrPlayListUrl = "";

				m_strCurrPluginListName ="";
				m_strCurrPluginListUrl = "";
#if 0/*added by lxj 2012-11-14*/
				DBG_TIME("%s() IR_KEY_STOP 2\n",__func__);
#endif
			}
			else if(IR_KEY_DOWN == keyEvent->key() || IR_KEY_UP == keyEvent->key())
			{
				On_DefKey_Down(keyEvent->key());
				m_ChannelNumber = "";
			}
			else if(IR_KEY_VOLUMETURNUP == keyEvent->key() || IR_KEY_VOLUMETURNDOWN == keyEvent->key())
			{
				m_HideVolumeBarTimer.stop();
				INT32_t VolumeChangeVal,CurVolume;
				m_MediaPlayerCtrl_sp->getCurrentVolume(&CurVolume);
				if(IR_KEY_VOLUMETURNUP == keyEvent->key()) VolumeChangeVal = 1;
				else VolumeChangeVal = -1;
				CurVolume += VolumeChangeVal;
				if(0 > CurVolume) CurVolume = 0;
				else if(100 < CurVolume) CurVolume = 100;
				if(0 > CurVolume) CurVolume = 0;
				else if(100 < CurVolume) CurVolume = 100;
				
				//LOG_BLINE("CurVolume = %d\n",CurVolume);
				ChangeVolume(CurVolume);
				bOutRet = true; //already processed
				m_ChannelNumber = "";
			}
			else if(IR_KEY_CHANGERESOLUTION== keyEvent->key())
			{
				if(&m_playWin != pActiveWindow)
				{
				   if(Flag==0)
				   	{
				   	Flag++;
					iRet = switchToNextVideoOutputMode();
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					else
					{
						QString strVoModeDescForLedDisp;
						MP7XX::QAudioVideoInOutInfo::getVoDescForLedDisp(m_iVideoOutMode, OUT strVoModeDescForLedDisp);
						if(!m_MediaPlayerCtrl_sp.isNull())
						{
							m_MediaPlayerCtrl_sp->DispOnLedScrAsync(strVoModeDescForLedDisp, 1);
						}
						BOOL_t bEnableWriteLoggin = TRUE;
						iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
						if(ERROR_SUCCESS != iRet)
						{
							break;
						}
						if(bEnableWriteLoggin)
						{
							char strTemp[64];
							snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_ResolutionChange);
							strTemp[sizeof(strTemp)-1] = '\0';
							CString::STD_STRING_LIST strList ;
							strList.push_back(strTemp);
							strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
							QString strVoModeDesc;
							MP7XX::QAudioVideoInOutInfo::getVoDesc(m_iVideoOutMode, OUT strVoModeDesc);
							strList.push_back(strVoModeDesc.toLocal8Bit().constData());
							if(ERROR_SUCCESS != WriteEventLog(strList))
							{
								PRINT_BFILE_LINENO_CRT_ERRINFO;
							}
							else
							{
								strList.clear();
							}
						}
					}
				  }
				  else
				  {
					Flag=0;
					break;				  
				  }
				}
				m_ChannelNumber = "";
			}
			else
			{
				On_DefKey_Down(keyEvent->key());
				/*Add by xuweiwei 2014-9-15 for press "mute" button will appear two key events*/
				if(keyEvent->key() == IR_KEY_VOLMUTE)
				{
					bOutRet = true;
					break;
				}
				event->accept();
			}
		}
	}while(FALSE);

	//LOG_BLINE("QWebBrowserApp::notify leave 1,ClsName:%s,ObjName:%s,Evt.Type:%d\n",
	//	receiver->metaObject()->className(), qPrintable(receiver->objectName()), event->type());

	if(bOutRet)
	{
		event->accept();
	}
	else
	{

		bOutRet = QApplication::notify(receiver, event);
	}

	//LOG_BLINE("QWebBrowserApp::notify leave 2\n");
#if 0/*added by lxj 2012-11-14*/
	DBG_TIME("%s() end\n",__func__);
#endif

	return bOutRet;
}

CMutex2 * QWebBrowserApp::getPlaylistAccessLocker()
{
	return &m_PlaylistAccessLocker;
}


CMp7xxGlobalStatus & QWebBrowserApp::getMp7xxGlobalStatusObj()
{
	return m_Mp7xxGlobalStatusData;
}

INT_t QWebBrowserApp::getDstFilePath(const QUrl & SrcFileUrl, const QString & strFileType,
	 QString & strDstFileName, QString & strDstFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS;

	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	QFileInfo SrcPathInfo(SrcFileUrl.path());
	QString strDstTypeDirName, strDstStorageDirName,strFileName;

	strDstFilePath = "";

	do
	{
		if(SrcPathInfo.fileName() == "" || strDstFileName == "")
		{
			iOutRet = ERR_INVALID_PATH;
			break;
		}
		strDstTypeDirName = getDirNameFromFileType(strFileType);
		if(strDstTypeDirName == "")
		{
			iOutRet = ERR_INVALID_PATH;
			break;
		}
		if(m_eFormatMediaStatus == FORMAT_MEDIA_InNow)
		{
			iOutRet = ERR_INVALID_PATH;
			break;
		}

		QString strStorageDirName, strActualPlaylistFilename;

		//strStorageDirName = getStorageDirNameFromDstFileName(strPlaylistName, &strActualPlaylistFilename);
		//LOG_BLINE("ActiveStorageDirName=%s\n", qPrintable(m_strActiveStorageDirName));

		// change Reslove First char to Store Dst.
		/*
			strDstStorageDirName = getStorageDirNameFromDstFileName(strDstFileName,&strActualPlaylistFilename);
			strDstFileName = strActualPlaylistFilename;
		*/
		//ent change

		strDstStorageDirName = m_strActiveStorageDirName;
		strDstFileName = strDstFileName;


		// = m_strActiveStorageDirName;
		if(strDstStorageDirName == "")
		{
			iOutRet = ERR_INVALID_PATH;
			break;
		}
        
        if(0 != strDstStorageDirName.compare(LOCAL_STORAGE_DIR_NAME, Qt::CaseInsensitive))
        {
            
            
            QString strDirPath =MOUNTDIR + qApp->applicationDirPath() + "/" + strDstStorageDirName  ;
            
            QDir dir(strDirPath);
            if(!dir.exists())
            {
                iOutRet = ERR_NOT_EXIST;
                break;
            }
            else
            {
                QString strTypeDirPath = MOUNTDIR + qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName  ;
                if(!dir.exists(strTypeDirPath))
                {
                    dir.mkdir(strTypeDirPath);
                }
            }
            strDstFilePath = MOUNTDIR + qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName + "/" + strDstFileName;       
        }
        else
        {
        
            QString strDirPath = qApp->applicationDirPath() + "/" + strDstStorageDirName  ;
            
            QDir dir(strDirPath);
            if(!dir.exists())
            {
                iOutRet = ERR_NOT_EXIST;
                break;
            }
            else
            {
                QString strTypeDirPath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName  ;
                if(!dir.exists(strTypeDirPath))
                {
                    dir.mkdir(strTypeDirPath);
                }
            }
            strDstFilePath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName + "/" + strDstFileName;
        }
        LOG_BLINE("%s strDstFilePath is %s\n",__FUNCTION__,qPrintable(strDstFilePath));

	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::getFullFilePath(const QString & strFilename, const QString & strFileType, QString & strFullFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strDstTypeDirName, strDstStorageDirName = LOCAL_STORAGE_DIR_NAME;

	strFullFilePath = "";

	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	do
	{
		strDstTypeDirName = getDirNameFromFileType(strFileType);
		if(strDstTypeDirName == "")
		{
			iOutRet = ERR_INVALID_PATH;
			break;
		}
		QString strDirPath = qApp->applicationDirPath() + "/" + strDstStorageDirName  ;
		QDir dir(strDirPath);
		if(!dir.exists())
		{
			dir.mkdir(strDirPath);
			QString strTypeDirPath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName  ;
			dir.mkdir(strTypeDirPath);
		}
		else
		{
			QString strTypeDirPath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName  ;
			if(!dir.exists(strTypeDirPath))
			{
				dir.mkdir(strTypeDirPath);
			}
		}

		strDstStorageDirName = m_strActiveStorageDirName;
		strFullFilePath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName + "/" + strFilename;
		if(0 != m_strActiveStorageDirName.compare(LOCAL_STORAGE_DIR_NAME, Qt::CaseInsensitive))
        {
            strFullFilePath = MOUNTDIR + qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName + "/" + strFilename;        
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::getPlaylistFullPath(const QString & strPlaylistName, QString & strFullPath)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strStorageDirName, strActualPlaylistFilename;

	do
	{
		strStorageDirName = getStorageDirNameFromDstFileName(strPlaylistName, &strActualPlaylistFilename);
		if(strStorageDirName.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(0 == strStorageDirName.compare(LOCAL_STORAGE_DIR_NAME, Qt::CaseInsensitive))
		{
			strFullPath = qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
			strStorageDirName + "/" + FOLDER_NAME_PLAYLIST + "/" + strActualPlaylistFilename;
		}
		else
		{
			strFullPath = MOUNTDIR + qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
			strStorageDirName + "/" + FOLDER_NAME_PLAYLIST + "/" + strActualPlaylistFilename;
		}
		
		
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::setActiveMedia(CONST ActiveMedia_TYPE eActiveMediaType)
{
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	INT_t iOutRet = ERROR_SUCCESS;
	ActiveMedia_TYPE eOldActiveMediaType = m_eActiveMediaType;

	do
	{
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		m_eActiveMediaType = eActiveMediaType;
		if(AMedia_TYPE_FLASH == m_eActiveMediaType)
		{
			m_strActiveStorageDirName = LOCAL_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_SdCard == m_eActiveMediaType)
		{
			m_strActiveStorageDirName = SD_CARD_DIR_NAME;
		}
		else if(AMedia_TYPE_USB == m_eActiveMediaType)
		{
			m_strActiveStorageDirName = USB_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_HDD == m_eActiveMediaType)
		{
			m_strActiveStorageDirName = StorageDirName_HDD;
		}
		else if(AMedia_TYPE_RAM == m_eActiveMediaType)
		{
			m_strActiveStorageDirName = StorageDirName_RAM;
		}
		else
		{
			m_eActiveMediaType = eOldActiveMediaType;
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS == iOutRet)
	{
		mp_Config->setConfigValue(INI_KEY_CurActiveMediaType, QString::number(eActiveMediaType));
	}

	return iOutRet;
}

INT_t QWebBrowserApp::getActiveMedia(QString & strActiveMedia, QString & strActiveMediaDirName)
{
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	INT_t iOutRet = ERROR_SUCCESS;
	do
	{	
		strActiveMedia = getActiveMediaDescFromType(m_eActiveMediaType);
		strActiveMediaDirName = m_strActiveStorageDirName;
	}while(FALSE);


	return iOutRet;
}

INT_t QWebBrowserApp::getMediaFreeSpace(qulonglong & MediaFreeSpaceInBytes)
{
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	INT_t iOutRet = ERROR_SUCCESS, iRet;
	struct stat FileStat;
	struct statfs FsStat;

	MediaFreeSpaceInBytes = 0;
	QString ActiveStorageSpace=m_strActiveStorageDirName;

	if(0 != m_strActiveStorageDirName.compare("LocalMedia", Qt::CaseInsensitive))
	{
		ActiveStorageSpace= MOUNTDIR + qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") + m_strActiveStorageDirName;
	}

	do
	{
		//this step must be neede to check whether active media is inserted.
		iRet = stat(ActiveStorageSpace.toLocal8Bit(), &FileStat);
		if(0 != iRet)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;
			break;
		}

		iRet = statfs(ActiveStorageSpace.toLocal8Bit(), &FsStat);
		if(0 != iRet)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;
			break;
		}
		MediaFreeSpaceInBytes = ((typeof(MediaFreeSpaceInBytes))FsStat.f_bsize * FsStat.f_bavail);
	}while(FALSE);

	return iOutRet;
}

/*
Will save the strPlaylist as system's default
*/
void QWebBrowserApp::startPlaylist(CONST QString & strPlaylist, int & iOutRet)
{
	INT_t iRet;
	QString strPlaylistA, strPlaylistB;
	QString strPlaylistAFullPath, strPlaylistBFullPath;

	iOutRet = ERROR_SUCCESS;

	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iRet=CheckPlayEnable();
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif
		if(strPlaylist.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(Playlist_NAME_DELIMETER_CHAR == strPlaylist[0])	//have the first Playlist_NAME_DELIMETER_CHAR, %...
		{
			int DelimeterCharPos = strPlaylist.indexOf(Playlist_NAME_DELIMETER_CHAR, 1);
			if(DelimeterCharPos > 1)	//have the second Playlist_NAME_DELIMETER_CHAR, %a%...
			{
				strPlaylistA = strPlaylist.mid(1, DelimeterCharPos - 1);
				strPlaylistB = strPlaylist.mid(DelimeterCharPos + 1);
			}
			else if(1 == DelimeterCharPos)	//%%a
			{
				strPlaylistB = strPlaylist.mid(2);
			}
			else	//%ab
			{
				strPlaylistA = strPlaylist.mid(1);
			}
		}
		else	//no %
		{
			int DelimeterCharPos = strPlaylist.indexOf(Playlist_NAME_DELIMETER_CHAR);
			if(DelimeterCharPos > 0)	//a%b
			{
				strPlaylistA = strPlaylist.left(DelimeterCharPos);
				strPlaylistB = strPlaylist.mid(DelimeterCharPos + 1);
			}
			else	//no %
			{
				strPlaylistA = strPlaylist;
			}
		}

		if(FALSE == strPlaylistA.isEmpty())
		{
			iRet = getPlaylistFullPath(strPlaylistA, strPlaylistAFullPath);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		if(FALSE == strPlaylistB.isEmpty())
		{
			iRet = getPlaylistFullPath(strPlaylistB, strPlaylistBFullPath);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(pActiveWindow== &m_playWin)
		{
			m_PlayPicAndHtmlWin->playHtmlFileEnd();
			UpdateVideoPosition(true);
		}
		iRet = OpenPlaylist(strPlaylistAFullPath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(m_bIsPlayingIdlePlayList)
		{
			m_bIsPlayingIdlePlayList = false;
		}
		iRet = SavePlaylistAsPersistent(strPlaylistAFullPath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

/*
MainThread context
*/
void QWebBrowserApp::playMedia(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType,
	CONST QString & strFilename, int & iOutRet, int iLoopTimes,int iDurationTime, int iPositionTime,const QString & strOriginUrl/* = QString()*/
	,bool isPlayNextFile ,bool bIsVideoLayer)
{
	INT_t iRet;
	QString strDevMediaDirName, strFileTypeDirName, strFileFullPath;
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	iOutRet = ERROR_SUCCESS;

	if(!bIsVideoLayer)
	{
		m_strCurrPlayListName = "";
		m_strCurrPlayListUrl = "";

		m_strCurrPluginListName ="";
		m_strCurrPluginListUrl = "";
	}
	
	/*LOG_BLINE("iLoopTimes=%d,iDurationTime=%d,iPositionTime=%d,strOriginUrl=%s,eDeviceType:%d,bIsVideoLayer:%d, 
		strFilename:%s,strFileType:%s,isPlayNextFile:%d\n",
		iLoopTimes,iDurationTime,iPositionTime,qPrintable(strOriginUrl),eDeviceType,bIsVideoLayer,
		qPrintable(strFilename),qPrintable(strFileType),isPlayNextFile);*/

	do
	{
		if(strFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		if(m_PlayIdlePlayList_Timer.isActive())
		{
			m_PlayIdlePlayList_Timer.stop();
			m_iIdleTime = 0;
		}
		if(m_bIsPlayingIdlePlayList)
		{
			m_bIsPlayingIdlePlayList = false;
			m_iIdleTime = 0;		//StopPlayback();
			//m_PlaylistDataA.ClearPlayListData();
		}		
		if(m_bIsPlayPluginList)
		{
			if(m_PluginPlaylistData_sp)
			{
				//m_PluginPlaylistData_sp->ClearPlayListData();
				m_bIsPlayPluginList = false;
			}
		}
		/**/
		if(!bIsVideoLayer)
		{
			if(m_bIsPlayVideoLayer)
			{
				StopPlayPluginList();
				m_bIsPlayVideoLayer = false;
				m_bIsPlayPluginList = false;
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
			}
		}
		
		m_BIsPlaySplash = false;
		if( eDeviceType == AMedia_TYPE_URL)
		{
			if(0 == strFileType.compare(FileType_Http, Qt::CaseInsensitive))
			{
				//http media file?
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(pActiveWindow== &m_playWin)
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
					UpdateVideoPosition(true);
				}
				URL_TYPE eUrlType = TYPE_URL_UNKNOWN;
				QString strUrl;
				if(FALSE == strOriginUrl.isEmpty())
				{
					strUrl = strOriginUrl;
				}
				else
				{
					strUrl = strFilename;
				}
				eUrlType = DetermineUrlType(strUrl.toUtf8(), NULL, 0);
				LOG_BLINE("Url:%s,UrlType=%d\n", qPrintable(strUrl), eUrlType);
				if(TYPE_URL_HTTP_MEDIA_FILE == eUrlType)
				{
					m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
					m_iStateType = State_Streaming;
					iRet = StopPlayback();
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
					m_iStateType = State_Streaming;
					StopAllPlayDurationTimer();
					iOutRet = PlayUrl(strUrl);
					if(ERROR_SUCCESS != iOutRet)
					{
						break;
					}
					else
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
					break;	//already sent to MediaPlayer
				}
			}
			if( 0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Http, Qt::CaseInsensitive) )
			{
				iOutRet = stopMPlayerWithEventLog();
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				m_PlaybackId = 0;
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_PlayingBrowserFile;
				StopAllPlayDurationTimer();
				if(iDurationTime <= 0 )
				{
					iOutRet = CallPlayWebpage(strFilename,0,true);
					if(ERROR_SUCCESS == iOutRet)
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
				}
				else
				{
					iOutRet = CallPlayWebpage(strFilename,iDurationTime);
					if(ERROR_SUCCESS == iOutRet)
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
				}
			}
			else if(0 == strFileType.compare(FileType_Udp, Qt::CaseInsensitive) ||
				0 == strFileType.compare(FileType_Rtsp, Qt::CaseInsensitive) ||
				0 == strFileType.compare(FileType_Rtp, Qt::CaseInsensitive))
			{
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_Streaming;
				StopPlayback();
				m_iStateType = State_Streaming;
				StopAllPlayDurationTimer();
				iOutRet = PlayUrl(strFilename);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				else
				{
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
				}
				break;
			}
			break;
		}

#if 1/*added by lxj 2012-9-11*/
		if( eDeviceType == AMedia_TYPE_CAPTURE){
			if(0 == strFileType.compare(FileType_Capture_USB, Qt::CaseInsensitive)){
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_USB_Capture;
				MediaServer_StopAudio();
				StopPlayback();
				m_iStateType = State_USB_Capture;
				iOutRet = PlayUrl(strOriginUrl);
				if(ERROR_SUCCESS != iOutRet){
					break;
				}else{
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
				}
				break;
			}
			else if(0 == strFileType.compare(FileType_Capture_HDMI, Qt::CaseInsensitive))/*added by abner 2014-11-3*/
			{
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_HDMI_Capture;
				MediaServer_StopAudio();
				StopPlayback();
				m_iStateType = State_HDMI_Capture;
					 
				iOutRet = PlayUrl(strOriginUrl);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				else
				{
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
				}
				
				bool bFullScreen = false;//true;/*zoom up or down*/
				if(bFullScreen)
				{
					UpdateVideoPosition(bFullScreen);
				}
				else
				{
					UpdateVideoPosition();
					ShowBrowserWindow();
				}
				
				break;
			}
			break;
		}
#endif
		strDevMediaDirName = getDevMediaDirNameByType(eDeviceType);
		strFileTypeDirName = getDirNameFromFileType(strFileType);
		if(strDevMediaDirName.isEmpty() || strFileTypeDirName.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(eDeviceType==AMedia_TYPE_FLASH)
		{
			strFileFullPath = qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
				strDevMediaDirName + "/" + strFileTypeDirName + "/" + strFilename;
		}
		else
		{
			strFileFullPath = MOUNTDIR + qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
				strDevMediaDirName + "/" + strFileTypeDirName + "/" + strFilename;
		}
		
		if( SNMP_DEFAULT_PLAY_DURATION_TIME == iDurationTime)
		{
			iDurationTime = DEFAULT_PLAY_DURATION_TIME;
		}
		if(!QFile::exists(strFileFullPath))
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}
		if(!bIsVideoLayer)
		{
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(pActiveWindow== &m_playWin)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				UpdateVideoPosition(true);
			}
		}
		if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
		{
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
			if(QFile::exists(strFileFullPath))
			{	
				bool bFullScreen = false;
				if(!bIsVideoLayer)
				{
					StopPlayback();	
					StopAllPlayDurationTimer();
					QWidget * pActiveWindow = NULL;
					pActiveWindow = activeWindow();					
					if(&m_playWin == pActiveWindow && (m_PlayPicAndHtmlWin->isPlayingHtml()))
					{
						bFullScreen = true;
						m_PlayPicAndHtmlWin->playHtmlFileEnd();					
						UpdateVideoPosition(bFullScreen);					
					}
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
				}
				if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive))
				{
					m_eCurPlayMediaFileType = AMedia_TYPE_VIDEO;
					m_iStateType = State_PlayingVideoFile;
					if(bFullScreen)
					{
						if(!m_MediaPlayerCtrl_sp.isNull())
						{
							iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Normal);
							if(ERROR_SUCCESS != iRet)
							{
								PRINT_BFILE_LINENO_IRET_STR;
							}
						}
					}
				}
				else if(0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive))
				{
					m_eCurPlayMediaFileType = AMedia_TYPE_AUDIO;
					m_iStateType = State_PlayingAudioFile;
				}
				else if(0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
				{
					m_eCurPlayMediaFileType = AMedia_TYPE_PICTURE;
					m_iStateType = State_PlayingPictureFile;
				}

				if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
				{
					if(iLoopTimes ==  0)
					{
						iOutRet = PlayUrl(strFileFullPath);
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						} 
						m_NextFileInfoToPlay.m_bIsInitZero = true;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;
						playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
						if(iOutRet == ERROR_SUCCESS)
						{
							if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
							{
								int iTimerTimeoutMs = (iDurationTime * 1000);
								if(0 < iTimerTimeoutMs)
								{
									m_PlayDurationTimer.start(iTimerTimeoutMs);
								}
							}
						}
						
						if(0 < iPositionTime)
						{
							palySpecificTime(iPositionTime);
						}
						break;
					}
					else   if(iLoopTimes == 1)
					{
						iOutRet = PlayUrl(strFileFullPath);
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
						m_NextFileInfoToPlay.m_bIsInitZero = false;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;
						
						iLoopTimes --;
						playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
					}
					else if(iLoopTimes >1)
					{
						iOutRet = PlayUrl(strFileFullPath);
						iLoopTimes --;
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
						m_NextFileInfoToPlay.m_bIsInitZero = false ;
						m_NextFileInfoToPlay.m_bIsInitOne  = true;
						playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
					}
					else
					{
						m_NextFileInfoToPlay.m_bIsInitZero = FALSE;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;
						if(!bIsVideoLayer)
						{
							StopPlayDurationTimer();
						}

#if 1					/*modify by lxj 2012-7-23 fixed play twice bug*/
						iOutRet = PlayUrl(strFileFullPath);//iLoopTimes
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
#endif
					}

					if(iOutRet == ERROR_SUCCESS)
					{
						if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
						{
							if(!bIsVideoLayer)
							{
								StopPlayDurationTimer();
								StopAllPlayDurationTimer();
							}
							int iTimerTimeoutMs = (iDurationTime * 1000);
							if(0 < iTimerTimeoutMs)
							{
								m_PlayDurationTimer.start(iTimerTimeoutMs);
							}
						}
					}
				}
				else
				{
					//iOutRet = PlayUrl(strFileFullPath,iLoopTimes);
					iOutRet = PlayUrl(strFileFullPath);	
					if(ERROR_SUCCESS != iOutRet)
					{
						StopPlayDurationTimer();
						break;
					}
					else
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
				//	m_NextFileInfoToPlay.m_bIsInitZero = false ;
				//	m_NextFileInfoToPlay.m_bIsInitOne  = false;
#if 1 //Added by jaylon  for loop  				
				if(0 == iLoopTimes)
				{
					m_NextFileInfoToPlay.m_bIsInitZero = true ;
					m_NextFileInfoToPlay.m_bIsInitOne  = false;
				}
				else
#endif
				{
					m_NextFileInfoToPlay.m_bIsInitZero = false ;
					m_NextFileInfoToPlay.m_bIsInitOne  = false;
				}

				
					playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
				}

				if(0 < iPositionTime)
				{
					palySpecificTime(iPositionTime);
				}
			}
		}
		else if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive))
		{
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(&m_playWin == pActiveWindow && (m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
			}
			m_PlaylistDataA.ClearPlayListData();
			StopAllPlayDurationTimer();
			// 是否要保存为curPlayList
			//mp_Config->setConfigValue(INI_KEY_CurPlaylistAFilePath, strFileFullPath);
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlayPlayList;
			m_eCurPlayMediaFileType = AMedia_TYPE_PLAYLIST;
			iRet =  m_PlaylistDataA.LoadFromIniFile(&strFileFullPath);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				iOutRet = iRet;
				break;
			}
			else
			{
				QFileInfo fileInfo(strFileFullPath);
				m_strCurrPlayListUrl = strFileFullPath;
				m_strCurrPlayListName = fileInfo.fileName();	;
			}
			m_bIsNeedLoop = false;
			PlayNextEntry(true);
		}
		else if(0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive))
		{
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
			m_iStateType = State_PlayingBrowserFile;
			m_PlaybackId = 0;
			StopPlayback();
			StopAllPlayDurationTimer();
			iLoopTimes = 0;
			if(iDurationTime <= 0 )
			{
				iOutRet = CallPlayWebpage(strFileFullPath,0,true);
				if(ERROR_SUCCESS == iOutRet)
				{
					m_PlaylistDataA.ClearPlayListData();
				}
			}
			else
			{
				iOutRet = CallPlayWebpage(strFileFullPath,iDurationTime);
				if(ERROR_SUCCESS == iOutRet)
				{
					m_PlaylistDataA.ClearPlayListData();
				}
			}
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
	}while(FALSE);
}

/*
MainThread context
*/
void QWebBrowserApp::playNextFile(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType,
	CONST QString & strFilename, int & iOutRet, int iLoopTimes ,int iDurationTime ,int iPositionTime,bool bIsVideoLayer)
{
	do
	{
		if(0 == m_PlaybackId)	//no pending playback task
		{
			playMedia(eDeviceType, strFileType, strFilename, iOutRet,iLoopTimes,iDurationTime,iPositionTime,"",true,bIsVideoLayer);
		}
		else	//should queue the media file to play on the next time
		{
			QString strDevMediaDirName, strFileTypeDirName, strFileFullPath;
			//calc the file full path first
			if(strFilename.isEmpty())
			{
				iOutRet = ERR_INVALID_FILENAME;
				break;
			}
			if(AMedia_TYPE_URL == eDeviceType)
			{
				QAutoLock AutoRangeLocker(&m_SharedDataMutex);
				strDevMediaDirName = getDevMediaDirNameByType(eDeviceType);
				strFileTypeDirName = getDirNameFromFileType(strFileType);
				strDevMediaDirName = LOCAL_STORAGE_DIR_NAME;
				if(strDevMediaDirName.isEmpty() || strFileTypeDirName.isEmpty())
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					//break;
				}
				iOutRet = QueueNextFileToPlay(strFilename, strFileType,eDeviceType,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
			}
			else
			{
				{
					QAutoLock AutoRangeLocker(&m_SharedDataMutex);
					strDevMediaDirName = getDevMediaDirNameByType(eDeviceType);
					strFileTypeDirName = getDirNameFromFileType(strFileType);
					strDevMediaDirName = LOCAL_STORAGE_DIR_NAME;
					if(strDevMediaDirName.isEmpty() || strFileTypeDirName.isEmpty())
					{
						iOutRet = ERROR_INVALID_PARAMETER;
						break;
					}
					strFileFullPath = qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
						strDevMediaDirName + "/" + strFileTypeDirName + "/" + strFilename;
				}
				iOutRet = QueueNextFileToPlay(strFilename, strFileType,eDeviceType,iLoopTimes,iDurationTime,iPositionTime,bIsVideoLayer);
			}
		}
	}while(FALSE);
}

/*add the fifo of the temporary play item.*/
void QWebBrowserApp::setItemTemporary(QString strFilePath)/*IN*/
{
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	//LOG_BLINE("m_PlayItemlist.isEmpty() = %d\n",m_PlayItemlist.isEmpty());
	m_PlayItemlist.append(strFilePath);
	m_bIsHavePlayItemTemp = true;
}

void QWebBrowserApp::PlayItemTemporary()/*OUT*/
{
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	int iOutRet = ERROR_SUCCESS;
	QString strFilePath;
	//LOG_BLINE("m_PlayItemlist.size() = %d\n",m_PlayItemlist.size());
	if(!m_PlayItemlist.isEmpty())
	{
		strFilePath = m_PlayItemlist.takeFirst();
		parsePlayMediaCommad(strFilePath,iOutRet,playNextFile_MODE);
		if(m_PlayItemlist.empty())
		{
			m_bIsHavePlayItemTemp = false;
		}
	}
}

void QWebBrowserApp::setMediaPlayType(AMedia_File_TYPE type)
{
	if( type < AMedia_TYPE_VIDEO || AMedia_TYPE_MAX < type)
	{
		return ;
	}
	m_eCurPlayMediaFileType = type;
}

void QWebBrowserApp::setVideoPosition(int x,int y,int width,int height, bool bFullScreen/* = false*/)
{
	INT_t iRet;
	QDesktopWidget * pDesktopWidget = desktop();
	iRet = 0;
    //LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,width,height);
	do
	{
        int iDeskWidth=0,iDeskHeight=0;
		if(NULL == pDesktopWidget)
		{
			break;
		}
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		QRect DesktopRect = pDesktopWidget->screenGeometry();
		int HardwareX, HardwareY, HardwareWidth, HardwareHeight;
		//LOG_BLINE("m_iVideoOutMode :%d,iCurrOutPutMode:%d\n",m_iVideoOutMode,iCurrOutPutMode);
        
        iDeskWidth=DesktopRect.width();
        iDeskHeight=DesktopRect.height();
        //LOG_LINE("%s->>desk (%d,%d)\n",__FUNCTION__,iDeskWidth,iDeskHeight);
        m_MediaPlayerCtrl_sp->setOsdSize(iDeskWidth,iDeskHeight);

		/*-------add by xuweiwei 2013-11-12 To Make 92 video mode always full screen------------*/
		if(bFullScreen || MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iVideoOutMode)
		{
			HardwareX = 0;
			HardwareY = 0;
			HardwareWidth = VideoSurface_WIDTH;
			HardwareHeight = VideoSurface_HEIGHT;
		}
		else
		{
			HardwareX = x * VideoSurface_WIDTH / DesktopRect.width();
			HardwareY = y * VideoSurface_HEIGHT / DesktopRect.height();
			HardwareWidth = width * VideoSurface_WIDTH / DesktopRect.width();
			HardwareHeight = height * VideoSurface_HEIGHT / DesktopRect.height();
		}

		if(!m_MediaPlayerCtrl_sp.isNull())
		{			
			iRet = m_MediaPlayerCtrl_sp->setVideoPosition(HardwareX, HardwareY, HardwareWidth, HardwareHeight);
		}
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);
}

void  QWebBrowserApp::getPlayListFiles(int media_type,QStringList & PlayListFiles,QStringList & PlayfileInfoList)
{
	m_StrUsbListDir = "";
	m_strPlayListDir = "";
	m_StrSdListDir = "";
	m_StrUsbListDir_2 = "";
	QStringList  playFileFilter; // 固定目录是否可不用过滤


	if(media_type == AMedia_TYPE_PLAYLIST)
	{
		if(qApp->applicationDirPath() ==  "/")
		{
			 m_strPlayListDir = qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST +"/";
			 m_StrUsbListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST +"/";
			 m_StrUsbListDir_2 = MOUNTDIR +qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST +"/";
			 m_StrSdListDir  = MOUNTDIR +qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST +"/";
			 m_StrHddListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_PLAYLIST +"/";
		}
		else
		{
			m_strPlayListDir =  qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST ;
			m_StrUsbListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST ;
			m_StrSdListDir  = MOUNTDIR +qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST ;
			m_StrUsbListDir_2 = MOUNTDIR +qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST ;
			m_StrHddListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_PLAYLIST ;
		}
		playFileFilter << "*"; //		没有扩展名 		文件大小小于64KB
	}
	else if(media_type == AMedia_TYPE_VIDEO)
	{
		if(qApp->applicationDirPath() ==  "/")
		{
			 m_strPlayListDir = qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO +"/";
			 m_StrUsbListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO+"/";
			 m_StrSdListDir  = MOUNTDIR +qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_VIDEO +"/";
			 m_StrUsbListDir_2 = MOUNTDIR +qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO +"/";
			 m_StrHddListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_VIDEO +"/";
		}
		else
		{
			m_strPlayListDir = qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO ;
			m_StrUsbListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO ;
			m_StrSdListDir  = MOUNTDIR + qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_VIDEO ;
			m_StrUsbListDir_2 = MOUNTDIR + qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_VIDEO ;
			m_StrHddListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_VIDEO ;
		}

		playFileFilter << "*.ts" <<"*.mpg" << "*.mpeg" <<"*.asf" << "*.avi" << "*.tp"
			<<"*.mp4" << "*.mkv" << "*.264"<<"*.263" << "*.wmv"<<"*.webm"
			<< "*.TS" <<"*.MPG" << "*.MPEG" <<"*.ASF" << "*.AVI" << "*.TP"
			<<"*.MP4" << "*.MKV" << "*.wmv" << "*.WMV"  << "*.vob" << "*.VOB"<< "*.mov"<< "*.mxf"<<"*.webm";
	}
	else if(media_type == AMedia_TYPE_AUDIO)
	{
		if(qApp->applicationDirPath() ==  "/")
		{
			  m_strPlayListDir = qApp->applicationDirPath() + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_MUSIC +"/";
			  m_StrUsbListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_MUSIC+"/";
			  m_StrSdListDir  = MOUNTDIR + qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_MUSIC +"/";
			  m_StrUsbListDir_2 = MOUNTDIR + qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_MUSIC +"/";
			  m_StrHddListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_MUSIC +"/";
		}
		else
		{
			m_strPlayListDir = qApp->applicationDirPath()+"/" + LOCAL_STORAGE_DIR_NAME "/" FOLDER_NAME_MUSIC;
			m_StrUsbListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_MUSIC;
			m_StrSdListDir  = MOUNTDIR + qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_MUSIC ;
			m_StrUsbListDir_2 = MOUNTDIR + qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_MUSIC ;
			m_StrHddListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_MUSIC;
		}
		playFileFilter <<"*.wma" << "*.mp3" <<"*.wav" <<"*.ac3" <<"*.aac" << "*.flac"
			<<"*.WMA" << "*.MP3" <<"*.WAV" <<"*.AC3" <<"*.AAC" << "*.FLAC";
	}
	else if(media_type == AMedia_TYPE_PICTURE)
	{
		if(qApp->applicationDirPath() ==  "/")
		{
			 m_strPlayListDir =qApp->applicationDirPath() + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE +"/";
			 m_StrUsbListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE+"/";
			 m_StrSdListDir  = MOUNTDIR + qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_PICTURE +"/";
			 m_StrUsbListDir_2 = MOUNTDIR + qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE +"/";
			 m_StrHddListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_PICTURE +"/";
		}
		else
		{
			m_strPlayListDir =qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE;
           	         m_StrUsbListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE;
			m_StrSdListDir  = MOUNTDIR + qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_PICTURE ;
			m_StrUsbListDir_2 = MOUNTDIR + qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PICTURE ;
			m_StrHddListDir = MOUNTDIR + qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_PICTURE ;
		}
		playFileFilter<<"*.JPG"<<"*.PNG" << "*.BMP"  <<"*.png" << "*.gif "<<"*.bmp" << "*.jpg" <<"*.jpeg" << "*.GIF";

	}
	else if(media_type == AMedia_TYPE_HTML)
	{
		if(qApp->applicationDirPath() ==  "/")
		{
			 m_strPlayListDir =qApp->applicationDirPath() +"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML+"/";
			 m_StrUsbListDir = MOUNTDIR +qApp->applicationDirPath()+"/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML+"/";
			 m_StrSdListDir  = MOUNTDIR +qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_HTML +"/";
			 m_StrUsbListDir_2 = MOUNTDIR +qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML +"/";
			 m_StrHddListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_HTML +"/";
		}
		else
		{
			m_strPlayListDir =qApp->applicationDirPath() +"/"+ LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML;
			m_StrUsbListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML;
			m_StrSdListDir  = MOUNTDIR +qApp->applicationDirPath() +"/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_HTML;
			m_StrUsbListDir_2 = MOUNTDIR +qApp->applicationDirPath() +"/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_HTML ;
			m_StrHddListDir = MOUNTDIR +qApp->applicationDirPath() +"/" + StorageDirName_HDD + "/" + FOLDER_NAME_HTML ;
		}
		playFileFilter << "*.html" << "*.htm";
	}

	QDir  playFileListDir(m_strPlayListDir);
	if(playFileListDir.exists())
	{
		playFileListDir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		playFileListDir.setNameFilters(playFileFilter);
		const QFileInfoList list = playFileListDir.entryInfoList();
		QFileInfoList::const_iterator iterator = list.begin();
		while ( iterator != list.end() )
		{
			QStringList strName = (*iterator).fileName().split(".");
			PlayListFiles <<  (*iterator).fileName() ;
			PlayfileInfoList << (*iterator).filePath ();
			iterator++;
		}
	}

	QDir  playFileUsbListDir(m_StrUsbListDir);
	if(playFileUsbListDir.exists())
	{
		playFileUsbListDir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		playFileUsbListDir.setNameFilters(playFileFilter);
		const QFileInfoList fileInfolist = playFileUsbListDir.entryInfoList();
		QFileInfoList::const_iterator iterators = fileInfolist.begin();
		while ( iterators != fileInfolist.end() )
		{
			QStringList strName = (*iterators).fileName().split(".");
			PlayListFiles <<  (*iterators).fileName() ;
			PlayfileInfoList << (*iterators).filePath ();
			iterators++;
		}
	}

	QDir  playFileUsbListDir_2(m_StrUsbListDir_2);
	if(playFileUsbListDir_2.exists())
	{
		playFileUsbListDir_2.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		playFileUsbListDir_2.setNameFilters(playFileFilter);
		const QFileInfoList fileInfolist = playFileUsbListDir_2.entryInfoList();
		QFileInfoList::const_iterator iterators = fileInfolist.begin();
		while ( iterators != fileInfolist.end() )
		{
			QStringList strName = (*iterators).fileName().split(".");
			PlayListFiles <<  (*iterators).fileName() ;
			PlayfileInfoList << (*iterators).filePath ();
			iterators++;
		}
	}

	QDir  playFileSdListDir(m_StrSdListDir);
	if(playFileSdListDir.exists())
	{
		playFileSdListDir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		playFileSdListDir.setNameFilters(playFileFilter);
		const QFileInfoList fileInfolist = playFileSdListDir.entryInfoList();
		QFileInfoList::const_iterator iterators = fileInfolist.begin();
		while ( iterators != fileInfolist.end() )
		{
			QStringList strName = (*iterators).fileName().split(".");
			PlayListFiles <<  (*iterators).fileName() ;
			PlayfileInfoList << (*iterators).filePath ();
			iterators++;
		}
	}

	QDir  playFileHddListDir(m_StrHddListDir);
	if(playFileHddListDir.exists())
	{
		playFileHddListDir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks );
		playFileHddListDir.setNameFilters(playFileFilter);
		const QFileInfoList fileInfolist = playFileHddListDir.entryInfoList();
		QFileInfoList::const_iterator iterators = fileInfolist.begin();
		while ( iterators != fileInfolist.end() )
		{
			QStringList strName = (*iterators).fileName().split(".");
			PlayListFiles <<  (*iterators).fileName() ;
			PlayfileInfoList << (*iterators).filePath ();
			iterators++;
		}
	}
}

void QWebBrowserApp::PlayIdlePlayListFile()
{
	QString strFullPath;
	bool bRet;
	do
	{
		if(m_iStateType != State_Idle )
		{
			break ;
		}
		if(m_TunerScanStatus ==TunerScanning || TunerScan_Start == m_TunerScanStatus)
		{					
			break;
		}
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		int iRet=CheckPlayEnable(FALSE);
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif

#if 0
		QString IdleTime = mp_Config ->getConfigValue(INI_KEY_IdleDetectTime);
		int idleDetect = -1;
		if(IdleTime.trimmed()!="")
		{
			idleDetect = IdleTime.trimmed().toInt();
			if(0  > idleDetect )
			{
				if(m_PlayIdlePlayList_Timer.isActive())
				{
					m_PlayIdlePlayList_Timer.stop();
					m_iIdleTime = 0;
				}
			}
		}
		else
		{
			if(m_PlayIdlePlayList_Timer.isActive())
			{
				m_PlayIdlePlayList_Timer.stop();
				m_iIdleTime = 0;
			}
			break;
		}
		m_iIdleTime++;
		if(m_iIdleTime == idleDetect)
#endif

		{
			if(m_PlayIdlePlayList_Timer.isActive())
			{
				m_PlayIdlePlayList_Timer.stop();
				m_iIdleTime = 0;
			}
			do
			{
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(&m_playWin == pActiveWindow && (m_PlayPicAndHtmlWin->isPlayingHtml()))
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
				}
				QString fileName = mp_Config->getConfigValue(INI_KEY_CurIdlePlaylistFilePath);
				if(fileName.trimmed()=="")
				{
					m_PlayIdlePlayList_Timer.stop();
					m_iIdleTime = 0;
					break;
				}
				bRet = LoadPlaylist(fileName);
				if(bRet)
				{
					StopPlayDurationTimer();
					m_bIsNeedLoop = false;
					PlayNextEntry(true);
				}
				else
				{
					ShowBrowserWindow();
				}
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlayIdlePlayList;
				m_bIsPlayingIdlePlayList = true;
				m_eCurPlayMode = ControlCenter::PlayMode_Normal;
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlayIdlePlayList;
			}while(false);
		}
	}while(FALSE);
}

int QWebBrowserApp::SaveMediaVirChInfoCfg()
{
	int iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = mMediaVirChannel_sp->SaveCfgToXmlFile(APP_DIR_PATH + MEDIA_VIR_CH_LIST_FILE_PATH);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int QWebBrowserApp::SyncCacheToStorage()
{
	int iOutRet = ERROR_SUCCESS, iRet;
	//LOG_BLINE("SyncCacheToStorage\n");
	do
	{
		if(m_SaveMediaVirCh_Timer.isActive())
		{
			m_SaveMediaVirCh_Timer.stop();
			iRet = SaveMediaVirChInfoCfg();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		sync();
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by su 2013-06-18*/
int QWebBrowserApp::On_ChangeLanguageDone(int data)
{
	int iOutRet = ERROR_SUCCESS;
	QString temstr;
	QByteArray bytedata;	

	if(Sw_LogDbusCmd){
		LOG_BLINE("%s()\n",__func__);
	}
	
	temstr=QString::number(data);
	bytedata = temstr.toLocal8Bit();
	
	do
	{
#if USE_QCOPCHANNEL
		if(!QCopChannel::send(QTSERVER_CopCh_Name, CopChMsg_ChangeLanguageDone, bytedata)){
			LOG_BLINE("QCopChannel send (CopChMsg_ChangeLanguageDone) error\n");
		}else{
			//LOG_BLINE("QCopChannel send  (Success)\n");
		}
#endif		
		sync();
	}while(FALSE);

	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-12-7*/
int QWebBrowserApp::On_TouchScreenCalibrationDone()
{
	int iOutRet = ERROR_SUCCESS, iRet;

	if(Sw_LogDbusCmd){
		LOG_BLINE("%s()\n",__func__);
	}

	do
	{
		BOOL_t bDevIsExists = false;
		iRet = QTouchScreen::ChkDevExistence(OUT bDevIsExists);
		if(ERROR_SUCCESS == iRet){
			if(bDevIsExists){
#if USE_QCOPCHANNEL
				if(!QCopChannel::send(QTSERVER_CopCh_Name, CopChMsg_TouchScreenCalibrationDone)){
					LOG_BLINE("QCopChannel send (CopChMsg_TouchScreenCalibrationDone) error\n");
				}else{
					//LOG_BLINE("QCopChannel send  (Success)\n");
				}
#endif
			}else{
				LOG_BLINE("ChkDevExistence (TouchScreen Dev) not exists\n");
			}
		}else{
			LOG_BLINE("ChkDevExistence (TouchScreen Dev) error\n"); 
		}
		
		sync();
	}while(FALSE);

	iOutRet = iRet;
	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-12-17 for overlay by javascript dynamic run*/
int QWebBrowserApp::getHtmlEvalJavascript( int & iOutRet, QString & strJavascript )
{
	if(Sw_LogDbusCmd){
		LOG_BLINE("%s()\n",__func__);
	}

	do
	{
		iOutRet = ERROR_SUCCESS;
		strJavascript = m_strRetJavascriptLast;
		//LOG_BLINE("iOutRet=%d\n%s\n",iOutRet,qPrintable(strJavascript));
	}while(FALSE);

	return iOutRet;
}

int QWebBrowserApp::setHtmlEvalJavascript( QString strJavascript, int & iOutRet )
{
	if(Sw_LogDbusCmd){
		LOG_BLINE("%s()\n",__func__);
	}

	do
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		
		if( (pActiveWindow== &m_Browser) )
		{
			iOutRet = ERROR_INVALID_STATE;
			m_strRetJavascriptLast = m_strRetJavascriptLast.sprintf("%d",iOutRet);
			LOG_BLINE("Is not playing html now, can't run javascript\n");
			break;
		}
		else if(pActiveWindow== &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml())
		{
			if(m_playWin.getPlayHtmlOrPicWin().isNull())
			{
				if(Sw_LogDbusCmd)
				{
					LOG_BLINE("play html or picture window is null\n");
				}
				iOutRet = ERROR_INVALID_STATE;
				break;
			}		
			iOutRet = m_playWin.getPlayHtmlOrPicWin()->setHtmlEvalJavascript(strJavascript,OUT m_strRetJavascriptLast);			
		}
		else
		{
			iOutRet = ERROR_INVALID_STATE;
			m_strRetJavascriptLast = m_strRetJavascriptLast.sprintf("%d",iOutRet);
			LOG_BLINE("Is not playing html now, can't run javascript\n");
		}
	}while(FALSE);

	return iOutRet;
}
#endif

#if 1/*added by lxj 2012-12-24*/
void QWebBrowserApp::Set_CursorVisible(int b)
{
	if(Sw_LogDbusCmd){
		LOG_BLINE("%s(%d)\n",__func__,b);
	}

#if USE_QCOPCHANNEL
	if( b ){
		if(!QCopChannel::send(QTSERVER_CopCh_Name, CopChMsg_SetCursorShow)){
			LOG_BLINE("QCopChannel send (CopChMsg_SetCursorShow) error\n");
		}
	}else{
		if(!QCopChannel::send(QTSERVER_CopCh_Name, CopChMsg_SetCursorHide)){
			LOG_BLINE("QCopChannel send (CopChMsg_SetCursorHide) error\n");
		}		
	}
#endif
}
#endif

int QWebBrowserApp::On_DataCommSrv_DonwloadFinished(
	int iDownloadErrNo, quint32 DonwloadId, const QString & strSrcUrl, const QString & strDestUrl)
{
	int iOutRet = ERROR_SUCCESS;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("DonwloadFinished,Id=%u,Err=%d,Src=%s,Dest=%s\n",
			DonwloadId, iDownloadErrNo, qPrintable(strSrcUrl), qPrintable(strDestUrl));
	}
	do
	{
		if(0 == DonwloadId)	//invalid id
		{
			break;
		}

		if(m_NewSysCfg_DownloadId == DonwloadId)
		{
			m_NewSysCfg_DownloadId = 0;
			iOutRet = OnNewSysCfgDownloadFinished(m_NewSysCfg_DownloadId);
		}
		if(m_NewSystem_DownloadId == DonwloadId)
		{
			m_NewSystem_DownloadId = 0;
			iOutRet = OnNewSystemVersionDownloadFinished(iDownloadErrNo, strSrcUrl);
		}
		if(m_NewSysVerCfg_DownloadId ==DonwloadId)
		{
			m_NewSysVerCfg_DownloadId = 0;
			iOutRet = OnNewSysVersionCfgDownloadFinished(iDownloadErrNo, strSrcUrl);
		}
		if(m_NewOsd_DownloadId == DonwloadId)
		{
			m_NewOsd_DownloadId = 0 ;
			iOutRet = OnNewOsdDownloadFinished(iDownloadErrNo, strSrcUrl);
		}
	}while(FALSE);

	return iOutRet;
}

int QWebBrowserApp::On_NewSysCfgIsReady(const QString & SysCfgDirPath)
{
	int iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("SysCfgDirPath=%s\n", qPrintable(SysCfgDirPath));
	}

	do
	{
		// /MP7XX/Config/SystemConfig.ini
		iRet = strShellCmd.Format("cp -fa \"%s/MP7XX/Config/SystemConfig.ini\" /MP7XX/Config/SystemConfig.ini", SysCfgDirPath.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
		// /MP7XX/Config/VirtualChannel.xml
		iRet = strShellCmd.Format("cp -fa \"%s/MP7XX/Config/VirtualChannel.xml\" /MP7XX/Config/VirtualChannel.xml", SysCfgDirPath.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
		// Script/WebBrowser.js
		iRet = strShellCmd.Format(
			"if [ -f \"%s/MP7XX/Script/WebBrowser.js\" ]; then "
				"rm -f /MP7XX/Script/WebBrowser.js;"
				"cp -fa \"%s/MP7XX/Script/WebBrowser.js\" /MP7XX/Script/WebBrowser.js;"
			"fi;", 
			SysCfgDirPath.toUtf8().constData(), SysCfgDirPath.toUtf8().constData());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
		// delete /data/Xapp/* first
		iRet = strShellCmd.Format("rm -fr /data/Xapp/*");
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
		//Then copy new ones to /data/Xapp/
		{
			QString strSrcDir = SysCfgDirPath + "/data/Xapp";
			QDir DummyDir;
			if(DummyDir.exists(strSrcDir))
			{
				iRet = strShellCmd.Format(
					"ItemsInDir=`ls \"%s/data/Xapp/\" | wc -l`;"
					"if [ -z \"${ItemsInDir}\" ]; then "
						"ItemsInDir=0;"
					"fi;"
					"if [ ${ItemsInDir} -gt 0 ]; then "
						"mkdir -p /data/Xapp;"
						"cp -fra %s/data/Xapp/* /data/Xapp/;"
					"fi;",
					SysCfgDirPath.toUtf8().constData(), SysCfgDirPath.toUtf8().constData());
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				}
			}
		}
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		if(ERROR_SUCCESS != iOutRet)
		{
			LOG_BLINE("On_NewSysCfgIsReady ret %d\n", iOutRet);
		}
	}

	return iOutRet;
}

int QWebBrowserApp::On_GpioBtnInput_Started()
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		//LOG_BLINE("On_GpioBtnInput_Started\n");
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::DbgLog(const QString & strDbgInfo)
{
	LOG("%s", qPrintable(strDbgInfo));
}

int QWebBrowserApp::UploadCfg()
{
	int iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	do
	{
		if(m_strSysCfgUploadUrl.isEmpty())
		{
			break;
		}

		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		iRet = system((APP_DIR_PATH + "CollectSysCfg.sh" + " " + "\"" + APP_DIR_PATH + "\"").toUtf8());
		if(0 != iRet)
		{
			LOG_BLINE("CollectSysCfg.sh ret %d\n", iRet);
			break;
		}

		//Call into org.thtfit.WebBrowser.DataCommSrv.UploadFile
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "UploadFile2");
		MsgArgsList << "/Temp/SysCfg.tar" << m_strSysCfgUploadUrl << "";
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2012-8-2 for player features enabled check*/
INT_t QWebBrowserApp::CheckPlayEnable(BOOL_t isShowError)
{
	INT_t iRet=ERROR_SUCCESS;
	BOOL_t bEnablePlayer = TRUE;

	do
	{
#if 0/*call by class*/
		iRet = m_Mp7xxGlobalStatusData.getEnablePlayer(OUT bEnablePlayer);
#else/*call by function for test*/
		iRet = getEnablePlayer(&bEnablePlayer);
#endif
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		if(!bEnablePlayer)
		{
#if 0/*use message box to show*/
			Show_StatusMsgBox("DISABLE", QString(LANG_NO_LICENSE), TRUE, 5*1000);
#else/*use osd bar to show*/
			if(isShowError)
			{
				if(m_MsgStatusBarItem_sp.isNull())
				{
					m_MsgStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
				}
				if(FALSE == m_MsgStatusBarItem_sp.isNull())
				{
					iRet = m_MsgStatusBarItem_sp->setText(LANG_NO_LICENSE,3*1000);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
			}
#endif
			iRet = ERR_DISABLE_PLAYER;
			//LOG_BLINE("No license to play\n");
			break;
		}
	}
	while(FALSE);

	return iRet;
}
#endif

void QWebBrowserApp::PlayCurrentFile(const QString fileName, int fileType)
{
	INT_t iRet;
	QString strFullPath;

	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iRet=CheckPlayEnable();
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif

#if 1 /*---------add by xuweiwei 2013-11-22 To Make 92 video mode always full screen--------- */ 
		if(MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iVideoOutMode)
		{
			UpdateVideoPosition(true);
		}
		else if(MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iOldVideoOutMode)
		{
			UpdateVideoPosition(false);
		}
#endif
		do
		{
			strFullPath = fileName;
			QFileInfo fileInfo(strFullPath);
			if(fileInfo.exists())
			{
				break;
			}
			else
			{
				QString fileNames = fileInfo.fileName ();
				QString strFileType = getFolderNameByFileType(fileType);
				QString strFullPath = LookForActualFilePath(QPlaylistEntry::ACTION_Video, fileNames, strFileType);
				if(QFile::exists(strFullPath))
				{
					break;
				}
				strFullPath = m_StrSdListDir + "/" + fileNames;
				if(QFile::exists(strFullPath))
				{
					break;
				}
				strFullPath = m_strPlayListDir + "/" + fileNames;
				if(QFile::exists(strFullPath))
				{
					break;
				}
				strFullPath = m_StrUsbListDir + "/" + fileNames;
				if(QFile::exists(strFullPath))
				{
					break;
				}
				strFullPath = m_StrUsbListDir_2 + "/" + fileNames;
				if(QFile::exists(strFullPath))
				{
					break;
				}
			}
		}while(FALSE);

		do
		{
			m_PlaybackId = 0;
			m_strCurrPlayListName ="";
			m_strCurrPlayListUrl = "";
			m_strCurrPluginListName ="";
			m_strCurrPluginListUrl = "";
			if(m_bIsPlayPluginList)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
			}
			StopPlayPluginList();
			StopPlayback();
			m_NextFileInfoToPlay.m_strFilename = "";
			m_NextFileInfoToPlay.m_iLoopTimes = 1;
			if(m_bIsPlayPluginList)
			{
				if(m_PluginPlaylistData_sp)
				{
					m_PluginPlaylistData_sp->ClearPlayListData();
					m_bIsPlayPluginList = false;
				}
			}
			m_BIsPlaySplash = false;
			if(fileType == AMedia_TYPE_PLAYLIST)
			{
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(pActiveWindow== &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml())
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
				}
				//for mp_signal
				m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_PlaySelectItem;
				m_PlaylistDataA.ClearPlayListData();
				iRet = m_PlaylistDataA.LoadFromIniFile(&strFullPath);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
				else
				{
					QFileInfo fileInfo(strFullPath);
					m_strCurrPlayListUrl = strFullPath;
					m_strCurrPlayListName = fileInfo.fileName();
				}
				m_eCurPlayMediaFileType = AMedia_TYPE_PLAYLIST;
				m_iStateType = State_PlayingPlayList;
				m_bIsNeedLoop = false;
				PlayNextEntry(true);
				m_eCurPlayMode = ControlCenter::PlayMode_Normal;
				break;
			}
			else if(fileType == AMedia_TYPE_HTML)
			{
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_PlayingBrowserFile;
				m_PlaylistDataA.ClearPlayListData();
				iRet = CallPlayWebpage(strFullPath,0,true);
				if(ERROR_SUCCESS == iRet)
				{					
				}
				m_eCurPlayMode = ControlCenter::PlayMode_Normal;
				m_iStateType = State_PlayingBrowserFile;
				break;
			}
			else
			{
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				if(QFile::exists(strFullPath))
				{
					StopPlayback();
					if(fileType == AMedia_TYPE_VIDEO)
					{
						m_iStateType = State_PlayingVideoFile;
					}
					else if(fileType == AMedia_TYPE_AUDIO)
					{
						m_iStateType = State_PlayingAudioFile;
					}
					else if(fileType == AMedia_TYPE_PICTURE)
					{
						m_iStateType = State_PlayingPictureFile;
					}
					iRet = PlayUrl(strFullPath);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					else
					{
						if(fileType == AMedia_TYPE_VIDEO)
						{
							m_iStateType = State_PlayingVideoFile;
						}
						else if(fileType == AMedia_TYPE_AUDIO)
						{
							m_iStateType = State_PlayingAudioFile;
						}
						else if(fileType == AMedia_TYPE_PICTURE)
						{
							m_iStateType = State_PlayingPictureFile;
						}
						m_eCurPlayMode = ControlCenter::PlayMode_Normal;
						m_PlaylistDataA.ClearPlayListData();
						if(m_bIsPlayingIdlePlayList)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
					
					bool bFullScreen = false;//true;/*zoom up or down*/
					if(bFullScreen)
					{
						UpdateVideoPosition(bFullScreen);
					}
					else
					{
						UpdateVideoPosition();
						ShowBrowserWindow();
					}			
					
					iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
					if(iRet !=ERROR_SUCCESS)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				else
				{
					LOG_BLINE("File not found,%s\n", qPrintable(strFullPath));
				}
				break;

			}
		}while(FALSE);
	}while(FALSE);
}


INT_t QWebBrowserApp::ChangeVolume(int VolumeChnageVal, bool isVideoSet)
{
	INT_t iRet = ERROR_SUCCESS;
	INT32_t CurVol = 0;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}

		setPlayVolume(VolumeChnageVal,iRet);
		CurVol = VolumeChnageVal;
		if(ERROR_SUCCESS == iRet)
		{
			m_CurPlayerVolume = CurVol;
			m_bVolumeMuted = FALSE;
			if(!isVideoSet)
			{
				m_TurnVolumeWin.show();
				m_TurnVolumeWin.setVisible(true);
				m_TurnVolumeWin.UpdateNewVolume(CurVol);
				m_HideVolumeBarTimer.start(2*1000);
			}

			BOOL_t bEnableWriteLoggin = TRUE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			if(bEnableWriteLoggin)
			{
				char strTemp[64];
				snprintf(strTemp, sizeof(strTemp), "%d", TYPE_LOG_VolumeChange);
				strTemp[sizeof(strTemp)-1] = '\0';
				CString::STD_STRING_LIST strList ;
				strList.push_back(strTemp);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp,"%d",m_CurPlayerVolume);
				strList.push_back(strTemp);
				if(ERROR_SUCCESS != WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				else
				{
					strList.clear();
				}
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(false);

	return iRet;
}


VOID QWebBrowserApp::CloseGpioBtnInputFile()
{
	INT_t iRet;

	if(INVALID_FILE_DESCRIPTOR != m_GpioBtnInputFd)
	{
		iRet = close(m_GpioBtnInputFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_GpioBtnInputFd = INVALID_FILE_DESCRIPTOR;
	}
	if(FALSE == m_GpioBtnInputNotifier_sp.isNull())
	{
		m_GpioBtnInputNotifier_sp->setEnabled(false);
	}
}

void QWebBrowserApp::setGpioPlayFilePath(QString storageMedium)
{
	INT_t iOutRet = ERROR_SUCCESS; 
	QStringList  playFileList;
	
	/*initial list*/
	playFileList 
	<< QString(MOUNTDIR + qApp->applicationDirPath() + "/" + USB_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST + "/")
	<< QString(MOUNTDIR +qApp->applicationDirPath() + "/" + USB2_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST + "/") 
	<< QString(MOUNTDIR +qApp->applicationDirPath() + "/" + SD_CARD_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST + "/")
	<< QString(qApp->applicationDirPath() + "/" + LOCAL_STORAGE_DIR_NAME + "/" + FOLDER_NAME_PLAYLIST + "/")
	<< QString(MOUNTDIR +qApp->applicationDirPath() + "/" + StorageDirName_HDD + "/" + FOLDER_NAME_PLAYLIST + "/");

	/*initial once*/
	static int isSet = playFileList.size();

	/*Storage medium priority*/
	if(storageMedium.compare(USB_STORAGE_DIR_NAME, Qt::CaseInsensitive))
	{
		for(int iPrior = 0; iPrior < playFileList.size(); iPrior++)
		{
			QStringList strTempList = playFileList.at(iPrior).split("/");
			if(0 == strTempList.at(2).compare(storageMedium, Qt::CaseInsensitive))
			{
				playFileList.swap(0, iPrior);
				break;
			}
		}
	}
	
	/*Sets path*/
	for(int iSet = 0; iSet < playFileList.size(); iSet++)
	{
		//LOG_BLINE("isSet = %d, iSet = %d, playFileList = %s,\n",
			//isSet,iSet,qPrintable(playFileList.at(iSet)));
		if(QFile::exists(playFileList.at(iSet)))
		{
			if(isSet != iSet)
			{
				isSet = iSet;
				
#if 0 /*because of the gpio board of MP800 desiged err.*/
				for(int i = 1; i<= GPIOBUTTONSIZE; i++)
				{
					QSharedPointer <QGpioFilePath> GpioFilePath_sp(new QGpioFilePath);
					if(GpioFilePath_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						goto EXIT_PROC;
					}
					GpioFilePath_sp->m_strUrl = playFileList.at(iSet) + "Gpio_PlayList" 
						+ QString("_%1").arg(QString::number(i));
					//LOG_BLINE("%s\n",qPrintable(GpioFilePath_sp->m_strUrl));
					m_GpioFilePath_Map[i] = GpioFilePath_sp;
				}
#else
				//for(int i = 1, j = 1, k = 5; i <= GPIOBUTTONSIZE; i++)
				for(int i = GPIOBUTTONSIZE, j = 1, k = 5; i > 0; i--)
				{
					QSharedPointer <QGpioFilePath> GpioFilePath_sp(new QGpioFilePath);
					if(GpioFilePath_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						goto EXIT_PROC;
					}
					if(i % 2)
					{
						GpioFilePath_sp->m_strUrl = playFileList.at(iSet) + "Gpio_PlayList" 
							+ QString("_%1").arg(QString::number(i));
						//LOG_BLINE("num = %d ,%s\n",j,qPrintable(GpioFilePath_sp->m_strUrl));
					    //m_GpioFilePath_Map[j++] = GpioFilePath_sp;
						m_GpioFilePath_Map[k++] = GpioFilePath_sp;
					}
					else
					{
						GpioFilePath_sp->m_strUrl = playFileList.at(iSet) + "Gpio_PlayList" 
							+ QString("_%1").arg(QString::number(i));
						//LOG_BLINE("num = %d ,%s\n",k,qPrintable(GpioFilePath_sp->m_strUrl));
				        //m_GpioFilePath_Map[k++] = GpioFilePath_sp;
						m_GpioFilePath_Map[j++] = GpioFilePath_sp;
					}
				}
#endif
				break;
			}
			else
			{
				//LOG_BLINE("Do not set again.\n");
				break;
			}
		}
	}

EXIT_PROC:
	return ;
}

VOID QWebBrowserApp::On_GpioBtnKey(BYTE BtnKeyCode)
{
	if(Sw_LogGpioButton)
	{
		LOG_BLINE("BtnKeyCode:%d\n", BtnKeyCode);
	}

	do
	{
		//parameter check
		if(FALSE == 
			((GPIO_BTN_0_DOWN <= BtnKeyCode && GPIO_BTN_7_DOWN >= BtnKeyCode) || 
			(GPIO_BTN_0_UP <= BtnKeyCode && GPIO_BTN_7_UP >= BtnKeyCode)))
		{
			break;
		}
		//status changed
		do
		{
			if(m_PlayIdlePlayList_Timer.isActive())
			{
				m_PlayIdlePlayList_Timer.stop();
				m_iIdleTime = 0;
				if(m_iStateType == State_Idle)
				{
					if(m_TunerScanStatus == TunerScanning)
					{					
						break;
					}
					startIdelDetectTimer();
				}
			}
		}while(false);
		//stop first
		if(m_bIsPlayingIdlePlayList)
		{
			m_bIsPlayingIdlePlayList = false;
			m_PlaylistDataA.ClearPlayListData();
			m_iIdleTime = 0;
			StopPlayback();
		}
		//internal action
		if(m_bInternalActionOnGpioBtnEvt)
		{
			if(FALSE == m_ScriptEng_sp.isNull())
			{
				//prepare
			  	QScriptValueList argList;
				QScriptValue FunctionValue;
				if(BtnKeyCode  >=  GPIO_BTN_0_DOWN  && BtnKeyCode <= GPIO_BTN_7_DOWN)
				{
					setGpioPlayFilePath(m_strActiveStorageDirName);/*set path*/
					
					int GpioBtnNo = (BtnKeyCode - GPIO_BTN_0_DOWN) + 1;
					QString strPlayUrl = getGpioPlayFilePath(GpioBtnNo);
					argList << BtnKeyCode  << strPlayUrl;
					FunctionValue = m_ScriptEng_sp->evaluate("on_GpioButton_Down");
				}
				else if(BtnKeyCode >= GPIO_BTN_0_UP &&  BtnKeyCode <= GPIO_BTN_7_UP)
				{
					argList << BtnKeyCode ;
					FunctionValue = m_ScriptEng_sp->evaluate("on_GpioButton_Up");
				}
				else	//invalid key
				{
					break;	//go out
				}
				//exec
				FunctionValue.call(m_ScriptEng_sp->globalObject(), argList);
				if(m_ScriptEng_sp->hasUncaughtException())
				{
					QScriptValue ExceptionValue;
					int LineNo = m_ScriptEng_sp->uncaughtExceptionLineNumber();
					ExceptionValue = m_ScriptEng_sp->uncaughtException();
					//LOG_BLINE("JsExcept,Line:%d,%s\n", LineNo, qPrintable(ExceptionValue.toString()));
					//add dbg log
					if(Sw_LogJavascript)
					{
						LOG_BLINE("JsExcept,Line:%d,%s\n", LineNo, qPrintable(ExceptionValue.toString()));
					//add dbg log
					}
					break;
				}
			}
		}
		//emit gpio btn key event
		emit onGpioBtnEvt(BtnKeyCode);		
		//log event
		int iRet;
		BOOL_t bEnableWriteLoggin = TRUE;
		iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		if(bEnableWriteLoggin)
		{
			CString::STD_STRING_LIST strList;
			strList.push_back(TYPE_LOG_GpioInput);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
			strList.push_back(BtnKeyCode);
			if(ERROR_SUCCESS != WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				strList.clear();
			}
		}
	}while(FALSE);
}

VOID QWebBrowserApp::StopPlayDurationTimer()
{
	if(m_PlayDurationTimer.isActive ())
	{
		m_PlayDurationTimer.stop();
	}
	
	if(m_PlaylistDurationTimer.isActive ())
	{
		m_PlaylistDurationTimer.stop();
	}
		
	if(m_HtmlDurationTimer.isActive ())
	{
		m_HtmlDurationTimer.stop();
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
	}		
	m_PlayDurationTimer.stop();
	m_PlaylistDurationTimer.stop();
	m_HtmlDurationTimer.stop();
}

//Caller must lock
QString QWebBrowserApp::getDirNameFromFileType(const QString & strFileType)
{
	QString strDirName;

	if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive))
	{
		strDirName = FOLDER_NAME_VIDEO;
	}
	else if(0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive))
	{
		strDirName = FOLDER_NAME_MUSIC;
	}
	else if(0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
	{
		strDirName = FOLDER_NAME_PICTURE;
	}
	else if(0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive))
	{
		strDirName = FOLDER_NAME_HTML;
	}
	else if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive))
	{
		strDirName = FOLDER_NAME_PLAYLIST;
	}

	return strDirName;
}


QString QWebBrowserApp::getStorageDirNameFromDstFileName(const QString & strDstFileName,
	QString * pstrActualPlaylistFilename/* = NULL*/)
{
	QString strStorageDirName;
	QString strStorageLocationId;

	strStorageLocationId = strDstFileName.left(1).toUpper();

	if(strStorageLocationId.isEmpty())
	{
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = "";
		}
	}
	else if(0 == strStorageLocationId.compare(DST_LOCATION_ID_FLASH, Qt::CaseInsensitive))
	{
		strStorageDirName = LOCAL_STORAGE_DIR_NAME;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName.mid(1);
		}
	}
	else if(0 == strStorageLocationId.compare(DST_LOCATION_ID_SDCARD, Qt::CaseInsensitive))
	{
		strStorageDirName = SD_CARD_DIR_NAME;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName.mid(1);
		}
	}
	else if(0 == strStorageLocationId.compare(DST_LOCATION_ID_UDISK, Qt::CaseInsensitive))
	{
		strStorageDirName = USB_STORAGE_DIR_NAME;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName.mid(1);
		}
	}
	else if(0 == strStorageLocationId.compare(DST_LOCATION_ID_HDD, Qt::CaseInsensitive))
	{
		strStorageDirName = StorageDirName_HDD;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName.mid(1);
		}
	}
	else if(0 == strStorageLocationId.compare(DST_LOCATION_ID_RAM, Qt::CaseInsensitive))
	{
		strStorageDirName = StorageDirName_RAM;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName.mid(1);
		}
	}
	else	//default location
	{
		strStorageDirName = m_strActiveStorageDirName;
		if(pstrActualPlaylistFilename)
		{
			*pstrActualPlaylistFilename = strDstFileName;
		}
	}

	return strStorageDirName;
}

QString QWebBrowserApp::getStorageDirNameFromCeLabsDataSrcDev(const QString & strCeLabsDataSrvDev)
{
	QString strStorageDirName = m_strActiveStorageDirName;

	if(0 == strCeLabsDataSrvDev.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive))
	{
		strStorageDirName = LOCAL_STORAGE_DIR_NAME;
	}
	else if(0 == strCeLabsDataSrvDev.compare(CeLabs_DATA_SRC_DEV_USB, Qt::CaseInsensitive))
	{
		strStorageDirName = USB_STORAGE_DIR_NAME;
	}
	else if(0 == strCeLabsDataSrvDev.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive) ||
		  0 == strCeLabsDataSrvDev.compare(CeLabs_DATA_SRC_DEV_IDE, Qt::CaseInsensitive))
	{
		strStorageDirName = StorageDirName_HDD;
	}
	else if(0 == strCeLabsDataSrvDev.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive))
	{
		strStorageDirName = SD_CARD_DIR_NAME;
	}

	return strStorageDirName;
}

QString QWebBrowserApp::getActiveMediaDescFromType(ActiveMedia_TYPE eActiveMediaType)
{
	QString strActiveMediaDesc = ActiveMedia_Desc_FLASH;

	if(AMedia_TYPE_FLASH == eActiveMediaType)
	{
		strActiveMediaDesc = ActiveMedia_Desc_FLASH;
	}
	else if(AMedia_TYPE_SdCard == eActiveMediaType)
	{
		strActiveMediaDesc = ActiveMedia_Desc_SdCard;
	}
	else if(AMedia_TYPE_USB == eActiveMediaType)
	{
		strActiveMediaDesc = ActiveMedia_Desc_USB;
	}
	else if(AMedia_TYPE_HDD == eActiveMediaType)
	{
		strActiveMediaDesc = ActiveMedia_Desc_HDD;
	}

	return strActiveMediaDesc;
}

int QWebBrowserApp::OnTimer_OpenGpioBtnInputFile()
{
	int iOutRet = ERROR_SUCCESS;
	bool bRet;

	do
	{
		CloseGpioBtnInputFile();
		m_GpioBtnInputFd = open(BTN_IN_SRV_PIPE_PATH, O_RDONLY | O_NONBLOCK);
		if(0 <= m_GpioBtnInputFd)
		{
			m_GpioBtnInputNotifier_sp = QSharedPointer <QSocketNotifier> (
				new QSocketNotifier(m_GpioBtnInputFd,QSocketNotifier::Read, this));
			if(m_GpioBtnInputNotifier_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			bRet = connect(&(*m_GpioBtnInputNotifier_sp), SIGNAL(activated(int)), this, SLOT(On_GpioBtnInputEvent(int)));
			if(false == bRet)
			{
				LOG_BLINE("connect failed\n");
				iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
				break;
			}
			m_GpioBtnInputNotifier_sp->setEnabled(TRUE);
		}
		else	//failed
		{
			//LOG_BLINE("Open(GpioBtnInputFile),%s\n", strerror(errno));
			QTimer::singleShot(2000, this, SLOT(OnTimer_OpenGpioBtnInputFile()));
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

VOID QWebBrowserApp::On_GpioBtnInputEvent(int SocketFd)
{
	int iRet;
	BYTE szDataBuf[128];

	do
	{
		if(m_GpioBtnInputFd != SocketFd)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		m_GpioBtnInputNotifier_sp->setEnabled(FALSE);
		iRet = read(m_GpioBtnInputFd, szDataBuf, sizeof(szDataBuf));
		if(0 <  iRet)
		{
			m_GpioBtnInputData.append((const char *)szDataBuf, iRet);
		}
		else if(0 == iRet)
		{
			//EINTR -- maybe the pipe fifo has already closed
			if(EINTR == errno || EAGAIN == errno || EWOULDBLOCK == errno)
			{
				CloseGpioBtnInputFile();
				iRet = OnTimer_OpenGpioBtnInputFile();
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
		}
		else	// < 0
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				break;
			}
			else if(EBADF == errno)
			{
				CloseGpioBtnInputFile();
				iRet = OnTimer_OpenGpioBtnInputFile();
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
				break;
			}
			else if(EINTR == errno)
			{
				break;
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
		}

		m_GpioBtnInputNotifier_sp->setEnabled(TRUE);

		//process the data
		while(TRUE)
		{
			if(4 <=  m_GpioBtnInputData.count())
			{
				UINT16_t PacketSize = 0;
				memcpy(&PacketSize, m_GpioBtnInputData.data(), 2);
				if(1024 < PacketSize)
				{
					LOG_BLINE("Invalid PacketSize(%hu)\n", PacketSize);
					m_GpioBtnInputData.remove(0, 1);
				}
				else if(2 > PacketSize)
				{
					LOG_BLINE("Invalid PacketSize(%hu)\n", PacketSize);
					m_GpioBtnInputData.remove(0, 1);
				}
				else if(PacketSize > m_GpioBtnInputData.count())
				{
					break;
				}
				else	//illegal
				{
					BYTE MsgId = m_GpioBtnInputData[2];
					if(MSG_ID_GPIO_INPUT_EVENT_DATA == MsgId)
					{
						BYTE BtnKeyCode = m_GpioBtnInputData[3];
						m_GpioBtnInputData.remove(0, 4);
						On_GpioBtnKey(BtnKeyCode);
					}
					else	//invalid msg id
					{
						LOG_BLINE("Invalid MsgId(%d)\n", MsgId);
						m_GpioBtnInputData.remove(0, 1);
					}
				}
			}
			else	//too less data
			{
				break;
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::OnTimer_checkVolumeWin()
{
	m_HideVolumeBarTimer.stop();
	if(!m_TurnVolumeWin.isHidden())
	{
		m_TurnVolumeWin.setVisible(false);
	}
}

void QWebBrowserApp::OnTimer_checkPlayDurationTimeOut()
{
	int iRet= ERROR_SUCCESS,iOutRet;
	if(m_PlayDurationTimer.isActive())
	{
		m_PlayDurationTimer.stop();
	}
	iRet = StopPlayback();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	if(m_NextFileInfoToPlay.m_iLoopTimes)
	{
		playMedia((ActiveMedia_TYPE)m_NextFileInfoToPlay.m_iDeviceType, 
			m_NextFileInfoToPlay.m_strFileType,m_NextFileInfoToPlay.m_strFilename, 
			iOutRet,m_NextFileInfoToPlay.m_iLoopTimes,m_NextFileInfoToPlay.m_iDuration,
			m_NextFileInfoToPlay.m_iPosition,"",true,m_NextFileInfoToPlay.m_bIsVideoLayer);
	}
	else
	{
#if 1 //Added by jaylon for loop 
		if(m_NextFileInfoToPlay.m_bIsInitZero && !m_NextFileInfoToPlay.m_bIsInitOne )
		{
			int iOutRet;
			playMedia((ActiveMedia_TYPE)m_NextFileInfoToPlay.m_iDeviceType, 
				m_NextFileInfoToPlay.m_strFileType,m_NextFileInfoToPlay.m_strFilename, 
				iOutRet,m_NextFileInfoToPlay.m_iLoopTimes,m_NextFileInfoToPlay.m_iDuration,
				m_NextFileInfoToPlay.m_iPosition,"",true,m_NextFileInfoToPlay.m_bIsVideoLayer);
		}
#endif 
		if(m_bIsHavePlayItemTemp)
		{
			PlayItemTemporary();
		}
	}
}

void QWebBrowserApp::OnTimer_checkPlaylistDurationTimeOut()
{
	int iRet= ERROR_SUCCESS,iOutRet;
	if(m_PlaylistDurationTimer.isActive())
	{
		m_PlaylistDurationTimer.stop();
	}
	iRet = StopPlayback();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	if(m_bIsHavePlayItemTemp)
	{
		PlayItemTemporary();
	}
	else
	{
		PlayNextEntry();
	}
}

void QWebBrowserApp::On_Timer_checkHtmlDisPlayTimeOut()
{
	int iRet= ERROR_SUCCESS;
    
#if 1 /*Add by xuweiwei 2014-2-21*/
       m_bIsCompletePlaybackEos=TRUE;
#endif

    do
	{
		m_HtmlDurationTimer.stop();	
		if(!m_bIsPlayVideoLayer)
		{
			iRet = StopPlayback();
		}
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
		if(m_bIsPlayWin)
		{
			m_bHasPlayFirst = true;
		}			
		if(!m_bIsPlayVideoLayer)
		{
			if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
			{
				m_PlayPluginListHtmlEntryDurationTimer.stop();
			}
			if(m_PlayPluginListEntryDurationTimer.isActive())
			{
				m_PlayPluginListEntryDurationTimer.stop();
			}
		}
		if(m_NextFileInfoToPlay.m_strFilename.trimmed() != "")
		{
			if(m_NextFileInfoToPlay.m_bIsVideoLayer)
			{
				PlayNextEntry();
			}
			if(FileType_Desc_Html == m_NextFileInfoToPlay.m_strFileType  || FileType_Desc_Playlist == m_NextFileInfoToPlay.m_strFileType  )
			{
				m_NextFileInfoToPlay.m_strFilename = "";
				m_NextFileInfoToPlay.m_strFileType = "";
				m_NextFileInfoToPlay.m_iDeviceType = 0;
				m_NextFileInfoToPlay.m_iLoopTimes = 1;
				m_NextFileInfoToPlay.m_iDuration = 0;
				m_NextFileInfoToPlay.m_iPosition= 0;
			}
			if(!m_NextFileInfoToPlay.m_bIsInitZero && !m_NextFileInfoToPlay.m_bIsInitOne )
			{
				if(m_NextFileInfoToPlay.m_iLoopTimes == 0 ||  m_NextFileInfoToPlay.m_iLoopTimes == 1)
				{
					m_NextFileInfoToPlay.m_strFilename = "";
					m_NextFileInfoToPlay.m_strFileType = "";
					m_NextFileInfoToPlay.m_iDeviceType = 0;
					m_NextFileInfoToPlay.m_iLoopTimes = 1;
					m_NextFileInfoToPlay.m_iDuration = 0;
					m_NextFileInfoToPlay.m_iPosition= 0;
				}
			}
			else if(!m_NextFileInfoToPlay.m_bIsInitZero && m_NextFileInfoToPlay.m_bIsInitOne)
			{
				if(m_NextFileInfoToPlay.m_iLoopTimes == 0)
				{
					m_NextFileInfoToPlay.m_strFilename = "";
					m_NextFileInfoToPlay.m_strFileType = "";
					m_NextFileInfoToPlay.m_iDeviceType = 0;
					m_NextFileInfoToPlay.m_iLoopTimes = 1;
					m_NextFileInfoToPlay.m_iDuration = 0;
					m_NextFileInfoToPlay.m_iPosition= 0;
				}
			}
		}
		else
		{
			PlayNextEntry();
		}
		/**/
		if(m_eContentTrigSrcType == CONTENT_TRIGGER_SRC_DefaultStartupItem &&
			AMedia_TYPE_HTML == m_eCurPlayMediaFileType && m_FristEntryIsHtml)
		{
			ShowBrowserWindow();
		}
	}while(false);
}

void QWebBrowserApp::SwitchToPlayWin()
{
	QWidget * pActiveWindow = NULL;
	int iRet= ERROR_SUCCESS;
	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iRet=CheckPlayEnable();
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif
		
		pActiveWindow = activeWindow();

		if(NULL == pActiveWindow)
		{
			break;
		}
		if(&m_Browser == pActiveWindow)
		{
			printf("[%s:%d] enter switch to play\n",__func__,__LINE__);
			ShowPlayerWindow();
			if(!m_PlayPicAndHtmlWin->isPlayingHtml())
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();	
			}
			if(m_eMediaPlayCurrStatus == MediaPlay_Status_PlayPlayList ||
			  m_eMediaPlayCurrStatus ==  MediaPlay_Status_PlayIdlePlayList)
			{
				ResetPlayListEntryPostionInfo();
				UpdateVideoPosition(true);
			}
			else if(m_eMediaPlayCurrStatus == MediaPlay_Status_PlayCeLabsFile
				 || m_eMediaPlayCurrStatus == MediaPlay_Status_PlaySelectFile )
			{
				if(m_bIsPlayPluginList)
				{
					ShowPlayPicOrHtmlWindow();
					int VideoPosX = 0, VideoPosY = 0, VideoPosW = 0, VideoPosH = 0;
					iRet = m_PlayPicAndHtmlWin->CalcVideoPluginPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
					if(ERROR_SUCCESS != iRet)
					{
						if(ERROR_NOT_FOUND == iRet)
						{
						}
						else
						{
							PRINT_BFILE_LINENO_IRET_STR;
						}
						break;
					}
					if(!m_MediaPlayerCtrl_sp.isNull())
					{
						iRet = m_MediaPlayerCtrl_sp->setVideoPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
					}
				}
				else
				{	
					if(!m_bIsPlayVideoLayer)
					{
						UpdateVideoPosition(true);
					}
				}
			}
			else
			{
				{
					UpdateVideoPosition(true);
				}
			}
			if(m_iStateType == State_PlayingTuner )
			{
				UpdateVideoPosition(true);
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::GetDhcpInfoByDbus(QStringList  & strDhcpInfoList)
{
	QString  strDefaultGatewayAddr,   strDns1,  strDns2 , strIPv4Addr,  SubnetMask;
	QString strDefaultDeviceName;
	bool  bAutoGetIP;
	INT_t iRet;

	do
	{
		mp_Config->getConfigValue(KEY_NetMgrEthernet_IPv4Addr);
		mp_Config->getConfigValue(KEY_NetMgrEthernet_SubnetMask);
		mp_Config->getConfigValue(KEY_NetMgrEthernet_GatewayAddr);
		mp_Config->getConfigValue(KEY_NetMgrEthernet_DNSAddr1);
		mp_Config->getConfigValue(KEY_NetMgrEthernet_DNSAddr2);

		iRet = m_NetMgrEthernetClient_sp->readSettings(strDefaultDeviceName,bAutoGetIP,  strIPv4Addr, SubnetMask);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		iRet  = m_NetMgrClient_sp->readSettings(strDefaultGatewayAddr,strDns1,strDns2);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(bAutoGetIP)
		{
			strDhcpInfoList <<  strIPv4Addr << SubnetMask << strDefaultGatewayAddr << strDns1 << strDns2 << "1" ;
		}
		else
		{
			strDhcpInfoList <<  strIPv4Addr << SubnetMask << strDefaultGatewayAddr << strDns1 << strDns2 << "0" ;
		}
	}while(false);

}

void QWebBrowserApp::SetDhcpInfoByDbus(QStringList dhcpInfoList, bool isAutoGetIp)
{
	INT_t iRet;

	do
	{
		if(dhcpInfoList.size() == 5)
		{
			iRet  = m_NetMgrEthernetClient_sp->applySettingsForDefaultDevice(isAutoGetIp,
				dhcpInfoList[0], dhcpInfoList[1]);

			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}

			iRet  = m_NetMgrClient_sp->applySettings(dhcpInfoList[2], dhcpInfoList[3], dhcpInfoList[4]);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}

			if(mp_Config.isNull())
			{
				break;
			}

			iRet = SysProp_setPersistent(SysProp_KEY_CurEthernetIP, dhcpInfoList[0].toUtf8().constData());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			//
			mp_Config->setConfigValue(KEY_NetMgrEthernet_IPv4Addr, dhcpInfoList[0]);
			mp_Config->setConfigValue(KEY_NetMgrEthernet_SubnetMask, dhcpInfoList[1]);
			mp_Config->setConfigValue(KEY_NetMgrEthernet_GatewayAddr, dhcpInfoList[2]);
			mp_Config->setConfigValue(KEY_NetMgrEthernet_DNSAddr1, dhcpInfoList[3]);
			mp_Config->setConfigValue(KEY_NetMgrEthernet_DNSAddr2, dhcpInfoList[4]);

			int iRet;
			BOOL_t bEnableWriteLoggin = TRUE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			if(bEnableWriteLoggin)
			{
				char strTemp[64];
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp,"%d",TYPE_LOG_IPChange);
				CString::STD_STRING_LIST strList ;
				strList.push_back(strTemp);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				strList.push_back(dhcpInfoList[0].toLocal8Bit().constData());
				if(ERROR_SUCCESS !=WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				else
				{
					strList.clear();
				}
			}
		}
	}while(false);
}

void QWebBrowserApp::SetVideoParInfoByDbus(QString paraName ,int paraValue)
{
	Q_UNUSED(paraName);
	Q_UNUSED(paraValue);
	
}

void QWebBrowserApp::GetNetWorkInfo(QStringList & netWorkInfoList)
{
	QString  strDefaultGatewayAddr, strIPv4Addr,  SubnetMask;
	QString strDefaultDeviceName;
	QString domain;
	QString strDns;
	INT_t iRet;

	do
	{
		if(!m_NetMgrClient_sp)
		{
			iRet = 1;
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		iRet = m_NetMgrEthernetClient_sp->getDefaultDeviceInfo(strDefaultDeviceName, strIPv4Addr, SubnetMask);

		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		iRet  = m_NetMgrClient_sp->getDefaultGatewayAddr(strDefaultGatewayAddr);
		if(ERROR_NOT_FOUND == iRet)
		{
			strDefaultGatewayAddr = "";
		}
		else if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		m_NetMgrClient_sp->getResolveConf(domain, strDns);

		QStringList strDnsList = strDns.split(";");

		if(strDnsList.size() >= 2)
		{
		     netWorkInfoList <<  strIPv4Addr << SubnetMask << strDefaultGatewayAddr << strDnsList[0] << strDnsList[1];
		}
		else if(strDnsList.size() > 0)
		{
			netWorkInfoList <<  strIPv4Addr << SubnetMask << strDefaultGatewayAddr << strDnsList[0]  << " ";
		}
		else
		{
			netWorkInfoList <<  strIPv4Addr << SubnetMask << strDefaultGatewayAddr << ""  << " ";
		}

		bool  bAutoGetIP;

		iRet = m_NetMgrEthernetClient_sp->readSettings(strDefaultDeviceName,bAutoGetIP,  strIPv4Addr, SubnetMask);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		if(bAutoGetIP)
		{
			netWorkInfoList << "1";
		}
		else
		{
			netWorkInfoList << "0";
		}
	}while(false);
}

void QWebBrowserApp::SetSysTimeInfo(QStringList timeInfoList)
{
	INT_t iRet;
	if(timeInfoList.size() == 7)
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet  = m_MediaPlayerCtrl_sp->setHwRtcTime(timeInfoList[0].toInt(),
				 timeInfoList[1].toInt(),timeInfoList[2].toInt(),timeInfoList[3].toInt(),
				timeInfoList[4].toInt(),timeInfoList[5].toInt(),timeInfoList[6].toInt());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}

}


int QWebBrowserApp::GetSysTimeZone (int & currTimeZone)
{
	INT_t iRet;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet  = m_MediaPlayerCtrl_sp->getTimeZoneSetting(currTimeZone);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
	}while(false);	
	
	return currTimeZone;
}

int QWebBrowserApp::Load_JsFile()
{
	int iOutRet = ERROR_SUCCESS;
	bool bRet;

	do
	{
		QString fileName(APP_JavaScript_FILE_PATH);
		QFile scriptFile(fileName);
		bRet = scriptFile.open(QIODevice::ReadOnly);
		if(false == bRet)
		{
			LOG_BLINE("Failed to open %s,%s\n", APP_JavaScript_FILE_PATH, qPrintable(scriptFile.errorString()));
			iOutRet = ERROR_NOT_FOUND;
			break;
		}

		QTextStream stream(&scriptFile);
		QString contents = stream.readAll();
		scriptFile.close();
		m_ScriptEng_sp = QSharedPointer <QScriptEngine> (new QScriptEngine());
		if(m_ScriptEng_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		QScriptValue scriptObj = m_ScriptEng_sp->newQObject(this);
     		m_ScriptEng_sp->globalObject().setProperty("WebBrowserApp", scriptObj);	//透露给JS对象
		QScriptValue result = m_ScriptEng_sp->evaluate(contents, fileName);	//先解析一次脚本
		if(result.isError())
		{
			if(m_ScriptEng_sp->hasUncaughtException())
			{
				QScriptValue ExceptionValue;
				int LineNo = m_ScriptEng_sp->uncaughtExceptionLineNumber();
				ExceptionValue = m_ScriptEng_sp->uncaughtException();
				//LOG_BLINE("JsExcept,Line:%d,%s\n", LineNo, qPrintable(ExceptionValue.toString()));
				if(Sw_LogJavascript)
				{
					LOG_BLINE("JsExcept,Line:%d,%s\n", LineNo, qPrintable(ExceptionValue.toString()));
				}
				break;
			}
		}
	}while(false);

	return iOutRet;
}

int QWebBrowserApp::OpenPlaylist(const QString & playListName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do
	{
		//LOG_BLINE("%s\n", qPrintable(playListName));
		if(playListName.trimmed() == "" || !QFile::exists(playListName))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		StopPlayback();
		m_eMediaPlayCurrStatus = MediaPlay_Status_PlayPlayList;		
		m_BIsPlaySplash = false;
		iRet = m_PlaylistDataA.LoadFromIniFile(&playListName);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		else
		{
			QFileInfo fileInfo(playListName);
			m_strCurrPlayListUrl = playListName;
			m_strCurrPlayListName = fileInfo.fileName();	
			m_eCurPlayMediaFileType = AMedia_TYPE_PLAYLIST;
			
		}
		m_bIsNeedLoop = false;
		PlayNextEntry(true);
		iOutRet = iRet;
	}while(false);

	return iOutRet;
}

void  QWebBrowserApp::SetGpioLedStatus(unsigned int setLedStatus)
{
	INT_t iRet = ERROR_SUCCESS;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		LOG_BLINE("setLedStatus:%x\n", setLedStatus);
		iRet = m_MediaPlayerCtrl_sp->setGpioLedStatus(setLedStatus);
		if(ERROR_SUCCESS != iRet && ERROR_I2C_READ_FAIL != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
	}while(false);
}

void QWebBrowserApp::GetMonitorTypeInfoByDbus(QStringList & monitorTypeList)
{
	INT_t iRet = ERROR_SUCCESS;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		QString str_MonitorType;
		iRet = m_MediaPlayerCtrl_sp->getMonitorTypeInfo(OUT str_MonitorType);
		if(ERROR_SUCCESS != iRet)
		{
			str_MonitorType.clear();
			break;
		}
		monitorTypeList << str_MonitorType;
	}while(false);
}


void QWebBrowserApp::GetVersionInfo(QStringList & versionInfoList)
{
	QString strOutPutMode;
	
	QAudioVideoInOutInfo::getVoDesc(m_iVideoOutMode, OUT strOutPutMode);

	QString strAppVersion =m_strXappVersion;
			
	//	QString strAppVersion =m_strXappVersion;
	QString strHardWareVersion = mp_AppInfo_Config ->getConfigValue(INI_KEY_HardWareVersion	);
	QString strFirmWareVersion = mp_AppInfo_Config ->getConfigValue(INI_KEY_FirmVersion	);
	QString strBrowserVersion = mp_AppInfo_Config ->getConfigValue(INI_KEY_BrowserVersion);

	versionInfoList << strOutPutMode <<strHardWareVersion<<  strFirmWareVersion << strAppVersion << strBrowserVersion;
}

void QWebBrowserApp::setPlayListActionPosition(int x, int y, int width, int height)
{
	INT_t iRet;
	QDesktopWidget * pDesktopWidget = desktop();

	iRet = 0;
	do
	{
		if(NULL == pDesktopWidget)
		{
			break;
		}

		if(m_bIsPlayVideoLayer)
		{
			if(x ==0 && y ==0 && width == -1 && height == -1)
			{
				setVideoLayerWndSize();
				break;
			}
		}
		
		if(-1 == width)
		{
			width = 100;
		}
		if(-1 == height)
		{
			height = 100;
		}

		QRect DesktopRect = pDesktopWidget->screenGeometry();
		int HardwareX, HardwareY, HardwareWidth, HardwareHeight;

		HardwareX = (x * VideoSurface_WIDTH / 100);
		HardwareY = (y * VideoSurface_WIDTH / 100);
		HardwareWidth = (width * VideoSurface_HEIGHT / 100);
		HardwareHeight = (height * VideoSurface_HEIGHT / 100);
		
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setVideoPosition(HardwareX, HardwareY, HardwareWidth, HardwareHeight);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				m_bHasPlayFirst = false;
				//every entery will this size
				if(m_bIsPlayVideoLayer)
				{
					int iCurrCorrdinateMode = mp_Config->getConfigValue(INI_KEY_CoordinateMode).toInt();
				
					if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
					{
						HardwareX = (x *  DesktopRect.width() / 100);
						HardwareY = (y *  DesktopRect.height() / 100);
						HardwareWidth = (DesktopRect.width()  * width /100);
						HardwareHeight = (DesktopRect.height()  * height /100);
					}
					else
					{
						HardwareX = (x *  DesktopRect.width() / VideoSurface_WIDTH);
						HardwareY = (y *  DesktopRect.height() / VideoSurface_WIDTH);
						HardwareWidth = (width * DesktopRect.width() / VideoSurface_WIDTH);
						HardwareHeight = (height * DesktopRect.width() / VideoSurface_WIDTH );
					}
					
					m_playWin.getVideoLayerWnd()->move(HardwareX, HardwareY);
					m_playWin.getVideoLayerWnd()->resize(HardwareWidth, HardwareHeight);
				}
			}
			
		}
	}while(FALSE);
}

void QWebBrowserApp::setPlayMode(CONST int PlayMode, int & iOutRet)
{
	int iRet = ERROR_SUCCESS;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(ControlCenter::PlayMode_Pause == PlayMode)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iRet = m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_Pause);
				if(ERROR_SUCCESS == iRet)
				{
					m_eCurPlayMode = ControlCenter::PlayMode_Pause;
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PAUSE,1);
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		else if(ControlCenter::PlayMode_Normal == PlayMode)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_Normal);
				if(ERROR_SUCCESS == iOutRet)
				{
					m_eCurPlayMode = ControlCenter::PlayMode_Normal;
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
				}
				else
				{
					iRet = iOutRet;
					if(ERR_INVALID_STATE != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
			}
		}
		else if(ControlCenter::PlayMode_FastForward == PlayMode)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_FastForward);
				if(ERROR_SUCCESS == iOutRet)
				{
					m_eCurPlayMode = ControlCenter::PlayMode_FastForward;
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_FAST,1);
				}
			}
		}
		else if(ControlCenter::PlayMode_Rewind == PlayMode)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				m_eCurPlayMode = ControlCenter::PlayMode_Rewind;
				iOutRet = m_MediaPlayerCtrl_sp->setPlayMode(MediaPlayer::PlayMode_Rewind);
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_REW,1);
			}
		}
		else if(ControlCenter::PlayMode_StopAll == PlayMode)
		{
			if(m_bIsPlayingIdlePlayList)
			{
				m_bIsPlayingIdlePlayList = false;				
			}
			On_DefKey_Down(IR_KEY_STOP);
			if(!m_PluginPlaylistData_sp.isNull())
			{
				m_PluginPlaylistData_sp->ClearPlayListData();
			}
			m_PlaylistDataA.ClearPlayListData();
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(pActiveWindow== &m_playWin)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				UpdateVideoPosition(true);
			}
			{
				//playMode 1 has no effect. playmode 7 stops play but keeps open OSD cannot find way to turn off OSD with playMode. - not consistent with MP500
				m_iStateType = State_Idle;
				m_eCurPlayMode = ControlCenter::PlayMode_StopAll;
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
				m_PlaylistDataA.ClearPlayListData();
				SwitchToPlayWin();
				bool bFullScreen = true;
				UpdateVideoPosition(bFullScreen);
				m_NextFileInfoToPlay.m_strFilename = "";				
			} 
			if(m_bIsPlayVideoLayer)
			{
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->lower();
			}
			m_bIsPlayVideoLayer = false;				
		} 
		else if(ControlCenter::PlayMode_StopMenu == PlayMode)
		{
			SwitchToPlayWin();
		}
		else if(ControlCenter::PlayMode_ChapterPlus == PlayMode)
		{
			//SwitchToPlayWin();
			if(State_PlayingPlayList == m_iStateType)
			{
				PlayNextEntry();
			}
		}	
		else if(ControlCenter::PlayMode_ChapterMinus == PlayMode)
		{
			//SwitchToPlayWin();
			if(State_PlayingPlayList == m_iStateType)
			{
				PlayNextEntry(false,true);
			}
		}	
		else if(ControlCenter::PlayMode_StopContent == PlayMode)
		{
			if(m_bIsPlayVideoLayer)
			{
				if(!m_MediaPlayerCtrl_sp.isNull())
				{
					iOutRet = stopMPlayerWithEventLog();
				}				
				break;
			}
			
			m_PlaybackId = 0;
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(pActiveWindow == &m_Browser)
			{
				StopPlayDurationTimer();
				StopPlayback();
				m_iStateType = State_Idle;
				m_eCurPlayMode = ControlCenter::PlayMode_StopContent;
			}
			else
			{
				On_DefKey_Down(IR_KEY_STOP);				
				m_iStateType = State_Idle;
				m_eCurPlayMode = ControlCenter::PlayMode_StopContent;
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
				m_PlaylistDataA.ClearPlayListData();
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				SwitchToPlayWin();
				bool bFullScreen = true;
				UpdateVideoPosition(bFullScreen);
			}
			if(m_bIsPlayVideoLayer)
			{
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->lower();
			}
			m_bIsPlayVideoLayer = false;	
		}
		else if(ControlCenter::PlayMode_StopVideoLayer == PlayMode)
		{
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = stopMPlayerWithEventLog();
			}	
			
			if(m_bIsPlayVideoLayer)
			{
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();				
				m_playWin.getVideoLayerWnd()->hide();
				m_playWin.getVideoLayerWnd()->close();
			}

			m_bIsPlayVideoLayer = false;	
		}
		else if(ControlCenter::PlayMode_StopHtmlLayer == PlayMode)
		{
			StopAllPlayDurationTimer();
			m_playWin.getPlayHtmlOrPicWin()->playHtmlFileEnd();
			m_playWin.getPlayHtmlOrPicWin()->close();
		}
		else
		{
			iOutRet = ERR_INVALID_CMD;
		}
	}while(false); 
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setPlayMode(%d) ret %d\n", PlayMode, iOutRet);
	}
}

INT_t QWebBrowserApp::SavePlaylistAsPersistent(const QString & strPlaylistAFilePath, const QString & strPlaylistBFilePath/* = ""*/)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_CurPlaylistAFilePath, strPlaylistAFilePath);
		mp_Config->setConfigValue(INI_KEY_CurPlaylistBFilePath, strPlaylistBFilePath);
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::SaveIdlePlaylist(const QString & strIdlePlaylistFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_CurIdlePlaylistFilePath, strIdlePlaylistFilePath);
	}while(FALSE);

	return iOutRet;
}



//Caller must lock
QString QWebBrowserApp::getDevMediaDirNameByType(CONST ActiveMedia_TYPE eDeviceType)
{
	QString strDevMediaDirName;

	do
	{
		if(AMedia_TYPE_FLASH == eDeviceType)
		{
			strDevMediaDirName = LOCAL_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_SdCard == eDeviceType)
		{
			strDevMediaDirName = SD_CARD_DIR_NAME;
		}
		else if(AMedia_TYPE_USB == eDeviceType)
		{
			strDevMediaDirName = USB_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_HDD == eDeviceType)
		{
			strDevMediaDirName = StorageDirName_HDD;
		}
		else if(AMedia_TYPE_RAM == eDeviceType)
		{
			strDevMediaDirName = StorageDirName_RAM;
		}
	}while(FALSE);

	return strDevMediaDirName;
}

INT_t QWebBrowserApp::QueueNextFileToPlay(const QString & strFileFullPath, const QString & strFileType,int eDeviceType,int  iLoopTimes,int iDuration,int iPosition,bool bIsVideoLayer)
{
	INT_t iOutRet = ERROR_SUCCESS;
	do
	{
		{
			QAutoLock AutoRangeLocker(&m_SharedDataMutex);
			m_NextFileInfoToPlay.m_strFilename = strFileFullPath;
			m_NextFileInfoToPlay.m_strFileType = strFileType;
			m_NextFileInfoToPlay.m_iLoopTimes = iLoopTimes;
			m_NextFileInfoToPlay.m_iDuration = iDuration;
			m_NextFileInfoToPlay.m_iDeviceType = eDeviceType;
			m_NextFileInfoToPlay.m_bIsVideoLayer = bIsVideoLayer;
			m_NextFileInfoToPlay.m_iPosition = iPosition;
		}
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::palySpecificTime(int & SpecificTime)
{
	INT_t iOutRet = ERROR_SUCCESS;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		iOutRet = m_MediaPlayerCtrl_sp->Seek(SpecificTime);
		if(iOutRet != ERROR_SUCCESS)
		{
			break;
		}
	}while(false);
}

void QWebBrowserApp::getCurrentPlayItem(QString & strFileName,int & iOutRet)
{
	
	do
	{		
		QString strPlayType , strPlayFileName,strPlayFileInfo,strPlayListName;
		QString playListUrl = m_strDefaultStartupItem;//mp_Config ->getConfigValue(INI_KEY_DefaultStartupItem);
		if(playListUrl.trimmed()!="")
		{
			QFileInfo playListInfo(playListUrl);
		 	playListUrl = playListInfo.fileName();
		}
		getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
		if(strPlayFileInfo.trimmed()!="")
		{
			if(strPlayType.trimmed() == FileType_Stream)
			{
				strPlayFileName  = strPlayFileInfo;
			}
			else
			{
				QFileInfo playListInfo(strPlayFileInfo);
				strPlayFileName = playListInfo.fileName();
				if(strPlayFileName=="")
				{
					strPlayFileName  = strPlayFileInfo;
				}
			}
		}
		else
		{
			strPlayFileName ="";
		}

		if(m_iStateType == State_Idle)
		{
			playListUrl = "";
		}
		else
		{
			if(m_eMediaPlayCurrStatus == MediaPlay_Status_PlayPlayList||
			   m_eMediaPlayCurrStatus == MediaPlay_Status_PlayCeLabsFile ||
			   m_eMediaPlayCurrStatus == MediaPlay_Status_PlayIdlePlayList)
			{
				playListUrl = m_PlaylistDataA.getCurrentPlayListName();
				if(playListUrl.trimmed()=="")
				{
					playListUrl = m_strCurrPlayListName ;
				}
			}
			else
			{
				playListUrl = "";
			}
		}
		QString str_playlist_fname = "";
		QString str_playlist_ControlName = "";
		/*
		if(m_iStateType == State_Idle)
		{
			str_playlist_fname = "";
		}
		else
		*/
		if(m_bIsPlayPluginList ||m_bIsPlayVideoLayer)
		{
			str_playlist_fname = m_strCurrPluginListName;
			if(str_playlist_fname.trimmed() != "")
			{
				str_playlist_ControlName = m_strCurrPluginListControlName;
				//LOG_BLINE("str_playlist_ControlName = %s\n",qPrintable(str_playlist_ControlName));
			}
		}
		else
		{
			str_playlist_fname = m_strCurrPlayListName;
			if(str_playlist_fname.trimmed() != "")
			{
				str_playlist_ControlName = m_PlaylistDataA.getCurrentPlayListName();
				//LOG_BLINE("str_playlist_ControlName = %s\n",qPrintable(str_playlist_ControlName));
			}
		}
		if(m_bIsPlayPluginList)
		{
			QString strHtmlFileName = "";
			strHtmlFileName= m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
			QFileInfo playListInfo(strHtmlFileName);
			strHtmlFileName = playListInfo.fileName();	
			
			if(m_strCurrPluginListUrl.trimmed() == "")
			{
				playListUrl = m_strCurrPluginListUrl ;
			}
			else
			{
				QFileInfo playListUrlInfo(m_strCurrPluginListUrl);					
				if(playListUrlInfo.fileName().trimmed() !="")
				{
					playListUrl  = playListUrlInfo.fileName();
				}
			}
			
			strFileName = str_playlist_fname +"," + str_playlist_ControlName +"," +FOLDER_NAME_HTML+ ","
				+strHtmlFileName+ "," + QString::number(State_PlayingBrowserFile)+","
				+m_strCurrPluginListName +"," + playListUrl +"," +strPlayType+ ","
				+strPlayFileName+ "," + QString::number(m_iStateType);

			if(State_PlayingTuner == m_iStateType)
			{
				int iCurrentSignalLevel = getTunerChannelStrength();
				iCurrentSignalLevel = iCurrentSignalLevel * 20 ;
				
				strFileName += QString(" ")  + QString::number(iCurrentSignalLevel);
			}
		}
		else if(m_bIsPlayVideoLayer)
		{
			QString strHtmlFileName = "";
			strHtmlFileName= m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
			QFileInfo playListInfo(strHtmlFileName);
			
			if(playListInfo.fileName().trimmed() !="")
			{
				strHtmlFileName =playListInfo.fileName();
			}
			if(m_strCurrPluginListUrl.trimmed() == "")
			{
				playListUrl = m_strCurrPluginListUrl ;
			}
			else
			{
				QFileInfo playListUrlInfo(m_strCurrPluginListUrl);					
				if(playListUrlInfo.fileName().trimmed() !="")
				{
					playListUrl  = playListUrlInfo.fileName();
				}
			}
			
			strFileName = str_playlist_fname +"," + str_playlist_ControlName +"," +FOLDER_NAME_HTML+ ","
				+strHtmlFileName+ "," + QString::number(State_PlayingBrowserFile)+","
				+m_strCurrPluginListName +"," + playListUrl +"," +strPlayType+ ","
				+strPlayFileName+ "," + QString::number(m_iStateType);
		}
		else
		{
			QWidget * pCurrActiveWindow = NULL;
			pCurrActiveWindow = activeWindow();	
			 if(pCurrActiveWindow== &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml())
			{
				QString strHtmlFileName = "";
				strPlayType = FOLDER_NAME_HTML;
				strHtmlFileName= m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
				QFileInfo playListInfo(strHtmlFileName);
				if(playListInfo.fileName().trimmed() !="")
				{
					strHtmlFileName  = playListInfo.fileName();
				}
				QFileInfo playListUrlInfo(playListUrl);
				playListUrl = playListUrlInfo.fileName();
				if(playListUrl.trimmed()=="")
				{
					playListUrl  = playListUrl;
				}					
				if(State_PlayingPictureFile == m_iStateType)
				{						
					strPlayType = FOLDER_NAME_PICTURE;
					strFileName = strHtmlFileName +"," + "" +"," +strPlayType+ ","
					+strHtmlFileName+ "," + QString::number(State_PlayingPictureFile)
					+",,,,,";
				}
				else
				{
					strFileName = str_playlist_fname +"," + str_playlist_ControlName +"," +strPlayType+ ","
					+strHtmlFileName+ "," + QString::number(State_PlayingBrowserFile)
					+",,,,,";
				}
			}
			else
			{
				strFileName = str_playlist_fname +"," + str_playlist_ControlName +"," +strPlayType+ ","
					+strPlayFileName+ "," + QString::number(m_iStateType)
					+",,,,,";
			}
			/**/
			if(State_PlayingTuner == m_iStateType)
			{
				//FOLDER_NAME_TUNER
				int iCurrentSignalLevel = getTunerChannelStrength();
				iCurrentSignalLevel = iCurrentSignalLevel * 20 ;
				//str_playlist_fname = "";
				playListUrl = "" ;
				strFileName =str_playlist_fname +"," + str_playlist_ControlName +"," + FOLDER_NAME_TUNER 
				+ ","	+strPlayFileName+ "," + QString::number(m_iStateType) + " " + QString::number(iCurrentSignalLevel)
					+",,,,,";
			}			
		}
		iOutRet = ERROR_SUCCESS;
	}while(false);
}

#if 1/*added by lxj 2012-10-10*/
void QWebBrowserApp::getCurrentPlayItemInfo(State_TYPE & iStateType,QString & strUrl)
{
	do
	{
		QString strPlayType;
		if(m_bIsPlayPluginList){
			iStateType = State_PlayingBrowserFile;
			strUrl = m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
		}else if( m_strCurrPlayListUrl != "" ){
			iStateType = State_PlayingPlayList;
			strUrl = m_strCurrPlayListUrl;			
		}else{
			iStateType = m_iStateType;
			getCurrentPlayEntryInfo(strPlayType,strUrl);
		}
	}while(false);
}
#endif
void QWebBrowserApp::setPlayVolume(int iVolumeValue,int & iOutRet)
{
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_MediaPlayerCtrl_sp->OnSaveVolume(iVolumeValue);
		//LOG_BLINE("inputVolumeValue = %d\n",iVolumeValue);
		
		if(iOutRet == ERROR_SUCCESS)
		{
			iOutRet = m_MediaPlayerCtrl_sp->changeVolume(iVolumeValue);
			if(ERROR_SUCCESS == iOutRet)
			{
				m_bVolumeMuted = FALSE;
			}
		}
	}while(false);
}

void QWebBrowserApp::getCurrentVolume(int &iCurVolume,int & iOutRet)
{
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iOutRet = m_MediaPlayerCtrl_sp->getCurrentVolume(&iCurVolume);
		//LOG_BLINE("iCurVolume = %d\n",iCurVolume);
	}while(false);
}

void QWebBrowserApp::setPlayerMute(int iMuted,int &iOutRet)
{
	INT_t iCurVolume = 50;

	iOutRet = ERROR_SUCCESS;

	m_MediaPlayerCtrl_sp->getCurrentVolume(&iCurVolume);

	do
	{
		if(iMuted == 1)
		{
			if(FALSE == m_bVolumeMuted)
			{
				iOutRet = m_MediaPlayerCtrl_sp->changeVolume(-iCurVolume);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}

				m_bVolumeMuted = TRUE;
			}
		}
		else if(iMuted == 0)
		{
			if(m_bVolumeMuted)
			{
				iOutRet = m_MediaPlayerCtrl_sp->changeVolume(iCurVolume);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				m_bVolumeMuted = FALSE;
			}
			else					
			{
				iOutRet = m_MediaPlayerCtrl_sp->changeVolume(iCurVolume);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
			}
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
	}while(false);
}

void QWebBrowserApp::getPlayerMute(int & iOutRet, int & bMuted)
{
	iOutRet = ERROR_SUCCESS;
	bMuted = m_bVolumeMuted;
}

void QWebBrowserApp::getPlayState(int & iOutRet,int &iState)
{
	INT_t iRet = ERROR_SUCCESS ;

	do
	{
		iState = m_iStateType;
		if(iState < 1)
		{
			iOutRet = 0;
		}
		else
		{
			iOutRet = 1;
		}
	}while(FALSE);
	iOutRet = iRet;
}

void QWebBrowserApp::getCurrentPlayEntryInfo(QString & strEntryType,QString & strEntryFileName)
{
	
	do
	{
		if(m_iStateType == State_Idle)
		{
			strEntryType = "Idle";
			strEntryFileName ="";
			break;
		}
		else if(m_iStateType == State_PlayingBrowserFile)
		{
			strEntryType = FOLDER_NAME_HTML;
			strEntryFileName = m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
			if(strEntryFileName=="" || m_bIsPlayPluginList)
			{
				strEntryFileName =  m_strCurPlayUrl ;	
			}
			if(m_bIsPlayVideoLayer)
			{
				strEntryFileName = m_strCurPlayUrl;
			}
			break;
		}
		else
		{
			if(m_iStateType == State_PlayingVideoFile)
			{
				strEntryType = FOLDER_NAME_VIDEO;
			}
			else if(m_iStateType == State_PlayingAudioFile)
			{
				strEntryType = FOLDER_NAME_MUSIC;
			}
			else if(m_iStateType == State_PlayingPictureFile)
			{
				strEntryType = FOLDER_NAME_PICTURE;
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(m_HtmlDurationTimer.isActive() && pActiveWindow == &m_playWin)
				{
					if(m_PlayPicAndHtmlWin->isPlayingPicture())
					{
						strEntryFileName = m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();
						break;
					}
				}
			}
			else if(m_iStateType == State_UserOSD)
			{
				strEntryType = "UserOSD";
			}
			else if(m_iStateType == State_MenuOSD)
			{
				strEntryType = "MenuOSD";
			}
			else if(m_iStateType == State_ImageWhileStreaming ||
				m_iStateType == State_Streaming)
			{
				strEntryType = FileType_Stream ;
			}
			else if(m_iStateType == State_PlayingPlayList)
			{
				strEntryType = FOLDER_NAME_PLAYLIST ;
			}
#if 1/*added by lxj 2012-10-9*/
			else if(m_iStateType == State_USB_Capture)
			{
				strEntryType = FOLDER_NAME_USB_CAPTURE ;
			}
#endif
			else if(m_iStateType == State_PlayingTuner)
			{
				strEntryType = FOLDER_NAME_TUNER ;
			}
			else if(m_iStateType == State_UnknownError ||
				 m_iStateType == State_FilePlayError ||
				m_iStateType == State_PlayerError )
			{
				strEntryType = "Error";
			}
			strEntryFileName =  m_strCurPlayUrl ;
			break;
		}
	}while(false);
}

#if 1/*add by lxj 2012-7-20 for get last status type string */
void QWebBrowserApp::getLastPlayEntryInfo(QString & strEntryType,unsigned int PlaybackId)
{
	UINT i=0;
	State_TYPE iStateType=State_Unknown;
	pMediaType p=(pMediaType)(&m_StateTypeList);

	do
	{
		for(i=0;i<MAX_MEDIATYPE_LIST;i++){
			if(p[i].PlaybackId == PlaybackId ){
				iStateType = p[i].iStateType;
				break;
			}
		}

		if(iStateType == State_Idle)
		{
			strEntryType = "Idle";
			break;
		}
		else if(iStateType == State_PlayingBrowserFile)
		{
			strEntryType = FOLDER_NAME_HTML;
			break;
		}
		else
		{
			if(iStateType == State_PlayingVideoFile)
			{
				strEntryType = FOLDER_NAME_VIDEO;
			}
			else if(iStateType == State_PlayingAudioFile)
			{
				strEntryType = FOLDER_NAME_MUSIC;
			}
			else if(iStateType == State_PlayingPictureFile)
			{
				strEntryType = FOLDER_NAME_PICTURE;
			}
			else if(iStateType == State_UserOSD)
			{
				strEntryType = "UserOSD";
			}
			else if(iStateType == State_MenuOSD)
			{
				strEntryType = "MenuOSD";
			}
			else if(iStateType == State_ImageWhileStreaming ||
				iStateType == State_Streaming)
			{
				strEntryType = FileType_Stream ;
			}
			else if(iStateType == State_PlayingPlayList)
			{
				strEntryType = FOLDER_NAME_PLAYLIST ;
			}
#if 1/*added by lxj 2012-10-9*/
			else if(iStateType == State_USB_Capture)
			{
				strEntryType = FOLDER_NAME_USB_CAPTURE ;
			}
#endif
			else if(iStateType == State_UnknownError ||
				 iStateType == State_FilePlayError ||
				iStateType == State_PlayerError )
			{
				strEntryType = "Error";
			}else{
				strEntryType = "";
			}
			break;
		}

	}while(false);
}
#endif

void QWebBrowserApp::setDefaultIdlePlaylist(QString strIdlePlaylist, int & iOutRet)
{
	INT_t iRet;
	QString strPlaylistFullPath ;
	iOutRet = ERROR_SUCCESS;
	QString strDstStorageDirName,strDstFileName;
	strDstFileName = "";

	do
	{
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(strIdlePlaylist.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = getPlaylistFullPath(strIdlePlaylist, strPlaylistFullPath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(!QFile::exists(strPlaylistFullPath))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		iRet = SaveIdlePlaylist(strPlaylistFullPath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(false);

}

void QWebBrowserApp::setIdleDetect(int iIdleSeconds,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_IdleDetectTime, QString::number(iIdleSeconds));
	}while(FALSE);

}

void QWebBrowserApp::setVideoPosition(int iPosType,int iPosValue,int & iOutRet)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;
	iRet = ERROR_SUCCESS;
	int HardwareX,HardwareY,HardwareWidth,HardwareHeight;
	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(iPosValue > VideoSurface_WIDTH)
		{
			iRet =ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = m_MediaPlayerCtrl_sp->getVideoPosition(RUA_DISP_VIDEO, &HardwareX,
					&HardwareY,&HardwareWidth,&HardwareHeight);
	
       		 int iCurrCorrdinateMode = mp_Config->getConfigValue(INI_KEY_CoordinateMode).toInt();

		switch(iPosType)
		{
			case AMedia_POSTION_X:
				{
					mp_Config->setConfigValue(INI_KEY_Xpos, QString::number(iPosValue));
					if(!m_MediaPlayerCtrl_sp.isNull())
					{
						if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
						{
							iPosValue = (iPosValue* VideoSurface_WIDTH / 100);
						}
						iRet = m_MediaPlayerCtrl_sp->setVideoPosition(iPosValue,HardwareY,HardwareWidth,HardwareHeight);
					}
				}
				break;
			case AMedia_POSTION_Y:
				{
					mp_Config->setConfigValue(INI_KEY_Ypos, QString::number(iPosValue));
					if(!m_MediaPlayerCtrl_sp.isNull())
					{
						if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
						{
							iPosValue = (iPosValue* VideoSurface_WIDTH / 100);
						}
						iRet = m_MediaPlayerCtrl_sp->setVideoPosition(HardwareX,iPosValue,HardwareWidth,HardwareHeight);
					}
				}
				break;
			case AMedia_POSTION_XSIZE:
				{
					mp_Config->setConfigValue(INI_KEY_XSize, QString::number(iPosValue));
					if(!m_MediaPlayerCtrl_sp.isNull())
					{
						if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
						{
							iPosValue = (iPosValue* VideoSurface_WIDTH / 100);
						}
						iRet = m_MediaPlayerCtrl_sp->setVideoPosition(HardwareX,HardwareY,iPosValue,HardwareHeight);
					}
				}
				break;
			case AMedia_POSTION_YSIZE:
				{
					mp_Config->setConfigValue(INI_KEY_YSize, QString::number(iPosValue));
					if(!m_MediaPlayerCtrl_sp.isNull())
					{
						if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
						{
							iPosValue = (iPosValue* VideoSurface_WIDTH / 100);
						}
						iRet = m_MediaPlayerCtrl_sp->setVideoPosition(HardwareX,HardwareY,HardwareWidth,iPosValue);
					}
				}
				break;
			default:
				break;
		}
	}while(false);
	iOutRet = iRet;
	if(iRet == ERROR_SUCCESS)
	{
		if(m_bIsPlayVideoLayer)
		{
			setVideoLayerWndSize();
		}
	}
}

void QWebBrowserApp::setDisplayParam(int eDispParamType, INT_t iValue,int &iOutRet,int bIsSave)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		/*if(iValue > 100 || iValue < 20)
		{
			iOutRet = ERR_INVALID_CMD;
			break;
		}*/
		
		if(DispPARAM_Brightness == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_Brightness,iValue,bIsSave);
			}
		}
		else if(DispPARAM_Contrast == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_Contrast,iValue,bIsSave);
			}
		}
		else if(DispPARAM_Saturation == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_Saturation,iValue,bIsSave);
			}
		}
		else if(DispPARAM_Hue == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_Hue,iValue,bIsSave);
			}
		}
		else if(DispPARAM_ColorSpace == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_ColorSpace,iValue,bIsSave);
			}		
		}
		else if(DispPARAM_ColorDepth == eDispParamType)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = m_MediaPlayerCtrl_sp->setDisplayParam(DispPARAM_ColorDepth,iValue,bIsSave);
			}

		}
		else
		{
			iOutRet = ERR_INVALID_CMD;
		}		
	}while(false);
}

INT_t QWebBrowserApp::getDisplayParam(int eDispParamType,int & iCurrValue)
{
	INT_t iRet;
	INT32_t CurVol;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_MediaPlayerCtrl_sp->getDisplayParam(eDispParamType, &CurVol);	
		if(ERROR_SUCCESS == iRet)
		{
			iCurrValue  = CurVol ;
		}
	}while(false);

	return iRet;
}

INT_t QWebBrowserApp::setOsdPosition(int x, int  y, int  width, int height,int type)
{
	INT_t iRet;
	QDesktopWidget * pDesktopWidget = desktop();
	iRet = 0;

	do
	{
		if(NULL == pDesktopWidget)
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setOsdPosition(x, y, width, height,type);
		}
		
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);
	return iRet;
}

INT_t QWebBrowserApp::switchToNextVideoOutputMode()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iId;
	int eActualVideoOutputMode;

	//LOG_BLINE("switchToNextVideoOutputMode()\n");

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		{
			BOOL_t bFound = FALSE;
			MediaPlayer::VIDEO_OUTPUT_MODE eOldVidOutMode = m_iVideoOutMode;
			for(iId=0; iId<(typeof(iId))ARRAY_COUNT(m_SwitchableVidOutMode_Table); iId++)
			{
				if(m_SwitchableVidOutMode_Table[iId] == m_iVideoOutMode)	//found
				{
					bFound = TRUE;
					if(iId < (typeof(iId))ARRAY_COUNT(m_SwitchableVidOutMode_Table)-1)	//can go to next one
					{
						m_iVideoOutMode = (typeof(m_iVideoOutMode))m_SwitchableVidOutMode_Table[iId+1];	//next
					}
					else	//last, roll to the first one
					{
						m_iVideoOutMode = (typeof(m_iVideoOutMode))m_SwitchableVidOutMode_Table[0];
					}
					if(m_iVideoOutMode != eOldVidOutMode)	//must be not same video mode
					{
						break;
					}
				}
			}
			if(FALSE == bFound)
			{
				m_iVideoOutMode = (typeof(m_iVideoOutMode))m_SwitchableVidOutMode_Table[0];	//default to a fixed one
			}
		}
		//set the new mode
		iRet = m_MediaPlayerCtrl_sp->ChangeVideoOutputMode(m_iVideoOutMode, &eActualVideoOutputMode);
		if(ERROR_SUCCESS == iRet)
		{
			//show led status
			{
				QString strVoModeDescForLedDisp;
				MP7XX::QAudioVideoInOutInfo::getVoDescForLedDisp(m_iVideoOutMode, OUT strVoModeDescForLedDisp);
				if(!m_MediaPlayerCtrl_sp.isNull())
				{
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(strVoModeDescForLedDisp, 1);
				}
			}
		}
		else if(ERROR_NOT_SUPPORTED == iRet)
		{
			iOutRet = iRet;
			break;
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(false);

	return iOutRet;
}

INT_t   QWebBrowserApp::getDisplayPosition(int eRuaDispType,int & pCurrXpos,
		int & pCurrYpos,int & pCurrWidth,int & pCurrHeight)
{
	INT_t iRet = ERROR_SUCCESS ;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}

  		if(eRuaDispType > RUA_DISP_OSD ||eRuaDispType <   RUA_DISP_UNKNOWN)
  		{
  			iRet = ERROR_INVALID_PARAMETER;
  			break;
  		}

		iRet = m_MediaPlayerCtrl_sp->getDisplayPosition(eRuaDispType, &pCurrXpos,
			&pCurrYpos,&pCurrWidth,&pCurrHeight);

	}while(false);

	return iRet;
}

INT_t QWebBrowserApp::getDefaultDisplayParam(int& dWidth,int& dHeight)
{	
	INT_t iRet,iOutRet = ERROR_SUCCESS;
	INT_t iCurResolutionId=0;
	do
	{		
		DECLARE_CLS_STACK_BUF_STRING(strAutoAnnounceIntervalSec, 32);
		iRet = SysProp_get(SysProp_KEY_CurOsdResolutionId, OUT strAutoAnnounceIntervalSec);
		if(ERROR_SUCCESS == iRet)
		{
			iCurResolutionId = strAutoAnnounceIntervalSec;
		}
		

		switch(iCurResolutionId)
		{
		case 1:
			dWidth=1366,dHeight=768;
			break;
		case 2:
			dWidth=1920,dHeight=1080;
			break;
		case 3:
			dWidth=3840,dHeight=2160;
			break;
		case 0:
		default:
			dWidth=1024,dHeight=768;
			break;
		}		
	}while(FALSE);

}


bool QWebBrowserApp::ResetConfiguration()
{
	INT_t iOutRet  = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	do
	{
		iRet = strShellCmd.Format("%sResetSysConfiguration.sh \"%s\"", qPrintable(APP_DIR_PATH), qPrintable(APP_DIR_PATH));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			iOutRet = ERR_RUN_ResetSysCfg_FAIL;
			break;
		}
	}while(false);

	return (ERROR_SUCCESS == iOutRet);
}

void QWebBrowserApp::setCoordinateMode(int iMode,int & iOutRet)
{
	 iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(RELATIVE_MODE  >  iMode || ABSOLUTE_MODE < iMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_CoordinateMode, QString::number(iMode));
	}while(FALSE);


}

void QWebBrowserApp::ReStart()
{
	QDBusMessage DbusMsgToSend;
	bool bSendMsgSuccess;
	int iOutRet;

	do
	{
		DbusMsgToSend = QDBusMessage::createSignal(DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "restartInitiated");
		bSendMsgSuccess = m_DBusConn_sp->send(DbusMsgToSend);
		if(bSendMsgSuccess)
		{
			iOutRet = ERROR_SUCCESS;
		}
		else
		{
			bSendMsgSuccess = m_DBusConn_sp->send(DbusMsgToSend);
			if(bSendMsgSuccess)
			{
				iOutRet = ERROR_SUCCESS;
			}
			else
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				break;
			}
		}
		if(m_CheckRebootTimer.isActive())
		{
			m_CheckRebootTimer.stop();
		}

		SystemReboot();
	}while(FALSE);
}

void QWebBrowserApp::SystemReboot()
{
	INT_t iRet;
	LPCSTR pszCmd = NULL;

	iRet = StopPlayback();
	if(ERROR_SUCCESS == iRet)
	{
	}
	else
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	sync();

	//ensure sleep for enough 2000ms
	{
		UINT64_t StartTimeMs;
		StartTimeMs = GetSysUpTimeMs();
		while(TRUE)
		{
			INT64_t iLeftSleepTimeMs = (2*1000) - (GetSysUpTimeMs()-StartTimeMs);
			if(0 >= iLeftSleepTimeMs)
			{
				break;
			}
			usleep(iLeftSleepTimeMs*1000);
		}
	}

	BOOL_t bEnableWriteLoggin = TRUE;
	iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	if(bEnableWriteLoggin)
	{
		CString::STD_STRING_LIST strList ;
		strList.push_back(TYPE_LOG_SystemOff);
		strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
		if(ERROR_SUCCESS != WriteEventLog(strList))
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}

	sync();
	
	LPCSTR pszStopSnmpCmd = NULL;
	pszStopSnmpCmd = "/etc/Service/SnmpdSrv stop";
	iRet = system(pszStopSnmpCmd);
	if(0 != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	sync();

	//the program reboot only sleep for 1 second
	pszCmd = "reboot";
	iRet = system(pszCmd);
	if(ERROR_SUCCESS != iRet)
	{
		sync();
		LOG_BLINE("RunCmd(%s) err %d\n", pszCmd, iRet);
		iRet = system(pszCmd);
	}

	sync();
}

void QWebBrowserApp::setShitfPosition(int eShitfType,int iShiftValue,int & iOutRet)
{

	INT_t iRet = ERROR_SUCCESS ;
	int x,y,width,height,dwidth,dheight;
	x = y = 0;
	width = height = 4096;
	getDefaultDisplayParam(dwidth,dheight);
	
	do
	{
		if(iShiftValue > 100 || iShiftValue < 0)
		{
			iRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = getDisplayPosition(RUA_DISP_OSD,x,y,width,height);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		int HardwareX, HardwareY, HardwareWidth, HardwareHeight;
		HardwareX = x;
		HardwareY = y;
		HardwareWidth = width;
		HardwareHeight = height;
		switch(eShitfType)
		{
			case AMedia_POSTION_ShiftX:
				{
					HardwareX = (iShiftValue-50) *10;
				}
				break;
			case AMedia_POSTION_ShiftY:
				{
					//HardwareY = (y * VideoSurface_WIDTH / 100);
					HardwareY= (iShiftValue-50) *10;
				}
				break;
			case AMedia_POSTION_ShiftXSIZE:
				{
					iShiftValue  = (( (iShiftValue - 50)) *10);
					HardwareWidth  =dwidth +( iShiftValue);
				}
				break;
			case AMedia_POSTION_ShiftYSIZE:
				{
					iShiftValue  = (( (iShiftValue - 50)) *10);
					HardwareHeight = dheight +( (iShiftValue));
				}
				break;
			default:
				break;
		}

		iRet = setOsdPosition(HardwareX,HardwareY,HardwareWidth,HardwareHeight,eShitfType);
	}while(false);
	iOutRet = iRet;
}

void QWebBrowserApp::setFlippingInterval(int iIntervalVaule,int &iOutRet)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			
			break;
		}
		mp_Config->setConfigValue(INI_KEY_FlippingIntervalTime, QString::number(iIntervalVaule));
	}while(FALSE);
}


void QWebBrowserApp::getFlippingInterval(int &iOutRet,int  & iIntervalVaule)
{
	
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iIntervalVaule   =  mp_Config->getConfigValue(INI_KEY_FlippingIntervalTime).toInt();
	}while(FALSE);
}

void QWebBrowserApp::setPlayInternalImage(int iIntervalImageId,int &iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iOutRet=CheckPlayEnable();
		if(iOutRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif
		if(iIntervalImageId >= 0  && iIntervalImageId < 5)
		{
			QString strPictureName,strPictureUrl;
			strPictureName = QString(INTERNALIMAGEID).arg(QString::number(iIntervalImageId));
			strPictureUrl = APP_DIR_PATH +  CONFIG_NAME +"/"+ strPictureName;
			if(!QFile::exists(strPictureUrl))
			{
				strPictureUrl = APP_DIR_PATH +  IMAGE_PATH_NAME +"/"+ strPictureName;
				if(!QFile::exists(strPictureUrl))
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
			}
			StopPlayback();
			m_PlaylistDataA.ClearPlayListData();
			On_Timer_checkHtmlDisPlayTimeOut();			
			m_PlayPicAndHtmlWin->playHtmlFileEnd();
			OnTimer_checkPlayPluginHtmlEntryDurationTimeOut();
			StopPlayPluginList();
			CallPlayPicturepage(strPictureUrl,0);
		}
	}while(FALSE);
}


void QWebBrowserApp::setDefaultStartupItem(const QString & strFileInfo, int & iOutRet)
{
	QString strPlayFileUrl;
	QString strFileType;

	iOutRet = ERROR_SUCCESS;

	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(strFileInfo.isEmpty() || strFileInfo == " ")	//Clear
		{
			mp_Config->setConfigValue(INI_KEY_DefaultStartupItem, "");
			break;
		}
		strPlayFileUrl = ParsePlayFilePathToPlayUrl(strFileInfo, strFileType);
		if(strPlayFileUrl.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
		{
			if(!QFile::exists(strPlayFileUrl))
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else
		{

		}
		mp_Config->setConfigValue(INI_KEY_DefaultStartupItem, strFileInfo);
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setDefaultStartupItem(%s),iOutRet=%d\n", qPrintable(strFileInfo), iOutRet);
	}
}

void QWebBrowserApp::setVirtualChannelId(int VirChid, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;

	m_iSnmpTempVirChId = VirChid;
}

void QWebBrowserApp::getVirtualChannelId(int & iOutRet, int & VirChid)
{
	iOutRet = ERROR_SUCCESS;

	VirChid = m_iSnmpTempVirChId;
}

void QWebBrowserApp::setVirChMedia(CONST QString & strVirChMedia, int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}
		m_strSnmpTempVirChMedia = strVirChMedia;

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_strUrl == m_strSnmpTempVirChMedia)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_strUrl = m_strSnmpTempVirChMedia;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_strUrl = m_strSnmpTempVirChMedia;
			VirChInfo_sp->m_strAlias = m_strSnmpTempVirChAlias;
			iRet = mMediaVirChannel_sp->setVirChInfo(m_iSnmpTempVirChId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChMedia(int & iOutRet, QString & strVirChMedia)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			strVirChMedia = m_strSnmpTempVirChMedia;
			break;
		}
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			strVirChMedia = VirChInfo_sp->m_strUrl;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			strVirChMedia = m_strSnmpTempVirChMedia;
		}
		//second part

	}while(FALSE);
}

void QWebBrowserApp::setVirtualChannelAlias(CONST QString & strVirChAlias, int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}
		m_strSnmpTempVirChAlias = strVirChAlias;

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_strAlias == m_strSnmpTempVirChAlias)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_strAlias = m_strSnmpTempVirChAlias;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_strUrl = m_strSnmpTempVirChMedia;
			VirChInfo_sp->m_strAlias = m_strSnmpTempVirChAlias;
			iRet = mMediaVirChannel_sp->setVirChInfo(m_iSnmpTempVirChId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirtualChannelAlias(int & iOutRet, QString & strVirChAlias)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			strVirChAlias = m_strSnmpTempVirChAlias;
			break;
		}
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			strVirChAlias = VirChInfo_sp->m_strAlias;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			strVirChAlias = m_strSnmpTempVirChAlias;
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

void QWebBrowserApp::setVirChLabelDispPos(int DispPos,  int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}
		m_eSnmpTempVirChLabelDispPos = DispPos;

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_eLabelDispPos == m_eSnmpTempVirChLabelDispPos)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_eLabelDispPos = m_eSnmpTempVirChLabelDispPos;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_strUrl = m_strSnmpTempVirChMedia;
			VirChInfo_sp->m_strAlias = m_strSnmpTempVirChAlias;
			VirChInfo_sp->m_eLabelDispPos = m_eSnmpTempVirChLabelDispPos;
			iRet = mMediaVirChannel_sp->setVirChInfo(m_iSnmpTempVirChId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChLabelDispPos(int & iOutRet,  int & DispPos)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			DispPos = m_eSnmpTempVirChLabelDispPos;
			break;
		}
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			DispPos = VirChInfo_sp->m_eLabelDispPos;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			DispPos = m_eSnmpTempVirChLabelDispPos;
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

void QWebBrowserApp::setVirChEnable(int bEnable, int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_bEnable == bEnable)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_bEnable = bEnable;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_bEnable = bEnable;
			iRet = mMediaVirChannel_sp->setVirChInfo(m_iSnmpTempVirChId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChEnable(int & iOutRet, int & bEnable)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			bEnable = VirChInfo_sp->m_bEnable;
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

void QWebBrowserApp::setVirChDispDuration(int DispDuration, int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_DispDuration == DispDuration)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_DispDuration = DispDuration;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_DispDuration = DispDuration;
			iRet = mMediaVirChannel_sp->setVirChInfo(m_iSnmpTempVirChId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChDispDuration(int & iOutRet, int & DispDuration)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > m_iSnmpTempVirChId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(m_iSnmpTempVirChId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			DispDuration = VirChInfo_sp->m_DispDuration;
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

void QWebBrowserApp::setVirChMin(int MinVal, int & iOutRet)
{
	INT_t iRet;
	int OldVirChMin;

	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = mMediaVirChannel_sp->getVirChMin(OUT OldVirChMin);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(OldVirChMin == MinVal)
		{
			break;
		}

		iRet = mMediaVirChannel_sp->setVirChMin(MinVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChMin(int & iOutRet, int & MinVal)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = mMediaVirChannel_sp->getVirChMin(OUT MinVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

void QWebBrowserApp::setVirChMax(int MaxVal, int & iOutRet)
{
	INT_t iRet;
	int OldVirChMax;

	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = mMediaVirChannel_sp->getVirChMax(OUT OldVirChMax);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(OldVirChMax == MaxVal)
		{
			break;
		}

		iRet = mMediaVirChannel_sp->setVirChMax(MaxVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);
}

void QWebBrowserApp::getVirChMax(int & iOutRet, int & MaxVal)
{
	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = mMediaVirChannel_sp->getVirChMax(OUT MaxVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
}

QString QWebBrowserApp::getFolderNameByFileType(int iFileType)
{
	QString  strTempFolderPath = "" ;

	if(AMedia_TYPE_VIDEO == iFileType)
	{
		strTempFolderPath = FOLDER_NAME_VIDEO;
	}
	else if(AMedia_TYPE_AUDIO == iFileType)
	{
		strTempFolderPath = FOLDER_NAME_MUSIC ;
	}
	else if(AMedia_TYPE_PICTURE == iFileType)
	{
		strTempFolderPath = FOLDER_NAME_PICTURE;
	}
	else if(AMedia_TYPE_PLAYLIST == iFileType)
	{
		strTempFolderPath = FOLDER_NAME_PLAYLIST;
	}
	else if(AMedia_TYPE_HTML == iFileType)
	{
		strTempFolderPath = FOLDER_NAME_HTML;
	}
	return strTempFolderPath;
}

INT_t QWebBrowserApp::PlayCeLabsUrl(const QString & strCeLabsUrl,bool isPlayVirtualChannle)
{
	INT_t iOutRet = ERROR_INVALID_PARAMETER, iRet;
	QString strDataSrcDevice, strFileType, strFilename;
	int iIndex;

	do
	{
		//first part
		iIndex = strCeLabsUrl.indexOf(":");
		if(0 < iIndex)
		{
			strDataSrcDevice = strCeLabsUrl.left(iIndex);
		}
		else if(0 == iIndex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		else
		{
			strDataSrcDevice = strCeLabsUrl;
		}
		if(strDataSrcDevice.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}		
		//second part
		if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_USB, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive) ||
		        0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_IDE, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive) )
		{
			if(0 < iIndex)
			{
				int iColonIndex;
				iColonIndex = strCeLabsUrl.indexOf("://", iIndex+2);
				if(0 < iColonIndex)
				{
					strFileType = strCeLabsUrl.mid(iIndex+2, iColonIndex - (iIndex + 2));
					//LOG_BLINE("%s\n", qPrintable(strFileType));
					if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
					{
						//the third part
						strFilename = strCeLabsUrl.mid(iColonIndex+3);
						if(FALSE == strFilename.isEmpty())
						{
							QString strFileFullPath;
							strFileFullPath = getFileFullPathByCeLabsUrl(strDataSrcDevice, strFileType, strFilename);
							if(!QFile::exists(strFileFullPath))
							{
								iOutRet =  ERROR_INVALID_PARAMETER;
								break;
							}
							if(FALSE == strFileFullPath.isEmpty())
							{
								if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive))
								{
									SwitchToPlayWin();
									iRet = OpenPlaylist(strFileFullPath);
									if(ERROR_SUCCESS == iRet)
									{
										iOutRet = ERROR_SUCCESS;
										m_eCurPlayMediaFileType = AMedia_TYPE_PLAYLIST;
									}
								}
								else if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive) ||
									  0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
									  0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
									  0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
								{
									m_eMediaPlayCurrStatus = MediaPlay_Status_PlayCeLabsFile;
									StopPlayback();
									QWidget * pActiveWindow = NULL;
									pActiveWindow = activeWindow();
									if(pActiveWindow== &m_playWin)
									{
										m_PlayPicAndHtmlWin->playHtmlFileEnd();
										UpdateVideoPosition(true);
									}
									if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive))
									{
										m_iStateType = State_PlayingVideoFile;
									}
									else if( 0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive))
									{
										m_iStateType = State_PlayingAudioFile;
									}
									else if( 0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
									{
										m_iStateType = State_PlayingPictureFile;
									}

									iRet = PlayUrl(strFileFullPath);
									if(ERROR_SUCCESS == iRet)
									{
										iOutRet = ERROR_SUCCESS;
										m_eCurPlayMediaFileType = AMedia_TYPE_VIDEO;
									}
									else
									{
										iOutRet = iRet;
									}

								}
								else if(0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
									0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive))
								{
									m_eMediaPlayCurrStatus = MediaPlay_Status_PlayCeLabsFile;
									m_eCurPlayMediaFileType = AMedia_TYPE_HTML;
									m_FristEntryIsHtml = true;
									iOutRet =CallPlayWebpage(strFileFullPath,0,true);
									m_iStateType = State_PlayingBrowserFile;
								}
							}
						}
					}
					else if(0 == strFileType.compare(CeLabs_DATA_SRC_DEV_UDP, Qt::CaseInsensitive))
					{
						StopPlayDurationTimer();
						QWidget * pActiveWindow = NULL;
						pActiveWindow = activeWindow();
						if(pActiveWindow== &m_playWin)
						{
							m_PlayPicAndHtmlWin->playHtmlFileEnd();
							UpdateVideoPosition(true);
						}
						m_iStateType = State_Streaming;
						iRet = PlayUrl(strCeLabsUrl);
						if(ERROR_SUCCESS == iRet)
						{
							m_iStateType = State_Streaming;
							iOutRet = ERROR_SUCCESS;
							m_eCurPlayMediaFileType = AMedia_TYPE_Tuner;
						}
						m_iStateType = State_Streaming;
					}
				}
			}
		}
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_UDP, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_RTP, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_RTSP, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_TUNER, Qt::CaseInsensitive))
		{
			StopPlayback();
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			if(pActiveWindow== &m_playWin)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				UpdateVideoPosition(true);
			}
			m_iStateType = State_Streaming;
			iRet = PlayUrl(strCeLabsUrl);
			m_iStateType = State_Streaming;
			if(ERROR_SUCCESS == iRet)
			{
				iOutRet = ERROR_SUCCESS;
				m_eCurPlayMediaFileType = AMedia_TYPE_Tuner;
			}
		}
		else  if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_WEBPAGE, Qt::CaseInsensitive) )
		{
			if(isPlayVirtualChannle)
			{
				StopPlayback();
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				if(pActiveWindow== &m_playWin)
				{
					m_PlayPicAndHtmlWin->playHtmlFileEnd();
					UpdateVideoPosition(true);
				}
				m_iStateType = State_Streaming;
				iRet = PlayUrl(strCeLabsUrl);
				m_iStateType = State_Streaming;
				if(ERROR_SUCCESS == iRet)
				{
					iOutRet = ERROR_SUCCESS;
					m_eCurPlayMediaFileType = AMedia_TYPE_Tuner;
				}
			}
			else
			{
				StopPlayback();
				iOutRet =CallPlayWebpage(strCeLabsUrl,0,true);
				m_iStateType = State_PlayingBrowserFile;
			}
		}
	}while(FALSE);

	return iOutRet;
}

QString QWebBrowserApp::getFileFullPathByCeLabsUrl(const QString & strDataSrcDevice, const QString & strFileType, const QString & strFilename)
{
	QString strFileFullPath;

	do
	{
		if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive))
		{
			strFileFullPath = APP_DIR_PATH + getStorageDirNameFromCeLabsDataSrcDev(strDataSrcDevice) + "/" +
			getDirNameFromFileType(strFileType) + "/" + strFilename;
		}
		else
		{
			strFileFullPath = MOUNTDIR + APP_DIR_PATH + getStorageDirNameFromCeLabsDataSrcDev(strDataSrcDevice) + "/" +
			getDirNameFromFileType(strFileType) + "/" + strFilename;
		}
	
		
	}while(FALSE);

	return strFileFullPath;
}

QString QWebBrowserApp::getStorageDirNameFromActiveMediaType(CONST ActiveMedia_TYPE eActiveMediaType)
{
//	INT_t iOutRet = ERROR_SUCCESS;
	QString strStorageDirName;

	QAutoLock AutoRangeLocker(&m_SharedDataMutex);

	do
	{
		if(AMedia_TYPE_FLASH == eActiveMediaType)
		{
			strStorageDirName = LOCAL_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_SdCard == eActiveMediaType)
		{
			strStorageDirName = SD_CARD_DIR_NAME;
		}
		else if(AMedia_TYPE_USB == eActiveMediaType)
		{
			strStorageDirName = USB_STORAGE_DIR_NAME;
		}
		else if(AMedia_TYPE_HDD == eActiveMediaType)
		{
			strStorageDirName = StorageDirName_HDD;
		}
		else if(AMedia_TYPE_RAM == eActiveMediaType)
		{
			strStorageDirName = StorageDirName_RAM;
		}
		else
		{
			strStorageDirName = LOCAL_STORAGE_DIR_NAME;
		}
	}while(FALSE);

	return strStorageDirName;
}

int QWebBrowserApp::ResetSnmpPassword()
{
	INT iOutRet ;
	iOutRet = ERROR_SUCCESS;

	do
	{

		if(m_PwdInputDlg_sp.isNull())
		{
			m_PwdInputDlg_sp = QSharedPointer <QInputcontentdlg> (new QInputcontentdlg(&m_Browser));//,Qt::FramelessWindowHint
			connect(&(*m_PwdInputDlg_sp), SIGNAL(finished(int)),this,SLOT(setSnmpPassFinishSlot(int)));
			m_PwdInputDlg_sp->setLabelText("Please input your password:");
			m_PwdInputDlg_sp->setTextEchoMode(QLineEdit::Password);
			m_PwdInputDlg_sp->setOkButtonText("OK");	
		}
		//tempInputDlg->setOption(QInputDialog::NoButtons);
		//tempInputDlg->setOptions(QInputDialog::NoButtons);
		//QString text = tempInputDlg.getText(0, "", "Please input your password",
		//	QLineEdit::Password, "", &isOK);

		//m_PwdInputDlg_sp->show();	
		
#if 1	/*the input dialog was modified for abner on 2014-11-11*/	
		m_PwdInputDlg_sp->showNormal();

		m_PwdInputDlg_sp->setMinimumWidth(250);
		m_PwdInputDlg_sp->setMinimumHeight(100);

		INT ixCoordinate = (m_Browser.width()-m_PwdInputDlg_sp->width())/2;
		INT iyCoordinate = (m_Browser.height()-m_PwdInputDlg_sp->height())/2;
		m_PwdInputDlg_sp->move(ixCoordinate,iyCoordinate);

		m_PwdInputDlg_sp->raise();
		m_PwdInputDlg_sp->activateWindow();
#endif		
	}while(FALSE);

 	return 0;
}

void  QWebBrowserApp::ResolveStr(QString strResolve,QString & strDstFilePath)
{
	QString  strDstFileName,strDstTypeDirName,strDstStorageDirName;
	strDstFilePath = ""; 
	do
	{
		QStringList strItemList  = strResolve.split(":");
		if(strItemList.size()  == 4)
		{
			strDstFileName = strItemList[3].mid(2) ;
			strDstStorageDirName = getStorageDirNameFromDstFileName(strItemList[0]);
			strDstTypeDirName = getDirNameFromFileType(strItemList[2]);
			if(strDstTypeDirName == "")
			{
				break;
			}
			strDstFilePath = qApp->applicationDirPath() + "/" + strDstStorageDirName + "/" + strDstTypeDirName + "/" + strDstFileName;
		}
	}while(FALSE);
}

void QWebBrowserApp::setPlayVirtualChannel(int iChannelId,int &iOutRet)
{

	INT_t iRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iRet =iOutRet = ERROR_SUCCESS;

	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iRet=CheckPlayEnable();
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif
		if(0 > iChannelId)
		{
			iOutRet = ERR_INVALID_ID;
			break;
		}

		//Put the info into the map table
		iRet = mMediaVirChannel_sp->getVirChInfo(iChannelId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_bEnable)
			{
				iRet = PlayCeLabsUrl(VirChInfo_sp->m_strUrl,true);
				if(ERROR_SUCCESS == iRet)
				{
					//CONTENT_TRIGGER_SRC_PlayVirChannleItem
					m_eContentTrigSrcType = CONTENT_TRIGGER_SRC_PlayVirChannleItem;
					m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
					break;
				}
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{

		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
	iOutRet = iRet;
}

void QWebBrowserApp::setPlayModeNormal(int iPlayMode)
{
	INT_t iOutRet;
	
	Q_UNUSED(iPlayMode);

	do
	{
		if(ControlCenter::PlayMode_Normal == m_eCurPlayMode)
		{
			setPlayMode(ControlCenter::PlayMode_Pause, iOutRet);
		}
		else if(ControlCenter::PlayMode_Pause == m_eCurPlayMode)
		{
			setPlayMode(ControlCenter::PlayMode_Normal, iOutRet);
		}
		else
		{
			setPlayMode(ControlCenter::PlayMode_Normal, iOutRet);
		}
	}while(FALSE);
}

void QWebBrowserApp::setSpeedControl(int iSpeedValue,int & iOutRet)
{
	INT_t iRet = ERROR_SUCCESS ;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}

  		if(iSpeedValue > 250 ||iSpeedValue <   5)
  		{
  			iRet = ERROR_INVALID_PARAMETER;
  			break;
  		}

		iRet = m_MediaPlayerCtrl_sp->setPlaySpeedCtrlAsync(iSpeedValue);

	}while(false);

	iOutRet = iRet;
}


void QWebBrowserApp::getShitfPosition(int eShitfType,int & iOutRet,int & iShiftValue)
{
	INT_t iRet = ERROR_SUCCESS ;
	int x,y,width,height,dwidth,dheight;
	x = y = 0;
	width = height = 4096;
	getDefaultDisplayParam(dwidth,dheight);
	
	do
	{
		switch(eShitfType)
		{
			case AMedia_POSTION_ShiftX:
				iRet = getDisplayPosition(RUA_DISP_OSD,iShiftValue,y,width,height);
				if(iRet == ERROR_SUCCESS)
				{
					iShiftValue = (500 + iShiftValue )  / 10;
				}
				break;
			case AMedia_POSTION_ShiftY:
				iRet = getDisplayPosition(RUA_DISP_OSD,x,iShiftValue,width,height);
				if(iRet == ERROR_SUCCESS)
				{
					iShiftValue = (500 + iShiftValue )  / 10;
				}
				break;
			case AMedia_POSTION_ShiftXSIZE:
				{
					iRet = getDisplayPosition(RUA_DISP_OSD,x,y,iShiftValue,height);
					if(iRet == ERROR_SUCCESS)
					{
						iShiftValue = (dwidth -  iShiftValue )  / 10;
						iShiftValue  = 50 - iShiftValue;
					}
				}
				break;
			case AMedia_POSTION_ShiftYSIZE:
				{
					iRet = getDisplayPosition(RUA_DISP_OSD,x,y,width,iShiftValue);
					if(iRet == ERROR_SUCCESS)
					{
						iShiftValue = (dheight -  iShiftValue )  / 10;
						iShiftValue  = 50 - iShiftValue;
					}
				}
				break;
		}

	}while(false);

	iOutRet = iRet;
}

void QWebBrowserApp::getSpeedControl(int & iOutRet, int &iCurrSpeed)
{
	INT_t iRet = ERROR_SUCCESS ;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = m_MediaPlayerCtrl_sp->getPlaySpeedCtrl(& iCurrSpeed);

	}while(false);

	iOutRet = iRet;
}

void QWebBrowserApp::getPlayMode(int & iOutRet, int & iCurrMode)
{

	INT_t iRet = ERROR_SUCCESS ;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}		
		iRet = m_MediaPlayerCtrl_sp->getPlayMode(& iCurrMode);			

		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		if(pActiveWindow== &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml())
		{
			m_eCurPlayMode = ControlCenter::PlayMode_Normal;
 			iCurrMode = m_eCurPlayMode;
		}
		else
		{
			if(iRet == ERROR_SUCCESS)
			{
				m_eCurPlayMode =(ControlCenter::PLAY_MODE) iCurrMode ;
			}
		}
	}while(false);

	iOutRet = iRet;
}

void QWebBrowserApp::getDefaultIdlePlaylist( int & iOutRet,QString &strIdlePlaylist)
{
	INT_t iRet = ERROR_SUCCESS ;
	do
	{
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		strIdlePlaylist = mp_Config->getConfigValue(INI_KEY_CurIdlePlaylistFilePath);

	}while(FALSE);

	iOutRet = iRet;
}


void QWebBrowserApp::getPlayInternalImage(int &iOutRet,int & iIntervalImageId)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		//if(0 > iIntervalImageId  && iIntervalImageId < 5)

		{
			iIntervalImageId = mp_Config->getConfigValue(INI_KEY_FlippingIntervaImageId).toInt();
		}
	}while(FALSE);
}

void QWebBrowserApp::getplayPlaylist(int &iOutRet,QString & strPlaylist)
{

	 iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		strPlaylist = m_strDefaultStartupItem;
	}while(FALSE);
}

void QWebBrowserApp::getDefaultStartupItem(int &iOutRet,QString & strPlaylist)
{
	 iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		strPlaylist = mp_Config->getConfigValue(INI_KEY_DefaultStartupItem);
	}while(FALSE);
}

VOID QWebBrowserApp::ShowIpInfoOnLed()
{
	QStringList dhcpInfoList;
	do
	{
		GetNetWorkInfo(dhcpInfoList);
		if(dhcpInfoList.size() > 0)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(dhcpInfoList[0], 1);
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::getIdleDetect(int & iOutRet,int & iIdleSeconds)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iIdleSeconds = mp_Config->getConfigValue(INI_KEY_IdleDetectTime).toInt();
	}while(FALSE);
}

void QWebBrowserApp::setPullConfig(CONST QString & strUrl, int & iOutRet)
{
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("mount -o rw,remount /");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}
		}
		//Call into SystemService.SystemEvent
		DbusMsgToSend = QDBusMessage::createMethodCall(SS_DBUS_SERVICE_NAME, DBUS_PATH_SysSrv_SysEvt,
			DbusIf_SysSrv_SysEvt, METHOD_SyncCacheToStorage);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		m_strSysCfgUploadUrl = strUrl;
		//Schedule delayed uploading config
		m_UploadCfgTimer.setSingleShot(TRUE);
		m_UploadCfgTimer.start(1*1000);
	}while(FALSE);
}

void QWebBrowserApp::setPushConfig(CONST QString & strUrl, int & iOutRet)
{
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setPushConfig,Url=%s\n", qPrintable(strUrl));
	}

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("mount -o rw,remount /");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}
		}
		
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "DownloadFile2");
		
		iRet = CreateDirectory2("/Temp", (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgArgsList << strUrl << NewSysCfg_DOWNLOAD_TEMP_FILE_PATH;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(1);
			m_NewSysCfg_DownloadId = Reply_Arg.toUInt();
		}
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setPushConfig,Ret=%d\n", iOutRet);
	}
}

void QWebBrowserApp::ResetPlayListEntryPostionInfo()
{
    	QSharedPointer <QPlaylistEntry> PlayEntry_sp;
	int iRet = ERROR_SUCCESS ;
	do
	{
		PlayEntry_sp = m_PlaylistDataA.CurrPlayEntry();
		if(PlayEntry_sp.isNull())	//end of list
		{
			break;
		}
		if(QPlaylistEntry::ACTION_Video == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Video> PlayEntry_Video_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Video>();
			if(PlayEntry_Video_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			setPlayListActionPosition(PlayEntry_Video_sp->m_xLeft,PlayEntry_Video_sp->m_yUpper,
			PlayEntry_Video_sp->m_xSize,PlayEntry_Video_sp->m_ySize);
		}
		else if(QPlaylistEntry::ACTION_Picture== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Picture> PlayEntry_Picture_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Picture>();
			if(PlayEntry_Picture_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			setPlayListActionPosition(PlayEntry_Picture_sp->m_xLeft,PlayEntry_Picture_sp->m_yUpper,
			PlayEntry_Picture_sp->m_xSize,PlayEntry_Picture_sp->m_ySize);
		}
		else if(QPlaylistEntry::ACTION_Html== PlayEntry_sp->getActionType() ||
			    QPlaylistEntry::ACTION_Browser== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Html> PlayEntry_Html_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Html>();
			if(PlayEntry_Html_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Html_sp->getActionType(),
				PlayEntry_Html_sp->m_strFilePath);

			m_FristEntryIsHtml = true;
			if(strActualFilePath == "")
			{
				strActualFilePath = PlayEntry_Html_sp->m_strFilePath;
			}
			/* maybe can not stop html playlist */
			ShowPlayerWindow();
			iRet = CallPlayWebpage(strActualFilePath,PlayEntry_Html_sp->m_iDuration);
			if(iRet != ERROR_SUCCESS)
			{
				PlayNextEntry();
				break;
			}		 
		}
	}while(FALSE);
}

int QWebBrowserApp::CallPlayWebpage(QString strFileFullPath,int iDurationTime,bool isPlaySelectFile)
{
	int iRet ;
	iRet = ERROR_SUCCESS;
	do
	{
		if(strFileFullPath.trimmed()=="")
		{
			iRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(m_bIsFirstPlayUrl)
		{
			m_bIsFirstPlayUrl = false;
		}
		bool bIsOpenUrl = false;
		if(!QFile::exists(strFileFullPath))
		{
			QUrl htmlUrl(strFileFullPath);
			if(htmlUrl.isValid())
			{
				bIsOpenUrl = true;
				strFileFullPath = htmlUrl.toString();
			}
			else
			{
				iRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}	
		/*
		if(m_bIsPlayVideoLayer)
		{
			if(!m_VideoLayerWnd.isNull())
			{
				m_VideoLayerWnd->PlayHtmlEntryOnVideoLayer(strFileFullPath,bIsOpenUrl);
			}
			m_strCurPlayUrl = strFileFullPath ;
			if(m_MediaPlayerCtrl_sp)
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
			}
			m_iStateType = State_PlayingBrowserFile;
			m_eCurPlayMode = ControlCenter::PlayMode_Normal;
		}
		else
		*/
		{
			QWidget * pActiveWindow = NULL;
			pActiveWindow = activeWindow();
			
			if(&m_playWin == pActiveWindow && (!m_PlayPicAndHtmlWin->isPlayingHtml()))
			{
				m_bIsPlayWin = true;
			}
			else if(&m_Browser== pActiveWindow)
			{
				m_bIsPlayWin = false;
			}
			//m_strCurPlayUrl = strFileFullPath ;
			if(m_playWin.getPlayHtmlOrPicWin().isNull())
			{
				if(Sw_LogDbusCmd)
				{
					LOG_BLINE("play html or picture window is null\n");
				}
				break;
			}

			if(&m_Browser== pActiveWindow)
			{
				if(m_eMediaPlayCurrStatus == MediaPlay_Status_PlaySelectFile)
				{
					ShowPlayerWindow();// show full screen like mp500 
				}
				m_playWin.getPlayHtmlOrPicWin()->playHtmlFile(strFileFullPath,bIsOpenUrl);
				//play in browser osd  plugin
				//m_Browser.PlayHtmlEntryInPluginEnd();
			}
			else
			{
				ShowPlayerWindow();// show full screen like mp500 
				if(!m_bIsPlayVideoLayer)
				{
					bool bFullScreen = true;
					UpdateVideoPosition(bFullScreen);
				}
				m_playWin.getPlayHtmlOrPicWin()->playHtmlFile(strFileFullPath,bIsOpenUrl);
			}
		}
		if(m_HtmlDurationTimer.isActive())
		{
			m_HtmlDurationTimer.stop();
		}
		if(!isPlaySelectFile)
		{
			if(iDurationTime!= DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerTimeoutMs = (iDurationTime * 1000);
				if(0 >= iTimerTimeoutMs)
				{
					iTimerTimeoutMs = DEFAULT_PICTURE_DURATION * 1000;
				}
				if(0 < iTimerTimeoutMs)
				{
					m_HtmlDurationTimer.start(iTimerTimeoutMs);
				}
			}
			else
			{
				m_HtmlDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
			}
		}			

		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		if(!m_bIsPlayVideoLayer)
		{
			m_iStateType = State_PlayingBrowserFile;
		}
		if(m_MediaPlayerCtrl_sp)
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
		}
		if(!m_bIsPlayVideoLayer)
		{
			m_iStateType = State_PlayingBrowserFile;
		}
		m_eCurPlayMode = ControlCenter::PlayMode_Normal;
		BOOL_t bEnableWriteLoggin = TRUE;
		iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		if(bEnableWriteLoggin)
		{
			char strTemp[64];
			snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_StartPlay);
			strTemp[sizeof(strTemp)-1] = '\0';
			CString::STD_STRING_LIST strList ;
			strList.push_back(strTemp);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
			QString strPlayType,strPlayFileInfo;
			getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
			strList.push_back(strPlayType.toLocal8Bit().constData());
			strList.push_back(strFileFullPath.toLocal8Bit().constData());
			if(ERROR_SUCCESS !=WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				strList.clear();
			}
		}
	}while(FALSE);

	if(ERROR_SUCCESS == iRet)
	{
		if(m_MediaPlayerCtrl_sp)
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
		}
	}
		
    QWidget * pActiveWindow = NULL;
	pActiveWindow = activeWindow();

    /* start modify by rocky */
	if(&m_playWin == pActiveWindow)
	{
        	// add for html focus 
        	m_playWin.repaint();
        	BringWindowToFront(&m_playWin);
        	//setActiveWindow(&m_playWin);	
        	setActiveWindow(&(*m_playWin.getPlayHtmlOrPicWin()));
        	//BringWindowToFront(&m_playWin);
        	setActiveWindow(&m_playWin);
	}
    /* end */
    
	return iRet;
}

VOID QWebBrowserApp::CallPlayPicturepage(QString strPictureUrl,int iDurationTime)
{
	int iRet ;
	iRet = ERROR_SUCCESS;
	do
	{
		if(strPictureUrl.trimmed()=="")
		{
			break;
		}
		if(m_bIsFirstPlayUrl)
		{
			m_bIsFirstPlayUrl = false;
		}
		if(m_playWin.getPlayHtmlOrPicWin().isNull())
		{
			if(Sw_LogDbusCmd)
			{
				LOG_BLINE("play html or picture window is null\n");
			}
			break;
		}		
		ShowPlayerWindow();
		m_playWin.getPlayHtmlOrPicWin()->playPictureFile(strPictureUrl);
		if(iDurationTime != 0)
		{
			if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerIntervalMs = (iDurationTime * 1000);
				if(0 == iTimerIntervalMs)
				{
					iTimerIntervalMs = 100;
				}
				if(0 < iTimerIntervalMs)
				{
					if(m_HtmlDurationTimer.isActive())
					{
						m_HtmlDurationTimer.stop();
					}
					m_HtmlDurationTimer.start(iTimerIntervalMs);
				}
			}
			else
			{
				m_HtmlDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
			}	
		}
		else
		{
			//m_HtmlDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
		}	
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		m_eCurPlayMode = ControlCenter::PlayMode_Normal;
		m_iStateType = State_PlayingPictureFile;
		m_strCurPlayUrl = strPictureUrl ;		
		if(ERROR_SUCCESS == iRet)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
			}
		} 
		BOOL_t bEnableWriteLoggin = TRUE;
		iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		if(bEnableWriteLoggin)
		{
			char strTemp[64];
			snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_StartPlay);
			strTemp[sizeof(strTemp)-1] = '\0';
			CString::STD_STRING_LIST strList ;
			strList.push_back(strTemp);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
			QString strPlayType,strPlayFileInfo;
			getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
			strList.push_back(strPlayType.toLocal8Bit().constData());
			strList.push_back(strPictureUrl.toLocal8Bit().constData());
			if(ERROR_SUCCESS !=WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				strList.clear();
			}
		}
	}while(FALSE);
}

VOID QWebBrowserApp::ShowPlayPicOrHtmlWindow()
{
	//LOG_BLINE("ShowPlayPicOrHtmlWindow\n");
	QWidget * pActiveWindow = NULL;
	pActiveWindow = activeWindow();
	setActiveWindow(pActiveWindow);

	if(pActiveWindow == &m_Browser)
	{
		m_bIsPlayWin  = false;
		m_bHasPlayFirst = false;
	}
	if(m_PlayPicAndHtmlWin->isHidden())
	{
		m_PlayPicAndHtmlWin->setFocus();
		m_PlayPicAndHtmlWin->show();
	}
	m_PlayPicAndHtmlWin->showMaximized();
	//BringWindowToFront(m_PlayPicAndHtmlWin);
	//setActiveWindow(m_PlayPicAndHtmlWin);
}

QString  QWebBrowserApp::GetDefaultStartupItemUrlByItemFullPath(QString strItemFullPath)
{
	QStringList itemlist;
	QString strStartItemUrl = "";
	do
	{
		itemlist = strItemFullPath.split("/");
		if(itemlist.size() == 5)
		{
			strStartItemUrl =getDevMediaDirNameByDirName(itemlist[2]) + QString("::") +(itemlist[3]) + "://" + itemlist[4]  ;
		}
	}while(FALSE);

	return strStartItemUrl;
}

QString   QWebBrowserApp::getDevMediaDirNameByDirName(CONST QString strDirName)
{
	QString strDevMediaDirName;

	do
	{
		if(0 == strDirName.compare(LOCAL_STORAGE_DIR_NAME, Qt::CaseInsensitive))
		{
			strDevMediaDirName = CeLabs_DATA_SRC_DEV_FLASH;
		}
		else if(0 == strDirName.compare(USB_STORAGE_DIR_NAME , Qt::CaseInsensitive))
		{
			strDevMediaDirName =CeLabs_DATA_SRC_DEV_USB  ;
		}
		else if(0 == strDirName.compare(StorageDirName_HDD , Qt::CaseInsensitive))
		{
			strDevMediaDirName =  CeLabs_DATA_SRC_DEV_HDD;
		}
		else if(0 == strDirName.compare(SD_CARD_DIR_NAME, Qt::CaseInsensitive))
		{
			strDevMediaDirName =  CeLabs_DATA_SRC_DEV_SD;
		}

	}while(FALSE);

	return strDevMediaDirName;

}

int QWebBrowserApp::EnableDisableInput(int iEnableFlag)
{
	INT_t iRet,iOutRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	iOutRet = ERROR_SUCCESS;

	//LOG_BLINE("iEnableFlag=%d\n",iEnableFlag);

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createMethodCall(SS_DBUS_SERVICE_NAME, DBUS_PATH_IrInputCtrlSrv,
			IF_NAME_SysSrv_IrInputCtrlSrv, METHOD_EnableDisableInput);
		MsgArgsList <<  iEnableFlag;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

QStringList  QWebBrowserApp::getInputEnabled()
{
	INT_t iRet,iOutRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;
	QStringList strResultList;

	iOutRet = ERROR_SUCCESS;
	//LOG_BLINE("getInputEnabled()\n");

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createMethodCall(SS_DBUS_SERVICE_NAME, DBUS_PATH_IrInputCtrlSrv,
			IF_NAME_SysSrv_IrInputCtrlSrv, METHOD_getInputEnabled);

		//DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			strResultList << QString::number(iRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_CurrPercent = MsgArgsList.at(1);
			strResultList << QString(Reply_CurrPercent.toString());
		}
	}while(FALSE);
	iOutRet = iRet;
	return strResultList;
}

QString QWebBrowserApp::getVirChannelStreamInfo(int iChannelId)
{
	INT_t iRet;
	QString strVirChannleStreamInfo = "";
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	do
	{
		iRet = mMediaVirChannel_sp->getVirChInfo(iChannelId, OUT VirChInfo_sp);

		if(ERROR_SUCCESS == iRet)
		{
			strVirChannleStreamInfo = VirChInfo_sp->m_strUrl;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

	}while(false);

	return strVirChannleStreamInfo;
}

int QWebBrowserApp::Send_AutoAnnounce()
{
	int iOutRet = ERROR_SUCCESS, iRet;
	QString strCardCurrentStatus;
	qint64 iBytesSent;
	quint8 szRawCardCurStatusData[1024];
	size_t sizeActualRawCardCurStatus;

	do
	{
		iRet = CreateSckForAutoAnnounceIfNeed();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		getCardCurrentStatus(OUT iRet, OUT strCardCurrentStatus, TRUE, szRawCardCurStatusData,
			sizeof(szRawCardCurStatusData), &sizeActualRawCardCurStatus);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		szRawCardCurStatusData[sizeof(szRawCardCurStatusData)-1] = '\0';
		iBytesSent = m_sckAutoAnnounce_sp->writeDatagram((const char *)szRawCardCurStatusData, sizeActualRawCardCurStatus,
			QHostAddress::Broadcast, DEFAULT_AutoAnnounce_UDP_PORT);
		if(sizeActualRawCardCurStatus != iBytesSent)
		{
			iOutRet = ERROR_SOCKET_SEND_FAIL;
			LOG_BLINE("ToSend=%d,Sent=%lld,SckErr=%d\n", sizeActualRawCardCurStatus, iBytesSent, m_sckAutoAnnounce_sp->error());
			if(QAbstractSocket::NetworkError == m_sckAutoAnnounce_sp->error())	//no default gateway
			{
			}
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		iRet = iOutRet;
		PRINT_BFILE_LINENO_IRET_STR;
	}
	return iOutRet;
}

QStringList QWebBrowserApp::getChannelStreamList()
{
	QStringList streamChannelList,streamChannelIdList;
	int iMinChannelId, iMaxChannelId;
	iMinChannelId=iMaxChannelId=0;
	INT_t iRet, iOutRet;

	iOutRet = ERROR_SUCCESS;

	do
	{
		QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

		iRet = mMediaVirChannel_sp->getVirChMin(iMinChannelId);
		iRet = mMediaVirChannel_sp->getVirChMax(iMaxChannelId);
		if(iRet != ERROR_SUCCESS)
		{
			break;
		}
		//mMediaVirChannel_sp->getVirChIdList(streamChannelIdList);
		streamChannelIdList = getChannelIdList();
		for(int i = 0;i< streamChannelIdList.size();i++)
		{
			iRet = mMediaVirChannel_sp->getVirChInfo(streamChannelIdList[i].toInt(), OUT VirChInfo_sp);
			if(ERROR_SUCCESS == iRet)
			{
				iOutRet = iRet;
				QString strVirChStreamInfo = VirChInfo_sp->m_strUrl;
				if(strVirChStreamInfo !="") //VirChInfo_sp->m_bEnable &&
				{
					streamChannelList << VirChInfo_sp->m_strUrl;
				}
				else
				{
					streamChannelList << "" ;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);

	return streamChannelList;
}

int QWebBrowserApp::UpdateVirChMedia(const  QString  strVirChMedia, int  iChannelId)
{
	INT_t iRet,iOutRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;
	//ebug ()  << " strVirChMedia" << strVirChMedia;
	do
	{

		iRet = mMediaVirChannel_sp->getVirChInfo(iChannelId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_strUrl == strVirChMedia)	//the new value is equal to the old value
			{
				break;
			}
			else
			{
				VirChInfo_sp->m_strUrl = strVirChMedia;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_strUrl = strVirChMedia;
			iRet = mMediaVirChannel_sp->setVirChInfo(iChannelId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::setAutoReboot(int iEnabled,int &iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		/*if(iEnabled == 1)
		{
			int iTempTime = mp_Config->getConfigValue(INI_KEY_AutoRebootInterVal).toInt();
			if(iTempTime > 0)
			{
				if(!m_CheckRebootTimer.isActive())
				{
					m_CheckRebootTimer.start(iTempTime*1000);
				}
			}
			mp_Config->setConfigValue(INI_KEY_AutoRebootEnable,QString::number(iEnabled));
		}else if(iEnabled == 0)
		{
			if(m_CheckRebootTimer.isActive())
			{
				m_CheckRebootTimer.stop();
			}
			mp_Config->setConfigValue(INI_KEY_AutoRebootEnable,QString::number(iEnabled));
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}*/
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_AutoRebootEnable,QString::number(iEnabled));;

	}while(FALSE);
}

void QWebBrowserApp::setAutoRebootInterval(int iTime,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		mp_Config->setConfigValue(INI_KEY_AutoRebootInterVal,QString::number(iTime));
	}while(FALSE);
}

int QWebBrowserApp::DeleteVirChMedia(const QString strVirChMedia , int  iChannelId)
{
	INT_t iRet,iOutRet;
	QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;

	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mMediaVirChannel_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = mMediaVirChannel_sp->getVirChInfo(iChannelId, OUT VirChInfo_sp);
		if(ERROR_SUCCESS == iRet)
		{
			if(VirChInfo_sp->m_strUrl == strVirChMedia)	//the new value is equal to the old value
			{
				VirChInfo_sp->m_strUrl = "";
				iRet = mMediaVirChannel_sp->setVirChInfo(iChannelId, VirChInfo_sp);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else
			{
				VirChInfo_sp->m_strUrl = strVirChMedia;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			VirChInfo_sp = QSharedPointer <QMediaVirtualChannel::QVirChInfo> (new QMediaVirtualChannel::QVirChInfo);
			if(VirChInfo_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			VirChInfo_sp->m_strUrl = "";
			iRet = mMediaVirChannel_sp->setVirChInfo(iChannelId, VirChInfo_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = iRet;
			break;
		}
		//Schedule a timer for save the above changes
		m_SaveMediaVirCh_Timer.setSingleShot(TRUE);
		m_SaveMediaVirCh_Timer.start(2*1000);
	}while(FALSE);

	return iOutRet;
}

QStringList QWebBrowserApp::getChannelIdList()
{
	INT_t iRet;
	QStringList TempVirChIdList;
	QStringList VirChIdList;

	do
	{
		if(mMediaVirChannel_sp.isNull())
		{
			break;
		}
		iRet = mMediaVirChannel_sp->getVirChIdList(TempVirChIdList);
		if(iRet != ERROR_SUCCESS)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		for(int i =0 ;i< TempVirChIdList.size() ;i ++)
		{
			if(TempVirChIdList[i].toInt()  < 10 )
			{
				VirChIdList << TempVirChIdList[i];
			}
		}
	}while(FALSE);

	return VirChIdList;
}

void QWebBrowserApp::getAutoRebootInterval(int & iOutRet,int &iTime)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iTime = mp_Config->getConfigValue(INI_KEY_AutoRebootInterVal).toInt();
	}while(FALSE);
}

INT_t QWebBrowserApp::OnNewSysCfgDownloadFinished(quint32 DownloadId)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	Q_UNUSED(DownloadId);

	do
	{
		if(false == QFile::exists(NewSysCfg_DOWNLOAD_TEMP_FILE_PATH))
		{
			break;
		}
		{
			QFileInfo NewSysCfgFileInfo(NewSysCfg_DOWNLOAD_TEMP_FILE_PATH);
			if((50*1024*1024) < NewSysCfgFileInfo.size())
			{
				iOutRet = ERR_FILE_SIZE_TOO_LARGE;
				LOG_BLINE("FileSize(%s) is too large(%llu)\n", NewSysCfg_DOWNLOAD_TEMP_FILE_PATH, NewSysCfgFileInfo.size());
				break;
			}
		}
		/*
		NewSysCfg_DOWNLOAD_TEMP_FILE_PATH will be deleted in ExtractNewSysCfg.sh.
		A dbus signal org.thtfit.SystemService.CfgMgr.NewSysCfgIsReady will be sent after ExtractNewSysCfg.sh finishes.
		*/
		iRet = system((APP_DIR_PATH + "ExtractNewSysCfg.sh").toUtf8());
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_RUN_ExtractSysCfg_FAIL;
			LOG_BLINE("Run(%s) err %d\n", qPrintable(APP_DIR_PATH + "ExtractNewSysCfg.sh"), iRet);
			break;
		}
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::ShowLogo()
{
	QString  strIPv4Addr,  SubnetMask;
	do
	{
		m_ShowlogoWin.showMaximized();
		QString strPictureName,strPictureUrl;
		strPictureName = QString(INTERNALIMAGEID).arg(QString::number(0));
		strPictureUrl = APP_DIR_PATH +  CONFIG_NAME +"/"+ strPictureName;
		if(!QFile::exists(strPictureUrl))
		{
			strPictureUrl = APP_DIR_PATH +  IMAGE_PATH_NAME +"/"+ strPictureName;
			if(!QFile::exists(strPictureUrl))
			{
				break;
			}
		}
		if(mp_Config.isNull())
		{
			break;
		}

		SubnetMask = mp_Config->getConfigValue(KEY_NetMgrEthernet_SubnetMask);
		m_ShowlogoWin.playBootLogo(strPictureUrl,m_strIPv4Addr,SubnetMask);
		m_ShowlogoWin.update();

		QTimer::singleShot(2*1000, this, SLOT(On_ShowLogoTimeOut()));

	}while(FALSE);
}

QString QWebBrowserApp::getGpioPlayFilePath(int iGpioKey)
{
	QString strFilePath;
	QMap < int, QSharedPointer <QGpioFilePath> >::iterator itGpioFilePath;

	do
	{
		itGpioFilePath = m_GpioFilePath_Map.find(iGpioKey);
		if(m_GpioFilePath_Map.end() == itGpioFilePath)
		{
			//LOG_BLINE("Not found\n");
			break;
		}
		if(m_GpioFilePath_Map.isEmpty())
		{
			break;
		}
		if(m_GpioFilePath_Map.size() < iGpioKey)
		{
			break;
		}
		strFilePath = (m_GpioFilePath_Map.value(iGpioKey))->m_strUrl ;
	}while(FALSE);

	//LOG_BLINE("%s\n", qPrintable(strFilePath));

	return strFilePath;
}

void QWebBrowserApp::On_VidOutModeChanged(int iCurrMode)
{	
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("On_VidOutModeChanged(%d)\n", iCurrMode);
	}

	do
	{
              m_iOldVideoOutMode=m_iVideoOutMode;
              m_iVideoOutMode = (VIDEO_OUTPUT_MODE)iCurrMode;            
		QString strVoModeDescForLedDisp;
		MP7XX::QAudioVideoInOutInfo::getVoDescForLedDisp(m_iVideoOutMode, OUT strVoModeDescForLedDisp);
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(strVoModeDescForLedDisp, 1);
		}
#if 1/*added by lxj 2012-8-3 for refresh video mode*/
		m_Browser.RefreshUiVideoMode();
#endif
	}while(false);

	setGuiOsd2AccordingToCurVideoMode();
}

void QWebBrowserApp::setGuiOsd2AccordingToCurVideoMode()
{
	INT_t iRet;
	
	if(MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iVideoOutMode)
	{
		iRet = m_GuiOsd2Ctrl.createGuiOsd2();
		if(ERR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	else
	{
		iRet = m_GuiOsd2Ctrl.releaseGuiOsd2();
		if(ERR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
}
	
int QWebBrowserApp::On_tnetReady_Sig()
{
	int iOutRet = ERROR_SUCCESS;
	bool bRet;
	QDBusMessage DbusMsgToSend;

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createSignal(
			DBUS_PATH_WebBrowser_DatCommSrv, DBUS_IF_WebBrowser_DataCommSrv, "tnetReady");
		bRet = m_DBusConn_sp->send(DbusMsgToSend);
		if(false == bRet)
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int QWebBrowserApp::On_HtmlPlaybackEos_Sig(const QString CurPlayUrl, const unsigned int PlaybackId, const int bIsCompletePlaybackEos)
{
	int iOutRet = ERROR_SUCCESS;
	bool bRet;
	QDBusMessage DbusMsgToSend;

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createSignal(
			DBUS_PATH_WebBrowser_UiSrv, DBUS_IF_ControlCenter_UiSrv, MP_SIGNAL_PlaybackEos2);
		DbusMsgToSend<<CurPlayUrl<<PlaybackId<<bIsCompletePlaybackEos;
		bRet = m_DBusConn_sp->send(DbusMsgToSend);
		m_bIsCompletePlaybackEos=FALSE;
		if(false == bRet)
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
	}while(FALSE);

	return iOutRet;	
}

void QWebBrowserApp::setTrapEnable(int iEnable,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(iEnable  > 1   ||  iEnable <  0 )
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_TrapEnable,QString::number(iEnable));
	}while(FALSE);
}

void QWebBrowserApp::getTrapEnable(int & iOutRet,int & iEnable)
{

	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iEnable = mp_Config->getConfigValue(INI_KEY_TrapEnable).toInt();
	}while(FALSE);
}


void QWebBrowserApp::getCoordinateMode(int & iOutRet,int &iMode)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iMode = mp_Config->getConfigValue(INI_KEY_CoordinateMode).toInt();
	}while(FALSE);
}

QString QWebBrowserApp::getDefEthDevName()
{
	QString strDefEthDevName = PREFERRED_ETH_DEV_gphy;
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	QList <QVariant> MsgArgsList;
	int TimeoutMs;

	if(m_DBusConn_sp.isNull())
	{
		LOG_BLINE("no dbus conn\n");
		goto EXIT_PROC;
	}

	DbusMsgToSend = QDBusMessage::createMethodCall(SS_DBUS_SERVICE_NAME, DBUS_PATH_NetworkManagerSrv_Ethernet,
		DbusIf_SysSrv_NetMgrSrv_Ethernet, "getDefEthDevName");
	DbusMsgToSend.setArguments(MsgArgsList);
	TimeoutMs = (5*1000);
	DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
	if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
	{
		LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		goto EXIT_PROC;
	}
	else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
	{
		LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		goto EXIT_PROC;
	}
	MsgArgsList = DbusMsgReply.arguments();
	if(1 > MsgArgsList.count())
	{
		LOG_BLINE("no reply arg\n");
		goto EXIT_PROC;
	}
	//replied arg 0
	{
		CONST QVariant & Reply_Arg = MsgArgsList.at(0);
		strDefEthDevName = Reply_Arg.toString();
	}

EXIT_PROC:
	return strDefEthDevName;
}

void QWebBrowserApp::On_DhcpDone2(const QString & strNetDevName, const QString & strIPv4Addr,const QString & strSubnetMask,
	const QString & strGatewayAddr,const QString & strDns)
{
	int iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(getDefEthDevName() == strNetDevName)
		{
			//cache eth ip
			iRet = getIPv4AddrFromStr(strIPv4Addr.toLocal8Bit(), &m_uiCurEthIpV4Addr);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("ip %s err %d\n", qPrintable(strIPv4Addr), iRet);
			}
			//
			mp_Config->setConfigValue(KEY_NetMgrEthernet_IPv4Addr, strIPv4Addr);
			iRet = SysProp_setPersistent(SysProp_KEY_CurEthernetIP, strIPv4Addr.toUtf8().constData());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			mp_Config->setConfigValue(KEY_NetMgrEthernet_SubnetMask, strSubnetMask);
			mp_Config->setConfigValue(KEY_NetMgrEthernet_GatewayAddr, strGatewayAddr);
			QStringList strDnsAddrlist = strDns.split(" ");
			if(strDnsAddrlist.size() >=2)
			{
				mp_Config->setConfigValue(KEY_NetMgrEthernet_DNSAddr1, strDnsAddrlist[0]);
				mp_Config->setConfigValue(KEY_NetMgrEthernet_DNSAddr2, strDnsAddrlist[1]);
			}
			else if(strDnsAddrlist.size() >=1)
			{
				mp_Config->setConfigValue(KEY_NetMgrEthernet_DNSAddr1, strDnsAddrlist[0]);
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::getAutoReboot(int & iOutRet,int &iEnable)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iEnable = mp_Config->getConfigValue(INI_KEY_AutoRebootEnable).toInt();
	}while(FALSE);
}

int QWebBrowserApp::setPluginListDataSource(const QSharedPointer <QPlaylistData> & PlaylistData_sp,const QString strPlayListUrl)
{
	INT_t iRet = ERROR_SUCCESS;
	do
	{
		if(m_PluginPlaylistData_sp.isNull() || PlaylistData_sp.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		StopPlayPluginList();
		m_PluginPlaylistData_sp->ClearPlayListData();
		m_PluginPlaylistData_sp = PlaylistData_sp;
		if(strPlayListUrl.trimmed()=="")
		{
			m_strCurrPluginListName = strPlayListUrl;;
			m_strCurrPluginListUrl = strPlayListUrl;
			m_strCurrPluginListControlName = strPlayListUrl;
		}
		else
		{
			QFileInfo playlistUrlInfo(strPlayListUrl);
			m_strCurrPluginListName = playlistUrlInfo.fileName();;
			m_strCurrPluginListUrl = strPlayListUrl;
			m_strCurrPluginListControlName = m_PluginPlaylistData_sp->getCurrentPlayListName();
		}
	}while(FALSE);

	return iRet;
}

INT_t QWebBrowserApp::PlayPluginListNextEntry(bool bFirstEntry/* = false*/,bool bPreEntry)
{
	INT_t iRet = ERROR_SUCCESS;
	QSharedPointer <QPlaylistEntry> PlayEntry_sp;

	PlayPluginListflag++;

	LOG_LINE("PlayPluginListNextEntry(%d,%d)\n",bFirstEntry,bPreEntry);
	if(m_PluginPlaylistData_sp->isEmpty())
	{
		iRet = ERROR_INVALID_STATE;
		return iRet;
	}
	if(m_PluginPlaylistData_sp.isNull())
	{
		iRet = ERROR_OUT_OF_MEMORY;
		return iRet;
	}

	if(bPreEntry)
	{
		PlayEntry_sp = m_PluginPlaylistData_sp->MoveToPreHeadPlayEntry();
	}
	else
	{
		if(bFirstEntry)
		{
			PlayEntry_sp = m_PluginPlaylistData_sp->MoveToFirstPlayEntry();
		}
		else
		{
			if(!m_bIsNeedLoopPlaylistBEntry)
			{
				PlayEntry_sp = m_PluginPlaylistData_sp->MoveToNextPlayEntry();
			}
			else
			{
				PlayEntry_sp = m_PluginPlaylistData_sp->MoveToNextPlayEntry(FALSE);
			}
		}
	}

	do 
	{
		if(PlayEntry_sp.isNull())	//end of list
		{			
			if(m_bIsPlayPluginList)
			{
				emit onPlayPluginListBackEnd();
			}
			m_strCurrPluginListName ="";
			m_strCurrPluginListUrl = "";
			break;
		}
		
		if(QPlaylistEntry::ACTION_Unknown != PlayEntry_sp->getActionType())
		{
			//m_eMediaPlayCurrStatus = MediaPlay_Status_PlayPlayList;
			m_bIsPlayPluginList = true ;
		}
		m_bIsPlayPluginList = true ;
		if(QPlaylistEntry::ACTION_Video == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Video> PlayEntry_Video_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Video>();
			if(PlayEntry_Video_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Video_sp->getActionType(),
				PlayEntry_Video_sp->m_strFilePath);
			m_iStateType = State_PlayingVideoFile;

			if(PlayEntry_Video_sp->m_iDuration == DEFAULT_PLAY_DURATION_TIME
				&& PlayEntry_Video_sp->m_iLoopTimes > 1 )
			{
				iRet = PlayUrl(strActualFilePath,PlayEntry_Video_sp->m_iLoopTimes);
			} 
			else
			{
				iRet = PlayUrl(strActualFilePath);
				PlayEntry_Video_sp->m_iHasLoopTimes ++;
				if( PlayEntry_Video_sp->m_iHasLoopTimes  >=  PlayEntry_Video_sp->m_iLoopTimes )
				{
					PlayEntry_Video_sp->m_iHasLoopTimes =0;
					m_bIsNeedLoopPlaylistBEntry = false ;
				}
				else
				{
					m_bIsNeedLoopPlaylistBEntry = true ;
				}
				if(PlayEntry_Video_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
				{
					int iTimerTimeoutMs = (PlayEntry_Video_sp->m_iDuration * 1000);
					if(0 < iTimerTimeoutMs)
					{
						if(m_PlayPluginListEntryDurationTimer.isActive())
						{
							m_PlayPluginListEntryDurationTimer.stop();
						}
						m_PlayPluginListEntryDurationTimer.start(iTimerTimeoutMs);
					}
				}
			}
			if(iRet != ERROR_SUCCESS)
			{
				PlayPluginListNextEntry();
				break;
			}
			else
			{
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				 if(&m_Browser== pActiveWindow )
				{
					UpdateVideoPosition();
				}
				
				if(m_bIsPlayVideoLayer)
				{
					setPlayListActionPosition(PlayEntry_Video_sp->m_xLeft,PlayEntry_Video_sp->m_yUpper,
					PlayEntry_Video_sp->m_xSize,PlayEntry_Video_sp->m_ySize);
				}
				m_iStateType = State_PlayingVideoFile;
				break;
			}
			m_iStateType = State_PlayingVideoFile;
		}
		else if(QPlaylistEntry::ACTION_Picture == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Picture> PlayEntry_Picture_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Picture>();
			if(PlayEntry_Picture_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Picture_sp->getActionType(),
				PlayEntry_Picture_sp->m_strFilePath);

			m_iStateType = State_PlayingPictureFile;
			iRet = PlayUrl(strActualFilePath);
			if(iRet != ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoopPlaylistBEntry = false ;
#endif
				PlayPluginListNextEntry();
				break;
			}
			 if(ERROR_SUCCESS ==iRet)
			 {
				if(PlayEntry_Picture_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
				{
					int iTimerIntervalMs = (PlayEntry_Picture_sp->m_iDuration * 1000);
					if(0 == iTimerIntervalMs)
					{
						iTimerIntervalMs = 100;
					}
					if(0 < iTimerIntervalMs)
					{
						if(m_PlayPluginListEntryDurationTimer.isActive())
						{
							m_PlayPluginListEntryDurationTimer.stop();
						}
						m_PlayPluginListEntryDurationTimer.start(iTimerIntervalMs);
					}
				}
				else
				{
					m_PlayPluginListEntryDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
				}

				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				
				 if(&m_Browser== pActiveWindow )
				{
					UpdateVideoPosition();
				}
				
				if(m_bIsPlayVideoLayer)
				{
					setPlayListActionPosition(PlayEntry_Picture_sp->m_xLeft,PlayEntry_Picture_sp->m_yUpper,
					PlayEntry_Picture_sp->m_xSize,PlayEntry_Picture_sp->m_ySize);
				}
			 }
			 else
			 {
			 	StopPlayDurationTimer();
			 }
			m_iStateType = State_PlayingPictureFile;
		}
		else if(QPlaylistEntry::ACTION_Audio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Audio> PlayEntry_Audio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Audio>();
			if(PlayEntry_Audio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Audio_sp->getActionType(),
				PlayEntry_Audio_sp->m_strFilePath);
			 m_iStateType = State_PlayingAudioFile;
			iRet = PlayUrl(strActualFilePath);
			if(iRet != ERROR_SUCCESS)
			{
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
				m_bIsNeedLoopPlaylistBEntry = false ;
#endif
				PlayPluginListNextEntry();
				break;
			}
			if(PlayEntry_Audio_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerIntervalMs = (PlayEntry_Audio_sp->m_iDuration * 1000);
				if(0 == iTimerIntervalMs)
				{
					iTimerIntervalMs = 100;
				}
				if(0 < iTimerIntervalMs)
				{
					if(m_PlayPluginListEntryDurationTimer.isActive())
					{
						m_PlayPluginListEntryDurationTimer.stop();
					}
					m_PlayPluginListEntryDurationTimer.start(iTimerIntervalMs);
				}
			}
			 else
			 {
			 	StopPlayDurationTimer();
			 }
			 m_iStateType = State_PlayingAudioFile;
		}
		else if(QPlaylistEntry::ACTION_Html== PlayEntry_sp->getActionType() ||
			    QPlaylistEntry::ACTION_Browser== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Html> PlayEntry_Html_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Html>();
			if(PlayEntry_Html_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_Html_sp->getActionType(),
				PlayEntry_Html_sp->m_strFilePath);

			if(strActualFilePath == "")
			{
				strActualFilePath = PlayEntry_Html_sp->m_strFilePath;
			}

			printf("[%s:%d] playlist is browser m_bIsPlayVideoLayer = %d\n",__FILE__,__LINE__,m_bIsPlayVideoLayer);

			if(m_bIsPlayVideoLayer)
			{
#if 1/*by su for layerplay play picture not full*/
				setPlayListActionPosition(PlayEntry_Html_sp->m_xLeft,PlayEntry_Html_sp->m_yUpper,
				PlayEntry_Html_sp->m_xSize,PlayEntry_Html_sp->m_ySize);
#endif
				if(!m_playWin.getVideoLayerWnd().isNull())
				{
					m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayer(strActualFilePath);
				}
				if(m_MediaPlayerCtrl_sp)
				{
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
				}
				m_iStateType = State_PlayingBrowserFile;
				m_eCurPlayMode = ControlCenter::PlayMode_Normal;
			}
			else
			{
				if(m_playWin.getPlayHtmlOrPicWin().isNull())
				{
					if(Sw_LogDbusCmd)
					{
						LOG_BLINE("play html or picture window is null\n");
					}
					break;
				}
				iRet =m_playWin.getPlayHtmlOrPicWin()->playPluginHtmlEntry(strActualFilePath);
				if(iRet !=ERROR_SUCCESS)
				{
					if(Sw_LogDbusCmd)
					{
						LOG_BLINE("playPluginHtmlEntry faile \n");
					}
				}
			}
			if(PlayEntry_Html_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerIntervalMs = (PlayEntry_Html_sp->m_iDuration * 1000);
				if(0 == iTimerIntervalMs)
				{
					iTimerIntervalMs = 100;
				}
				if(0 < iTimerIntervalMs)
				{
					if(m_PlayPluginListEntryDurationTimer.isActive())
					{
						m_PlayPluginListEntryDurationTimer.stop();
					}
					if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
					{
						m_PlayPluginListHtmlEntryDurationTimer.stop();
					}
					m_PlayPluginListHtmlEntryDurationTimer.start(iTimerIntervalMs);
				}
			}
			else
			{
				m_PlayPluginListHtmlEntryDurationTimer.start((DEFAULT_PICTURE_DURATION * 1000));
			}
			m_iStateType = State_PlayingBrowserFile;
			m_PlaybackId = 0;
			m_strCurPlayUrl = strActualFilePath;
		}
		else if(QPlaylistEntry::ACTION_PlaySavedPlaylist == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlaySavedPlaylist> PlayEntry_PlaySavedPlaylist_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlaySavedPlaylist>();
			if(PlayEntry_PlaySavedPlaylist_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			StopPlayPluginList();
			StopPlayback();
#if 0/*No need recall LoadPlaylist*/
			LoadPlaylist();
#else
			QString playListUrl = m_strDefaultStartupItem;			
			if(playListUrl.trimmed()==""  || !QFile::exists(playListUrl))
			{
				QFileInfo playListInfo(playListUrl);
				QString strFileName = playListInfo.fileName();
				playListUrl = applicationDirPath() + "/" + m_strActiveStorageDirName +"/" + FOLDER_NAME_PLAYLIST +"/"+ strFileName;
			}
			
			m_PluginPlaylistData_sp->LoadFromIniFile(&playListUrl);
#endif
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoopPlaylistBEntry = false;
#endif
			PlayPluginListNextEntry(true);
		}
		else if(QPlaylistEntry::ACTION_Volume == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlayerVolume> PlaylistEntry_SetPlayVolume_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlayerVolume>();
			if(PlaylistEntry_SetPlayVolume_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			int iOutRet;
			setPlayVolume(PlaylistEntry_SetPlayVolume_sp->m_iVolumeVaule,iOutRet);
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoopPlaylistBEntry = false ;
#endif
			PlayPluginListNextEntry();
		}
		else if(QPlaylistEntry::ACTION_Reboot == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Reboot> PlayEntry_Reboot_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Reboot>();
			if(PlayEntry_Reboot_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			ReStart();
		}
		else if(QPlaylistEntry::ACTION_MuteVolume == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_PlayerMuteVolume> PlaylistEntry_SetPlayVolumeMute_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_PlayerMuteVolume>();
			if(PlaylistEntry_SetPlayVolumeMute_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			int iOutRet;
			setPlayerMute(PlaylistEntry_SetPlayVolumeMute_sp->m_iIsMute,iOutRet);
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoopPlaylistBEntry = false ;
#endif
			PlayPluginListNextEntry();
		}
		else if(QPlaylistEntry::ACTION_Loop == PlayEntry_sp->getActionType())
		{
			int PlayEntry_Loop_id = m_PluginPlaylistData_sp->getCurPlayEntryId();
			QSharedPointer <QPlaylistEntry_Loop> PlayEntry_Loop_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Loop>();
			if(PlayEntry_Loop_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			PlayEntry_Loop_sp->m_iHasLoopTimes ++;

			if(1 >= m_PluginPlaylistData_sp->PlayEntryCount() || 0 == PlayEntry_Loop_id)
			{
				break;
			}
			if( PlayEntry_Loop_sp->m_iTimes == -1)
			{

			}
			else if( PlayEntry_Loop_sp->m_iHasLoopTimes  >=  PlayEntry_Loop_sp->m_iTimes )
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				break;
			}
			//Process this message later because we may trap into this infinite loop.
			removePostedEvents(this, QEVENT_TYPE_PlayPluginListFirstEntry);
			QEvent * pEvent = new QEvent((enum QEvent::Type)QEVENT_TYPE_PlayPluginListFirstEntry);
			if(NULL == pEvent)
			{
				break;
			}
			postEvent(this, pEvent);
		}
		else if(QPlaylistEntry::ACTION_Stream == PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_Stream> PlayEntry_Stream_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_Stream>();
			if(PlayEntry_Stream_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}

			PlayEntry_Stream_sp->m_iHasLoopTimes ++;
			if( PlayEntry_Stream_sp->m_iLoopTimes == -1)
			{
				m_bIsNeedLoopPlaylistBEntry = false ;
			}
			else  if( PlayEntry_Stream_sp->m_iHasLoopTimes  >=  PlayEntry_Stream_sp->m_iLoopTimes )
			{
				PlayEntry_Stream_sp->m_iHasLoopTimes = -1;
				m_bIsNeedLoopPlaylistBEntry = false ;
				PlayPluginListNextEntry();
				break;
			}
			else
			{
				m_bIsNeedLoopPlaylistBEntry = true ;
			}

			QString strActualFilePath = PlayEntry_Stream_sp->m_strFilePath;
			
			if(strActualFilePath.indexOf("tuner://") == 0)
			{
				QString strTunerUrl = strActualFilePath.mid(8);	
				m_iStateType = State_PlayingTuner;
				setPlayTuner(strTunerUrl, iRet);
				m_iStateType = State_PlayingTuner;
			}
			else
			{
				m_iStateType = State_Streaming;
				iRet = PlayUrl(strActualFilePath);
			}
			
			if(iRet != ERROR_SUCCESS)
			{
				PlayPluginListNextEntry();
				break;
			}
			if(PlayEntry_Stream_sp->m_iDuration != DEFAULT_PLAY_DURATION_TIME)
			{
				int iTimerIntervalMs = PlayEntry_Stream_sp->m_iDuration * 1000;
				if(0 == iTimerIntervalMs)
				{
					iTimerIntervalMs = 100;
				}
				if(0 < iTimerIntervalMs)
				{
					if(m_PlayPluginListEntryDurationTimer.isActive())
					{
						m_PlayPluginListEntryDurationTimer.stop();
					}
					m_PlayPluginListEntryDurationTimer.start(iTimerIntervalMs);
				}
			}
			
			if(m_bIsPlayVideoLayer)
			{
				setPlayListActionPosition(PlayEntry_Stream_sp->m_xLeft,PlayEntry_Stream_sp->m_yUpper,
				PlayEntry_Stream_sp->m_xSize,PlayEntry_Stream_sp->m_ySize);
			}
		}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
		else if(QPlaylistEntry::ACTION_MediaServer_PlayAudio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlayEntry_MediaServerAudio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_MediaServerAudio>();
			if(PlayEntry_MediaServerAudio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}
			QString strActualFilePath = LookForActualFilePath(PlayEntry_MediaServerAudio_sp->getActionType(),
				PlayEntry_MediaServerAudio_sp->m_strFilePath);

			m_iStateType = State_PlayingAudioFile;
			iRet = MediaServerPlayAudio(strActualFilePath);

			//LOG_BLINE("MediaServerPlayAudio(%s),iRet=%d\n",qPrintable(strActualFilePath),iRet);

#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoopPlaylistBEntry = false;
#endif
			/*always to next,play in background*/
			PlayPluginListNextEntry();
		}
		else if(QPlaylistEntry::ACTION_MediaServer_StopAudio== PlayEntry_sp->getActionType())
		{
			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlayEntry_MediaServerAudio_sp = PlayEntry_sp.dynamicCast<QPlaylistEntry_MediaServerAudio>();
			if(PlayEntry_MediaServerAudio_sp.isNull())
			{
				PRINT_BFILE_LINENO_BUG_STR;
				break;
			}

			m_iStateType = State_Idle;
			iRet = MediaServerStopAudio();

			//LOG_BLINE("MediaServer_StopAudio(),iRet=%d\n",iRet);
#if 1/*added by lxj 2012-10-11 for avoid always loop to playlist entry*/
			m_bIsNeedLoopPlaylistBEntry = false ;
#endif
			/*always to next*/
			PlayPluginListNextEntry();
		}
#endif
	}while(FALSE);
	return iRet;
}


void QWebBrowserApp::checkAutoReboot()
{
	int iEnabledReboot;
	do
	{
		iEnabledReboot = mp_Config->getConfigValue(INI_KEY_AutoRebootEnable).toInt();
		if(iEnabledReboot == 1)
		{
			int iTempTime = mp_Config->getConfigValue(INI_KEY_AutoRebootInterVal).toInt();
			if(iTempTime == 0)
			{
				if(m_CheckRebootTimer.isActive())
				{
					m_CheckRebootTimer.stop();
				}
				break;
			}
			else if(iTempTime > 60)
			{
				if(!m_CheckRebootTimer.isActive())
				{
					m_CheckRebootTimer.start(iTempTime*1000);
				}
			}
			else
			{
				m_CheckRebootTimer.start(60 *1000);
			}
		}
		else if(iEnabledReboot == 0)
		{
			if(m_CheckRebootTimer.isActive())
			{
				m_CheckRebootTimer.stop();
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::OnTimer_checkPlayPluginEntryDurationTimeOut()
{
	int  iRet = ERROR_SUCCESS;
	do
	{
		if(m_PlayPluginListEntryDurationTimer.isActive())
		{
			m_PlayPluginListEntryDurationTimer.stop();
		}
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		iRet = stopMPlayerWithEventLog();
		if(ERR_NO_QDBUS_CONN == iRet)
		{
			OnTimer_ConnectDBus();
			if(m_DBusConn_sp->isConnected())
			{
				iRet = stopMPlayerWithEventLog();
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		else if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);
}

INT_t QWebBrowserApp::StopPlayPluginList()
{
	INT_t  iRet = ERROR_SUCCESS;
	do
	{
		if(m_PluginPlaylistData_sp.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		m_PluginPlaylistData_sp->ClearPlayListData();
		if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
		{
			m_PlayPluginListHtmlEntryDurationTimer.stop();
		}
		if(m_PlayPluginListEntryDurationTimer.isActive())
		{
			m_PlayPluginListEntryDurationTimer.stop();
		}		
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		iRet = stopMPlayerWithEventLog();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);
	
	return iRet;
}

void QWebBrowserApp::OnTimer_checkPlayPluginHtmlEntryDurationTimeOut()
{
	int  iRet = ERROR_SUCCESS;
	do
	{
		if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
		{
			m_PlayPluginListHtmlEntryDurationTimer.stop();
		}
		if(m_bIsPlayVideoLayer)
		{
			if(!m_playWin.getVideoLayerWnd().isNull())
			{
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();
			}
		}
		else
		{	
			if(m_playWin.getPlayHtmlOrPicWin().isNull())
			{
				if(Sw_LogDbusCmd)
				{
					LOG_BLINE("play html or picture window is null\n");
				}
				break;
			}
			iRet =m_playWin.getPlayHtmlOrPicWin()->playPluginHtmlEntryEnd_timeOut();
		}
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		else
		{
			PlayPluginListNextEntry();
		}
	}while(FALSE);
}


QString QWebBrowserApp::getPluginPlayListUrl()
{
	int  iRet = ERROR_SUCCESS;
	QString strPlayListPath;
	do
	{
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		strPlayListPath = mp_Config->getConfigValue(INI_KEY_CurPlaylistBFilePath);
	}while(FALSE);
	return strPlayListPath;
}


void QWebBrowserApp::setAlphaLayer(int iAlphaValue,int & iOutRet)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(iAlphaValue< 0 ||  iAlphaValue > 255)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}

		mp_Config->setConfigValue(INI_KEY_AlphaValue,QString::number(iAlphaValue));

	}while(FALSE);
}

void QWebBrowserApp::setActiveLayer(int iLayerId,int & iOutRet)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(iLayerId< 0 ||  iLayerId > 1)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_LayerNumber,QString::number(iLayerId));
	}while(FALSE);
}

QString QWebBrowserApp::ParsePlayFilePathToPlayUrl(const QString & strPlayFilePath,QString & fileType)
{
	QString strDataSrcDevice, strFileType, strFilename,strPlayFileUrl;
	int iIndex;
	strPlayFileUrl = "";
	do
	{
		//first part
		iIndex = strPlayFilePath.indexOf(":");
		if(0 < iIndex)
		{
			strDataSrcDevice = strPlayFilePath.left(iIndex);
		}
		else if(0 == iIndex)
		{
			break;
		}
		else
		{
			strDataSrcDevice = strPlayFilePath;
		}
		if(strDataSrcDevice.isEmpty())
		{
			break;
		}
		//second part
		if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_USB, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_IDE, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive) )
		{
			if(0 < iIndex)
			{
				int iColonIndex;
				iColonIndex = strPlayFilePath.indexOf("://", iIndex+2);
				if(0 < iColonIndex)
				{
					strFileType = strPlayFilePath.mid(iIndex+2, iColonIndex - (iIndex + 2));
					//LOG_BLINE("%s\n", qPrintable(strFileType));
					if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
					{
						fileType = strFileType;
						//the third part
						strFilename = strPlayFilePath.mid(iColonIndex+3);
						if(FALSE == strFilename.isEmpty())
						{
							QString strFileFullPath;
							strFileFullPath = getFileFullPathByCeLabsUrl(strDataSrcDevice, strFileType, strFilename);
							if(!QFile::exists(strFileFullPath))
							{
								break;									break;
							}
							strPlayFileUrl = strFileFullPath ;
						}
					}
				}
			}
		}
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_UDP, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_RTP, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_TUNER, Qt::CaseInsensitive))
		{
			strPlayFileUrl = strPlayFilePath;
			fileType = strDataSrcDevice;
		}
		else  if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_WEBPAGE, Qt::CaseInsensitive) )
		{
			strPlayFileUrl = strPlayFilePath;
			fileType = strDataSrcDevice;
		}
	}while(FALSE);

	return strPlayFileUrl;
}

void QWebBrowserApp::getCardCurrentStatus(int & iOutRet, QString &strCardCurStatus, BOOL_t bOnlyGetRawData/* = FALSE*/,
	OUT quint8 * pRawDataBuf/* = NULL*/, size_t sizeRawDataBuf/* = 0*/, size_t * pActualRawDataSize/* = NULL*/)
{
	int iRet;
	BYTE szCardCurrentStatus[1024];
	size_t sizeCardCurrentStatus;

	szCardCurrentStatus[0] = szCardCurrentStatus[sizeof(szCardCurrentStatus)-1] = '\0';
	sizeCardCurrentStatus = 0;
	iOutRet = ERROR_SUCCESS;

	do
	{
		//IP
		{
			BOOL_t bAutoGetIp = TRUE;
			UINT32_t uiIpV4Addr = 0;
			UINT32_t uiSubnetMaskV4 = 0;
			iRet = m_Mp7xxGlobalStatusData.getCurEthSettings(OUT bAutoGetIp, OUT uiIpV4Addr, OUT uiSubnetMaskV4);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			UINT32_t uiEthIpV4Addr_NetByteOrder;
			if(bAutoGetIp && 0 != m_uiCurEthIpV4Addr)
			{
				uiEthIpV4Addr_NetByteOrder = htonl(m_uiCurEthIpV4Addr);
			}
			else
			{
				uiEthIpV4Addr_NetByteOrder = htonl(uiIpV4Addr);
			}
			memcpy(szCardCurrentStatus+sizeCardCurrentStatus, &uiEthIpV4Addr_NetByteOrder, 4);
			sizeCardCurrentStatus += 4;
		}
		//Display power status
		szCardCurrentStatus[sizeCardCurrentStatus] = 1;
		sizeCardCurrentStatus += 1;


		//Media Name field length
		{
			QString strPlayType , strPlayFileName,strPlayFileInfo;
			getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
			if(strPlayFileInfo.trimmed()!="")
			{
				QFileInfo playListInfo(strPlayFileInfo);
				strPlayFileName = playListInfo.fileName();
				if(strPlayFileName=="")
				{
					strPlayFileName  = strPlayFileInfo;
				}
			}
			else
			{
				strPlayFileName ="";
			}

			//media name field length
			BYTE MediaNameLength = strPlayFileName.length()+1;
			szCardCurrentStatus[sizeCardCurrentStatus] = MediaNameLength;
			sizeCardCurrentStatus += 1;

			//Media Name in ASCII format.
			if(MediaNameLength > sizeof(szCardCurrentStatus)-sizeCardCurrentStatus)
			{
				MediaNameLength = sizeof(szCardCurrentStatus)-sizeCardCurrentStatus;
			}
			memcpy(szCardCurrentStatus+sizeCardCurrentStatus, strPlayFileName.toLocal8Bit(), MediaNameLength);
			sizeCardCurrentStatus += MediaNameLength;
		}

		//Other Status field length
		szCardCurrentStatus[sizeCardCurrentStatus] = 32;
		sizeCardCurrentStatus += 1;
#define OtherStatusFiled 17
		//Other Status field.
		for(int i=0;i<OtherStatusFiled;i++)
		{
			szCardCurrentStatus[sizeCardCurrentStatus] =48;
			sizeCardCurrentStatus += 1;
		}

		char szCardTransferStatus[64];
		qint64 iFileTransferredSize = 0;
		int iFormatOrScanPercentage = 0;
		int sizeMediaStatus = 0;

		iFileTransferredSize = mWebBrowserSrv_sp->getCurFileTransferredSize();
		iRet = snprintf(szCardTransferStatus, sizeof(szCardTransferStatus), "%08x%02x", (quint32)(iFileTransferredSize/1024), iFormatOrScanPercentage);
		if(0 >= iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		szCardTransferStatus[sizeof(szCardTransferStatus)-1] = '\0';
		sizeMediaStatus += 10;
		int iByteSize= strlen(szCardTransferStatus);

		for(int i = 0;i <iByteSize;i++)
		{
			szCardCurrentStatus[sizeCardCurrentStatus] = szCardTransferStatus[i];
			sizeCardCurrentStatus += 1;
		}

		szCardCurrentStatus[sizeCardCurrentStatus] =11;
		sizeCardCurrentStatus += 1;

		szCardCurrentStatus[sizeCardCurrentStatus] =0;
		sizeCardCurrentStatus += 1;

		szCardCurrentStatus[sizeCardCurrentStatus] =48;
		sizeCardCurrentStatus += 1;

		szCardCurrentStatus[sizeCardCurrentStatus] =48;
		sizeCardCurrentStatus += 1;

		szCardTransferStatus[sizeof(szCardTransferStatus) - 1] = '\0';

		//base64 encode
		if(FALSE == bOnlyGetRawData)
		{
			char szBase64CardCurStatus[1024];
			INT_t iOutSize = sizeof(szBase64CardCurStatus);
			iRet = Base64Encode((PBYTE)szCardCurrentStatus, sizeCardCurrentStatus, (PBYTE)szBase64CardCurStatus, &iOutSize);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(iOutSize < (typeof(iOutSize))sizeof(szBase64CardCurStatus))
			{
				szBase64CardCurStatus[iOutSize] = '\0';
			}
			else
			{
				szBase64CardCurStatus[sizeof(szBase64CardCurStatus)-1] = '\0';
			}
			strCardCurStatus = szBase64CardCurStatus;
		}

		//get raw data
		if(pRawDataBuf)
		{
			if(sizeRawDataBuf < sizeCardCurrentStatus)
			{
				iOutRet = ERROR_NOT_ENOUGH_BUFFER;
				break;
			}
			memcpy(pRawDataBuf, szCardCurrentStatus, sizeCardCurrentStatus);
			if(pActualRawDataSize)
			{
				*pActualRawDataSize = sizeCardCurrentStatus;
			}
		}
	}while(FALSE);
}

void QWebBrowserApp::setFormatMedia(int iAction, int & iOutRet)
{
	INT_t iRet;
	StorageManage storageInfo;
	FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType = (typeof(eFmtMediaActionType))iAction;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setFormatMedia\n");
	}

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(FORMAT_MEDIA_ACTION_FORMAT != eFmtMediaActionType
			&& FORMAT_MEDIA_ACTION_REPAIR != eFmtMediaActionType)
		{
			Show_StatusMsgBox("", QString("Invalid Operation"), TRUE, 4*1000);
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(AMedia_TYPE_Unknown == m_eActiveMediaType
			|| AMedia_TYPE_TUNER == m_eActiveMediaType
    			|| AMedia_TYPE_URL == m_eActiveMediaType
    			|| AMedia_TYPE_Invalid == m_eActiveMediaType)
		{
			Show_StatusMsgBox("FormatMedia", QString("Invalid Active Media "), TRUE, 4*1000);
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(AMedia_TYPE_USB == m_eActiveMediaType)
		{
			iRet = strShellCmd.Format("%s USB Media",eFmtMediaActionType==FORMAT_MEDIA_ACTION_FORMAT?"Format":"Repair");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(AMedia_TYPE_SdCard == m_eActiveMediaType)
		{
			iRet = strShellCmd.Format("%s SD Card Media",eFmtMediaActionType==FORMAT_MEDIA_ACTION_FORMAT?"Format":"Repair");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(AMedia_TYPE_HDD == m_eActiveMediaType)
		{
			iRet = strShellCmd.Format("%s Hard Disk Media",eFmtMediaActionType==FORMAT_MEDIA_ACTION_FORMAT?"Format":"Repair");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		if(AMedia_TYPE_FLASH != m_eActiveMediaType)
		{
			Show_StatusMsgBox("FormatMedia", QString("Initializing(%1)...").arg(getActiveMediaDescFromType(m_eActiveMediaType)), TRUE);
			iRet = storageInfo.InitStorageMedia();
			if(ERROR_SUCCESS == iRet)
			{
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				Show_StatusMsgBox("FormatMedia", QString("Not Found Valid Partition"), TRUE, 4*1000);
				iOutRet = ERROR_NOT_FOUND;
				break;
			}
			else
			{
				iOutRet = iRet;
				Show_StatusMsgBox("FormatMedia", QString("Initialize(%1) err %1").arg(iRet), TRUE, 4*1000);
				break;
			}
		}
		if(FORMAT_MEDIA_ACTION_FORMAT == eFmtMediaActionType)
		{
			m_eFormatMediaStatus = FORMAT_MEDIA_InNow;
			Show_StatusMsgBox("FormatMedia", QString("Formatting(%1)...").arg(getActiveMediaDescFromType(m_eActiveMediaType)), TRUE);
			if(AMedia_TYPE_FLASH == m_eActiveMediaType)	//not need
			{
				iRet = strShellCmd.Format("rm -fr /MP7XX/LocalMedia/*");
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
					StopPlayback();
					iRet = system(strShellCmd);
				}

				iOutRet = iRet;
				Show_StatusMsgBox("FormatMedia",
					QString("(%1)Finished,%2(%3)").arg(getActiveMediaDescFromType(m_eActiveMediaType)).arg(0==iRet?"success":"fail").arg(iRet),
					TRUE, 4*1000);
				break;
			}
			else
			{
				if(FORMAT_MEDIA_ACTION_FORMAT == eFmtMediaActionType)
				m_eFormatMediaStatus = FORMAT_MEDIA_InNow;
				iRet = storageInfo.OprateStorageMedia((UINT_t)eFmtMediaActionType,(UINT_t)m_eActiveMediaType);
				m_eFormatMediaStatus = FORMAT_MEDIA_InNow;
				if(ERROR_SUCCESS == iRet)
				{
					m_eFormatMediaStatus = FORMAT_MEDIA_Success;;
					iOutRet = ERROR_SUCCESS;
				}
				else
				{
					m_eFormatMediaStatus =FORMAT_MEDIA_Error ;
					strShellCmd +=  " Failed !";//QString::number(iRet) +
					Show_StatusMsgBox("FormatMedia", QString(strShellCmd), TRUE, 2*1000);

					iOutRet = iRet;
				}
				Show_StatusMsgBox("FormatMedia",
					QString("(%1)Finished,%2(%3)").arg(getActiveMediaDescFromType(m_eActiveMediaType)).arg(0==iRet?"success":"fail").arg(iRet),
					TRUE, 4*1000);
			}
		}
		else if(FORMAT_MEDIA_ACTION_REPAIR == eFmtMediaActionType)
		{
			m_eFormatMediaStatus = REPAIR_MEDIA_InNow;
			Show_StatusMsgBox("Repair", QString("Start Repair Media"), TRUE, 2*1000);
			if(AMedia_TYPE_FLASH == m_eActiveMediaType)	//not need
			{
				Show_StatusMsgBox("Repair", QString("Repair Flash Nothing to do !"), TRUE, 2*1000);
				break;
			}
			else
			{
				iRet = storageInfo.OprateStorageMedia((UINT_t)eFmtMediaActionType,(UINT_t)m_eActiveMediaType);
				if(ERROR_SUCCESS == iRet)
				{
					m_eFormatMediaStatus = FORMAT_MEDIA_Success;;
					strShellCmd += " Success !";
					Show_StatusMsgBox("Repair", (LPCSTR)strShellCmd, TRUE, 2*1000);
					iOutRet = ERROR_SUCCESS;
					break;
				}
				else
				{
					m_eFormatMediaStatus =FORMAT_MEDIA_Error ;
					strShellCmd += " Failed !";
					Show_StatusMsgBox("Repair", (LPCSTR)strShellCmd, TRUE, 2*1000);

					iOutRet = iRet;
					break;
				}
			}
		}
	}while(FALSE);
	sync();

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setFormatMedia,ret=%d\n", iOutRet);
	}
}

void QWebBrowserApp::setFormatMediaAsync(int iAction, int & iOutRet)
{
	INT_t iRet;
	QSharedPointer <QMsg_FormatMedia> Msg_FmtMedia_sp(new QMsg_FormatMedia);
	FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType = (typeof(eFmtMediaActionType))iAction;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(Msg_FmtMedia_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(FORMAT_MEDIA_ACTION_FORMAT != eFmtMediaActionType
			&& FORMAT_MEDIA_ACTION_REPAIR != eFmtMediaActionType)
		{
			Show_StatusMsgBox("", QString("Invalid Operation"), TRUE, 2*1000);
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus
			|| REPAIR_MEDIA_InNow == m_eFormatMediaStatus )
		{
			iOutRet = ERR_BUSY;
			break;
		}

		BOOL_t bEnableWriteLoggin = TRUE;
		iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			bEnableWriteLoggin = false;
		}
		if(bEnableWriteLoggin)
		{
			char strTemp[64];
			snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_Format);
			strTemp[sizeof(strTemp)-1] = '\0';
			CString::STD_STRING_LIST strList ;
			strList.push_back(strTemp);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
			sprintf(strTemp,"%d",iAction);
			strList.push_back(strTemp);
			if(ERROR_SUCCESS !=WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				strList.clear();
			}
		}

		iRet = Msg_FmtMedia_sp->setMsgId(ControlCenter::AsyncMsgId_FormatMedia);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = Msg_FmtMedia_sp->setParameters(eFmtMediaActionType, m_eActiveMediaType);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		QString strTitle = "FormatMedia";
		if(FORMAT_MEDIA_ACTION_REPAIR == eFmtMediaActionType)
		{
			m_eFormatMediaStatus = REPAIR_MEDIA_InNow;
			strTitle = "RepairMedia";
		}
		else if(FORMAT_MEDIA_ACTION_FORMAT == eFmtMediaActionType)
		{
			m_eFormatMediaStatus = FORMAT_MEDIA_InNow;
		}
		Show_StatusMsgBox(strTitle, QString("Dev:%1,processing...").arg(getActiveMediaDescFromType(m_eActiveMediaType)), TRUE,2000);
		emit PostAsyncTransaction(Msg_FmtMedia_sp);
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setFormatMediaAsync,Act=%d,ret=%d\n", iAction, iOutRet);
	}
}

void QWebBrowserApp::Show_StatusMsgBox(const QString & strTitle, const QString & strContent, BOOL_t bPaintImmediately/* = FALSE*/, INT_t iTimeoutMs/* = -1*/)
{
	INT_t iRet;
	do
	{
		if(m_StatusMsgBox_sp.isNull())
		{
			m_StatusMsgBox_sp = QSharedPointer <QStatusMsgBox> (new QStatusMsgBox(QMessageBox::Information,
				"", "", QMessageBox::NoButton, NULL, Qt::WindowStaysOnTopHint));
			if(m_StatusMsgBox_sp.isNull())
			{
				break;
			}
		}
		iRet = m_StatusMsgBox_sp->Show(strTitle, strContent, iTimeoutMs);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(bPaintImmediately)
		{
			m_StatusMsgBox_sp->repaint();
		}
	}while(FALSE);
}

int QWebBrowserApp::parsePlayMediaCommad(const QString & strPlayFilePath,int  & iOutRet,int iCommadType)
{
	iOutRet = ERROR_SUCCESS;
	ActiveMedia_TYPE eActiveMediaType = AMedia_TYPE_Unknown;
	QString strDeviceType, strFileType, strFilename,strPlayFileUrl;
	int iLoopTimes = 1, iDurationTime = 0, iPositionTime = 0;
	QString strOriginUrl = strPlayFilePath;
	QStringList tempContorlList;
	bool bIsConverSuccess = true;
	int iParaValue;

	do
	{
#if 1/*added by lxj 2012-8-2 for player features enabled check*/
		iOutRet=CheckPlayEnable();
		if(iOutRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}
#endif

#if 1 /*---------add by xuweiwei 2013-11-22 To Make 92 video mode always full screen--------- */ 
		if(MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iVideoOutMode)
		{
			UpdateVideoPosition(true);
		}
		else if( MediaPlayer::VO_MODE_SPLIT_HdmiMain_1080p60_VgaVcr == m_iOldVideoOutMode)
		{
			UpdateVideoPosition(false);
		}
#endif
		if(strPlayFilePath.trimmed() == "")
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		QStringList  strTempList;
		strTempList = strPlayFilePath.split("?");
		if(strTempList.size() < 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		getFileInfoByStr(strTempList[0],strDeviceType,strFileType,strFilename);
		eActiveMediaType = getDevTypeByStr(strDeviceType);
		
		#if 1 //added by abner 
		if(strTempList.size() > 1)
		{
			for(int iPars = 1; iPars < strTempList.size(); iPars++)
			{
				tempContorlList = strTempList[iPars].split("=");
				if(tempContorlList.size() != 2)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
				
				if(tempContorlList[1].trimmed () == "")
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
				else
				{
					iParaValue = tempContorlList[1].toInt(&bIsConverSuccess);
					if(!bIsConverSuccess)
					{
						iOutRet = ERROR_INVALID_PARAMETER;
						break;
					}
					else
					{
						if(iParaValue < 0)
						{
							iOutRet = ERROR_INVALID_PARAMETER;
							break;
						}
					}
				}
				
				if(0 == tempContorlList[0].compare("loop", Qt::CaseInsensitive))
				{
					iLoopTimes = iParaValue;
				}
				else if(0 == tempContorlList[0].compare("duration", Qt::CaseInsensitive))
				{
					iDurationTime = iParaValue;
				}
				else if(0 == tempContorlList[0].compare("startpoint", Qt::CaseInsensitive))
				{
					iPositionTime = iParaValue;
				}
			}
		}

		/*LOG_BLINE("iLoopTimes = %d,i DurationTime = %d, position = %d, strFilename = %s,iCommadType:%d\n",iLoopTimes,iDurationTime,iPositionTime,qPrintable(strFilename),iCommadType);*/

#endif

		if(PlayMedia_MODE == iCommadType)
		{
			playMedia((CONST ActiveMedia_TYPE)eActiveMediaType, strFileType, strFilename,iOutRet, iLoopTimes,iDurationTime, iPositionTime,strOriginUrl);
		}
		else if(playNextFile_MODE == iCommadType)
		{
			//playNextFile((CONST ActiveMedia_TYPE)eActiveMediaType, strFileType, strFilename,iOutRet, iLoopTimes,iDurationTime,iPositionTime);
			playMedia((CONST ActiveMedia_TYPE)eActiveMediaType, strFileType, strFilename,iOutRet, iLoopTimes,iDurationTime, iPositionTime,strOriginUrl);
		}
		else if(playVideoLayer_MODE == iCommadType)
		{
			playVideoLayer((CONST ActiveMedia_TYPE)eActiveMediaType, strFileType, strFilename,iOutRet, iLoopTimes,iDurationTime,iPositionTime, strOriginUrl,true);
		}
		else if(playHtmlLayer_MODE == iCommadType)
		{
			playHtmlLayer((CONST ActiveMedia_TYPE)eActiveMediaType, strFileType, strFilename,iOutRet, iLoopTimes,iDurationTime, iPositionTime,strOriginUrl,true);
		}

	}	while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("parsePlayMediaCommad, iOutRet=%d\n", iOutRet);
	}

	return iOutRet;
}

void QWebBrowserApp::getAudioOutputMode(int & iOutRet, int & iAudioOutputMode)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		iAudioOutputMode = m_eAudioOutputMode;
	}while(FALSE);
}

void QWebBrowserApp::setAudioOutputMode(int iAudioOutputMode, int & iOutRet)
{
	INT_t iRet;
	AUDIO_OUTPUT_MODE eAudioOutputMode = (typeof(eAudioOutputMode))iAudioOutputMode;

	iOutRet = ERROR_SUCCESS;
	do
	{
		if(eAudioOutputMode == m_eAudioOutputMode)	//no change
		{
			break;
		}
		if(AUD_OUT_MODE_Normal == eAudioOutputMode)
		{
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setHdmiAudioOutput(TRUE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setAnalogAudioMute(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			setPlayerMute(FALSE, OUT iRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(AUD_OUT_MODE_DisableHdmi == eAudioOutputMode)
		{
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setHdmiAudioOutput(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setAnalogAudioMute(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			setPlayerMute(FALSE, OUT iRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(AUD_OUT_MODE_DisableAll == eAudioOutputMode)
		{
			setPlayerMute(TRUE, OUT iRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(AUD_OUT_MODE_DisableAnalog == eAudioOutputMode)
		{
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setHdmiAudioOutput(TRUE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = m_MediaPlayerCtrl_sp->setAnalogAudioMute(TRUE);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			setPlayerMute(FALSE, OUT iRet);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		m_eAudioOutputMode = eAudioOutputMode;
	}while(FALSE);
	
	do
	{
		if(mp_Config.isNull())
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		if(ERROR_SUCCESS == iOutRet)
		{
			mp_Config->setConfigValue(INI_KEY_CurrAudioOutputMode,QString::number(eAudioOutputMode));
		}
	}while(FALSE);
}

void QWebBrowserApp::setAutoAnnounce(int iAutoAnnounceIntervalSec, int & iOutRet)
{
	INT_t iRet;
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(10 > m_iAutoAnnounceIntervalSec && 0 < m_iAutoAnnounceIntervalSec)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_iAutoAnnounceIntervalSec = iAutoAnnounceIntervalSec;
		if(0 >= m_iAutoAnnounceIntervalSec)
		{
			m_iAutoAnnounceIntervalSec = 0;
			m_timerAutoAnnounce.stop();
			break;
		}
		//save
		iRet = SysProp_setPersistent(SysProp_KEY_AutoAnnounceIntervalSec,
			QString::number(m_iAutoAnnounceIntervalSec).toLocal8Bit().constData());;
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//apply
		m_timerAutoAnnounce.setSingleShot(FALSE);
		m_timerAutoAnnounce.start(m_iAutoAnnounceIntervalSec*1000);
	}while(FALSE);
}

void QWebBrowserApp::getAutoAnnounce(int & iOutRet, int & iAutoAnnounceIntervalSec)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		iAutoAnnounceIntervalSec = m_iAutoAnnounceIntervalSec;
	}while(FALSE);
}

void QWebBrowserApp::getMediaStatus(int & iOutRet, QString & strMediaStatus)
{
	int iRet;
	char szMediaStatus[64];
	qint64 iFileTransferredSize = 0;
	int iFormatOrScanPercentage = 0;
	int sizeMediaStatus = 0;

	iOutRet = ERROR_SUCCESS;

	strMediaStatus.clear();

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("getMediaStatus\n");
	}

	do
	{
		iFileTransferredSize = mWebBrowserSrv_sp->getCurFileTransferredSize();
		iRet = snprintf(szMediaStatus, sizeof(szMediaStatus), "%08x%02x", (quint32)(iFileTransferredSize/1024), iFormatOrScanPercentage);
		if(0 >= iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		szMediaStatus[sizeof(szMediaStatus)-1] = '\0';
		sizeMediaStatus += 10;

		if(m_iStateType == State_Idle)
		{
			szMediaStatus[sizeMediaStatus] = 0;
		}

		if(AMedia_TYPE_USB == m_eActiveMediaType)
		{
			BOOL_t bUsbMounted = FALSE;
			iOutRet = getDevMountStatus_Usb(&bUsbMounted);
			if(ERROR_SUCCESS != iOutRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			if(FALSE == bUsbMounted)
			{
				szMediaStatus[sizeMediaStatus] = '2'; //Hard Drive Not Present
				break;
			}
		}
		else if(AMedia_TYPE_SdCard == m_eActiveMediaType)
		{
			BOOL_t bSdMounted = FALSE;
			iOutRet = getDevMountStatus_Sd(&bSdMounted);
			if(ERROR_SUCCESS != iOutRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			if(FALSE == bSdMounted)
			{
				szMediaStatus[sizeMediaStatus] = '2'; //Hard Drive Not Present
				break;
			}
		}
		else if(AMedia_TYPE_HDD == m_eActiveMediaType)
		{
			BOOL_t bHddMounted = FALSE;
			iOutRet = getDevMountStatus_Hdd(&bHddMounted);
			if(ERROR_SUCCESS != iOutRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			if(FALSE == bHddMounted)
			{
				szMediaStatus[sizeMediaStatus] = '2'; //Hard Drive Not Present
				break;
			}
		}

		if(FORMAT_MEDIA_Error == m_eFormatMediaStatus)
		{
			szMediaStatus[sizeMediaStatus] =  '6'; //Hard Drive Format Failed
			break;
		}
		else if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			szMediaStatus[sizeMediaStatus] = '7';  //Hard Drive Format in Progress
			break;
		}
		else if(REPAIR_MEDIA_InNow == m_eFormatMediaStatus)
		{
			szMediaStatus[sizeMediaStatus] = '8';  //Hard Drive Format in Progress
			break;
		}
		//Hard Drive Unchanged
		szMediaStatus[sizeMediaStatus] ='9';
		//Hard Drive On with File System Error
		szMediaStatus[sizeMediaStatus] ='A';
		//Hard Drive Running without Error
		szMediaStatus[sizeMediaStatus] ='B';
		//Hard Drive Setup in Progress
		//szMediaStatus[sizeMediaStatus] ='C';
	}while(FALSE);

	sizeMediaStatus += 1;
	szMediaStatus[sizeMediaStatus] = '\0';
	szMediaStatus[sizeof(szMediaStatus)-1] = '\0';
	strMediaStatus = szMediaStatus;
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("ret %d\n", iOutRet);
	}
}

void QWebBrowserApp::getFileInfoByStr(const QString & strPlayFilePath,QString & getstrDataSrcDevice,QString &  getStrFileType,QString & getStrFileName)
{
	QString strDataSrcDevice, strFileType, strFilename,strPlayFileUrl;
	int iIndex;
	strPlayFileUrl = "";
	do
	{
		//first part
		iIndex = strPlayFilePath.indexOf(":");
		if(0 < iIndex)
		{
			strDataSrcDevice = strPlayFilePath.left(iIndex);
		}
		else if(0 == iIndex)
		{
			break;
		}
		else
		{
			strDataSrcDevice = strPlayFilePath;
		}
		if(strDataSrcDevice.isEmpty())
		{
			break;
		}
		//second part
		if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_USB, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_IDE, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive) )
		{
			if(0 < iIndex)
			{
				int iColonIndex;
				iColonIndex = strPlayFilePath.indexOf("://", iIndex+2);
				if(0 < iColonIndex)
				{
					strFileType = strPlayFilePath.mid(iIndex+2, iColonIndex - (iIndex + 2));
					//LOG_BLINE("%s\n", qPrintable(strFileType));
					if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive)
					    )
					{
						//the third part
						strFilename = strPlayFilePath.mid(iColonIndex+3);
						//LOG_BLINE("strFilename=%s\n", qPrintable(strFilename));
						if(FALSE == strFilename.isEmpty())
						{
							QString strFileFullPath;
							strFileFullPath = getFileFullPathByCeLabsUrl(strDataSrcDevice, strFileType, strFilename);
							//LOG_BLINE("strFileFullPath=%s\n", qPrintable(strFileFullPath));
							if(!QFile::exists(strFileFullPath))
							{
								break;
							}
							strPlayFileUrl = strFileFullPath ;//strFilename = strPlayFileUrl;
						}
					}
				}
			}
		}
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_UDP, Qt::CaseInsensitive) )
		{
			strFilename = strPlayFilePath;
			strDataSrcDevice = CeLabs_DATA_SRC_DEV_Url;
			strFileType = FileType_Udp;
		}
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_RTP, Qt::CaseInsensitive))
		{
			strFilename = strPlayFilePath;
			strDataSrcDevice = CeLabs_DATA_SRC_DEV_Url;
			strFileType = FileType_Udp;
		}
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_TUNER, Qt::CaseInsensitive))
		{
			strFilename = strPlayFilePath;
			strDataSrcDevice = CeLabs_DATA_SRC_DEV_Url;
			strFileType = FileType_Tuner;
		}
		else  if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_WEBPAGE, Qt::CaseInsensitive))
		{
			strFilename = strPlayFilePath;
			strDataSrcDevice = CeLabs_DATA_SRC_DEV_Url;
			strFileType = FileType_Http;
		}
#if 1/*added by lxj 2012-9-11*/
		else if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_CAPTURE, Qt::CaseInsensitive))
		{
			strFilename = strPlayFilePath;
			int i;
			i = strPlayFilePath.indexOf("://");
			if(i > 0){
				strFileType = strPlayFilePath.mid(i+3, i - 3);
			}
		}
#endif
		getstrDataSrcDevice = strDataSrcDevice;
		getStrFileType = strFileType;
		getStrFileName = strFilename ;
	}while(FALSE);
}

ActiveMedia_TYPE QWebBrowserApp::getDevTypeByStr(QString strActiveMedia)
{
	ActiveMedia_TYPE eActiveMediaType = AMedia_TYPE_Unknown;
	int iOutRet = ERROR_SUCCESS;

	do
	{
		if(strActiveMedia.compare(ActiveMedia_Desc_FLASH, Qt::CaseInsensitive) == 0 )
		{
			eActiveMediaType = AMedia_TYPE_FLASH;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_SdCard, Qt::CaseInsensitive) == 0 ||
			0 == strActiveMedia.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive))
		{
			eActiveMediaType = AMedia_TYPE_SdCard;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_USB, Qt::CaseInsensitive)== 0)
		{
			eActiveMediaType = AMedia_TYPE_USB;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_HDD, Qt::CaseInsensitive) == 0 ||
			0 == strActiveMedia.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive))
		{
			eActiveMediaType = AMedia_TYPE_HDD;
		}
		else if(strActiveMedia.compare(Media_Desc_URL, Qt::CaseInsensitive)== 0)
		{
			eActiveMediaType = AMedia_TYPE_URL;
		}
#if 1/*added by lxj 2012-9-11*/
		else if(strActiveMedia.compare(ActiveMedia_Desc_Capture, Qt::CaseInsensitive)== 0)
		{
			eActiveMediaType = AMedia_TYPE_CAPTURE;
		}
#endif
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	}while(FALSE);

	return eActiveMediaType;
}

int QWebBrowserApp::formatFlashLocalMedia(const QString strDirPath)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		QDir directory(strDirPath);
		if (!directory.exists())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

		QList<QString>::iterator itfileInfo = files.begin();
		for (; itfileInfo!= files.end(); ++itfileInfo)
		{
			QString filePath = QDir::fromNativeSeparators(directory.path() + '/' + (*itfileInfo));
			QFileInfo fi(filePath);
			if (fi.isFile() || fi.isSymLink())
			{
				QFile::setPermissions(filePath, QFile::WriteOwner);
				if (!QFile::remove(filePath))
				{
					iOutRet = ERROR_INVALID_STATE;
					break;
				}
			}
			else if (fi.isDir())
			{
				if (!formatFlashLocalMedia(filePath))
				{
					iOutRet = ERROR_INVALID_STATE;
				}
			}
		}

		if(LOCAL_STORAGE_DIR_NAME !=directory.dirName())
		{
			if(!directory.rmdir(QDir::fromNativeSeparators(directory.path())))
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
		}
	}while(FALSE);

	QDir dirLocalMedia (APP_DIR_PATH + LOCAL_STORAGE_DIR_NAME);
	if(!dirLocalMedia.exists())
	{
		if(dirLocalMedia.mkdir(strDirPath))
		{
			iOutRet = ERROR_SUCCESS ;
		}
	}
	else
	{
		QStringList filelist = dirLocalMedia.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
		if(filelist.isEmpty())
		{
			iOutRet = ERROR_SUCCESS ;
		}
	}
	return iOutRet;

}

bool QWebBrowserApp::parseUpdateXml(QString strXmlUrl,QString & strUpgradeUrl,QString & strVersion)
{
	QXmlDocument tempXmlDoc;
	int iRet,iOutRet;

	do
	{
		if(NULL == strXmlUrl)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			return false;
		}
		iRet = tempXmlDoc.OpenXmlFile(strXmlUrl);//"/MP7XX/Update3.xml");
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			return false;
			break;
		}

		QDomElement ElementList = tempXmlDoc.documentElement();
		if(ElementList.isNull())
		{
			LOG_BLINE("No root element\n");
			return false;
			break;
		}
		QDomElement Element_Package = ElementList.firstChildElement("Package");
		if(Element_Package.isNull())
		{
			LOG_BLINE("No root.Package element\n");
			return false;
			break;
		}
		strVersion = Element_Package.attribute("Version");
		strUpgradeUrl = Element_Package.attribute("Url");
	}while(FALSE);

	return true;
}

void QWebBrowserApp::setSystemUpgradeAddress(const QString & strUrl,int & iOutRet)
{
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setSystemUpgradeAddress,Url=%s\n", qPrintable(strUrl));
	}
//	LOG_LINE("setSystemUpgradeAddress,Url=%s\n", qPrintable(strUrl));
	iOutRet = ERROR_SUCCESS;
	iRet = ERROR_SUCCESS;
	
	do
	{
		if(SystemUpgrade_Idle != m_eCurUpgradeStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(mWebBrowserSrv_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		if(strUrl.trimmed() =="")
		{
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadAddress);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadAddress);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_SystemUpgradeUrl,QString(strUrl));
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
	//	LOG_LINE("setSystemUpgradeAddress,Url=%s\n", qPrintable(strUrl));
		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("mount -o rw,remount /");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}
		}
	
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "DownloadFile2");
		iRet = CreateDirectory2("/Temp", (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgArgsList << strUrl << NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareConnectionError);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			else
			{
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_DownloadInProgress);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(1);
			m_NewSysVerCfg_DownloadId = Reply_Arg.toUInt();
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_DownloadInProgress);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		if(m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
		}
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			iRet = m_UpgradeStatusBarItem_sp->setText("Upgrade:downloading Update.xml");
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setSystemUpgradeAddress,Ret=%d\n", iOutRet);
	}
}

INT_t QWebBrowserApp::OnNewSysVersionCfgDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strUpgradeUrl, strVersion;
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 != iDownloadErrNo || false == QFile::exists(NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH))
		{
			iOutRet = ERR_DOWNLOAD_FAIL;
			//Show_StatusMsgBox("Donwload err", QString("Failed to download %1,err %2").arg(strSrcUrl).arg(iDownloadErrNo),
			//	true, 4*1000);
			if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
			{
				m_UpgradeStatusBarItem_sp->setText(
					"Upgrade:" + QString("failed to download %1,err %2").arg(strSrcUrl).arg(iDownloadErrNo), 5*1000);
			}

			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareConnectionError);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText(
				"Upgrade:" + QString("successful to download %1").arg(strSrcUrl), 5*1000);
		}
		if(false == parseUpdateXml(NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH, strUpgradeUrl, strVersion) ||
			strUpgradeUrl.isEmpty() || strVersion.isEmpty())
		{
			iOutRet = ERR_INVALID_FILE_DATA;
			Show_StatusMsgBox("Donwload err", QString("%1 has invalid data,DownloadErr %2").arg(strSrcUrl).arg(iDownloadErrNo),
				true, 4*1000);
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
		{
			if(mp_AppInfo_Config.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			//add
			QString strFirmWareVersion = mp_AppInfo_Config ->getConfigValue("VersionInfo/FirmVersion").trimmed();
			if(0 >= CompareVersion(strVersion, strFirmWareVersion))
			{
				DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
				iRet = strShellCmd.Format("rm -fr " NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;//	break;
				}
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				}
				if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
				{
					m_UpgradeStatusBarItem_sp->setText(
						"Upgrade:" + QString("no newer version(current:%1,update:%2)").arg(strFirmWareVersion).arg(strVersion), 5*1000);
						m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
				}
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_DownloadCanceled);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				break;
			}

			if(strUpgradeUrl.left(1) == "/")	//absolutely path
			{
				strUpgradeUrl = strUpgradeUrl;
			}
			else	//relative path
			{
				QStringList strUpgradeUrlList = strSrcUrl.split("/");
				if(0 > strUpgradeUrlList.size())
				{
					iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBurning1);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					break;
				}
				strUpgradeUrl = strSrcUrl.mid(0,(strSrcUrl.length() - strUpgradeUrlList[strUpgradeUrlList.size()-1].length())) + strUpgradeUrl;
			}
		}
		if(SystemUpgrade_Idle != m_eCurUpgradeStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		m_eCurUpgradeStatus = SystemUpgrade_Start;
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "DownloadFile2");
		iRet = CreateDirectory2("/Temp", (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareTooLarge);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
		qulonglong curActiveMediaFreeSpaceInByte = 0;
		iRet = getMediaFreeSpace(curActiveMediaFreeSpaceInByte);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
			{
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Err(%1),no enough disk space.").arg(iRet), 6*1000);
			}
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareTooLarge);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			break;
		}
#define SysVersionMaxSize (130 * 1024) //80M
		if(SysVersionMaxSize > (curActiveMediaFreeSpaceInByte / 1024 ))
		{
			m_strTempSysUpgradeUrl  =  FILE_PATH_NewSystemUpgradePackage_InRam;
		}
		else
		{
			QString strActiveStorageDir=APP_DIR_PATH + m_strActiveStorageDirName;
			if(0 != m_strActiveStorageDirName.compare("LocalMedia", Qt::CaseInsensitive))
			{
				strActiveStorageDir = MOUNTDIR + APP_DIR_PATH + m_strActiveStorageDirName;
			}
			
			QString strTempDirUrl = strActiveStorageDir + "/Temp" ;
			QDir dir(strTempDirUrl);
			if(!dir.exists())
			{
				if(!dir.mkdir(strTempDirUrl))
				{
					if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
					{
						m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Err(%1),no enough disk space.").arg(iRet), 6*1000);
					}
					m_eCurUpgradeStatus = SystemUpgrade_Idle;
					iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareTooLarge);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					break;
				}
			}
			m_strTempSysUpgradeUrl = strActiveStorageDir + NewSystemVersion_DOWNLOAD_TEMP_FILE_PATH;
		}
		/*
#define LocalMediaMaxSize (130 * 1024) //130M
		if(LocalMediaMaxSize > (curActiveMediaFreeSpaceInByte / 1024 ))
		{
			m_strTempSysUpgradeUrl  =  FILE_PATH_NewSystemUpgradePackage_InRam;
		}
		*/
		MsgArgsList << strUpgradeUrl << m_strTempSysUpgradeUrl;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				m_eCurUpgradeStatus =SystemUpgrade_DownloadFileError  ;
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iOutRet = iRet;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareFailed);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				break;
			}
			else
			{
				m_eCurUpgradeStatus = SystemUpgrade_DownloadFile;//if download ing  error ?
				if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
				{
					m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("downloading %1").arg(strUpgradeUrl));
				}
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareUploading);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(1);
			m_NewSystem_DownloadId = Reply_Arg.toUInt();
		}
		// delete update.xml
		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("rm -fr " NewSystemVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::OnNewSystemVersionDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	Q_UNUSED(iRet);
	iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_Download_Image_Successful);
	do
	{
		if(ERR_CONTENT_NOT_FOUND==iDownloadErrNo)
		{
			m_eCurUpgradeStatus = SystemUpgrade_DownloadFileError; //url: 地址，file not found
			if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
			{
				m_UpgradeStatusBarItem_sp->setText("Donwload err:" + QString("Failed to download url %1 , file not found").arg(strSrcUrl),
					5*1000);
			}
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("downloaded %1,ErrNo:%2").arg(strSrcUrl).arg(iDownloadErrNo),
				5*1000);
		}
		if(false == QFile::exists(m_strTempSysUpgradeUrl))
		{
			m_eCurUpgradeStatus = SystemUpgrade_DownloadFileError;
			if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
			{
				m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("Failed to download %1,err %2").arg(strSrcUrl).arg(iDownloadErrNo),
					5*1000);
			}
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			break;
		}
		QFileInfo NewSysCfgFileInfo(m_strTempSysUpgradeUrl);
		if(m_TarUpgradeProcess_sp.isNull())
		{
			m_TarUpgradeProcess_sp = QSharedPointer <QProcess> (new QProcess);
			if(m_TarUpgradeProcess_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				break;
			}

			bool bRet = true;
			bRet = connect(&(*m_TarUpgradeProcess_sp), SIGNAL(finished(int, QProcess::ExitStatus)),
				this, SLOT(TarUpgradeZipFinished(int, QProcess::ExitStatus)));

			//add by rocky
			bRet = connect(&(*m_TarUpgradeProcess_sp), SIGNAL(error(QProcess::ProcessError)),
				this, SLOT(TarUpgradeZipError(QProcess::ProcessError)));
			//end add

			if(false == bRet)
			{
				LOG_BLINE("ConnFinishedSig(Sntp) err\n");
			}
		}
		QDir UpgradeDir(NewSystemVersion_DECOMPRESSION_TEMP_DIR_PATH);
		if(!UpgradeDir.exists())
		{
			if(!UpgradeDir.mkdir(NewSystemVersion_DECOMPRESSION_TEMP_DIR_PATH))
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareOtherError);
				break;
			}
		}

		// delete SystemUpgrade
		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("rm -fr " NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_3, 1024);
			iRet = strShellCmd_3.Format("rm -fr  /SystemUpgrade");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd_3);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_3, iRet);
			}
		}		

		if(QProcess::NotRunning != m_TarUpgradeProcess_sp->state())
		{
			m_TarUpgradeProcess_sp->terminate();
			m_TarUpgradeProcess_sp->waitForFinished(5*1000);
		}
		m_TarUpgradeProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
		//start WD
		iRet = WD_SetParam(0, (100*1000)*2);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Set Watch dog noauto err:%d\n", iRet);
		}
		m_TarUpgradeProcess_sp->start(QString("tar -C /Temp/Updgrade  -zxf ") + m_strTempSysUpgradeUrl);
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBurning1);
			m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("uncompressing %1").arg(m_strTempSysUpgradeUrl));
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::CreateSckForAutoAnnounceIfNeed()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_sckAutoAnnounce_sp.isNull())
		{
			m_sckAutoAnnounce_sp = QSharedPointer <QUdpSocket> (new QUdpSocket(this));
			if(m_sckAutoAnnounce_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QWebBrowserApp::RecreateSckForAutoAnnounce()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_sckAutoAnnounce_sp = QSharedPointer <QUdpSocket> (new QUdpSocket(this));
		if(m_sckAutoAnnounce_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::getXappVersion(int & iOutRet, QString & strXappVersion)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		strXappVersion = m_strXappVersion;
	}while(FALSE);
}

void QWebBrowserApp::TarUpgradeZipFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	INT_t iRet,iOutRet;
	iOutRet =ERROR_SUCCESS;
	iRet = ERROR_SUCCESS;

	//WD, restore its default.
	iRet = WD_SetParam(1, (30*1000));
	if(ERROR_SUCCESS != iRet)
	{
		LOG_BLINE("Set Watch dog auto err:%d\n", iRet);
	}

	if(0 != exitCode || QProcess::NormalExit != exitStatus)
	{
		iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		{
			QString strCommand = "";
			strCommand = (QString)"rm -fr "  + m_strTempSysUpgradeUrl ;
			//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));		
			iRet = system(strCommand.toLocal8Bit().constData());
			if(-1 == iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				iOutRet = ERROR_FAILED;
				//break;
			}
			else if(0 != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", strCommand.toLocal8Bit().constData(), iRet);
				iOutRet = ERROR_FAILED;
				//break;
			}	
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_3, 1024);
			iRet = strShellCmd_3.Format("rm -fr  /SystemUpgrade");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd_3);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_3, iRet);
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_2, 1024);
			iRet = strShellCmd_2.Format("rm -fr " NewSysCfg_DOWNLOAD_TEMP_FILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//				break;
			}
			iRet = system(strShellCmd_2);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_2, iRet);
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("rm -fr " NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//				break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}

		}

		m_eCurUpgradeStatus = SystemUpgrade_Fail;
		m_eCurUpgradeStatus = SystemUpgrade_Idle;
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText(
				"Upgrade:" + QString("failed to uncompress %1,ExitCode:%2").arg(NewSystemVersion_DOWNLOAD_TEMP_FILE_PATH).arg(exitCode),
				5*1000);
		}
		iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareOtherError);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	else	//success
	{
		iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBurning2);
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("successful to uncompress %1").arg(m_strTempSysUpgradeUrl),
				5*1000);
		}
		{
			QDir SysUpgradeFileDir(NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH);
			if(!SysUpgradeFileDir.exists())
			{
				m_UpgradeStatusBarItem_sp->setText(
					"Upgrade:" + QString("failed to upgrade: %1 not exists ").arg(NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				return ;
			}
			QDir UpgradeDir(NewSystemVersion_DECOMPRESSION_TEMP_DIR_PATH);
			QStringList dirInfoList = UpgradeDir.entryList(QDir::AllEntries);
			if(dirInfoList.size() > 3)
			{
				iRet = system("rm -rf /Temp/Upgrade");
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Upgrade package is incorrect. "),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				return ;
			}
			QFile upgradeInitFile( NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH "/upgrade-init.sh");
			if(!upgradeInitFile.exists())
			{
				iRet = system("rm -rf /Temp/Upgrade");
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Upgrade package is incorrect. "),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				return ;
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_2, 1024);
			iRet = strShellCmd_2.Format("rm -fr /SystemUpgrade");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd_2);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_2, iRet);
			}

			iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBurning3);
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("mv /Temp/Updgrade/SystemUpgrade/ /");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//				break;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareOtherError);
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				system("rm -rf /Temp/Upgrade");
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("The upgrading package is not correct."),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareOtherError);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				return ;
			}
		}

		INT_t iRet,iOutRet;
		iOutRet =ERROR_SUCCESS;
		iRet = ERROR_SUCCESS;
		QString strCommand = "";
		strCommand = (QString)"rm -fr "  + m_strTempSysUpgradeUrl ;
		//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
		iRet = system(strCommand.toLocal8Bit().constData());
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)(strCommand.toLocal8Bit().constData()), iRet);
		}
		iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareDone);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}

		QScopedPointer<QUpgradeRebootConfirmDlg> oUpgradeRebootConfirmDlg(new QUpgradeRebootConfirmDlg());
		int dlgRes = QMessageBox::Yes;

		if(FALSE == oUpgradeRebootConfirmDlg.isNull())
		{
			int dlgTimeoutConfirmMs = (10*1000);
			//start watch dog
			iRet = WD_SetParam(0, dlgTimeoutConfirmMs*1.5);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Set Watch dog unauto err:%d\n", iRet);
			}

			iRet = oUpgradeRebootConfirmDlg->setTimeoutConfirm(dlgTimeoutConfirmMs);
			if(ERR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			dlgRes = oUpgradeRebootConfirmDlg->exec();

			//WD, resotre its default.
			iRet = WD_SetParam(1, (30*1000));
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Set Watch dog auto err:%d\n", iRet);
			}
			//delete
			oUpgradeRebootConfirmDlg.reset();	
		}
		
		if(dlgRes == QMessageBox::Yes)
		{
			ReStart();
		}
		else if(dlgRes == QMessageBox::No)
		{
			// delete update.tgz
			INT_t iRet,iOutRet;
			iOutRet =ERROR_SUCCESS;
			iRet = ERROR_SUCCESS;
			QString strCommand = "";
			strCommand = (QString)"rm -fr "  + m_strTempSysUpgradeUrl ;
			//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
			iRet = system(strCommand.toLocal8Bit().constData());
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)(strCommand.toLocal8Bit().constData()), iRet);
			}
		}
		else
		{
			ReStart();
		}
		
		iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareDone);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		m_eCurUpgradeStatus = SystemUpgrade_Idle;
	}
}

void QWebBrowserApp::On_AsyncTransactionFinished(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	Q_UNUSED(MsgItem_sp);
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("On_AsyncTransactionFinished\n");
	}
	do
	{
		if(m_MainThreadId != QThread::currentThreadId ())
		{
			m_eFormatMediaStatus =FORMAT_MEDIA_Error ;
			LOG_BLINE("BUG,CurThrdId=%u,MainThreadId=%u\n", (quint32)QThread::currentThreadId(), (quint32)m_MainThreadId);
			break;
		}
		if(MsgItem_sp.isNull())
		{
			m_eFormatMediaStatus =FORMAT_MEDIA_Error;
			LOG_BLINE("BUG,MsgItem_sp is NULL\n");
			break;
		}
		if(ControlCenter::AsyncMsgId_FormatMedia == MsgItem_sp->getMsgId())
		{
			On_FormatMedia_Finished(MsgItem_sp);
			QTimer::singleShot( 4000, this, SLOT(BringBrowserToFront()) );	
		}
		else if(ControlCenter::AsyncMsgId_SnmpSet == MsgItem_sp->getMsgId())
		{
			onSnmpSetFinished(MsgItem_sp);
		}
		else if(ControlCenter::AsyncMsgId_SnmpGet == MsgItem_sp->getMsgId())
		{
			onSnmpGetFinished(MsgItem_sp);
		}
	}while(FALSE);
}

void QWebBrowserApp::On_FormatMedia_Finished(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	INT_t iRet;
	QSharedPointer <QMsg_FormatMedia> Msg_FmtMedia_sp = MsgItem_sp.dynamicCast<QMsg_FormatMedia>();
	ControlCenter::FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType;
	ActiveMedia_TYPE eActiveMediaType;
	QWidget * pBoxWindow=NULL;

	do
	{
		if(Msg_FmtMedia_sp.isNull())
		{
			m_eFormatMediaStatus =FORMAT_MEDIA_Error;
			break;
		}

		iRet = Msg_FmtMedia_sp->getParameters(OUT eFmtMediaActionType, OUT eActiveMediaType);
		if(ERROR_SUCCESS != iRet)
		{
			m_eFormatMediaStatus =FORMAT_MEDIA_Error;
			break;
		}
		QString strTitle = "FormatMedia";
		if(FORMAT_MEDIA_ACTION_REPAIR == eFmtMediaActionType)
		{
			strTitle = "RepairMedia";
		}	
		if(ERROR_SUCCESS == Msg_FmtMedia_sp->getOutRet())
		{
			Show_StatusMsgBox(strTitle, QString("Dev:%1,success").arg(getActiveMediaDescFromType(eActiveMediaType)), TRUE, 4*1000);
			m_eFormatMediaStatus = FORMAT_MEDIA_Success;
			pBoxWindow=activeWindow();
			BringWindowToFront(pBoxWindow);			
		}
		else	//FAIL
		{
			QString strErrInfo;
			QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
			for(int iId = 0; iId < (typeof(iId))(ErrInfoList.getErrInfoListSize()); iId++)
			{
				strErrInfo += (QString("ErrNo:") + QString().setNum(ErrInfoList[iId]->m_iOutRet) + QString(",") + ErrInfoList[iId]->m_strErrDesc + "\n");
			}
			strErrInfo += QString("ErrNo:%1,FormatMedia failed.").arg(Msg_FmtMedia_sp->getOutRet()) + "\n";
			Show_StatusMsgBox(strTitle, strErrInfo, TRUE, 4*1000);
			m_eFormatMediaStatus =FORMAT_MEDIA_Error;
			pBoxWindow=activeWindow();
			BringWindowToFront(pBoxWindow);	
		}
	}while(FALSE);
}

void QWebBrowserApp::onSnmpSetFinished(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	QSharedPointer <QMsg_SnmpSet> Msg_SnmpSet_sp = MsgItem_sp.dynamicCast<QMsg_SnmpSet>();

	do
	{
		if(Msg_SnmpSet_sp.isNull())
		{
			break;
		}		
	}while(false);

	if(Msg_SnmpSet_sp->getTransactionId() == m_iCurSnmpOpTransationId)
	{
		if(m_bWaitingForSnmpResponse)
		{
			m_bMustWaitForSnmpResponse = false;
		}
	}
}

void QWebBrowserApp::onSnmpGetFinished(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	QSharedPointer <QMsg_SnmpGet> Msg_SnmpGet_sp = MsgItem_sp.dynamicCast<QMsg_SnmpGet>();

	do
	{
		if(Msg_SnmpGet_sp.isNull())
		{
			break;
		}		
	}while(false);

	if(Msg_SnmpGet_sp->getTransactionId() == m_iCurSnmpOpTransationId)
	{
		if(m_bWaitingForSnmpResponse)
		{
			m_bMustWaitForSnmpResponse = false;
		}
	}
}

INT_t QWebBrowserApp::CompareVersion(const QString & strVersionA, const QString & strVersionB)
{
	INT_t iOutRet = 0;	//equal

#define	VERSION_FIELD_COUNT		4

	do
	{
		QStringList VerFieldListA = strVersionA.split(".");
		QStringList VerFieldListB = strVersionB.split(".");
		int VerFieldArrayA[VERSION_FIELD_COUNT] = {0,}, VerFieldArrayB[VERSION_FIELD_COUNT] = {0,};
		int iId;
		for(iId=0; iId < qMin(VerFieldListA.size(), VERSION_FIELD_COUNT); iId++)
		{
			VerFieldArrayA[iId] = VerFieldListA[iId].toInt();
		}
		for(iId=0; iId < qMin(VerFieldListB.size(), VERSION_FIELD_COUNT); iId++)
		{
			VerFieldArrayB[iId] = VerFieldListB[iId].toInt();
		}
		for(iId=0; iId < VERSION_FIELD_COUNT; iId++)
		{
			if(VerFieldArrayA[iId] > VerFieldArrayB[iId])
			{
				iOutRet = 1;
				break;
			}
			else if(VerFieldArrayA[iId] < VerFieldArrayB[iId])
			{
				iOutRet = -1;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::setMaintainLog(int eCurrActiveMedia, int & iOutRet)
{
	int iRet;
	QString strActiveMedia,  strActiveMediaDirName,strCommand;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(0 > eCurrActiveMedia)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(!QFile::exists(EVENT_LOG_FILE_PATH))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if(eCurrActiveMedia == 0)
		{
			break;
		}
		getActiveMedia(strActiveMedia,strActiveMediaDirName);
		if(strActiveMedia.trimmed() == "" || strActiveMediaDirName.trimmed() =="")
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		strCommand = (QString)"cp -fa " + "\"" + EVENT_LOG_FILE_PATH + "\"" + " " + "\"" + APP_DIR_PATH + strActiveMediaDirName + "/" + "\"";
		//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
		iRet = system(strCommand.toLocal8Bit().constData());
		if(-1 == iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_FAILED;
			break;
		}
		else if(0 != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", strCommand.toLocal8Bit().constData(), iRet);
			iOutRet = ERROR_FAILED;
			break;
		}
		else
		{
			iRet = StopWriteEventLog();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			strCommand = (QString)"rm -rf  " + "\"" + EVENT_LOG_FILE_PATH  + "\"";
			//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
			iRet = system(strCommand.toLocal8Bit().constData());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				ReInitEventLog();
			}
		}
	}while(FALSE);
}


void QWebBrowserApp::setTransferLog(QString strLogUrl,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	int iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	do
	{
		if(strLogUrl.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(!QFile::exists(EVENT_LOG_FILE_PATH))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = StopWriteEventLog();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//Call into org.thtfit.WebBrowser.DataCommSrv.UploadFile
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "UploadFile2");
		MsgArgsList << EVENT_LOG_FILE_PATH << strLogUrl << "";
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			ReInitEventLog();
		}
	}while(FALSE);
}

void QWebBrowserApp::setUserOSDAddress (const QString & strUrl,int & iOutRet)
{
	INT_t iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(SystemUpgrade_Idle != m_eCurUpgradeStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(strUrl.trimmed() =="")
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_UserOSDAddressUrl,QString(strUrl));
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_SrvName_WebBrowser, DBUS_PATH_WebBrowser_DatCommSrv,
			DBUS_IF_WebBrowser_DataCommSrv, "DownloadFile2");
		iRet = CreateDirectory2("/Temp", (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		MsgArgsList << strUrl << NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(1);
			m_NewOsd_DownloadId = Reply_Arg.toUInt();
		}
		if(m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
		}
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			iRet = m_UpgradeStatusBarItem_sp->setText("Upgrade Osd: downloading GuiDef.tgz");
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		m_eCurUpgradeStatus = SystemUpgrade_DownloadFile;
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setUserOSDAddress,url=%s,iOutRet=%d\n", qPrintable(strUrl), iOutRet);
	}
}

void QWebBrowserApp::On_StartTransferSlot(QString strSrcUrl,QString strDestUrl,int iAction)
{
	int iRet ;
	iRet = ERROR_SUCCESS;
	do
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_TRANSFER,1);
		}

		BOOL_t bEnableWriteLoggin = TRUE;
		iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		if(bEnableWriteLoggin)
		{
			char strTemp[64];
			snprintf(strTemp, sizeof(strTemp), "%d",TYPE_LOG_Transfer);
			strTemp[sizeof(strTemp)-1] = '\0';
			CString::STD_STRING_LIST strList ;
			strList.push_back(strTemp);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
			sprintf(strTemp,"%d",iAction);
			strList.push_back(strTemp);
	 		strList.push_back(strSrcUrl.toLocal8Bit().constData());
			strList.push_back(strDestUrl.toLocal8Bit().constData() );
			if(ERROR_SUCCESS !=WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}
			else
			{
				strList.clear();
			}
		}
	}while(FALSE);
}

INT_t QWebBrowserApp::OnNewOsdDownloadFinished(int iDownloadErrNo, const QString & strSrcUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	Q_UNUSED(iRet);
	do
	{
		if(ERR_CONTENT_NOT_FOUND==iDownloadErrNo)
		{
			m_eCurUpgradeStatus = SystemUpgrade_DownloadFileError; //url: 地址，file not found
			if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
			{
				m_UpgradeStatusBarItem_sp->setText("Donwload err:" + QString("Failed to download url %1 , file not found").arg(strSrcUrl),
					5*1000);
			}
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if(false == QFile::exists(NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH))
		{
			m_eCurUpgradeStatus = SystemUpgrade_DownloadFileError;
			m_UpgradeStatusBarItem_sp->setText("Donwload err:" + QString("Failed to download %1,ErrNo:%2").arg(strSrcUrl).arg(iDownloadErrNo),
				5*1000);
			m_eCurUpgradeStatus = SystemUpgrade_Idle;
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if(m_TarUpgradeOsdProcess_sp.isNull())
		{
			m_TarUpgradeOsdProcess_sp = QSharedPointer <QProcess> (new QProcess);
			if(m_TarUpgradeOsdProcess_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				break;
			}
			bool bRet = true;
			bRet = connect(&(*m_TarUpgradeOsdProcess_sp), SIGNAL(finished(int, QProcess::ExitStatus)),
				this, SLOT(TarUpgradeOsdFinished(int, QProcess::ExitStatus)));
           	if(false == bRet)
			{
				LOG_BLINE("ConnFinishedSig(Sntp) err\n");
			}
		}
		// delete SystemUpgrade
		{
			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("rm -fr " NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
			}
		}

		if(QProcess::NotRunning != m_TarUpgradeOsdProcess_sp->state())
		{
			m_TarUpgradeOsdProcess_sp->terminate();
			m_TarUpgradeOsdProcess_sp->waitForFinished(5*1000);
		}

		m_TarUpgradeOsdProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
		m_TarUpgradeOsdProcess_sp->start(QString("tar -C /Temp  -xf ") + NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH);
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("uncompressing %1").arg(m_strTempSysUpgradeUrl));
		}
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::TarUpgradeOsdFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	INT_t iRet,iOutRet;
	iOutRet =ERROR_SUCCESS;
	iRet = ERROR_SUCCESS;
	if(0 != exitCode || QProcess::NormalExit != exitStatus)
	{

		{
			QString strCommand = "";
			strCommand = (QString)"rm -fr "  + NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH ;
			LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
			iRet = system(strCommand.toLocal8Bit().constData());
			if(-1 == iRet)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				iOutRet = ERROR_FAILED;
				//break;
			}
			else if(0 != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", strCommand.toLocal8Bit().constData(), iRet);
				iOutRet = ERROR_FAILED;
				//break;
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_2, 1024);
			iRet = strShellCmd_2.Format("rm -fr " NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//				break;
			}
			iRet = system(strShellCmd_2);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_2, iRet);
			}
		}
		m_eCurUpgradeStatus = SystemUpgrade_Fail;
		m_eCurUpgradeStatus = SystemUpgrade_Idle;
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText(
				"Upgrade Osd:" + QString("failed to uncompress %1,ExitCode:%2").arg(NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH).arg(exitCode),
				5*1000);
		}
	}
	else	//success
	{
		if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
		{
			m_UpgradeStatusBarItem_sp->setText("Upgrade:" + QString("successful to uncompress %1").arg(NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH),
				5*1000);
		}

		{
			QDir SysUpgradeFileDir(NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH);
			if(!SysUpgradeFileDir.exists())
			{
				m_UpgradeStatusBarItem_sp->setText(
					"Upgrade:" + QString("failed to upgrade: %1 not exists ").arg(NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				return ;
			}
			QFile upgradeOsdFile(NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH "/osd.htm");
			if(!upgradeOsdFile.exists())
			{
				iRet = system("rm -rf /Temp/GuiDef");
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Upgrade package is incorrect. "),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				return ;
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd_2, 1024);
			iRet = strShellCmd_2.Format("rm -fr   /MP7XX/GuiDef");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//	break;
			}
			iRet = system(strShellCmd_2);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_2, iRet);
			}

			DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
			iRet = strShellCmd.Format("mv /Temp/GuiDef/ /MP7XX/");
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;//				break;
			}
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
				system("rm -rf /Temp/Upgrade");
				m_UpgradeStatusBarItem_sp->setText(	"Upgrade:" + QString("Upgrade package does not correct. "),5*1000);
				m_eCurUpgradeStatus = SystemUpgrade_Idle;
				return ;
			}
		}

		QString strCommand = "";
		strCommand = (QString)"rm -fr  "  + NewOsdVersion_DOWNLOAD_TEMP_UPDATEFILE_PATH ;
		//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
		iRet = system(strCommand.toLocal8Bit().constData());
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)(strCommand.toLocal8Bit().constData()), iRet);
		}

		QMessageBox msgBox;
		msgBox.setWindowTitle("Upgrade");
		msgBox.setText("New Osd version is ready.");
		msgBox.setInformativeText("Do you want to reboot for update now?");
		msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::Yes);
		 int ret =msgBox.exec();
		if(ret == QMessageBox::Yes)
		{
			ReStart();
		}
		else if(ret == QMessageBox::No)
		{
			// delete update.tgz
			INT_t iRet,iOutRet;
			iOutRet =ERROR_SUCCESS;
			iRet = ERROR_SUCCESS;
			QString strCommand = "";
			strCommand = (QString)"rm -fr  "  + NewOsdVersion_DECOMPRESSION_UPDATEFILE_PATH ;
			//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));
			iRet = system(strCommand.toLocal8Bit().constData());
			if(ERROR_SUCCESS != iRet)
			{
				LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)(strCommand.toLocal8Bit().constData()), iRet);
			}
		}
		m_eCurUpgradeStatus = SystemUpgrade_Idle;
	}

}

void QWebBrowserApp::setPlayTuner(QString strTunerUrl,int  & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	unsigned int PlaybackId = 0;
	iRet = ERROR_SUCCESS;

	do
	{
		if(strTunerUrl.trimmed() == "")
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet  = StopPlayback();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
		}
		
		StopAllPlayDurationTimer();
		strTunerUrl = "tuner://" + strTunerUrl ;
		if(!m_MediaPlayerCtrl_sp.isNull())
		{	
			iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			iRet = m_MediaPlayerCtrl_sp->PlayTuner(strTunerUrl, OUT PlaybackId);
		}
		
		if(ERROR_SUCCESS == iRet)	//success, break the loop
		{
			m_iStateType = State_PlayingTuner;			
			m_strCurPlayUrl = strTunerUrl;
			m_PlaybackId = PlaybackId;
			if(mp_Config.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			mp_Config->setConfigValue(INI_KEY_CurrPlayTunerUrl,strTunerUrl);
		
			if(m_bIsFirstPlayUrl)
			{
				m_bIsFirstPlayUrl = false;
			}
			m_eCurPlayMode = ControlCenter::PlayMode_Normal;

			BOOL_t bEnableWriteLoggin = TRUE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
			if(ERROR_SUCCESS != iRet)
			{
				break;
			}
			if(bEnableWriteLoggin)
			{
				char strTemp[64];
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp,"%d",TYPE_LOG_StartPlay);
				CString::STD_STRING_LIST strList ;
				strList.push_back(strTemp);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() );
				sprintf(strTemp,"%d",m_iStateType);
				QString strPlayType,strPlayFileInfo;
				getCurrentPlayEntryInfo(strPlayType,strPlayFileInfo);
				if(m_strCurrPlayListName.trimmed() == "")
				{
					strList.push_back(strPlayType.toLocal8Bit().constData());
					strList.push_back(strTunerUrl.toLocal8Bit().constData());
				}
				else
				{
					strList.push_back("PlayList");
					strList.push_back(m_strCurrPlayListUrl.toLocal8Bit().constData());
					QFileInfo fileInfo(strTunerUrl);
					strList.push_back(fileInfo.fileName().toLocal8Bit().constData());
				}
				if(ERROR_SUCCESS !=WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_CRT_ERRINFO;
				}
				else
				{
					strList.clear();
				}
			}
			break;
		}
		else if(ERROR_SUCCESS != iRet)	//any other errors
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);


	if(ERROR_SUCCESS == iRet)
	{
		m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY, 1);
	}
	else
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
		}
	}
} 

void QWebBrowserApp::getTunerChannelStrength(int & iOutRet, int & iStrength)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = m_MediaPlayerCtrl_sp->getTunerChannelStrength(&iStrength);
		}
		else
		{
			iOutRet = ERROR_INVALID_STATE;
		}
	}while(FALSE);

}


void QWebBrowserApp::setTunerScan(int iScanAction,int  & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		if(iScanAction <  Stop_TunerScan  || iScanAction > Reset_TunerScan)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		switch(iScanAction)
		{
			case Stop_TunerScan:	
			{
				iRet  = StopPlayback();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				m_PlaylistDataA.ClearPlayListData();
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				m_PlaybackId = 0 ;
				OnHideProgressBar(0x11);
				m_TunerScanStatus =TunerStauts_Idle;
				break;
			}			
			case Reset_TunerScan:
			{
				iRet  = StopPlayback();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				m_PlaylistDataA.ClearPlayListData();
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				m_PlaybackId = 0 ;
				iRet = m_MediaPlayerCtrl_sp->ResetDuration(0);
				if(ERROR_SUCCESS != iRet)	//any other errors
				{
					iOutRet = iRet;
					break;
				}			
				m_TunerScanStatus =TunerStauts_Idle;				
				break;
			}
			case Start_TunerScan:
			{	
				if(m_TunerScanStatus ==TunerScanning)
				{					
					break;
				}
				iRet  = StopPlayback();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				m_PlaylistDataA.ClearPlayListData();
				m_PlaybackId = 0 ;
				/*
				if(m_PlayPicAndHtmlWin->isActiveWindow())
				{
					if(!m_bIsPlayPluginList)
					{
						m_PlayPicAndHtmlWin->playHtmlFileEnd();
						UpdateVideoPosition();
					}
				}
				*/
				iRet = m_MediaPlayerCtrl_sp->TunerScan();
				if(ERROR_SUCCESS != iRet)	//any other errors
				{
					iOutRet = iRet;
					break;
				}
				OnShowProgressBarForScan(0x10);				
				m_TunerScanStatus =TunerScan_Start;
				break;
			}	
		}		
	}while(FALSE);
}


void QWebBrowserApp::setResetDuration (int iDurationTime,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		if(iDurationTime < 0 )
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet  = StopPlayback();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		m_PlaylistDataA.ClearPlayListData();
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
		/*
		if(m_PlayPicAndHtmlWin->isActiveWindow())
		{
			if(!m_bIsPlayPluginList)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				UpdateVideoPosition();
			}
		}
		*/
		iRet = m_MediaPlayerCtrl_sp->ResetDuration(iDurationTime);
		if(ERROR_SUCCESS != iRet)	//any other errors
		{
			iOutRet = iRet;
			break;
		}
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			iOutRet = iRet;
			break;
		}
		mp_Config->setConfigValue(INI_KEY_ResetDurationTime,QString::number(iDurationTime));

	}while(FALSE);
}

void QWebBrowserApp::setTunerMode(int iSetMode,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		iRet  = StopPlayback();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		m_PlaylistDataA.ClearPlayListData();
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
		/*
		if(m_PlayPicAndHtmlWin->isActiveWindow())
		{
			if(!m_bIsPlayPluginList)
			{
				m_PlayPicAndHtmlWin->playHtmlFileEnd();
				UpdateVideoPosition();
			}
		}
		*/
		iRet = m_MediaPlayerCtrl_sp->TunerMode(iSetMode);
		if(ERROR_SUCCESS != iRet)	//any other errors
		{
			iOutRet = iRet;
			break;
		}
		
	}while(FALSE);
}

void  QWebBrowserApp::setChannelUp(int iAction,int  & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		if(iAction <  0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		
		if( (pActiveWindow== &m_Browser) )
		{
			 iRet = ERROR_INVALID_STATE;
			break;
		}
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->ChannelUp();
		}
		if(ERROR_SUCCESS != iRet)	//any other errors
		{
			iOutRet = iRet;
			break;
		}
		if(ERROR_INVALID_STATE == iRet)
		{
			if(m_TunerStatusBarItem_sp.isNull())
			{
				m_TunerStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
			}
			if(FALSE == m_TunerStatusBarItem_sp.isNull())
			{
				iRet = m_TunerStatusBarItem_sp->setText("Warning,This controller only works on the Tuner is playing.",5*1000);
			}

			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
			}
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

void  QWebBrowserApp::setChannelDown(int iAction,int  & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		if(iAction < 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		
		if( (pActiveWindow== &m_Browser) )
		{
			 iRet = ERROR_INVALID_STATE;
			break;
		}
		m_PlayPicAndHtmlWin->playHtmlFileEnd();
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = m_MediaPlayerCtrl_sp->ChannelDown();
		}
		if(ERROR_SUCCESS != iRet)	//any other errors
		{
			iOutRet = iRet;
			break;
		}
		if(ERROR_INVALID_STATE == iRet)
		{
			if(m_TunerStatusBarItem_sp.isNull())
			{
				m_TunerStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
			}
			if(FALSE == m_TunerStatusBarItem_sp.isNull())
			{
				iRet = m_TunerStatusBarItem_sp->setText("Warning, This controller only works on the Tuner is playing.",5*1000);
			}
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
			}
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

void QWebBrowserApp::getResetDuration(int & iOutRet, int & iDurationTime)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			iOutRet = iRet;
			break;
		}
		iDurationTime = mp_Config->getConfigValue(INI_KEY_ResetDurationTime).toInt();
	}while(FALSE);
}

void QWebBrowserApp::getTransferControl(int & iOutRet, int & TransferStatusCode)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mWebBrowserSrv_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = mWebBrowserSrv_sp->getTransferControl(OUT TransferStatusCode);
	}while(FALSE);
}

int  QWebBrowserApp::tunerScan(int iMode)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		if(ATSC_MODE > iMode || iMode > QAM256_MODE )
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		//setPlayMode(iMode,  iOutRet);
		if(m_pScangressBar.isNull())
		{
			m_pScangressBar = QSharedPointer <QScanProgressBar> (new QScanProgressBar);
			if(m_pScangressBar.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
		}
		iOutRet = m_MediaPlayerCtrl_sp->TunerMode(iMode);
		if(ERROR_SUCCESS != iOutRet)	//any other errors
		{
			iOutRet = iOutRet;
			break;
		}
		setTunerScan(Start_TunerScan, iOutRet);
		if(iOutRet==ERROR_SUCCESS)
		{
			m_Browser.RefreshOsdUi();
		}
	}while(FALSE);
	return iOutRet;
}

void QWebBrowserApp::OnTimer_checkMenuOsdTimeOut()
{
	do
	{
		if(m_OsdTimer.isActive())
		{
			m_OsdTimer.stop();
		} 
		if(m_bIsPlayVideoLayer)
		{
			break;
		}
		ShowPlayerWindow();
		if(m_bIsPlayPluginList)
		{
			int VideoPosX = 0, VideoPosY = 0, VideoPosW = 0, VideoPosH = 0;
			int iRet = ERROR_SUCCESS;
			iRet = m_PlayPicAndHtmlWin->CalcVideoPluginPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
			if(ERROR_SUCCESS != iRet)
			{
				if(ERROR_NOT_FOUND == iRet)
				{
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				break;
			}
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				iRet = m_MediaPlayerCtrl_sp->setVideoPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
			}
		}
		else
		{
			UpdateVideoPosition(true);
		}
		break;
#if 0
		if(m_bIsPlayPluginList)
		{
			ShowPlayPicOrHtmlWindow();
			int VideoPosX = 0, VideoPosY = 0, VideoPosW = 0, VideoPosH = 0;
			int iRet = ERROR_SUCCESS;
			iRet = m_PlayPicAndHtmlWin->CalcVideoPluginPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
			if(ERROR_SUCCESS != iRet)
			{
				if(ERROR_NOT_FOUND == iRet)
				{
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				break;
			}
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				iRet = m_MediaPlayerCtrl_sp->setVideoPosition(VideoPosX, VideoPosY, VideoPosW, VideoPosH);
			}
			break;
		}
		if(m_PlayPicAndHtmlWin->isActiveWindow() )//|| m_HtmlDurationTimer.isActive()
		{			
			m_OsdTimer.start(MENUOSDTIMEOUT_MS);
			break;
		}	
		if(m_HtmlDurationTimer.isActive() || m_iStateType == State_PlayingBrowserFile)
		{			
			ShowPlayPicOrHtmlWindow();
			break;
		}		
		if(m_IsPlayHtmlStatus)
		{
			ShowPlayPicOrHtmlWindow();
			break;
		}
		ShowPlayerWindow();
		UpdateVideoPosition(true);
#endif

	}while(FALSE);
}

void QWebBrowserApp::OnNotifyScanProgress(int iPgmNum,int iProgress)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		if(TunerStauts_Idle == m_TunerScanStatus)
		{
			break;
		}
		if(m_pScangressBar.isNull())
		{
			m_pScangressBar = QSharedPointer <QScanProgressBar> (new QScanProgressBar);
			if(m_pScangressBar.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
		}
		
		m_TunerScanStatus =TunerScanning;
		m_pScangressBar->setValue(iProgress,iPgmNum);
		m_pScangressBar->show();
		m_pScangressBar->raise();
		
#define SCAN_PROGRESS 100
		if(iProgress == SCAN_PROGRESS)
		{
			m_TunerScanStatus =TunerStauts_Idle;
			m_Browser.RefreshOsdUi();		
			if(iPgmNum > 0)
			{
				int iRet =ERROR_SUCCESS;
				int imajorNo,iminorNo;
				getFirstChNumber(iRet,imajorNo,iminorNo);
				if(iRet == ERROR_SUCCESS)
				{
					if(imajorNo > 0 && iminorNo > 0)
					{
						QString strChannle = QString::number(imajorNo) + QString("-")+QString::number(iminorNo);
						setPlayTuner(strChannle,iOutRet);	
						if(iOutRet == ERROR_SUCCESS)
						{
							m_iTunerScanStatus = TUNER_HAS_SCAN;	
						}
					}
				}
			}
		}

	}while(FALSE);
}

void QWebBrowserApp::OnShowProgressBarForScan(int iType)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
#define DUBS_TYPE_SHOW  0x10
		if(iType != DUBS_TYPE_SHOW)
		{
			break;
		}
		if(m_pScangressBar.isNull())
		{
			m_pScangressBar = QSharedPointer <QScanProgressBar> (new QScanProgressBar);
			if(m_pScangressBar.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
		}
		m_pScangressBar->setValue(0,0);
		m_TunerScanStatus =TunerScan_Start;
		m_pScangressBar->show();
	}while(FALSE);
}


void QWebBrowserApp::OnHideProgressBar(int iType)
{
	int iOutRet = ERROR_SUCCESS;
	
#define DUBS_TYPE_HIDE  0x11

	do
	{
		if(iType != DUBS_TYPE_HIDE)
		{
			break;
		}
		if(m_pScangressBar.isNull())
		{
			m_pScangressBar = QSharedPointer <QScanProgressBar> (new QScanProgressBar);
			if(m_pScangressBar.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
		}
		m_pScangressBar->setValue(0,0);
		m_pScangressBar->hide();
		m_TunerScanStatus =TunerScan_Complete;
		m_TunerScanStatus =TunerStauts_Idle;
		m_Browser.RefreshOsdUi();
		ShowBrowserWindow();
	}while(FALSE);
}

#if 1/*added by lxj 2012-7-31 for features enabled en/decrypt*/
QString QWebBrowserApp::GetMacAddress(void)
{
	return m_strMacAddress;
}

void QWebBrowserApp::setDeviceKey(QString strFromEnabledFeatures,int  & iOutRet)
{
	int iEnabledFeatures=0;
	
	QString strScrambleMac="";
	QString strKey2="";
	QString strDeviceKeySource="";
	QString strEnabledFeatures="";

	iOutRet = ERROR_SUCCESS;
	
	//LOG_BLINE("setDeviceKey(),strEnabledFeatures=%s\n",qPrintable(strEnabledFeatures));

	strScrambleMac = ScrambleMac(GetMacAddress());
	strKey2 = StrEncrypt(ENCRYPT_FIRST_KEY,strScrambleMac);
	strDeviceKeySource = StrDecrypt(strFromEnabledFeatures,strKey2);

	/*match,this copy from tsi_agent_new/Tnetapp.c in read_device_key func*/
	if(strDeviceKeySource.mid(0,3) == "LIC" )
	{
		int i;
		for(i=0; i< MAX_ENABLED_FEATURES_LENGTH; i++)
		{
			QChar c;
			c = strDeviceKeySource.at(i+3);
			if( c.toLatin1() && c.isDigit() )/*the char must is '0-9'*/
			{
				strEnabledFeatures += c;
			}else{
				break;
			}
		}

		if(strDeviceKeySource.mid(i+3,4) == "ENSE" )
		{
			bool ok;
			iEnabledFeatures = strEnabledFeatures.toInt(&ok,10);/*convert to decimal*/
			if(ok)
			{
				iOutRet = m_Mp7xxGlobalStatusData.setEnabledFeatures(iEnabledFeatures);
				if(iOutRet == ERROR_SUCCESS )
				{
					/*save to config file*/
					mp_Config->setConfigValue(INI_KEY_DEVICE_KEY,strEnabledFeatures);
				}else{
					goto invalid_key;
				}
			}else{
				goto invalid_key;
			}
		}else{
			goto invalid_key;
		}
	}else
	{
invalid_key:
		iOutRet = ERR_INVALID_DEVICE_KEY;
		if(Sw_LogDbusCmd)
		{
			LOG_BLINE("Invalid DeviceKey\n");
		}
	}

#if 0/*for debug*/
	LOG_BLINE("strScrambleMac:%s,strKey2=%s,strSource=%s,iEnabledFeatures=%d(0x%x)\n",
		                     qPrintable(strScrambleMac),
		                     qPrintable(strKey2),
		                     qPrintable(strDeviceKeySource),
		                     iEnabledFeatures,
		                     iEnabledFeatures);
#endif
}

/*MAC format: 00:22:78:00:87:7A
   This function copy from decoder_mac.htm scrambleMac()
*/
QString QWebBrowserApp::ScrambleMac( QString mac )
{
	QString macHex[6];
	QString str1="",str2="",str3="";
	int iValue;
	bool ok;  

	macHex[0] = mac.mid(0,2);
	macHex[1] = mac.mid(3,2);
	macHex[2] = mac.mid(6,2);
	macHex[3] = mac.mid(9,2);
	macHex[4] = mac.mid(12,2);
	macHex[5] = mac.mid(15,2);

	iValue = (macHex[1].toInt(&ok,16)*2) & 0xFF;
	str1 = str1.sprintf("%x",iValue);
	if( str1.length() < 2 )
		str1 = "0" + str1;

	iValue = (macHex[3].toInt(&ok,16)*2) & 0xFF;
	str2 = str2.sprintf("%x",iValue);
	if( str2.length() < 2 )
		str2 = "0" + str2;

	iValue = (macHex[5].toInt(&ok,16)*2) & 0xFF;
	str3 = str3.sprintf("%x",iValue);
	if( str3.length() < 2 )
		str3 = "0" + str3;

	mac = macHex[0] +
		  str1 +
		  macHex[2] +
		  str2 +
		  macHex[4] +
		  str3;

	//LOG_LINE("Decode MAC:%s\n",qPrintable(mac));
	return mac;
}


/*This function copy from decoder_mac.htm DoE(1)
*/
QString QWebBrowserApp::StrEncrypt(QString source,QString key)
{
	int n=0,k=0,tmp=0;
	QString strRet="";

	//LOG_LINE("Source:%s,key:%s\n",qPrintable(source),qPrintable(key));

	for( n=0; n < source.length(); n++ )
	{
		if( k >= key.length()-1 )
			k = 0;
		else
			k++;

		if( source.at(n).toLatin1() > ENCRYPT_ASCII_HIGH || source.at(n).toLatin1() < ENCRYPT_ASCII_LOW )
		{
			strRet += source.at(n);
			continue;
		}

		tmp = source.at(n).toLatin1() + key.at(k).toLatin1();
		if( key.at(k).toLatin1() % 2 > 0 )
			tmp = tmp+51;

		while( tmp >= ENCRYPT_ASCII_HIGH ){
			tmp = tmp-ENCRYPT_ASCII_HIGH+ENCRYPT_ASCII_LOW;
		}

		while ( tmp < ENCRYPT_ASCII_LOW ){
			tmp = tmp+(ENCRYPT_ASCII_HIGH-ENCRYPT_ASCII_LOW);
		}

		strRet += tmp;
	}

	return strRet;
}

/*This function copy from decoder_mac.htm DoE(0)
*/
QString QWebBrowserApp::StrDecrypt(QString secret,QString key)
{
	int n=0,k=0,tmp=0;
	QString strRet="";

	//LOG_LINE("secret:%s,key:%s\n",qPrintable(secret),qPrintable(key));

	for( n=0; n < secret.length(); n++ )
	{
		if( k >= key.length()-1 )
			k = 0;
		else
			k++;

		if( secret.at(n).toLatin1() > ENCRYPT_ASCII_HIGH || secret.at(n).toLatin1() < ENCRYPT_ASCII_LOW )
		{
			strRet += secret.at(n);
			continue;
		}

		tmp = secret.at(n).toLatin1() - key.at(k).toLatin1();
		if( key.at(k).toLatin1() % 2 > 0 )
			tmp = tmp-51;

		while( tmp >= ENCRYPT_ASCII_HIGH ){
			tmp = tmp-ENCRYPT_ASCII_HIGH+ENCRYPT_ASCII_LOW;
		}

		while ( tmp < ENCRYPT_ASCII_LOW ){
			tmp = tmp+(ENCRYPT_ASCII_HIGH-ENCRYPT_ASCII_LOW);
		}

		strRet += tmp;
	}

	return strRet;
}
#endif

/*
Only sent to MediaPlayer
*/
int QWebBrowserApp::PlayMedia(const QString & strUrl, int iLoopCount/* = 1*/)
{
	int iOutRet = ERROR_SUCCESS;
	unsigned int PlaybackId = 0;
	
	do
	{
		if(strUrl.trimmed() == "")
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			return iOutRet;
		}
		if(iLoopCount == 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			return iOutRet;
		}

		if(QFile::exists(strUrl))
		{
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}		
			iOutRet = m_MediaPlayerCtrl_sp->Play(OUT strUrl, OUT PlaybackId,iLoopCount);
		}
#if 1/*added by lxj 2012-10-10 for play usb video from js call*/
		else if(strUrl.left(strlen(USB_CAPTURE_URL)) == USB_CAPTURE_URL){
			if(m_MediaPlayerCtrl_sp.isNull())
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			MediaServer_StopAudio();		
			iOutRet = m_MediaPlayerCtrl_sp->Play(OUT strUrl, OUT PlaybackId,iLoopCount);
		}
#endif
		else
		{
			QString strFullPath = "";		
			if("/" != strUrl[0])	// relative path
			{		
				QString  strActiveMedia,strActiveMediaDirName;
				
				getActiveMedia(strActiveMedia, strActiveMediaDirName);
				if(applicationDirPath() == "/")
				{
					strFullPath = applicationDirPath() +strActiveMediaDirName + "/"+  FOLDER_NAME_MUSIC +"/"+ strUrl;
				}
				else
				{
					strFullPath = applicationDirPath() +"/" + strActiveMediaDirName + "/"+  FOLDER_NAME_MUSIC +"/"+ strUrl;
				}							
			}
			if(QFile::exists(strFullPath))
			{
				if(m_MediaPlayerCtrl_sp.isNull())
				{
					iOutRet = ERROR_INVALID_STATE;
					break;
				}		
				iOutRet = m_MediaPlayerCtrl_sp->Play(OUT strFullPath, OUT PlaybackId,iLoopCount);
			}	
			else
			{
				//iOutRet = ERROR_INVALID_PARAMETER;
				iOutRet = m_MediaPlayerCtrl_sp->Play(OUT strUrl, OUT PlaybackId,iLoopCount);
			}
		}	
	}while(FALSE);

	return iOutRet;
}


/*
Only sent to MediaPlayer
*/
int QWebBrowserApp::StopMedia()
{
	int iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = stopMPlayerWithEventLog();
		if(iOutRet == ERROR_SUCCESS)
		{
			m_PlaybackId = 0;
		}	
		if(m_PlayPluginListEntryDurationTimer.isActive())
		{
			m_PlayPluginListEntryDurationTimer.stop();
		}
		if(m_PlayDurationTimer.isActive())
		{
			m_PlayDurationTimer.isActive();
		}

		if(m_PlaylistDurationTimer.isActive())
		{
			m_PlaylistDurationTimer.isActive();
		}
	}while(FALSE);

	return iOutRet;
}


int QWebBrowserApp::setVideoMode(const int eCeSnmpVidMode)
{
	int iOutRet = ERROR_SUCCESS, iRet;
	MediaPlayer::DISPLAY_ASPECT_RATIO eDispAspectRatio = MediaPlayer::DispAspectRatio_DEFAULT;
	QDBusMessage DbusMsgToSend;	
	QDBusMessage DbusMsgReply;	
	int TimeoutMs;	
	QList <QVariant> MsgArgsList;
	INT  value;
	UINT flag;
	
	do
	{
		eDispAspectRatio = getDispAspectRatio_From_SnmpVideoMode((ControlCenter::CE_SNMP_VIDEO_MODE_T)eCeSnmpVidMode);
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		value=(INT)eDispAspectRatio;
		flag=(UINT)DAR_ONLY_APPLY_ONCE_AT_NEXT_PLAY;
		
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_MPLAYER_SERVICE_NAME, DBUS_MPLAYER_OBJECT_PATH,
			DBUS_MPLAYER_OBJECT_INTERFACE, METHOD_setVideoDispAspectRatio);
		MsgArgsList << value << flag;	//takes effect on next playback
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setVideoMode(%d) ret %d\n", eCeSnmpVidMode, iOutRet);
	}
	
	return iOutRet;
}

#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
INT_t QWebBrowserApp::MediaServerPlayAudio(const QString & strUrl)
{
	INT_t iRet;

	//LOG_BLINE("QWebBrowserApp::MediaServerPlayAudio\n");

	do
	{
		iRet=CheckPlayEnable();
		if(iRet!=ERROR_SUCCESS)
		{
			//LOG_BLINE("No license to play\n");
			break;
		}

		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}

		iRet = m_MediaPlayerCtrl_sp->MediaServer_PlayAudio(strUrl);
		if(iRet != ERROR_SUCCESS)
		{
			/*nothing*/
		}
	}while(FALSE);

	return iRet;
}

INT_t QWebBrowserApp::MediaServerStopAudio(void)
{
	INT_t iRet;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}

		iRet = m_MediaPlayerCtrl_sp->MediaServer_StopAudio();
		if(iRet != ERROR_SUCCESS)
		{
			/*nothing*/
		}
	}while(FALSE);

	return iRet;
}

int QWebBrowserApp::MediaServer_PlayAudio(const QString & strUrl)
{
	return MediaServerPlayAudio(strUrl);
}

int QWebBrowserApp::MediaServer_StopAudio()
{
	return MediaServerStopAudio();
}

#endif

void QWebBrowserApp::getTunerScan(int & iOutRet, int & iScanStatus)
{
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		if(m_TunerScanStatus == TunerStauts_Idle)
		{
			iScanStatus = 0;
		}
		else
		{
			iScanStatus = 1;
		}
	}while(FALSE);
}

void QWebBrowserApp::getForceTxMode(int & iOutRet, int & iTxMode)
{
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		if(m_TunerScanStatus == TunerStauts_Idle)
		{
			iTxMode = 0;
		}
		else
		{
			iTxMode = 1;
		}
				
		iOutRet = m_MediaPlayerCtrl_sp->getForceTxMode(& iTxMode);
		
	}while(FALSE);
}


void QWebBrowserApp::getChannelTable( int & iOutRet, QStringList  & strInfoList)
{

	iOutRet = ERROR_SUCCESS;	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		int iEntryId, channelNumer,subChannelCount;
		QString  strProgramPids;
		iOutRet = m_MediaPlayerCtrl_sp->getChannelTable(&iEntryId,&channelNumer, &subChannelCount,strProgramPids,strInfoList);
	}while(FALSE);
}

void QWebBrowserApp::get_tnetVersion(int & iOutRet, qint32 & iNumber)
{
 	iOutRet = ERROR_SUCCESS;		
	do
	{
		if(mp_AppInfo_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		QString strBrowserVersion = mp_AppInfo_Config ->getConfigValue("VersionInfo/BrowserVersion");
		if(strBrowserVersion.trimmed() == "")
		{
			iNumber = 0;
			break;
		}
		QStringList netVersionList =  strBrowserVersion.trimmed().split(".");
			
		if(3 == netVersionList.size())
		{
		    iNumber  = ((netVersionList[0].toInt() & 0xff) << 24) | ((netVersionList[1].toInt() & 0xff) << 16) | ((netVersionList[2].toInt() & 0xff) << 8);
		}
		else if(2 == netVersionList.size())
		{
		    iNumber  = ((netVersionList[0].toInt() & 0xff) << 24) | ((netVersionList[1].toInt() & 0xff) << 16)  ;
		}
		else if(1 == netVersionList.size())
		{
			iNumber  = ((netVersionList[0].toInt() & 0xff) << 24);
		}		
	}while(FALSE);
}

void QWebBrowserApp::setSnmpPassFinishSlot(int iResult)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
	
		if(QDialog::Accepted == iResult)
		{
			if(m_PwdInputDlg_sp.isNull())
			{
				break;
			}
			QString text = m_PwdInputDlg_sp->textValue();
			if(QString("1234") == text.trimmed())
			{
				if(m_NetMgrClient_sp)
				{
					iOutRet = m_NetMgrClient_sp->ResetSnmpdV3Password();
				}
				if(iOutRet == ERROR_SUCCESS)
				{
					m_Browser.RefreshSnmpPassWordResult(1);
				}
			}
			else
			{
				m_Browser.RefreshSnmpPassWordResult(0);
			}
		}
		else if(QDialog::Rejected ==  iResult)
		{
			m_Browser.RefreshSnmpPassWordResult(0);
		}
		m_PwdInputDlg_sp->setTextValue("");
		m_PwdInputDlg_sp->hide();
		
#if 1	/*added by abner for show dialog on 2014-11-11*/
		m_Browser.raise();
		m_Browser.activateWindow();
#endif

	}while(FALSE);
}

int QWebBrowserApp::MPlayerPlugin_Play(QString strPlaylistUrl)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();
		
		if( (pActiveWindow== &m_Browser) )
		{
			iOutRet = ERROR_INVALID_STATE;
		}
		else if(pActiveWindow== &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml())
		{			
			if(m_playWin.getPlayHtmlOrPicWin().isNull())
			{
				if(Sw_LogDbusCmd)
				{
					LOG_BLINE("play html or picture window is null\n");
				}
				iOutRet = ERROR_INVALID_STATE;
				break;
			}		
			iOutRet = m_playWin.getPlayHtmlOrPicWin()->playPluginPlayList(strPlaylistUrl);			
		}		
	}while(FALSE);
	
	return iOutRet;
}

QStringList QWebBrowserApp::GetWirelessStatus()
{
	QStringList strResultList;
	int iOutRet = ERROR_SUCCESS;
	do
	{		
		int  iCurWifiStatus = -1;
		if(m_NetMgrClient_sp)
		{
			iOutRet = m_NetMgrClient_sp->getWifiStatus(iCurWifiStatus);
		}
		if(iOutRet != ERROR_SUCCESS)
		{
			break;
		}
		else
		{
			printf("[%s:%d]iCurWifiStatus = %d \n",__func__,__LINE__,iCurWifiStatus);
			strResultList << QString::number(iCurWifiStatus);
			if(iCurWifiStatus == Mp7xxCommon::WIFI_STATUS_NotInstalled ||
			  iCurWifiStatus == Mp7xxCommon::WIFI_STATUS_DISABLED)
			{
				break;
			}
		}			
		
		if(m_NetMgrClient_sp)
		{			
			QString strValue = "";
			iOutRet = m_NetMgrClient_sp->getWifiStatus(iCurWifiStatus);
			//iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_EnableDhcp,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				break;
			}
			strResultList << QString::number(iCurWifiStatus);
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Channel,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			else
			{
				strValue = QString("1");
			}
			
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_SSID,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;

			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Security,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Key,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;			
		}		
			
	}while(FALSE);

	return strResultList;
}

QStringList QWebBrowserApp::GetWirelessDhcpInfo()
{
	QStringList strResultList;
	int iOutRet = ERROR_SUCCESS;
	do
	{		
		int  iCurWifiStatus = -1;
		if(m_NetMgrClient_sp)
		{
			iOutRet = m_NetMgrClient_sp->getWifiStatus(iCurWifiStatus);
		}
		if(iOutRet != ERROR_SUCCESS)
		{
			break;
		}
		else
		{
			if(iCurWifiStatus == Mp7xxCommon::WIFI_STATUS_NotInstalled ||
			  iCurWifiStatus == Mp7xxCommon::WIFI_STATUS_DISABLED)
			{
				break;
			}
			QString strValue = "";
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_IPv4Addr,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_SubnetMask,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Gateway,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Dns1,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_Dns2,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}
			strResultList << strValue;			
			iOutRet = m_NetMgrClient_sp->getWifiCfg(Type_EnableDhcp,strValue);
			if(iOutRet != ERROR_SUCCESS)
			{
				//break;
			}			
			strResultList << strValue;
		}			
	}while(FALSE);

	return strResultList;
}

INT_t QWebBrowserApp::SaveWifiCfg(QString & strParam,int iCurStatus)
{
	int iRet,iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(m_NetMgrClient_sp)
		{			
			if(Mp7xxCommon::WIFI_STATUS_DISABLED == iCurStatus)
			{
				iRet = m_NetMgrClient_sp->setWifiStatus(iCurStatus);
				iOutRet = iRet;
				if(iOutRet != ERROR_SUCCESS)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}	
				break;
			}
			else if(Mp7xxCommon::WIFI_STATUS_ENABLED == iCurStatus)
			{
				iRet = m_NetMgrClient_sp->setWifiStatus(0);
				iRet = m_NetMgrClient_sp->setWifiStatus(iCurStatus);
				iOutRet = iRet;
				if(iOutRet != ERROR_SUCCESS)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}	
			}
			else
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
			QString strValue = "" ;
			m_NetMgrClient_sp->getWifiCfg(Type_EnableDhcp,strValue);
			strParam += QString(PARAM_KEY_WifiEthDhcp)+ "=" +strValue + "\n";

			iRet = m_NetMgrClient_sp->setWifiCfg(strParam);	
			
			if(Sw_LogDbusCmd)
			{
				LOG_BLINE("strParam(%s),iOutRet=%d\n", qPrintable(strParam), iOutRet);
			}
			iOutRet = iRet;
			if(ERROR_SUCCESS != iOutRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);	

	return iOutRet ;
}

void QWebBrowserApp::OnTimer_checkBootSplashTimeOut()
{
	int iRet = ERROR_SUCCESS;
	do
	{
		if(m_BootSplashTimer.isActive())
		{
			m_BootSplashTimer.stop();
		}
		else
		{
			break;
		}
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			break;
		}
		if(m_BIsPlaySplash)
		{
			StopPlayback();
			m_PlaylistDataA.ClearPlayListData();
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			ShowPlayerWindow();
			UpdateVideoPosition(true);
		}
	}while(FALSE);	

}

void QWebBrowserApp::setBootSplashPlayUrl(QString strBootSplashUrl, int & iOutRet)
{
	QString strPlayFileUrl;
	QString strFileType;

	iOutRet = ERROR_SUCCESS;
	
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		if(strBootSplashUrl.isEmpty() || strBootSplashUrl == " ")	//Clear
		{
			mp_Config->setConfigValue(INI_KEY_BootSplashPlayUrl, "");
			break;
		}
		strPlayFileUrl = ParsePlayFilePathToPlayUrl(strBootSplashUrl, strFileType);
		if(strPlayFileUrl.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
			0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
			0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
		{
			if(!QFile::exists(strPlayFileUrl))
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else
		{

		}
		mp_Config->setConfigValue(INI_KEY_BootSplashPlayUrl, strBootSplashUrl);
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setDefaultStartupItem(%s),iOutRet=%d\n", qPrintable(strBootSplashUrl), iOutRet);
	}
}

void QWebBrowserApp::getBootSplashPlayUrl(int & iOutRet, QString & strBootSplashUrl)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		strBootSplashUrl = mp_Config->getConfigValue(INI_KEY_BootSplashPlayUrl);
	}while(FALSE);
}

int QWebBrowserApp::SaveWirelessNetworkAddr(QString tempNetworkAddrInfo)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		if(m_NetMgrClient_sp)
		{
			QStringList strWirlessList = GetWirelessStatus();

			if(strWirlessList.size() < 6)
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			tempNetworkAddrInfo += QString(PARAM_KEY_WifiChannel)+ "="  +strWirlessList[2] + "\n";
			tempNetworkAddrInfo += QString(PARAM_KEY_WifiSsid)+ "=" +strWirlessList[3] + "\n";
			tempNetworkAddrInfo += QString(PARAM_KEY_WifiSecurity)+ "=" +strWirlessList[4] + "\n";
			tempNetworkAddrInfo += QString(PARAM_KEY_WifiKey)+ "=" +strWirlessList[5] + "\n";
			tempNetworkAddrInfo += QString(PARAM_KEY_WifiChannel)+ "=" +QString("1")+ "\n";

			iOutRet = m_NetMgrClient_sp->setWifiCfg(tempNetworkAddrInfo);
		}
	}while(FALSE);	

	return iOutRet ;
}

int QWebBrowserApp::getWifiSignalLevel()
{
	int iOutRet = ERROR_SUCCESS;
	int iCurrSigLevel = 0 ;
	do
	{
		if(m_NetMgrClient_sp)
		{
			iOutRet = m_NetMgrClient_sp->getWifiSignalLevel(iCurrSigLevel);
		}
		else
		{
			iOutRet = ERROR_INVALID_STATE;
			iCurrSigLevel = 0 ;
		}
	}while(FALSE);	

	if(iCurrSigLevel > 100)
	{
		iCurrSigLevel = 100 ;
	}
	else if(iCurrSigLevel < 0)
	{
		iCurrSigLevel = 0 ;
	}
	return iCurrSigLevel ;
}

int QWebBrowserApp::getTunerChannelStrength()
{
	int iOutRet = ERROR_SUCCESS;
	int iCurrSigLevel = 0 ;
	do
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = m_MediaPlayerCtrl_sp->getTunerChannelStrength(&iCurrSigLevel);
		}
		else
		{
			iOutRet = ERROR_INVALID_STATE;
			iCurrSigLevel = 0 ;
		}
	}while(FALSE);	
	
#define MAX_STRENGTH 5	

	if(iCurrSigLevel > MAX_STRENGTH ||iCurrSigLevel < 0 )
	{
		iCurrSigLevel = 0;
	}
	return iCurrSigLevel ;
}

void QWebBrowserApp::getFirstChNumber(int & iOutRet, int & majorNo,int  & minorNo)
{
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		iOutRet = m_MediaPlayerCtrl_sp->getFirstChNumber(& majorNo, & minorNo);
	}while(FALSE);
}


int QWebBrowserApp::Exec_PlayTunerChannel_Number(int iKeyCode)
{
	
	int iRet = ERROR_SUCCESS;	
	do
	{
		switch(iKeyCode)
		{
			case Qt::Key_0:
				m_ChannelNumber += QString("0");
				break;
			case Qt::Key_1:
				m_ChannelNumber += QString("1");
				break;
			case Qt::Key_2	:	
				m_ChannelNumber += QString("2");
				break;
			case Qt::Key_3:
				m_ChannelNumber += QString("3");
				break;
			case Qt::Key_4	:
				m_ChannelNumber += QString("4");
				break;
			case Qt::Key_5	:
				m_ChannelNumber += QString("5");
				break;
			case Qt::Key_6	:
				m_ChannelNumber += QString("6");
				break;
			case Qt::Key_7	:
				m_ChannelNumber += QString("7");
				break;
			case Qt::Key_8	:
				m_ChannelNumber += QString("8");
				break;
			case Qt::Key_9:
				m_ChannelNumber += QString("9");
				break;	
			default:
				m_ChannelNumber = "";
				break;
		}
		
		if(m_TunerChannelBar_sp.isNull())
		{
			m_TunerChannelBar_sp = QSharedPointer <QInputChannelWin> (new QInputChannelWin(&m_playWin));
			connect(&(*m_TunerChannelBar_sp),SIGNAL(inputHide_timeOut(QString)),this,SLOT(Play_InputChannel(QString)));
		}		
		if(m_TunerChannelBar_sp.isNull())
		{
			break;
		}
		{
			if(m_ChannelNumber.length()==1)
			{
				iRet = m_TunerChannelBar_sp->setText(m_ChannelNumber ,2*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			else if(m_ChannelNumber.length()==2)
			{
				QString strShowText = m_ChannelNumber.mid(0,1) + QString("-") 
					+m_ChannelNumber.mid(1,1);	
				
				iRet = m_TunerChannelBar_sp->setText(strShowText,2*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			else if(m_ChannelNumber.length()==3)
			{
				QString strShowText = m_ChannelNumber.mid(0,2) + QString("-") 
					+m_ChannelNumber.mid(2,2);				
				iRet = m_TunerChannelBar_sp->setText(strShowText,2*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			else if(m_ChannelNumber.length()==4)
			{
				QString strShowText = m_ChannelNumber.mid(0,3) + QString("-") 
					+m_ChannelNumber.mid(3,1);				
				iRet = m_TunerChannelBar_sp->setText(strShowText,2*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
			else 
			{
				m_ChannelNumber ="";
			}
		}		
	}while(FALSE);

	return iRet;
}


// exec select channel number
void QWebBrowserApp::OnNotifyTunerPlayerPropmtMsg(int iType)
{
	int iRet = ERROR_SUCCESS;
	
	printf("======>>>>>>OnNotifyTunerPlayerPropmtMsg\n");
	
	do
	{
		if(iType == ERR_NO_DEV)
		{
			if(m_TunerStatusBarItem_sp.isNull())
			{
				m_TunerStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
			}
			if(FALSE == m_TunerStatusBarItem_sp.isNull())
			{
				iRet = m_TunerStatusBarItem_sp->setText("Not found tuner hardware !",5*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
				 	//m_UpgradeStatusBarItem_sp->show();
				}
				if(!m_pScangressBar.isNull())
				{					
					m_pScangressBar->setValue(0,0);
					m_pScangressBar->hide();
				}
				m_Browser.RefreshOsdUi();
			}
		}
		else if(ERROR_NOT_FOUND == iType)
		{
			if(m_TunerStatusBarItem_sp.isNull())
			{
				m_TunerStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
			}
			if(! m_TunerStatusBarItem_sp.isNull())
			{
				iRet = m_TunerStatusBarItem_sp->setText("Err, invalid channel number.", 5*1000);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}

				if(!m_MediaPlayerCtrl_sp.isNull())
				{
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_IDLE,1);
				}
			}	
		}
	}while(FALSE);
}

// play input channel number
void QWebBrowserApp::Play_InputChannel(QString strChannelNum)
{
	int iRet = ERROR_SUCCESS;
	do
	{	
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();		
		int iVirChIsEnable = 0 ;
		strChannelNum = "";
		int currTunerChMode=-1;
		int channelNumber=0;

		if(m_iTunerScanStatus != TUNER_HAS_SCAN)
		{
			DECLARE_CLS_STACK_BUF_STRING(strTunerScanStatus, 32);
			iRet = SysProp_get(SysProp_KEY_TunerScanStatus, OUT strTunerScanStatus);
			if(ERROR_SUCCESS == iRet)
			{
				m_iTunerScanStatus = QString(strTunerScanStatus).toInt();
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				m_iTunerScanStatus = TUNER_NO_SCAN;
			}
			else
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}

#if 1 /*Ade by xuweiwei 2014-5-8*/
		getTunerChannelMode(iRet,currTunerChMode);
		if(ERROR_SUCCESS == iRet)
		{
		}
		else if(ERROR_NOT_FOUND == iRet)
			currTunerChMode = 4;
		else
			break;

		if(m_TunerStatusBarItem_sp.isNull())
		{
			m_TunerStatusBarItem_sp = m_WbStatusBar_sp->RegisterStatusBarItem();
		}

		channelNumber=m_ChannelNumber.length();
		switch(channelNumber)
		{
		case 1:
		{
			QSharedPointer <QMediaVirtualChannel::QVirChInfo> VirChInfo_sp;
			iRet = mMediaVirChannel_sp->getVirChInfo(m_ChannelNumber.toInt(), OUT VirChInfo_sp);
			if(ERROR_SUCCESS == iRet)
			{
				iVirChIsEnable = VirChInfo_sp->m_bEnable;
			}
			if(iRet  == ERROR_SUCCESS  && iVirChIsEnable == 1)
			{
				setPlayVirtualChannel(m_ChannelNumber.toInt(),iRet);
				break;
			}
			if(m_ChannelNumber.toInt() == 0)
				break;
		}
		case 2:
		case 3:
		case 4:
			printf("channelNumber2:%d,currTunerChMode:%d,m_iTunerScanStatus:%d\n",channelNumber,currTunerChMode,m_iTunerScanStatus);
			if( m_iTunerScanStatus != TUNER_HAS_SCAN && currTunerChMode == 4 && FALSE == m_TunerStatusBarItem_sp.isNull())
			{
				if( 1 == channelNumber )
				{
					iRet = m_TunerStatusBarItem_sp->setText("Warning, tuner is not scan status.",5*1000);
					break;
				}
				iRet = m_TunerStatusBarItem_sp->setText("Warning, Tuner not scanned yet, please scan first.",5*1000);

			}
			else
			{
				if( 1 == channelNumber )
				{
					setPlayTuner(QString("#") +m_ChannelNumber,iRet);
					break;
				}
				m_ChannelNumber.insert(channelNumber-1, QString("-"));
				setPlayTuner(m_ChannelNumber,iRet);
			}
			break;
		default:
			if(FALSE == m_TunerStatusBarItem_sp.isNull())
			{
				iRet = m_TunerStatusBarItem_sp->setText("Warning, err Channel number.",5*1000);
			}
			break;
		}
#endif
	}while(FALSE);


	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		if(Sw_LogDbusCmd)
		{
			LOG_BLINE("Err Type:%d\n", iRet);
		}
	}
	m_ChannelNumber = "";	
}

void QWebBrowserApp::getPlayTuner(int & iOutRet, QString & strCurrTunerChannel)
{
	iOutRet = ERROR_SUCCESS;		
	
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		
		strCurrTunerChannel = mp_Config->getConfigValue(INI_KEY_CurrPlayTunerUrl);
	}while(FALSE);
}

int QWebBrowserApp::MP_Play(const QString & strUrl, int iLoopCount)
{
	return PlayMedia(strUrl,iLoopCount);
}

void QWebBrowserApp::startIdelDetectTimer()
{	
	do
	{
		if(mp_Config.isNull())
		{
			break;
		}		
		QString IdleTime = mp_Config ->getConfigValue(INI_KEY_IdleDetectTime);
		if(IdleTime.trimmed() =="")
		{
			break;
		}		
		int iIdleTime = -1;
		iIdleTime = IdleTime.trimmed().toInt();
		if(0 > iIdleTime)
		{
			break;
		}
		
		if(!m_bIsPlayingIdlePlayList)
		{
			if(m_eMediaPlayCurrStatus == MediaPlay_Status_PlaySelectFile)
			{
			}			
			{
				if(m_TunerScanStatus ==TunerScanning)
				{					
					break;
				}	
				
				if(m_iStateType == State_Idle )
				{
					m_PlayIdlePlayList_Timer.setSingleShot(true);
					m_PlayIdlePlayList_Timer.start(iIdleTime * 1000);  
				}
			}
		}		
	}while(FALSE);
}

void QWebBrowserApp::getTunerMode(int & iOutRet, int & iCurrTunerMode)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_MediaPlayerCtrl_sp->getTunerMode(& iCurrTunerMode);
		
	}while(FALSE);
}
 
QString  QWebBrowserApp::ResolveCommadToUrl(const QString & strCmd,QString & strFileType,QString & strOtherPar)
{
	QString strDataSrcDevice,  strFilename,strFileFullPath;
	int iIndex;
	int iOutRet = ERROR_SUCCESS;
	strFileFullPath = "" ;
	QString strCommad = strCmd;

	do
	{
		QStringList  strTempCommadList;
		strTempCommadList = strCommad.split("?");
		if(strTempCommadList.size() < 0)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		else
		{
			strCommad = strTempCommadList[0];
		}
		
		strOtherPar = strCmd.mid(strCommad.length()+1,strCmd.length()-strCommad.length());
		strOtherPar = strOtherPar.replace("?",",");		
		//first part
		iIndex = strCommad.indexOf(":");
		if(0 < iIndex)
		{
			strDataSrcDevice = strCommad.left(iIndex);
		}
		else if(0 == iIndex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		else
		{
			strDataSrcDevice = strCommad;
		}
		if(strDataSrcDevice.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}		
		//second part
		if(0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_FLASH, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_USB, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_HDD, Qt::CaseInsensitive) ||
		        0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_IDE, Qt::CaseInsensitive) ||
			0 == strDataSrcDevice.compare(CeLabs_DATA_SRC_DEV_SD, Qt::CaseInsensitive) )
		{
			if(0 < iIndex)
			{
				int iColonIndex;
				iColonIndex = strCommad.indexOf("://", iIndex+2);
				if(0 < iColonIndex)
				{
					strFileType = strCommad.mid(iIndex+2, iColonIndex - (iIndex + 2));
					//LOG_BLINE("strFileType = %s\n", qPrintable(strFileType));
					if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Music, Qt::CaseInsensitive)||
						0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
						0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
					{
						//the third part
						strFilename = strCommad.mid(iColonIndex+3);
						if(FALSE == strFilename.isEmpty())
						{
							strFileFullPath = getFileFullPathByCeLabsUrl(strDataSrcDevice, strFileType, strFilename);
							if(!QFile::exists(strFileFullPath))
							{
								iOutRet =  ERROR_INVALID_PARAMETER;
								break;
							}
						}
					}
				}
			}
		}
	}while(FALSE);		

	return strFileFullPath;
}

INT_t QWebBrowserApp::PlayUrl(const QString & strFileName, const QString  & strFileType,QString strOtherPar,int iEntryIndex)
{
	int iOutRet = ERROR_SUCCESS;
	int iRet = ERROR_SUCCESS;
	QSharedPointer <QPlaylistData> PluginPlaylistData_sp(new QPlaylistData);
	do
	{	
		if(PluginPlaylistData_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iOutRet = PluginPlaylistData_sp->insertPlayEntry(strFileName,strFileType,strOtherPar,iEntryIndex);
		if(iOutRet != ERROR_SUCCESS)
		{
			break;
		}
		iOutRet = setPluginListDataSource(PluginPlaylistData_sp,"");	
		if(iOutRet == ERROR_SUCCESS)
		{
			PlayPluginListNextEntry(true);
		}	
	}while(FALSE);	

	if(iOutRet != ERROR_SUCCESS)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	return iOutRet;
}

#if 1/*added by lxj 2012-12-11*/
int QWebBrowserApp::setTouchScreenCalibrateAsync(void)
{
	bool bRet;
	int iOutRet = ERROR_SUCCESS;
	QStringList CmdListParam;
	QString sPrograms;

	do{
		if( m_TouchScreenCalibrationProcess_sp.isNull() ){
			//LOG_BLINE("m_TouchScreenCalibrationProcess_sp is null\n");
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;			
		}

		/*kill last time ts calibration*/
		if( m_TouchScreenCalibrationProcess_sp->state() == QProcess::Running ){
			if(closeTouchScreenCalibrate() != ERROR_SUCCESS ){
				break;
			}
		}

		sPrograms = TS_CALIBRATION_PROGRAM_PATH "/" TS_CALIBRATION_PROGRAM_NAME;
		if( QFile::exists(sPrograms) == FALSE ){
			LOG_BLINE("Programs: \"%s\" is not exits, try to \"%s\"\n",qPrintable(sPrograms),TS_CALIBRATION_PROGRAM_NAME);
			sPrograms = TS_CALIBRATION_PROGRAM_NAME;
		}

		m_TouchScreenCalibrationProcess_sp->setProcessChannelMode(QProcess::ForwardedChannels);
		m_TouchScreenCalibrationProcess_sp->start( sPrograms, CmdListParam, QIODevice::NotOpen );
		bRet = m_TouchScreenCalibrationProcess_sp->waitForStarted( TS_CALIBRATION_TIMEOUT_MS );
		if( FALSE == bRet ){
			LOG_BLINE("Programs: \"%s\" run fails(timeout:%d), maybe the paths/variables of Environments is set incorrectly!\n",qPrintable(sPrograms),TS_CALIBRATION_TIMEOUT_MS);
			iOutRet = ERR_TIMEOUT;
			break;
		}

		iOutRet = ERROR_SUCCESS;
		if( Sw_LogDbusCmd ){
			LOG_BLINE("Ok, you can calibrate the touch screen now!\n");
		}
	}while(FALSE);

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setTouchScreenCalibrateAsync,iOutRet=%d\n", iOutRet);
	}

	return iOutRet;
}

int QWebBrowserApp::closeTouchScreenCalibrate(void)
{
	bool bRet;
	int iOutRet = ERROR_SUCCESS;

	//LOG_BLINE("%s()\n",__func__);

	do{
		if(m_TouchScreenCalibrationProcess_sp.isNull()){
			//LOG_BLINE("m_TouchScreenCalibrationProcess_sp is null\n");
			break;
		}

		if( m_TouchScreenCalibrationProcess_sp->state() == QProcess::NotRunning ){
			//LOG_BLINE("Touch screen calibration is not running!\n");
			break;
		}

		system("killall ts_calibrate");/*kill ts_calibrate first*/

		m_TouchScreenCalibrationProcess_sp->terminate();
		bRet = m_TouchScreenCalibrationProcess_sp->waitForFinished( TS_CALIBRATION_TIMEOUT_MS );
		if( FALSE == bRet ){
			LOG_BLINE("Waitfor TouchScreenCalibration process finished timeout(%d)\n",TS_CALIBRATION_TIMEOUT_MS);
			iOutRet = ERR_TIMEOUT;
			break;
		}

		iOutRet = ERROR_SUCCESS;
		if(Sw_LogDbusCmd){
			LOG_BLINE("TouchScreenCalibration is end\n");
		}
	}while(FALSE);

	return iOutRet;
}
#endif

int QWebBrowserApp::getOutputSpdifMode()
{
	int iOutRet = ERROR_SUCCESS;
	int iRet = ERROR_SUCCESS;
	int iCurMode = 0;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		iOutRet = m_MediaPlayerCtrl_sp->getOutputSpdifMode(& iCurMode);
		if( iOutRet != ERROR_SUCCESS)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iCurMode;
}

int QWebBrowserApp::setOutputSpdifMode(int iMode)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_MediaPlayerCtrl_sp->setOutputSpdifMode(iMode);
	}while(FALSE);

	return iOutRet;
}

void QWebBrowserApp::StopAllPlayDurationTimer()
{
	if(m_PlayDurationTimer.isActive ())
	{
		m_PlayDurationTimer.stop();
	}
	if(m_PlaylistDurationTimer.isActive ())
	{
		m_PlaylistDurationTimer.stop();
	}
	if(m_HtmlDurationTimer.isActive ())
	{
		m_HtmlDurationTimer.stop();
	}
	if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
	{
		m_PlayPluginListHtmlEntryDurationTimer.stop();
	}
	if(m_PlayPluginListEntryDurationTimer.isActive())
	{
		m_PlayPluginListEntryDurationTimer.stop();
	}
}

int QWebBrowserApp::adjustVolume(int adjustVolumePercent)
{
	INT_t iCurVolume;
	int iOutRet = ERROR_SUCCESS;
	do
	{
		iCurVolume = 0;
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		//iOutRet = m_MediaPlayerCtrl_sp->changeVolume(0, &iCurVolume);
		if(adjustVolumePercent == 0)
		{
			iCurVolume = iCurVolume;
			break;
		}
		if(iOutRet == ERROR_SUCCESS)
		{
			//adjustVolumePercent = iCurVolume + adjustVolumePercent;
			//iOutRet = m_MediaPlayerCtrl_sp->changeVolume(adjustVolumePercent, &iCurVolume);
			if(ERROR_SUCCESS == iOutRet)
			{
				m_CurPlayerVolume = iCurVolume;
				m_bVolumeMuted = FALSE;
			}
		}
	}while(false);
	
	if(ERROR_SUCCESS == iOutRet)
	{
		return iCurVolume;
	}
	else
	{
		return iOutRet;
	}
}

int QWebBrowserApp::SnmpCall_Set(const QString strOid, const QVariant varArgs)
{
	int iOutRet = ERROR_SUCCESS, iRet;
	bool bWaitingForSnmpResponse_IsSetByme = false;

	do
	{
		if(m_MainThreadId != QThread::currentThreadId ())
		{
			iOutRet = ERR_INCORRECT_THREAD_CONTEXT;
			break;
		}

		if(m_bWaitingForSnmpResponse)
		{
			iOutRet = ERR_BUSY;
			break;
		}

		m_bWaitingForSnmpResponse = true;
		bWaitingForSnmpResponse_IsSetByme = true;

		QSharedPointer <QMsg_SnmpSet> Msg_SnmpSet_sp(new QMsg_SnmpSet);
		if(Msg_SnmpSet_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = Msg_SnmpSet_sp->setParameters(strOid, varArgs);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		m_bMustWaitForSnmpResponse = true;
		m_iCurSnmpOpTransationId = Msg_SnmpSet_sp->getTransactionId();
		emit PostAsyncTransaction(Msg_SnmpSet_sp);
		UINT64_t startTimeMs = GetSysUpTimeMs();
		while(m_bMustWaitForSnmpResponse)
		{
			processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents, 100/*timeout,ms*/);
			if(GetSysUpTimeMs() - startTimeMs >= (10*1000))
			{
				iOutRet = ERR_TIMEOUT;
				break;
			}
		}
		m_iCurSnmpOpTransationId = 0;
		if(ERROR_SUCCESS == iOutRet)
		{
			iOutRet = Msg_SnmpSet_sp->getOutRet();
		}
	}while(false);

	if(bWaitingForSnmpResponse_IsSetByme)
	{
		m_bWaitingForSnmpResponse = false;
	}

	return iOutRet;
}

QString QWebBrowserApp::SnmpCall_Get(const QString strOid)
{
	QString strOutRet;
	int iOutRet = ERROR_SUCCESS, iRet;
	bool bWaitingForSnmpResponse_IsSetByme = false;

	do
	{
		if(m_MainThreadId != QThread::currentThreadId ())
		{
			iOutRet = ERR_INCORRECT_THREAD_CONTEXT;
			break;
		}

		if(m_bWaitingForSnmpResponse)
		{
			iOutRet = ERR_BUSY;
			break;
		}

		m_bWaitingForSnmpResponse = true;
		bWaitingForSnmpResponse_IsSetByme = true;

		QSharedPointer <QMsg_SnmpGet> Msg_SnmpGet_sp(new QMsg_SnmpGet);
		if(Msg_SnmpGet_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = Msg_SnmpGet_sp->setParameters(strOid);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		m_bMustWaitForSnmpResponse = true;
		m_iCurSnmpOpTransationId = Msg_SnmpGet_sp->getTransactionId();
		emit PostAsyncTransaction(Msg_SnmpGet_sp);
		UINT64_t startTimeMs = GetSysUpTimeMs();
		while(m_bMustWaitForSnmpResponse)
		{
			processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents, 100/*timeout,ms*/);
			if(GetSysUpTimeMs() - startTimeMs >= (10*1000))
			{
				iOutRet = ERR_TIMEOUT;
				break;
			}
		}
		m_iCurSnmpOpTransationId = 0;
		if(ERROR_SUCCESS == iOutRet)
		{
			iOutRet = Msg_SnmpGet_sp->getOutRet();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}			
		}
		strOutRet = Msg_SnmpGet_sp->getResult();
	}while(false);

	if(bWaitingForSnmpResponse_IsSetByme)
	{
		m_bWaitingForSnmpResponse = false;
	}

	return strOutRet;
}

int QWebBrowserApp::setVideoLayerPlayMedia(const QString & strUrl)
{
	int iOutRet = ERROR_SUCCESS;
	do
	{
		QWidget * pActiveWindow = NULL;
		pActiveWindow = activeWindow();

		/*
        	 if((pActiveWindow == &m_playWin && m_PlayPicAndHtmlWin->isPlayingHtml()))
		{			
		}
		else 
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		*/
		
		if(m_playWin.getVideoLayerWnd().isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(m_bIsPlayVideoLayer)
		{
			if(!m_playWin.getVideoLayerWnd().isNull())
			{
				m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayerEnd();
			}
		}
		setVideoLayerWndSize();	
		iOutRet = parsePlayMediaCommad(strUrl,iOutRet,playVideoLayer_MODE);
		if(iOutRet != ERROR_SUCCESS)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_bIsPlayVideoLayer = true;		
		m_playWin.getPlayHtmlOrPicWin()->lower();
		m_playWin.getVideoLayerWnd()->raise();
		m_playWin.getVideoLayerWnd()->show();
	}while(false);

	return iOutRet;
}

void QWebBrowserApp::playVideoLayer(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType,
	CONST QString & strFilename, int & iOutRet, int iLoopTimes,int iDurationTime, int iPositionTime,const QString & strOriginUrl/* = QString()*/,bool isPlayNextFile)
{
	INT_t iRet =ERROR_SUCCESS;
	QString strDevMediaDirName, strFileTypeDirName, strFileFullPath;
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	iOutRet = ERROR_SUCCESS;
	
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("iLoopTimes=%d,iDurationTime=%d,strFilename=%s\n",iLoopTimes,iDurationTime,qPrintable(strFilename));
		//LOG_BLINE("eDeviceType=%d,strOriginUrl=%d,strFilename=%s\n",eDeviceType,qPrintable(strOriginUrl),qPrintable(strFilename));
	}

	do
	{
		if(strFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}
		if(FORMAT_MEDIA_InNow == m_eFormatMediaStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		if(m_PlayIdlePlayList_Timer.isActive())
		{
			m_PlayIdlePlayList_Timer.stop();
			m_iIdleTime = 0;
		}
		if(m_bIsPlayingIdlePlayList)
		{
			m_bIsPlayingIdlePlayList = false;
			m_iIdleTime = 0;		
		}		
		if(m_bIsPlayPluginList)
		{
			if(m_PluginPlaylistData_sp)
			{
				m_PluginPlaylistData_sp->ClearPlayListData();
				m_bIsPlayPluginList = false;
			}
		}
		
		m_bIsPlayVideoLayer = true;
		m_BIsPlaySplash = false;		

	
		if( eDeviceType == AMedia_TYPE_URL)
		{
			if(0 == strFileType.compare(FileType_Http, Qt::CaseInsensitive))
			{
				//http media file?
				URL_TYPE eUrlType = TYPE_URL_UNKNOWN;
				QString strUrl;
				if(FALSE == strOriginUrl.isEmpty())
				{
					strUrl = strOriginUrl;
				}
				else
				{
					strUrl = strFilename;
				}
				eUrlType = DetermineUrlType(strUrl.toUtf8(), NULL, 0);
				//LOG_BLINE("Url:%s,UrlType=%d\n", qPrintable(strUrl), eUrlType);
				if(TYPE_URL_HTTP_MEDIA_FILE == eUrlType)
				{
					m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
					m_iStateType = State_Streaming;
					QWidget * pActiveWindow = NULL;
					pActiveWindow = activeWindow();	
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
					StopPlayPluginList();
					m_iStateType = State_Streaming;
					iOutRet = PlayUrl(strUrl);
					if(ERROR_SUCCESS != iOutRet)
					{
						break;
					}
					else
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
					break;	//already sent to MediaPlayer
				}
			}
			
			if( 0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Http, Qt::CaseInsensitive) )
			{				
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_PlayingBrowserFile;			
				if(m_bIsPlayVideoLayer)
				{
					if(!m_playWin.getVideoLayerWnd().isNull())
					{
						m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayer(strFilename);
					}
					if(m_MediaPlayerCtrl_sp)
					{
						m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
					}
					m_iStateType = State_PlayingBrowserFile;
					m_eCurPlayMode = ControlCenter::PlayMode_Normal;
					m_strCurPlayUrl =strFileFullPath ;
				}
				if(iDurationTime <= 0)
				{
					iDurationTime  = 10;
				}
				else
				{
					int iTimerIntervalMs = (iDurationTime * 1000);
					if(0 == iTimerIntervalMs)
					{
						iTimerIntervalMs = 100;
					}
					if(0 < iTimerIntervalMs)
					{
						if(m_PlayPluginListEntryDurationTimer.isActive())
						{
							m_PlayPluginListEntryDurationTimer.stop();
						}
						if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
						{
							m_PlayPluginListHtmlEntryDurationTimer.stop();
						}
						m_PlayPluginListHtmlEntryDurationTimer.start(iTimerIntervalMs);
					}			
				}
				m_iStateType = State_PlayingBrowserFile;
			}
			else if(0 == strFileType.compare(FileType_Udp, Qt::CaseInsensitive) ||
				0 == strFileType.compare(FileType_Rtsp, Qt::CaseInsensitive) ||
				0 == strFileType.compare(FileType_Rtp, Qt::CaseInsensitive))
			{
				m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
				m_iStateType = State_Streaming;
				iOutRet = PlayUrl(strFilename);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				else
				{
					m_iStateType = State_Streaming;
					if(!isPlayNextFile)
					{
						m_PlaylistDataA.ClearPlayListData();
					}
				}
				break;
			}
			break;
		}

		strDevMediaDirName = getDevMediaDirNameByType(eDeviceType);
		strFileTypeDirName = getDirNameFromFileType(strFileType);
		if(strDevMediaDirName.isEmpty() || strFileTypeDirName.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		strFileFullPath = qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
			strDevMediaDirName + "/" + strFileTypeDirName + "/" + strFilename;


		if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive))
		{
			QSharedPointer <QPlaylistData> PluginPlaylistData_sp(new QPlaylistData);
			iRet = PluginPlaylistData_sp->LoadFromIniFile(&strFileFullPath);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}		
			iRet= setPluginListDataSource(PluginPlaylistData_sp,strFileFullPath);
			if(iRet == ERROR_SUCCESS)
			{
				PlayPluginListNextEntry(true);
			}			
		}
		else if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
		{
			if(!QFile::exists(strFileFullPath))
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
			else
			{	
				QWidget * pActiveWindow = NULL;
				pActiveWindow = activeWindow();
				bool bFullScreen = false;				
				if(!isPlayNextFile)
				{
					m_PlaylistDataA.ClearPlayListData();
				}
				StopPlayPluginList();
				StopAllPlayDurationTimer();
				if(0 == strFileType.compare(FileType_Desc_Video, Qt::CaseInsensitive))
				{
					m_iStateType = State_PlayingVideoFile;
					if(bFullScreen)
					{
						if(!m_MediaPlayerCtrl_sp.isNull())
						{
							iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Normal);
							if(ERROR_SUCCESS != iRet)
							{
								PRINT_BFILE_LINENO_IRET_STR;
							}
						}
					}
				}
				else if(0 == strFileType.compare(FileType_Desc_Audio, Qt::CaseInsensitive))
				{
					m_iStateType = State_PlayingAudioFile;
				}
				else if(0 == strFileType.compare(FileType_Desc_Picture, Qt::CaseInsensitive))
				{
					m_iStateType = State_PlayingPictureFile;
				}

				if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
				{
					 if(iLoopTimes ==  0)
					{
						if(iDurationTime == 0)
						{
							iOutRet = PlayUrl(strFileFullPath,0);	
							break;
						}
						else
						{
							iOutRet = PlayUrl(strFileFullPath);
						}
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						} 
						m_NextFileInfoToPlay.m_bIsInitZero = true;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;
						playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,m_bIsPlayVideoLayer);
						if(iOutRet == ERROR_SUCCESS)
						{
							if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
							{
								int iTimerTimeoutMs = (iDurationTime * 1000);
								if(0 < iTimerTimeoutMs)
								{ 
									m_PlayDurationTimer.stop();
									m_PlayDurationTimer.start(iTimerTimeoutMs);
								}
							}
						}
						break;
					}
					else   if(iLoopTimes == 1)
					{
						iOutRet = PlayUrl(strFileFullPath);
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
						m_NextFileInfoToPlay.m_bIsInitZero = false;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;
					}
					else if(iLoopTimes >1)
					{
						iOutRet = PlayUrl(strFileFullPath);
						iLoopTimes --;
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
						m_NextFileInfoToPlay.m_bIsInitZero = false ;
						m_NextFileInfoToPlay.m_bIsInitOne  = true;
						playNextFile(eDeviceType, strFileType, strFilename,iOutRet,iLoopTimes,iDurationTime,iPositionTime,m_bIsPlayVideoLayer);
					}
					else
					{
						m_NextFileInfoToPlay.m_bIsInitZero = FALSE;
						m_NextFileInfoToPlay.m_bIsInitOne  = false;

#if 1					/*modify by lxj 2012-7-23 fixed play twice bug*/
						iOutRet = PlayUrl(strFileFullPath);//iLoopTimes
						if(ERROR_SUCCESS != iOutRet)
						{
							StopPlayDurationTimer();
							break;
						}
						else
						{
							if(!isPlayNextFile)
							{
								m_PlaylistDataA.ClearPlayListData();
							}
						}
#endif
					}

					if(iOutRet == ERROR_SUCCESS)
					{
						if(iDurationTime != DEFAULT_PLAY_DURATION_TIME)
						{
							int iTimerTimeoutMs = (iDurationTime * 1000);
							if(0 < iTimerTimeoutMs)
							{
								m_PlayDurationTimer.stop();
								m_PlayDurationTimer.start(iTimerTimeoutMs);
							}
						}
					}
				}
				else
				{				
					iOutRet = PlayUrl(strFileFullPath,iLoopTimes);					
					if(ERROR_SUCCESS != iOutRet)
					{
						StopPlayDurationTimer();
						break;
					}
					else
					{
						if(!isPlayNextFile)
						{
							m_PlaylistDataA.ClearPlayListData();
						}
					}
					m_NextFileInfoToPlay.m_bIsInitZero = false ;
					m_NextFileInfoToPlay.m_bIsInitOne  = false;
				}
			}
		}
		else if(0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive))
		{
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlaySelectFile;
			m_iStateType = State_PlayingBrowserFile;
			
			if(m_bIsPlayVideoLayer)
			{
				if(!m_playWin.getVideoLayerWnd().isNull())
				{
					m_playWin.getVideoLayerWnd()->PlayHtmlEntryOnVideoLayer(strFileFullPath);
				}
				if(m_MediaPlayerCtrl_sp)
				{
					m_MediaPlayerCtrl_sp->DispOnLedScrAsync(PLAY_STATUS_PLAY,1);
				}
				m_iStateType = State_PlayingBrowserFile;
				m_eCurPlayMode = ControlCenter::PlayMode_Normal;
				m_strCurPlayUrl =strFileFullPath ;
			}
			if(iDurationTime <= 0)
			{
				iDurationTime  = 10;
			}

			int iTimerIntervalMs = (iDurationTime * 1000);
			if(0 == iTimerIntervalMs)
			{
				iTimerIntervalMs = 100;
			}
			if(0 < iTimerIntervalMs)
			{
				if(m_PlayPluginListEntryDurationTimer.isActive())
				{
					m_PlayPluginListEntryDurationTimer.stop();
				}
				if(m_PlayPluginListHtmlEntryDurationTimer.isActive())
				{
					m_PlayPluginListHtmlEntryDurationTimer.stop();
				}
				m_PlayPluginListHtmlEntryDurationTimer.start(iTimerIntervalMs);
			}			
			m_iStateType = State_PlayingBrowserFile;
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

void QWebBrowserApp::setVideoLayerWndSize()
{
	do
	{
		int iXPos, iYPos, iWidth, iHeight;
		iXPos = mp_Config->getConfigValue(INI_KEY_Xpos).toInt();//, QString::number(iPosValue));
		iYPos = mp_Config->getConfigValue(INI_KEY_Ypos).toInt();
		iWidth = mp_Config->getConfigValue(INI_KEY_XSize).toInt();
		iHeight = mp_Config->getConfigValue(INI_KEY_YSize).toInt();
		if(iWidth == 0 || iHeight ==0)
		{
			m_playWin.getVideoLayerWnd()->move(0, 0);
			m_playWin.getVideoLayerWnd()->resize(0, 0);
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				m_MediaPlayerCtrl_sp->setVideoPosition(iXPos, iYPos, iWidth, iHeight);
			}
		}
		else
		{
			int HardwareX, HardwareY, HardwareWidth, HardwareHeight;
			HardwareX = iXPos;
			HardwareY = iYPos;
			HardwareWidth = iWidth;
			HardwareHeight = iHeight;
			
			int iCurrCorrdinateMode = mp_Config->getConfigValue(INI_KEY_CoordinateMode).toInt();
			int iPosValue = 0;
			int iWndPosX ,iWndPosY,iWndWidth,iWndHeight;
			iWndPosX = 0 ;
			iWndPosY = 0;
			iWndWidth = 0;
			iWndHeight = 0;
			
			if(!m_MediaPlayerCtrl_sp.isNull())
			{
				QDesktopWidget * pDesktopWidget = desktop();					
				if(NULL == pDesktopWidget)
				{
					break;
				}
				QRect DesktopRect = pDesktopWidget->screenGeometry();
				
				if(iCurrCorrdinateMode == RELATIVE_MODE) // 百分比
				{
					iPosValue = (iXPos* VideoSurface_WIDTH / 100);
					HardwareY = (iYPos* VideoSurface_WIDTH / 100);
					HardwareWidth = (iWidth* VideoSurface_WIDTH / 100);
					HardwareHeight = (iHeight* VideoSurface_WIDTH / 100);

					
					iWndPosX = (iXPos *  DesktopRect.width() / 100);
					iWndPosY = (iYPos *  DesktopRect.height() / 100);
					iWndWidth = (DesktopRect.width()  * iWidth /100);
					iWndHeight = (DesktopRect.height()  * iHeight /100);
				}	
				else
				{
					iPosValue = iXPos;
					iWndPosX = (iXPos *  DesktopRect.width() / VideoSurface_WIDTH);
					iWndPosY = (iYPos *  DesktopRect.height() / VideoSurface_WIDTH);
					iWndWidth = (iWidth * DesktopRect.width() / VideoSurface_WIDTH);
					iWndHeight = (iHeight * DesktopRect.width() / VideoSurface_WIDTH );
				}
				
				if(!m_MediaPlayerCtrl_sp.isNull())
				{
					m_MediaPlayerCtrl_sp->setVideoPosition(iPosValue, HardwareY, HardwareWidth, HardwareHeight);
				}
				//setVideoPosition(iPosValue, HardwareY, HardwareWidth, HardwareHeight);
				m_playWin.getVideoLayerWnd()->move(iWndPosX, iWndPosY);
				m_playWin.getVideoLayerWnd()->resize(iWndWidth, iWndHeight);
			}		 
		}	
	}while(false);
}

bool QWebBrowserApp::getIsPlayVideoLayer()
{
	return m_bIsPlayVideoLayer;
}

void QWebBrowserApp::setTunerChannelMode(int iTunerChMode, int & iOutRet)
{
	INT_t iRet = ERROR_SUCCESS;
	iOutRet = ERROR_SUCCESS;
	do
	{
		if(4 < iTunerChMode && 0 < iTunerChMode)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = m_MediaPlayerCtrl_sp->setTunerChannelMode(iTunerChMode);
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		/*save
		iOutRet = SysProp_setPersistent(SysProp_KEY_TunerChannelMode,
			QString::number(iTunerChMode).toLatin1());;
		if(ERROR_SUCCESS != iOutRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		*/
		//apply
	}while(FALSE);
}

void QWebBrowserApp::getTunerChannelMode(int & iOutRet, int & iTunerChMode)
{
	INT_t iRet = ERROR_SUCCESS;
	
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = m_MediaPlayerCtrl_sp->getTunerChannelMode(& iTunerChMode);
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			iRet = iOutRet;
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
	}while(FALSE);
}

int QWebBrowserApp::getTunerChannelMode()
{
	INT_t iOutRet = ERROR_SUCCESS;
	int iTempCurrTunerChannelMode = 0 ;
	
	do
	{
		getTunerChannelMode(iOutRet,iTempCurrTunerChannelMode);
		
	}while(false);

	return iTempCurrTunerChannelMode;
}

int QWebBrowserApp::SaveTunerChannelMode(int iSaveMode)
{
	INT_t iOutRet = ERROR_SUCCESS;
	do
	{
		setTunerChannelMode(iSaveMode,iOutRet);		
	}while(false);

	return iOutRet;
}

int QWebBrowserApp::setHdmi3DTvModeSlot(int Hdmi3DTvMode)
{
	int iRet = ERROR_SUCCESS;
	do
	{
		if(!m_MediaPlayerCtrl_sp.isNull())
		{
			if(MediaPlayer::H3DTvMode_Normal == (HDMI_3D_TV_MODE)Hdmi3DTvMode)
			{
				iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Normal);
			}
			else if(MediaPlayer::H3DTvMode_Force2D == (HDMI_3D_TV_MODE)Hdmi3DTvMode)
			{
				iRet = m_MediaPlayerCtrl_sp->setHdmi3DTvMode(MediaPlayer::H3DTvMode_Force2D);
			}
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);


	return iRet;
}

int QWebBrowserApp::setFormatMediaAsync(int iAction)
{
	int iRet = ERROR_SUCCESS;
	do
	{
		setFormatMediaAsync(iAction,iRet);
	}while(FALSE);
	return iRet;
}

void QWebBrowserApp::setVideoLayerPlayMode(int iVideoLayerMode,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(!m_bIsPlayVideoLayer)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
	
		if(ControlCenter::PlayMode_StopContent  == iVideoLayerMode)
		{
			if(m_MediaPlayerCtrl_sp)
			{
				iOutRet = stopMPlayerWithEventLog();
			}
		}
		else 
		{
			setPlayMode(iVideoLayerMode,  iOutRet);
		}
	}while(false); 
	
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setVideoLayerPlayMode(%d) ret %d\n", iVideoLayerMode, iOutRet);
	}
}


int QWebBrowserApp::setHtmlLayerPlay(const QString & strUrl)
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		/*
		if(!m_bIsPlayVideoLayer)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		*/
		iOutRet = parsePlayMediaCommad(strUrl,iOutRet,playHtmlLayer_MODE);
		if(iOutRet != ERROR_SUCCESS)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
	}while(false); 


	return iOutRet;
}


void QWebBrowserApp::playHtmlLayer(CONST ActiveMedia_TYPE eDeviceType, CONST QString & strFileType, CONST QString & strFilename,
					int & iOutRet, int iLoopTimes ,int iDurationTime, int iPositionTime,const QString & strOriginUrl , 
					bool isPlayNextFile __attribute__ ((__unused__)))
{
	QString strDevMediaDirName, strFileTypeDirName, strFileFullPath;
	QAutoLock AutoRangeLocker(&m_SharedDataMutex);
	iOutRet = ERROR_SUCCESS;
	
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("iLoopTimes=%d,iDurationTime=%d,strOriginUrl=%s\n",iLoopTimes,iDurationTime,qPrintable(strOriginUrl));
	}

	do
	{
		if( eDeviceType == AMedia_TYPE_URL)
		{
			
			if( 0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive)
				|| 0 == strFileType.compare(FileType_Http, Qt::CaseInsensitive) )
			{				

				//http media file?
				URL_TYPE eUrlType = TYPE_URL_UNKNOWN;
				QString strUrl;
				if(FALSE == strOriginUrl.isEmpty())
				{
					strUrl = strOriginUrl;
				}
				else
				{
					strUrl = strFilename;
				}
				eUrlType = DetermineUrlType(strUrl.toUtf8(), NULL, 0);
				//LOG_BLINE("Url:%s,UrlType=%d\n", qPrintable(strUrl), eUrlType);
				if(TYPE_URL_HTTP_MEDIA_FILE == eUrlType)
				{
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
				
				iLoopTimes = 0;
				if(iDurationTime <= 0 )
				{
					iOutRet = CallPlayWebpage(strUrl,0,true);
				}
				else
				{
					iOutRet = CallPlayWebpage(strUrl,iDurationTime);
				}
				
				m_iStateType = State_PlayingBrowserFile;
			}
			break;
		}


		strDevMediaDirName = getDevMediaDirNameByType(eDeviceType);
		strFileTypeDirName = getDirNameFromFileType(strFileType);
		
		if(strDevMediaDirName.isEmpty() || strFileTypeDirName.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		strFileFullPath = qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/") +
			strDevMediaDirName + "/" + strFileTypeDirName + "/" + strFilename;


		if(0 == strFileType.compare(FileType_Desc_Html, Qt::CaseInsensitive) ||
		0 == strFileType.compare(FileType_Browser, Qt::CaseInsensitive))
		{
			m_eMediaPlayCurrStatus = MediaPlay_Status_PlayCeLabsFile;
			m_iStateType = State_PlayingBrowserFile;
			
			iLoopTimes = 0;
			if(iDurationTime <= 0 )
			{
				iOutRet = CallPlayWebpage(strFileFullPath,0,true);
			}
			else
			{
				iOutRet = CallPlayWebpage(strFileFullPath,iDurationTime);
			}
			
			m_iStateType = State_PlayingBrowserFile;
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
		
	}while(false); 
}

QString QWebBrowserApp::getCurrMainHtmlUrl()
{
	return m_PlayPicAndHtmlWin->getCurrPlayHtmlUrl();;
}

bool QWebBrowserApp::getCurrPlaybackEosFlag()
{
    return m_bIsCompletePlaybackEos;
}

int QWebBrowserApp::WD_SetParam(int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs)
{
    	INT_t iRet,iOutRet=ERROR_SUCCESS;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;

	do
	{
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createMethodCall(SS_DBUS_SERVICE_NAME, 
		DBUS_PATH_SysSrv_HwWatchDog,DbusIf_SysSrv_HwWatchDog,"WD_SetParam");
		MsgArgsList <<  bAutoFeedWatchDog <<nWatchDogTimeoutMs;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
        
		DbusMsgReply = m_DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}


int QWebBrowserApp::setPlayNext()
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		//On_DefKey_Down(IR_KEY_DOWN);
		if(!m_PlayPicAndHtmlWin->isPlayingHtml())
		{
			StopAllPlayDurationTimer();		
			StopPlayback();
			PlayNextEntry();
		}
	}while(false); 
	
	return iOutRet;
}
#if 1 /*by_su for save Language index to SystemConfig.ini*/
int QWebBrowserApp::SaveLanguageIndex(int languageindex)
{
	int iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
		else
		{
			mp_Config->setConfigValue(KEY_Language_Type, QString::number(languageindex));

			if(languageindex >= 0)
			{
				On_ChangeLanguageDone(languageindex);
			}
		}
	}while(false); 

	return iOutRet;
}

int QWebBrowserApp::GetLanguageIndex()
{
	int iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(mp_Config.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
		}
		else
		{
			return mp_Config ->getConfigValue(KEY_Language_Type).toInt();
		}
	}while(false); 

	return iOutRet;
}
#endif

QVariantList QWebBrowserApp::GuiOsd2_getScreenSize()
{
	INT_t iRet;
	int scrWidth, scrHeight;
	QVariantList sizeList;
	
	iRet = m_GuiOsd2Ctrl.getScreenSize(OUT scrWidth, OUT scrHeight);
	if(ERR_SUCCESS != iRet)
	{
		scrWidth = 0;
		scrHeight = 0;
	}

	sizeList.append(scrWidth);
	sizeList.append(scrHeight);

	return sizeList;
}

int QWebBrowserApp::GuiOsd2_ClearScreen()
{
	int iOutRet = ERR_SUCCESS;

	iOutRet = m_GuiOsd2Ctrl.ClearScreen();

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_ClearRect(int x, int y, int w, int h)
{
	int iOutRet = ERR_SUCCESS;

	iOutRet = m_GuiOsd2Ctrl.ClearRect(x, y, w, h);

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_drawTextAlignCenter(int x, int y, int w, int h, const QString & text)
{
	int iOutRet = ERR_SUCCESS;

	iOutRet = m_GuiOsd2Ctrl.drawTextAlignCenter(x, y, w, h, text);

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_setFontSize(int pixelSize)
{
	int iOutRet = ERR_SUCCESS;

	iOutRet = m_GuiOsd2Ctrl.setFontSize(pixelSize);

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_setTextColor(int r, int g, int b, int alpha)
{
	return m_GuiOsd2Ctrl.setTextColor(r, g, b, alpha);
}

int QWebBrowserApp::GuiOsd2_fillRect(int x, int y, int w, int h, int r, int g, int b, int a)
{
	return m_GuiOsd2Ctrl.fillRect(x, y, w, h, r, g, b, a);
}

int QWebBrowserApp::GuiOsd2_DrawImage(const QString & strFilePath, int scrX, int scrY)
{
	int iOutRet = ERR_SUCCESS;

	iOutRet = m_GuiOsd2Ctrl.drawImage(strFilePath, scrX, scrY);

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_DrawTiledImage(const QString & strFilePath, int scrX, int scrY, int scrW, int scrH)
{
	int iOutRet = ERR_SUCCESS;
	iOutRet = m_GuiOsd2Ctrl.drawTiledImage(strFilePath, scrX, scrY, scrW, scrH);

	return iOutRet;
}

int QWebBrowserApp::GuiOsd2_setFontName(const QString& strFontName)
{
	int iOutRet = ERR_SUCCESS;
	iOutRet = m_GuiOsd2Ctrl.setFontName(strFontName);
	return iOutRet;
}

int QWebBrowserApp::GuiOsd2VideolistPlay(const QString strUrl)
{
	int iOutRet = ERR_SUCCESS;
	iOutRet = setVideoLayerPlayMedia(strUrl);
	return iOutRet;
}

INT_t QWebBrowserApp::stopMPlayerWithEventLog()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	quint32 stoppedPlaybackId;
	QString stoppedUrl;

	do
	{
		if(m_MediaPlayerCtrl_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = m_MediaPlayerCtrl_sp->Stop(&stoppedPlaybackId, &stoppedUrl);
		//write into the evt log
		if(ERROR_SUCCESS == iOutRet && 0 < stoppedPlaybackId && (FALSE == playbackIdIsEnded(stoppedPlaybackId)))
		{
			rememberEndedPlaybackId(stoppedPlaybackId);
			//
			BOOL_t bEvtLogIsEnalbed = FALSE;
			iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEvtLogIsEnalbed);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			if(bEvtLogIsEnalbed)
			{
				CString::STD_STRING_LIST strList;
				strList.push_back(TYPE_LOG_PlaybackEos);
				strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData());
				QString strPlayType;
				getLastPlayEntryInfo(OUT strPlayType, stoppedPlaybackId);
				if(strPlayType.isEmpty())
				{
					strPlayType = "unknown";
				}
				strList.push_back(strPlayType.toLocal8Bit().constData());
				strList.push_back(stoppedUrl.toLocal8Bit().constData());
				strList.push_back(stoppedPlaybackId);
				strList.push_back(0);
				if(ERROR_SUCCESS != WriteEventLog(strList))
				{
					PRINT_BFILE_LINENO_IRET_CRT_ERR;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

VOID QWebBrowserApp::rememberEndedPlaybackId(const quint32 endedPlaybackId)
{
	if(0 <= mEndedPlaybackIdList.lastIndexOf(endedPlaybackId))	//exists
	{
		goto EXIT_PROC;
	}
	mEndedPlaybackIdList.push_back(endedPlaybackId);
#define	MAX_EndedPlaybackIdList_SIZE		(30)
	if(MAX_EndedPlaybackIdList_SIZE < mEndedPlaybackIdList.size())
	{
		mEndedPlaybackIdList.removeFirst();
	}
EXIT_PROC:
	;
}

BOOL_t QWebBrowserApp::playbackIdIsEnded(const quint32 playbackId)
{
	return (0 <= mEndedPlaybackIdList.lastIndexOf(playbackId));
}


//added by Rocky
void QWebBrowserApp::TarUpgradeZipError(QProcess::ProcessError exitCode)
{
	INT_t iRet,iOutRet;
	iOutRet = ERROR_SUCCESS;

	//WD, restore its default.
	iRet = WD_SetParam(1, (30*1000));
	if(ERROR_SUCCESS != iRet)
	{
		LOG_BLINE("Set Watch dog auto err:%d\n", iRet);
	}

	iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareBadFile);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	{
		QString strCommand = "";
		strCommand = (QString)"rm -fr "  + m_strTempSysUpgradeUrl ;
		//LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));		
		iRet = system(strCommand.toLocal8Bit().constData());
		if(-1 == iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_FAILED;
			//break;
		}
		else if(0 != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", strCommand.toLocal8Bit().constData(), iRet);
			iOutRet = ERROR_FAILED;
			//break;
		}	
		DECLARE_CLS_STACK_BUF_STRING(strShellCmd_3, 1024);
		iRet = strShellCmd_3.Format("rm -fr  /SystemUpgrade");
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;//	break;
		}
		iRet = system(strShellCmd_3);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_3, iRet);
		}

		DECLARE_CLS_STACK_BUF_STRING(strShellCmd_2, 1024);
		iRet = strShellCmd_2.Format("rm -fr " NewSysCfg_DOWNLOAD_TEMP_FILE_PATH);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;//				break;
		}
		iRet = system(strShellCmd_2);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd_2, iRet);
		}

		DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
		iRet = strShellCmd.Format("rm -fr " NewSystemVersion_DECOMPRESSION_TEMP_FILE_PATH);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;//				break;
		}
		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);
		}
	}

	m_eCurUpgradeStatus = SystemUpgrade_Fail;
	m_eCurUpgradeStatus = SystemUpgrade_Idle;
	if(FALSE == m_UpgradeStatusBarItem_sp.isNull())
	{
		m_UpgradeStatusBarItem_sp->setText(
			"Upgrade:" + QString("failed to uncompress %1,ExitCode:%2").arg(NewSystemVersion_DOWNLOAD_TEMP_FILE_PATH).arg(exitCode),
			5*1000);
	}
	iRet = mWebBrowserSrv_sp->setTransferControlStatus(TranCtrlStat_FirmwareOtherError);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

