#ifndef BROWSER_H
#define BROWSER_H

#include <QtWidgets/QtWidgets>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QNetworkInterface>
#include <QDesktopServices>
#include <QtWebKitWidgets/QWebFrame>
#include <QPainter>
#include <QKeyEvent>
#include "ui_browser.h"
#include "web_plugin.h"
#include <QWidgetEx.h>
#include <QDate>
#include <QTime>
#include <QFile>
#include <QWebHistory>

#define OSD_PATH 		"GuiDef/osd.htm"
#define WIRELESSNAME_PATH_FILE   "UserData/data/wirelessName.txt"

class browser : public QWidget
{
	Q_OBJECT

public:
	browser(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~browser();
	void initSys();
	INT_t CalcVideoPluginPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h);
	INT_t getShitfPosition(INT_t & x, INT_t & y, INT_t & w, INT_t & h);
	INT_t PlayHtmlEntryInPlugin(QString strUrl); 
	INT_t PlayHtmlEntryInPluginEnd();
#if 1/*added by lxj 2012-8-3 for refresh video mode*/
	void RefreshUiVideoMode(void);
#endif	
	void RefreshOsdUi(void);
	INT_t getTimeFuc(void);
	INT_t MibsetSysTime(void);

protected:
	virtual bool eventFilter(QObject *target, QEvent *event);
    virtual void paintEvent ( QPaintEvent * event );
	virtual void keyPressEvent(QKeyEvent * event) ;	
	virtual void keyReleaseEvent(QKeyEvent * event) ;
public slots:
	void addJavaScriptObject();
	QStringList GetInformationMsg();
	QStringList getPlayListFiles(int reqListType);
	QStringList GetPlayListFileInfo();
	void playSelectFile(const QString selectFileName,int playListType);
	void SwitchToPlayWin();
	QStringList GetDhcpInfo(int isGetRealTimeNetInfo  = 1);
	void turnVideoSetting(const QString videoType,int videoValue);
	void SaveDhcpInfo(QStringList tempDhcpInfo,bool isAutoGetIp);
	QStringList GetSystemTime();
	void SetSystemTime(QStringList tempTimeInfoList);
	QStringList getVideoSettingMsg();
	void RefreshSnmpPassWordResult(int iResutlt);
	QStringList getWirelessStatus();
	QStringList getWirelessDhcpInfo();
	int SaveWirelessStatusInfo(QStringList wirelessStatutsInfo);
	int SaveWirelessNetworkAddrInfo(QStringList tempDhcpInfo,bool isAutoGetIp);
	//int saveUsefulWirelessName(QString UsefulWirelessName);
	//int getUsefulWirelessName();
	#if 0/*by_su*/
	void ChangeLanguage(int index);
	#endif
signals:
	void GetMonitorTypeInfo_Sig(QStringList & infoList);
	void getPlayListFiles_signal(int reqListType,QStringList & fileNamelist,QStringList & filePathInfoList);
	void playSelectFile_signal(QString selectFileName,int playListType);
	void SwitchWin_signal();
	void reqDhcpInfo_signal(QStringList & dhcpInfoList);
	void saveDhcpInfo_signal(QStringList dhcpInfoList,bool isAutoGetIp);
	void SetVideoPar_signal(QString videoParType,int parValue);
	void reqNetWorkInfo_signal(QStringList & dhcpInfoList);
	void SetSystemCurTime_Sig(QStringList  timeInfoList);
	//add
	void SetCurTimeZone(int  timezoneInfo);

	//
	void GetSysCurrTimeZone_Sig(int & curTimeZone);
	void On_Key_Sig(int);
	void SIGNAL_Evt_ChangeLanguage(int index);
private:
	Ui::browserClass ui;
	web_plugin m_WebPluginFactory;
	QStringList m_strFilePathInfoList;
};

#endif // BROWSER_H
