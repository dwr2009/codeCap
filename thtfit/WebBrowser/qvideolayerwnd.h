#ifndef QVIDEOLAYERWND_H
#define QVIDEOLAYERWND_H

#include <QWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QSharedPointer>
#include <QtWebKitWidgets/QWebView>
#include <BaseErrDef.h>
#include "QWbStatusBar.h"
#include "qplaypic_html_win.h"

class QVideoLayerWnd : public QWidget
{
	Q_OBJECT

public:
	QVideoLayerWnd(QWidget *parent = 0);
	~QVideoLayerWnd();
public:
	QSharedPointer <QPlayPic_Html_Win> getPlayHtmlOrPicWin();
	void PlayHtmlEntryOnVideoLayer(QString strPlayFileUrl,bool isHttp = false);
	void PlayHtmlEntryOnVideoLayerEnd();
signals:
	void On_KeyMenu_Sig(int keyValue);	
protected:
	virtual void keyPressEvent(QKeyEvent * event) ;
	virtual void paintEvent ( QPaintEvent * event );
private:
	void  createWebView();
	int StopPlayCurrPluginList();
private:	
	QSharedPointer <QWebView> m_WebView_sp;	
};

#endif // QVIDEOLAYERWND_H
