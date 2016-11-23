#ifndef	_WEB_BROWSER_APP_IF_H_
#define	_WEB_BROWSER_APP_IF_H_

#include <Mutex2.h>
#include <BaseTypeDef.h>
#include "Api/WebBrowserAppDef.h"
#include <Mp7xxGlobalStatus.h>

#define	APP_DIR_PATH			((qApp->applicationDirPath() + (qApp->applicationDirPath() == "/"?"":"/")))

class CWebBrowserAppIf
{
public:
	virtual CMutex2 * getPlaylistAccessLocker() = 0;
	virtual INT_t getDstFilePath(const QUrl & SrcFileUrl, const QString & strFileType, 
		 QString & strDstFileName, QString & strDstFilePath) = 0;
	virtual INT_t getFullFilePath(const QString & strFilename, const QString & strFileType, QString & strFullFilePath) = 0;
	virtual INT_t setActiveMedia(CONST ActiveMedia_TYPE eActiveMediaType) = 0;
	virtual INT_t getActiveMedia(QString & strActiveMedia, QString & strActiveMediaDirName) = 0;
	virtual INT_t getMediaFreeSpace(qulonglong & MediaFreeSpaceInBytes) = 0;
	virtual void DbgLog(const QString & strDbgInfo) = 0;
	virtual QString getActiveMediaDescFromType(ActiveMedia_TYPE eActiveMediaType) = 0;
};

#endif	//_WEB_BROWSER_APP_IF_H_

