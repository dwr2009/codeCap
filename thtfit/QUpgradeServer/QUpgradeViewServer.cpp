#include "QUpgradeViewServer.h"
#include <QDesktopWidget>
#include <signal.h>
#include <ErrPrintHelper.h>
#include <QDBusError>
#include <LinuxSignalHandlerEx.h>


#define	QWB_STATUS_BAR_HEIGHT			30
#define	QWB_STATUS_BAR_BOTTOM_DISTANCE		30


QDBusIf_UpgradeViewServer::QDBusIf_UpgradeViewServer(UpgradeViewServer * pRealObj) : QDBusAbstractAdaptor(pRealObj)
{
	m_pRealObj = pRealObj;
	setAutoRelaySignals(true);
}

void QDBusIf_UpgradeViewServer::setViewText(const QString & strText, qint32 iTimeoutMs ,int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setViewText(strText,iTimeoutMs);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}


void QDBusIf_UpgradeViewServer::setViewFont(qint32 iPointSize,qint32 isBold,qint32 iWeight,int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setViewFont(iPointSize,isBold,iWeight);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}

void QDBusIf_UpgradeViewServer::setViewColor(qint32 ired,qint32 igreed,qint32 iblue,int & iOutRet)
{
	if(m_pRealObj)
	{
		iOutRet = m_pRealObj->setViewColor(ired,igreed,iblue);
	}
	else
	{
		iOutRet = ERR_INV_STATE;
	}
}



DisplayWindow::DisplayWindow(QWidget* parent):m_upgradetext(this)
{
	Qt::WindowFlags	curWinFlags = windowFlags();
	setWindowFlags(curWinFlags | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
	setFocusPolicy(Qt::NoFocus);
	
	QDesktopWidget * pDesktopWidget = QApplication::desktop();
	if(pDesktopWidget)
	{
		QRect AvailableDestiopGeometry = pDesktopWidget->availableGeometry();
		QRect rcThisWidget = AvailableDestiopGeometry;
		//rcThisWidget.adjust(0,0,AvailableDestiopGeometry.width(),AvailableDestiopGeometry.height());
		setGeometry(rcThisWidget);
	}
	
	m_upgradetext.resize(size());
	m_upgradetext.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	m_upgradetext.setWordWrap(true); 

	m_font.setPointSize(20);
    m_font.setBold(true);
    m_font.setWeight(75);
	m_upgradetext.setFont(m_font);
	
}

DisplayWindow::~DisplayWindow()
{
}

void DisplayWindow::setViewText(const QString & strText)
{
	m_upgradetext.setText(strText);
	show();
}

void DisplayWindow::setViewFont(int iPointSize,bool isBold,int iWeight)
{
	m_font.setPointSize(iPointSize);
    m_font.setBold(isBold);
    m_font.setWeight(iWeight);	
	m_upgradetext.setFont(m_font);
}

void DisplayWindow::setViewColor(int ired,int igreed,int iblue)
{
	m_Palette.setColor(QPalette::WindowText,QColor(ired,igreed,iblue));
	m_upgradetext.setPalette(m_Palette);
}

void UpgradeViewServer::On_TimeoutHide()
{
	upgradeview.hide();
}

UpgradeViewServer::UpgradeViewServer(int argc, char *argv[]): QApplication(argc, argv)
{	
	bool bRet;
	
	m_bUpgradeDbusServIsReged=false;

	bRet = this->connect(&m_viewtimeoutremove, SIGNAL(timeout()), this, SLOT(On_TimeoutHide()));
	if(false == bRet)
	{
		LOG_BLINE("ConnSig(TimeoutRemvoe) err\n");
	}
}


UpgradeViewServer::~UpgradeViewServer()
{
}

int UpgradeViewServer::exec()
{
	int iOutRet = ERROR_SUCCESS, iRet;

	iRet = InitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
		goto EXIT_PROC;
	}
	iOutRet = QApplication::exec();


	iRet = ExitInstance();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

EXIT_PROC:
	return iOutRet;
}

VOID UpgradeViewServer::SigHandler(VOID * pCbContext, int SigNo)
{
	UpgradeViewServer * pApp = (typeof(pApp))pCbContext;
	if(SIGINT == SigNo || SIGTERM == SigNo || SIGQUIT == SigNo)
	{
		quit();
	}
}



INT_t UpgradeViewServer::InitInstance()
{
	
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	bool bRet;

	iRet = CLinuxSignalHandlerEx::HookSignal(SIGINT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGTERM, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGQUIT, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	
	iRet = CLinuxSignalHandlerEx::HookSignal(SIGCHLD, SigHandler, (PVOID)(this));
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
	

	m_Upgrade_DBusConn_sp = QSharedPointer <QDBusConnection> (new QDBusConnection(QDBUS_CONN_NAME_UPGRADE_VIEW_SERVER));
	if(m_Upgrade_DBusConn_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}

	*m_Upgrade_DBusConn_sp = QDBusConnection::systemBus();
	if(m_Upgrade_DBusConn_sp->isConnected())
	{
	}
	else	//connection failed
	{
		iOutRet = ERR_CONNECTION_FAIL;
		QDBusError QDBusErr = m_Upgrade_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to connect dbus\n");
		}
		goto EXIT_PROC;
	}

	bRet = m_Upgrade_DBusConn_sp->registerService(DBUS_UPGRADE_VIEW_SERVER_NAME);
	if(false == bRet)
	{
		iOutRet = ERR_REGISTER_FAIL;
		QDBusError QDBusErr = m_Upgrade_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_UPGRADE_VIEW_SERVER_NAME);
		}
		goto EXIT_PROC;
	}
	
	m_bUpgradeDbusServIsReged = TRUE;

	m_DbusIf_UpgradeView_sp = QSharedPointer <QDBusIf_UpgradeViewServer>(new QDBusIf_UpgradeViewServer(this));
	if(m_DbusIf_UpgradeView_sp.isNull())
	{
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	
	bRet = m_Upgrade_DBusConn_sp->registerObject(DBUS_UPGRADE_VIEW_PATH, this);
	if(false == bRet)
	{
		iOutRet = ERR_REGISTER_FAIL;
		QDBusError QDBusErr = m_Upgrade_DBusConn_sp->lastError();
		if(QDBusErr.isValid())
		{
			LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
		}
		else
		{
			LOG_BLINE("Failed to reg dbus obj %s\n", DBUS_UPGRADE_VIEW_PATH);
		}
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t UpgradeViewServer::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS,bRet;
	if(FALSE == m_Upgrade_DBusConn_sp.isNull())
	{
		if(m_Upgrade_DBusConn_sp->isConnected())
		{
			m_Upgrade_DBusConn_sp->unregisterObject(DBUS_UPGRADE_VIEW_PATH); 		
			if(m_bUpgradeDbusServIsReged)
			{
				bRet = m_Upgrade_DBusConn_sp->unregisterService(DBUS_UPGRADE_VIEW_SERVER_NAME);
				if(false == bRet)
				{
					LOG_BLINE("Failed to unreg dbus srv %s\n", DBUS_UPGRADE_VIEW_SERVER_NAME);
				}
				m_bUpgradeDbusServIsReged = FALSE;
			}
			QDBusConnection::disconnectFromBus(QDBUS_CONN_NAME_UPGRADE_VIEW_SERVER);
		}
	}

	m_DbusIf_UpgradeView_sp.clear();
	m_Upgrade_DBusConn_sp.clear();
	
	return iOutRet;
}



int UpgradeViewServer::setViewText(const QString & strText, qint32 iTimeoutMs/*=-1*/)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strMultiplexedText;
	if(0 > iTimeoutMs)
	{
		m_viewtimeoutremove.stop();
	}
	else	
	{
		m_viewtimeoutremove.setSingleShot(TRUE);
		m_viewtimeoutremove.start(iTimeoutMs);
	}
	
	strMultiplexedText += "[" + strText + "]";

	upgradeview.setViewText(strMultiplexedText);

	
	return iOutRet;
}

int UpgradeViewServer::setViewFont(qint32 iPointSize,qint32 isBold,qint32 iWeight)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	upgradeview.setViewFont(iPointSize,isBold,iWeight);
	
	return iOutRet;
}

int UpgradeViewServer::setViewColor(qint32 ired,qint32 igreed,qint32 iblue)
{
	INT_t iOutRet = ERROR_SUCCESS;
	
	upgradeview.setViewColor(ired,igreed,iblue);
	
	return iOutRet;
}


int main(int argc, char *argv[])
{
	INT_t iOutRet = ERROR_SUCCESS;
	UpgradeViewServer app(argc,argv);
	
	iOutRet = app.exec();

	return iOutRet;
}



