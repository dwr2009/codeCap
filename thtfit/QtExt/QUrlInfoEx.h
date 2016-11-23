#ifndef	_QURL_INFO_EX_H_
#define	_QURL_INFO_EX_H_

#include <QUrl>
#include <BaseTypeDef.h>

class QUrlInfoEx : public QUrl
{
public:
	QUrlInfoEx();
	virtual ~QUrlInfoEx();
	static QString RemoveExtensionName(CONST QString & strUrl);
	static QString getRealPath(const QString & strSrcPath);
};

#endif	//_QURL_INFO_EX_H_
