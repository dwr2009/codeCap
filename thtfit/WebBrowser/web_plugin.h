#ifndef WEB_PLUGIN_H
#define WEB_PLUGIN_H

#include <QWebPluginFactory>
#include "PlaylistData.h"
#include "thtfit_webview.h"

class web_plugin : public QWebPluginFactory 
{
	Q_OBJECT

public:
	web_plugin(QObject *parent=0);
	~web_plugin();
protected:
	virtual QObject* create(const QString& mimeType,const QUrl& url,const QStringList& argumentNames, 
		const QStringList& argumentValues) const; 
	virtual QList<QWebPluginFactory::Plugin> plugins() const;
private:
};

#endif // WEB_PLUGIN_H

