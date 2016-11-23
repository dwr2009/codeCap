#include "StatusMsgBox.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <QDesktopWidget>
#include <QApplication>

INT_t QStatusMsgBox::CommonConstruct()
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	
	bRet = connect(&timerTimeoutHide, SIGNAL(timeout()), this, SLOT(HideMsgBox()));
	if(true == bRet)
	{
	}
	else
	{
		LOG_BLINE("Timer.Conn failed\n");
	}

	return iOutRet;
}

QStatusMsgBox::QStatusMsgBox( QWidget * parent/* = 0 */) : QMessageBox(parent)
{
	CommonConstruct();
}

QStatusMsgBox::QStatusMsgBox(Icon icon, const QString & strTitle, const QString & strText, 
	StandardButtons buttons/* = NoButton*/, QWidget * parent/* = 0*/, Qt::WindowFlags f/* = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint*/) :
	QMessageBox(icon, strTitle, strText, buttons, parent, f)
{
	CommonConstruct();	
	setStandardButtons(buttons);
	setEscapeButton(QMessageBox::NoButton);
	Qt::WindowFlags wFlags = windowFlags();	
	if(Qt::WindowCloseButtonHint == (wFlags & Qt::WindowCloseButtonHint))
	{
		wFlags = wFlags ^ Qt::WindowCloseButtonHint;
		setWindowFlags(wFlags);
	}
}

QStatusMsgBox::~QStatusMsgBox()
{
}

INT_t QStatusMsgBox::Show(const QString & strTitle, const QString & strContent, INT_t iTimeoutMs/* = -1*/)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{					
		show();
		setWindowTitle(strTitle);
		setText(strContent);
		if(timerTimeoutHide.isActive())
		{
			timerTimeoutHide.stop();
		}
		if(0 <= iTimeoutMs)
		{
			timerTimeoutHide.setSingleShot(TRUE);
			timerTimeoutHide.start(iTimeoutMs);
		}
		QDesktopWidget* desktop = QApplication::desktop();
		move((desktop->width() - this->width())/2, (desktop->height() - this->height())/2);	
	}while(FALSE);

	return iOutRet;
}

void QStatusMsgBox::HideMsgBox()
{
	//LOG_BLINE("QStatusMsgBox::HideMsgBox\n");
	hide();	
}

