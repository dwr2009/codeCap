#include "thtfit_webview.h"
#include <QKeyEvent>
#include <QFocusEvent>
#include <QFile>
#include "WebBrowserApp.h"
#include <DbgLogSwitchDef.h>
#include <QApplication> 
#include <QtCore> 
#include <QtGui> 

class QWebPageEx;

QWebPageEx::QWebPageEx(QObject *parent)
    :QWebPage(parent)
{
    
}

QWebPageEx::~QWebPageEx()
{
    
}

void QWebPageEx::javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID )
{
	if(Sw_LogJavascript)
	{
		LOG("[%s:%d]%s\n", qPrintable(sourceID), lineNumber, qPrintable(message));
	}
}

void QWebPageEx::triggerAction(WebAction action, bool checked)
{    
    
    QWebPage::triggerAction(action,checked);
}

bool QWebPageEx::event(QEvent * ev)
{
    
    return QWebPage::event(ev);
}

QAtomicInt Thtfit_WebView::m_ObjCnt = 0;

Thtfit_WebView::Thtfit_WebView(QWidget *parent)
    :QWebView(parent),
    m_pWebPageEx(new QWebPageEx(this))
{
    
	m_ObjCnt.ref();
    //qWarning("%s->%s(%d)  total Thtfit_WebView is %d\n",__FILE__,__FUNCTION__,__LINE__,m_ObjCnt.load());
	setPage(m_pWebPageEx);	//The parent QObject of the provided page remains the owner of the object.
	settings()->setAttribute(QWebSettings::SpatialNavigationEnabled, true);
	settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls,true);
    settings()->setMaximumPagesInCache(0);
    connect(this, SIGNAL(loadFinished(bool)),this, SLOT(loadHtmlFinished(bool)));
    connect(this, SIGNAL(loadStarted()),this, SLOT(loadHtmlStarted()));
}

Thtfit_WebView::~Thtfit_WebView()
{
    //qWarning("%s->%s(%d) \n",__FILE__,__FUNCTION__,__LINE__);
	m_ObjCnt.deref();
}

void Thtfit_WebView::load(const QUrl& url)
{
    
    //qWarning()<<"Current url is "<<url.url();
    QWebView::load(url);
}


bool Thtfit_WebView::event(QEvent * ev)
{
    
    return QWebView::event(ev);
}


void Thtfit_WebView::loadHtmlFinished(bool ok)
{
    //qWarning("%s->%s(%d) this is %p ok is %d \n",__FILE__,__FUNCTION__,__LINE__,this,ok);
}

void Thtfit_WebView::loadHtmlStarted()
{
    //qWarning("%s->%s(%d) this is %p \n",__FILE__,__FUNCTION__,__LINE__,this);
    settings()->clearMemoryCaches();
    settings()->clearMemoryCaches();
}
void Thtfit_WebView::loadHtmlUrl(const QString & strHtmlUrl)
{
    
	if(strHtmlUrl.trimmed() == "")
	{
		return;
	}
	if(QFile::exists(strHtmlUrl))	//local file
	{
		load(QUrl("file://" + strHtmlUrl));			
	}
	else if(0 == strHtmlUrl.left(7).compare("http://", Qt::CaseInsensitive))	// http:// link
	{
		load(strHtmlUrl);
	}
	else if("/" != strHtmlUrl[0])	// relative path
	{		
		QWebBrowserApp * pQWebBrowserApp =NULL;
		pQWebBrowserApp= dynamic_cast <QWebBrowserApp *> (qApp);
		if(pQWebBrowserApp)
		{			
			QString  strActiveMedia,strActiveMediaDirName;
			QString strFullPath,strMainHtmlUrl;

			//Main html page path  http://xxx?sfsdk=2342423234&sdfsfd=adafd 
			/// /MP7XX/LocalMedia/Html/PlayPluginOverlay_2.htm

			strMainHtmlUrl = pQWebBrowserApp->getCurrMainHtmlUrl();
			if(0 == strMainHtmlUrl.left(7).compare("http://", Qt::CaseInsensitive))
			{
				QString strMainUrl ; 
				int iFlagPos = strMainHtmlUrl.indexOf("?");
				if(iFlagPos >=0)
				{
					strMainUrl = strMainHtmlUrl.mid(0,iFlagPos);
					strFullPath = strMainUrl + strHtmlUrl;
				}
				else
				{ 
					strMainHtmlUrl  =strMainHtmlUrl.mid(0,(strMainHtmlUrl.size() - QUrl(strMainHtmlUrl).path().size())); 
					strFullPath = strMainHtmlUrl + "/" + strHtmlUrl; 
				}
			}
			else
			{
				if(strMainHtmlUrl.trimmed() != "")
				{
					QFileInfo strCurrHtmlFileInfo(strMainHtmlUrl);	
					if(strCurrHtmlFileInfo.exists())
					{
						if(strCurrHtmlFileInfo.fileName().trimmed() !="")
						{
							strMainHtmlUrl  =strMainHtmlUrl.mid(0,(strMainHtmlUrl.size() - strCurrHtmlFileInfo.fileName().size()));
							strFullPath = strMainHtmlUrl + strHtmlUrl;
						}
					}
					else
					{
						pQWebBrowserApp->getActiveMedia(strActiveMedia, strActiveMediaDirName);
						if(qApp->applicationDirPath() ==  "/")
						{
							strFullPath =  qApp->applicationDirPath() +strActiveMediaDirName + "/"+  FOLDER_NAME_HTML +"/"+ strHtmlUrl;
						}
						else
						{
							strFullPath =  qApp->applicationDirPath() +"/" + strActiveMediaDirName + "/"+  FOLDER_NAME_HTML +"/"+ strHtmlUrl;
						}
					}
				}
			}
			
 			if(QFile::exists(strFullPath))
			{
				load(QUrl("file://" + strFullPath));		 
			}
			else
			{
				QUrl htmlUrl(strFullPath);
				
				if(htmlUrl.isValid())
				{
					load(QUrl(htmlUrl));
				}
			}
		}
	}
	else
	{
		QUrl htmlUrl(strHtmlUrl);
		if(htmlUrl.isValid())
		{
			load(QUrl(strHtmlUrl));
		}
	}	
}

int Thtfit_WebView::setAlpha(float fAlphaVal)
{
	int iRet = 0;
    
	
	do
	{
		if(fAlphaVal > 1.0   || fAlphaVal < 0.0)
		{
			break;
		}
		setStyleSheet(QString("background-color: rgba(0, 0 , 0, %1);").arg(QString::number(fAlphaVal * 255)));	
	}while(FALSE);

	return iRet;
}

int Thtfit_WebView::getObjCnt()
{
	return m_ObjCnt.load();
}


/*
void Thtfit_WebView::mouseMoveEvent ( QMouseEvent * ev )
{
	if(m_bIsPressMove)
	{
		QWheelEvent  pWheel (ev->pos(),15, Qt::LeftButton, Qt::NoModifier,  Qt::Vertical );
		//scroll(0,ev->pos().y() +20);
		this->page()->mainFrame()->setScrollBarValue(Qt::Vertical,page()->mainFrame()->scrollBarValue(Qt::Vertical)+10);
	}
}

void Thtfit_WebView::mousePressEvent ( QMouseEvent * ev )
{	
	m_bIsPressMove = true;
	QWebView::mousePressEvent(ev);
}

void Thtfit_WebView::mouseReleaseEvent  ( QMouseEvent * ev )
{
	m_bIsPressMove = false;
	QWebView::mouseReleaseEvent(ev);
}

*/
