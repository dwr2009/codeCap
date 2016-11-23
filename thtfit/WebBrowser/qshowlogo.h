#ifndef QSHOWLOGO_H
#define QSHOWLOGO_H

#include <QWidget>
#include "ui_qshowlogo.h"
#include <QObject>

class QShowLogo : public QWidget
{
	Q_OBJECT

public:
	QShowLogo(QWidget *parent = 0);
	~QShowLogo();
	void  playBootLogo(QString selectFileName,QString strIp,QString strMac);
	void  playBootLogoEnd();

private:
	Ui::QShowLogo ui;
};

#endif // QSHOWLOGO_H
