#include "NetMgrEthernetClient.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <QTimer>

// DBus service name
#define	SS_DBUS_SERVICE_NAME				"org.thtfit.SystemService"
// PATH name
#define	DBUS_PATH_NetworkManagerSrv_Ethernet		"/org/thtfit/SystemService/NetworkManagerSrv/Ethernet"
// Interface name
#define	IF_NAME_SysSrv_NetMgrEthernet	"org.thtfit.SystemService.NetworkManagerSrv.Ethernet"

// METHOD names
#define	METHOD_getDefaultDeviceInfo				"getDefaultDeviceInfo"
#define	METHOD_readSettings				"readSettings"
#define	METHOD_applySettingsForDefaultDevice				"applySettingsForDefaultDevice"



QNetMgrEthernetClient::QNetMgrEthernetClient()
{

}

QNetMgrEthernetClient::~QNetMgrEthernetClient()
{

}

INT_t QNetMgrEthernetClient::setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_DBusConn_wp = DBusConn_sp;
		OnTimer_PrepareDBus();
	}while(FALSE);

	return iOutRet;
}

void QNetMgrEthernetClient::OnTimer_PrepareDBus()
{
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		DBusConn_sp = m_DBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			break;
		}
		m_DBusIf_NetMgrEthernet_sp = QSharedPointer <QDBusInterface> (new QDBusInterface(SS_DBUS_SERVICE_NAME,
			DBUS_PATH_NetworkManagerSrv_Ethernet, IF_NAME_SysSrv_NetMgrEthernet, (*DBusConn_sp), this));
		if(m_DBusIf_NetMgrEthernet_sp.isNull())
		{
			break;
		}
		if(m_DBusIf_NetMgrEthernet_sp->isValid())
		{
		}
		else
		{
			LOG_BLINE("QDbus: Can't get the SysSrv_NetMgrEthernet default interface.\n");
			QTimer::singleShot(2*1000, this, SLOT(OnTimer_PrepareDBus()));
		}
	}while(FALSE);
}

INT_t QNetMgrEthernetClient::getDefaultDeviceInfo(QString & strDeviceName, QString & strIPv4Addr, QString & strSubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetMgrEthernet_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetMgrEthernet_sp->call(QDBus::Block, METHOD_getDefaultDeviceInfo);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			break;
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(4 > MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			strDeviceName = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(2);
			strIPv4Addr = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(3);
			strSubnetMask = Reply_Param.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernetClient::readSettings(QString & strDefaultDeviceName, bool & bAutoGetIP, QString & strIPv4Addr, QString & SubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetMgrEthernet_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetMgrEthernet_sp->call(QDBus::Block, METHOD_readSettings);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			break;
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(5 > MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			strDefaultDeviceName = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(2);
			bAutoGetIP = Reply_Param.toBool();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(3);
			strIPv4Addr = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(4);
			SubnetMask = Reply_Param.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetMgrEthernetClient::applySettingsForDefaultDevice(bool bAutoGetIP, QString & strIPv4Addr, QString & strSubnetMask)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetMgrEthernet_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetMgrEthernet_sp->call(QDBus::Block, METHOD_applySettingsForDefaultDevice,
			bAutoGetIP, strIPv4Addr, strSubnetMask);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			break;
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

