#ifndef CHINESEBAR_H
#define CHINESEBAR_H

#include <QtWidgets/QtWidgets>

namespace Ui {
    class chineseBar;
}

class chineseBar : public QWidget {
    Q_OBJECT
public:
    chineseBar(QWidget *parent = 0);
    ~chineseBar();
    Ui::chineseBar* GetChineseBarUi();
private:
    Ui::chineseBar *m_ui;

protected:
    void ChangeEvent(QEvent *e);

};

#endif // CHINESEBAR_H
