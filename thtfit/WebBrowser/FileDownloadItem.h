#ifndef	_FILE_DOWNLOAD_ITEM_H_
#define	_FILE_DOWNLOAD_ITEM_H_

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <BaseTypeDef.h>
#include <QNetworkReply>
#include <QFile>
#include <QEvent>

class QFileDownloadFinishedEvent : public QEvent
{
public:
	explicit QFileDownloadFinishedEvent(enum QEvent::Type);
public:
	quint32 m_DownloadId;
};

class QFileDownloadItem : public QObject
{
	Q_OBJECT
public:
	explicit QFileDownloadItem(quint32 Id);
	virtual ~QFileDownloadItem();
	virtual INT_t StartDownload(CONST QUrl & Url, const QString & strDstFileName,  QString & strFileType);
	virtual INT_t StartDownload2(CONST QString & strSrcUrl, const QString & strDstFileName);
	virtual INT_t CancelDownload();
	virtual quint32 getId();
	virtual QNetworkReply::NetworkError getNetworkErr();
	virtual BOOL_t isFileErr();
	virtual const QString & getSrcUrl();
	virtual const QString & getDestUrl();
	virtual quint64 getFileTransferredSize();
signals:
	void DownloadFinished(QFileDownloadItem & FileDownloadItem);	
	void StartTransfer(QFileDownloadItem & FileDownloadItem);
public slots:
	void Reply_ReadyRead();
	void Reply_error(QNetworkReply::NetworkError NetworkErrCode);
	void replyFinished(QNetworkReply * pNetworkReply);	
private:
protected:
	QSharedPointer <QNetworkAccessManager> m_NetAccessMgr_sp;
private:
	quint32 m_Id;
	QUrl m_OrigUrl;
	QString m_strSrcUrl;
	QString m_strDstFilePath;
	QSharedPointer <QFile> m_DstFile_sp;
	QNetworkReply * m_pCurNetworkReply;
	QNetworkReply::NetworkError m_eNetworkErr;
	BOOL_t m_bFileErr;
	quint64 m_uiFileTransferredSize;
};

typedef QList < QSharedPointer <QFileDownloadItem> > FILE_DOWNLOAD_ITEM_LIST;

#endif	//_FILE_DOWNLOAD_ITEM_H_

