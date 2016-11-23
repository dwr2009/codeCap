#include "BackendTransactionThread.h"
#include "DbgLogSwitchDef.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include "CustomAsyncMsg.h"
#include <storageManage.h>
#include <StackBufString.h>
#include <stdlib.h>
#include <QErrorInfoList.h>
#include <QUrl>
#include <WebBrowserAppIf.h>
#include <QApplication>
#include <snmp_pp/snmperrs.h>
#include <snmp_pp/pdu.h>
#include <snmp_pp/oid.h>
#include <snmp_pp/vb.h>

QBackendTransactionThread::QBackendTransactionThread()
{
	m_BackendTransThreadId = 0;
	this->moveToThread(this);
}

QBackendTransactionThread::~QBackendTransactionThread()
{

}

void QBackendTransactionThread::On_AsyncTransaction(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	//LOG_BLINE("On_AsyncTransaction\n");
	
	do
	{
		if(MsgItem_sp.isNull())
		{
			break;
		}
		if(m_BackendTransThreadId != QThread::currentThreadId ())
		{
			LOG_BLINE("BUG,CurThrdId=%u,BackendTransThrdId=%u\n", (quint32)QThread::currentThreadId (),
				(quint32)m_BackendTransThreadId);	
			MsgItem_sp->setOutRet(ERR_INCORRECT_THREAD_CONTEXT);
			break;
		}
		MsgItem_sp->setOutRet(DoTransaction(MsgItem_sp));
	}while(FALSE);

	if(FALSE == MsgItem_sp.isNull())
	{
		emit AsyncTransactionFinished(MsgItem_sp);
	}
}

INT_t QBackendTransactionThread::DoTransaction(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	if(ControlCenter::AsyncMsgId_FormatMedia == MsgItem_sp->getMsgId())
	{
		iOutRet = DoFormatMedia(MsgItem_sp);
	}
	else if(ControlCenter::AsyncMsgId_SnmpSet == MsgItem_sp->getMsgId())
	{
		iOutRet = DoSnmpSet(MsgItem_sp);
	}
	else if(ControlCenter::AsyncMsgId_SnmpGet == MsgItem_sp->getMsgId())
	{
		iOutRet = DoSnmpGet(MsgItem_sp);
	}
	else
	{
		iOutRet = ERR_INVALID_CMD;
	}

	return iOutRet;
}

INT_t QBackendTransactionThread::DoFormatMedia(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QMsg_FormatMedia> Msg_FmtMedia_sp = MsgItem_sp.dynamicCast<QMsg_FormatMedia>();
	ControlCenter::FORMAT_MEDIA_ACTION_TYPE eFmtMediaActionType;
	ActiveMedia_TYPE eActiveMediaType;	
	StorageManage storageInfo;
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
	CWebBrowserAppIf * pAppIf = dynamic_cast<CWebBrowserAppIf *>(qApp);

	do
	{
		if(Msg_FmtMedia_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = Msg_FmtMedia_sp->getParameters(OUT eFmtMediaActionType, OUT eActiveMediaType);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
			QString strErrDesc = QString("Invalid parameters(%1,%2)").arg(eFmtMediaActionType).arg(eActiveMediaType);
			QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
			ErrInfoList << ErrInfoItem_sp;
			break;
		}
		
		if(AMedia_TYPE_FLASH != eActiveMediaType)
		{
			iRet = storageInfo.InitStorageMedia();
			if(ERROR_SUCCESS == iRet)
			{
			}
			else if(ERROR_NOT_FOUND == iRet)
			{
				iOutRet = ERROR_NOT_FOUND;
				QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
				QString strErrDesc = QString("Storage not found(Dev:%1)").arg(pAppIf->getActiveMediaDescFromType(eActiveMediaType));
				QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
				ErrInfoList << ErrInfoItem_sp;
				break;
			}
			else
			{
				iOutRet = iRet;
				break;
			}
		}
		
		if(ControlCenter::FORMAT_MEDIA_ACTION_FORMAT == eFmtMediaActionType)
		{
			if(AMedia_TYPE_FLASH == eActiveMediaType)	//not need
			{
				iRet = strShellCmd.Format("rm -fr /MP7XX/LocalMedia/*"); 
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
					QString strErrDesc = QString("Constructing Del(LocalMedia) err %1").arg(iRet);
					QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
					ErrInfoList << ErrInfoItem_sp;
					break;
				}				
				iRet = system(strShellCmd);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = ERR_DEL_FAIL;
					if(Sw_LogDbusCmd)
					{
						LOG_BLINE("Cmd(%s) ret %d\n", (LPCSTR)strShellCmd, iRet);					
					}
					QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
					QString strErrDesc = QString("Del(LocalMedia) err 0x%1").arg(iRet, 0, 16);
					QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
					ErrInfoList << ErrInfoItem_sp;
				}
				break;
			}
			else
			{
				iRet = storageInfo.OprateStorageMedia((UINT_t)eFmtMediaActionType,(UINT_t)eActiveMediaType);
				if(ERROR_SUCCESS == iRet)
				{
					iOutRet = ERROR_SUCCESS;
				}
				else
				{
					iOutRet = iRet;
					QString strRetErrDesc;
					if(ERROR_NOT_FOUND == iRet)
					{
						strRetErrDesc = "dev not found";
					}
					QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
					QString strErrDesc = 
						QString("Format(Dev:%1) err %2(%3)").arg(pAppIf->getActiveMediaDescFromType(eActiveMediaType)).arg(iRet).arg(strRetErrDesc);
					QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
					ErrInfoList << ErrInfoItem_sp;
				}
			}
		}
		else if(ControlCenter::FORMAT_MEDIA_ACTION_REPAIR == eFmtMediaActionType)
		{			
			if(AMedia_TYPE_FLASH == eActiveMediaType)	//not need
			{
				break;
			}
			else
			{
				iRet = storageInfo.OprateStorageMedia((UINT_t)eFmtMediaActionType,(UINT_t)eActiveMediaType);
				if(ERROR_SUCCESS == iRet)
				{
					break;
				}
				else
				{
					iOutRet = iRet;
					QErrorInfoList & ErrInfoList = Msg_FmtMedia_sp->getErrInfoList();
					QString strErrDesc = QString("Repair(Dev:%1) err %2").arg(pAppIf->getActiveMediaDescFromType(eActiveMediaType)).arg(iRet);
					QSharedPointer <QErrorInfoItem> ErrInfoItem_sp(new QErrorInfoItem(iRet, strErrDesc));
					ErrInfoList << ErrInfoItem_sp;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QBackendTransactionThread::CreateLocalSnmpSrvTarget()
{
	INT_t iOutRet = ERROR_SUCCESS;
	do
	{
		if(m_LocalSnmpSrvTarget_sp.isNull())
		{
			Snmp_pp::UdpAddress addrLocalSnmpSrv("127.0.0.1");
			addrLocalSnmpSrv.set_port(161);
			m_LocalSnmpSrvTarget_sp = 
				QSharedPointer<Snmp_pp::CTarget>(new Snmp_pp::CTarget(addrLocalSnmpSrv));
			if(m_LocalSnmpSrvTarget_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_LocalSnmpSrvTarget_sp->set_version(Snmp_pp::version2c);
			m_LocalSnmpSrvTarget_sp->set_retry(1);
			m_LocalSnmpSrvTarget_sp->set_timeout(10*100);	//10s
			m_LocalSnmpSrvTarget_sp->set_readcommunity("public");
			m_LocalSnmpSrvTarget_sp->set_writecommunity("public");
		}
	}while(false);
	return iOutRet;	
}

INT_t QBackendTransactionThread::DoSnmpSet(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QSharedPointer <QMsg_SnmpSet> Msg_SnmpSet_sp = MsgItem_sp.dynamicCast<QMsg_SnmpSet>();

	do
	{
		if(Msg_SnmpSet_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = CreateLocalSnmpSrvTarget();
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		QString strOid;
		QVariant varArgs;
		iOutRet = Msg_SnmpSet_sp->getParameters(strOid, varArgs);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		Snmp_pp::Pdu setPdu;
		Snmp_pp::Vb varBind;
		Snmp_pp::Oid objId(strOid.toUtf8().constData());
		if(false == objId.valid())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		varBind.set_oid(objId);
		if(QVariant::Bool == varArgs.type() || QVariant::Int == varArgs.type() ||
			QVariant::LongLong == varArgs.type() || QVariant::UInt == varArgs.type() ||
			QVariant::ULongLong == varArgs.type() || QVariant::Double == varArgs.type())
		{
			bool bConvOk = true;
			varBind.set_value(varArgs.toInt(&bConvOk));
			if(false == bConvOk)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}		
		else
		{
			varBind.set_value(varArgs.toString().toUtf8().constData());
		}
		setPdu += varBind;
		int iSnmpStatus = 0;
		iSnmpStatus = m_LocalSnmpSession_sp->set(setPdu, *m_LocalSnmpSrvTarget_sp);
		if(SNMP_CLASS_SUCCESS != iSnmpStatus)
		{
			iOutRet = ERR_SNMP_SET_FAIL;
			break;
		}
	}while(false);

	return iOutRet;
}

INT_t QBackendTransactionThread::DoSnmpGet(const MessageItem_SharedPointer_t & MsgItem_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QSharedPointer <QMsg_SnmpGet> Msg_SnmpGet_sp = MsgItem_sp.dynamicCast<QMsg_SnmpGet>();

	do
	{
		if(Msg_SnmpGet_sp.isNull())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = CreateLocalSnmpSrvTarget();
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		QString strOid;
		iOutRet = Msg_SnmpGet_sp->getParameters(strOid);
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
		Snmp_pp::Pdu getPdu;
		Snmp_pp::Vb varBind;
		Snmp_pp::Oid objId(strOid.toUtf8().constData());
		if(false == objId.valid())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		varBind.set_oid(objId);
		getPdu += varBind;
		int iSnmpStatus = 0;
		iSnmpStatus = m_LocalSnmpSession_sp->get(getPdu, *m_LocalSnmpSrvTarget_sp);
		if(SNMP_CLASS_SUCCESS != iSnmpStatus)
		{
			iOutRet = ERR_SNMP_GET_FAIL;
			break;
		}
		if(0 < getPdu.get_vb_count())
		{
			bool bRet = getPdu.get_vb(varBind, 0);
			if(true == bRet)
			{
				Msg_SnmpGet_sp->setResult(varBind.get_printable_value());
			}
			else
			{
				Msg_SnmpGet_sp->setResult("");
			}
		}
		else
		{
			Msg_SnmpGet_sp->setResult("");
		}
	}while(false);

	return iOutRet;
}

void QBackendTransactionThread::run()
{
	int iThreadExitCode = 0;
	bool bRet = false;

	Q_UNUSED(bRet);
	
	do
	{
		int iStatus = 0;
		m_LocalSnmpSession_sp = 
			QSharedPointer <Snmp_pp::Snmp> (new Snmp_pp::Snmp(iStatus, 0));
		if(m_LocalSnmpSession_sp.isNull())
		{
			LOG_BLINE("Out of memory.\n");
			break;
		}
		if(SNMP_CLASS_SUCCESS != iStatus)
		{
			LOG_BLINE("SnmpSession init err %d.\n", iStatus);
			break;
		}
		m_BackendTransThreadId = QThread::currentThreadId();
		iThreadExitCode = exec();
	}while(FALSE);

	m_LocalSnmpSrvTarget_sp.clear();
	m_LocalSnmpSession_sp.clear();
}

