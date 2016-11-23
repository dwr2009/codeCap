#include "QTarPackageReader.h"
#include <QProcess>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

QTarPackageReader::QTarPackageReader()
{
	m_bFileIsOpened = FALSE;
}

QTarPackageReader::~QTarPackageReader()
{
	INT_t iRet;

	iRet = CloseTarFile();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

INT_t QTarPackageReader::OpenTarFile(const QString & strTarFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		if(strTarFilePath.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(m_bFileIsOpened)
		{
			iOutRet = ERR_ALREADY_OPENED;
			break;
		}
		/*
		QProcess CmdProcess;
		QStringList CmdLineParams;
		CmdProcess.setProcessChannelMode(QProcess::ForwardedChannels);
		CmdLineParams << "t";
		CmdLineParams << "-f";
		CmdLineParams << strTarFilePath;
		CmdProcess.start("tar", CmdLineParams, QIODevice::NotOpen);
		*/
	}while(FALSE);

	return iOutRet;
}

INT_t QTarPackageReader::CloseTarFile()
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_bFileIsOpened = FALSE;
	}while(FALSE);
	
	return iOutRet;
}

