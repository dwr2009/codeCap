#ifndef QVOLUMEWIN_H
#define QVOLUMEWIN_H

#include <QWidget>
#include <QObject>
#include "ui_qvolumewin.h"

class QVolumeWin : public QWidget
{
	Q_OBJECT

public:
	QVolumeWin(QWidget *parent = 0);
	~QVolumeWin();

	void UpdateNewVolume(int nVolume);
private:
	Ui::QVolumeWin ui;
};

#endif // QVOLUMEWIN_H
