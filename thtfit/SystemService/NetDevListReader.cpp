#include "NetDevListReader.h"
#include <BaseErrDef.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSharedPointer>
#include <ErrPrintHelper.h>
#include <QPointer>

INT_t QNetDevListReader::Scan()
{
	INT_t iOutRet = ERR_SUCCESS;
	bool bRet;
	QFile fileNetDevInfo("/proc/net/dev");
	QSharedPointer <QTextStream> NetDevInfoTs_sp;
	QString strOneLine;
	int iCnt = 0;

	m_lstNetDev.clear();
	
	bRet = fileNetDevInfo.open(QIODevice::ReadOnly);
	if(false == bRet)
	{
		iOutRet = ERR_FILE_OPEN_FAIL;
		goto EXIT_PROC;
	}
	NetDevInfoTs_sp = QSharedPointer <QTextStream>(new QTextStream(&fileNetDevInfo));
	if(NetDevInfoTs_sp.isNull())
	{
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	
	//ignore the first two lines
	iCnt = 2;
	while(iCnt--)
	{
		strOneLine = NetDevInfoTs_sp->readLine(1024/*maxlen*/);
		if(strOneLine.isNull())
		{
			goto EXIT_PROC;
		}
		if(NetDevInfoTs_sp->atEnd())
		{
			goto EXIT_PROC;
		}
	}
	//read significant lines
	while(TRUE)
	{
		strOneLine = NetDevInfoTs_sp->readLine(1024/*maxlen*/);
		if(strOneLine.isNull())
		{
			break;
		}
		//LOG_BLINE("Line=%s\n", qPrintable(strOneLine));		
		//fetch one item
		QStringList lstFields = strOneLine.split(" ", QString::SkipEmptyParts);
		if(0 < lstFields.size())
		{
			QSharedPointer <QNetDevInfo> spNetDevInfo(new QNetDevInfo);
			QString sNetDevName;
			if(spNetDevInfo.isNull())
			{
				iOutRet = ERR_OUT_OF_MEMORY;
				goto EXIT_PROC;
			}
			sNetDevName = lstFields[0];
			if(0 == sNetDevName.right(1).compare(":"))
			{
				sNetDevName = sNetDevName.left(sNetDevName.size()-1);
			}
			//LOG_BLINE("NetDevName=%s\n", qPrintable(sNetDevName));
			spNetDevInfo->strDevName = sNetDevName;
			m_lstNetDev.append(spNetDevInfo);
		}
		//
		if(NetDevInfoTs_sp->atEnd())
		{
			break;
		}
	}

EXIT_PROC:

	NetDevInfoTs_sp.clear();
 	
	return iOutRet;
}

BOOL_t QNetDevListReader::HasDev(const QString & sDevName)
{
	BOOL_t bOutRet = FALSE;
	QSharedPointer <QNetDevInfo> NetDevInfo_sp;
	QList < QSharedPointer<QNetDevInfo> >::const_iterator NetDevInfo_it;
	
	for(NetDevInfo_it = m_lstNetDev.constBegin(); NetDevInfo_it != m_lstNetDev.constEnd(); NetDevInfo_it++)
	{
		NetDevInfo_sp = *NetDevInfo_it;
		if(FALSE == NetDevInfo_sp.isNull())
		{
			if(0 == NetDevInfo_sp->strDevName.compare(sDevName))
			{
				bOutRet = TRUE;
				break;
			}
		}
	}

EXIT_PROC:
	return bOutRet;
}

