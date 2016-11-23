#if	!defined(_UpgradeRebootConfirmDlg_h_)
#define	_UpgradeRebootConfirmDlg_h_

#include <QMessageBox>
#include <BaseTypeDef.h>
#include <QTimer>
#include <QObject>

class QUpgradeRebootConfirmDlg : QMessageBox
{
	Q_OBJECT	
public:
	QUpgradeRebootConfirmDlg();
	virtual ~QUpgradeRebootConfirmDlg();
	INT_t setTimeoutConfirm(CONST INT_t iTimeoutMs);
	int exec();
public slots:
	void onConfirmTimeout();
private:
	INT_t m_iConfirmTimeoutMs;
	QTimer m_ConfirmTimer;
};

#endif	//_UpgradeRebootConfirmDlg_h_
