#ifndef	_QPARAM_PARSER_H_
#define	_QPARAM_PARSER_H_

#include <QMap>
#include <QString>
#include <BaseTypeDef.h>

class QParamParser
{
public:
	typedef QMap <QString, QString> PROPERTY_VALUE_MAP;
public:
	QParamParser();
	virtual ~QParamParser();
	virtual INT_t ParseText(const QString & strText, PROPERTY_VALUE_MAP & PropertyValueMap);
};

#endif	//_QPARAM_PARSER_H_

