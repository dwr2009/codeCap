#ifndef	_QRESOLVE_CONF_H_
#define	_QRESOLVE_CONF_H_

#include <QObject>
#include <BaseTypeDef.h>
#include <QStringList>

class QResolveConf : public QObject
{	
public:
	QResolveConf();
	virtual ~QResolveConf();
	INT_t Reset();
	virtual INT_t ReloadConf();
	virtual INT_t clearNameServer();
	virtual INT_t setDomainName(const QString & strDomainName);
	virtual INT_t setNameServer(QStringList & listNameServer);
	virtual INT_t SaveConf();
private:
public:
	static const char * RESOLVE_CONF_FILE;
private:
	QString m_strDomain;
	QString m_strSearch;
	QStringList m_listNameServer;
};

#endif	//_QRESOLVE_CONF_H_

