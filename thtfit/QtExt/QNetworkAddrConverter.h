#ifndef	_QNETWORK_ADDR_CONVERTER_H_
#define	_QNETWORK_ADDR_CONVERTER_H_

#include <QString>

class QNetworkAddrConverter
{
public:
	QNetworkAddrConverter();
	virtual ~QNetworkAddrConverter();
	static QString getIpAddrFromBin(quint32 IpV4Addr);
};

#endif	//_QNETWORK_ADDR_CONVERTER_H_

