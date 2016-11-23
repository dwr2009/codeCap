#include "web_plugin.h"
#include <QPushButton>
#include <QLabel>
#include <QUrl>
#include <QList>
#include <QFile>
#include <ErrPrintHelper.h>
#include <QPaintEngine>
#include <BaseErrDef.h>
#include "IrKeyQtKeyMap.h"
#include "WebBrowserApp.h"
#include "qplayerwindow.h"

web_plugin::web_plugin(QObject *parent)
	: QWebPluginFactory (parent)
{
}

web_plugin::~web_plugin()
{

}

QObject* web_plugin::create(const QString& mimeType, 
						  const QUrl& url, 
						  const QStringList& argumentNames, 
						  const QStringList& argumentValues) const 
{ 

	QObject* pPluginObj = NULL;
	
	Q_UNUSED(argumentNames);   
	Q_UNUSED(argumentValues);
	Q_UNUSED(url);

	do
	{
		if("application/x-coolcatplugin" == mimeType)
		{
			QPlayerWindow * pPlayerWindow = NULL;
			pPlayerWindow = new QPlayerWindow;
			if(NULL == pPlayerWindow)
			{
				break;
			}
			pPluginObj = pPlayerWindow;
		}
		else if("application/x-webview" == mimeType)
		{
			Thtfit_WebView * pHtmlView = NULL;
			pHtmlView = new Thtfit_WebView;
			if(NULL == pHtmlView)
			{
				break;
			}			
			//QtWebPlugin is not affected by QWebSettings::PluginsEnabled.
			pHtmlView->settings()->setAttribute(QWebSettings::PluginsEnabled, false); 
			pHtmlView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);	
			//No need to open a new web window
			pHtmlView->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);	
			//Currently Java applets are not supported.
			pHtmlView->settings()->setAttribute(QWebSettings::JavaEnabled, false);
			//Page navigation using arrow keys
			pHtmlView->settings()->setAttribute(QWebSettings::SpatialNavigationEnabled, true);
			//=>out return
			pPluginObj = pHtmlView;
		}
	}while(FALSE);

	return pPluginObj; 
} 

QList<QWebPluginFactory::Plugin> web_plugin::plugins() const 
{ 
	QList<Plugin> list; 
	QWebPluginFactory::Plugin entry; 
	
	entry.name = "QtWebKitPlayerPlugin"; 
	entry.description = "QtWebKitPlayerPlugin"; 
	list.push_back(entry);
	
	return list; 
}

