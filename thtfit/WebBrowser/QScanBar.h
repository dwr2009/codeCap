#ifndef QSCANBAR_H
#define QSCANBAR_H

#include <QProgressBar>

class QScanBar : public QProgressBar
{
	Q_OBJECT

public:
	QScanBar(QWidget *parent = 0);
	~QScanBar();

private:
	
};

#endif // QSCANBAR_H
