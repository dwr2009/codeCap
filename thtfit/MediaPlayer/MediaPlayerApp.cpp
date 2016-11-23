#include "MediaPlayerApp.h"
#include "MediaPlayerAppDef.h"
#include <SysBaseSrvApi.h>
#include <QDBusError>
#include <QDebug>
#include <unistd.h>
#include <DbgLogSwitchDef.h>
#include <AutoScopeLock.h>
#include <SystemTimezone.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include "memory.h"
#include "Mp7xxGlobalStatus.h"
#if 1	//added by jaylon for getcpunum
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qtextcodec.h>
#include <QStringList>

#endif
QDBusIf_MediaPlayer::QDBusIf_MediaPlayer(QMediaPlayerApp * pRealObj) : QDBusAbstractAdaptor(pRealObj)
{
	m_pRealObj = pRealObj;
	setAutoRelaySignals(true);
}

void QDBusIf_MediaPlayer::setDataSource(const QString & strUrl, int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setDataSource(strUrl);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Prepare(int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->Prepare();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Play(int & iOutRet, quint32 & playbackId)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->Play(1, OUT playbackId);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Seek(const unsigned int &Offset,int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->Seek(Offset);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setVideoOutputModeAsync(int mode,int & iOutRet, qint32 & newVoMode)
{
    VIDEO_OUTPUT_MODE eVoMode = VO_MODE_NotSet, eNewVoMode = VO_MODE_NotSet, eOldVoMode = VO_MODE_NotSet;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setVideoOutputModeAsync\n");
	}

    eVoMode= (VIDEO_OUTPUT_MODE)mode;
	
    if(m_pRealObj){
        iOutRet = m_pRealObj->ChangeVidOutModeAsync(eVoMode, &eNewVoMode,&eOldVoMode);
		newVoMode = eNewVoMode;
    } else {
        iOutRet = ERR_INV_STATE;
    }
}

#if 1 /*added by bert 2016/11/12*/
void QDBusIf_MediaPlayer::setHDRAsync(int mode,int & iOutRet, qint32 & smoothMode)
{
	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setHDRAsync\n");
	}

	smoothMode = mode;
	iOutRet = SysProp_setPersistent("smooth", mode);
	if(ERROR_SUCCESS != iOutRet)
	{
		LOG_BLINE("can not save smoothMode\n");
	}
	
	printf("set HDR => %d [%s] => %s  line:%d\n", mode,__FILE__, __func__, __LINE__);

}

void QDBusIf_MediaPlayer::getHDRAsync(int mode,int & iOutRet, qint32 & smoothMode)
{
    char strVideoFormatBuf[32]={0};
	int cursmoothMode;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setHDRAsync\n");
	}

	//printf("mode = %d, iOutRet = %d, smoothMode = %d\n", mode, iOutRet, smoothMode);

    iOutRet = SysProp_get("smooth",strVideoFormatBuf,32);
    if(0 == iOutRet)
    {         
    	cursmoothMode=atoi(strVideoFormatBuf);
		//printf("smoothMode => SysProp_get=%d\n", cursmoothMode);
        smoothMode=cursmoothMode;
    }else
    {
		printf("can not get smoothMode\n");
	}
}
#endif

void QDBusIf_MediaPlayer::ChangeVideoOutputMode(int mode,int & iOutRet, qint32 & newVoMode)
{
    VIDEO_OUTPUT_MODE eVoMode = VO_MODE_NotSet, eNewVoMode = VO_MODE_NotSet, eOldVoMode = VO_MODE_NotSet;

    eVoMode = (VIDEO_OUTPUT_MODE)mode;
//	LOG_LINE("%s,%s,mode is %d\n",__FILE__, __LINE__,mode);
//	LOG_LINE("mode is %d\n",mode);

    if(m_pRealObj){
        iOutRet = m_pRealObj->ChangeVideoOutputMode(eVoMode, &eNewVoMode, &eOldVoMode );
		newVoMode = eNewVoMode;
    } else {
        iOutRet = ERR_INV_STATE;
    }
}

void QDBusIf_MediaPlayer::getDisplayOutputMode(int & iOutRet, qint32 & displayoutputMode)
{
    if(m_pRealObj)
	{
        iOutRet = m_pRealObj->getDisplayOutputMode(OUT displayoutputMode);
    } 
	else 
    {
        iOutRet = ERR_INV_STATE;
    }
}

void QDBusIf_MediaPlayer::setDisplayOutputMode(qint32 displayoutputMode,int & iOutRet)
{
    if(m_pRealObj)
	{
        iOutRet = m_pRealObj->setDisplayOutputMode(displayoutputMode);
    } 
	else 
    {
        iOutRet = ERR_INV_STATE;
    }
}

void QDBusIf_MediaPlayer::getvideoColorSpace(int & iOutRet, qint32 & colorspace)
{
	if(m_pRealObj)
	{
        iOutRet = m_pRealObj->getvideoColorSpace(OUT colorspace);
    } 
	else 
    {
        iOutRet = ERR_INV_STATE;
    }

}


void QDBusIf_MediaPlayer::setvideoColorSpace(qint32 colorspace,int & iOutRet)
{
	if(m_pRealObj)
	{
        iOutRet = m_pRealObj->setvideoColorSpace(colorspace);
    } 
	else 
    {
        iOutRet = ERR_INV_STATE;
    }

}

void QDBusIf_MediaPlayer::setVideoPosition(int x, int y, int w, int h, int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setVideoPosition(x, y, w, h);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
void QDBusIf_MediaPlayer::setOsdSize(int w, int h, int & iOutRet)
{
    //LOG_BLINE("%s (%d,%d)\n",__FUNCTION__,w,h);
    if(m_pRealObj)
    {
        iOutRet = m_pRealObj->setOsdSize(w, h);
    }
    else
    {
        iOutRet = ERR_INV_STATE;
    }
}


void QDBusIf_MediaPlayer::DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos, int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->DispOnLedScrAsync(strContentToDisp, dispStartPos);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::scanI2cDev(int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->scanI2cDev();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setGpioLedStatus(const quint32 nGpioLedStatus, int & iOutRet, quint32 & nActualLedStatus)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setGpioLedStatus(nGpioLedStatus, OUT nActualLedStatus);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setGpioOutputVal(const int iGpioNum, const int iVal, int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setGpioOutputVal(iGpioNum, iVal);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setPlayMode(const qint32 iPlayMode, qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setPlayMode(iPlayMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getPlayMode(qint32 & iOutRet, qint32 & iPlayMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getPlayMode(OUT iPlayMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setHdmi3DTvMode(const qint32 iHdmi3DTvMode, qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setHdmi3DTvMode(iHdmi3DTvMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getMonitorInfo(qint32 & iOutRet, QString & strMonitorInfo)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getMonitorInfo(OUT strMonitorInfo);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getSerialControlData(qint32 & iOutRet,QString & strSerialControlData)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getSerialControlData(strSerialControlData);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags, qint32 & iOutRet)
{
	if(Sw_LogDbusCmd)
	{
		LOG("setVideoDispAspectRatio(%d,0x%08x)\n", iVidAspectRatio, uOptFlags);
	}

	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setVideoDispAspectRatio(iVidAspectRatio, uOptFlags);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}

	if(Sw_LogDbusCmd)
	{
		LOG("ret=%d\n", iOutRet);
	}
}

void QDBusIf_MediaPlayer::getVideoDispAspectRatio(qint32 & iOutRet, qint32 & iVidDispAspectRatio)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getVideoDispAspectRatio(OUT iVidDispAspectRatio);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
/* closed caption*/
void QDBusIf_MediaPlayer::getShowClosedCaption(qint32 & iOutRet, qint32 & iCcValue)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getShowClosedCaption(iCcValue);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setShowClosedCaption(qint32 iCcValue, qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setShowClosedCaption(iCcValue);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}


/*add by clyde 2015-06-01:MIB realize the function of fast forward  */
void QDBusIf_MediaPlayer::setPlaySpeed(qint32 iPlaySpeed, qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setPlaySpeed(iPlaySpeed);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
void QDBusIf_MediaPlayer::getPlaySpeed(qint32 & iOutRet, OUT qint32 & iPlaySpeed)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getPlaySpeed(OUT iPlaySpeed);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

/*add 2015-06-05 Realization MIB Browser to get/set MonitorInfo*/

void QDBusIf_MediaPlayer::MibsetMonitorInfo(qint32 MonitorInfo, qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->MibsetMonitorInfo(MonitorInfo);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::MibgetMonitorInfo(qint32 & iOutRet, qint32 & MonitorInfo)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->MibgetMonitorInfo(OUT MonitorInfo);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
//
void QDBusIf_MediaPlayer::setSerialControlData(const QString & strSerialControlData,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setSerialControlData(strSerialControlData);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getSerialParameters(qint32 & iOutRet,QString & strSerialParameters)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getSerialParameters(strSerialParameters);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setSerialParameters(const QString & strSerialControlData,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setSerialParameters(strSerialControlData);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getDisplayParam(const qint32 iDispParam,qint32 & iOutRet,qint32 &oValue)
{
	if(m_pRealObj)
	{
		printf("[%s:%d]QDBusIf_MediaPlayer iDispParam = %d\n",__func__,__LINE__,iDispParam);
		iOutRet = m_pRealObj->getDisplayParam(iDispParam,oValue);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getCurrentVolume(qint32 & iOutRet,qint32 &oValue)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getCurrentVolume(oValue);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}	
}

void QDBusIf_MediaPlayer::OnSaveVolume(qint32 iVolume,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->OnSaveVolume(iVolume);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}	
}

void QDBusIf_MediaPlayer::setDisplayParam(qint32 iDispParam,qint32 iValue,qint32 bSave,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		printf("[%s:%d]QDBusIf_MediaPlayer iDispParam = %d\n",__func__,__LINE__,iDispParam);
		iOutRet = m_pRealObj->setDisplayParam(iDispParam,iValue,bSave);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getOutputSpdifMode(qint32 & iOutRet,qint32 &oSpdifMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getOutputSpdifMode(oSpdifMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setOutputSpdifMode(qint32 iSpdifMode,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setOutputSpdifMode(iSpdifMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getTimeZoneSetting(qint32 & iOutRet,OUT qint32 & TimezoneHourOff)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getTimeZoneSetting(TimezoneHourOff);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}


void QDBusIf_MediaPlayer::setTimeZoneSetting(qint32 TimezoneHourOff,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setTimeZoneSetting(TimezoneHourOff);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setHdmiAudioOutput(qint32 bHdmiAudioOutput,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setHdmiAudioOutput(bHdmiAudioOutput);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setAnalogAudioMute(qint32 bAnalogAudioMute,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setAnalogAudioMute(bAnalogAudioMute);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}


void QDBusIf_MediaPlayer::setConfigDateTime(qint32 DateTime,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setConfigDateTime(DateTime);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}


void QDBusIf_MediaPlayer::getConfigDateTime(qint32 & iOutRet,QString & ConfigDateTime)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getConfigDateTime(ConfigDateTime);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setCurClkTicks(qint32 ClkTicks,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setCurClkTicks(ClkTicks);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}

}

void QDBusIf_MediaPlayer::getCurClkTicks(qint32 & iOutRet,OUT qint32& ClkTicks)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getCurClkTicks(ClkTicks);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setHdmiCecCtrl(qint32 HdmiCecCtrl,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setHdmiCecCtrl(HdmiCecCtrl);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

#ifdef ENABLE_DTV
void QDBusIf_MediaPlayer::PlayTuner(int & iOutRet, quint32 & playbackId)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->PlayTuner(OUT playbackId);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::TunerScan(int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->TunerScan();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getTunerChannelMode(quint32 & iOutRet, quint32 & TunerChannleMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getTunerChannelMode(TunerChannleMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setTunerChannelMode(qint32 TunerChannleMode,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setTunerChannelMode(TunerChannleMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::TunerMode(qint32 iStdMode,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->TunerMode(iStdMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::GetFirstChNumber(qint32 & iOutRet,quint32 & OMajor,quint32 & OMinor)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->GetFirstChNumber(OMajor,OMinor);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::GetTunerChannelStrength(qint32 & iOutRet,quint32& Strength)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->GetTunerChannelStrength(Strength);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::ResetDuration(qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->ResetDuration();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::ChannelUp(qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->ChannelUp();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
void QDBusIf_MediaPlayer::ChannelDown(qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->ChannelDown();
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::GetTunerMode(qint32 & iOutRet,qint32& iStdMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->GetTunerMode(iStdMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Get_EntryChannelInfo(qint32 EntryId,qint32 & iOutRet,qint32& pChannelNumber,qint32& pSubChannelCount,QString& StrProgramPids)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->GetEntryChannelInfo(EntryId,pChannelNumber,pSubChannelCount,StrProgramPids);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}
void QDBusIf_MediaPlayer::GetForceTxMode(qint32 & iOutRet,qint32 & iTxMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->GetForceTxMode(iTxMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}



#endif

void QDBusIf_MediaPlayer::setHwRtcTime(qint32 Year, qint32 Month, qint32 Date, qint32 Hour, qint32 Minute, qint32 Second, int  pszTimeZone,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setHwRtcTime(Year,Month,Date,Hour,Minute,Second,pszTimeZone);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer:: getDisplayPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getDisplayPosition(eRuaDispType,x,y,width,height);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer:: getVideoPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getVideoPosition(eRuaDispType,x,y,width,height);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setOsdPosition(qint32 x,qint32 y,qint32 width,qint32 height,qint32 type,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setOsdPosition(x,y,width,height,type);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Play2(qint32 iLoopCnt, int & iOutRet, quint32 & playbackId)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->Play(iLoopCnt, OUT playbackId);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::Stop(int & iOutRet, quint32 & playbackId, QString & strUrl)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->Stop(OUT playbackId, OUT strUrl);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

#if 0
void QDBusIf_MediaPlayer::ChangeVolume(int VolumePercentToChange,int & iOutRet,int & pNewVolumePercent, bool & pbVolChanged)
{
	int 	iNewVolumePercent=0;
	bool 	ibVolChanged=FALSE;
	
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->ChangeVolume(VolumePercentToChange,OUT &iNewVolumePercent, OUT &ibVolChanged);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}

	pNewVolumePercent=iNewVolumePercent;
	pbVolChanged=ibVolChanged;
}
#endif


void QDBusIf_MediaPlayer::getVideoSyncMode(qint32 & iOutRet,qint32 &SyncMode)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getVideoSyncMode(SyncMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setVideoSyncMode(qint32 SyncMode,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setVideoSyncMode((VIDEO_SYNC_MODE_t)SyncMode);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::getVideoSyncMaster(qint32 & iOutRet, QString &strMasterAddr)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getVideoSyncMaster(strMasterAddr);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::setVideoSyncMaster(const QString &strMasterAddr,qint32 & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setVideoSyncMaster(strMasterAddr);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_MediaPlayer::ChangeVolume(int VolumePercentToChange,int & iOutRet,bool & pbVolChanged)
{
	int 	iNewVolumePercent=0;
	bool 	ibVolChanged=FALSE;
	
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->ChangeVolume(VolumePercentToChange);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}

	pbVolChanged=ibVolChanged;
}

#if 1		//added by jaylon for getcpunum
void QDBusIf_MediaPlayer::getCpuSerialNo(qint32 & iOutRet, QString & CpuSerialNo)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->getCpuSerialNo(CpuSerialNo);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

int QMediaPlayerApp::getCpuSerialNo( QString & CpuSerialNo)
{
	 QFile CPU_info( "/proc/cpuinfo" );  
  	if( CPU_info.open( QIODevice::ReadOnly | QIODevice::Text ) )
  	{
		QTextStream stream( &CPU_info );
        QString cpu_info_string;
	
        //while ( !stream.atEnd() )
		do
		{
            cpu_info_string = stream.readLine(); 
		
			#if 1
			QStringList  strTempList;
			if(cpu_info_string.trimmed () != "")
			{
				strTempList = cpu_info_string.split(":");
			}
			if((strTempList.size() > 0))
			{
				if(0 == strTempList[0].trimmed().compare("Serial", Qt::CaseInsensitive))
				{
					CpuSerialNo = strTempList.at(1).trimmed();
					break;
				}
			}
			#endif
			
        }while(!cpu_info_string.isNull());
        CPU_info.close();
		return ERR_SUCCESS;
  	}
	return ERROR_NOT_SUPPORTED;
	
}
#endif

#if 1 //add by bert 2016/08/27
void QDBusIf_MediaPlayer::gethardwareCapability(qint32 & iOutRet, qint32 & hardwareCapability)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->gethardwareCapability(hardwareCapability);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

int QMediaPlayerApp::gethardwareCapability( qint32 & hardwareCapability)
{
	
	 QFile CPU_info( "/proc/cpuinfo" );  
  	if( CPU_info.open( QIODevice::ReadOnly | QIODevice::Text ) )
  	{
		QTextStream stream( &CPU_info );
        QString cpu_info_string;
	
        //while ( !stream.atEnd() )
		do
		{
            cpu_info_string = stream.readLine(); 
		
			#if 1
			QStringList  strTempList;
			if(cpu_info_string.trimmed () != "")
			{
				strTempList = cpu_info_string.split(":");
			}
			if((strTempList.size() > 0))
			{
				if(0 == strTempList[0].trimmed().compare("Platform", Qt::CaseInsensitive))
				{
					if ( "BCM97252C" == strTempList.at(1).trimmed())
					{
						hardwareCapability = 1;
						break;

					}
					else if ( "BCM97252SSFF_NOAVS" == strTempList.at(1).trimmed())
					{
						hardwareCapability = 2;
						break;
					}
					else
					{
						hardwareCapability = 0;
						break;

					}
				}
			}
			#endif		
        }while(!cpu_info_string.isNull());
		
        CPU_info.close();
		return ERR_SUCCESS;
  	}
	return ERROR_NOT_SUPPORTED;
	
}
#endif

QMediaPlayerApp::QMediaPlayerApp(int argc, char * argv[]) : QApplication(argc, argv),  CGeneralApp(argc, argv)
{
	INT_t iRet;

	m_hThread = 0;
	m_bMPlayerDbusServIsReged = FALSE;
	m_VolumePercent=0;

	iRet = CLinuxSignalHandlerEx::HookSignal(SIGINT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGTERM, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGQUIT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGCHLD, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	iRet = RegDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

QMediaPlayerApp::~QMediaPlayerApp()
{
	INT_t iRet;
	
	iRet = UnregDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

VOID QMediaPlayerApp::SigHandler(VOID * pCbContext, int SigNo)
{
	QMediaPlayerApp * pApp = (typeof(pApp))pCbContext;
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		if(pApp)
		{
			pApp->OnLinuxSignal(SigNo);
		}
	}
}

VOID QMediaPlayerApp::OnLinuxSignal(int SigNo)
{	
	LOG("(%d)Signal:%s\n", getpid(), CSignalInfo::GetSingalDescFromSigNo(SigNo));
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		quit();	//only post a message
	}
}

INT_t QMediaPlayerApp::setVolume(int VolumePercent)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		int CurVolPercent = 30;
		//iRet = m_MediaSrv_sp->ChangeVolume(0, &CurVolPercent);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
		//VolumePercent = 0;	//zero for debug
		int VolChange = VolumePercent - CurVolPercent;
		//iRet = m_MediaSrv_sp->ChangeVolume(VolChange, &CurVolPercent);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet = iRet;
			break;
		}
		m_VolumePercent = CurVolPercent;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::SaveVoMode(VIDEO_OUTPUT_MODE eNewVoMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	bool bCreateNodeIfNotExist = TRUE;

	{
		CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
		iRet = m_SettingXmlDoc_sp->setElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_VoMode, eNewVoMode, bCreateNodeIfNotExist);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		bRet = m_SettingXmlDoc_sp->SaveFile(MP_SETTING_XML_DOC_PATH);
		if(false == bRet)
		{
			LOG_BLINE("Save %s err\n", PropDesc_VoMode);
		}
	}
	//to disk
	sync();

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaPlayerApp::SaveDisplayParam(DISP_PARAM_TYPE eDispParamType, INT_t iValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	LPCSTR pszPropName = "";
	bool bCreateNodeIfNotExist = TRUE;

	do
	{
		pszPropName = getPropNameDescFromDispParamType(eDispParamType);
		if(NULL == pszPropName || 0 == strcmp(pszPropName, ""))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->setElementPropertyValue(XML_DispParam_Ele_PATH, pszPropName, iValue, bCreateNodeIfNotExist);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			bRet = m_SettingXmlDoc_sp->SaveFile(MP_SETTING_XML_DOC_PATH);
			if(false == bRet)
			{
				LOG_BLINE("Save %s err\n", pszPropName);
			}
			sync();
		}
	}while(FALSE);

	return iOutRet;
}

LPCSTR QMediaPlayerApp::getPropNameDescFromDispParamType(DISP_PARAM_TYPE eDispParamType)
{
	LPCSTR pszPropNameDesc = "";

	if(DispPARAM_Brightness == eDispParamType)
	{
		pszPropNameDesc = PropDesc_Brightness;
	}
	else if(DispPARAM_Contrast == eDispParamType)
	{
		pszPropNameDesc = PropDesc_Contrast;
	}
	else if(DispPARAM_Saturation == eDispParamType)
	{
		pszPropNameDesc = PropDesc_Saturation;
	}
	else if(DispPARAM_Hue == eDispParamType)
	{
		pszPropNameDesc = PropDesc_Hue;
	}
	else if(DispPARAM_ColorSpace == eDispParamType)
	{
		pszPropNameDesc = PropDesc_ColorSpace;
	}
	else if(DispPARAM_ColorDepth == eDispParamType)
	{
		pszPropNameDesc = PropDesc_ColorDepth;
	}
	else if(DispPARAM_matrixcoefficients == eDispParamType)
	{
		pszPropNameDesc = PropDesc_Matrix;
	}

	return pszPropNameDesc;
}

INT_t QMediaPlayerApp::applyDisplayParamsFromCfg(BOOL_t bContinueIfErr/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CString strValue;
	INT_t iValue = 50;

	if(Sw_LogVideoOutputInfo)
	{
		LOG_BLINE("applyDisplayParamsFromCfg\n");
	}

	do
	{
		//Brightness
		CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Brightness, OUT strValue, "50");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(DispParam_LowLimit_Percent > iValue)
			{
				iValue = DispParam_LowLimit_Percent;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_Brightness, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//Contrast
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Contrast, OUT strValue, "50");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(DispParam_LowLimit_Percent > iValue)
			{
				iValue = DispParam_LowLimit_Percent;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_Contrast, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//Saturation
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Saturation, OUT strValue, "50");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(DispParam_LowLimit_Percent > iValue)
			{
				iValue = DispParam_LowLimit_Percent;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_Saturation, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//Hue
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Hue, OUT strValue, "50");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(DispParam_LowLimit_Percent > iValue)
			{
				iValue = DispParam_LowLimit_Percent;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_Hue, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}

		//ColorSpace
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_ColorSpace, OUT strValue, "3");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(ColorSpace_Auto > iValue)
			{
				iValue = ColorSpace_Auto;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_ColorSpace, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}


		//ColorDepth
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_ColorDepth, OUT strValue, "1");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(ColorDepth_Auto > iValue)
			{
				iValue = ColorDepth_Auto;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_ColorDepth, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}

		//matrixcoefficients
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Matrix, OUT strValue, "1");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			GENERALIZE_PERCENT(iValue);
			if(Matrixcoefficients_Auto > iValue)
			{
				iValue = Matrixcoefficients_Auto;
			}
			iRet = m_MediaSrv_sp->setDisplayParam(DispPARAM_matrixcoefficients, iValue);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}	
	}while(FALSE);

	return iOutRet;
}


LPCSTR QMediaPlayerApp::getPropNameDescFromOsdPositionParamType(DISP_PARAM_TYPE eOsdPosParamType)
{
	LPCSTR pszPropNameDesc = "";

	if(QMedia_POSTION_ShiftX == eOsdPosParamType)
	{
		pszPropNameDesc = PropDesc_ShiftX;
	}
	else if(QMedia_POSTION_ShiftXSIZE == eOsdPosParamType)
	{
		pszPropNameDesc = PropDesc_shiftXSIZE;
	}
	else if(QMedia_POSTION_ShiftY == eOsdPosParamType)
	{
		pszPropNameDesc = PropDesc_ShiftY;
	}
	else if(QMedia_POSTION_ShiftYSIZE == eOsdPosParamType)
	{
		pszPropNameDesc = PropDesc_ShiftYSIZE;
	}

	return pszPropNameDesc;
}

INT_t QMediaPlayerApp::applyOsdPositionParamsFromCfg(BOOL_t bContinueIfErr/* = TRUE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CString strValue;
	INT_t iValue = 0;
	INT_t x = 0,y = 0,width = 0,height =4096;

	if(Sw_LogVideoOutputInfo)
	{
		LOG_BLINE("applyOsdPositionParamsFromCfg\n");
	}

	do
	{
		//ShiftHorizontal
		CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_OsdPositionParam_Ele_PATH, PropDesc_ShiftX, OUT strValue, "0");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			iRet = setOsdPosition(iValue,y,width,height,QMedia_POSTION_ShiftX);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//WidthAdjustment
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_OsdPositionParam_Ele_PATH, PropDesc_shiftXSIZE, OUT strValue, "4096");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			iRet = setOsdPosition(x,y,iValue,height,QMedia_POSTION_ShiftXSIZE);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//ShiftVertical
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_OsdPositionParam_Ele_PATH, PropDesc_ShiftY, OUT strValue, "0");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			iRet = setOsdPosition(x,iValue,width,height,QMedia_POSTION_ShiftY);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		//HeightAdjustment
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_OsdPositionParam_Ele_PATH, PropDesc_ShiftYSIZE, OUT strValue, "4096");
		if(ERROR_SUCCESS == iRet)
		{
			iValue = atoi((LPCSTR)strValue);
			iRet = setOsdPosition(x,y,width,iValue,QMedia_POSTION_ShiftYSIZE);
			if(ERROR_SUCCESS != iRet)
			{
				if(bContinueIfErr)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				else
				{
					iOutRet = iRet;
					break;
				}
			}
		}
		else
		{
			if(bContinueIfErr)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerApp::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;

	m_SettingXmlDoc_sp = SharedPtr <CTinyXmlDoc3> (new CTinyXmlDoc3);
	if(m_SettingXmlDoc_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	/*Add by xuweiwei 2014-9-17 for Set Serial Parameters*/
	do
	{
		DECLARE_CLS_STACK_BUF_STRING(strSerialParameters, 64);
		
		m_SerialComPort_sp = SharedPtr <QSerialComPort> (new QSerialComPort);
		if(m_SerialComPort_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_SerialComPort_sp->m_this_wp = m_SerialComPort_sp;

		iRet = SysProp_get(SysProp_KEY_SerialParameters, OUT strSerialParameters);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = m_SerialComPort_sp->SerialPortParamDesc_To_ParamValue(QString(strSerialParameters));
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{	
			if(ERROR_NOT_FOUND != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		/*Open the ttyS1*/
		iRet = m_SerialComPort_sp->Open(SerialComPort_DevPath);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}
		iRet = m_SerialComPort_sp->setComParameters();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}
	}while(FALSE);

	m_DBusConn_sp = SharedPtr <QDBusConnection> (new QDBusConnection(QDBUS_CONN_NAME_MediaPlayerSrv));
	if(m_DBusConn_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	*m_DBusConn_sp = QDBusConnection::systemBus();
	if(m_DBusConn_sp->isConnected())
	{
	}
	else	//connection failed
	{
		iOutRet = ERR_CONNECTION_FAIL;
		QDBusError QDBusErr = m_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to connect dbus\n");
		}
		goto EXIT_PROC;
	}

	bRet = m_DBusConn_sp->registerService(DBUS_MPLAYER_SERVICE_NAME);
	if(false == bRet)
	{
		iOutRet = ERR_REGISTER_FAIL;
		QDBusError QDBusErr = m_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_MPLAYER_SERVICE_NAME);
		}
		goto EXIT_PROC;
	}
	m_bMPlayerDbusServIsReged = TRUE;

	m_DbusIf_MediaPlayer_sp = SharedPtr <QDBusIf_MediaPlayer>(new QDBusIf_MediaPlayer(this));
	if(m_DbusIf_MediaPlayer_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	bRet = m_DBusConn_sp->registerObject(DBUS_MPLAYER_OBJECT_PATH, this);
	if(false == bRet)
	{
		iOutRet = ERR_REGISTER_FAIL;
		QDBusError QDBusErr = m_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to reg dbus obj %s\n", DBUS_MPLAYER_OBJECT_PATH);
		}
		goto EXIT_PROC;
	}

	m_MediaSrv_sp = SharedPtr <QMediaSrv> (new QMediaSrv);
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	m_MediaSrv_sp->m_this_wp = m_MediaSrv_sp;
	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigPlaybackEos(const quint32, const QString &, const bool)),
		this, SLOT(onMediaSrvPlaybackEos(const quint32, const QString &, const bool)),
		Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigPlaybackEos) err\n");
	}

#ifdef ENABLE_DTV
	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigNotifyTunerPlayerPropmtMsg(int)),
		this, SLOT(onNotifyTunerPlayerPropmtMsg(int)),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigNotifyTunerPlayerPropmtMsg) err\n");
	}

	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigNotifyScanProgress(qint32,qint32)),
		this, SLOT(onNotifyScanProgress(qint32,qint32)),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigNotifyScanProgress) err\n");
	}

	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigTuner_StartScan()),
		this, SLOT(onTuner_StartScan()),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigTuner_StartScan) err\n");
	}

	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigTuner_StopScan()),
		this, SLOT(onTuner_StopScan()),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigTuner_StopScan) err\n");
	}
#endif

	//Save Video Output mode and notify other processes
	bRet = connect(&(*m_MediaSrv_sp), SIGNAL(sigVidOutModeChangedEvent(int)),this,SLOT(onVidOutModeChanged(int)),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(sigVidOutModeChangedEvent) err\n");
	}

	//Save Volume
/*	bRet = connect(&m_VolumeSaveTimer,SIGNAL(timeout()),this,SLOT(OnSaveVolume()),Qt::QueuedConnection);
	if(false == bRet)
	{
		LOG_BLINE("connect(timeout) err\n");
	}*/

	bRet = m_SettingXmlDoc_sp->LoadFile(MP_SETTING_XML_DOC_PATH);
	if(true == bRet)
	{
		CString strValue;
		DECLARE_CLS_STACK_BUF_STRING(strDefValue, 32);
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_VOLUME_ELEMENT_PATH,
			AUD_ENG0_MASTER_NAME, OUT strValue, "30");
		if(ERROR_SUCCESS == iRet)
		{
			m_VolumePercent = atoi(strValue);
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//Video output mode
		iRet = strDefValue.Format("%d", VO_MODE_HDMI_720p50);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_VoMode, OUT strValue, strDefValue);
		if(ERROR_SUCCESS == iRet)
		{
			VIDEO_OUTPUT_MODE eVoMode = (typeof(eVoMode))atoi(strValue);
			m_MediaSrv_sp->m_eVoMode = eVoMode;
			//LOG_BLINE("strValue:%s\n", (LPCSTR)strValue);
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	else
	{
		LOG_BLINE("Failed to load %s\n", MP_SETTING_XML_DOC_PATH);
	}
	
	iRet = m_MediaSrv_sp->InitInstance();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	iRet = setVolume(m_VolumePercent);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	m_MediaSrv_sp->start();

	iRet = ChangeVidOutModeAsync(m_MediaSrv_sp->m_eVoMode);
	if(ERROR_NOT_SUPPORTED == iRet || ERROR_INVALID_PARAMETER == iRet)
	{
		m_MediaSrv_sp->m_eVoMode = VO_MODE_HDMI_720p59;
		iRet = ChangeVideoOutputMode(m_MediaSrv_sp->m_eVoMode);
	}
	if(ERROR_SUCCESS != iRet)
	{
		LOG_BLINE("iRet=%d,VoMode=%d\n", iRet, m_MediaSrv_sp->m_eVoMode);
	}
	
	iRet = SysProp_del(SysProp_KEY_NeedInitOsdForVcr);
	if(ERROR_SUCCESS != iRet)
	{
		if(ERROR_NOT_FOUND != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}

	/*Load and apply DispParam from the config file*/
	applyDisplayParamsFromCfg();

#if 1
	m_QVideoSync_sp = SharedPtr <QVideoSync> (new QVideoSync(m_MediaSrv_sp,this));
	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}	
	
	m_QVideoSync_sp->m_this_wp = m_QVideoSync_sp;
	
	iRet = m_QVideoSync_sp->InitInstance();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	//m_QVideoSync_sp->start();//start run
	
#endif

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;

	if(FALSE == m_MediaSrv_sp.isNull())
	{
		m_MediaSrv_sp->quit();
		iRet = m_MediaSrv_sp->ExitInstance();
		if(ERR_SUCCESS != iRet)
		{
			LOG_BLINE("MediaSrv.ExitInst() err %d\n", iRet);
		}
	}

	if(m_SerialComPort_sp.isValid())
	{
		m_SerialComPort_sp->Close();
	}

	if(FALSE == m_DBusConn_sp.isNull())
	{
		if(m_DBusConn_sp->isConnected())
		{
			m_DBusConn_sp->unregisterObject(DBUS_MPLAYER_OBJECT_PATH); 		
			if(m_bMPlayerDbusServIsReged)
			{
				bRet = m_DBusConn_sp->unregisterService(DBUS_MPLAYER_SERVICE_NAME);
				if(false == bRet)
				{
					LOG_BLINE("Failed to unreg dbus srv %s\n", DBUS_MPLAYER_SERVICE_NAME);
				}
				m_bMPlayerDbusServIsReged = FALSE;
			}
			QDBusConnection::disconnectFromBus(QDBUS_CONN_NAME_MediaPlayerSrv);
		}
	}

	m_DbusIf_MediaPlayer_sp.Clear();
	m_MediaSrv_sp.Clear();
	m_DBusConn_sp.Clear();

	return iOutRet;
}

SharedPtr <CTinyXmlDoc3> QMediaPlayerApp::getXmlCfgDoc()
{
	CAutoScopeLock autoRangeLocker(&m_SettingXmlDocSpMutex);
	return m_SettingXmlDoc_sp;
}

int QMediaPlayerApp::exec()
{
	int iProcessExitCode = ERROR_SUCCESS;
	INT_t iRet;

	m_hThread = QThread::currentThreadId ();

	iRet = InitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		iProcessExitCode = SS_PEC_INIT_INSTANCE_FAIL;
		goto EXIT_PROC;
	}
	
	iProcessExitCode = QApplication::exec();
	
EXIT_PROC:

	iRet = ExitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		iProcessExitCode = SS_PEC_EXIT_INSTANCE_FAIL;
		goto EXIT_PROC;
	}
	
	return iProcessExitCode;
}

int QMediaPlayerApp::setDataSource(const QString & strUrl)
{
	int iOutRet = ERR_SUCCESS, iRet;
/*
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setDataSource(strUrl);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
*/

	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_QVideoSync_sp->setDataSource(strUrl);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

int QMediaPlayerApp::Prepare()
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->Prepare();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

int QMediaPlayerApp::Play(qint32 iLoopCnt, quint32 & playbackId)
{
	int iOutRet = ERR_SUCCESS, iRet;
	QMediaSrv::PLAY_PARAMS playParams;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	QMediaSrv::PlayParams_Init(&playParams);
	playParams.LoopCount = iLoopCnt;
	iRet = m_MediaSrv_sp->Play(&playbackId, &playParams);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

int QMediaPlayerApp::ChangeVideoOutputMode(VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode /*= NULL*/,
	VIDEO_OUTPUT_MODE * peOldVoMode /*= NULL*/)
{
    int iOutRet = ERR_SUCCESS, iRet;
	
    if(m_MediaSrv_sp.isNull()){
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->ChangeVideoOutputMode(eVoMode, peNewVoMode, peOldVoMode);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::getDisplayOutputMode(qint32 & displayoutputMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
    if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->getDisplayOutputMode(displayoutputMode);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::setDisplayOutputMode(qint32 displayoutputMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
    if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->setDisplayOutputMode(displayoutputMode);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::getvideoColorSpace(qint32 & colorspace)
{
	int iOutRet = ERR_SUCCESS, iRet;
    if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->getvideoColorSpace(colorspace);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::setvideoColorSpace(qint32 colorspace)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->setvideoColorSpace(colorspace);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::setVideoPosition(int x, int y, int w, int h)
{
    int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setVideoPosition(x, y, w, h);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
    return iOutRet;
}


INT_t QMediaPlayerApp::setOsdSize(int w, int h)
{
    int iOutRet = ERR_SUCCESS, iRet;

    if(m_MediaSrv_sp.isNull())
    {
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }
    iRet = m_MediaSrv_sp->setOsdSize(w, h);
    if(ERR_SUCCESS != iRet)
    {
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}

int QMediaPlayerApp::ChangeVidOutModeAsync( VIDEO_OUTPUT_MODE eVoMode, VIDEO_OUTPUT_MODE * peNewVoMode /*= NULL*/,
	VIDEO_OUTPUT_MODE * peOldVoMode /*= NULL*/)
{
    int iOutRet = ERR_SUCCESS, iRet;

    if(m_MediaSrv_sp.isNull()){
        iOutRet = -ERR_INV_STATE;
        goto EXIT_PROC;
    }
	//LOG_LINE("[%s,%d]****===eVoMode=%d\n",__FILE__, __LINE__,eVoMode);
    iRet = m_MediaSrv_sp->ChangeVidOutModeAsync( eVoMode,peNewVoMode, peOldVoMode);
    if(ERR_SUCCESS != iRet) {
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
    return iOutRet;
}

INT_t QMediaPlayerApp::DispOnLedScrAsync(const QString & strContentToDisp, const qint32 & dispStartPos)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->DispOnLedScrAsync(strContentToDisp, dispStartPos);
    if(ERR_SUCCESS != iRet) {
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::scanI2cDev()
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->scanI2cDev();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setGpioLedStatus(const quint32 nGpioLedStatus, quint32 & nActualLedStatus)
{
	Q_UNUSED(nGpioLedStatus);

	nActualLedStatus = 0x00000000;
	
	return ERR_SUCCESS;
}

INT_t QMediaPlayerApp::setGpioOutputVal(const int iGpioNum, const bool bVal)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setGpioOutputVal(iGpioNum, bVal);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setPlayMode(const int iPlayMode)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setPlayMode(iPlayMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
	{ /*sync*/
		if(m_QVideoSync_sp->m_eVideoSyncMode == VidSyncMode_MASTER)
		{
			if(MediaPlayer::PlayMode_Normal != iPlayMode)
			{
				LOG_BLINE("Stop Sync !\n");
				if(m_QVideoSync_sp->m_SyncTimer_sp->isActive())
				{
					/*stop sync*/
					m_QVideoSync_sp->m_SyncTimer_sp->stop();
					//m_QVideoSync_sp->m_TestTimer_sp->stop();
				}
			}
			else
			{
				LOG_BLINE("Start Sync !\n");
				m_QVideoSync_sp->m_SyncTimer_sp->start();
				//m_QVideoSync_sp->m_TestTimer_sp->start(20);
			}
		}
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getPlayMode(OUT INT_t & iPlayMode)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getPlayMode(OUT iPlayMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setHdmi3DTvMode(const int iHdmi3DTvMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	CONST HDMI_3D_TV_MODE eHdmi3DTvMode = (typeof(eHdmi3DTvMode))iHdmi3DTvMode;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setHdmi3DTvMode(eHdmi3DTvMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getMonitorInfo(OUT QString & strMonitorInfo)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getMonitorInfo(OUT strMonitorInfo);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setVideoDispAspectRatio(const qint32 iVidAspectRatio, const quint32 uOptFlags)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setVideoDispAspectRatio(iVidAspectRatio, uOptFlags);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getVideoDispAspectRatio(OUT qint32 & iVidDispAspectRatio)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getVideoDispAspectRatio(OUT iVidDispAspectRatio);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getShowClosedCaption(qint32 &iCcValue)
{

	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getShowClosedCaption(OUT iCcValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setShowClosedCaption(qint32 iCcValue)
{

	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setShowClosedCaption(iCcValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

//
INT_t QMediaPlayerApp::setPlaySpeed(qint32 iPlaySpeed)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setPlaySpeed(iPlaySpeed);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getPlaySpeed(OUT qint32 & iPlaySpeed)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getPlaySpeed(OUT iPlaySpeed);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}
//

//add
INT_t QMediaPlayerApp::MibsetMonitorInfo(qint32 MonitorType)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_MediaSrv_sp->MibsetMonitorInfo(MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::MibgetMonitorInfo(OUT qint32 & MonitorType)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->MibgetMonitorInfo(OUT MonitorType);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}
//
INT_t QMediaPlayerApp::getSerialControlData(QString& strSerialControlData)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CByteArray2 receivedSerialData;
	
	do
	{
		iRet = m_SerialComPort_sp->ReadResult(OUT receivedSerialData);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = receivedSerialData.Add('\0');
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
		}

		strSerialControlData =((LPCSTR)(receivedSerialData.GetData()));
			
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::setSerialControlData(const QString & strSerialControlData)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	do
	{
		iRet = m_SerialComPort_sp->SendCommand(strSerialControlData);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::getSerialParameters(OUT QString & strSerialParameters)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_SerialComPort_sp->SerialPortParmVal_To_Desc(OUT strSerialParameters);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
	}

	return iOutRet;
}

INT_t QMediaPlayerApp::setSerialParameters(const QString & strSerialControlData)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QString istrSerialControlData=strSerialControlData;
	
	do
	{
		if(1 > istrSerialControlData.length())
		{
			iOutRet=ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = m_SerialComPort_sp->SerialPortParamDesc_To_ParamValue(istrSerialControlData);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//Save the setting string to persistent storage
		char*  chSerialControlData;
		QByteArray baSerialControlData = istrSerialControlData.toLatin1();
		chSerialControlData=baSerialControlData.data();
		iRet = SysProp_setPersistent(SysProp_KEY_SerialParameters,chSerialControlData);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//set parameters on port
		//set baudrate, etc.
		iRet = m_SerialComPort_sp->setComParameters();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::getDisplayParam(const qint32 iDispParam,OUT qint32 &oValue)
{
	int iOutRet = ERR_SUCCESS, iRet;
	CString strValue;
	DISP_PARAM_TYPE eDispParamType;
	eDispParamType=(DISP_PARAM_TYPE)(iDispParam);

	if(eDispParamType < DispPARAM_Brightness || eDispParamType > DispPARAM_matrixcoefficients)
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }
	
	
	switch(iDispParam)
	{
	case DispPARAM_Brightness:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Brightness, OUT strValue, "50");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}
			goto EXIT_PROC;
		}

	case DispPARAM_Contrast:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Contrast, OUT strValue, "50");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}
			goto EXIT_PROC;
		}

	case DispPARAM_Saturation:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Saturation, OUT strValue, "50");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}
			goto EXIT_PROC;
		}

	case DispPARAM_Hue:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Hue, OUT strValue, "50");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}
			goto EXIT_PROC;
		}

	case DispPARAM_ColorSpace:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_ColorSpace, OUT strValue, "0");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}

			iRet = m_MediaSrv_sp->getDisplayParam(iDispParam,oValue);
		    if(ERR_SUCCESS != iRet) 
			{
		        iOutRet = iRet;
		    }
			goto EXIT_PROC;
		}
	case DispPARAM_ColorDepth:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_ColorDepth, OUT strValue, "0");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}
			
			iRet = m_MediaSrv_sp->getDisplayParam(iDispParam,oValue);
		    if(ERR_SUCCESS != iRet) 
			{
		        iOutRet = iRet;
		    }
			goto EXIT_PROC;
		}
	case DispPARAM_matrixcoefficients:
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_DispParam_Ele_PATH, PropDesc_Matrix, OUT strValue, "1");
			if(ERROR_SUCCESS == iRet)
			{
				oValue = atoi((LPCSTR)strValue);
			}
			else
			{
				iOutRet = iRet;
			}

			iRet = m_MediaSrv_sp->getDisplayParam(iDispParam,oValue);
		    if(ERR_SUCCESS != iRet) 
			{
		        iOutRet = iRet;
		    }
			goto EXIT_PROC;
		}
	default:
		break;
	}

EXIT_PROC:
	return iOutRet;
	
}

INT_t QMediaPlayerApp::setDisplayParam(qint32 iDispParam,qint32 iValue,qint32 bSave)
{
	int iOutRet = ERR_SUCCESS, iRet;
	DISP_PARAM_TYPE eDispParamType;
	eDispParamType=(DISP_PARAM_TYPE)(iDispParam);

	if(eDispParamType < DispPARAM_Brightness || eDispParamType > DispPARAM_matrixcoefficients)
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	iRet = m_MediaSrv_sp->setDisplayParam(eDispParamType, iValue);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	if(bSave)
	{
		iRet = SaveDisplayParam(eDispParamType, iValue);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iOutRet=iRet;
			goto EXIT_PROC;
		}
	}


EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getOutputSpdifMode(qint32 &oSpdifMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
    if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }
	
    iRet = m_MediaSrv_sp->getOutputSpdifMode(oSpdifMode);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::setOutputSpdifMode(qint32 iSpdifMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
    if(m_MediaSrv_sp.isNull())
	{
        iOutRet = ERR_INV_STATE;
        goto EXIT_PROC;
    }

    iRet = m_MediaSrv_sp->setOutputSpdifMode(iSpdifMode);
    if(ERR_SUCCESS != iRet) 
	{
        iOutRet = iRet;
        goto EXIT_PROC;
    }

EXIT_PROC:

    return iOutRet;
}

INT_t QMediaPlayerApp::getTimeZoneSetting(OUT qint32 & TimezoneHourOff)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	time_t timeUTC, timeLocal;
	struct tm tmLocal;
	struct tm * ptmRet = NULL;

	do
	{
		
		/*system("echo TZ=${TZ}");
		system("date");
		system("date -u");*/
		
		timeUTC = time(NULL);
		if(-1 == timeUTC)	//error
		{
			iOutRet = ERR_GET_SYS_TIME_FAIL;
			break;
		}

		/*LOG_BLINE("timeUTC=%d\n", timeUTC);*/
		
		ptmRet = localtime_r(&timeUTC, &tmLocal);
		if(NULL == ptmRet)
		{
			iOutRet = ERR_GET_SYS_TIME_FAIL;
			break;
		}
		
		timeLocal = timegm(&tmLocal);
		/*LOG_BLINE("timeLocal=%d\n", timeLocal);*/
		
		TimezoneHourOff = (timeLocal - timeUTC) / (1*60*60);
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::setTimeZoneSetting(qint32 TimezoneHourOff)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	do
	{
		//set system global timezone settings
		iRet = CSystemTimezone::setSystemGlobalTimeZone(TimezoneHourOff);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		tzset();
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerApp::setHdmiAudioOutput(qint32 bHdmiAudioOutput)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setHdmiAudioOutput(bHdmiAudioOutput);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;

}

INT_t QMediaPlayerApp::setAnalogAudioMute(qint32 bAnalogAudioMute)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setAnalogAudioMute(bAnalogAudioMute);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


INT_t QMediaPlayerApp::setConfigDateTime(qint32 DateTime)
{
	int iOutRet = ERR_SUCCESS, iRet;
	int CurClkTicks;
	struct timeval tv;
	struct tm tmRtcTimeToSet, *ptmRet;
	time_t RelTimeSec;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = gettimeofday(&tv, NULL);
	if(0 != iRet)
	{
		iOutRet = ERR_GET_SYS_TIME_FAIL;
		goto EXIT_PROC;
	}
	
	CurClkTicks = tv.tv_sec;
	RelTimeSec = CurClkTicks;
	ZeroMemory(&tmRtcTimeToSet, sizeof(tmRtcTimeToSet));
	ptmRet = gmtime_r(&RelTimeSec, &tmRtcTimeToSet);

	if(NULL == ptmRet)
	{
		iOutRet = ERROR_CALL_SYSTEM_FAILED;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->setHwRtcTime(tmRtcTimeToSet.tm_year,tmRtcTimeToSet.tm_mon + 1,tmRtcTimeToSet.tm_mday,\
		tmRtcTimeToSet.tm_hour,tmRtcTimeToSet.tm_min,tmRtcTimeToSet.tm_sec,0);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}


INT_t QMediaPlayerApp::getConfigDateTime(QString & ConfigDateTime)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getHwRtcTime(ConfigDateTime);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:

	return iOutRet;
}

INT_t QMediaPlayerApp::setCurClkTicks(qint32 ClkTicks)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int CurClkTicks;
	struct timeval tv;
	struct tm tmRtcTimeToSet, *ptmRet;
	time_t RelTimeSec;
	//add by bert 2016/08/26
	struct tm tmStart, tmEnd;
	time_t tS, tE;
	
	tmStart = {0,0,0,1,1-1,2000-1900,0,0,0};
	tS = mktime(&tmStart);

	tmEnd = {0,0,0,1,1-1,2038-1900,0,0,0};
	tE = mktime(&tmEnd);
	if (tS>ClkTicks || tE<ClkTicks)
	{
		LOG_BLINE("the Date you set from 2000-01-01 to 2038-01-01, and take care of TimeZone\n");
		goto EXIT_PROC;
	}

	CurClkTicks = ClkTicks;
	tv.tv_sec = CurClkTicks;
	tv.tv_usec = 0;
	iRet = settimeofday(&tv, NULL);
	if(0 != iRet)
	{
		iOutRet = ERR_SET_SYS_TIME_FAIL;
		goto EXIT_PROC;
	}
	//set hw rtc time, tmRtcTimeToSet is UTC time
	RelTimeSec = CurClkTicks;
	ZeroMemory(&tmRtcTimeToSet, sizeof(tmRtcTimeToSet));
	
	ptmRet = gmtime_r(&RelTimeSec, &tmRtcTimeToSet);
	if(NULL == ptmRet)
	{
		iOutRet = ERROR_CALL_SYSTEM_FAILED;
		goto EXIT_PROC;
	}
	//modify by bert 2016/08/23
	iRet = m_MediaSrv_sp->setHwRtcTime(tmRtcTimeToSet.tm_year+1900,tmRtcTimeToSet.tm_mon + 1,tmRtcTimeToSet.tm_mday,\
	tmRtcTimeToSet.tm_hour,tmRtcTimeToSet.tm_min,tmRtcTimeToSet.tm_sec,0);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getCurClkTicks(OUT qint32& ClkTicks)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	struct timeval tv;
	
	iRet = gettimeofday(&tv, NULL);
	if(0 != iRet)
	{
		iOutRet = ERR_GET_SYS_TIME_FAIL;
		goto EXIT_PROC;
	}
	ClkTicks = tv.tv_sec;

EXIT_PROC:
	return iOutRet;
}


INT_t QMediaPlayerApp::setHdmiCecCtrl(qint32 HdmiCecCtrl)
{
	int iOutRet = ERR_SUCCESS, iRet;
	HDMI_CEC_CTRL_CMD eHdmiCecCtrlCmd = (typeof(eHdmiCecCtrlCmd))(HdmiCecCtrl);
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setHdmiCecCtrl(eHdmiCecCtrlCmd);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

#ifdef ENABLE_DTV
INT_t QMediaPlayerApp::GetEntryChannelInfo(qint32 EntryId,qint32& pChannelNumber,qint32& pSubChannelCount,QString& StrProgramPids)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	INT32_t channelNumer = 0,subChannelCount = 0;
	DECLARE_CLS_STACK_BUF_STRING(strProgramPids, 64);
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->GetEntryChannelInfo(EntryId,&channelNumer,&subChannelCount,strProgramPids);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	pChannelNumber=channelNumer;
	pSubChannelCount=subChannelCount;
	
	StrProgramPids=QString(strProgramPids);

EXIT_PROC:
	return iOutRet;
	
}

INT_t QMediaPlayerApp::PlayTuner(quint32 & playbackId)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->PlayTuner(&playbackId);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::TunerScan()
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	{
		INT_t iCurrPlayStatus= PLAY_STAT_IDLE;
		iRet = getMpSrvPlayStatus(&iCurrPlayStatus);
		if(ERROR_SUCCESS != iRet) 
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}

		if(PLAY_STAT_TUNER_SCANNING == iCurrPlayStatus)
		{
			iOutRet = ERROR_INVALID_STATE;
			goto EXIT_PROC;
		}
	}

	iRet = m_MediaSrv_sp->TunerScan();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getTunerChannelMode(quint32 & TunerChannleMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getTunerChannelMode(&TunerChannleMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setTunerChannelMode(qint32 TunerChannleMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setTunerChannelMode(TunerChannleMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::TunerMode(qint32 iStdMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->TunerMode(iStdMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

INT_t QMediaPlayerApp::GetTunerMode(qint32& iStdMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->GetTunerMode(&iStdMode);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

INT_t QMediaPlayerApp::GetFirstChNumber(quint32 &OMajor,quint32 &OMinor)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->GetFirstChNumber(&OMajor,&OMinor);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::GetForceTxMode(qint32 & iTxMode)
{

	int iOutRet = ERR_SUCCESS, iRet;
	int iEnableTuner = 1;

	iTxMode=iEnableTuner;

EXIT_PROC:
	return iOutRet;

}


INT_t QMediaPlayerApp::GetTunerChannelStrength(quint32& Strength)
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->GetTunerChannelStrength(&Strength);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::ResetDuration()
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->ResetDuration();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::ChannelUp()
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->ChannelUp();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::ChannelDown()
{
	int iOutRet = ERR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->ChannelDown();
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}
#endif

INT_t QMediaPlayerApp::setHwRtcTime(qint32 Year, qint32 Month, qint32 Date, qint32 Hour, qint32 Minute, qint32 Second, int pszTimeZone)
{
	int iOutRet = ERR_SUCCESS, iRet;

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setHwRtcTime(Year,Month,Date,Hour,Minute,Second,pszTimeZone);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

INT_t QMediaPlayerApp:: getDisplayPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height)
{
	int iOutRet = ERR_SUCCESS, iRet;

	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getDisplayPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

INT_t QMediaPlayerApp:: getVideoPosition(qint32 eRuaDispType,qint32 & x,qint32 & y,qint32 & width,qint32 & height)
{
	int iOutRet = ERR_SUCCESS, iRet;

	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->getVideoPosition(eRuaDispType,x,y,width,height);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}


INT_t QMediaPlayerApp:: setOsdPosition(qint32 x,qint32 y,qint32 width,qint32 height,qint32 type)
{
	int iOutRet = ERR_SUCCESS, iRet,iValue;

	DISP_PARAM_TYPE eOsdPosParamType;
	eOsdPosParamType=(DISP_PARAM_TYPE)(type);

	if(eOsdPosParamType < QMedia_POSTION_ShiftX || eOsdPosParamType > QMedia_POSTION_ShiftYSIZE)
	{
		iOutRet=ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}
	
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->setOsdPosition(x,y,width,height,type);
	if(ERR_SUCCESS != iRet) 
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;

}

INT_t QMediaPlayerApp::getCurrentVolume(qint32 & CurVolume)
{
	int iOutRet = ERR_SUCCESS;
	CString strValue;
	CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
	iOutRet = m_SettingXmlDoc_sp->getElementPropertyValue(XML_VOLUME_ELEMENT_PATH, AUD_ENG0_MASTER_NAME, OUT strValue, "50");
	if(ERROR_SUCCESS == iOutRet)
	{
		CurVolume = atoi((LPCSTR)strValue);
		//LOG_BLINE("CurVolume = %d\n",CurVolume);
	}
	return iOutRet;
		
}

INT_t QMediaPlayerApp::SaveOsdPositionParam(DISP_PARAM_TYPE eOsdPosParamType, INT_t iValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	LPCSTR pszPropName = "";
	bool bCreateNodeIfNotExist = TRUE;

	do
	{
		pszPropName = getPropNameDescFromOsdPositionParamType(eOsdPosParamType);
		if(NULL == pszPropName || 0 == strcmp(pszPropName, ""))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->setElementPropertyValue(XML_OsdPositionParam_Ele_PATH, pszPropName, iValue, bCreateNodeIfNotExist);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			
			bRet = m_SettingXmlDoc_sp->SaveFile(MP_SETTING_XML_DOC_PATH);
			if(false == bRet)
			{
				LOG_BLINE("Save %s err\n", pszPropName);
			}
			sync();
		}
	}while(FALSE);

	return iOutRet;
}


int QMediaPlayerApp::Stop(quint32 & playbackId, QString & strUrl)
{
	int iOutRet = ERR_SUCCESS, iRet;

	//LOG_BLINE("Stop\n");

	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->Stop(&playbackId, &strUrl);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

void QMediaPlayerApp::onMediaSrvPlaybackEos(const quint32 playbackId, const QString & strPlaybackUrl, 
	const bool bIsCompletePlaybackEos)
{
	if(QThread::currentThreadId() != m_hThread)
	{
		LOG_BLINE("inv calling thread ctx,caller:0x%08x,handler:0x%08x\n", QThread::currentThreadId(), m_hThread);
		goto EXIT_PROC;
	}

	emit PlaybackEos(strPlaybackUrl, playbackId);
	emit PlaybackEos2(strPlaybackUrl, playbackId, bIsCompletePlaybackEos);

	{
		if(m_QVideoSync_sp->m_SyncTimer_sp->isActive())
		{
			/*stop sync*/
			m_QVideoSync_sp->m_SyncTimer_sp->stop();
		}
	}

EXIT_PROC:
	;
}

void QMediaPlayerApp::onVidOutModeChanged(const int iCurVoMode)
{
	//notify other processes
	emit VidOutModeChanged(iCurVoMode);
	
	onSaveMode(iCurVoMode);
}

void QMediaPlayerApp::onSaveMode(int eNewVoMode)
{
	int iOutRet = ERR_SUCCESS, iRet;
	
	VIDEO_OUTPUT_MODE iVoMode =(typeof(iVoMode))(eNewVoMode);
	iRet = SaveVoMode(iVoMode);
	if(ERR_SUCCESS != iRet)
	{
		LOG_BLINE("Save output mode:%d err %d\n",iVoMode, iRet);
		iOutRet = iRet;
		goto EXIT_PROC;
	}

EXIT_PROC:
	;		
}

INT_t QMediaPlayerApp::OnSaveVolume(qint32 iVolume)
{
	int 	iOutRet = ERR_SUCCESS, iRet;

	bool bRet;
	bool bCreateNodeIfNotExist = TRUE;

	/*if (m_VolumeSaveTimer.isActive())
		m_VolumeSaveTimer.stop();*/

	do
	{
		{
			CAutoScopeLock autoRangeLocker(&(m_SettingXmlDoc_sp->m_Mutex));
			iRet = m_SettingXmlDoc_sp->setElementPropertyValue(XML_VOLUME_ELEMENT_PATH, AUD_ENG0_MASTER_NAME, iVolume,
				bCreateNodeIfNotExist);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}
			bRet = m_SettingXmlDoc_sp->SaveFile(MP_SETTING_XML_DOC_PATH);
			if(false == bRet)
			{
				LOG_BLINE("Save vol err\n");
			}
			//LOG_BLINE("Save vol = %d success!!\n",iVolume);
		}
		sync();
	}while(FALSE);

	return iOutRet;
}

#ifdef ENABLE_DTV
void QMediaPlayerApp::onNotifyTunerPlayerPropmtMsg(int MsgTag)
{
	if(QThread::currentThreadId() != m_hThread)
	{
		LOG_BLINE("inv calling thread ctx,caller:0x%08x,handler:0x%08x\n", QThread::currentThreadId(), m_hThread);
		goto EXIT_PROC;
	}
	
	emit NotifyTuPlayerPropmtMsg(MsgTag);

EXIT_PROC:
	;
}

void QMediaPlayerApp::onNotifyScanProgress(qint32 PgmNum,qint32 Progress)
{
	if(QThread::currentThreadId() != m_hThread)
	{
		LOG_BLINE("inv calling thread ctx,caller:0x%08x,handler:0x%08x\n", QThread::currentThreadId(), m_hThread);
		goto EXIT_PROC;
	}
	
	emit NotifyScanProgress(PgmNum,Progress);

EXIT_PROC:
	;
}

void QMediaPlayerApp::onTuner_StartScan()
{
	qint32 startid=0x10;
	if(QThread::currentThreadId() != m_hThread)
	{
		LOG_BLINE("inv calling thread ctx,caller:0x%08x,handler:0x%08x\n", QThread::currentThreadId(), m_hThread);
		goto EXIT_PROC;
	}
	
	emit Tuner_StartScan(startid);

EXIT_PROC:
	;
}

void QMediaPlayerApp::onTuner_StopScan()
{
	qint32 stopid=0x11;
	
	if(QThread::currentThreadId() != m_hThread)
	{
		LOG_BLINE("inv calling thread ctx,caller:0x%08x,handler:0x%08x\n", QThread::currentThreadId(), m_hThread);
		goto EXIT_PROC;
	}
	
	emit Tuner_StopScan(stopid);

EXIT_PROC:
	;
}
#endif

int QMediaPlayerApp::ChangeVolume(int VolumePercentToChange)
{
	int 	iOutRet = ERR_SUCCESS, iRet;
	int 	iNewVolumePercent = 0;
	bool 	ibVolChanged = FALSE;
	
	//LOG_BLINE("ChangeVolume\n");
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_MediaSrv_sp->ChangeVolume(VolumePercentToChange);
	if(ERR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	if(ibVolChanged)
	{
		/*m_VolumeSaveTimer.setSingleShot(TRUE);
		    m_VolumeSaveTimer.start(1*1000);*/
		//OnSaveVolume();
	}

EXIT_PROC:
	return iOutRet;
	
}


INT_t QMediaPlayerApp::getVideoSyncMode(OUT qint32 &SyncMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_QVideoSync_sp->getVideoSyncMode(SyncMode);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setVideoSyncMode(VIDEO_SYNC_MODE_t SyncMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_QVideoSync_sp->setVideoSyncMode(SyncMode);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getVideoSyncMaster(OUT QString &strMasterAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}	

	iRet = m_QVideoSync_sp->getVideoSyncMaster(strMasterAddr);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_GETADDRINFO_FAIL;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setVideoSyncMaster(const QString &strMasterAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_QVideoSync_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}

	iRet = m_QVideoSync_sp->setVideoSyncMaster(strMasterAddr);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getCurrentTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->getCurrentTime(uTimestamp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::getLastTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->getLastTime(uTimestamp);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setSyncTime(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->setSyncTime(Offset);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::setSyncSeek(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->setSyncSeek(Offset);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

INT_t QMediaPlayerApp::Seek(const unsigned int & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	if(m_MediaSrv_sp.isNull())
	{
		iOutRet = ERR_INV_STATE;
		goto EXIT_PROC;
	}
	
	iRet = m_MediaSrv_sp->Seek(Offset);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = ERR_NOT_FOUND;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

