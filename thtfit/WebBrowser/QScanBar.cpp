#include "QScanBar.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QObject>

#define	QWB_STATUS_BAR_HEIGHT				30
#define	QWB_STATUS_BAR_BOTTOM_DISTANCE		30

QScanBar::QScanBar(QWidget *parent)
	: QProgressBar(parent)
{
	Qt::WindowFlags	curWinFlags = windowFlags();	
	setWindowFlags(curWinFlags | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);	
}

QScanBar::~QScanBar()
{

}

