#include "DataCommService.h"
#include <QDBusError>
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <QUrl>
#include <QPointer>
#include <QFileInfo>
#include <QApplication>
#include "Api/WebBrowserAppDef.h"
#include <stdlib.h>
#include "WebBrowserAppIf.h"
#include <AutoLock.h>
//#include <QHttp>
#include <QNetworkAccessManager>
#include <Compiler.h>
#include "QEventType.h"
#include <QUrlInfoEx.h>
#include <QDir>
#include <QTextStream>
#include <DirectoryManage.h>
#include "DbgLogSwitchDef.h"

QDBusIf_DataCommSrv::QDBusIf_DataCommSrv(CDataCommService * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pDataCommSrv = pParentObj;
	setAutoRelaySignals(true);
}

void QDBusIf_DataCommSrv::setParentObj(CDataCommService * pParentObj)
{
	m_pDataCommSrv = pParentObj;
}

void QDBusIf_DataCommSrv::DownloadFile(QString strFileUrl, QString strFileType, QString strDstFileName, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->DownloadFile(strFileUrl, strFileType, strDstFileName, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::DownloadFile2(QString strSrcUrl, QString strDestFile, int & iOutRet, quint32 & DownloadId)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->DownloadFile2(strSrcUrl, strDestFile, iOutRet, DownloadId);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::UploadFile(QString strFilename, QString strFileType, QString strUploadUrl, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->UploadFile(strFilename, strFileType, strUploadUrl, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::UploadFile2(QString strSrcFile, QString strUploadUrl, QString strOptions, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->UploadFile2(strSrcFile, strUploadUrl, strOptions, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::renameMediaFile(QString strOldFilename, QString strNewFilename, QString strFileType, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->renameMediaFile(strOldFilename, strNewFilename, strFileType, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::deleteMediaFile(QString strFilename, QString strFileType, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->deleteMediaFile(strFilename, strFileType, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::setActiveMedia(QString strActiveMedia, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->setActiveMedia(strActiveMedia, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::getActiveMedia(int & iOutRet, QString & strActiveMediaDesc, QString & strActiveMediaDirNanme)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->getActiveMedia(iOutRet, strActiveMediaDesc, strActiveMediaDirNanme);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::getMediaFreeSpace(int & iOutRet, qulonglong & MediaFreeSpaceInBytes)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->getMediaFreeSpace(iOutRet, MediaFreeSpaceInBytes);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::getTransferControl(int & iOutRet, int & TransferStatusCode)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->getTransferControl(iOutRet, TransferStatusCode);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::setTransferControl(int TransferCtrlCmdCode, int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->setTransferControl(TransferCtrlCmdCode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::getFileList(QString strFileType, int & iOutRet, QStringList & FilenameList)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->getFileList(strFileType, iOutRet, FilenameList);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::setStorePlaylistFile(QString strPlaylistUrl,int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->setStorePlaylistFile(strPlaylistUrl, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_DataCommSrv::setDispOsdResolutionId(int iResolutionId,int & iOutRet)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->setDispOsdResolutionId (iResolutionId,iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

void QDBusIf_DataCommSrv::getDispOsdResolutionId(int & iOutRet, int & iCurResolutionId)
{
	if(m_pDataCommSrv)
	{
		m_pDataCommSrv->getDispOsdResolutionId (iOutRet,iCurResolutionId);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}	
}

CDataCommService::CDataCommService() : m_FileDownloadDataMutex(QMutex::Recursive)
{
	m_NextDownloadId = 0;
	m_NextUploadId = 0;
	m_TransferCtrlStatus = TranCtrlStat_DownloadIdle;
}

CDataCommService::~CDataCommService()
{
	{
		QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
		m_FileDownloadItemList.clear();
	}
}

INT_t CDataCommService::setQDBusConn(QSharedPointer <QDBusConnection> & QDBusConn_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_QDBusConn_wp = QDBusConn_sp;
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		DBusConn_sp = m_QDBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		m_DBusIfAdaptor_DataCommSrv_sp = QSharedPointer <QDBusIf_DataCommSrv> (new QDBusIf_DataCommSrv(this));
		if(m_DBusIfAdaptor_DataCommSrv_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		bRet = DBusConn_sp->registerObject(DBUS_PATH_WebBrowser_DatCommSrv, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus srv %s\n", DBUS_PATH_WebBrowser_DatCommSrv);
			}
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::ExitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		m_FileUploadItemList.clear();
		{
			QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
			m_FileDownloadItemList.clear();
		}

		if(FALSE == m_DBusIfAdaptor_DataCommSrv_sp.isNull())
		{
			m_DBusIfAdaptor_DataCommSrv_sp->setParentObj(NULL);
			m_DBusIfAdaptor_DataCommSrv_sp.clear();
		}
		
		DBusConn_sp = m_QDBusConn_wp.toStrongRef();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_WebBrowser_DatCommSrv);			
		}
	}while(FALSE);

	return iOutRet;
}

void CDataCommService::DownloadFile(const QString & strFileUrl, const QString & strFileType, 
	 QString & strDstFileName, int & iOutRet)
{	
	QUrl FileUrl(strFileUrl);
	QString strUrlScheme;

	iOutRet = ERROR_SUCCESS;

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("DownloadFile(%s,%s,%s)\n", qPrintable(strFileUrl), qPrintable(strFileType), qPrintable(strDstFileName));
	}
		
	do
	{
		strUrlScheme = FileUrl.scheme();
		//LOG_BLINE("Scheme:%s\n", qPrintable(strUrlScheme));
		if("file" == strUrlScheme || "" == strUrlScheme)	//local file
		{
			iOutRet = DownloadFile_Local(FileUrl, strFileType, strDstFileName);
		}
		else if("http" == strUrlScheme || "ftp" == strUrlScheme)	//http or ftp file
		{
			iOutRet = DownloadFile_HttpFtp(FileUrl, strFileType, strDstFileName);
		}
		else	//other not supported
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS == iOutRet)
	{
		m_TransferCtrlStatus = TranCtrlStat_DownloadInProgress;
	}

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("DownloadFile ret %d\n", iOutRet);
	}
}

void CDataCommService::DownloadFile2(const QString & strSrcUrl, const QString & strDestFile, int & iOutRet, quint32 & DownloadId)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		iOutRet = DownloadFile_HttpFtp2(strSrcUrl, strDestFile, DownloadId);
	}while(FALSE);
}

void CDataCommService::UploadFile(const QString & strFilename, const QString & strFileType, const QString & strUploadUrl, int & iOutRet)
{
	INT_t iRet;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);
	QString strSrcFullFilePath, strFullUploadUrl;
	
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(MAX_UploadFile_ConcurrentCnt <= m_FileUploadItemList.count())
		{
			iOutRet = ERR_BUSY;
			break;
		}

		if(strFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}

		iRet = pWebBrowserAppIf->getFullFilePath(strFilename, strFileType, strSrcFullFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//construct the complete destination url
		strFullUploadUrl = strUploadUrl;
		if("/" != strUploadUrl.right(1))
		{
			strFullUploadUrl += '/';
		}
		strFullUploadUrl += strFilename;
		iOutRet = UploadFile_HttpFtp(strSrcFullFilePath, strFullUploadUrl);
	}while(FALSE);
}


void CDataCommService::UploadFile2(const QString & strSrcFile, const QString & strUploadUrl, const QString & strOptions, int & iOutRet)
{
	INT_t iRet;
	QParamParser oParamParser;
	QParamParser::PROPERTY_VALUE_MAP PropertyValueMap;
	QString strFullUploadUrl = strUploadUrl;
	
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		if(MAX_UploadFile_ConcurrentCnt <= m_FileUploadItemList.count())
		{
			iOutRet = ERR_BUSY;
			break;
		}

		if(strSrcFile.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}

		iRet = oParamParser.ParseText(strOptions, PropertyValueMap);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if("/" == strFullUploadUrl.right(1))
		{
			QFileInfo SrcFileInfo(strSrcFile);		
			QString strSrcBasename = SrcFileInfo.fileName();		
			strFullUploadUrl += strSrcBasename;
		}
		iOutRet = UploadFile_HttpFtp(strSrcFile, strFullUploadUrl, PropertyValueMap);
	}while(FALSE);
}

void CDataCommService::renameMediaFile(const QString & strOldFilename, const QString & strNewFilename, 
	const QString & strFileType, int & iOutRet)
{
	INT_t iRet;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);
	QString strOldFullFilePath, strNewFullFilePath;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(strOldFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}

		iRet = pWebBrowserAppIf->getFullFilePath(strOldFilename, strFileType, strOldFullFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(strOldFullFilePath == "")
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		
		if(strNewFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}

		iRet = pWebBrowserAppIf->getFullFilePath(strNewFilename, strFileType, strNewFullFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(strNewFullFilePath == "")
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		
		if(strOldFullFilePath == strNewFullFilePath)
		{
			break;
		}

		if(false == QFile::exists(strOldFullFilePath))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}

		iRet = rename(strOldFullFilePath.toLocal8Bit().constData(), strNewFullFilePath.toLocal8Bit().constData());
		if(0 != iRet)
		{
			iOutRet = ERR_RENAME_FAIL;
			break;
		}

		if(FileType_Desc_Video == strFileType)
		{
			QString OldPrfFilePath;
			//<Filename>.prf
			OldPrfFilePath = strOldFullFilePath + "." + PRF_FILE_EXTENSION_NAME;
			if(QFile::exists(OldPrfFilePath))
			{
				QString NewPrfFilePath = strNewFullFilePath + "." + PRF_FILE_EXTENSION_NAME;
				iRet = rename(OldPrfFilePath.toLocal8Bit().constData(), NewPrfFilePath.toLocal8Bit().constData());
				if(0 != iRet)
				{
					iOutRet = ERR_RENAME_FAIL;
					break;
				}
			}

			//<Filename.MainName>.prf
			OldPrfFilePath = QUrlInfoEx::RemoveExtensionName(strOldFullFilePath) + "." + PRF_FILE_EXTENSION_NAME;
			if(QFile::exists(OldPrfFilePath))
			{
				QString NewPrfFilePath = QUrlInfoEx::RemoveExtensionName(strNewFullFilePath) + "." + PRF_FILE_EXTENSION_NAME;
				iRet = rename(OldPrfFilePath.toLocal8Bit().constData(), NewPrfFilePath.toLocal8Bit().constData());
				if(0 != iRet)
				{
					iOutRet = ERR_RENAME_FAIL;
					break;
				}
			}
		}
	}while(FALSE);
}

void CDataCommService::deleteMediaFile(const QString & strFilename, const QString & strFileType, int & iOutRet)
{
	INT_t iRet;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);
	QString strFullFilePath;

	iOutRet = ERROR_SUCCESS;

	do
	{
		if(strFilename.isEmpty())
		{
			iOutRet = ERR_INVALID_FILENAME;
			break;
		}

		iRet = pWebBrowserAppIf->getFullFilePath(strFilename, strFileType, strFullFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(strFullFilePath == "")
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
				
		if(false == QFile::exists(strFullFilePath))
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}

		iRet = remove(strFullFilePath.toLocal8Bit().constData());
		if(0 != iRet)
		{
			iOutRet = ERR_SYS_remove_FAIL;
			break;
		}
		
		if(FileType_Desc_Video == strFileType)
		{
			QString PrfFilePath;
			//<Filename>.prf
			PrfFilePath = strFullFilePath + "." + PRF_FILE_EXTENSION_NAME;
			if(QFile::exists(PrfFilePath))
			{
				iRet = remove(PrfFilePath.toLocal8Bit().constData());
				if(0 != iRet)
				{
					iOutRet = ERR_SYS_remove_FAIL;
					break;
				}
			}

			//<Filename.MainName>.prf
			PrfFilePath = QUrlInfoEx::RemoveExtensionName(strFullFilePath) + "." + PRF_FILE_EXTENSION_NAME;
			if(QFile::exists(PrfFilePath))
			{
				iRet = remove(PrfFilePath.toLocal8Bit().constData());
				if(0 != iRet)
				{
					iOutRet = ERR_SYS_remove_FAIL;
					break;
				}
			}
		}
	}while(FALSE);
}

void CDataCommService::setActiveMedia(const QString & strActiveMedia, int & iOutRet)
{
	ActiveMedia_TYPE eActiveMediaType = AMedia_TYPE_Unknown;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);

	iOutRet = ERROR_SUCCESS;
	do
	{
		if(strActiveMedia.compare(ActiveMedia_Desc_FLASH, Qt::CaseInsensitive)==0 )
		{
			eActiveMediaType = AMedia_TYPE_FLASH;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_SdCard, Qt::CaseInsensitive)==0)
		{
			eActiveMediaType = AMedia_TYPE_SdCard;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_USB, Qt::CaseInsensitive)==0)
		{
			eActiveMediaType = AMedia_TYPE_USB;
		}
		else if(strActiveMedia.compare(ActiveMedia_Desc_HDD, Qt::CaseInsensitive)==0)
		{
			eActiveMediaType = AMedia_TYPE_HDD;
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iOutRet = pWebBrowserAppIf->setActiveMedia(eActiveMediaType);
	}while(FALSE);
}

void CDataCommService::getActiveMedia(int & iOutRet, QString & strActiveMediaDesc, QString & strActiveMediaDirNanme)
{
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);

	iOutRet = ERROR_SUCCESS;

	do
	{
		iOutRet = pWebBrowserAppIf->getActiveMedia(strActiveMediaDesc, strActiveMediaDirNanme);
	}while(FALSE);
}

void CDataCommService::getMediaFreeSpace(int & iOutRet, qulonglong & MediaFreeSpaceInBytes)
{
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);

	iOutRet = ERROR_SUCCESS;

	do
	{
		iOutRet = pWebBrowserAppIf->getMediaFreeSpace(MediaFreeSpaceInBytes);
	}while(FALSE);
}

void CDataCommService::getTransferControl(int & iOutRet, int & TransferStatusCode)
{
	iOutRet = ERROR_SUCCESS;
	
	do
	{
		TransferStatusCode = m_TransferCtrlStatus;
	}while(FALSE);
}

void CDataCommService::setTransferControl(int TransferCtrlCmdCode, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;

	do
	{
		if(TranCtrlCmd_CancelDownload == TransferCtrlCmdCode)
		{
			iOutRet = CancelAllDownloadItem();
			if(ERROR_SUCCESS == iOutRet)
			{
				m_TransferCtrlStatus = TranCtrlStat_DownloadCanceled;
			}
		}
		else
		{
			iOutRet = ERR_INVALID_CMD;
		}
	}while(FALSE);
}

void CDataCommService::getFileList(const QString & strFileType, int & iOutRet, QStringList & FilenameList)
{
	INT_t iRet;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);
	QString strDstDirPath;
	
	iOutRet = ERROR_SUCCESS;

	//LOG_BLINE("FileType=%s\n", qPrintable(strFileType));
	do
	{
		iRet = pWebBrowserAppIf->getFullFilePath("", strFileType, strDstDirPath);
	//	LOG_BLINE("FileType=%s\n", qPrintable(strDstDirPath));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		QDir oDstDir(strDstDirPath);
		QFileInfoList oFileInfoList = oDstDir.entryInfoList(QDir::Files);
		QFileInfoList::const_iterator itFileInfo = oFileInfoList.begin();
		QString strFilenameFieldContent;
		while(itFileInfo != oFileInfoList.end())
		{
			strFilenameFieldContent.clear();
			QTextStream(&strFilenameFieldContent) << (*itFileInfo).fileName() << "\n" << (*itFileInfo).size();
			FilenameList << strFilenameFieldContent;
			itFileInfo++;
		}
	}while(FALSE);
}

void CDataCommService::customEvent(QEvent * event)
{
	do
	{
		if(CC_UNLIKELY(NULL == event))
		{
			break;
		}
		if(QEVENT_TYPE_DownloadFinished == event->type())
		{
			QFileDownloadFinishedEvent * pDownloadFinishedEvent = dynamic_cast <QFileDownloadFinishedEvent *> (event);
			if(pDownloadFinishedEvent)
			{
				ProcessDownloadFinished(pDownloadFinishedEvent);
			}
		}
		else if(QEVENT_TYPE_UploadFinished == event->type())
		{
			QFileUploadFinishedEvent * pUploadFinishedEvent = dynamic_cast <QFileUploadFinishedEvent *> (event);
			if(pUploadFinishedEvent)
			{
				ProcessUploadFinished(pUploadFinishedEvent);
			}
		}
	}while(FALSE);
}

INT_t CDataCommService::getCurFileTransferredSize(qint64 & uiFileTransferredSize)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;

	do
	{
		QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
		if(m_FileDownloadItemList.isEmpty())
		{
			uiFileTransferredSize = 0;
			break;
		}
		FileDownloadItem_sp = m_FileDownloadItemList.last();
		if(FileDownloadItem_sp.isNull())
		{
			uiFileTransferredSize = 0;
			break;
		}
		uiFileTransferredSize = FileDownloadItem_sp->getFileTransferredSize();
	}while(FALSE);

	return iOutRet;
}

void CDataCommService::On_DownloadFinished(QFileDownloadItem & FileDownloadItem)
{
	quint32 DownloadId;
	QFileDownloadFinishedEvent * pDownloadFinishedEvent = NULL;

	do
	{
		pDownloadFinishedEvent = new QFileDownloadFinishedEvent((enum QEvent::Type)QEVENT_TYPE_DownloadFinished);
		if(NULL == pDownloadFinishedEvent)
		{
			LOG_BLINE("no memory\n");
			break;
		}
		DownloadId = FileDownloadItem.getId();
		pDownloadFinishedEvent->m_DownloadId = DownloadId;
		QCoreApplication::postEvent(this, pDownloadFinishedEvent);
	}while(FALSE);
}

void CDataCommService::On_UploadFinished(QFileUploadItem & FileUploadItem)
{
	quint32 UploadId;
	QFileUploadFinishedEvent * pUploadFinishedEvent = NULL;

	do
	{
		pUploadFinishedEvent = new QFileUploadFinishedEvent((enum QEvent::Type)QEVENT_TYPE_UploadFinished);
		if(NULL == pUploadFinishedEvent)
		{
			LOG_BLINE("no memory\n");
			break;
		}
		UploadId = FileUploadItem.getId();
		pUploadFinishedEvent->m_UploadId = UploadId;
		QCoreApplication::postEvent(this, pUploadFinishedEvent);
	}while(FALSE);
}

INT_t CDataCommService::DownloadFile_Local(const QUrl & SrcUrl, const QString & strFileType,  QString & strDstFileName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QFileInfo> SrcFileInfo_sp;
	QString strCommand;
	QString FullSrcFilePath;
	QString FullDstFilePath;
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);
	QString strFilePath = SrcUrl.path();

	CAutoLock AutoRangeLocker(pWebBrowserAppIf->getPlaylistAccessLocker());

	
	do
	{
		//LOG_BLINE("FilePath:%s\n", qPrintable(strFilePath));
		if("" == strFilePath)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if('/' == strFilePath[0])
		{
			FullSrcFilePath = strFilePath;
		}
		else
		{
			FullSrcFilePath = qApp->applicationDirPath() + "/" + strFilePath;
		}
		LOG_BLINE("FullPath:%s\n", qPrintable(FullSrcFilePath));
		SrcFileInfo_sp = QSharedPointer <QFileInfo>(new QFileInfo(FullSrcFilePath));
		if(SrcFileInfo_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(false == SrcFileInfo_sp->exists())
		{
			iOutRet = ERR_NOT_EXIST;
			break;
		}
		if((128*1024) < SrcFileInfo_sp->size())
		{
			iOutRet = ERR_FILE_SIZE_MISMATCH;
			break;
		}
		iRet = pWebBrowserAppIf->getDstFilePath(SrcUrl, strFileType, strDstFileName, FullDstFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(FullDstFilePath == "")
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
		if(FullSrcFilePath == FullDstFilePath)
		{
			break;
		}
		strCommand = (QString)"cp -fa " + "\"" + FullSrcFilePath + "\"" + " " + "\"" + FullDstFilePath + "\"";
		LOG_BLINE("Cmd:%s\n", qPrintable(strCommand));		
		iRet = system(strCommand.toLocal8Bit().constData());
		if(-1 == iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_FAILED;
			break;
		}
		else if(0 != iRet)
		{
			LOG_BLINE("Cmd(%s) ret %d\n", strCommand.toLocal8Bit().constData(), iRet);
			iOutRet = ERROR_FAILED;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::DownloadFile_HttpFtp(CONST QUrl & FileUrl, const QString & strFileType,  QString & strDstFileName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;
	quint32 DownloadId;
	bool bRet;

	do
	{
		{
			QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
			if(MAX_DownloadFile_ConcurrentCnt <= m_FileDownloadItemList.count())
			{
				iOutRet = ERR_BUSY;
				break;
			}
		}
			
		iRet = AllocDownloadId(OUT DownloadId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		FileDownloadItem_sp = QSharedPointer <QFileDownloadItem> (new QFileDownloadItem(DownloadId)); 
		if(FileDownloadItem_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*FileDownloadItem_sp), SIGNAL(StartTransfer(QFileDownloadItem &)), 
			this, SLOT(On_StartTransfer(QFileDownloadItem &)));
		bRet = connect(&(*FileDownloadItem_sp), SIGNAL(DownloadFinished(QFileDownloadItem &)), 
			this, SLOT(On_DownloadFinished(QFileDownloadItem &)));		
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}

		
		iRet = FileDownloadItem_sp->StartDownload(FileUrl, strFileType, strDstFileName);
		if(ERROR_SUCCESS != iRet)
		{			
			iOutRet = iRet;
			FileDownloadItem_sp.clear();
			break;
		}
		{
			QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
			m_FileDownloadItemList.append(FileDownloadItem_sp);
		}
		sync();
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::DownloadFile_HttpFtp2(CONST QString & strSrcUrl, CONST QString & strDestFile, quint32 & DownloadId)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;
	bool bRet;

	do
	{
		{
			QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
			if(MAX_DownloadFile_ConcurrentCnt <= m_FileDownloadItemList.count())
			{
				iOutRet = ERR_BUSY;
				break;
			}
		}

		iRet = AllocDownloadId(OUT DownloadId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		FileDownloadItem_sp = QSharedPointer <QFileDownloadItem> (new QFileDownloadItem(DownloadId)); 
		if(FileDownloadItem_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*FileDownloadItem_sp), SIGNAL(StartTransfer(QFileDownloadItem &)), 
			this, SLOT(On_StartTransfer(QFileDownloadItem &)));
		bRet = connect(&(*FileDownloadItem_sp), SIGNAL(DownloadFinished(QFileDownloadItem &)), 
			this, SLOT(On_DownloadFinished(QFileDownloadItem &)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}
		
		iRet = FileDownloadItem_sp->StartDownload2(strSrcUrl, strDestFile);
		if(ERROR_SUCCESS != iRet)
		{			
			iOutRet = iRet;
			FileDownloadItem_sp.clear();
			break;
		}
		{
			QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
			m_FileDownloadItemList.append(FileDownloadItem_sp);
		}
		sync();
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::UploadFile_HttpFtp(const QString & strSrcFullFilePath, const QString & strUploadUrl,
	const QParamParser::PROPERTY_VALUE_MAP & PropertyValueMap/* = QParamParser::PROPERTY_VALUE_MAP()*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	quint32 UploadId;
	QSharedPointer <QFileUploadItem> FileUploadItem_sp;
	bool bRet;
	Q_UNUSED(PropertyValueMap);
	
	do
	{
		iRet = AllocUploadId(OUT UploadId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		FileUploadItem_sp = QSharedPointer <QFileUploadItem> (new QFileUploadItem(UploadId)); 
		if(FileUploadItem_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*FileUploadItem_sp), SIGNAL(UploadFinished(QFileUploadItem &)), 
			this, SLOT(On_UploadFinished(QFileUploadItem &)));
		if(false == bRet)
		{
			iOutRet = ERR_QT_SIGNAL_CONN_FAIL;
			break;
		}
#define UploadTransferAction 1
		iRet = FileUploadItem_sp->StartUpload(strSrcFullFilePath, strUploadUrl);
		if(ERROR_SUCCESS != iRet)
		{			
			iOutRet = iRet;
			FileUploadItem_sp.clear();
			break;
		}
		else
		{
			emit StartTransfer_Sig(strSrcFullFilePath,strUploadUrl,UploadTransferAction);
		}
		m_FileUploadItemList.append(FileUploadItem_sp);
	}while(FALSE);
	
	return iOutRet;
}

INT_t CDataCommService::AllocDownloadId(quint32 & DownloadId)
{
	INT_t iOutRet = ERROR_SUCCESS;
	int iId;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;
	bool bFound = false;

	do
	{
		QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);

		m_NextDownloadId++;
		if(0 == m_NextDownloadId)
		{
			m_NextDownloadId++;
		}
		
		while(TRUE)
		{			
			DownloadId = m_NextDownloadId;

			bFound = false;
			for(iId = 0; iId < m_FileDownloadItemList.size(); iId++)
			{
				FileDownloadItem_sp = m_FileDownloadItemList.at(iId);
				if(FALSE == FileDownloadItem_sp.isNull())
				{
					if(DownloadId == FileDownloadItem_sp->getId())
					{
						bFound = true;
						break;
					}
				}
				else
				{
					m_FileDownloadItemList.takeAt(iId);
				}
			}

			if(false == bFound)
			{
				break;
			}

			m_NextDownloadId++;
			while(0 == m_NextDownloadId)
			{
				m_NextDownloadId++;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CDataCommService::AllocUploadId(quint32 & UploadId)
{
	INT_t iOutRet = ERROR_SUCCESS;
	int iId;
	QSharedPointer <QFileUploadItem> FileUploadItem_sp;
	bool bFound = false;

	do
	{
		m_NextUploadId++;
		if(0 == m_NextUploadId)
		{
			m_NextUploadId++;
		}
		
		while(TRUE)
		{			
			UploadId = m_NextUploadId;

			bFound = false;
			for(iId = 0; iId < m_FileUploadItemList.size(); iId++)
			{
				FileUploadItem_sp = m_FileUploadItemList.at(iId);
				if(FALSE == FileUploadItem_sp.isNull())
				{
					if(UploadId == FileUploadItem_sp->getId())
					{
						bFound = true;
						break;
					}
				}
				else
				{
					m_FileUploadItemList.takeAt(iId);
				}
			}

			if(false == bFound)
			{
				break;
			}

			m_NextUploadId++;
			if(0 == m_NextUploadId)
			{
				m_NextUploadId++;
			}
		}
	}while(FALSE);

	return iOutRet;
}

VOID CDataCommService::ProcessDownloadFinished(QFileDownloadFinishedEvent * pDownloadFinishedEvent)
{
	quint32 DownloadId;
	int iId;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;
	bool bNeedRescan = false;
	
	QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
	
	do
	{
		DownloadId = pDownloadFinishedEvent->m_DownloadId;

		while(true)
		{
			bNeedRescan = false;
			for(iId = 0; iId < m_FileDownloadItemList.size(); iId++)
			{
				FileDownloadItem_sp = m_FileDownloadItemList.at(iId);
				if(FALSE == FileDownloadItem_sp.isNull())
				{
					if(DownloadId == FileDownloadItem_sp->getId())	//Found the task
					{
						m_FileDownloadItemList.removeAt(iId);
						INT_t iDownloadErrNo = ERROR_SUCCESS;
						//check err
						QNetworkReply::NetworkError eNetworkErr = FileDownloadItem_sp->getNetworkErr();
						m_TransferCtrlStatus = TranCtrlStat_DownloadIdle;
						if(QNetworkReply::NoError == eNetworkErr)
						{
							if(FileDownloadItem_sp->isFileErr())
							{
								m_TransferCtrlStatus = TranCtrlStat_DownloadErr_FileErr;
								iDownloadErrNo = ERROR_FILE_OPEN_FAIL;
							}
						}
						else if(QNetworkReply::HostNotFoundError == eNetworkErr || QNetworkReply::ProxyNotFoundError == eNetworkErr)
						{
							m_TransferCtrlStatus = TranCtrlStat_DownloadErr_NoServer;
							iDownloadErrNo = ERR_HOST_NOT_FOUND;
						}
						else if(QNetworkReply::AuthenticationRequiredError == eNetworkErr)
						{
							m_TransferCtrlStatus = TranCtrlStat_DownloadErr_LoginErr;
							iDownloadErrNo = ERR_LOGIN_AUTH_FAIL;
						}
						else if(QNetworkReply::ConnectionRefusedError == eNetworkErr ||
							QNetworkReply::RemoteHostClosedError == eNetworkErr ||
							QNetworkReply::SslHandshakeFailedError == eNetworkErr ||
							QNetworkReply::TemporaryNetworkFailureError == eNetworkErr ||
							QNetworkReply::ContentNotFoundError == eNetworkErr ||
							QNetworkReply::UnknownNetworkError == eNetworkErr)
						{							
							if(QNetworkReply::ContentNotFoundError == eNetworkErr)
							{
								iDownloadErrNo = ERR_CONTENT_NOT_FOUND;								
							}
							else if(QNetworkReply::ConnectionRefusedError == eNetworkErr)
							{
								iDownloadErrNo = ERR_CONNECTIN_REFUSED;								
							}
							else if(QNetworkReply::RemoteHostClosedError == eNetworkErr)
							{
								iDownloadErrNo = ERR_SCK_CONN_CLOSED_BY_PEER;								
							}
							else if(QNetworkReply::SslHandshakeFailedError == eNetworkErr)
							{
								iDownloadErrNo = ERR_LOGIN_AUTH_FAIL;								
							}
							else if(QNetworkReply::TemporaryNetworkFailureError == eNetworkErr)
							{
								iDownloadErrNo = ERR_TEMPORARY_NETWORK_FAIL;								
							}
							else
							{
								iDownloadErrNo = ERROR_UNKNOWN;								
							}
							m_TransferCtrlStatus = TranCtrlStat_DownloadErr_NetworkErr;
						}
						else if(QNetworkReply::ContentNotFoundError == eNetworkErr)
						{
							iDownloadErrNo = ERR_CONTENT_NOT_FOUND;								
							m_TransferCtrlStatus = TranCtrlStat_DownloadErr_NetworkErr;							
						}
						else
						{
							iDownloadErrNo = ERROR_UNKNOWN;								
							m_TransferCtrlStatus = TranCtrlStat_DownloadErr_OtherTransferErr;
						}
						//emit an event
						emit DownloadFinished(iDownloadErrNo, DownloadId, FileDownloadItem_sp->getSrcUrl(), FileDownloadItem_sp->getDestUrl());
						//cleanup
						FileDownloadItem_sp.clear();
						bNeedRescan = true;
						break;
					}
				}
				else	//isNull
				{
					m_FileDownloadItemList.removeAt(iId);
					bNeedRescan = true;
					break;
				}
			}
			if(false == bNeedRescan)
			{
				break;
			}
		}
	}while(FALSE);

	if(TranCtrlStat_DownloadIdle == m_TransferCtrlStatus && 0 < m_FileDownloadItemList.size())
	{
		m_TransferCtrlStatus = TranCtrlStat_DownloadInProgress;
	}
}

VOID CDataCommService::ProcessUploadFinished(QFileUploadFinishedEvent * pUploadFinishedEvent)
{
	quint32 UploadId;
	int iId;
	QSharedPointer <QFileUploadItem> FileUploadItem_sp;
	bool bNeedRescan = false;
	
	do
	{
		UploadId = pUploadFinishedEvent->m_UploadId;

		while(true)
		{
			bNeedRescan = false;
			for(iId = 0; iId < m_FileUploadItemList.size(); iId++)
			{
				FileUploadItem_sp = m_FileUploadItemList.at(iId);
				if(FALSE == FileUploadItem_sp.isNull())
				{
					if(UploadId == FileUploadItem_sp->getId())
					{
						//LOG_BLINE("Remove UploadId %u\n", UploadId);
						m_FileUploadItemList.takeAt(iId);
						FileUploadItem_sp.clear();
						bNeedRescan = true;
						break;
					}
				}
				else	//isNull
				{
					m_FileUploadItemList.takeAt(iId);
					bNeedRescan = true;
					break;
				}
			}
			if(false == bNeedRescan)
			{
				break;
			}
		}
	}while(FALSE);
}

INT_t CDataCommService::CancelAllDownloadItem()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QSharedPointer <QFileDownloadItem> FileDownloadItem_sp;
	int iId;
	bool bRet;

	do
	{
		QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);

		if(0 >= m_FileDownloadItemList.size())	//not need to do
		{
			break;
		}

		while(TRUE)
		{
			for(iId = 0; iId < m_FileDownloadItemList.size(); iId++)
			{
				FileDownloadItem_sp = m_FileDownloadItemList[iId];
				m_FileDownloadItemList.removeAt(iId);
				if(FileDownloadItem_sp.isNull())
				{
					break;
				}
				bRet = disconnect(&(*FileDownloadItem_sp), NULL, this, NULL);
				if(false == bRet)
				{
					LOG_BLINE("Disconnect QtSignal err\n");
				}
				iRet = FileDownloadItem_sp->CancelDownload();
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				FileDownloadItem_sp.clear();
			}
			if(0 >= m_FileDownloadItemList.size())	//not need to do
			{
				break;
			}
		}
		if(0 >= m_FileDownloadItemList.size())	//not need to do
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

void CDataCommService::setStorePlaylistFile(QString strPlaylistUrl, int & iOutRet)
{
	QUrl FileUrl(strPlaylistUrl);
	QString strUrlScheme,strDstFileName;
	iOutRet = ERROR_SUCCESS;	
	
	do
	{
		QStringList strUrlFiledList = strPlaylistUrl.split("/");
		int iTempFiledSize = strUrlFiledList.size();
		if(iTempFiledSize < 0)
		{
			iOutRet =  ERROR_INVALID_PARAMETER;
			break;
		}
		strUrlFiledList = strUrlFiledList[iTempFiledSize -1].split("?") ;
		strDstFileName = strUrlFiledList[0];
		if(strDstFileName.trimmed() == "")
		{
		 	iOutRet =  ERROR_INVALID_PARAMETER;
			break;
		}
	
		strUrlScheme = FileUrl.scheme();
		//LOG_BLINE("Scheme:%s\n", qPrintable(strUrlScheme));
		if("file" == strUrlScheme || "" == strUrlScheme)	//local file
		{
			iOutRet = DownloadFile_Local(FileUrl, FileType_Desc_Playlist, strDstFileName);
		}
		else if("http" == strUrlScheme || "ftp" == strUrlScheme)	//http or ftp file
		{
			iOutRet = DownloadFile_HttpFtp(FileUrl, FileType_Desc_Playlist, strDstFileName);
		}
		else	//other not supported
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}
	}while(FALSE);

	if(ERROR_SUCCESS == iOutRet)
	{
		m_TransferCtrlStatus = TranCtrlStat_DownloadInProgress;
	}

	if(Sw_LogDbusCmd)
	{
		LOG_BLINE("setStorePlaylistFile ret %d\n", iOutRet);
	}
}

void CDataCommService::On_StartTransfer(QFileDownloadItem & FileDownloadItem)
{
	if(FileDownloadItem.getId() > 0 &&  FileDownloadItem.getSrcUrl() !="")
	{
		
	}
#define DownlaodAction 2
	do
	{
		emit StartTransfer_Sig(FileDownloadItem.getSrcUrl(),FileDownloadItem.getDestUrl(),DownlaodAction);
	}while(FALSE);
}

void CDataCommService::setTransferControlStatus(int TransferCtrlCmdCode, int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	do
	{
		QAutoLock AutoRangeLocker(&m_FileDownloadDataMutex);
		switch(TransferCtrlCmdCode)
		{
			case TranCtrlStat_FirmwareDone:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareDone;
				break;
			case TranCtrlStat_FirmwareUploading:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareUploading;
				break;
			case TranCtrlStat_FirmwareBurning1:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareBurning1;
				break;
			case TranCtrlStat_FirmwareBurning2:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareBurning2;
				break;
			case TranCtrlStat_FirmwareBurning3:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareBurning3;
				break;
			case TranCtrlStat_FirmwareFailed:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareFailed;
				break;
			case TranCtrlStat_FirmwareBadAddress:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareBadAddress;
				break;
			case TranCtrlStat_FirmwareConnectionError:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareConnectionError;
				break;
			case TranCtrlStat_FirmwareBadFile:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareBadFile;
				break;
			case TranCtrlStat_FirmwareTooLarge:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareTooLarge;
				break;
			case TranCtrlStat_FirmwarePowerError:
				m_TransferCtrlStatus = TranCtrlStat_FirmwarePowerError;
				break;
			case TranCtrlStat_FirmwareOtherError:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareOtherError;
				break;
			case TranCtrlStat_DownloadInProgress:
				m_TransferCtrlStatus = TranCtrlStat_FirmwareOtherError;
				break;
			case TranCtrlStat_DownloadCanceled:
				m_TransferCtrlStatus =TranCtrlStat_DownloadCanceled;
				break;
			default:
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
		}
	}while(FALSE);
}

void CDataCommService::setDispOsdResolutionId(int iResolutionId,int & iOutRet)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;	 
	do
	{		
		if(iResolutionId < DISP_OSD_RES_DEFAULT || iResolutionId >= DISP_OSD_RES_MAX)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = SysProp_setPersistent(SysProp_KEY_CurOsdResolutionId,iResolutionId);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet =	iRet;
			PRINT_BFILE_LINENO_IRET_STR;	
		}		
	}while(FALSE);
}

void CDataCommService::getDispOsdResolutionId(int & iOutRet, int & iCurResolutionId)
{
	iOutRet = ERROR_SUCCESS;
	INT_t iRet;
	iRet = ERROR_SUCCESS;	 
	do
	{		
		DECLARE_CLS_STACK_BUF_STRING(strAutoAnnounceIntervalSec, 32);
		iRet = SysProp_get(SysProp_KEY_CurOsdResolutionId, OUT strAutoAnnounceIntervalSec);
		if(ERROR_SUCCESS == iRet)
		{
			iCurResolutionId = strAutoAnnounceIntervalSec;
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iCurResolutionId = DISP_OSD_RES_DEFAULT;	//default
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
			iCurResolutionId = DISP_OSD_RES_DEFAULT; //default
			iOutRet = iRet;
		}
	}while(FALSE);
}
