#include "QNetworkAddrConverter.h"
#include <NetIF.h>
#include <BaseErrDef.h>

QNetworkAddrConverter::QNetworkAddrConverter()
{
}

QNetworkAddrConverter::~QNetworkAddrConverter()
{
}

QString QNetworkAddrConverter::getIpAddrFromBin(quint32 IpV4Addr)
{
	INT iRet;
	QString strIpV4Addr;
	char szBuf[256];
	
	iRet = convertIPv4Addr_BinToStr(IpV4Addr, szBuf, sizeof(szBuf));
	if(ERROR_SUCCESS == iRet)
	{
		strIpV4Addr = szBuf;
	}

	return strIpV4Addr;
}

