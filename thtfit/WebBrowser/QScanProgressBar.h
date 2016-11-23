#ifndef QSCANPROGRESSBAR_H
#define QSCANPROGRESSBAR_H

#include <QWidget>
#include "ui_qscanprogressbar.h"
#include <QTimer>

class QScanProgressBar : public QWidget
{
	Q_OBJECT

public:
	QScanProgressBar(QWidget *parent = 0);
	~QScanProgressBar();
public:
	void setValue(int iProgressValue,int iPgmNum);
private:
	Ui::QScanProgressBar ui;
public slots:
	void On_TimeoutHide();
private:
	QTimer m_timerTimeoutHide;
};

#endif // QSCANPROGRESSBAR_H
