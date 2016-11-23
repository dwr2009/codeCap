#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QButtonGroup>
#include <Qt>
#include <QMouseEvent>
#include <QtGui/QInputMethod>

#if 1/*by su for test*/
#include <QtWidgets/QLineEdit>
#include <QFile>
#include <QRegExp>
#include "chinesebar.h"

#define EnglishPath  "/MP7XX/English.qm"
#define ChinesePath  "/MP7XX/English.qm"
#define ArabicPath  "/MP7XX/Arabic.qm"
#define  SpanishPath  "/MP7XX/Spanish.qm"

//define by rocky
#define	PY_IME_DICTIONARY	"/MP7XX/py.txt"
#define	PY_IME_SERCHFILE	"/MP7XX/serch.txt"





typedef enum
{
	Language_TYPE_ENGLISH,
	Language_TYPE_CHINESE,
	Language_TYPE_ARABIC,
	Language_TYPE_SPANISH,
}Language_TYPE;
#endif

#define PAGE_INDEX_ONE  0
#define PAGE_INDEX_TWO 1

namespace Ui {
    class InputWidget;
}

class InputWidget : public QWidget
{
    Q_OBJECT

public:
	explicit InputWidget(QInputMethod* im);//IPMethod * ipm,
	~InputWidget();
	void InstallDefaultLanguage(int index);
private:
	void SetUpKeyBoard();
public slots:
	void ButtonJudge(int);
	void on_key_Shift_clicked();
	void On_SwitchInputPanel_Clicked();
	//QString unicodeToString(QString str);
#if 1/*by su for test*/
	void InitChineseBar();
	void OpenPinyingFile();
	void HideBar();
	QString TransPy(QString pyInput);
	void ShowPage(int index);
	void PageUp();
	void PageDown();
	void SetTextEmpty();
	void ChineseSelectFont_1();
	void ChineseSelectFont_2();
	void ChineseSelectFont_3();
	void ChineseSelectFont_4();
	void ChineseSelectFont_5();
	void ChineseSelectFont_6();
	void ChineseSelectFont_7();
	void ChineseSelectFont_8();
#endif
signals:
	void SendInputMessage(QString);
	void SendEnter();
	void SendBackspace();
	void SenEscape();
	void SendCurrKeyValue(int);
	void CapsLock_Clicked();
private:
	Ui::InputWidget *ui;
	QButtonGroup *button_group;
	bool m_SwitchCapsLock;
#if 1/*by su for test*/
	QLineEdit *keyRev;
	QString m_strInputPy;
	QFile m_PyImeDicFile;
	QFile m_PyImeSerchFile;
	QRegExp regExp;

	Ui::chineseBar *m_barUi;
	chineseBar *bar;
	int m_currentPage;
	int m_pageCount;
	int m_currentLanguage;
	bool findNothing;
#endif	
};

#endif // INPUTWIDGET_H
