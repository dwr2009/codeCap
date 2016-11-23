#include "qshowlogo.h"

QShowLogo::QShowLogo(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

QShowLogo::~QShowLogo()
{

}

void  QShowLogo:: playBootLogo(QString selectFileName,QString strIp,QString strMac)
{
	
	ui.logoPic_lab->setPixmap(QPixmap(selectFileName));
	ui.ipMacAddr_lab->setText("IP :" + strIp + " MAC :" + strMac);
}

void  QShowLogo:: playBootLogoEnd()
{
	this->showMinimized();
	this->hide();
	ui.logoPic_lab->clear();
	ui.logoPic_lab->setPixmap(QPixmap(""));
	ui.ipMacAddr_lab->setText("");
	ui.ipMacAddr_lab->setVisible(false);
}