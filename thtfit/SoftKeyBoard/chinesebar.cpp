#include "chinesebar.h"
#include "ui_chinesebar.h"

chineseBar::chineseBar(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::chineseBar)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

}

chineseBar::~chineseBar()
{
    delete m_ui;
}

Ui::chineseBar* chineseBar::GetChineseBarUi()
{
	return m_ui;
}

void chineseBar::ChangeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
