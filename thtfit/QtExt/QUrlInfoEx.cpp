#include "QUrlInfoEx.h"
#include <stdlib.h>

QUrlInfoEx::QUrlInfoEx()
{
}

QUrlInfoEx::~QUrlInfoEx()
{
}

QString QUrlInfoEx::RemoveExtensionName(CONST QString & strUrl)
{
	QString strOutRet;
	int LastDotIndex, LastSlashIndex;

	do
	{
		LastSlashIndex = strUrl.indexOf('/');
		LastDotIndex = strUrl.indexOf('.');
		if(0 <= LastSlashIndex && LastSlashIndex > LastDotIndex)	//the case "a.b/c"
		{
			break;
		}
		if(0 > LastDotIndex)	//no "."
		{
			break;
		}
		strOutRet = strUrl.left(LastDotIndex);
	}while(FALSE);

	return strOutRet;
}

QString QUrlInfoEx::getRealPath(const QString & strSrcPath)
{
	char szDestPath[PATH_MAX+1];
	char * pszRet;
	pszRet = realpath(strSrcPath.toUtf8().constData(), szDestPath);
	if(pszRet)
	{
		szDestPath[PATH_MAX] = '\0';
		return pszRet;
	}
	else
	{
		return "";
	}
}

