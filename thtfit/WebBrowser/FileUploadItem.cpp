#include "FileUploadItem.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include "WebBrowserAppIf.h"
#include <QApplication>

QFileUploadFinishedEvent::QFileUploadFinishedEvent(enum QEvent::Type EventType) : QEvent(EventType)
{
	m_UploadId = 0;
}

QFileUploadItem::QFileUploadItem(quint32 Id)
{
	m_Id = Id;
}

QFileUploadItem::~QFileUploadItem()
{
	if(FALSE != m_SrcFile_sp.isNull())
	{
		m_SrcFile_sp->close();
		m_SrcFile_sp.clear();
	}
	m_NetAccessMgr_sp.clear();
}

quint32 QFileUploadItem::getId()
{
	return m_Id;
}

INT_t QFileUploadItem::StartUpload(CONST QString & strSrcFullFilePath, CONST QString & strUploadUrl)
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	QNetworkRequest UploadRequest;
	QUrl oUploadUrl(strUploadUrl);
	QNetworkReply * pNetworkReply = NULL;

	do
	{
		m_SrcFile_sp = QSharedPointer <QFile> (new QFile(strSrcFullFilePath));
		if(m_SrcFile_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = m_SrcFile_sp->open(QIODevice::ReadOnly);
		if(false == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		UploadRequest.setUrl(oUploadUrl);
		m_NetAccessMgr_sp = QSharedPointer <QNetworkAccessManager> (new QNetworkAccessManager(this));
		if(m_NetAccessMgr_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pNetworkReply = m_NetAccessMgr_sp->put(UploadRequest, &(*m_SrcFile_sp));
		if(NULL == pNetworkReply)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*m_NetAccessMgr_sp), SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}
		bRet = connect(&(*pNetworkReply), SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(Reply_error(QNetworkReply::NetworkError)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}		
	}while(FALSE);

	return iOutRet;
}

void QFileUploadItem::Reply_error(QNetworkReply::NetworkError NetworkErrCode)
{
	LOG_BLINE("ErrCode:%d\n", NetworkErrCode);
	if(QNetworkReply::ContentNotFoundError == NetworkErrCode)
	{		
	}

	if(QNetworkReply::NoError != NetworkErrCode)
	{
		
	}
}

void QFileUploadItem::replyFinished(QNetworkReply * pNetworkReply)
{		
	do
	{
		emit UploadFinished(*this);

		if(NULL == pNetworkReply)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
	}while(FALSE);

	if(FALSE == m_SrcFile_sp.isNull())
	{
		m_SrcFile_sp->close();
	}

	if(pNetworkReply)
	{		
		pNetworkReply->close();
		pNetworkReply->deleteLater();
	}
}

