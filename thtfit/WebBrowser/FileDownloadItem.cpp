#include "FileDownloadItem.h"
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include "WebBrowserAppIf.h"
#include <QApplication>
#include <ControlCenterApp.h>

QFileDownloadFinishedEvent::QFileDownloadFinishedEvent(enum QEvent::Type EventType) : QEvent(EventType)
{
	m_DownloadId = 0;
}

QFileDownloadItem::QFileDownloadItem(quint32 Id)
{
	m_Id = Id;
	m_pCurNetworkReply = NULL;
	m_eNetworkErr = QNetworkReply::NoError;
	m_bFileErr = FALSE;
	m_uiFileTransferredSize = 0;
}

QFileDownloadItem::~QFileDownloadItem()
{
	bool bRet;
	
	if(FALSE == m_DstFile_sp.isNull())
	{
		m_DstFile_sp->close();
		m_DstFile_sp.clear();
	}

	if(m_pCurNetworkReply)
	{
		bRet = disconnect(m_pCurNetworkReply, NULL, this, NULL);
		if(false == bRet)
		{
			LOG_BLINE("Disconnect QtSignal err\n");
		}
		m_pCurNetworkReply->deleteLater();
		m_pCurNetworkReply = NULL;
	}

	if(FALSE == m_NetAccessMgr_sp.isNull())
	{
		bRet = disconnect(&(*m_NetAccessMgr_sp), NULL, this, NULL);
		if(false == bRet)
		{
			LOG_BLINE("Disconnect QtSignal err\n");
		}
		m_NetAccessMgr_sp.clear();
	}
}

quint32 QFileDownloadItem::getId()
{
	return m_Id;
}

QNetworkReply::NetworkError QFileDownloadItem::getNetworkErr()
{
	return m_eNetworkErr;
}

BOOL_t QFileDownloadItem::isFileErr()
{
	return m_bFileErr;
}

const QString & QFileDownloadItem::getSrcUrl()
{
	return m_strSrcUrl;
}

const QString & QFileDownloadItem::getDestUrl()
{
	return m_strDstFilePath;
}

quint64 QFileDownloadItem::getFileTransferredSize()
{
	return m_uiFileTransferredSize;
}

INT_t QFileDownloadItem::StartDownload(CONST QUrl & Url, const QString & strFileType,  QString & strDstFileName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QNetworkRequest DownloadRequest;
	QNetworkReply * pNetworkReply = NULL;
	bool bRet;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> qApp;

	do
	{
		m_OrigUrl = Url;
		m_strSrcUrl = m_OrigUrl.toString();
		DownloadRequest.setUrl(m_OrigUrl);
 
		iRet = pWebBrowserAppIf->getDstFilePath(Url, strFileType, strDstFileName, m_strDstFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		m_DstFile_sp = QSharedPointer <QFile> (new QFile(m_strDstFilePath));
		if(m_DstFile_sp.isNull())
		{
			//LOG_BLINE("DstFilePath:%s\n", qPrintable(m_strDstFilePath));
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		//		LOG_BLINE("DstFilePath:%s\n", qPrintable(m_strDstFilePath));
	
		m_NetAccessMgr_sp = QSharedPointer <QNetworkAccessManager> (new QNetworkAccessManager(this));
		if(m_NetAccessMgr_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pNetworkReply = m_NetAccessMgr_sp->get(DownloadRequest);
		if(NULL == pNetworkReply)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_pCurNetworkReply = pNetworkReply;
				
		bRet = connect(&(*m_NetAccessMgr_sp), SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}
		
		bRet = connect(&(*pNetworkReply), SIGNAL(readyRead()), this, SLOT(Reply_ReadyRead()));
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

	if(ERROR_SUCCESS != iOutRet)
	{
	}

	return iOutRet;
}

INT_t QFileDownloadItem::StartDownload2(CONST QString & strSrcUrl, const QString & strDstFileName)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QNetworkRequest DownloadRequest;
	QNetworkReply * pNetworkReply = NULL;
	bool bRet;

	do
	{ 
		m_OrigUrl = QUrl(strSrcUrl);
		m_strSrcUrl = strSrcUrl;
		DownloadRequest.setUrl(m_OrigUrl);
		m_strDstFilePath = strDstFileName;

		m_DstFile_sp = QSharedPointer <QFile> (new QFile(m_strDstFilePath));
		if(m_DstFile_sp.isNull())
		{
			LOG_BLINE("DstFilePath 2:%s\n", qPrintable(m_strDstFilePath));
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		//
	
		m_NetAccessMgr_sp = QSharedPointer <QNetworkAccessManager> (new QNetworkAccessManager(this));
		if(m_NetAccessMgr_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pNetworkReply = m_NetAccessMgr_sp->get(DownloadRequest);
		if(NULL == pNetworkReply)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		m_pCurNetworkReply = pNetworkReply;
				
		bRet = connect(&(*m_NetAccessMgr_sp), SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}
		
		bRet = connect(&(*pNetworkReply), SIGNAL(readyRead()), this, SLOT(Reply_ReadyRead()));
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

	if(ERROR_SUCCESS != iOutRet)
	{
	}

	return iOutRet;
}

INT_t QFileDownloadItem::CancelDownload()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		//LOG_BLINE("CancelDownload\n");
		if(NULL == m_pCurNetworkReply)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		if(m_pCurNetworkReply)
		{
			m_pCurNetworkReply->abort();
		}
	}while(FALSE);

	return iOutRet;
}

void QFileDownloadItem::Reply_ReadyRead()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	char szReadBuf[16*1024];
	qint64 BytesRead, BytesWritten;
	QObject * pSender = NULL;
	QNetworkReply * pNetworkReply = NULL;
	bool bRet;
	QControlCenterApp * pApp = (typeof(pApp))qApp;
	CMp7xxGlobalStatus & Mp7xxGlobalStatus = pApp->getMp7xxGlobalStatusObj();
	
	do
	{
		pSender = sender();
		if(NULL == pSender)
		{
			break;
		}
		pNetworkReply = qobject_cast <QNetworkReply *> (pSender);
		if(NULL == pNetworkReply)
		{
			break;
		}
		//LOG_BLINE("ReplySize:%lld\n", pNetworkReply->size());

		if(m_DstFile_sp.isNull())
		{
			break;
		}

		if(false == m_DstFile_sp->isOpen())
		{
			bRet = m_DstFile_sp->open(QIODevice::WriteOnly | QIODevice::Truncate);
			if(false == bRet)
			{
				m_bFileErr = TRUE;
				iOutRet = ERROR_FILE_OPEN_FAIL;
				break;
			}
		}
		
		while(TRUE)
		{
			BytesRead = pNetworkReply->read(szReadBuf, sizeof(szReadBuf));
			if(0 >= BytesRead)
			{
				break;
			}
			m_uiFileTransferredSize += BytesRead;
			//LOG_BLINE("Read:%lld\n", BytesRead);
			BytesWritten = m_DstFile_sp->write(szReadBuf, BytesRead);
			if(BytesWritten != BytesRead)
			{
				LOG_BLINE("ToWrite:%lld,Written:%lld\n", BytesRead, BytesWritten);
				break;
			}
			//mark writing fs
			iRet = Mp7xxGlobalStatus.set_WritingFs_Info(TRUE, time(NULL));
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
	}while(FALSE);

	if(ERROR_SUCCESS != iOutRet)
	{
		if(pNetworkReply)
		{
			pNetworkReply->abort();
			if(FALSE == m_DstFile_sp.isNull())
			{
				m_DstFile_sp->close();
			}
		}
	}	
}

void QFileDownloadItem::Reply_error(QNetworkReply::NetworkError NetworkErrCode)
{
	LOG_BLINE("ErrCode:%d\n", NetworkErrCode);
	m_eNetworkErr = NetworkErrCode;	
	if(QNetworkReply::ContentNotFoundError == NetworkErrCode)
	{		
	}

	if(QNetworkReply::NoError != NetworkErrCode)
	{
		
	}
}

void QFileDownloadItem::replyFinished(QNetworkReply * pNetworkReply)
{
	char szReadBuf[16*1024];
	qint64 BytesRead, BytesWritten;
	bool bRet;

	//LOG_BLINE("replyFinished\n");
	
	do
	{
		//emit DownloadFinished(*this);

		if(NULL == pNetworkReply)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		
		if(m_DstFile_sp.isNull())
		{
			break;
		}

		if(QNetworkReply::NoError == pNetworkReply->error())
		{
			if(false == m_DstFile_sp->isOpen())
			{
				bRet = m_DstFile_sp->open(QIODevice::WriteOnly | QIODevice::Truncate);
				if(false == bRet)
				{
					m_bFileErr = TRUE;
					break;
				}
			}
			
			while(TRUE)
			{
				BytesRead = pNetworkReply->read(szReadBuf, sizeof(szReadBuf));
				if(0 >= BytesRead)
				{
					break;
				}
				//LOG_BLINE("Read:%lld\n", BytesRead);
				BytesWritten = m_DstFile_sp->write(szReadBuf, BytesRead);
				if(BytesWritten != BytesRead)
				{
					LOG_BLINE("ToWrite:%lld,Written:%lld\n", BytesRead, BytesWritten);
					break;
				}
			}			
		}
	}while(FALSE);

	if(FALSE == m_DstFile_sp.isNull())
	{
		m_DstFile_sp->close();
	}

	if(pNetworkReply)
	{		
		m_pCurNetworkReply = NULL;
		pNetworkReply->close();
		pNetworkReply->deleteLater();
		emit DownloadFinished(*this);		
	}

	{
		if(pNetworkReply)
		{		
			emit StartTransfer(*this);
		}
	}
}

