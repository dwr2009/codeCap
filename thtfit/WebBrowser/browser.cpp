#include "browser.h"
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include "WebAppInfoDef.h"
#include <QtWidgets/QApplication>
#include <ErrPrintHelper.h>
#include <QPaintEngine>
#include <BaseErrDef.h>
#include "IrKeyQtKeyMap.h"
#include "WebBrowserApp.h"
#include <DbgLogSwitchDef.h>

#include<stdio.h>
#include<sys/time.h>

#include<unistd.h>


browser::browser(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	//ui.webView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

browser::~browser()
{

}

/*
* init system load main_menu
*/
void browser::initSys()
{
	bool bRet;
	ui.webView->setContextMenuPolicy (Qt::NoContextMenu);

	//QtWebPlugin is not affected by QWebSettings::PluginsEnabled.
	ui.webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
	ui.webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
	//No need to open a new web window
	ui.webView->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
	//Currently Java applets are not supported.
	ui.webView->settings()->setAttribute(QWebSettings::JavaEnabled, false);

	bRet = connect(ui.webView->page()->mainFrame(),
		SIGNAL(javaScriptWindowObjectCleared ()), this, SLOT(addJavaScriptObject()));
	if(false == bRet)
	{
		LOG_BLINE("ConnectSig err\n");
	}
	ui.webView->page()->setPluginFactory(&m_WebPluginFactory);

	QString html="file://" + APP_DIR_PATH + OSD_PATH;

#if 1/*Add by xuweiwei 2014-8-2 for Set background to black*/
	this->setStyleSheet("background-color:black");
#endif
	
	ui.webView->load(html);
	//LOG("Url:%s\n", qPrintable(html));
	
	/*When load html complete,Set to original(white) Color*/
	this->setStyleSheet("background-color:white");
	//setWindowOpacity(0.6);
	//QWidgetEx::AllWidgets_setWindowOpacity(ui.webView, 0.6);	//setWindowOpacity is not applied to sub widget
}

/*
Thread context: main thread
*/
void browser::addJavaScriptObject()
{
	do
	{
		QWebPage * pWebPage = ui.webView->page();
		if(CC_UNLIKELY(NULL == pWebPage))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		QWebFrame * pWebFrame = pWebPage->mainFrame();
		if(CC_UNLIKELY(NULL == pWebFrame))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		pWebFrame->addToJavaScriptWindowObject("browser", this);
		pWebFrame->addToJavaScriptWindowObject("WebBrowserApp", qApp);
		//window.ControlCenter
		pWebFrame->addToJavaScriptWindowObject("ControlCenter", qApp);
	}while(FALSE);
}

/*
* slot by  Js call
* then slot call js
*/
QStringList browser::GetInformationMsg()
{
	QStringList infoList;
	QStringList monitorTypeList;
	QStringList versionInfoList;

	QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
	pQWebBrowserApp->GetVersionInfo(versionInfoList);
	
	emit GetMonitorTypeInfo_Sig(monitorTypeList);	
	if(monitorTypeList.size() < 1)
	{
		monitorTypeList<<QString("undefined");
	}

	infoList << monitorTypeList << versionInfoList << QString("1.0.1 (415-M)");

	return infoList;
}

INT_t browser::CalcVideoPluginPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		QVariant varPluginPosition = pWebFrame->evaluateJavaScript("CalcVideoPosition();");
		if(FALSE == varPluginPosition.isValid())
		{
			//maybe a plugin has not been created.
			//iOutRet = ERR_JSCRIPT_ERR;
			//LOG_BLINE("invalid\n");
			break;
		}
		QList<QVariant> RetValList = varPluginPosition.toList();
		if(4 > RetValList.count())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			LOG_BLINE("count=%d\n", RetValList.count());
			break;
		}
		QVariant varX = RetValList.at(0);
		QVariant varY = RetValList.at(1);
		QVariant varWidth = RetValList.at(2);
		QVariant varHeight = RetValList.at(3);
		x = varX.toInt();
		y = varY.toInt();
		w = varWidth.toInt();
		h = varHeight.toInt();
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2012-8-3 for refresh video mode*/
void browser::RefreshUiVideoMode(void)
{
	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}
		QVariant varRet = pWebFrame->evaluateJavaScript("get_config_value();");
	}while(FALSE);
}
#endif

/*
* load play list
*/
QStringList browser::getPlayListFiles(int reqListType)
{
	QStringList  fileNamelist ,filePathInfoList;
	emit getPlayListFiles_signal(reqListType,fileNamelist,filePathInfoList);
	m_strFilePathInfoList  = filePathInfoList;
	return fileNamelist;
}

/*
* load play list file info
*/
QStringList browser::GetPlayListFileInfo()
{
	return m_strFilePathInfoList;
}


void browser::playSelectFile(const QString selectFileName,int playListType)
{
	//LOG_BLINE("%s\n", qPrintable(selectFileName));
	emit playSelectFile_signal(selectFileName,playListType);
}

void  browser::SwitchToPlayWin()
{
	emit SwitchWin_signal();
}

QStringList browser::GetDhcpInfo(int isGetRealTimeNetInfo)
{
	//INT_t iOutRet = ERROR_SUCCESS;
	QStringList dhcpInfoList;
	if(isGetRealTimeNetInfo)
	{
		emit reqNetWorkInfo_signal(dhcpInfoList);
	}
	else
	{
		emit reqDhcpInfo_signal(dhcpInfoList);
	}

	return dhcpInfoList;
}


void browser::turnVideoSetting(const QString videoParType, int videoValue)
{

	int iRet = -1;
	QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
	if(videoParType == "PlayerVolume" )
	{
		pQWebBrowserApp->setPlayVolume(videoValue,iRet);
	}
	else if(videoParType == "Brightness" )
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_Brightness,videoValue,iRet);
	}
	else if(videoParType == "Contrast" )
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_Contrast,videoValue,iRet);
	}
	else if(videoParType == "Color" )
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_Saturation,videoValue,iRet);
	}
	else if(videoParType == "Tint" )
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_Hue,videoValue,iRet);
	}
	else if(videoParType == "ColorSpace")
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_ColorSpace,videoValue,iRet);
	}
	else if(videoParType == "ColorDepth")
	{
		pQWebBrowserApp->setDisplayParam( DispPARAM_ColorDepth,videoValue,iRet);	
	}
	else if(videoParType == "OffsetX" )
	{
		{
			int x,y,width,height;
			x = y = 0;
			width = height = 4096;
			pQWebBrowserApp->getDisplayPosition(RUA_DISP_OSD,x,y,width,height);
			videoValue  = (( (videoValue - 50)) *10);
			if(videoValue != x)
			{
				pQWebBrowserApp->setOsdPosition(videoValue,y,width,height,AMedia_POSTION_ShiftX);
			}
		}
	}
	else if(videoParType == "OffsetWidth" )
	{
		int x,y,width,height,dwidth,dheight;
		x = y = 0;
		width = height = 4096;
		pQWebBrowserApp->getDisplayPosition(RUA_DISP_OSD,x,y,width,height);
		videoValue  = (( (videoValue - 50)) *10);
		pQWebBrowserApp->getDefaultDisplayParam(dwidth,dheight);
		pQWebBrowserApp->setOsdPosition(x,y,dwidth+ videoValue,height,AMedia_POSTION_ShiftXSIZE);
	}
	else if(videoParType == "OffsetY" )
	{
		int x,y,width,height;
		x = y = 0;
		width = height = 4096;

		pQWebBrowserApp->getDisplayPosition(RUA_DISP_OSD,x,y,width,height);
		{
			videoValue  = (( (videoValue - 50)) *10);
			pQWebBrowserApp->setOsdPosition(x,videoValue,width,height,AMedia_POSTION_ShiftY);
		}
	}
	else if(videoParType == "OffsetHeight" )
	{
		int x,y,width,height,dwidth,dheight;
		x = y = 0;
		width = height = 4096;
		pQWebBrowserApp->getDefaultDisplayParam(dwidth,dheight);
		pQWebBrowserApp->getDisplayPosition(RUA_DISP_OSD,x,y,width,height);
		//videoValue  = (videoValue - 50 / 100 *1000);
		videoValue  = (( (videoValue - 50)) *10);
		pQWebBrowserApp->setOsdPosition(x,y,width,dheight+ videoValue,AMedia_POSTION_ShiftYSIZE);
	}


}

//add
INT_t browser::MibsetSysTime()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
	if(NULL == pWebFrame)
	{
		iOutRet = ERROR_INVALID_STATE;
	}
	pWebFrame->evaluateJavaScript("MibsetSysTime();");
	return iOutRet;
}


INT_t browser::getTimeFuc()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
	if(NULL == pWebFrame)
	{
		iOutRet = ERROR_INVALID_STATE;
	}
	pWebFrame->evaluateJavaScript("getTimeFuc();");
	return iOutRet;
}

INT_t browser::getShitfPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		QVariant varPluginPosition = pWebFrame->evaluateJavaScript("CalcShiftPosition();");
		if(FALSE == varPluginPosition.isValid())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			if(Sw_LogJavascript)
			{
				LOG_BLINE("CalcShiftPosition invalid\n");
			}
			
			break;
		}
		QList<QVariant> RetValList = varPluginPosition.toList();
		if(4 > RetValList.count())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			if(Sw_LogJavascript)
			{
				LOG_BLINE("count=%d\n", RetValList.count());
			}			
			break;
		}
		QVariant varX = RetValList.at(0);
		QVariant varY = RetValList.at(1);
		QVariant varWidth = RetValList.at(2);
		QVariant varHeight = RetValList.at(3);
		x = varX.toInt();
		y = varY.toInt();
		w = varWidth.toInt();
		h = varHeight.toInt();

	}while(false);

	return iOutRet;
}

void browser::SaveDhcpInfo(QStringList tempDhcpInfo,bool isAutoGetIp)
{
	emit saveDhcpInfo_signal(tempDhcpInfo,isAutoGetIp);
}
QStringList browser::GetSystemTime()
{
	QString timezonenum;
	QStringList strTimeList;
	QString currentDate;

	QDateTime dt;
	QTime time;
	QDate date;

	int currentTimeZone = 8 ;
	do
	{
        QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
        if(!pQWebBrowserApp) break;
		pQWebBrowserApp ->GetSysTimeZone(currentTimeZone);
        dt.setTime(time.currentTime());
        dt.setDate(date.currentDate());
        currentDate = dt.toString("yyyy:MM:dd:hh:mm:ss");
        strTimeList= currentDate.split(":");
		strTimeList << QString::number(currentTimeZone);
	}while(false);
	return strTimeList;
}


void browser::SetSystemTime(QStringList tempTimeInfoList)
{
	emit SetSystemCurTime_Sig(tempTimeInfoList);
}

QStringList browser::getVideoSettingMsg()
{
	QStringList  strVideoSettinginfoList;
	do
	{

		int  iRet,iBrighValue,iContrastValue,iSaturationValue,iHueValue,iColorSpace,iColorDepth;
		int  iCurrVolume,iShiftHorizontal,iWidth,iShiftVertical,iHeight;
		iRet = iBrighValue =iContrastValue = iSaturationValue= iHueValue = iColorSpace= iColorDepth = -1;
		iCurrVolume = -1;
		int x,y,width,height,dwidth,dheight;
		x = y = 0;
		width = height = 4096;
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			break;
		}
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_Brightness,iBrighValue);
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_Contrast,iContrastValue);
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_Saturation,iSaturationValue);
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_Hue,iHueValue);
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_ColorSpace,iColorSpace);
		iRet = pQWebBrowserApp->getDisplayParam(DispPARAM_ColorDepth,iColorDepth);
		pQWebBrowserApp->getCurrentVolume(iCurrVolume,iRet);

		pQWebBrowserApp->getDisplayPosition(RUA_DISP_OSD,x,y,width,height);

		pQWebBrowserApp->getDefaultDisplayParam(dwidth,dheight); 

		iShiftHorizontal = ( 50 +  (x / SHIFTMINSIZE ));
		iWidth  = ( 50 + ( (width - dwidth) / SHIFTMINSIZE));
		iShiftVertical =   ( 50 +  (y / SHIFTMINSIZE ));
		iHeight =( 50 + ( (height- dheight ) / SHIFTMINSIZE));

		if(ERROR_SUCCESS == iRet)
		{
			strVideoSettinginfoList << QString::number(iBrighValue)
				<< QString::number(iContrastValue) 
				<< QString::number(iSaturationValue)
				<< QString::number(iHueValue)
				<< QString::number(iCurrVolume)
				<< QString::number(iColorSpace)
				<< QString::number(iColorDepth)
			    << QString::number(iShiftHorizontal)
			    << QString::number(iWidth)
				<< QString::number(iShiftVertical)
				<< QString::number(iHeight) ;
		}
	}while(false);
	return strVideoSettinginfoList;
}

bool browser::eventFilter(QObject *target, QEvent *event)
{
	bool bProcessed = false;

	if(false == bProcessed)
	{
		bProcessed = QWidget::eventFilter(target, event);
	}

	return bProcessed;
}


void browser::RefreshSnmpPassWordResult(int iResutlt)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}
		
		QString strJsFucName = QString("ResetSnmpPassWordResult('%1');").arg((iResutlt));
		
		QVariant p_PluginWinObj = pWebFrame->evaluateJavaScript(strJsFucName);
		if(FALSE == p_PluginWinObj.isValid())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			if(Sw_LogJavascript)
			{
				LOG_BLINE("ResetSnmpPassWordResult invalid\n");
			}
			break;
		}
		
	}while(FALSE);
}


void browser::RefreshOsdUi(void)
{
	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}
		QVariant varRet = pWebFrame->evaluateJavaScript("clearMsg();");
		if(FALSE == varRet.isValid())
		{
			//LOG_BLINE("invalid\n");
			break;
		}
	}while(FALSE);
}

QStringList browser::getWirelessStatus()
{
	QStringList  strCurWirelessStatusList;

	do
	{
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			break;
		}
		strCurWirelessStatusList = pQWebBrowserApp->GetWirelessStatus();		
	}while(false);
	
	return strCurWirelessStatusList;
}

QStringList browser::getWirelessDhcpInfo()
{
	QStringList  strCurWirelessStatusList;
	do
	{
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			break;
		}
		strCurWirelessStatusList = pQWebBrowserApp->GetWirelessDhcpInfo();		
		if(strCurWirelessStatusList.isEmpty())
		{
			break;
		}
	}while(false);
	
	return strCurWirelessStatusList;
}

int browser::SaveWirelessStatusInfo(QStringList wirelessStatutsInfo)
{
	int  iRet = ERROR_SUCCESS;	
	do
	{
		if(wirelessStatutsInfo.size() != 6)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		if(wirelessStatutsInfo[0]  == "")
		{
		
		}
		QString strChannel = wirelessStatutsInfo[2] ;
		if(strChannel.trimmed()== "")
		{
			strChannel  = QString("1");
		}

		
		QString strIsEnable  =QString ((wirelessStatutsInfo[1] == "true" ) ? QString("1") :QString("0"));
		QString strParam = "";		/**/
		
		strParam +=QString(PARAM_KEY_CommitType)+ "=" +QString::number(3) + "\n";
		//strParam += QString(PARAM_KEY_WifiEthDhcp)+ "=" +wirelessStatutsInfo[1] + "\n";
		strParam += QString(PARAM_KEY_WifiChannel)+ "="  +"" + "\n";
		strParam += QString(PARAM_KEY_WifiSsid)+ "=" +wirelessStatutsInfo[3] + "\n";
		strParam += QString(PARAM_KEY_WifiSecurity)+ "=" +wirelessStatutsInfo[4] + "\n";
		strParam += QString(PARAM_KEY_WifiKey)+ "=" +wirelessStatutsInfo[5] + "\n";

		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = pQWebBrowserApp->SaveWifiCfg(strParam,strIsEnable.toInt());	
	}while(false);	
	return iRet;
}

int browser::SaveWirelessNetworkAddrInfo(QStringList tempNetworkAddrInfo,bool isAutoGetIp)
{
	int  iRet = ERROR_SUCCESS;	
	do
	{
		if(tempNetworkAddrInfo.size() != 5)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		QString strParam = "";		/**/
		QString strIsDhcp  =QString ((isAutoGetIp == true ) ? QString("1") :QString("0"));
		
		strParam +=QString(PARAM_KEY_CommitType) + "=" +QString::number(3) + "\n";
		strParam += QString(PARAM_KEY_WifiEthDhcp )+ "=" +strIsDhcp + "\n";
		strParam +=QString( PARAM_KEY_WifiEthIP)+ "="  +tempNetworkAddrInfo[0] + "\n";
		strParam += QString(PARAM_KEY_WifiEthSubnetMask)+ "="  +tempNetworkAddrInfo[1] + "\n";
		strParam += QString(PARAM_KEY_WifiGateway)+ "="  +tempNetworkAddrInfo[2] + "\n";
		strParam += QString(PARAM_KEY_WifiDns1)+ "="  +tempNetworkAddrInfo[3] + "\n";
		strParam += QString(PARAM_KEY_WifiDns2)+ "="  +tempNetworkAddrInfo[4] + "\n";

		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
		pQWebBrowserApp->SaveWirelessNetworkAddr(strParam);		
	}while(false);	
	return iRet;
}

/*int browser::saveUsefulWirelessName(QString UsefulWirelessName)
{
	int iRet = ERROR_SUCCESS;
	QStringList strWirelessName;
    QFile file( "WIRELESSNAME_PATH_FILE" );
	
	do{
		strWirelessName = getUsefulWirelessName();
		if(strWirelessName == "")
		{
			LOG_BLINE("file is empty");
			if ( file.open( IO_WriteOnly ) ) {
				QTextStream stream( &file );
				for ( QStringList::Iterator it = strWirelessName.begin(); it != strWirelessName.end(); ++it )
					stream << *it << "\n";
				file.close();
			}
			else
			{
				LOG_BLINE("File open fail");
				iRet = ERR_FILE_OPEN_FAIL;
				break;
			}
			break;
		}
		else
		{
			if(strWirelessName.length >5){
				if ( file.open( IO_WriteOnly ) ) {
					QTextStream stream( &file );
					for ( QStringList::Iterator it = strWirelessName.begin(); it != strWirelessName.end(); ++it )
						stream << *it << "\n";
					file.close();
				}
				else
				{
					LOG_BLINE("File open fail");
					iRet = ERR_FILE_OPEN_FAIL;
					break;
				}

			}
			else
			{
				if ( file.open( IO_WriteOnly | IO_Append ) ) {
					QTextStream stream( &file );
					for ( QStringList::Iterator it = strWirelessName.begin(); it != strWirelessName.end(); ++it )
						stream << *it << "\n";
					file.close();
				}
				else
				{
					LOG_BLINE("File open fail");
					iRet = ERR_FILE_OPEN_FAIL;
					break;
				}
			}
		}

	}while(false);
	return iRet;
}

int browser::getUsefulWirelessName()
{
	int iRet = ERROR_SUCCESS;
	QStringList strWirelessName;
    QFile file( "WIRELESSNAME_PATH_FILE" );

	do{
	    if( file.open( IO_ReadOnly ) ) {
	        QTextStream stream( &file );
	        QString line;
	        int n = 1;
	        while ( !stream.eof() ) {
	            line = stream.readLine();
	            LOG_BLINE( "%3d: %s\n", n++, line.latin1() );
	            strWirelessName += line;
	        }
	        file.close();
	    }else
	    {
			LOG_BLINE("File open fail");
			break;
		}
	}while(false);
	
	return strWirelessName;
}*/

INT_t browser::PlayHtmlEntryInPlugin(QString strUrl)
{
	int  iRet = ERROR_SUCCESS;	
	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
	
		QString strFunction = QString("PlayHtmlEntryInPlugin('%1');").arg(strUrl);;
		QVariant FunctionValue = pWebFrame->evaluateJavaScript(strFunction);
		if(FALSE == FunctionValue.isValid())
		{
			iRet = ERR_JSCRIPT_ERR;
			break;
		}			
	}while(false);	

	if(Sw_LogJavascript)
	{
		if(iRet !=ERROR_SUCCESS)
		{
			LOG_BLINE("PlayHtmlEntryInPlugin fail ,error no =%d\n",iRet);
		}
	}
	return iRet;	
}

INT_t browser::PlayHtmlEntryInPluginEnd()
{
	int  iRet = ERROR_SUCCESS;	
	do
	{
		QWebFrame *pWebFrame = ui.webView->page()->mainFrame();
		if(NULL == pWebFrame)
		{
			iRet = ERROR_INVALID_STATE;
			break;
		}
	
		QVariant FunctionValue = pWebFrame->evaluateJavaScript("PlayHtmlEntryInPluginEnd();");
		if(FALSE == FunctionValue.isValid())
		{
			iRet = ERR_JSCRIPT_ERR;
			break;
		}			
	}while(false);	

	if(Sw_LogJavascript)
	{
		if(iRet !=ERROR_SUCCESS)
		{
			LOG_BLINE("PlayHtmlEntryInPluginEnd fail ,error no =%d\n",iRet);
		}
	}
	
	return iRet;
}


void browser::paintEvent ( QPaintEvent * event )
{

    QWidget::paintEvent(event);
}

void browser::keyPressEvent(QKeyEvent * event) 
{

    int iKeyCode = event->key();
    //LOG_BLINE(" iKeyCode is %d\n",iKeyCode);
    QWidget::keyPressEvent(event);
}

void browser::keyReleaseEvent(QKeyEvent * event) 
{
    int iKeyCode = event->key();
    //LOG_BLINE(" iKeyCode is %d\n",iKeyCode);
    QWidget::keyReleaseEvent(event);
}



#if 0/*added by su 2013-06-18*/
void  browser::ChangeLanguage(int index)
{
	emit SIGNAL_Evt_ChangeLanguage(index);
}
#endif

