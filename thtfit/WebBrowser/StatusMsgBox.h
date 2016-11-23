#ifndef	_STATUS_MSG_BOX_H_
#define	_STATUS_MSG_BOX_H_

#include <QMessageBox>
#include <QTimer>
#include <BaseTypeDef.h>

class QStatusMsgBox : public QMessageBox
{
	Q_OBJECT
public:
	QStatusMsgBox( QWidget * parent = 0 );
	QStatusMsgBox(Icon icon, const QString & strTitle, const QString & strText, 
		StandardButtons buttons = NoButton, QWidget * parent = 0, Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
	virtual ~QStatusMsgBox();
	INT_t Show(const QString & strTitle, const QString & strContent, INT_t iTimeoutMs = -1);
private:
	INT_t CommonConstruct();
public slots:
	void HideMsgBox();
private:
	QTimer timerTimeoutHide;
};

#endif	//_STATUS_MSG_BOX_H_
