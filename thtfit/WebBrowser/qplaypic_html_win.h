#ifndef QPLAYPIC_HTML_WIN_H
#define QPLAYPIC_HTML_WIN_H

#include <QWidget>
#include "ui_qplaypic_html_win.h"
#include "web_plugin.h"
#include <BaseTypeDef.h>
#include <BaseErrDef.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>

class QPlayPic_Html_Win : public QWidget
{
	Q_OBJECT

public:
	QPlayPic_Html_Win(QWidget *parent = 0);
	~QPlayPic_Html_Win();
	
	void playPictureFile(QString selectFileName);	
	void playHtmlFile(QString htmlFile,bool isPlayHtmlFile = false);
	void playHtmlFileEnd();
	QString getCurrPlayHtmlUrl() { return m_strCurPlayHtmlUrl; }; 
	void setCurrPlayHtmlUrl(QString htmlUrl = "") { m_strCurPlayHtmlUrl = htmlUrl; };
	void  playBootLogo(QString selectFileName,QString strIp,QString strMac);
	void  playBootLogoEnd();
	int  playPluginHtmlEntry(QString strPlayFileUrl) ;	
	INT_t CalcVideoPluginPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h);
	int playPluginPlayList(const QString & strPlayFileUrl) ;
#if 1/*added by lxj 2012-12-17*/
	int setHtmlEvalJavascript(const QString & strHtmlEvalJavascript, QString & strRet );
#endif
	bool isPlayingHtml();
	bool isPlayingPicture();

	QStringList getPlayListFiles(int reqListType);
	QString isPermission(bool &isPerm);
	
public slots:
	void addObjectForJs();
	int   playPluginHtmlEntryEnd_timeOut();
	virtual void DbgLog(const QString & strDbgInfo);
	virtual void On_PageLoadFinished(bool bOK);
	virtual QString RunShellScript(const QString & strShellScript);
	virtual QString CheckIRStatus();
	virtual QString playSelectFile(const QString selectFileName);
	
signals:
	void getPlayListFiles_signal(int reqListType,QStringList & fileNamelist,QStringList & filePathInfoList);
	void playSelectFile_signal(QString selectFileName,int playListType);
	
protected:
	void setWebElementTargetToSelf(QWebFrame * pWebFrame);
private:
	Ui::QPlayPic_Html_Win ui;
	web_plugin m_WebPluginFactory;
	QString m_strCurPlayHtmlUrl;
	QString GetFileMD5(QFile * file);
	bool m_bIsPlayHtml;
	bool m_bIsPlayPicture;
	QStringList m_strFilePathInfoList;
};

#endif // QPLAYPIC_HTML_WIN_H
