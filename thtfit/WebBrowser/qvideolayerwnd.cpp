#include "qvideolayerwnd.h"

#include "WebBrowserApp.h"
#include "IrKeyQtKeyMap.h"

QVideoLayerWnd::QVideoLayerWnd(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowOpacity(1);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_PaintOnScreen);   //WA_PaintOnScreen is not supported by QWS
	//setAttribute(Qt::WA_NativeWindow);
	setFocusPolicy(Qt::NoFocus);
	createWebView();
}

QVideoLayerWnd::~QVideoLayerWnd()
{
}

void QVideoLayerWnd::paintEvent ( QPaintEvent * event )
{
	QPainter Painter(this); 
	Painter.setCompositionMode(QPainter::CompositionMode_Clear);
	Painter.fillRect(0, 0, width(), height(), Qt::SolidPattern);
	Painter.end();
	QWidget::paintEvent(event);
}

void QVideoLayerWnd::keyPressEvent(QKeyEvent * event) 
{
	int iKeyCode = event->key();

	switch(iKeyCode)
	{
	case IR_KEY_MENU:
		{
			//emit On_KeyMenu_Sig(IR_KEY_MENU);	
		}
		break;
	case IR_KEY_STOP:
		emit On_KeyMenu_Sig(IR_KEY_STOP);
		break;
	case IR_KEY_RIGHT:
		emit On_KeyMenu_Sig(IR_KEY_RIGHT);
		break;
	case IR_KEY_LEFT:
		emit On_KeyMenu_Sig(IR_KEY_LEFT);
		break;
	case IR_KEY_CHANGERESOLUTION:
		emit On_KeyMenu_Sig(IR_KEY_CHANGERESOLUTION);
		break;
	default:
		QWidget::keyPressEvent(event);
		break;
	}
	
}

void QVideoLayerWnd::createWebView()
{
	int iRet;
	iRet = 0;
	do
	{		
		/**/m_WebView_sp  = QSharedPointer <QWebView> (new QWebView(this)); 
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
		
		QWebBrowserApp * pQWebBrowserApp = dynamic_cast <QWebBrowserApp *> (qApp);
		if(!pQWebBrowserApp)
		{
			iRet = ERROR_OUT_OF_MEMORY;
			LOG_BLINE("ERROR_OUT_OF_MEMORY err\n");
			break;
		}
		/*
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
		*/
	}while(FALSE);
}


void QVideoLayerWnd::PlayHtmlEntryOnVideoLayer(QString strPlayFileUrl,bool isHttp)
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

void QVideoLayerWnd::PlayHtmlEntryOnVideoLayerEnd()
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
	}while(FALSE);
}


