#include "MediaPlayerCtrl.h"
#include <BaseErrDef.h>
#include <QDBusMessage>
#include <ErrPrintHelper.h>
#include <QVariant>
#include <MediaPlayerAppDef.h>
#include <Mp7xxGlobalStatus.h>
#include "Api/WebBrowserAppDef.h"
#include "QPowerMateClient.h"
#include "WebBrowserAppIf.h"



QMediaPlayerCtrl::QMediaPlayerCtrl()
{

}

QMediaPlayerCtrl::~QMediaPlayerCtrl()
{

}

INT_t QMediaPlayerCtrl::setDBusConnection(QWeakPointer <QDBusConnection> DBusConn_wp)
{
	m_DBusConn_wp = DBusConn_wp;

	return ERROR_SUCCESS;
}

INT_t QMediaPlayerCtrl::setQDBusMpDefaultIf(QSharedPointer <QDBusInterface> & QDBusMpDefaultIf_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_QDBusMpDefaultIf_wp = QDBusMpDefaultIf_sp;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::Play(CONST QString & strUrl, unsigned int & PLaybackId, qint32 iLoopCount)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int replayMediaTimes = 0;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setDataSource, strUrl);
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
		//QDBusMessage::ReplyMessage
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

replay_media:	
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_Prepare);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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
#define INFINITELYLOOP  0
		if(iLoopCount == INFINITELYLOOP)
		{
			iLoopCount = 0x7FFFFFFF ;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_Play2, iLoopCount);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}		
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(iRet == ERROR_INVALID_STATE)
			{
				INT_t iCurrPlayStatus= PLAY_STAT_IDLE;
				iOutRet = getMpSrvPlayStatus(&iCurrPlayStatus);
				if(iOutRet != ERROR_SUCCESS) 
				{
					break;
				}
				if(iCurrPlayStatus == PLAY_STAT_IDLE)//maybe not think about idle
				{
					goto replay_media;
					break;
				}
				iOutRet = Stop();
				if(iOutRet == ERROR_SUCCESS) 
				{
					if(replayMediaTimes == 0)
					{
						replayMediaTimes ++ ;
						goto replay_media;
					}
				}
				iOutRet = iRet;
				break;
			}
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			CONST QVariant & Reply_Param = MsgArgsList.at(1);
			PLaybackId = Reply_Param.toUInt();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::Stop(UINT32_t * pPlaybackId/* = NULL*/, QString * pStrUrl/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		if(pPlaybackId)
		{
			*pPlaybackId = 0;	//invalid
		}

		/*maybe tuner scan problem*/ 
		INT_t iCurrPlayStatus= PLAY_STAT_IDLE;
		iOutRet = getMpSrvPlayStatus(&iCurrPlayStatus);
		if(iOutRet != ERROR_SUCCESS) 
		{
			break;
		}
		if(iCurrPlayStatus == PLAY_STAT_IDLE)
		{
			break;
		}		

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_Stop);
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
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		//iOutRet
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		if(3 > MsgArgsList.count())
		{
			break;
		}
		//PlaybackId
		{
			if(pPlaybackId)
			{
				CONST QVariant & replayParam = MsgArgsList.at(1);
				uint playbackId = replayParam.toUInt();
				*pPlaybackId = playbackId;
			}
		}
		//url
		{
			if(pStrUrl)
			{
				CONST QVariant & replayParam = MsgArgsList.at(2);
				*pStrUrl = replayParam.toString();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t  QMediaPlayerCtrl::Seek(int & iPosition)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	unsigned int SpecificTime = iPosition * 1000;/*ms*/

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_Seek,SpecificTime);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
		}
		else	//QDBusMessage::ReplyMessage
		{
			iOutRet = ERROR_SUCCESS;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
			break;
		}

		CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
		iRet = Reply_iOutRet.toInt();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(false);
		
	return iOutRet;
}
INT_t  QMediaPlayerCtrl::setVideoPosition(int x , int  y,int width,int height)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		LeftTryTimes = 2;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setVideoPosition, x, y, width, height);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

INT_t QMediaPlayerCtrl::setOsdSize(int iWidth,int iHeight)
{
    INT_t iOutRet = ERROR_SUCCESS, iRet;
    QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
    QDBusMessage QDBusMsgReply;
    QList <QVariant> MsgArgsList;
    int LeftTryTimes;

    do
    {
        QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
        if(QDBusMpDefaultIf_sp.isNull())
        {
            iOutRet = ERR_NO_QDBUS_CONN;
            break;
        }
        if(FALSE == QDBusMpDefaultIf_sp->isValid())
        {
            iOutRet = ERR_NO_QDBUS_CONN;
            break;
        }
        LeftTryTimes = 2;
        while(LeftTryTimes--)
        {
            QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block,METHOD_setOsdSize,iWidth,iHeight);
            if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
            {
                iOutRet = ERR_DBUS_CALL_ERR;
                LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
            }
            else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
            {
                iOutRet = ERR_DBUS_CALL_ERR;
                LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
            }
            else//QDBusMessage::ReplyMessage
            {
                iOutRet = ERROR_SUCCESS;
                break;	//success
            }
        }
        if(ERROR_SUCCESS != iOutRet)
        {
            break;
        }
        MsgArgsList = QDBusMsgReply.arguments();
        if(0 >= MsgArgsList.count())
        {
            iOutRet = ERROR_INVALID_PARAMETER;
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

INT_t QMediaPlayerCtrl::changeVolume(INT_t VolChangePercent)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_ChangeVolume, VolChangePercent);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
		}
		else	//QDBusMessage::ReplyMessage
		{
			iOutRet = ERROR_SUCCESS;
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
		
#if 1 /*Add by xuweiwei 2014-3-5*/
            emit onChangeVolume_signal(VolChangePercent);
#endif 
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::getCurrentVolume(INT_t * iCurVolume)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getCurrentVolume);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
		}
		else	//QDBusMessage::ReplyMessage
		{
			iOutRet = ERROR_SUCCESS;
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_CurVolPercent = MsgArgsList.at(1);
			*iCurVolume = Reply_CurVolPercent.toInt();
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::OnSaveVolume(INT_t iVolume)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_OnSaveVolume,iVolume);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
		}
		else	//QDBusMessage::ReplyMessage
		{
			iOutRet = ERROR_SUCCESS;
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::setHwRtcTime(int Year, int Month, int Date, int Hour, int Minute, int Second, int strTimeZone)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setHwRtcTime, Year, Month, Date, Hour, Minute, Second, strTimeZone);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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

INT_t QMediaPlayerCtrl::getTimeZoneSetting(OUT int & TimezoneHourOff)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getTimeZoneSetting);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewVolPercent = MsgArgsList.at(1);
			TimezoneHourOff = Reply_NewVolPercent.toInt();
			LOG_BLINE("~~~~TimezoneHourOff:%d\n", TimezoneHourOff);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::setGpioLedStatus(CONST UINT32_t GpioLedStatusToSet, UINT32_t * CONST pActualLedStatus/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setGpioLedStatus, GpioLedStatusToSet);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
		if(pActualLedStatus)
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(1);
			*pActualLedStatus = Reply_Arg.toUInt();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::getMonitorTypeInfo(OUT QString & MonitorTypeInfo)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getMonitorInfo);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_MonitorInfo = MsgArgsList.at(1);
			MonitorTypeInfo = Reply_MonitorInfo.toString();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::setPlayMode(int PlayMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setPlayMode, PlayMode);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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

INT_t QMediaPlayerCtrl::DispOnLedScrAsync(QString strContentToDisp, CONST INT_t StartPosToDisp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	//LOG_LINE("strContentToDisp=%s\n",qPrintable(strContentToDisp));

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

#define MAXSHOWCONTENT 14

#if 1/*added by lxj 2012-7-30, LED can't support lower case char*/
		strContentToDisp = strContentToDisp.toUpper();
#endif
		if(strContentToDisp.size() < MAXSHOWCONTENT)
		{
			for(int i = strContentToDisp.size() ; i < 15 ;i ++)
			{
				strContentToDisp.append(" ");
			}
		}
		else
		{

		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_displayOnLedScreen, strContentToDisp,StartPosToDisp);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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


INT_t QMediaPlayerCtrl::setDisplayParam(int eDispParamType, INT_t iValue,INT_t isSave)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;


	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}


		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setDisplayParam, eDispParamType,iValue,isSave);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(1 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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

INT_t QMediaPlayerCtrl::getDisplayParam(int eDispParamType, INT_t * pCurValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;


	do
	{	
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}


		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getDisplayParam, eDispParamType);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_CurrPercent = MsgArgsList.at(1);
			if(pCurValue)
			{
				*pCurValue = Reply_CurrPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::setOsdPosition(int x,int y,int width,int height,int type)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setOsdPosition, x, y, width, height,type);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

INT_t QMediaPlayerCtrl::ChangeVideoOutputMode(int iResolutionType,int * iCurrMode)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

	//	LOG_BLINE("iResolutionType Type:%d\n", iResolutionType);

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_ChangeVideoOutputMode, iResolutionType);
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
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewVolPercent = MsgArgsList.at(1);
			if(iCurrMode)
			{
				*iCurrMode = Reply_NewVolPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::getDisplayPosition(int eRuaDispType,int * pCurrXpos,
		int * pCurrYpos,int * pCurrWidth,int * pCurrHeight)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getDisplayPosition,eRuaDispType);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(5 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(4);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			if(pCurrXpos)
			{
				*pCurrXpos = MsgArgsList.at(0).toInt();
			}
			if(pCurrYpos)
			{
				*pCurrYpos = MsgArgsList.at(1).toInt();
			}
			if(pCurrWidth)
			{
				*pCurrWidth =  MsgArgsList.at(2).toInt();
			}
			if(pCurrHeight)
			{
				*pCurrHeight =  MsgArgsList.at(3).toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::getVideoPosition(int eRuaDispType,int * pCurrXpos,
		int * pCurrYpos,int * pCurrWidth,int * pCurrHeight)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getVideoPosition,eRuaDispType);
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
		else	//QDBusMessage::ReplyMessage
		{
		}

		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		MsgArgsList = QDBusMsgReply.arguments();
		if(5 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(4);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		{
			if(pCurrXpos)
			{
				*pCurrXpos = MsgArgsList.at(0).toInt();
			}
			if(pCurrYpos)
			{
				*pCurrYpos = MsgArgsList.at(1).toInt();
			}
			if(pCurrWidth)
			{
				*pCurrWidth =  MsgArgsList.at(2).toInt();
			}
			if(pCurrHeight)
			{
				*pCurrHeight =  MsgArgsList.at(3).toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::setPlaySpeedCtrlAsync(int iSpeed)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setPlaySpeedCtrlAsync,iSpeed);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::getPlaySpeedCtrl(int *  iSpeedValue)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getPlaySpeedCtrl);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iSpeedValue)
			{
				*iSpeedValue = Reply_NewSpeedPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::getPlayMode(int *  iPlayMoed)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getPlayMode);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iPlayMoed)
			{
				*iPlayMoed = Reply_NewSpeedPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::setHdmiAudioOutput(BOOL_t bEnable)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QDBusMessage DbusMsgToSend;
	QDBusMessage DbusMsgReply;
	int TimeoutMs;
	QList <QVariant> MsgArgsList;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		DBusConn_sp = m_DBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		DbusMsgToSend = QDBusMessage::createMethodCall(DBUS_MPLAYER_SERVICE_NAME, DBUS_MPLAYER_OBJECT_PATH,
			DBUS_MPLAYER_OBJECT_INTERFACE, METHOD_setHdmiAudioOutput);
		MsgArgsList <<  bEnable;
		DbusMsgToSend.setArguments(MsgArgsList);
		TimeoutMs = (5*1000);
		DbusMsgReply = DBusConn_sp->call(DbusMsgToSend, QDBus::Block, TimeoutMs);
		if(QDBusMessage::ErrorMessage == DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(DbusMsgReply.errorName()), qPrintable(DbusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != DbusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", DbusMsgReply.type());
		}
		MsgArgsList = DbusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_Arg = MsgArgsList.at(0);
			iRet = Reply_Arg.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::PlayTuner(CONST QString & strUrl, unsigned int & PLaybackId)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int CallTimeoutMs;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		CallTimeoutMs = (10*1000);
		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_setDataSource, strUrl);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_Prepare);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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
		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_PlayTuner);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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
			CONST QVariant & Reply_Param = MsgArgsList.at(MsgArgsList.size() - 1);
			PLaybackId = Reply_Param.toUInt();
		}
	}while(FALSE);

	return iOutRet;
}

INT_t  QMediaPlayerCtrl::getTunerChannelStrength(int * iStrength)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_getTunerChannelStrength);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iStrength)
			{
				*iStrength = Reply_NewSpeedPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::ResetDuration(int iDuration)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	Q_UNUSED(iDuration);

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_resetDuration);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

INT_t QMediaPlayerCtrl::ChannelUp()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int CallTimeoutMs;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_ChannelUp);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::ChannelDown()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int CallTimeoutMs;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_ChannelDown);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::TunerScan()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int CallTimeoutMs;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_TunerScan);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::TunerMode(int iMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int CallTimeoutMs;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		CallTimeoutMs = (10*1000);
		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_TunerMode,iMode);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}

#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
INT_t QMediaPlayerCtrl::setQDBusMediaServerIf(QSharedPointer <QDBusInterface> & QDBusMediaServerIf_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_QDBusMediaServerIf_wp = QDBusMediaServerIf_sp;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::MediaServer_PlayAudio(const QString & strUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		//LOG_BLINE("MediaServer_PlayAudio() url=%s\n",qPrintable(strUrl));

		QDBusMediaServerIf_sp = m_QDBusMediaServerIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_MEDIASERVER_PLAY_AUDIO, strUrl);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

	//LOG_BLINE("MediaServer_PlayAudio() iOutRet=%d,%s\n",iOutRet,qPrintable(strUrl));
	return iOutRet;
}

INT_t QMediaPlayerCtrl::MediaServer_StopAudio(void)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMediaServerIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_MEDIASERVER_STOP_AUDIO);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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

	//LOG_BLINE("MediaServer_StopAudio() iOutRet=%d\n",iOutRet);
	return iOutRet;
}
#endif


INT_t QMediaPlayerCtrl::getChannelTable(int * iEntryId,int * channelNumer,
		int * subChannelCount, QString & strProgramPids,QStringList & FilenameList)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;
	
	int iCurrEntryId = 0;
	do
	{
		while(1)
		{
			iCurrEntryId ++;
			QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
			if(QDBusMpDefaultIf_sp.isNull())
			{
				iOutRet = ERR_NO_QDBUS_CONN;
				break;
			}
			if(FALSE == QDBusMpDefaultIf_sp->isValid())
			{
				iOutRet = ERR_NO_QDBUS_CONN;
				break;
			}

			LeftTryTimes = 1;
			while(LeftTryTimes--)
			{
				QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_GetChannelTable,iCurrEntryId);
				if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
				{
					iOutRet = ERR_DBUS_CALL_ERR;
					LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
				}
				else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
				{
					iOutRet = ERR_DBUS_CALL_ERR;
					LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
				}
				else	//QDBusMessage::ReplyMessage
				{
					iOutRet = ERROR_SUCCESS;
					break;	//success
				}
			}
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
			MsgArgsList = QDBusMsgReply.arguments();
			if(4 > MsgArgsList.count())
			{
				iOutRet = ERR_INVALID_REPLY;
				break;
			}
			
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS == iRet )
			{
				iOutRet = iRet;		
			}
			else if(ERR_NO_MORE_DATA == iRet)
			{
				iOutRet = iRet;	
				break;
			}
			else 
			{
				iOutRet = iRet;	
				break;
			}				
			{
				if(iEntryId)
				{
					 *iEntryId = iCurrEntryId;;
				}
				CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
				if(channelNumer)
				{
					*channelNumer = Reply_NewSpeedPercent.toInt();
				}

				CONST QVariant & Reply_SubChannelCount= MsgArgsList.at(2);
				if(subChannelCount)
				{
					*subChannelCount = Reply_SubChannelCount.toInt();
				}					
				CONST QVariant & Reply_StrProgramPids = MsgArgsList.at(3);
				strProgramPids = Reply_StrProgramPids.toString();			
			}
			
			QString strInfoList;
			QTextStream(&strInfoList) <<* iEntryId << "\n"  
				<<* channelNumer << "\n"  <<* subChannelCount << "\n"   
				<<  strProgramPids ;
			
			FilenameList << strInfoList;	
		}
	}while(FALSE);

	return iOutRet;
}

INT_t  QMediaPlayerCtrl::setHdmi3DTvMode(int iTvMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_SetHdmi3DTvMode,iTvMode);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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


INT_t QMediaPlayerCtrl::setAnalogAudioMute(CONST BOOL_t bMute)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_SetAnalogAudioMute,bMute);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
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


INT_t  QMediaPlayerCtrl::getFirstChNumber(int * majorNo,int  *minorNo)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_GetFirstChNumber);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(3 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(majorNo)
			{
				*majorNo = Reply_NewSpeedPercent.toInt();
			}
		}
		{
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(2);
			if(minorNo)
			{
				*minorNo = Reply_NewSpeedPercent.toInt();
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t  QMediaPlayerCtrl::getForceTxMode(int * iTxMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_GetForceTxMode);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iTxMode)
			{
				*iTxMode = Reply_NewSpeedPercent.toInt();
			}
		}
		
	}while(FALSE);

	return iOutRet;
}


INT_t  QMediaPlayerCtrl::getTunerMode(int * iCurrTunerMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_GetTunerMode);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iCurrTunerMode)
			{
				*iCurrTunerMode = Reply_NewSpeedPercent.toInt();
			}
		}
		
	}while(FALSE);

	return iOutRet;
}


INT_t QMediaPlayerCtrl::getOutputSpdifMode(int * iSpdifMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;
	int LeftTryTimes;
		
	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		LeftTryTimes = 1;
		while(LeftTryTimes--)
		{
			QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_GetOutputSpdifMode);
			if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
			}
			else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
			{
				iOutRet = ERR_DBUS_CALL_ERR;
				LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
			}
			else	//QDBusMessage::ReplyMessage
			{
				iOutRet = ERROR_SUCCESS;
				break;	//success
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & Reply_NewSpeedPercent = MsgArgsList.at(1);
			if(iSpdifMode)
			{
				*iSpdifMode = Reply_NewSpeedPercent.toInt();
			}
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaPlayerCtrl::setOutputSpdifMode(int iMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_SetOutputSpdifMode,iMode);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;

}

INT_t QMediaPlayerCtrl::setTunerChannelMode(int iTunerChannelMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMpDefaultIf_sp;
	QDBusMessage QDBusMsgCall, QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMpDefaultIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMpDefaultIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMpDefaultIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMpDefaultIf_sp->call(QDBus::Block, METHOD_SetTunerChannelMode,iTunerChannelMode);
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
		if(0 >= MsgArgsList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		{
			CONST QVariant & Reply_iOutRet = MsgArgsList.at(0);
			iRet = Reply_iOutRet.toInt();
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
	}while(FALSE);

	return iOutRet;
}


INT_t  QMediaPlayerCtrl::getTunerChannelMode(int * iCurrTunerMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QDBusInterface> QDBusMediaServerIf_sp;
	QDBusMessage QDBusMsgReply;
	QList <QVariant> MsgArgsList;

	do
	{
		QDBusMediaServerIf_sp = m_QDBusMpDefaultIf_wp.toStrongRef();
		if(QDBusMediaServerIf_sp.isNull())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}
		if(FALSE == QDBusMediaServerIf_sp->isValid())
		{
			iOutRet = ERR_NO_QDBUS_CONN;
			break;
		}

		QDBusMsgReply = QDBusMediaServerIf_sp->call(QDBus::Block, METHOD_GetTunerChannelMode);
		if(QDBusMessage::ErrorMessage == QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("%s,%s\n", qPrintable(QDBusMsgReply.errorName()), qPrintable(QDBusMsgReply.errorMessage()));
		}
		else if(QDBusMessage::ReplyMessage != QDBusMsgReply.type())
		{
			iOutRet = ERR_DBUS_CALL_ERR;
			LOG_BLINE("Type:%d\n", QDBusMsgReply.type());
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		MsgArgsList = QDBusMsgReply.arguments();
		if(2 > MsgArgsList.count())
		{
			iOutRet = ERR_INVALID_REPLY;
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
			CONST QVariant & replyParam = MsgArgsList.at(1);
			if(iCurrTunerMode)
			{
				*iCurrTunerMode = replyParam.toInt();
			}
		}
		
	}while(FALSE);

	return iOutRet;
}

