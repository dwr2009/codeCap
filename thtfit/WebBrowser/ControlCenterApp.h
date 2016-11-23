#ifndef	_CONTROL_CENTER_APP_H_
#define	_CONTROL_CENTER_APP_H_

#include <QApplication>
#include <Mp7xxGlobalStatus.h>
#include "Api/WebBrowserAppDef.h"

class QControlCenterApp : public QApplication
{
	Q_OBJECT		
public:
	QControlCenterApp(int & argc, char ** argv);
	virtual ~QControlCenterApp();
	virtual CMp7xxGlobalStatus & getMp7xxGlobalStatusObj() = 0;
	virtual int On_FsMountPointEvent(ControlCenter::FS_MOUNT_EVENT_ID eFsMountEvtId,
		const QString & strFsMntSymFilePath);
	virtual INT_t UpdateDevMountStatus();
	virtual INT_t RunShellScript(const QString & strShellScript, QString & strResult);
	virtual int CheckIRStatus(int, int);
	virtual VOID On_GpioBtnKey(BYTE BtnKeyCode) = 0;
#if 1/*by_su*/
	virtual int On_ChangeLanguageDone(int);
#endif
signals:
	void onGpioBtnEvt(int iKeyCode);
public slots:
	virtual int setVideoMode(const int eCeSnmpVidMode) = 0;
	virtual int setInternalActionOnGpioBtnEvt(const bool bEnable);
	virtual int setHdmi3DTvModeSlot(const int Hdmi3DTvMode) = 0;
protected:	
	CMp7xxGlobalStatus m_Mp7xxGlobalStatusData;
	bool m_bInternalActionOnGpioBtnEvt;
};

#endif	//_CONTROL_CENTER_APP_H_

