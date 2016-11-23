#ifndef THTFIT_WEBVIEW_H
#define THTFIT_WEBVIEW_H

#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QtDebug>
#include <QWebElement>
#include <QScriptEngine>
#include <QWebPage>

class QWebPageEx : public QWebPage
{
    Q_OBJECT
public:
	QWebPageEx(QObject *parent = 0);
	virtual ~QWebPageEx();
    virtual void triggerAction(WebAction action, bool checked = false);
protected:
    virtual bool event(QEvent * ev);
	void javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID );	
};

class Thtfit_WebView : public QWebView
{
	Q_OBJECT

public:
	Thtfit_WebView(QWidget *parent =0);
	virtual ~Thtfit_WebView();
    void load(const QUrl& url);
	static int getObjCnt(); 
protected:
    virtual bool event(QEvent * ev);
public slots:
	void loadHtmlUrl(const QString & strHtmlUrl);
	int setAlpha(float fAlphaVal = 0.5);
    void loadHtmlFinished(bool ok);
    void loadHtmlStarted();
//protected:
	//virtual void mouseMoveEvent ( QMouseEvent * ev );
 	//virtual void mousePressEvent ( QMouseEvent * ev );
	//virtual void mouseReleaseEvent  ( QMouseEvent * ev );
private:
	static QAtomicInt m_ObjCnt;
	bool 	m_bIsPressMove;
	QWebPageEx *m_pWebPageEx;
};

#endif // THTFIT_WEBVIEW_H

