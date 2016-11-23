#ifndef	_DATA_COMM_SERVICE_H_
#define	_DATA_COMM_SERVICE_H_

#include <QObject>
#include <QSharedPointer>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>
#include <BaseTypeDef.h>
#include <QSharedPointer>
#include "FileDownloadItem.h"
#include "FileUploadItem.h"
#include <QParamParser.h>
#include <QAutoLock.h>
#include "SysBaseSrvApi.h"
#include <DevConnectionStatus.h>

class CDataCommService;

class QDBusIf_DataCommSrv : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.WebBrowser.DataCommSrv")
public:
	QDBusIf_DataCommSrv(CDataCommService * pParentObj);
	void setParentObj(CDataCommService * pParentObj);
public slots:
	void DownloadFile(QString strFileUrl, QString strFileType, QString strDstFileName, int & iOutRet);
	void DownloadFile2(QString strSrcUrl, QString strDestFile, int & iOutRet, quint32 & DownloadId);
	void UploadFile(QString strFilename, QString strFileType, QString strUploadUrl, int & iOutRet);
	void UploadFile2(QString strSrcFile, QString strUploadUrl, QString strOptions, int & iOutRet);
	void renameMediaFile(QString strOldFilename, QString strNewFilename, QString strFileType, int & iOutRet);
	void deleteMediaFile(QString strFilename, QString strFileType, int & iOutRet);
	void setActiveMedia(QString strActiveMedia, int & iOutRet);
	void getActiveMedia(int & iOutRet, QString & strActiveMediaDesc, QString & strActiveMediaDirNanme);
	void getMediaFreeSpace(int & iOutRet, qulonglong & MediaFreeSpaceInBytes);
	void getTransferControl(int & iOutRet, int & TransferStatusCode);
	void setTransferControl(int TransferCtrlCmdCode, int & iOutRet);
	void getFileList(QString strFileType, int & iOutRet, QStringList & FilenameList);	
	void setStorePlaylistFile(QString strPlaylistUrl,int & iOutRet);	
	void setDispOsdResolutionId(int iResolutionId,int & iOutRet);
	void getDispOsdResolutionId(int & iOutRet, int & iCurResolutionId);
signals:
	void DownloadFinished(int iOutRet, quint32 DonwloadId, const QString & strSrcUrl, const QString & strDestUrl);
	void StartTransfer_Sig(QString strSrcUrl,QString strDestUrl,int iAction);
private:
	CDataCommService * m_pDataCommSrv;
};

class CDataCommService : public QObject
{
	Q_OBJECT
	friend class QDBusIf_DataCommSrv;
public:
	CDataCommService();
	virtual ~CDataCommService();
	virtual INT_t setQDBusConn(QSharedPointer <QDBusConnection> & QDBusConn_sp);
	virtual INT_t InitInstance();
	virtual INT_t ExitInstance();
	void DownloadFile(const QString & strFileUrl, const QString & strFileType,  QString & strDstFileName, int & iOutRet);
	void DownloadFile2(const QString & strSrcUrl, const QString & strDestFile, int & iOutRet, quint32 & DownloadId);
	void UploadFile(const QString & strFilename, const QString & strFileType, const QString & strUploadUrl, int & iOutRet);
	void UploadFile2(const QString & strSrcFile, const QString & strUploadUrl, const QString & strOptions, int & iOutRet);
	void renameMediaFile(const QString & strOldFilename, const QString & strNewFilename, const QString & strFileType, int & iOutRet);
	void deleteMediaFile(const QString & strFilename, const QString & strFileType, int & iOutRet);
	void setActiveMedia(const QString & strActiveMedia, int & iOutRet);
	void getActiveMedia(int & iOutRet, QString & strActiveMediaDesc, QString & strActiveMediaDirNanme);
	void getMediaFreeSpace(int & iOutRet, qulonglong & MediaFreeSpaceInBytes);
	void getTransferControl(int & iOutRet, int & TransferStatusCode);
	void setTransferControl(int TransferCtrlCmdCode, int & iOutRet);
	void getFileList(const QString & strFileType, int & iOutRet, QStringList & FilenameList);
	virtual void customEvent(QEvent * event);
	virtual INT_t getCurFileTransferredSize(qint64 & uiFileTransferredSize);
	void setStorePlaylistFile(QString strPlaylistUrl,int & iOutRet);
	void setTransferControlStatus(int TransferCtrlCmdCode, int & iOutRet);	
	void setDispOsdResolutionId(int eCurrActiveMedia,int & iOutRet);
	void getDispOsdResolutionId(int & iOutRet, int & iCurResolutionId);
public slots:
	void On_DownloadFinished(QFileDownloadItem & FileDownloadItem);
	void On_UploadFinished(QFileUploadItem & FileUploadItem);
	void On_StartTransfer(QFileDownloadItem & FileDownloadItem);
signals:
	void DownloadFinished(int iOutRet, quint32 DonwloadId, const QString & strSrcUrl, const QString & strDestUrl);
	void StartTransfer_Sig(QString strSrcUrl,QString strDestUrl,int iAction);
private:
	INT_t DownloadFile_Local(const QUrl & SrcUrl, const QString & strFileType,  QString & strDstFileName);
	INT_t DownloadFile_HttpFtp(CONST QUrl & FileUrl, const QString & strFileType,  QString & strDstFileName);
	INT_t DownloadFile_HttpFtp2(CONST QString & strSrcUrl, CONST QString & strDestFile, quint32 & DownloadId);
	INT_t UploadFile_HttpFtp(const QString & strSrcFullFilePath, const QString & strUploadUrl, 
		const QParamParser::PROPERTY_VALUE_MAP & PropertyValueMap = QParamParser::PROPERTY_VALUE_MAP());
	INT_t AllocDownloadId(quint32 & DownloadId);
	INT_t AllocUploadId(quint32 & UploadId);
	VOID ProcessDownloadFinished(QFileDownloadFinishedEvent * pDownloadFinishedEvent);
	VOID ProcessUploadFinished(QFileUploadFinishedEvent * pUploadFinishedEvent);
	INT_t CancelAllDownloadItem();
private:
	quint32 m_NextDownloadId, m_NextUploadId;
	QWeakPointer <QDBusConnection> m_QDBusConn_wp;
	QSharedPointer <QDBusIf_DataCommSrv> m_DBusIfAdaptor_DataCommSrv_sp;
	FILE_DOWNLOAD_ITEM_LIST m_FileDownloadItemList;	
	FILE_UPLOAD_ITEM_LIST m_FileUploadItemList;
	int m_TransferCtrlStatus;
	QMutex m_FileDownloadDataMutex;
};

#endif	//_DATA_COMM_SERVICE_H_

