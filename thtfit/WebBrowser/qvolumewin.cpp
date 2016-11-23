#include "qvolumewin.h"
#include <QDesktopWidget>

QVolumeWin::QVolumeWin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

	//setWindowOpacity(0.8);

	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	QRect DesktopRect = pDesktopWidget->screenGeometry();
	this->setGeometry(DesktopRect.width() / 2 -200, DesktopRect.height() -120,400,100);

}

QVolumeWin::~QVolumeWin()
{
}

void QVolumeWin::UpdateNewVolume(int nVolume)
{
	ui.lab_Volume->setText(QString::number(nVolume));
}

