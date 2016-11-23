#include "QXmlDocument.h"
#include <BaseErrDef.h>
#include <QDir>
#include <QTextStream>

QXmlDocument::QXmlDocument()
{

}

QXmlDocument::~QXmlDocument()
{

}

INT_t QXmlDocument::OpenXmlFile(CONST QString & strFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS;
	SharedPtr <QFile> InputXmlFile_sp;
	bool bRet;
	QString errorMsg;
	int errorLine;
	int errorColumn;

	do
	{
		//Save the input xml file path
		if(strFilePath.left(1) == "/")	//absolutely path
		{
			m_strXmlFilePath = strFilePath;
		}
		else	//relative path
		{
			m_strXmlFilePath = QDir::currentPath() + strFilePath;
		}
		InputXmlFile_sp = SharedPtr <QFile> (new QFile(m_strXmlFilePath));
		if(NULL == InputXmlFile_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = setContent(&(*InputXmlFile_sp), &errorMsg, &errorLine, &errorColumn);
		if(false == bRet)
		{
			iOutRet = ERR_PARSE_XML_FAIL;
			LOG("XmlFileErr(%s):Line:%d,Column:%d,%s\n", qPrintable(m_strXmlFilePath), errorLine, errorColumn, qPrintable(errorMsg));
			break;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t QXmlDocument::OpenXmlFile(QString & strFilePath)
{
	return OpenXmlFile((CONST QString &)strFilePath);
}

INT_t QXmlDocument::SaveXmlFile(CONST QString * pstrOutputFilePath/* = NULL*/)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QString strOutFilePath = m_strXmlFilePath;
	SharedPtr <QFile> OutXmFile_sp;
	QTextStream OutTextStream;
	bool bRet;

	do
	{
		if(pstrOutputFilePath)
		{
			strOutFilePath = *pstrOutputFilePath;
		}
		OutXmFile_sp = SharedPtr <QFile> (new QFile(strOutFilePath));
		if(NULL == OutXmFile_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = OutXmFile_sp->open(QIODevice::WriteOnly | QIODevice::Text);
		if(FALSE == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		OutTextStream.setDevice(&(*OutXmFile_sp));
		save(OutTextStream, 4);
	}while(FALSE);

	return iOutRet;
}

INT_t QXmlDocument::SaveXmlFile(CONST QString & strOutputFilePath)
{
	return SaveXmlFile(&strOutputFilePath);
}

