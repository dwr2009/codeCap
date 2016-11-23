#ifndef	_Q_NET_DHCP_H_
#define	_Q_NET_DHCP_H_

#include <BaseTypeDef.h>
#include <QString>

class QNetDhcp
{
public:
	static INT_t ChkDhcpcProcess(IN CONST QString & strNetIfName, OUT BOOL_t & bIsRunning, OUT pid_t & pidDhcpc);
};

#endif	//_Q_NET_DHCP_H_

