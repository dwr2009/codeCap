#include <QDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "imframe.h"
#include "inputwidget.h"
#include <QtWidgets/QDesktopWidget>

#if 0
IMFrame::IMFrame()
{
	inputwidget = new InputWidget(this);
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		rcThisWidget.adjust(0, (AvailableDestiopGeometry.height() - 330), 
			0, 0);
		inputwidget->setGeometry(rcThisWidget);
	}
	m_iIsCapital = 0 ;
	m_IsFisrtReqShow  = true;
}

IMFrame::~IMFrame()
{
	if(inputwidget)
	{
		delete inputwidget;
		inputwidget = NULL;
	}
}
#endif

void IMFrame::sendContent(const QString& newcontent)
{
	m_strContent += newcontent;	
	if(m_iIsCapital)
	{
		//newcontent=newcontent.toUpper();
		//sendCommitString(newcontent.toUpper());
	}
	else
	{
		//sendCommitString(newcontent);
	}
}

void IMFrame::confirmContent()
{	
	if(m_strContent.trimmed() == "")
	{
		return ;
	}
	//sendCommitString(m_strContent);
	m_strContent.clear();
}

void IMFrame::updateHandler(int type)
{
#if 0
	switch(type)
	{
	case QWSInputMethod::FocusOut:
		m_strContent.clear();		
		if(inputwidget)
		{
			inputwidget->hide();
		}
		break;
	case QWSInputMethod::FocusIn:
		{
			if(!inputwidget)
			{
				inputwidget = new InputWidget(this);
								
				QDesktopWidget * pDesktopWidget = QApplication::desktop();
				if(pDesktopWidget)
				{
					QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
					QRect rcThisWidget = AvailableDestiopGeometry;
					rcThisWidget.adjust(0, (AvailableDestiopGeometry.height() - 330), 
						0, 0);
					inputwidget->setGeometry(rcThisWidget);
				}
			}		
			inputwidget->show();
		}
		break;
	case QWSInputMethod::Update:
		break;
	case QWSInputMethod::Reset:
		if(inputwidget)
		{
			inputwidget->hide();
		}
		break;
	default:
		break;
	}
#endif
}

void IMFrame::BackspaceContent()
{
	//QWSServer::sendKeyEvent(0x01000003,Qt::Key_Backspace,Qt::NoModifier,true,false);
}

void IMFrame::Escape()
{
	/*m_strContent.clear();
	if(inputwidget)
	{
		inputwidget->hide();
	}
	updateHandler(QWSInputMethod::Reset);
	*/
	//updateHandler(QWSInputMethod::FocusOut);
	//QWSServer::sendKeyEvent(0x01000003,Qt::Key_Tab,Qt::NoModifier,true,false);
}

void IMFrame::OnKey_Go_Clicked()
{
	//QWSServer::sendKeyEvent(0x01000003,Qt::Key_Return,Qt::NoModifier,true,false);
	m_strContent.clear();
	if(inputwidget)
	{
//		inputwidget->hide();
	}
	//updateHandler(QWSInputMethod::Reset);
	//updateHandler(QWSInputMethod::FocusOut);
	//QWSServer::sendKeyEvent(0x01000003,Qt::Key_Tab,Qt::NoModifier,true,false);
}

void IMFrame::On_CapsLock_Clicked()
{
	if(m_iIsCapital==0)
	{
		m_iIsCapital =1;
	}
	else
	{
		m_iIsCapital =0;
	}
}

#if 1/*added by su 2013-06-18*/
void IMFrame::On_ChangeLanguage(int index)
{
	inputwidget->InstallDefaultLanguage(index);
}
#endif

