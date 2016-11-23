#include "WebBrowserSrv.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <QDBusError>
#include <LinuxSignalHandlerEx.h>
#include <SignalInfo.h>
#include <QApplication>
#include "Api/WebBrowserAppDef.h"
#include <SystemSrvApi.h>
#include "ControlCenterApp.h"
#include <QUrlInfoEx.h>

CWebBrowserSrv::CWebBrowserSrv() : m_mutexDataCommSrvSp(QMutex::Recursive)
{
	this->moveToThread(this);
}

CWebBrowserSrv::~CWebBrowserSrv()
{
}

/*
ThreadContext: any thread
*/
qint64 CWebBrowserSrv::getCurFileTransferredSize()
{
	INT_t iRet;
	qint64 iCurFileTransferredSize = 0;
	QSharedPointer <CDataCommService> DataCommSrv_sp;

	do
	{
		{
			QAutoLock AutoRangeLocker(&m_mutexDataCommSrvSp);
			if(m_DataCommSrv_sp.isNull())
			{
				break;
			}
			DataCommSrv_sp = m_DataCommSrv_sp;
		}
		iRet = m_DataCommSrv_sp->getCurFileTransferredSize(OUT iCurFileTransferredSize);
		if(ERROR_SUCCESS != iRet)
		{
			iCurFileTransferredSize = 0;
			break;
		}
	}while(FALSE);

	return iCurFileTransferredSize;
}

INT_t CWebBrowserSrv::setTransferControlStatus(int TransferCtrlCmdCode)
{
	INT_t iRet;
	
	iRet = ERROR_SUCCESS;
	
	do
	{
		{
			QAutoLock AutoRangeLocker(&m_mutexDataCommSrvSp);
			if(m_DataCommSrv_sp.isNull())
			{
				iRet =  ERROR_INVALID_STATE;
				break;
			}
		}
		m_DataCommSrv_sp->setTransferControlStatus(TransferCtrlCmdCode,iRet);		
	}while(FALSE);

	return iRet;	
}

INT_t CWebBrowserSrv::getTransferControl(int & TransferStatusCode)
{
	INT_t iRet;
	
	do
	{
		{
			QAutoLock AutoRangeLocker(&m_mutexDataCommSrvSp);
			if(m_DataCommSrv_sp.isNull())
			{
				iRet =	ERROR_INVALID_STATE;
				break;
			}
		}
		m_DataCommSrv_sp->getTransferControl(OUT iRet, OUT TransferStatusCode);
	}while(FALSE);
	
	return iRet;	
}

int CWebBrowserSrv::On_FsMountPointSymLink(const QString & strSymLinkFilePath)
{
	int iOutRet = ERROR_SUCCESS;
	QControlCenterApp * pApp = (typeof(pApp))qApp;

	do
	{
		if(m_WebBrowserSrvThreadId != QThread::currentThreadId ())
		{
			LOG_BLINE("BUG,CurThrdId=%u,m_WebBrowserSrvThreadId=%u\n", (quint32)QThread::currentThreadId(),
				(quint32)m_WebBrowserSrvThreadId);	
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = pApp->On_FsMountPointEvent(ControlCenter::FsMountEvt_SymLink, strSymLinkFilePath);
	}while(FALSE);
	
	return iOutRet;
}

int CWebBrowserSrv::On_FsMountPointSymUnlink(const QString & strSymUnlinkFilePath)
{
	int iOutRet = ERROR_SUCCESS;
	QControlCenterApp * pApp = (typeof(pApp))qApp;

	do
	{
		if(m_WebBrowserSrvThreadId != QThread::currentThreadId ())
		{
			LOG_BLINE("BUG,CurThrdId=%u,m_WebBrowserSrvThreadId=%u\n", (quint32)QThread::currentThreadId(),
				(quint32)m_WebBrowserSrvThreadId);	
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iOutRet = pApp->On_FsMountPointEvent(ControlCenter::FsMountEvt_SymUnlink, strSymUnlinkFilePath);
	}while(FALSE);
	
	return iOutRet;
}

void CWebBrowserSrv::run()
{
	int iOutRet = ERROR_SUCCESS, iRet = 0;
	bool bRet;

	do
	{	
		m_WebBrowserSrvThreadId = QThread::currentThreadId ();

		m_DBusConn_sp = QSharedPointer <QDBusConnection> (new QDBusConnection(QDBUS_CONN_NAME_WebBrowser_Srv));
		if(m_DBusConn_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		*m_DBusConn_sp = QDBusConnection::systemBus();
		if(m_DBusConn_sp->isConnected())
		{
		}
		else
		{
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to connectg dbus\n");
			}
			break;
		}

		//Connect the dbus signal SystemService.SystemEvent.FsMountPointSymLink
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_SysEvt, DbusIf_SysSrv_SysEvt,
			SigName_FsMountPointSymLink, this, SLOT(On_FsMountPointSymLink(const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.FsMountPointSymLink) err\n");
		}
		//Connect the dbus signal SystemService.SystemEvent.FsMountPointSymUnlink
		bRet = m_DBusConn_sp->connect("", DBUS_PATH_SysSrv_SysEvt, DbusIf_SysSrv_SysEvt,
			SigName_FsMountPointSymUnlink, this, SLOT(On_FsMountPointSymUnlink(const QString &)));
		if(false == bRet)
		{
			LOG_BLINE("ConnDbusSig(SysEvt.FsMountPointSymUnlink) err\n");
		}

		bRet = m_DBusConn_sp->registerService(DBUS_SrvName_WebBrowser);
		if(false == bRet)
		{
			QDBusError QDBusErr = m_DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_SrvName_WebBrowser);
			}
			break;
		}

		{
			QAutoLock AutoRangeLocker(&m_mutexDataCommSrvSp);
			m_DataCommSrv_sp = QSharedPointer <CDataCommService> (new CDataCommService);
			if(m_DataCommSrv_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
		}
		iRet = m_DataCommSrv_sp->setQDBusConn(m_DBusConn_sp);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = m_DataCommSrv_sp->InitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = exec();
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}

	if(FALSE == m_DataCommSrv_sp.isNull())
	{
		iRet = m_DataCommSrv_sp->ExitInstance();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		{
			QAutoLock AutoRangeLocker(&m_mutexDataCommSrvSp);
			m_DataCommSrv_sp.clear();
		}
	}

	if(FALSE == m_DBusConn_sp.isNull())
	{
		if(m_DBusConn_sp->isConnected())
		{
			bRet = m_DBusConn_sp->unregisterService(DBUS_SrvName_WebBrowser);
			if(false == bRet)
			{
				LOG_BLINE("Failed to unreg dbus srv %s\n", DBUS_SrvName_WebBrowser);
			}
		}
		QDBusConnection::disconnectFromBus(QDBUS_CONN_NAME_WebBrowser_Srv);
	}	
}

