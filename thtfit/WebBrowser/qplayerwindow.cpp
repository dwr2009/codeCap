#include "qplayerwindow.h"
#include <ErrPrintHelper.h>
#include "IrKeyQtKeyMap.h"
#include "WebBrowserApp.h"
#include <QDesktopWidget>

QPlayerWindow::QPlayerWindow(QWidget *parent)
	: QWidget(parent)
{	
    QDesktopWidget *pDesktopWidget=qApp->desktop();
    QRect screenRect=pDesktopWidget->screenGeometry();
    
    qWarning("%s->%s(%d) %p \n",__FILE__,__FUNCTION__,__LINE__,this);
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowOpacity(1);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
    setGeometry(screenRect);
	m_bKeyIsRelease = false;
	m_bIsFirstPress = true;
	m_bIsNeedSendSigByActiveWin = false;
	m_dOpticeValue = 10;
	createWebView();
}

QPlayerWindow::~QPlayerWindow()
{
	bool bRet;
	QControlCenterApp * pApp = (typeof(pApp))qApp;
	
	bRet = disconnect(pApp, NULL, this, NULL);
	if(false == bRet)
	{
		LOG_BLINE("Disconnect QtSignal err\n");
	}
}
 
void QPlayerWindow::createWebView()
{
	int iRet;
	iRet = 0;
    QWebSettings *pGlobalWebSetting;
	do
	{		
		m_WebView_sp  = QSharedPointer <QWebView> (new QWebView(this)); 
		if(m_WebView_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		m_WebView_sp->setUrl(QUrl("about:blank"));
		m_WebView_sp->hide();
		if(m_WebView_sp.isNull())
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
        pGlobalWebSetting=QWebSettings::globalSettings();
        
        if(NULL==pGlobalWebSetting)
        {
            qWarning("%s->%s(%d) pGlobalWebSetting is  %p \n",__FILE__,__FUNCTION__,__LINE__,pGlobalWebSetting);
        }
        else
        {
            pGlobalWebSetting->setMaximumPagesInCache(0);
            pGlobalWebSetting->setOfflineStorageDefaultQuota(1024*1024*8);
        }
        
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			iRet = ERROR_OUT_OF_MEMORY;
			LOG_BLINE("ERROR_OUT_OF_MEMORY err\n");
			break;
		}
		bool bRet=connect(pQWebBrowserApp,SIGNAL(onPlayPluginListBackEnd()),this,SLOT(onPlayPluginListEnd()));
		if(false == bRet)
		{
			LOG_BLINE("ConnectSig err\n");
		}
		m_PlayHtmlOrPicWin = QSharedPointer <QPlayPic_Html_Win> (new QPlayPic_Html_Win(this)); 
		if(m_PlayHtmlOrPicWin.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			LOG_BLINE("ERROR_OUT_OF_MEMORY err\n");
			break;
		}
		m_PlayHtmlOrPicWin->hide();
	}while(FALSE);
}


int QPlayerWindow::StopPlayCurrPluginList()
{
	int iRet;
	iRet = ERROR_SUCCESS;
	do
	{		
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(pQWebBrowserApp)
		{
			iRet = pQWebBrowserApp->StopPlayPluginList();
		}
		
	}while(FALSE);

	return iRet;
}

void QPlayerWindow::paintEvent ( QPaintEvent * event )
{
	QPainter Painter(this); 
	Painter.setCompositionMode(QPainter::CompositionMode_Clear);
	Painter.fillRect(0, 0, width(), height(), Qt::SolidPattern);
	Painter.end();
	QWidget::paintEvent(event);
}

void QPlayerWindow::keyPressEvent(QKeyEvent * event) 
{
	m_bIsNeedSendSigByActiveWin = true; 
	int iKeyCode = event->key();

    //LOG_BLINE("iKeyCode is %d\n",iKeyCode);
	switch(iKeyCode)
	{
	case IR_KEY_MENU:
		{
			#if 0/*by_su*/
			emit On_KeyMenu_Sig(IR_KEY_MENU);	
			m_bIsNeedSendSigByActiveWin = false; // has switch to browser win,so not need send  Release sig
			#endif
		}
		break;
	case IR_KEY_STOP:
		emit On_KeyMenu_Sig(IR_KEY_STOP);
		m_bIsNeedSendSigByActiveWin = false;
		break;
	case IR_KEY_DOWN:
		//emit On_KeyMenu_Sig(IR_KEY_DOWN);
		break;
	case IR_KEY_UP:
		//emit On_KeyMenu_Sig(IR_KEY_UP);
		break;
	case IR_KEY_RIGHT:
		//emit On_KeyMenu_Sig(IR_KEY_RIGHT);		//by_su
		break;
	case IR_KEY_LEFT:
		//emit On_KeyMenu_Sig(IR_KEY_LEFT);		//by_su
		break;
	case IR_KEY_CHANGERESOLUTION:
		emit On_KeyMenu_Sig(IR_KEY_CHANGERESOLUTION);
		break;
	case IR_KEY_ENTER:
		{
			if(m_bIsFirstPress)
			{
				m_bIsFirstPress = false;
				m_OnKeyDownTime.start();
			}
			else
			{
				if(m_bKeyIsRelease)
				{
					m_bKeyIsRelease = false;
					m_OnKeyDownTime.restart();
				}
			}
		}
		break;
	case IR_KEY_PLAY:
		{
			if(m_bIsFirstPress)
			{
				m_bIsFirstPress = false;
				m_OnPlayKeyDownTime.start();
			}
			else
			{
				if(m_bKeyIsRelease)
				{
					m_bKeyIsRelease = false;
					m_OnPlayKeyDownTime.restart();
				}
			}
			//emit On_KeyMenuLongPress_Sig(0);
		}
		break;	
	default:
		QWidget::keyPressEvent(event);
		break;
	}

	
}

void QPlayerWindow::keyReleaseEvent(QKeyEvent * event) 
{
	do
	{
		if(!m_bIsNeedSendSigByActiveWin)
		{
			break;
		}
		m_bIsNeedSendSigByActiveWin = false; 
		switch(event->key())
		{	
		case IR_KEY_ENTER:
			{
				int  iPressDurationTime =  m_OnKeyDownTime.elapsed();			
				m_bKeyIsRelease = true ; 
				if(iPressDurationTime > 500)
				{
					emit On_KeyMenu_Sig(IR_KEY_STOP);		
				}
			}		
			break;	
		case IR_KEY_PLAY:
			{
				int  iPressDurationTime =  m_OnPlayKeyDownTime.elapsed();	
				if(iPressDurationTime > 500)
				{
					if(!m_bKeyIsRelease)
					{
						emit On_KeyMenu_Sig(IR_KEY_MENU);		
					}
				}
				else
				{
					emit On_KeyMenuLongPress_Sig(0);
				}	
				m_bKeyIsRelease = true ; 
			}
			break;
#if 0/*add by lxj 2012-7-24*/
		case IR_KEY_PAUSE:
			{
				LOG_BLINE("IR_KEY_PAUSE\n");
				emit On_KeyMenuLongPress_Sig(0);
			}
			break;
#endif
		default:
			QWidget::keyReleaseEvent(event);
			break;
		}
	}while(FALSE);
}

void QPlayerWindow::PlayPluginListHtmlEntry(QString strPlayFileUrl)
{
	do
	{
		if(m_WebView_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		m_WebView_sp->resize(size());
		m_WebView_sp->load(QUrl(strPlayFileUrl));
		m_WebView_sp->show();
	}while(FALSE);
}

void QPlayerWindow::PlayPluginListHtmlEntryEnd()
{
	do
	{
		if(m_WebView_sp.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		m_WebView_sp->hide();
		m_WebView_sp->stop();
		m_WebView_sp->load(QUrl(""));
//		m_WebView_sp->setContent(QByteArray(""));
	}while(FALSE);
}

int QPlayerWindow::playPluginPlaylist(const QString & strPlaylistUrl)
{
	int iRet;
	QSharedPointer <QPlaylistData> PluginPlaylistData_sp(new QPlaylistData);
	iRet = ERROR_SUCCESS;
	QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);

	do
	{	
		if(!pQWebBrowserApp)
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(PluginPlaylistData_sp.isNull())
		{
			iRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		QString strPlayFileUrl ="";
		QString strFileType;
		if(strPlaylistUrl.trimmed()==""  || !QFile::exists(strPlaylistUrl))
		{			
			strPlayFileUrl = pQWebBrowserApp->ParsePlayFilePathToPlayUrl(strPlaylistUrl,strFileType);	
			if(strPlayFileUrl.trimmed()==""  || !QFile::exists(strPlayFileUrl))
			{
				iRet = ERROR_INVALID_PARAMETER;
				break;
			}
			
			iRet = PluginPlaylistData_sp->LoadFromIniFile(&strPlayFileUrl);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
				break;
			}		
			StopPlayCurrPluginList();
			if(pQWebBrowserApp)
			{
				PlayPluginListHtmlEntryEnd();
				iRet= pQWebBrowserApp->setPluginListDataSource(PluginPlaylistData_sp,strPlaylistUrl);
				if(iRet == ERROR_SUCCESS)
				{
					pQWebBrowserApp->setMediaPlayType(AMedia_TYPE_PLAYLIST);
					pQWebBrowserApp->PlayPluginListNextEntry(true);
				}			
			}	
			break;
		}
		iRet =StopPlayCurrPluginList();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}	

		iRet = PluginPlaylistData_sp->LoadFromIniFile(&strPlaylistUrl);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}				
		if(pQWebBrowserApp)
		{
			PlayPluginListHtmlEntryEnd();
			iRet= pQWebBrowserApp->setPluginListDataSource(PluginPlaylistData_sp,strPlaylistUrl);
			if(iRet == ERROR_SUCCESS)
			{
				pQWebBrowserApp->setMediaPlayType(AMedia_TYPE_PLAYLIST);
				pQWebBrowserApp->PlayPluginListNextEntry(true);
			}
		}		
	}while(FALSE);

	if(iRet == ERROR_SUCCESS)
	{
		m_strLastPlayUrl = strPlaylistUrl;
	}
	
	return iRet;
}

int QPlayerWindow::PlayUrl(const QString & strUrl)
{
	int iOutRet = ERROR_SUCCESS;
	int iRet;
	iRet = ERROR_SUCCESS;
	QString strFileFullPath,strFileType,strOtherPar;
	
	QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
	do
	{
		if(!pQWebBrowserApp)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}	
		strFileFullPath =pQWebBrowserApp->ResolveCommadToUrl(strUrl,strFileType,strOtherPar);
		if(!QFile::exists(strFileFullPath))
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(strFileFullPath.trimmed()=="")
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 == strFileType.compare(FileType_Desc_Playlist, Qt::CaseInsensitive) )
		{
			iOutRet = playPluginPlaylist(strFileFullPath);
			break;
		}		
		QFileInfo playFileInfo(strFileFullPath);
		QString strFileName = playFileInfo.fileName();
		iOutRet = pQWebBrowserApp->PlayUrl(strFileName,strFileType,strOtherPar);		
		if(iOutRet == ERROR_SUCCESS)
		{
			m_strLastPlayUrl = strUrl;
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	return iOutRet;
}

int QPlayerWindow::setPluginWinPosition(int x, int y,int width,int height)
{
	int iRet;
	iRet = ERROR_SUCCESS;
    LOG_LINE("%s->>x:%d,y:%d,width:%d,height:%d\n",__FUNCTION__,x,y,width,height);
	do
	{		
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(pQWebBrowserApp)
		{
			pQWebBrowserApp->setVideoPosition(x, y,width,height);
		}
		else 
		{
			iRet = ERROR_INVALID_STATE;
		}

		
	}while(FALSE);
	
	return iRet;
}


void QPlayerWindow::getInputChannelNumber(int iKeyCode)
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
	}

	int iRet =ERROR_SUCCESS;
	if(m_TunerChannelBar_sp.isNull())
	{
		m_TunerChannelBar_sp = QSharedPointer <QInputChannelWin> (new QInputChannelWin(this));
	}	
	if(m_ChannelNumber.length()==2)
	{
		iRet = m_TunerChannelBar_sp->setText(m_ChannelNumber + QString("-"),1*1000);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	else
	{
		iRet = m_TunerChannelBar_sp->setText(m_ChannelNumber,1*1000);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}
	if(m_ChannelNumber.length() == 3)
	{
		m_ChannelNumber.insert(2, QString("-"));
		m_ChannelNumber = "";
	}	
}

void QPlayerWindow::onPlayPluginListEnd()
{
	emit onPlaybackEnd(m_strLastPlayUrl);
}

QSharedPointer <QPlayPic_Html_Win> QPlayerWindow::getPlayHtmlOrPicWin()
{
	do
	{
		if(m_PlayHtmlOrPicWin.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}		
		m_PlayHtmlOrPicWin->resize(size());
		m_PlayHtmlOrPicWin->setFocus();
		focusNextChild();
	}while(FALSE);
	
	return m_PlayHtmlOrPicWin;
}

void QPlayerWindow::PlayHtmlEntryInPlugin(const QString strUrl,bool isHttp)
{
	int iRet =ERROR_SUCCESS;
	do
	{
		if(m_PlayHtmlOrPicWin.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}		
		m_PlayHtmlOrPicWin->resize(size());
		m_PlayHtmlOrPicWin->playHtmlFile(strUrl,isHttp);
	}while(FALSE);
}

void QPlayerWindow::PlayHtmlEntryInPluginEnd()
{
	do
	{
		if(m_PlayHtmlOrPicWin.isNull())
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}		
		m_PlayHtmlOrPicWin->playHtmlFileEnd();
	}while(FALSE);
}

QSharedPointer <QVideoLayerWnd> QPlayerWindow::getVideoLayerWnd()
{
	int iRet =ERROR_SUCCESS;
	do
	{
		if(m_VideoLayerWnd.isNull())
		{
			m_VideoLayerWnd = QSharedPointer <QVideoLayerWnd> (new QVideoLayerWnd(this));
			bool bRet = false;
			QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
			if(pQWebBrowserApp)
			{
				bRet = connect(&(*m_VideoLayerWnd), SIGNAL(On_KeyMenu_Sig(int)), pQWebBrowserApp, SLOT(On_DefKey_Down(int)));
				if(!bRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
			}
		}	
	}while(FALSE);

	return m_VideoLayerWnd;
}

int QPlayerWindow::ChangeLanguage(const QString & iCurrentLang)
{
	int iRet;
	iRet = ERROR_SUCCESS;

	do
	{		
		if(iCurrentLang.trimmed() == "" )
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(pQWebBrowserApp)
		{
  			pQWebBrowserApp->On_ChangeLanguageDone(iCurrentLang.toInt());
		}
		else 
		{
			iRet = ERROR_INVALID_STATE;
			PRINT_BFILE_LINENO_IRET_STR;
		}
		
	}while(FALSE);
	
	return iRet;
}

