#include <UpgradeRebootConfirmDlg.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

QUpgradeRebootConfirmDlg::QUpgradeRebootConfirmDlg()
{
	m_iConfirmTimeoutMs = (10*1000);
}

QUpgradeRebootConfirmDlg::~QUpgradeRebootConfirmDlg()
{
	bool bRet;
	
	bRet = disconnect(&m_ConfirmTimer, SIGNAL(timeout()), this, SLOT(onConfirmTimeout()));
	if(false == bRet)
	{
		LOG_BLINE("disconnect sig err\n");
	}
}

INT_t QUpgradeRebootConfirmDlg::setTimeoutConfirm(CONST INT_t iTimeoutMs)
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;

	bRet = disconnect(&m_ConfirmTimer, SIGNAL(timeout()), this, SLOT(onConfirmTimeout()));
	if(false == bRet)
	{
		//LOG_BLINE("disconnect sig err\n");
	}

	m_iConfirmTimeoutMs = iTimeoutMs;

	bRet = connect(&m_ConfirmTimer, SIGNAL(timeout()), this, SLOT(onConfirmTimeout()));
	if(false == bRet)
	{
		LOG_BLINE("connect sig err\n");
	}

	return iOutRet;
}

int QUpgradeRebootConfirmDlg::exec()
{
	int iOutRet = 0, iRet;

	setWindowTitle("Upgrade");
	setText("The new version is ready.");
	setInformativeText("Do you want to reboot for update now?");
	setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
	setDefaultButton(QMessageBox::Yes);

	m_ConfirmTimer.start(m_iConfirmTimeoutMs);
	
	iRet = QMessageBox::exec();
	iOutRet = iRet;

	m_ConfirmTimer.stop();

	return iOutRet;
}

void QUpgradeRebootConfirmDlg::onConfirmTimeout()
{
	accept();
	setResult(QMessageBox::Yes);
}

