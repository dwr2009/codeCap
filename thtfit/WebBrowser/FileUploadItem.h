#ifndef	_FILE_UPLOAD_ITEM_H_
#define	_FILE_UPLOAD_ITEM_H_

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include <BaseTypeDef.h>
#include <QNetworkReply>
#include <QFile>
#include <QEvent>

class QFileUploadFinishedEvent : public QEvent
{
public:
	explicit QFileUploadFinishedEvent(enum QEvent::Type);
public:
	quint32 m_UploadId;
};

class QFileUploadItem : public QObject
{
	Q_OBJECT
public:
	explicit QFileUploadItem(quint32 Id);
	virtual ~QFileUploadItem();
	virtual INT_t StartUpload(CONST QString & strSrcFullFilePath, CONST QString & strUploadUrl);
	virtual quint32 getId();
signals:
	void UploadFinished(QFileUploadItem & FileUploadItem);	
public slots:
	void Reply_error(QNetworkReply::NetworkError NetworkErrCode);
	void replyFinished(QNetworkReply * pNetworkReply);	
protected:
	QSharedPointer <QNetworkAccessManager> m_NetAccessMgr_sp;
private:
	quint32 m_Id;
	QSharedPointer <QFile> m_SrcFile_sp;	
};

typedef QList < QSharedPointer <QFileUploadItem> > FILE_UPLOAD_ITEM_LIST;

#endif	//_FILE_UPLOAD_ITEM_H_

