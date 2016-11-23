#include "MediaVirtualChannel.h"
#include <BaseErrDef.h>
#include <QXmlDocument.h>
#include <QFileInfo>
#include <QSharedPointer>

QMediaVirtualChannel::QMediaVirtualChannel()
{
	m_iVirChMin = -1;
	m_iVirChMax = -1;
}

QMediaVirtualChannel::~QMediaVirtualChannel()
{
}

INT_t QMediaVirtualChannel::LoadCfgFromXmlFile(CONST QString & strCfgXmlFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QXmlDocument XmlDoc;
	int iIndex;

	do
	{		
		QFileInfo XmlFileInfo(strCfgXmlFilePath);
		if((1*1024*1024) < XmlFileInfo.size())
		{
			iOutRet = ERR_FILE_SIZE_TOO_LARGE;
			break;
		}
		
		iRet = XmlDoc.OpenXmlFile(strCfgXmlFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		m_VirChInfo_Map.clear();

		QDomElement Element_VirCh = XmlDoc.documentElement();
		if(Element_VirCh.isNull())
		{
			LOG("%s,no root element\n", qPrintable(strCfgXmlFilePath));
			break;
		}
		QDomElement Element_ChList = Element_VirCh.firstChildElement("ChannelList");
		if(Element_ChList.isNull())
		{
			LOG("%s,no root.ChannelList element\n", qPrintable(strCfgXmlFilePath));
			break;
		}
		m_iVirChMin = Element_ChList.attribute(MVC_PropDescName_MinChId, "-1").toInt();
		m_iVirChMax = Element_ChList.attribute(MVC_PropDescName_MaxChId, "-1").toInt();
		QDomNodeList Element_Ch_List = Element_ChList.elementsByTagName("Channel");
		for(iIndex = 0; iIndex < Element_Ch_List.size(); iIndex++)
		{
			QDomNode ChannelNode = Element_Ch_List.at(iIndex);
			if(ChannelNode.isElement())
			{
				QDomElement ChannelEle = ChannelNode.toElement();
				if(FALSE == ChannelEle.isNull() && ChannelEle.hasAttribute("Id") && ChannelEle.hasAttribute("Url"))
				{
					int iId;
					QString strUrl;
					iId = ChannelEle.attribute("Id").toInt();
					strUrl = ChannelEle.attribute("Url");
					//LOG_BLINE("Ch:%d,%s\n", iId, qPrintable(strUrl));
					QSharedPointer <QVirChInfo> VirChInfo_sp(new QVirChInfo);
					if(VirChInfo_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					VirChInfo_sp->m_strUrl = strUrl;
					VirChInfo_sp->m_strAlias = ChannelEle.attribute(MVC_PropDescName_ALIAS);
					VirChInfo_sp->m_eLabelDispPos = ChannelEle.attribute(MVC_PropDescName_LabelDispPos).toInt();
					VirChInfo_sp->m_bEnable = ChannelEle.attribute(MVC_PropDescName_Enable, "1").toInt();
					VirChInfo_sp->m_DispDuration = ChannelEle.attribute(MVC_PropDescName_DispDuration, "-1").toInt();
					m_VirChInfo_Map[iId] = VirChInfo_sp;
				}				
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::SaveCfgToXmlFile(CONST QString & strCfgXmlFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QXmlDocument XmlDoc;
	VirChInfo_MAP::iterator itVirChInfo;
	QSharedPointer <QVirChInfo> VirChInfo_sp;
	QDomNode RetDomNode;

	do
	{
		QDomProcessingInstruction ProcessInstructionNode = XmlDoc.createProcessingInstruction("xml", 
			"version=\"1.0\" encoding=\"utf-8\"");
		RetDomNode = XmlDoc.appendChild(ProcessInstructionNode);
		if(RetDomNode.isNull())
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			break;
		}
		QDomElement RootEle = XmlDoc.createElement("VirtualChannel");
		if(RootEle.isNull())
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			break;
		}
		RetDomNode = XmlDoc.appendChild(RootEle);
		if(RetDomNode.isNull())
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			break;
		}
		QDomElement ChListEle = XmlDoc.createElement("ChannelList");
		if(ChListEle.isNull())
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			break;
		}
		ChListEle.setAttribute(MVC_PropDescName_MinChId, m_iVirChMin);
		ChListEle.setAttribute(MVC_PropDescName_MaxChId, m_iVirChMax);
		QDomNode RetDomNode = RootEle.appendChild(ChListEle);
		if(RetDomNode.isNull())
		{
			iOutRet = ERR_CREATE_OBJ_FAIL;
			break;
		}
		for(itVirChInfo = m_VirChInfo_Map.begin(); itVirChInfo != m_VirChInfo_Map.end(); itVirChInfo++)
		{
			VirChInfo_sp = itVirChInfo.value();
			if(FALSE == VirChInfo_sp.isNull())
			{
				QDomElement VirChInfoEle = XmlDoc.createElement("Channel");
				if(ChListEle.isNull())
				{
					iOutRet = ERR_CREATE_OBJ_FAIL;
					break;
				}
				VirChInfoEle.setAttribute("Id", itVirChInfo.key());
				VirChInfoEle.setAttribute("Url", VirChInfo_sp->m_strUrl);
				VirChInfoEle.setAttribute(MVC_PropDescName_ALIAS, VirChInfo_sp->m_strAlias);
				VirChInfoEle.setAttribute(MVC_PropDescName_LabelDispPos, VirChInfo_sp->m_eLabelDispPos);
				VirChInfoEle.setAttribute(MVC_PropDescName_Enable, VirChInfo_sp->m_bEnable);
				VirChInfoEle.setAttribute(MVC_PropDescName_DispDuration, VirChInfo_sp->m_DispDuration);
				QDomNode RetDomNode = ChListEle.appendChild(VirChInfoEle);
				if(RetDomNode.isNull())
				{
					iOutRet = ERR_CREATE_OBJ_FAIL;
					break;
				}
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		iRet = XmlDoc.SaveXmlFile(strCfgXmlFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		//LOG("Xml:\n%s\n", qPrintable(XmlDoc.toString()));
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::getVirChInfo(int iId, QSharedPointer <QVirChInfo> & VirChInfo_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	VirChInfo_MAP::iterator itVirChInfo;

	do
	{
		itVirChInfo = m_VirChInfo_Map.find(iId);
		if(itVirChInfo == m_VirChInfo_Map.end())
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		VirChInfo_sp = itVirChInfo.value();
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::setVirChInfo(int iId, CONST QSharedPointer <QVirChInfo> VirChInfo_sp)
{
	INT_t iOutRet = ERROR_SUCCESS;
	VirChInfo_MAP::iterator itVirChInfo;
	
	do
	{
		itVirChInfo = m_VirChInfo_Map.find(iId);
		if(itVirChInfo == m_VirChInfo_Map.end())	//not exist
		{
			if(500 <= m_VirChInfo_Map.size())	//reach the uppper limitation
			{
				iOutRet = ERROR_OUT_OF_RANGE;
				break;
			}
			m_VirChInfo_Map[iId] = VirChInfo_sp;
		}
		else	//exist
		{
			m_VirChInfo_Map[iId] = VirChInfo_sp;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t QMediaVirtualChannel::setVirChMin(int iVirChMin)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_iVirChMin = iVirChMin;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::getVirChMin(int & iVirChMin)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		iVirChMin = m_iVirChMin;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::setVirChMax(int iVirChMax)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_iVirChMax = iVirChMax;
	}while(FALSE);

	return iOutRet;
}

INT_t QMediaVirtualChannel::getVirChMax(int & iVirChMax)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		iVirChMax = m_iVirChMax;
	}while(FALSE);

	return iOutRet;
}

INT_t  QMediaVirtualChannel::getVirChIdList(QStringList & strIdList)
{
	INT_t iOutRet = ERROR_SUCCESS;
	VirChInfo_MAP::iterator itVirChInfo;
	QList <int> KeyList;
	QList <int>::iterator itInt;

	do
	{
		KeyList = m_VirChInfo_Map.uniqueKeys();
		qSort(KeyList.begin(), KeyList.end());
			
		for(itInt = KeyList.begin(); itInt != KeyList.end(); itInt++)
		{
			strIdList << QString::number(*itInt);
		}
	}while(FALSE);

	return iOutRet;
}

