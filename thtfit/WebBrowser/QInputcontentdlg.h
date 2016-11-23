#ifndef QINPUTCONTENTDLG_H
#define QINPUTCONTENTDLG_H

#include <QInputDialog>

class QInputcontentdlg : public QInputDialog
{
	Q_OBJECT

public:
	QInputcontentdlg(QWidget *parent = 0);
	~QInputcontentdlg();
protected:
	void focusOutEvent ( QFocusEvent * event );
private:
	
};

#endif // QINPUTCONTENTDLG_H
