#include "inputpanelcontext.h"
#include <QPointer>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QWSServer>

MyInputPanelContext::MyInputPanelContext()
{
	inputPanel = new InputWidget(0);
	
	connect(inputPanel, SIGNAL(SendInputMessage(QString)), SLOT(sendCharacter(QString)));
   // QObject::connect(inputPanel,SIGNAL(SendEnter()),this,SLOT(ConfirmContent()));
        QObject::connect(inputPanel,SIGNAL(SendBackspace()),this,SLOT(BackspaceContent()));
        QObject::connect(inputPanel,SIGNAL(SenEscape()),this,SLOT(Escape()));
	connect(inputPanel, SIGNAL(SendCurrKeyValue(int)), SLOT(sendKeyValue(int)));

}

MyInputPanelContext::~MyInputPanelContext()
{
	if(inputPanel)
	{
	//	delete inputPanel;
		inputPanel = NULL;
	}
}


bool MyInputPanelContext::filterEvent(const QEvent* event)
{
	if (event->type() == QEvent::RequestSoftwareInputPanel) 
	{
		updatePosition();
		if(inputPanel == NULL)
		{
			inputPanel = new InputWidget;
		}
		inputPanel->raise();
		inputPanel->activateWindow();
		inputPanel->show();
		return true;
	}
	else if (event->type() == QEvent::CloseSoftwareInputPanel) 
	{
		if(inputPanel == NULL)
		{
			inputPanel = new InputWidget;
		}
		inputPanel->hide();
		return false;
	}
	else if(QEvent::MouseButtonPress == event->type())
	{
		if(inputPanel == NULL)
		{
			return false;
		}
		if(!inputPanel->isHidden())
		{
		  	 inputPanel->hide();
		}
		else
		{
			return true;
		}
	}
	if(QEvent::MouseButtonRelease == event->type())
	{
		return false;
	}
	
	return false;
}

void MyInputPanelContext::sendKeyValue(int key_value)
{
	QPointer<QWidget> w = focusWidget();
	if (!w)
	{
		return;
	}
	QKeyEvent keyPress(QEvent::KeyPress, key_value, Qt::NoModifier, QString(""));
	QApplication::sendEvent(w, &keyPress);
}

void MyInputPanelContext::sendCharacter(QString character)
{
	QPointer<QWidget> w = focusWidget();
	if (!w)
	{
		return;
	}
	
	QKeyEvent keyPress(QEvent::KeyPress, character.at(0).unicode(), Qt::NoModifier, QString(character));
	QApplication::sendEvent(w, &keyPress);
	QKeyEvent keyRelease(QEvent::KeyPress, character.at(0).unicode(), Qt::NoModifier, QString());
	QApplication::sendEvent(w, &keyRelease);
}

void MyInputPanelContext::updatePosition()
{
	QWidget *widget = focusWidget();
	if (!widget)
	{
		return;
	}	
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		rcThisWidget.adjust(0, (AvailableDestiopGeometry.height() - 220), 
			0, 0);
		inputPanel->setGeometry(rcThisWidget);
	}
	//inputPanel->move(panelPos);
}

QString  MyInputPanelContext::identifierName()
{
	return "ime_english";
}


QString  MyInputPanelContext::language()
{
	return "en";
}

bool  MyInputPanelContext::isComposing() const
{
	return true;
}

void  MyInputPanelContext::reset()
{

}

void MyInputPanelContext::BackspaceContent()
{
 	QPointer<QWidget> w = focusWidget();
	if (!w)
	{
		return;
	}
	QKeyEvent keyPress(QEvent::KeyPress, 0x01000003, Qt::NoModifier, QString(""));
	QApplication::sendEvent(w, &keyPress);
	
}

void MyInputPanelContext::Escape()
{
	inputPanel->hide();
}

