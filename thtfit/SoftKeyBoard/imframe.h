/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#ifndef IMFRAME_H
#define IMFRAME_H

#include <QtGui/QInputMethod>
#include "inputwidget.h"

class IMFrame:public QInputMethod
{
	Q_OBJECT
public:
	IMFrame();
	~IMFrame();
	void updateHandler(int);
	void On_ChangeLanguage(int);
private:
	InputWidget* inputwidget;
	QString m_strContent;
	int m_iIsCapital;
	bool m_IsFisrtReqShow;
private slots:
	void sendContent(const QString&);
	void confirmContent();
	void BackspaceContent();
	void Escape();
	void OnKey_Go_Clicked();
	void On_CapsLock_Clicked();
};

#endif
