#include "QScanProgressBar.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QObject>


#define	QWB_STATUS_BAR_HEIGHT				30
#define	QWB_STATUS_BAR_BOTTOM_DISTANCE		30

#define AutoHideTimeOut 5*60 * 1000

QScanProgressBar::QScanProgressBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	Qt::WindowFlags	curWinFlags = windowFlags();	
	setWindowFlags(curWinFlags  | Qt::FramelessWindowHint);

	//focus policy
	//setFocusPolicy(Qt::NoFocus);
	//set window size and position
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		rcThisWidget.adjust(0, (AvailableDestiopGeometry.height() - QWB_STATUS_BAR_HEIGHT - QWB_STATUS_BAR_BOTTOM_DISTANCE), 
			0, -QWB_STATUS_BAR_BOTTOM_DISTANCE);
		setGeometry(rcThisWidget);
	}

	bool bRet= true;
	bRet = this->connect(&m_timerTimeoutHide, SIGNAL(timeout()), this, SLOT(On_TimeoutHide()));
	if(false == bRet)
	{
	//	LOG_BLINE("ConnSig(TimeoutRemvoe) err\n");
	}
	else
	{
		 if(m_timerTimeoutHide.isActive())
		 {
		 	m_timerTimeoutHide.stop();
		 }
		 m_timerTimeoutHide.start(AutoHideTimeOut);
	}
}

QScanProgressBar::~QScanProgressBar()
{

}


void QScanProgressBar::setValue(int iProgressValue,int iPgmNum)
{
	 ui.ScanProgressBar->setValue(iProgressValue);
	 ui.lab_ProgramNumber->setText(QString::number(iPgmNum));
	 if(m_timerTimeoutHide.isActive())
	 {
	 	m_timerTimeoutHide.stop();
	 }
	 m_timerTimeoutHide.start(AutoHideTimeOut);
}

void QScanProgressBar::On_TimeoutHide()
{
	hide();
}

