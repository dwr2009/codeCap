#include "QParamParser.h"
#include <BaseErrDef.h>
#include <QStringList>
#include <QDebug>

QParamParser::QParamParser()
{
}

QParamParser::~QParamParser()
{
}

INT_t QParamParser::ParseText(const QString & strText, PROPERTY_VALUE_MAP & PropertyValueMap)
{
	INT_t iOutRet = ERROR_SUCCESS;
	QStringList LineList;
	QStringList::iterator itString;

	do
	{
		PropertyValueMap.clear();
		LineList = strText.split("\n", QString::SkipEmptyParts);		
		for(itString = LineList.begin(); itString != LineList.end(); itString++)
		{
			PROPERTY_VALUE_MAP::iterator itPairInserted;
			//qDebug() << "Line:" << (*itString);
			int iCharIndex = (*itString).indexOf("=");
			if(0 < iCharIndex)
			{
				//qDebug() << "Key:" << (*itString).left(iCharIndex) << "Val:" << (*itString).mid(iCharIndex+1);
				itPairInserted = PropertyValueMap.insert((*itString).left(iCharIndex), (*itString).mid(iCharIndex+1));
				if(itPairInserted == PropertyValueMap.end())
				{
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

