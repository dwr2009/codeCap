#include "NetworkManagerClient.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <QTimer>

// DBus service name
#define	SS_DBUS_SERVICE_NAME				"org.thtfit.SystemService"
// PATH name
#define	DBUS_PATH_NetworkManagerSrv			"/org/thtfit/SystemService/NetworkManagerSrv"


// Interface name
#define	IF_NAME_SysSrv_NetworkManagerSrv	"org.thtfit.SystemService.NetworkManagerSrv"


// METHOD names
#define	METHOD_readSettings				"readSettings"
#define	METHOD_applySettings			"applySettings"
#define	METHOD_getResolveConf			"getResolveConf"
#define	METHOD_ResetSnmpdV3Password			"ResetSnmpdV3Password"
#define	METHOD_ReqGetWifiCfg			"getWifiCfg"
#define	METHOD_ReqGetWifiStatus			"getWifiStatus"
#define	METHOD_ReqGetWifiNetworkCfg			"getNetworkCfg"
#define	METHOD_ReqSetWifiCfg			"setWifiCfg"
#define	METHOD_ReqSetNetworkCfg		"setNetworkCfg"
#define	METHOD_ReqSetWifiStatus		"setWifiStatus"
#define	METHOD_ReqGetWifiSignalLevel	"getWifiSignalLevel"






QNetworkManagerClient::QNetworkManagerClient()
{

}

QNetworkManagerClient::~QNetworkManagerClient()
{

}

INT_t QNetworkManagerClient::setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_DBusConn_wp = DBusConn_sp;
		OnTimer_PrepareDBus();
	}while(FALSE);

	return iOutRet;
}

void QNetworkManagerClient::OnTimer_PrepareDBus()
{
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		DBusConn_sp = m_DBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			break;
		}
		m_DBusIf_NetworkManager_sp = QSharedPointer <QDBusInterface> (new QDBusInterface(SS_DBUS_SERVICE_NAME,
			DBUS_PATH_NetworkManagerSrv, IF_NAME_SysSrv_NetworkManagerSrv, (*DBusConn_sp), this));
		if(m_DBusIf_NetworkManager_sp.isNull())
		{
			break;
		}
		if(m_DBusIf_NetworkManager_sp->isValid())
		{
		}
		else
		{
			LOG_BLINE("QDbus: Can't get the SysSrv_NetworkManaerSrv default interface.\n");
			QTimer::singleShot(2*1000, this, SLOT(OnTimer_PrepareDBus()));
		}
	}while(FALSE);
}

INT_t QNetworkManagerClient::getDefaultGatewayAddr(QString & strGateway)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, "getDefaultGatewayAddr");
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
		if(2 > MsgArgsList.count())
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
			strGateway = Reply_Param.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::getResolveConf(QString & strDomain, QString & strNameServerList)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_getResolveConf);
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
		if(3 > MsgArgsList.count())
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
			strDomain = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(2);
			strNameServerList = Reply_Param.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::readSettings(QString & strDefaultGatewayAddr, QString & strDns1, QString & strDns2)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_readSettings);
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
			strDefaultGatewayAddr = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(2);
			strDns1 = Reply_Param.toString();
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(3);
			strDns2 = Reply_Param.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::applySettings(CONST QString & strDefaultGateway, CONST QString & strDns1, CONST QString & strDns2)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_applySettings, strDefaultGateway,
			strDns1, strDns2);
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


INT_t QNetworkManagerClient::ResetSnmpdV3Password()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ResetSnmpdV3Password);
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
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				iOutRet = iOutRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QNetworkManagerClient::getWifiCfg(int type, QString & strValue)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqGetWifiCfg,type);
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
		if(2> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(1);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(0);
			strValue = Reply_Param.toString();
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::getWifiStatus(int & iCurStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqGetWifiStatus);
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
		if(2> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			iCurStatus = Reply_Param.toInt();
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::getNetworkCfg(const QString & strInParam, QString & strOutParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqGetWifiNetworkCfg,strInParam);
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
		if(2> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			strOutParam = Reply_Param.toString();
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::setWifiCfg(const QString & strParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqSetWifiCfg,strParam);
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
		if(1> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::setNetworkCfg(const QString & strParam)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqSetNetworkCfg,strParam);
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
		if(1> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}

INT_t QNetworkManagerClient::setWifiStatus(int iCurStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqSetWifiStatus,iCurStatus);
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
		if(1> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}


INT_t QNetworkManagerClient::getWifiSignalLevel(int & iCurrSigLevel)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		if(FALSE == m_DBusIf_NetworkManager_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = m_DBusIf_NetworkManager_sp->call(QDBus::Block, METHOD_ReqGetWifiSignalLevel);
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
		if(2> MsgArgsList.count())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iOutRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			iCurrSigLevel = Reply_Param.toInt();			 
		}
	}while(FALSE);

	return iOutRet;
}