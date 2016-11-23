#ifndef QPLAYERWINDOW_H
#define QPLAYERWINDOW_H

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
#include "qvideolayerwnd.h"

class QPlayerWindow : public QWidget
{
	Q_OBJECT
public:
	QPlayerWindow(QWidget *parent=0);
	~QPlayerWindow();
public slots:
	void PlayPluginListHtmlEntry(QString strPlayFileUrl);
	void PlayPluginListHtmlEntryEnd();
	int playPluginPlaylist(const QString & strPlaylistPath);
	int PlayUrl(const QString & strUrl);
	int setPluginWinPosition(int x, int y,int width,int height);
	void onPlayPluginListEnd();
	void PlayHtmlEntryInPlugin(const QString strUrl,bool isHttp);
	void PlayHtmlEntryInPluginEnd();	
	int ChangeLanguage(const QString & iCurrentLang);
public:
	QSharedPointer <QPlayPic_Html_Win> getPlayHtmlOrPicWin();
	QSharedPointer <QVideoLayerWnd> getVideoLayerWnd();
signals:
	void On_KeyMenu_Sig(int keyValue);	
	void On_KeyMenuLongPress_Sig(int keyValue);
	void onPlaybackEnd(QString strPlayLastUrl);
protected:
	virtual void paintEvent ( QPaintEvent * event );
	virtual void keyPressEvent(QKeyEvent * event) ;	
	virtual void keyReleaseEvent(QKeyEvent * event) ;
private:	
	void  createWebView();
	int StopPlayCurrPluginList();
	void getInputChannelNumber(int iKeyCode);
private:
	QTime m_OnKeyDownTime;
	QTime m_OnPlayKeyDownTime;
	bool  m_bKeyIsRelease;
	bool  m_bIsFirstPress;	
	bool m_bIsNeedSendSigByActiveWin;
	QSharedPointer <QWebView> m_WebView_sp;
	int m_dOpticeValue;
	QString m_ChannelNumber;	
	QSharedPointer <QInputChannelWin> m_TunerChannelBar_sp;
	QString m_strLastPlayUrl;
	QSharedPointer <QPlayPic_Html_Win> m_PlayHtmlOrPicWin;
	QSharedPointer <QVideoLayerWnd> m_VideoLayerWnd;
};

#endif // QPLAYERWINDOW_H

