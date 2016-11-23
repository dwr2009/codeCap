#include "inputwidget.h"
#include "ui_inputwidget.h"
#include <QTextCodec>
#include <QString>
#include <QtDebug>
#include <QtWidgets/QTextEdit>
#include <QTranslator>
#include <QCoreApplication>

#if 1/*by su for Chinese input method*/
#include <QtWidgets/QMessageBox>
#include "ui_chinesebar.h"
#endif

#define FALSE 0

InputWidget::InputWidget(QInputMethod* im) :
QWidget(0, Qt::Tool| Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint),
ui(new Ui::InputWidget)
{	
	ui->setupUi(this);
	m_SwitchCapsLock = false;
	this->button_group=new QButtonGroup;
	this->SetUpKeyBoard();
	
#if 1/*by su for Chinese input method*/
	InitChineseBar();
#endif
	
	QObject::connect(this->button_group,SIGNAL(buttonClicked (int)),this,SLOT(ButtonJudge(int)));
	connect(this, SIGNAL(SendInputMessage(QString)),  im, SLOT(sendContent(const QString&)));
	connect(this, SIGNAL(SendEnter()),  im, SLOT(OnKey_Go_Clicked()));
	connect(this, SIGNAL(SendBackspace()),  im, SLOT(BackspaceContent()));
	connect(this,SIGNAL(SenEscape()),im,SLOT(Escape()));
	connect(this,SIGNAL(CapsLock_Clicked()),im,SLOT(On_CapsLock_Clicked()));
	connect(ui->key_DigitalOrABC,SIGNAL(clicked()),this,SLOT(On_SwitchInputPanel_Clicked()));
}

InputWidget::~InputWidget()
{
	if(button_group)
	{
		delete this->button_group;
	}
	if(ui)
	{
		delete ui;
	}
#if 1/*by su for Chinese input method*/	
	if(bar)
	{
		delete bar;
	}
	if(m_barUi)
	{
		delete m_barUi;
	}
	if(keyRev)
	{
		delete keyRev;
	}
#endif
}

void InputWidget::SetUpKeyBoard()
{
	if(button_group == NULL)
	{
		button_group=new QButtonGroup;
	}
	this->button_group->addButton(ui->key_0,Qt::Key_0);
	this->button_group->addButton(ui->key_1,Qt::Key_1);
	this->button_group->addButton(ui->key_2,Qt::Key_2);
	this->button_group->addButton(ui->key_3,Qt::Key_3);
	this->button_group->addButton(ui->key_4,Qt::Key_4);
	this->button_group->addButton(ui->key_5,Qt::Key_5);
	this->button_group->addButton(ui->key_6,Qt::Key_6);
	this->button_group->addButton(ui->key_7,Qt::Key_7);
	this->button_group->addButton(ui->key_8,Qt::Key_8);
	this->button_group->addButton(ui->key_9,Qt::Key_9);

	this->button_group->addButton(ui->key_A,Qt::Key_A);
	this->button_group->addButton(ui->key_B,Qt::Key_B);
	this->button_group->addButton(ui->key_C,Qt::Key_C);
	this->button_group->addButton(ui->key_D,Qt::Key_D);
	this->button_group->addButton(ui->key_E,Qt::Key_E);
	this->button_group->addButton(ui->key_F,Qt::Key_F);
	this->button_group->addButton(ui->key_G,Qt::Key_G);
	this->button_group->addButton(ui->key_H,Qt::Key_H);
	this->button_group->addButton(ui->key_I,Qt::Key_I);
	this->button_group->addButton(ui->key_J,Qt::Key_J);
	this->button_group->addButton(ui->key_K,Qt::Key_K);
	this->button_group->addButton(ui->key_L,Qt::Key_L);
#if 1/*by su for Chinese input method*/
	this->button_group->addButton(ui->key_N_2,Qt::Key_F35);
#endif
	this->button_group->addButton(ui->key_M,Qt::Key_M);
	this->button_group->addButton(ui->key_N,Qt::Key_N);
	this->button_group->addButton(ui->key_O,Qt::Key_O);
	this->button_group->addButton(ui->key_P,Qt::Key_P);
	this->button_group->addButton(ui->key_Q,Qt::Key_Q);
	this->button_group->addButton(ui->key_R,Qt::Key_R);
	this->button_group->addButton(ui->key_S,Qt::Key_S);
	this->button_group->addButton(ui->key_T,Qt::Key_T);
	this->button_group->addButton(ui->key_U,Qt::Key_U);
	this->button_group->addButton(ui->key_V,Qt::Key_V);
	this->button_group->addButton(ui->key_W,Qt::Key_W);
	this->button_group->addButton(ui->key_X,Qt::Key_X);
	this->button_group->addButton(ui->key_Y,Qt::Key_Y);
	this->button_group->addButton(ui->key_Z,Qt::Key_Z);

	this->button_group->addButton(ui->key_Backspace,Qt::Key_Backspace);
	this->button_group->addButton(ui->key_Return,Qt::Key_Return);

	this->button_group->addButton(ui->key_Symbol_1,Qt::Key_At);
	this->button_group->addButton(ui->key_Symbol_2,Qt::Key_NumberSign);
	this->button_group->addButton(ui->key_Symbol_3,Qt::Key_Dollar);	
	this->button_group->addButton(ui->key_Symbol_4,Qt::Key_Percent);	
	this->button_group->addButton(ui->key_Symbol_5,Qt::Key_Semicolon);
	this->button_group->addButton(ui->key_Symbol_6,Qt::Key_Slash);
	this->button_group->addButton(ui->key_Symbol_7,Qt::Key_Asterisk);
	this->button_group->addButton(ui->key_Symbol_8,Qt::Key_Plus);
	this->button_group->addButton(ui->key_Symbol_9,Qt::Key_Minus);	
	this->button_group->addButton(ui->key_space,Qt::Key_Space);
	this->button_group->addButton(ui->key_tab,Qt::Key_Tab);

	this->button_group->addButton(ui->key_Esc,Qt::Key_Escape);
	this->button_group->addButton(ui->key_Question,Qt::Key_Question);
	this->button_group->addButton(ui->key_comma,Qt::Key_Comma);	
	this->button_group->addButton(ui->key_Backspace_2,Qt::Key_Backspace);
	this->button_group->addButton(ui->key_point,Qt::Key_Period);
	
	this->button_group->addButton(ui->key_Symbol_10,Qt::Key_BracketLeft);
	this->button_group->addButton(ui->key_Symbol_11,Qt::Key_BracketRight);
	this->button_group->addButton(ui->key_Symbol_12,Qt::Key_BraceLeft);
	this->button_group->addButton(ui->key_Symbol_13,Qt::Key_BraceRight);
	this->button_group->addButton(ui->key_Symbol_14,Qt::Key_Colon);
}

void InputWidget::ButtonJudge(int id )
{
	QString strInput = " ";
	switch (id)
	{
	case Qt::Key_0 :
		strInput = "0";
		break;
	case Qt::Key_1 :
		strInput = "1";
		break;
	case Qt::Key_2 :
		strInput = "2";
		break;
	case Qt::Key_3 :
		strInput = "3";
		break;
	case Qt::Key_4 :
		strInput = "4";
		break;
	case Qt::Key_5 :
		strInput = "5";
		break;
	case Qt::Key_6 :
		strInput = "6";
		break;
	case Qt::Key_7 :
		strInput = "7";
		break;
	case Qt::Key_8 :
		strInput = "8";
		break;
	case Qt::Key_9 :
		strInput = "9";
		break;
	case Qt::Key_A :
		strInput = ui->key_A->text();
		break;
	case Qt::Key_B :
		strInput = ui->key_B->text();
		break;
	case Qt::Key_C :
		strInput = ui->key_C->text();
		break;
	case Qt::Key_D :
		strInput = ui->key_D->text();
		break;
	case Qt::Key_E :
		strInput = ui->key_E->text();
		break;
	case Qt::Key_F :
		strInput = ui->key_F->text();
		break;
	case Qt::Key_G :
		strInput = ui->key_G->text();
		break;
	case Qt::Key_H :
		strInput = ui->key_H->text();
		break;
	case Qt::Key_I :
		strInput = ui->key_I->text();
		break;
	case Qt::Key_J :
		strInput = ui->key_J->text();
		break;
	case Qt::Key_K :
		strInput = ui->key_K->text();
		break;
	case Qt::Key_L :
		strInput = ui->key_L->text();
		break;
	case Qt::Key_M :
		strInput = ui->key_M->text();
		break;
	case Qt::Key_N :
		strInput = ui->key_N->text();
		break;
#if 1/*by su for Chinese input method*/
	case Qt::Key_F35 :
		strInput = ui->key_N_2->text();
		break;
#endif
	case Qt::Key_O :
		strInput = ui->key_O->text();
		break;
	case Qt::Key_P :
		strInput = ui->key_P->text();
		break;
	case Qt::Key_Q :
		strInput = ui->key_Q->text();
		break;
	case Qt::Key_R :
		strInput = ui->key_R->text();
		break;
	case Qt::Key_S :
		strInput = ui->key_S->text();
		break;
	case Qt::Key_T :
		strInput = ui->key_T->text();
		break;
	case Qt::Key_U :
		strInput = ui->key_U->text();
		break;
	case Qt::Key_V :
		strInput = ui->key_V->text();
		break;
	case Qt::Key_W :
		strInput = ui->key_W->text();
		break;
	case Qt::Key_X :
		strInput = ui->key_X->text();
		break;
	case Qt::Key_Y :
		strInput = ui->key_Y->text();
		break;
	case Qt::Key_Z :
		strInput = ui->key_Z->text();
		break;
	case Qt::Key_Space :
		emit this->SendInputMessage(" ");
		break;
	case Qt::Key_Backspace :
		{
			if(m_barUi->lineEdit->text().trimmed() == "")
			{
		emit this->SendBackspace();
			}
			else
			{
				m_barUi->lineEdit->backspace();
				if(m_barUi->lineEdit->text().trimmed() == "")
				{
					HideBar();
				}
				else
				{	
					QString tmptext = "";
					tmptext = m_barUi->lineEdit->text();
					m_strInputPy.clear();
					m_barUi->lineEdit->clear();
					TransPy(tmptext);
				}
			}
		break;
		}
	case Qt::Key_Enter :
	case Qt::Key_Return:
		emit this->SendEnter();
		break;
	case Qt::Key_Escape :
		emit this->SenEscape(); 
		break;
	case Qt::Key_Left:
		emit SendCurrKeyValue(Qt::Key_Left);
		break;    
	case Qt::Key_Right:
		emit SendCurrKeyValue(Qt::Key_Right);	
		break;
	case Qt::Key_Up:
		emit SendCurrKeyValue(Qt::Key_Up);	
		break;
	case Qt::Key_Down:
		emit SendCurrKeyValue(Qt::Key_Down);	
		break;
	case Qt::Key_At:
		strInput = "@";
		break;
	case Qt::Key_NumberSign:
		strInput = "#";
		break;
	case Qt::Key_Dollar:
		strInput = "$";
		break;
	case Qt::Key_Percent:
		strInput = "%";
		break;	
	case Qt::Key_Semicolon:
		strInput = ";";
		break;
	case Qt::Key_Slash:
		strInput = "/";
		break;
	case Qt::Key_Asterisk:
		strInput = "*";
		break;
	case Qt::Key_Plus:
		strInput = "+";
		break;
	case Qt::Key_Minus:
		strInput = "-";
		break;
	case Qt::Key_Tab:
		emit this->SendInputMessage("	");
		break;
	case Qt::Key_Comma:
		emit this->SendInputMessage(ui->key_comma->text());
		break;
	case Qt::Key_Question:
		emit this->SendInputMessage(ui->key_Question->text());
		break;
	case Qt::Key_Shift:
		emit CapsLock_Clicked();
		break;
	case Qt::Key_Period:
		strInput = ".";
		break;
	case Qt::Key_BracketLeft:
		strInput = "(";
		break;
	case Qt::Key_BracketRight:
		strInput = ")";
		break;
	case Qt::Key_BraceLeft:
		strInput = "<";
		break;
	case Qt::Key_BraceRight:
		strInput = ">";
		break;
	case Qt::Key_Colon:
		strInput = ui->key_Symbol_14->text();
		break;
	default:
		break;
	}
#if 1/*by su for Chinese input method*/
	if(strInput.trimmed() != "")
	{
		if(m_SwitchCapsLock && (Language_TYPE_CHINESE == m_currentLanguage) && (PAGE_INDEX_ONE == ui->inputPanel->currentIndex()))
		{
			TransPy(strInput);
			bar->show();
		}
		else
			emit this->SendInputMessage(strInput);
	}
#endif
}


void InputWidget::on_key_Shift_clicked()
{
	if(Language_TYPE_ARABIC != m_currentLanguage)
	{
		if(m_SwitchCapsLock)
		{
			m_SwitchCapsLock = false;
#if 1/*by su for Chinese input method*/
			HideBar();
#endif
			ui->key_A->setText("A");
			ui->key_B->setText("B");
			ui->key_C->setText("C");
			ui->key_D->setText("D");
			ui->key_E->setText("E");
			ui->key_F->setText("F");
			ui->key_G->setText("G");
			ui->key_H->setText("H");
			ui->key_I->setText("I");
			ui->key_J->setText("J");
			ui->key_K->setText("K");
			ui->key_L->setText("L");
			ui->key_M->setText("M");
			ui->key_N->setText("N");
#if 1/*by su for Chinese input method*/
			ui->key_N_2->setText("\xD1\x00");
#endif
			ui->key_O->setText("O");
			ui->key_P->setText("P");
			ui->key_Q->setText("Q");
			ui->key_R->setText("R");
			ui->key_S->setText("S");
			ui->key_T->setText("T");
			ui->key_U->setText("U");
			ui->key_V->setText("V");
			ui->key_W->setText("W");
			ui->key_X->setText("X");
			ui->key_Y->setText("Y");
			ui->key_Z->setText("Z");
		}
		else
		{
			m_SwitchCapsLock = true;
			ui->key_A->setText("a");
			ui->key_B->setText("b");
			ui->key_C->setText("c");
			ui->key_D->setText("d");
			ui->key_E->setText("e");
			ui->key_F->setText("f");
			ui->key_G->setText("g");
			ui->key_H->setText("h");
			ui->key_I->setText("i");
			ui->key_J->setText("j");
			ui->key_K->setText("k");
			ui->key_L->setText("l");
			ui->key_M->setText("m");
			ui->key_N->setText("n");
#if 1/*by su for Chinese input method*/
			ui->key_N_2->setText("\xF1\x00");
#endif
			ui->key_O->setText("o");
			ui->key_P->setText("p");
			ui->key_Q->setText("q");
			ui->key_R->setText("r");
			ui->key_S->setText("s");
			ui->key_T->setText("t");
			ui->key_U->setText("u");
			ui->key_V->setText("v");
			ui->key_W->setText("w");
			ui->key_X->setText("x");
			ui->key_Y->setText("y");
			ui->key_Z->setText("z");
		}
	}
}

void InputWidget::On_SwitchInputPanel_Clicked()
{
	do
	{
		if(!ui)
		{
			break;
		}
		if(ui->inputPanel->currentIndex() == PAGE_INDEX_ONE)
		{
			ui->inputPanel->setCurrentIndex(PAGE_INDEX_TWO);
			ui->key_DigitalOrABC->setText("ABC");			
#if 1/*by su for Chinese input method*/
			HideBar();
#endif
		}	
		else
		{
			ui->inputPanel->setCurrentIndex(PAGE_INDEX_ONE);
			ui->key_DigitalOrABC->setText("123");	
		}
	}while(FALSE);
}



#if 1/*added by su 2013-06-18*/
void InputWidget::InstallDefaultLanguage(int index)
{
	QString QmPath;
	m_currentLanguage = index;
	
	if(Language_TYPE_ENGLISH == index)
		QmPath = EnglishPath;
	else if(Language_TYPE_CHINESE  == index)
		QmPath = ChinesePath;
	else if(Language_TYPE_ARABIC == index)
		QmPath = ArabicPath;
	else if(Language_TYPE_SPANISH == index)
		QmPath = SpanishPath;
	else
		QmPath = EnglishPath;

	QTranslator translator;
	translator.load(QmPath);
	QCoreApplication::installTranslator(&translator);

	ui->key_N_2->hide();	
	
	if(Language_TYPE_CHINESE == m_currentLanguage)
	{
		OpenPinyingFile();
	}
	else
	{
		HideBar();
		m_PyImeDicFile.close();
		m_PyImeSerchFile.close();
	}
	if(Language_TYPE_SPANISH == m_currentLanguage)
	{
		ui->key_N_2->show();	
	}
	
	m_SwitchCapsLock = false;
	ui->inputPanel->setCurrentIndex(PAGE_INDEX_ONE);
	ui->key_DigitalOrABC->setText("123");	
	
	ui->retranslateUi(this);
	
	if(Language_TYPE_CHINESE == m_currentLanguage)
	{
		on_key_Shift_clicked();
	}
}
#endif

#if 0/*added by su 2013-06-18*/
QString InputWidget::unicodeToString(QString str)

{
    //例如 str="4F60597D";
    int temp[400];
    QChar qchar[100];
    QString strOut;
    bool ok;
    int count=str.count();
    int len=count/4;
	
    for(int i=0;i<count;i+=4)
    {
     temp[i]=str.mid(i,4).toInt(&ok,16);//每四位转化为16进制整型
     qchar[i/4]=temp[i];
     QString str0(qchar, len);
     strOut=str0;
    }
    return strOut;
}
#endif

#if 1/*by su for Chinese input method*/
void InputWidget::InitChineseBar()
{
	findNothing = true;
	m_pageCount = 0;
	m_currentPage = 0;
	
	do
	{
	    QTextCodec::setCodecForLocale( QTextCodec::codecForName("GB2312") );
		bar = new chineseBar(this);
		if(bar == NULL)
		{
			break;
		}
		m_barUi = bar->GetChineseBarUi();
		if(m_barUi == NULL)
		{
			break;
		}

		//qDebug ()  << "bar = "  << bar;
		bar->setGeometry(30,0,820,38);
		bar->hide();
		connect(m_barUi->label_1,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_1()));
		connect(m_barUi->label_2,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_2()));
		connect(m_barUi->label_3,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_3()));
		connect(m_barUi->label_4,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_4()));
		connect(m_barUi->label_5,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_5()));
		connect(m_barUi->label_6,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_6()));
		connect(m_barUi->label_7,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_7()));
		connect(m_barUi->label_8,SIGNAL(Clicked()),this,SLOT(ChineseSelectFont_8()));
		connect(m_barUi->listUp,SIGNAL(Clicked()),this,SLOT(PageUp()));
		connect(m_barUi->listDown,SIGNAL(Clicked()),this,SLOT(PageDown()));

		keyRev = new QLineEdit();
		if(keyRev == NULL)
		{
			break;
		}
		keyRev->hide();
		//setFocusProxy(QApplication::focusWidget());

		regExp.setPatternSyntax(QRegExp::RegExp);
		regExp.setCaseSensitivity(Qt::CaseSensitive);
	}while(FALSE);

}
void InputWidget::OpenPinyingFile()
{
	m_PyImeDicFile.setFileName(PY_IME_DICTIONARY);
	m_PyImeSerchFile.setFileName(PY_IME_SERCHFILE);

	if(!m_PyImeDicFile.exists())
  	{
      //QMessageBox::warning(0,tr("Initialization Error"),tr("The py file /MP7XX/py.txt is not exists!"));
	  return;
  	}
	if( !m_PyImeSerchFile.open(QIODevice::ReadWrite|QIODevice::Truncate) )
	{
	  //  QMessageBox::warning(0,tr("open Error"),tr("Unable to write the query data,Please check the file is valid!"));
	}
	if( !m_PyImeDicFile.open(QIODevice::ReadOnly) )
	{
	   // QMessageBox::warning(0,tr("open Error"),tr("Unable to load character encoding,Please check the file is valid!"));
	}	
}
void InputWidget::HideBar()
{
	m_strInputPy.clear();
	//keyRev->clear();
	m_barUi->lineEdit->clear();
	bar->hide();
	//keyRev->hide();
}

QString InputWidget::TransPy(QString pyInput)
{
	
	m_strInputPy.append(pyInput);
	m_barUi->lineEdit->insert(pyInput);

	if(!m_PyImeSerchFile.open(QIODevice::ReadWrite|QIODevice::Truncate))
	{
		//QMessageBox::warning(0,tr("TransPy open Error"),tr("Unable to write the query data,Please check the Out file is exists!"));
	}
	

	int ret = 0;
	int count = 0;  //The number of characters to create pages
	//qDebug ()  << "Press key = "  << pyInput;
	regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(m_strInputPy));//Chinese encoding uppercase letters containing,so distinguish Uppercase and lowercase letters
	while(!m_PyImeDicFile.atEnd())
	{
		QByteArray data = m_PyImeDicFile.readLine();
		ret = regExp.indexIn(QString(data.data()),0,QRegExp::CaretAtZero);
		if( ret > -1)   //Find matching Chinese characters
		{
			findNothing = false;
			m_PyImeSerchFile.write(regExp.cap(1).toLatin1().data());         
			m_PyImeSerchFile.write("\n");
			count++;
		}
	}
	m_PyImeDicFile.seek(0);
	m_PyImeSerchFile.close();

	if(count%8 == 0)
	{
		m_pageCount = count/8;
	}else{
		m_pageCount = (count/8)+1;
	}

	if(findNothing)    //Not find matching Chinese characters
	{
		m_barUi->label_1->setText("");
		m_barUi->label_2->setText("");
		m_barUi->label_3->setText("");
		m_barUi->label_4->setText("");
		m_barUi->label_5->setText("");
		m_barUi->label_6->setText("");
		m_barUi->label_7->setText("");
		m_barUi->label_8->setText("");
		return NULL;
	}

	findNothing = true;
	m_currentPage = 0;
	
	m_barUi->listUp->setDisabled(true);
	
	if(1 == m_pageCount)
		m_barUi->listDown->setDisabled(true);
	else
		m_barUi->listDown->setEnabled(true);

	ShowPage(0);    //show the first page

	return m_strInputPy;

}
void InputWidget::ShowPage(int index)
{
	int n =0;
	QString chinese[8];

	if(!m_PyImeSerchFile.open(QIODevice::ReadWrite))
	{
		//QMessageBox::warning(0,tr("ShowPage open Error"),tr("Unable to write the query data,Please check the Out file is exists!"));
	}
	while(!m_PyImeSerchFile.atEnd())
	{
		for(int j=0;j<8;j++)
		{
			QByteArray data = m_PyImeSerchFile.readLine();
			data.replace("\n","\0");
			n++;
			if((index*8)< n && n <((index+1)*8)+1)
			{
				chinese[j].append(data);
			}
		}
	}

	m_barUi->label_1->setText(tr(chinese[0].toLatin1()));
	m_barUi->label_2->setText(tr(chinese[1].toLatin1()));
	m_barUi->label_3->setText(tr(chinese[2].toLatin1()));
	m_barUi->label_4->setText(tr(chinese[3].toLatin1()));
	m_barUi->label_5->setText(tr(chinese[4].toLatin1()));
	m_barUi->label_6->setText(tr(chinese[5].toLatin1()));
	m_barUi->label_7->setText(tr(chinese[6].toLatin1()));
	m_barUi->label_8->setText(tr(chinese[7].toLatin1()));

	m_PyImeSerchFile.close();
}
void InputWidget::PageUp()
{
	if(m_currentPage == m_pageCount-1)
		m_barUi->listDown->setEnabled(true);
	if(--m_currentPage < 1)
		m_barUi->listUp->setDisabled(true);
	ShowPage(m_currentPage);
}

void InputWidget::PageDown()
{
	if(m_currentPage == 0)
		m_barUi->listUp->setEnabled(true);
	if(++m_currentPage == m_pageCount-1)
		m_barUi->listDown->setDisabled(true);
	ShowPage(m_currentPage);
}
void InputWidget::SetTextEmpty()
{
	m_strInputPy.clear();
	m_barUi->lineEdit->clear();
	m_barUi->label_1->setText("");
	m_barUi->label_2->setText("");
	m_barUi->label_3->setText("");
	m_barUi->label_4->setText("");
	m_barUi->label_5->setText("");
	m_barUi->label_6->setText("");
	m_barUi->label_7->setText("");
	m_barUi->label_8->setText("");
	bar->hide();
}

void InputWidget::ChineseSelectFont_1()
{
	if(m_barUi->label_1->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_1->text());
		emit this->SendInputMessage(m_barUi->label_1->text());
		SetTextEmpty();
	}
}
void InputWidget::ChineseSelectFont_2()
{
	if(m_barUi->label_2->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_2->text());
		emit this->SendInputMessage(m_barUi->label_2->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_3()
{
	if(m_barUi->label_3->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_3->text());
		emit this->SendInputMessage(m_barUi->label_3->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_4()
{
	if(m_barUi->label_4->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_4->text());
		emit this->SendInputMessage(m_barUi->label_4->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_5()
{
	if(m_barUi->label_5->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_5->text());
		emit this->SendInputMessage(m_barUi->label_5->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_6()
{
	if(m_barUi->label_6->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_6->text());
		emit this->SendInputMessage(m_barUi->label_6->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_7()
{
	if(m_barUi->label_7->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_7->text());
		emit this->SendInputMessage(m_barUi->label_7->text());
		SetTextEmpty();
	}
}

void InputWidget::ChineseSelectFont_8()
{
	if(m_barUi->label_8->text().trimmed() != "")
	{
		keyRev->insert(m_barUi->label_8->text());
		emit this->SendInputMessage(m_barUi->label_8->text());
		SetTextEmpty();
	}
}

#endif	
