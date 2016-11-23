#include "QPowerMateClient.h"
#include <BaseErrDef.h>
#include <QTimer>
#include <ErrPrintHelper.h>
#include <linux/input.h>

QPowerMateClient::QPowerMateClient()
{
    m_pmstatus=PowerMate_Open;
}

QPowerMateClient::~QPowerMateClient()
{
}

INT_t QPowerMateClient::setDBusConn(QSharedPointer <QDBusConnection> & DBusConn_sp)
{
    INT_t iOutRet = ERROR_SUCCESS;
    do
    {
        m_DBusConn_pm = DBusConn_sp;
        OnTimer_PrepareDBus();
     }while(FALSE);
   
    return iOutRet;
}


INT_t QPowerMateClient::setPowerMateBrightness(int & volume)
{
    INT_t iOutRet = ERROR_SUCCESS;
    int newbrightness;
    int pmglobalcontrol=1;
    QDBusMessage QDBusMsgReply;
    QList <QVariant> MsgArgsList;
             
    do
    {
        
        if(FALSE == m_DBusIf_PowerMate_sp->isValid())
        {
            iOutRet = ERR_NO_QDBUS_CONN;
            break;
        }

        if(PowerMate_Open != m_pmstatus)
        {
            break;
        }

        iOutRet=getPowerMateGlobalControl(pmglobalcontrol);
        if(ERROR_SUCCESS != iOutRet)
        {
            LOG_BLINE("Get Power Mate Global Control err:%d\n",iOutRet);
        }

        if(0 == pmglobalcontrol)
        {
            break;
        }

        if( 0> volume)
        {
            volume=0;
        }

        if( 100 < volume)
        {
            volume=100;
        }
        
        newbrightness=(255/100)*volume;
        QDBusMsgReply = m_DBusIf_PowerMate_sp->call(QDBus::Block, METHOD_setPowerMateBrightness,newbrightness);
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
                m_pmstatus=PowerMate_Close;
                break;
            }
        }
    }while(FALSE);
      
    return iOutRet ;
}

int QPowerMateClient::getPowerMateGlobalControl(int& pmgbct)
{
    INT_t iOutRet = ERROR_SUCCESS;
    QDBusMessage QDBusMsgReply;
    QList <QVariant> MsgArgsList;
             
    do
    {
        
        if(FALSE == m_DBusIf_PowerMate_sp->isValid())
        {
            iOutRet = ERR_NO_QDBUS_CONN;
            break;
        }
        
        QDBusMsgReply = m_DBusIf_PowerMate_sp->call(QDBus::Block, METHOD_getPowerMateGlobalControl);
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
            pmgbct = Reply_iOutRet.toInt();
        }
    }while(FALSE);

    return iOutRet ;
    
}


void QPowerMateClient::OnTimer_PrepareDBus()
{
    QSharedPointer <QDBusConnection> DBusConn_sp;
    do
    {
        DBusConn_sp = m_DBusConn_pm.toStrongRef();
        if(DBusConn_sp.isNull())
        {
            break;
        }
        m_DBusIf_PowerMate_sp=QSharedPointer <QDBusInterface> (new QDBusInterface(SS_DBUS_SERVICE_NAME,
			DBUS_PATH_PowerMateSrv, IF_NAME_SysSrv_PowerMateSrv, (*DBusConn_sp), this));
        if(m_DBusIf_PowerMate_sp.isNull())
        {
            break;
        }
        if(m_DBusIf_PowerMate_sp->isValid())
        {
        }
	 else
	 {
		LOG_BLINE("QDbus: Can't get the SysSrv_PowerMateSrv default interface.\n");
		QTimer::singleShot(2*1000, this, SLOT(OnTimer_PrepareDBus()));
	 }
        
    }while(FALSE);
    
}

void QPowerMateClient::onPowerMateOpen()
{
    m_pmstatus=PowerMate_Open;
}

void QPowerMateClient::onPowerMateClose()
{
    m_pmstatus=PowerMate_Close;
}

