#ifndef	_QXML_DOCUMENT_H_
#define	_QXML_DOCUMENT_H_

#include <QtXml/QDomDocument>
#include <BaseTypeDef.h>
#include <SharedPtr.h>

using namespace CppBase;

class QXmlDocument : public QDomDocument
{
public:
	QXmlDocument();
	virtual ~QXmlDocument();
	INT_t OpenXmlFile(CONST QString & strFilePath);
	INT_t OpenXmlFile(QString & strFilePath);
	INT_t SaveXmlFile(CONST QString * pstrOutputFilePath = NULL);
	INT_t SaveXmlFile(CONST QString & strOutputFilePath);
private:
	QString m_strXmlFilePath;
};

#endif	//_QXML_DOCUMENT_H_

