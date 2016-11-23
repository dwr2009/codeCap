#include "qplaypic_html_win.h"
#include <QFile>
#include <QtWebKitWidgets/QWebFrame>
#include "WebBrowserAppIf.h"
#include <AppFilePathManage.h>
#include "WebBrowserApp.h"
#include <EventLog.h>
#include <EventLogExt.h>
#include <QCryptRC4.h>
#include "IrKeyQtKeyMap.h"
#include <DbgLogSwitchDef.h>
#include <QEvent>


#define PERMISSION_FILE_NAME "Permission.ini"
#define INI_KEY_FileContentDigest "FileContentDigest"
#define ENCDECRYPT_KEY "s6s9_234&-12"

QPlayPic_Html_Win::QPlayPic_Html_Win(QWidget *parent)
	: QWidget(parent)
{
	bool bRet;
	
	ui.setupUi(this);	
	setWindowFlags(Qt::FramelessWindowHint);
	m_strCurPlayHtmlUrl = "" ;
	m_bIsPlayHtml = false ;
	m_bIsPlayPicture = false;
	
	//QtWebPlugin is not affected by QWebSettings::PluginsEnabled.
	ui.html_View->settings()->setAttribute(QWebSettings::PluginsEnabled, true);	
	ui.html_View->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);  
	//No need to open a new web window
	ui.html_View->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false); 	
	//Currently Java applets are not supported.
	ui.html_View->settings()->setAttribute(QWebSettings::JavaEnabled, false);
	//Page navigation using arrow keys
	ui.html_View->settings()->setAttribute(QWebSettings::SpatialNavigationEnabled, true);
	
	ui.html_View->page()->setPluginFactory(&m_WebPluginFactory);
	bRet = connect((QObject *)ui.html_View->page()->mainFrame(), 
		SIGNAL(javaScriptWindowObjectCleared ()), this, SLOT(addObjectForJs()));
	if(false == bRet)
	{
		LOG_BLINE("ConnSig(HtmlView.WinOjbCleared) failed\n");
	}
	bRet = connect(ui.html_View, SIGNAL(loadFinished(bool)) ,this, SLOT(On_PageLoadFinished(bool)));
	if(false == bRet)
	{
		LOG_BLINE("ConnSig(HtmlView.loadFinished) failed\n");
	}
}

QPlayPic_Html_Win::~QPlayPic_Html_Win()
{

}

void QPlayPic_Html_Win:: playHtmlFile(QString selectFileName,bool isPlayHtmlFile)
{		
	do
	{
		if(NULL == ui.html_View || ui.page_widget == NULL)
		{
			break;
		}
		QWebPage * pWebPage = ui.html_View->page();
		if(NULL == pWebPage)
		{
			break;
		}
		QWebFrame* pWebFrame = pWebPage->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}	
		if(isPlayHtmlFile)
		{
			ui.html_View->load(QUrl(selectFileName));
		}
		else
		{
			if(QFile::exists(selectFileName))
			{
				ui.html_View->load(QUrl("file://" + selectFileName));			
			}
		}		

		ui.page_widget->setCurrentIndex(1);
		m_strCurPlayHtmlUrl = selectFileName ; 
		m_bIsPlayHtml = true;
		m_bIsPlayPicture = false;
		show();
	}while(FALSE);
}

void  QPlayPic_Html_Win:: playPictureFile(QString selectFileName)
{
	ui.page_widget->setCurrentIndex(0);
	ui.play_image_lab->setPixmap(QPixmap(selectFileName));
	ui.IpInfo_lab->setVisible(false);
	m_strCurPlayHtmlUrl = selectFileName ; 
	m_bIsPlayHtml = false;
	m_bIsPlayPicture = true;
	show();
}

void QPlayPic_Html_Win:: playHtmlFileEnd()
{	
#if 1/*add by lxj 2012-7-23, write HTML PlaybackEos event to log */
	do
	{		
		BOOL_t bEnableWriteLoggin = TRUE;
		CMp7xxGlobalStatus m_Mp7xxGlobalStatusData;
		INT_t iRet = m_Mp7xxGlobalStatusData.getEnableLogging(OUT bEnableWriteLoggin);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}

		if(bEnableWriteLoggin && m_bIsPlayHtml)			
		{
			CString::STD_STRING_LIST strList;
			strList.push_back(TYPE_LOG_PlaybackEos);
			strList.push_back(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm::ss").toLocal8Bit().constData() ); 
			QString strPlayType = FOLDER_NAME_HTML;
			strList.push_back(strPlayType.toLocal8Bit().constData());		
			strList.push_back(getCurrPlayHtmlUrl().toLocal8Bit().constData()); 
			strList.push_back(0);	//playback id
			strList.push_back(TRUE);	//bIsCompletePlaybackEos
			if(ERROR_SUCCESS != WriteEventLog(strList))
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
			}	
			strList.clear();
		}
	}while(FALSE);
#endif
	do
	{
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(pQWebBrowserApp)
		{
#if 1 /*Add by xuweiwei 2014-2-21*/
			if(m_bIsPlayHtml)
			{
				INT_t iRet;
				bool playHtmlEosFlag=pQWebBrowserApp->getCurrPlaybackEosFlag();
				iRet=pQWebBrowserApp->On_HtmlPlaybackEos_Sig(m_strCurPlayHtmlUrl,0,playHtmlEosFlag);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}		
			}
#endif
			if(pQWebBrowserApp->getIsPlayVideoLayer())
			{
				break;
			}
		}
		else
		{
			break;
		}
		if(NULL == ui.html_View || ui.page_widget == NULL)
		{
			break;
		}
		QWebPage * pWebPage = ui.html_View->page();
		if(NULL == pWebPage)
		{
			break;
		}
		QWebFrame* pWebFrame = pWebPage->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}	
		m_bIsPlayPicture = false;
		m_bIsPlayHtml = false;
		this->hide();
		ui.html_View->stop();
		ui.html_View->load(QUrl(""));
		ui.html_View->setHtml("");
		m_strCurPlayHtmlUrl = "" ;
		//ui.html_View->setContent(QByteArray("")); //Maybe Segmentation fault
	}while(FALSE);
		
}

void  QPlayPic_Html_Win:: playBootLogo(QString selectFileName,QString strIp,QString strMac)
{
	ui.page_widget->setCurrentIndex(0);
	ui.play_image_lab->setPixmap(QPixmap(selectFileName));
	ui.IpInfo_lab->setText("IP :" + strIp + "MAC :" + strMac);
}

void  QPlayPic_Html_Win:: playBootLogoEnd()
{
	//this->setVisible(false);
	this->showMinimized();
	this->hide();
	ui.play_image_lab->clear();
	ui.play_image_lab->setPixmap(QPixmap(""));
	ui.IpInfo_lab->setText("");
	ui.IpInfo_lab->setVisible(false);
}

void QPlayPic_Html_Win::addObjectForJs()
{
	do
	{
		if(CC_UNLIKELY(NULL == ui.html_View))
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		QWebPage * pWebPage = ui.html_View->page();
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
		pWebFrame->addToJavaScriptWindowObject("App", this);
		//window.ControlCenter
		pWebFrame->addToJavaScriptWindowObject("ControlCenter", qApp);
		//window.WebBrowser
		pWebFrame->addToJavaScriptWindowObject("WebBrowser", this);
	}while(FALSE);
}

int QPlayPic_Html_Win::playPluginHtmlEntry(QString strPlayFileUrl) 
{
	INT_t iOutRet = ERROR_SUCCESS;	
	do
	{
		QWebFrame *pWebFrame = ui.html_View->page()->mainFrame();	
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		QString strJsFucName = QString("playPluginPlaylistHtmlEntry('%1');").arg(strPlayFileUrl);
		
		QVariant p_PluginWinObj = pWebFrame->evaluateJavaScript(strJsFucName);
		if(FALSE == p_PluginWinObj.isValid())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			if(Sw_LogJavascript)
			{
				LOG_BLINE("invalid\n");
			}
			break;
		}		
		iOutRet = ERROR_SUCCESS;		
	}while(FALSE);

	return iOutRet;
}

int QPlayPic_Html_Win::playPluginHtmlEntryEnd_timeOut()
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do
	{
		QWebFrame *pWebFrame = ui.html_View->page()->mainFrame();	
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		QString strJsFucName = QString("playPluginPlaylistHtmlEntryEnd();");		
		QVariant  varExecResult= pWebFrame->evaluateJavaScript(strJsFucName);
		if(FALSE == varExecResult.isValid())
		{
			iOutRet = ERR_JSCRIPT_ERR;
			if(Sw_LogJavascript)
			{
				LOG_BLINE("invalid\n");
			}
			break;
		}		
		iOutRet = ERROR_SUCCESS;		
	}while(FALSE);

	return iOutRet;
}

void QPlayPic_Html_Win::DbgLog(const QString & strDbgInfo)
{
	CWebBrowserAppIf * pAppIf = dynamic_cast<CWebBrowserAppIf *>(qApp);
	if(pAppIf)
	{
		pAppIf->DbgLog(strDbgInfo);
	}
	else
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
}

void QPlayPic_Html_Win::On_PageLoadFinished(bool bOK)
{
	do
	{
		if(false == bOK)
		{
			break;
		}

		if(NULL == ui.html_View)
		{
			break;
		}
		QWebPage * pWebPage = ui.html_View->page();
		if(NULL == pWebPage)
		{
			break;
		}
		QWebFrame* pWebFrame = pWebPage->mainFrame();
		if(NULL == pWebFrame)
		{
			break;
		}
		setWebElementTargetToSelf(pWebFrame);
		

		pWebFrame->setFocus();
	}while(FALSE);
}

QString QPlayPic_Html_Win::RunShellScript(const QString & strShellScript)
{
	QString strOutResult;
	INT_t iRet;
	QControlCenterApp * pApp = (typeof(pApp))qApp;	//pApp must not be NULL

	do
	{
		QFileInfo strCurrPlayFileInfo(m_strCurPlayHtmlUrl);
		QString strCurrPlayFileName = strCurrPlayFileInfo.fileName();
		QStringList strUrlList= m_strCurPlayHtmlUrl.split("/");
		if(strUrlList.size() < 3)
		{
			break;
		}
		QString strCurrFilePath="";
		QString strDevType = "";
		if(strUrlList[2].trimmed() == LOCAL_STORAGE_DIR_NAME)
		{
			strDevType = APP_DIR_PATH  + LOCAL_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() == USB_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + USB_STORAGE_DIR_NAME ;
		} 
		else if(strUrlList[3].trimmed() ==SD_CARD_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  +SD_CARD_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==USB2_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  +USB2_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_HDD)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  +StorageDirName_HDD ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_RAM)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  +StorageDirName_RAM ;
		}
		
		strCurrFilePath = (strDevType + "/" + FOLDER_NAME_HTML +"/"+PERMISSION_FILE_NAME);
		if(!QFile::exists(strCurrFilePath))
		{			
			break;
		}

		QSharedPointer<configuration> mp_Config;
		mp_Config = QSharedPointer <configuration> (new configuration(strCurrFilePath));;
		if(mp_Config.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail !");
			break;
		}
		if(!mp_Config->open_file())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail!");
			break;
		}
		QString strKeyValue = QString(INI_KEY_FileContentDigest) + "/" + strCurrPlayFileName;
		QString strPermissionValue = mp_Config ->getConfigValue(strKeyValue);
		mp_Config->close_file();
		QFile tempFile(m_strCurPlayHtmlUrl);
		QString strGetMD5Value =GetFileMD5(&tempFile);
		//qDebug()<<"md5 : "<<strGetMD5Value;
		QByteArray destArrray("");
		//iRet =QCryptRC4::EncDecrypt(strPermissionValue.toLatin1(),destArrray ,ENCDECRYPT_KEY);
		iRet =QCryptRC4::EncDecrypt((QByteArray::fromBase64(strPermissionValue.toLatin1())),destArrray, ENCDECRYPT_KEY);
		if(ERROR_SUCCESS != strGetMD5Value.compare(QString(destArrray), Qt::CaseInsensitive))
		{
			iRet = ERROR_INVALID_STATE;
			strOutResult = QString("Permission is invalid!");
			break;
		}			
		if(Sw_LogJavascript)
		{
			LOG_BLINE("strShellScript =%s\n", qPrintable(strShellScript));
		}
		
		iRet = pApp->RunShellScript(strShellScript, OUT strOutResult);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			strOutResult = QString("Exec shell fail!");
		}
	}while(FALSE);
	
	return strOutResult;
}

 QString QPlayPic_Html_Win::CheckIRStatus()
{
	QString strOutResult;
	INT_t iRet;
	QControlCenterApp * pApp = (typeof(pApp))qApp;	//pApp must not be NULL

	do
	{
		QFileInfo strCurrPlayFileInfo(m_strCurPlayHtmlUrl);
		QString strCurrPlayFileName = strCurrPlayFileInfo.fileName();		
		QStringList strUrlList= m_strCurPlayHtmlUrl.split("/");
		if(strUrlList.size() < 3)
		{
			break;
		}
		QString strCurrFilePath="";
		QString strDevType = "";

		if(strUrlList[2].trimmed() == LOCAL_STORAGE_DIR_NAME)
		{
			strDevType =APP_DIR_PATH  + LOCAL_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() == USB_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR +APP_DIR_PATH  + USB_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==SD_CARD_DIR_NAME)
		{
			strDevType =MOUNTDIR +APP_DIR_PATH  + SD_CARD_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==USB2_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR +APP_DIR_PATH  + USB2_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_HDD)
		{
			strDevType =MOUNTDIR +APP_DIR_PATH  + StorageDirName_HDD ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_RAM)
		{
			strDevType =MOUNTDIR +APP_DIR_PATH  + StorageDirName_RAM ;
		}
		
		strCurrFilePath = (strDevType + "/" + FOLDER_NAME_HTML +"/"+PERMISSION_FILE_NAME);
		if(!QFile::exists(strCurrFilePath))
		{		
			strOutResult = QString("Exec shell fail!");
			break;
		}

		QSharedPointer<configuration> mp_Config;
		mp_Config = QSharedPointer <configuration> (new configuration(strCurrFilePath));;
		if(mp_Config.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail !");
			break;
		}
		if(!mp_Config->open_file())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail!");
			break;
		}
		QString strKeyValue = QString(INI_KEY_FileContentDigest) + "/" + strCurrPlayFileName;
		QString strPermissionValue = mp_Config ->getConfigValue(strKeyValue);
		mp_Config->close_file();
		QFile tempFile(m_strCurPlayHtmlUrl);
		QString strGetMD5Value =GetFileMD5(&tempFile);
		QByteArray destArrray("");
		//iRet =QCryptRC4::EncDecrypt(strPermissionValue.toLatin1(),destArrray ,ENCDECRYPT_KEY);
		iRet =QCryptRC4::EncDecrypt((QByteArray::fromBase64(strPermissionValue.toLatin1())),destArrray, ENCDECRYPT_KEY);
		if(ERROR_SUCCESS != strGetMD5Value.compare(QString(destArrray), Qt::CaseInsensitive))
		{
			iRet = ERROR_INVALID_STATE;
			strOutResult = QString("Permission is invalid!");
			break;
		}			
		
		iRet = pApp->CheckIRStatus(0, 0x95);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			strOutResult = QString("Exec shell fail!");
		}
		else
		{
			strOutResult = QString("Exec shell Success!");
		}
	}while(FALSE);
	
	return strOutResult;
}

QString QPlayPic_Html_Win::isPermission(bool &isPerm)
{
	QString strOutResult;
	INT_t iRet;
	do
	{
		QFileInfo strCurrPlayFileInfo(m_strCurPlayHtmlUrl);
		QString strCurrPlayFileName = strCurrPlayFileInfo.fileName();		
		QStringList strUrlList= m_strCurPlayHtmlUrl.split("/");
		if(strUrlList.size() < 3)
		{
			break;
		}
		QString strCurrFilePath="";
		QString strDevType = "";

		if(strUrlList[2].trimmed() == LOCAL_STORAGE_DIR_NAME)
		{
			strDevType =APP_DIR_PATH  + LOCAL_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() == USB_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + USB_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==SD_CARD_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + SD_CARD_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==USB2_STORAGE_DIR_NAME)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + USB2_STORAGE_DIR_NAME ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_HDD)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + StorageDirName_HDD ;
		}
		else if(strUrlList[3].trimmed() ==StorageDirName_RAM)
		{
			strDevType =MOUNTDIR + APP_DIR_PATH  + StorageDirName_RAM ;
		}
		
		strCurrFilePath = (strDevType + "/" + FOLDER_NAME_HTML +"/"+PERMISSION_FILE_NAME);
		if(!QFile::exists(strCurrFilePath))
		{		
			strOutResult = QString("Exec shell fail!");
			break;
		}

		QSharedPointer<configuration> mp_Config;
		mp_Config = QSharedPointer <configuration> (new configuration(strCurrFilePath));;
		if(mp_Config.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail !");
			break;
		}
		if(!mp_Config->open_file())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			strOutResult = QString("Exec shell fail!");
			break;
		}
		QString strKeyValue = QString(INI_KEY_FileContentDigest) + "/" + strCurrPlayFileName;
		QString strPermissionValue = mp_Config ->getConfigValue(strKeyValue);
		mp_Config->close_file();
		QFile tempFile(m_strCurPlayHtmlUrl);
		QString strGetMD5Value =GetFileMD5(&tempFile);
		QByteArray destArrray("");
		iRet =QCryptRC4::EncDecrypt((QByteArray::fromBase64(strPermissionValue.toLatin1())),destArrray, ENCDECRYPT_KEY);
		if(ERROR_SUCCESS != strGetMD5Value.compare(QString(destArrray), Qt::CaseInsensitive))
		{
			iRet = ERROR_INVALID_STATE;
			strOutResult = QString("Permission is invalid!");
			isPerm = false;
			break;
		}			
		else
		{
			strOutResult = QString("Success!");
			isPerm = true;
		}
	}while(FALSE);
	
	return strOutResult;
}
 
 QStringList QPlayPic_Html_Win::getPlayListFiles(int reqListType)
 {
	 QStringList  fileNamelist ,filePathInfoList;
	 emit getPlayListFiles_signal(reqListType,fileNamelist,filePathInfoList);
	 m_strFilePathInfoList	= filePathInfoList;
	 return fileNamelist;
 }
  
 QString QPlayPic_Html_Win::playSelectFile(const QString selectFileName)
 {
	//LOG_BLINE("%s\n", qPrintable(selectFileName));
	int iOutRet = ERROR_SUCCESS;
	QString strOutResult;
	QStringList fileNamelist;
	int iMeidaType, iFileNums;
	QString filePathInfo;
	bool isPerm = false;

	strOutResult = isPermission(isPerm);

	if(isPerm)
	{
		for(iMeidaType = 0; iMeidaType < AMedia_TYPE_MAX; iMeidaType++)
		{
			fileNamelist = getPlayListFiles(iMeidaType);
			if(fileNamelist.size() < 0)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
			for(iFileNums = 0; iFileNums < fileNamelist.size(); iFileNums++)
			{
				if(0 == fileNamelist[iFileNums].compare(selectFileName, Qt::CaseSensitive))
				{
					filePathInfo = m_strFilePathInfoList[iFileNums];
					emit playSelectFile_signal(filePathInfo,iMeidaType);
					goto EXIT_PROC;
				}
			}
		}
	}
	
EXIT_PROC:
	return strOutResult;
}

void QPlayPic_Html_Win::setWebElementTargetToSelf(QWebFrame * pWebFrame)
{
	do
	{
		if(NULL == pWebFrame)
		{
			break;
		}

		QWebElementCollection WebEleCollection_a = pWebFrame->findAllElements("a[target=_blank]");
		foreach(QWebElement WebElement_a, WebEleCollection_a) 
		{
			//LOG_BLINE("target(%s):_blank=>_self\n", qPrintable(WebElement_a.localName()));
			WebElement_a.setAttribute("target", "_self");
		}
		QList <QWebFrame *> ChildFrameList = pWebFrame->childFrames();
		QList <QWebFrame *>::iterator itWebFrame;
		for(itWebFrame = ChildFrameList.begin(); itWebFrame != ChildFrameList.end(); itWebFrame++)
		{
			setWebElementTargetToSelf(*itWebFrame);
		}
	}while(FALSE);
}


INT_t QPlayPic_Html_Win::CalcVideoPluginPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do
	{
		QWebFrame *pWebFrame = ui.html_View->page()->mainFrame();	
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		QVariant varPluginPosition = pWebFrame->evaluateJavaScript("CalcVideoPosition();");
		if(FALSE == varPluginPosition.isValid())
		{
			iOutRet = ERROR_NOT_FOUND;
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
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2012-12-17*/
int QPlayPic_Html_Win::setHtmlEvalJavascript(const QString & strHtmlEvalJavascript, QString & strRet )
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do{
		QWebFrame *pWebFrame = ui.html_View->page()->mainFrame();	
		if(NULL == pWebFrame){
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if( (1 != ui.page_widget->currentIndex()) ){
			iOutRet = ERROR_INVALID_STATE;
			strRet = strRet.sprintf("%d",iOutRet);
			LOG_BLINE("Is not playing html now, can't run javascript\n");
			break;
		}

#if 0
		strRet = pWebFrame->evaluateJavaScript(strHtmlEvalJavascript).toString();
#else
		QVariant varExecResult = pWebFrame->evaluateJavaScript(strHtmlEvalJavascript);
		if(FALSE == varExecResult.isValid()){
			strRet = strRet.sprintf("%d",ERROR_INVALID_PARAMETER);
		}else{
			strRet = varExecResult.toString();
		}
#endif
	}while(FALSE);

	return iOutRet;
}
#endif

QString QPlayPic_Html_Win::GetFileMD5(QFile * tempFile)
{
	QString strMD5Value = "";
	do
	{
		if (tempFile == NULL)
		{
			break;
		}
		if (!tempFile->exists())
		{
			return strMD5Value;
		}		
		if (!tempFile->open(QIODevice::ReadOnly)) 
		{
			return strMD5Value;
		}
		

		QByteArray hash = QCryptographicHash::hash(tempFile->readAll(),
										QCryptographicHash::Md5);
		strMD5Value = hash.toHex();//.toUpper();
		
		tempFile->close();
	}while(FALSE);
	
	return strMD5Value;
}

int QPlayPic_Html_Win::playPluginPlayList(const QString & strPlaylistUrl) 
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	do
	{
		INT_t iOutRet = ERROR_SUCCESS;
			
		QWebFrame *pWebFrame = ui.html_View->page()->mainFrame();	
		if(NULL == pWebFrame)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		QString strJsFucName = QString("playPluginPlaylist('%1')").arg(strPlaylistUrl);
		
		QVariant p_PluginWinObj = pWebFrame->evaluateJavaScript(strJsFucName);
		if(p_PluginWinObj.toInt() != ERROR_SUCCESS)
		{
			iOutRet = ERR_JSCRIPT_ERR;
			break;
		}
		
		iOutRet = ERROR_SUCCESS;		
	}while(FALSE);

	return iOutRet;
}

bool QPlayPic_Html_Win::isPlayingHtml()
{
	return m_bIsPlayHtml;
}

bool QPlayPic_Html_Win::isPlayingPicture()
{
	return m_bIsPlayPicture;
}
