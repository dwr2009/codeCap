#if	!defined(_NetDevListReader_h_)
#define	_NetDevListReader_h_

#include <QString>
#include <BaseTypeDef.h>
#include <QList>
#include <QSharedPointer>

class QNetDevListReader
{
public:
	class QNetDevInfo
	{
	public:
		QString strDevName;
	};
public:
	INT_t Scan();
	BOOL_t HasDev(const QString & sDevName);
private:
	QList < QSharedPointer<QNetDevInfo> > m_lstNetDev;
};

#endif	//_NetDevListReader_h_
