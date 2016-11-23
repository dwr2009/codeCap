#include "PlaylistData.h"
#include <BaseErrDef.h>
#include <iostream>
#include <QSettings>
#include <QStringList>
#include "WebBrowserAppIf.h"
#include <QApplication>
#include <AutoLock.h>

using namespace std;

QPlaylistEntry::QPlaylistEntry()
{
	m_eActionType = QPlaylistEntry::ACTION_Unknown;
}

QPlaylistEntry::~QPlaylistEntry()
{
}

QPlaylistEntry::ACTION_TYPE QPlaylistEntry::getActionType()
{
	return m_eActionType;
}

QPlaylistEntry_Video::QPlaylistEntry_Video()
{
	m_eActionType = QPlaylistEntry::ACTION_Video;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;	//invalid
	m_iLoopTimes = 1;
	m_iHasLoopTimes = 0;
}

QPlaylistEntry_Video::~QPlaylistEntry_Video()
{

}

QPlaylistEntry_Audio::QPlaylistEntry_Audio()
{
	m_eActionType = QPlaylistEntry::ACTION_Audio;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;	//invalid	
}

QPlaylistEntry_Audio::~QPlaylistEntry_Audio()
{

}

#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
QPlaylistEntry_MediaServerAudio::QPlaylistEntry_MediaServerAudio()
{
	m_eActionType = QPlaylistEntry::ACTION_MediaServer_PlayAudio;
}

QPlaylistEntry_MediaServerAudio::~QPlaylistEntry_MediaServerAudio()
{

}
#endif
QPlaylistEntry_Html::QPlaylistEntry_Html()
{
	m_eActionType = QPlaylistEntry::ACTION_Html;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;
}

QPlaylistEntry_Html::~QPlaylistEntry_Html()
{

}

QPlaylistEntry_Browser::QPlaylistEntry_Browser()
{
	m_eActionType = QPlaylistEntry::ACTION_Browser;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;
}

QPlaylistEntry_Browser::~QPlaylistEntry_Browser()
{

}





QPlaylistEntry_Picture::QPlaylistEntry_Picture()
{
	m_eActionType = QPlaylistEntry::ACTION_Picture;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;
}

QPlaylistEntry_Picture::~QPlaylistEntry_Picture()
{

}

QPlaylistEntry_Loop::QPlaylistEntry_Loop()
{
	m_eActionType = QPlaylistEntry::ACTION_Loop;
	m_iHasLoopTimes = 0;
	m_iTimes = -1;
}

QPlaylistEntry_Loop::~QPlaylistEntry_Loop()
{
	
}


QPlaylistEntry_PlayerVolume::QPlaylistEntry_PlayerVolume()
{
	m_eActionType = QPlaylistEntry::ACTION_Volume;
 	m_iVolumeVaule= -1;
}

QPlaylistEntry_PlayerVolume::~QPlaylistEntry_PlayerVolume()
{
	
}


QPlaylistEntry_PlayerMuteVolume::QPlaylistEntry_PlayerMuteVolume()
{
	m_eActionType = QPlaylistEntry::ACTION_MuteVolume;
 	m_iIsMute= -1;
}

QPlaylistEntry_PlayerMuteVolume::~QPlaylistEntry_PlayerMuteVolume()
{
	
}

QPlaylistEntry_Reboot::QPlaylistEntry_Reboot()
{
	m_eActionType = QPlaylistEntry::ACTION_Reboot;
}

QPlaylistEntry_Reboot::~QPlaylistEntry_Reboot()
{
	
}

QPlaylistEntry_Stream::QPlaylistEntry_Stream()
{
	m_eActionType = QPlaylistEntry::ACTION_Stream;
	m_xLeft = 0;
	m_yUpper = 0;
	m_xSize = -1;	//full size
	m_ySize = -1;	//full size
	m_iDuration = -1;	//invalid
	m_iLoopTimes = -1;
	m_iHasLoopTimes = -1;
}

QPlaylistEntry_Stream::~QPlaylistEntry_Stream()
{

}


QPlaylistEntry_PlaySavedPlaylist::QPlaylistEntry_PlaySavedPlaylist()
{
	m_eActionType = QPlaylistEntry::ACTION_PlaySavedPlaylist;
}

QPlaylistEntry_PlaySavedPlaylist::~QPlaylistEntry_PlaySavedPlaylist()
{
	
}

QPlaylistEntry_PlayPlaylist::QPlaylistEntry_PlayPlaylist()
{
	m_eActionType = QPlaylistEntry::ACTION_PlayPlaylist;
}

QPlaylistEntry_PlayPlaylist::~QPlaylistEntry_PlayPlaylist()
{
	
}


QPlaylistData::QPlaylistData()
{
	m_strPlaylistVersion = DEFAULT_PLAYLIST_VERSION;
	m_eLayerType = LAYER_Video;
	m_CurEntryId = -1;	//invalid
	m_keydownflag = 0;
	m_strPlaylistControlName = "";
}

QPlaylistData::~QPlaylistData()
{
}

INT_t QPlaylistData::LoadFromXmlFile(QString * pstrFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(NULL == pstrFilePath)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = m_XmlDoc.OpenXmlFile(*pstrFilePath);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}		

		QDomElement Element_Playlist = m_XmlDoc.documentElement();
		if(Element_Playlist.isNull())
		{
			LOG_BLINE("No root element\n");
			break;
		}
		QDomElement Element_Control = Element_Playlist.firstChildElement("Control");
		if(Element_Control.isNull())
		{
			LOG_BLINE("No root.Control element\n");
			break;
		}
		m_strPlaylistVersion = Element_Control.attribute("PlaylistVersion", DEFAULT_PLAYLIST_VERSION);
		LOG_BLINE("PlaylistVersion:%s\n", qPrintable(m_strPlaylistVersion));
		QString strLayerType = Element_Control.attribute("LayerType", "Unknown");;
		m_eLayerType = getLayerTypeFromDesc(strLayerType);
		LOG_BLINE("LayerType:%d\n", m_eLayerType);
		QDomElement Element_EntryList = Element_Playlist.firstChildElement("EntryList");
		if(Element_EntryList.isNull())
		{
			LOG_BLINE("No root.Element_EntryList element\n");
			break;
		}

		QDomElement Element_Entry = Element_EntryList.firstChildElement("Entry");
		while(FALSE == Element_Entry.isNull())
		{
			QString strAction = Element_Entry.attribute("Action", "Unknown");
			QPlaylistEntry::ACTION_TYPE eActionType = getActionTypeFromDesc(strAction);
			do
			{
				QSharedPointer <QPlaylistEntry> PlaylistEntry_sp;
				if(QPlaylistEntry::ACTION_Video == eActionType)
				{
					QSharedPointer <QPlaylistEntry_Video> PlaylistEntry_Video_sp(new QPlaylistEntry_Video);
					if(PlaylistEntry_Video_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Video_sp->m_strFilePath = Element_Entry.attribute("File");
					PlaylistEntry_Video_sp->m_xLeft = Element_Entry.attribute("xleft").toInt();
					PlaylistEntry_Video_sp->m_yUpper = Element_Entry.attribute("yupper").toInt();
					PlaylistEntry_Video_sp->m_xSize = Element_Entry.attribute("xsize").toInt();
					PlaylistEntry_Video_sp->m_ySize = Element_Entry.attribute("ysize").toInt();
					PlaylistEntry_sp = PlaylistEntry_Video_sp;
					LOG_BLINE("Entry_Video,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d\n", 
						qPrintable(PlaylistEntry_Video_sp->m_strFilePath), PlaylistEntry_Video_sp->m_xLeft,
						PlaylistEntry_Video_sp->m_yUpper, PlaylistEntry_Video_sp->m_xSize,
						PlaylistEntry_Video_sp->m_ySize);
				}
				else if(QPlaylistEntry::ACTION_Picture == eActionType)
				{
					QSharedPointer <QPlaylistEntry_Picture> PlaylistEntry_Picture_sp(new QPlaylistEntry_Picture);
					if(PlaylistEntry_Picture_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Picture_sp->m_strFilePath = Element_Entry.attribute("File");
					PlaylistEntry_Picture_sp->m_xLeft = Element_Entry.attribute("xleft").toInt();
					PlaylistEntry_Picture_sp->m_yUpper = Element_Entry.attribute("yupper").toInt();
					PlaylistEntry_Picture_sp->m_xSize = Element_Entry.attribute("xsize").toInt();
					PlaylistEntry_Picture_sp->m_ySize = Element_Entry.attribute("ysize").toInt();
					PlaylistEntry_sp = PlaylistEntry_Picture_sp;
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d\n", 
						qPrintable(PlaylistEntry_Picture_sp->m_strFilePath), PlaylistEntry_Picture_sp->m_xLeft,
						PlaylistEntry_Picture_sp->m_yUpper, PlaylistEntry_Picture_sp->m_xSize,
						PlaylistEntry_Picture_sp->m_ySize);
				}
				else if(QPlaylistEntry::ACTION_Html== eActionType)
				{
					QSharedPointer <QPlaylistEntry_Html> PlaylistEntry_Html_sp(new QPlaylistEntry_Html);
					if(PlaylistEntry_Html_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Html_sp->m_strFilePath = Element_Entry.attribute("File");
					PlaylistEntry_Html_sp->m_xLeft = Element_Entry.attribute("xleft").toInt();
					PlaylistEntry_Html_sp->m_yUpper = Element_Entry.attribute("yupper").toInt();
					PlaylistEntry_Html_sp->m_xSize = Element_Entry.attribute("xsize").toInt();
					PlaylistEntry_Html_sp->m_ySize = Element_Entry.attribute("ysize").toInt();
					PlaylistEntry_sp = PlaylistEntry_Html_sp;
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d\n", 
						qPrintable(PlaylistEntry_Html_sp->m_strFilePath), PlaylistEntry_Html_sp->m_xLeft,
						PlaylistEntry_Html_sp->m_yUpper, PlaylistEntry_Html_sp->m_xSize,
						PlaylistEntry_Html_sp->m_ySize);					
				}
				else if(QPlaylistEntry::ACTION_Audio== eActionType)
				{
					QSharedPointer <QPlaylistEntry_Audio> PlaylistEntry_Audio_sp(new QPlaylistEntry_Audio);
					if(PlaylistEntry_Audio_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Audio_sp->m_strFilePath = Element_Entry.attribute("File");
					PlaylistEntry_Audio_sp->m_xLeft = Element_Entry.attribute("xleft").toInt();
					PlaylistEntry_Audio_sp->m_yUpper = Element_Entry.attribute("yupper").toInt();
					PlaylistEntry_Audio_sp->m_xSize = Element_Entry.attribute("xsize").toInt();
					PlaylistEntry_Audio_sp->m_ySize = Element_Entry.attribute("ysize").toInt();
					PlaylistEntry_sp = PlaylistEntry_Audio_sp;
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d\n", 
						qPrintable(PlaylistEntry_Audio_sp->m_strFilePath), PlaylistEntry_Audio_sp->m_xLeft,
						PlaylistEntry_Audio_sp->m_yUpper, PlaylistEntry_Audio_sp->m_xSize,
						PlaylistEntry_Audio_sp->m_ySize);
				}
				else if(QPlaylistEntry::ACTION_Loop == eActionType)
				{
					QSharedPointer <QPlaylistEntry_Loop> PlaylistEntry_Loop_sp(new QPlaylistEntry_Loop);
					if(PlaylistEntry_Loop_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_sp = PlaylistEntry_Loop_sp;
					LOG_BLINE("Entry_Loop\n");
				}
				else
				{
					break;
				}
				m_PlaylistEntryList.append(PlaylistEntry_sp);
			}while(FALSE);
			Element_Entry = Element_Entry.nextSiblingElement("Entry");			
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QPlaylistData::LoadFromIniFile(CONST QString * pstrFilePath)
{
	INT_t iOutRet = ERROR_SUCCESS, iId;
	QSettings IniSettings(*pstrFilePath, QSettings::IniFormat);
	CWebBrowserAppIf * pWebBrowserAppIf = dynamic_cast <CWebBrowserAppIf *> (qApp);

	CAutoLock AutoRangeLocker(pWebBrowserAppIf->getPlaylistAccessLocker());

	do
	{
		m_PlaylistEntryList.clear();
		m_CurEntryId = -1;	//invalid
		
		m_strPlaylistVersion = IniSettings.value("Control/PlaylistVersion", "2.0").toString();
	//	LOG_BLINE("PlaylistVersion:%s\n", qPrintable(m_strPlaylistVersion));
		QString strLayerType = IniSettings.value("Control/LayerType", "Unknown").toString();
		m_eLayerType = getLayerTypeFromDesc(strLayerType);
	//	LOG_BLINE("LayerType:%d\n", m_eLayerType);
		m_strPlaylistControlName = IniSettings.value("Control/Name", "").toString();
	//	LOG_BLINE("m_strPlaylistControlName:%s\n", qPrintable(m_strPlaylistControlName));
		
		IniSettings.beginGroup("List");
		QStringList KeyList = IniSettings.childKeys();
		
		QList<int> list;
		int iKeyListSize = KeyList.size();
 		for(int i = 0;i < iKeyListSize;i++)
		{
			QString  str_id = KeyList.at(i).mid(5);//"ENTRY"
			list << str_id.trimmed().toInt();
		}

		qSort(list.begin(), list.end(), qLess<int>());		
		KeyList.clear();
		
		for(int i = 0;i <iKeyListSize;i++)
		{
			KeyList << QString("ENTRY%1").arg(list[i]);
		}

		for(iId = 0; iId < KeyList.count(); iId++)
		{
			QVariant Value = IniSettings.value(KeyList.at(iId));
			QString strValue;
			QStringList ParamList;
			QString strAction;

			do
			{				
				if(QVariant::String == Value.type())
				{
					strAction = Value.toString();
				}
				else if(QVariant::StringList == Value.type())
				{
					ParamList = Value.toStringList();
					if(0 < ParamList.count())
					{
						strAction = ParamList.at(0);
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}
				QPlaylistEntry::ACTION_TYPE eActionType = getActionTypeFromDesc(strAction);
				QSharedPointer <QPlaylistEntry> PlaylistEntry_sp;
				if(QPlaylistEntry::ACTION_Video == eActionType)
				{
					if(2 > ParamList.count())
					{		
						break;
					}
					QSharedPointer <QPlaylistEntry_Video> PlaylistEntry_Video_sp(new QPlaylistEntry_Video);
					if(PlaylistEntry_Video_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Video_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_Video_sp->m_strFilePath.isEmpty())
					{
						break;
					}
					int PropertyId = 2;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						
						QStringList KeyValuePairList = strPropertyPair.split('=');
						
						if(2 <= KeyValuePairList.count())
						{
							if("xleft" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("yupper" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("xsize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("ysize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("duration" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("loop" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Video_sp->m_iLoopTimes = KeyValuePairList.at(1).trimmed().toInt();
							}
						}
					}
					/*
					LOG_BLINE("Entry_Video,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
						qPrintable(PlaylistEntry_Video_sp->m_strFilePath), PlaylistEntry_Video_sp->m_xLeft,
						PlaylistEntry_Video_sp->m_yUpper, PlaylistEntry_Video_sp->m_xSize,
						PlaylistEntry_Video_sp->m_ySize,PlaylistEntry_Video_sp->m_iDuration);
					*/
					PlaylistEntry_sp = PlaylistEntry_Video_sp;
				}
				else if(QPlaylistEntry::ACTION_Picture == eActionType)
				{
					if(2 > ParamList.count())
					{
						break;
					}
					QSharedPointer <QPlaylistEntry_Picture> PlaylistEntry_Picture_sp(new QPlaylistEntry_Picture);
					if(PlaylistEntry_Picture_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Picture_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_Picture_sp->m_strFilePath.isEmpty())
					{
						break;
					}
					int PropertyId = 2;
					PlaylistEntry_Picture_sp->m_iDuration = DEFAULT_PICTURE_DURATION;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						QStringList KeyValuePairList = strPropertyPair.split('=');
						if(2 <= KeyValuePairList.count())
						{
							if("xleft" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Picture_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("yupper" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Picture_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("xsize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Picture_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("ysize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Picture_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("duration" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Picture_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
							}
						}
					}
					/*
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
						qPrintable(PlaylistEntry_Picture_sp->m_strFilePath), PlaylistEntry_Picture_sp->m_xLeft,
						PlaylistEntry_Picture_sp->m_yUpper, PlaylistEntry_Picture_sp->m_xSize,
						PlaylistEntry_Picture_sp->m_ySize,PlaylistEntry_Picture_sp->m_iDuration);
					*/
					PlaylistEntry_sp = PlaylistEntry_Picture_sp;
				}
				else if(QPlaylistEntry::ACTION_Html == eActionType || QPlaylistEntry::ACTION_Browser == eActionType)
				{
					if(2 > ParamList.count())
					{
						break;
					}
					QSharedPointer <QPlaylistEntry_Html> PlaylistEntry_Html_sp(new QPlaylistEntry_Html);
					if(PlaylistEntry_Html_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Html_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_Html_sp->m_strFilePath.isEmpty())
					{
						break;
					}
					int PropertyId = 2;
					PlaylistEntry_Html_sp->m_iDuration = DEFAULT_PICTURE_DURATION;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						QStringList KeyValuePairList = strPropertyPair.split('=');
						if(2 <= KeyValuePairList.count())
						{
							if("xleft" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Html_sp->m_xLeft = KeyValuePairList.at(1).toInt();
							}
							else if("yupper" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Html_sp->m_yUpper = KeyValuePairList.at(1).toInt();
							}
							else if("xsize" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Html_sp->m_xSize = KeyValuePairList.at(1).toInt();
							}
							else if("ysize" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Html_sp->m_ySize = KeyValuePairList.at(1).toInt();
							}
							else if("duration" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Html_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
							}
						}
					}
					/*
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
						qPrintable(PlaylistEntry_Html_sp->m_strFilePath), PlaylistEntry_Html_sp->m_xLeft,
						PlaylistEntry_Html_sp->m_yUpper, PlaylistEntry_Html_sp->m_xSize,
						PlaylistEntry_Html_sp->m_ySize,PlaylistEntry_Html_sp->m_iDuration);
					*/
					PlaylistEntry_sp = PlaylistEntry_Html_sp;
				}
				else if(QPlaylistEntry::ACTION_Audio == eActionType)
				{
					if(2 > ParamList.count())
					{
						break;
					}
					QSharedPointer <QPlaylistEntry_Audio> PlaylistEntry_Audio_sp(new QPlaylistEntry_Audio);
					if(PlaylistEntry_Audio_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Audio_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_Audio_sp->m_strFilePath.isEmpty())
					{
						break;
					}
					int PropertyId = 2;
					PlaylistEntry_Audio_sp->m_iDuration = -1;	//invalid
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						QStringList KeyValuePairList = strPropertyPair.split('=');
						if(2 <= KeyValuePairList.count())
						{
							if("xleft" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Audio_sp->m_xLeft = KeyValuePairList.at(1).toInt();
							}
							else if("yupper" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Audio_sp->m_yUpper = KeyValuePairList.at(1).toInt();
							}
							else if("xsize" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Audio_sp->m_xSize = KeyValuePairList.at(1).toInt();
							}
							else if("ysize" == KeyValuePairList.at(0))
							{
								PlaylistEntry_Audio_sp->m_ySize = KeyValuePairList.at(1).toInt();
							}
							else if("duration" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Audio_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
							}
						}
					}
					/*
					LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
						qPrintable(PlaylistEntry_Audio_sp->m_strFilePath), PlaylistEntry_Audio_sp->m_xLeft,
						PlaylistEntry_Audio_sp->m_yUpper, PlaylistEntry_Audio_sp->m_xSize,
						PlaylistEntry_Audio_sp->m_ySize,PlaylistEntry_Audio_sp->m_iDuration);
					*/
					PlaylistEntry_sp = PlaylistEntry_Audio_sp;
				}
				else if(QPlaylistEntry::ACTION_Loop == eActionType)
				{
					QSharedPointer <QPlaylistEntry_Loop> PlaylistEntry_Loop_sp(new QPlaylistEntry_Loop);
					if(PlaylistEntry_Loop_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					int PropertyId = 1;
					PlaylistEntry_Loop_sp->m_iTimes = -1;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						PlaylistEntry_Loop_sp->m_iTimes = strPropertyPair.trimmed().toInt();
					}
					PlaylistEntry_sp = PlaylistEntry_Loop_sp;
					//LOG_BLINE("Entry_Loop\n");
				}
				else if(QPlaylistEntry::ACTION_Volume == eActionType)
				{
					QSharedPointer <QPlaylistEntry_PlayerVolume> PlaylistEntry_Volume_sp(new QPlaylistEntry_PlayerVolume);
					if(PlaylistEntry_Volume_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					int PropertyId = 1;
					PlaylistEntry_Volume_sp->m_iVolumeVaule = -1;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						PlaylistEntry_Volume_sp->m_iVolumeVaule = strPropertyPair.trimmed().toInt();
					}
					PlaylistEntry_sp = PlaylistEntry_Volume_sp;
					//LOG_BLINE("Entry_Loop\n");
				}
				else if(QPlaylistEntry::ACTION_MuteVolume == eActionType)
				{
					QSharedPointer <QPlaylistEntry_PlayerMuteVolume> PlaylistEntry_MuteVolume_sp(new QPlaylistEntry_PlayerMuteVolume);
					if(PlaylistEntry_MuteVolume_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					int PropertyId = 1;
					PlaylistEntry_MuteVolume_sp->m_iIsMute = -1;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						PlaylistEntry_MuteVolume_sp->m_iIsMute = strPropertyPair.trimmed().toInt();
					}
					PlaylistEntry_sp = PlaylistEntry_MuteVolume_sp;
					
				}
				else if(QPlaylistEntry::ACTION_PlaySavedPlaylist == eActionType)
				{
					QSharedPointer <QPlaylistEntry_PlaySavedPlaylist> PlaylistEntry_PlaySavedPlaylist_sp(new QPlaylistEntry_PlaySavedPlaylist);
					if(PlaylistEntry_PlaySavedPlaylist_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}					
					PlaylistEntry_sp = PlaylistEntry_PlaySavedPlaylist_sp;
					//LOG_BLINE("Entry_PlaySavedPlaylist\n");
				}
				else if(QPlaylistEntry::ACTION_Reboot == eActionType)
				{					
					QSharedPointer <QPlaylistEntry_Reboot> PlaylistEntry_Reboot_sp(new QPlaylistEntry_Reboot);
					if(PlaylistEntry_Reboot_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}					
					PlaylistEntry_sp = PlaylistEntry_Reboot_sp;
				}
				else if(QPlaylistEntry::ACTION_Stream == eActionType)
				{
					if(2 > ParamList.count())
					{
						break;
					}
					QSharedPointer <QPlaylistEntry_Stream> PlaylistEntry_Stream_sp(new QPlaylistEntry_Stream);
					if(PlaylistEntry_Stream_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_Stream_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_Stream_sp->m_strFilePath.isEmpty())
					{
						break;
					}
					int PropertyId = 2;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{
						
						CONST QString & strPropertyPair = ParamList.at(PropertyId);
						
						QStringList KeyValuePairList = strPropertyPair.split('=');
						
						if(2 <= KeyValuePairList.count())
						{
							if("xleft" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("yupper" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("xsize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("ysize" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("duration" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
							}
							else if("loop" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_Stream_sp->m_iLoopTimes = KeyValuePairList.at(1).trimmed().toInt();
							}
						}
					}
					/*
					LOG_BLINE("Entry_Video,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
						qPrintable(PlaylistEntry_Video_sp->m_strFilePath), PlaylistEntry_Video_sp->m_xLeft,
						PlaylistEntry_Video_sp->m_yUpper, PlaylistEntry_Video_sp->m_xSize,
						PlaylistEntry_Video_sp->m_ySize,PlaylistEntry_Video_sp->m_iDuration);
					*/
					PlaylistEntry_sp = PlaylistEntry_Stream_sp;
				}
				else if(QPlaylistEntry::ACTION_PlayPlaylist == eActionType)
				{					
					QSharedPointer <QPlaylistEntry_PlayPlaylist> PlaylistEntry_PlayPlaylist_sp(new QPlaylistEntry_PlayPlaylist);
					if(PlaylistEntry_PlayPlaylist_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}					
					PlaylistEntry_sp = PlaylistEntry_PlayPlaylist_sp;
					int PropertyId = 1;
					for(; PropertyId < ParamList.count(); PropertyId++)
					{						
						CONST QString & strPropertyPair = ParamList.at(PropertyId);							
						QStringList KeyValuePairList = strPropertyPair.split('=');
						if(1 <= KeyValuePairList.count())
						{
							if("url" == KeyValuePairList.at(0).trimmed())
							{
								PlaylistEntry_PlayPlaylist_sp->m_strFilePath = KeyValuePairList.at(1).trimmed();
							}
						}
					}
				}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
				else if(QPlaylistEntry::ACTION_MediaServer_PlayAudio == eActionType)
				{
					if(2 > ParamList.count())
					{
						break;
					}
					QSharedPointer <QPlaylistEntry_MediaServerAudio> PlaylistEntry_MediaServerAudio_sp(new QPlaylistEntry_MediaServerAudio);
					if(PlaylistEntry_MediaServerAudio_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}
					PlaylistEntry_MediaServerAudio_sp->m_strFilePath = ParamList.at(1);
					if(PlaylistEntry_MediaServerAudio_sp->m_strFilePath.isEmpty())
					{
						break;
					}

					PlaylistEntry_sp = PlaylistEntry_MediaServerAudio_sp;
				}
				/*Media Stop Audio*/
				else if(QPlaylistEntry::ACTION_MediaServer_StopAudio == eActionType)
				{

					QSharedPointer <QPlaylistEntry_MediaServerAudio> PlaylistEntry_MediaServerAudio_sp(new QPlaylistEntry_MediaServerAudio);
					if(PlaylistEntry_MediaServerAudio_sp.isNull())
					{
						iOutRet = ERROR_OUT_OF_MEMORY;
						break;
					}

					PlaylistEntry_sp = PlaylistEntry_MediaServerAudio_sp;
				}
#endif
				else
				{
					break;
				}
				if(FALSE == PlaylistEntry_sp.isNull())
				{					
					m_PlaylistEntryList.append(PlaylistEntry_sp);
				}
			}while(FALSE);
		}
		IniSettings.endGroup();
	}while(FALSE);

	return iOutRet;
}

QPlaylistData::LAYER_TYPE QPlaylistData::getLayerTypeFromDesc(QString & strLayerType)
{
	LAYER_TYPE eLayerType = LAYER_UNKNOWN;

	if(strLayerType == LAYER_TYPE_DESC_Video)
	{
		eLayerType = LAYER_Video;
	}
	else if(strLayerType == LAYER_TYPE_DESC_Browser)
	{
		eLayerType = LAYER_Browser;
	}
	else if(strLayerType == LAYER_TYPE_DESC_Unknown)
	{
		eLayerType = LAYER_UNKNOWN;
	}

	return eLayerType;
}

QPlaylistEntry::ACTION_TYPE QPlaylistData::getActionTypeFromDesc(CONST QString & strAction)
{
	QPlaylistEntry::ACTION_TYPE eActionType = QPlaylistEntry::ACTION_Unknown;

	if(strAction.trimmed() == ACTION_TYPE_DESC_Video)
	{
		eActionType = QPlaylistEntry::ACTION_Video;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Picture)
	{
		eActionType = QPlaylistEntry::ACTION_Picture;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Loop)
	{
		eActionType = QPlaylistEntry::ACTION_Loop;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Audio)
	{
		eActionType = QPlaylistEntry::ACTION_Audio;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Html)
	{
		eActionType = QPlaylistEntry::ACTION_Html;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Browser)
	{
		eActionType = QPlaylistEntry::ACTION_Browser;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_PlaySavedPlaylist)
	{
		eActionType = QPlaylistEntry::ACTION_PlaySavedPlaylist;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Volume)
	{
		eActionType = QPlaylistEntry::ACTION_Volume;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_MuteVolume)
	{
		eActionType = QPlaylistEntry::ACTION_MuteVolume;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Reboot)
	{
		eActionType = QPlaylistEntry::ACTION_Reboot;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_Stream)
	{
		eActionType = QPlaylistEntry::ACTION_Stream;
	}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/	
	else if(strAction.trimmed() == ACTION_TYPE_DESC_MEDIASERVER_PALY_AUDIO	)
	{
		eActionType = QPlaylistEntry::ACTION_MediaServer_PlayAudio;
	}
	else if(strAction.trimmed() == ACTION_TYPE_DESC_MEDIASERVER_STOP_AUDIO	)
	{
		eActionType = QPlaylistEntry::ACTION_MediaServer_StopAudio;
	}	
	else if(strAction.trimmed() == ACTION_TYPE_DESC_PlayPlaylist	)
	{
		eActionType = QPlaylistEntry::ACTION_PlayPlaylist;
	}
#endif
	return eActionType;
}

QSharedPointer <QPlaylistEntry> QPlaylistData::MoveToNextPlayEntry(BOOL bMoveNext)
{
	QSharedPointer <QPlaylistEntry> PlayEntry_sp;
	static int sicount = 0;
    static int Keydown = 0;
//	LOG_LINE("m_CurEntryId:%d\n", m_CurEntryId);
	do
	{
		if(0 >= m_PlaylistEntryList.count())
		{
			break;
		}
		if(-1 > m_CurEntryId)
		{
			m_CurEntryId = 0;
		}
		
		if(bMoveNext || (m_keydownflag == 1))
		{	
			if(m_keydownflag == 1)
			{
					Keydown=1;
			}
			m_CurEntryId++;	
			m_keydownflag = 0;
			sicount++;
	//		LOG_LINE("m_CurEntryId:%d,sicount:%d\n", m_CurEntryId, sicount);
		}
		
		if(m_PlaylistEntryList.count() <= m_CurEntryId)
		{
			break;
		}
		if((sicount >= 3)&&(Keydown>=1))/*Press the down button to detect the press two times*/
		{
			Keydown=0;
			sicount = 1; 
			m_CurEntryId = m_CurEntryId - 1;
			if(m_CurEntryId < 0)
				m_CurEntryId = 0;
		}
//		LOG_LINE("m_CurEntryId:%d\n", m_CurEntryId);
		PlayEntry_sp = m_PlaylistEntryList.at(m_CurEntryId);
	}while(FALSE);

	return PlayEntry_sp;
}

QSharedPointer <QPlaylistEntry> QPlaylistData::MoveToFirstPlayEntry()
{
	m_CurEntryId = -1;
	return MoveToNextPlayEntry();
}

QSharedPointer <QPlaylistEntry> QPlaylistData::MoveToPreHeadPlayEntry()
{
	static	INT_t keyupflag=0;
	QSharedPointer <QPlaylistEntry> PlayEntry_sp;
	do
	{	
		if(0 >= m_PlaylistEntryList.count())
		{
			break;
		}
		if(-1 >= m_CurEntryId)
		{
			m_CurEntryId = -1;
		}
		else
		{
			m_CurEntryId--;	
			keyupflag++;
		}
		if(keyupflag>=2)
			{
				m_CurEntryId++;
				keyupflag=0;
			}
		if(m_PlaylistEntryList.count() <= m_CurEntryId)
		{
			break;
		}
		LOG_BLINE("m_CurEntryId  :  %d   \n",m_CurEntryId);
		PlayEntry_sp = m_PlaylistEntryList.at(m_CurEntryId);
	}while(FALSE);
	return PlayEntry_sp;
}

QSharedPointer <QPlaylistEntry> QPlaylistData::CurrPlayEntry()
{
	QSharedPointer <QPlaylistEntry> PlayEntry_sp;

	do
	{
		if(0 >= m_PlaylistEntryList.count())
		{
			break;
		}
		if(0 >= m_CurEntryId)
		{
			m_CurEntryId = 0;
		}
		if(m_PlaylistEntryList.count() <= m_CurEntryId)
		{
			break;
		}
		PlayEntry_sp = m_PlaylistEntryList.at(m_CurEntryId);
	}while(FALSE);

	return PlayEntry_sp;
}

int QPlaylistData::getCurPlayEntryId()
{
	return m_CurEntryId;
}

int QPlaylistData::PlayEntryCount()
{
	return m_PlaylistEntryList.count();
}

BOOL_t QPlaylistData::IsLastEntry()
{
	BOOL_t bOutRet = ERROR_SUCCESS;

	do
	{
		bOutRet = ((m_CurEntryId == (m_PlaylistEntryList.count() - 1)) || 0 == m_PlaylistEntryList.count());
	}while(FALSE);

	return bOutRet;
}

void QPlaylistData::ClearPlayListData()
{
	do
	{
		if(m_PlaylistEntryList.isEmpty())
		{
			break;
		}
		m_PlaylistEntryList.clear();
	}while(FALSE);
}

BOOL_t QPlaylistData::isEmpty()
{
	BOOL_t bOutRet = false;
	do
	{
		if(m_PlaylistEntryList.isEmpty())
		{
			bOutRet = true;
			break;
		}
	}while(FALSE);

	return bOutRet;
}

QString QPlaylistData::getCurrentPlayListName()
{
	return m_strPlaylistControlName;
}

//ENTRY2 = VIDEO, Lady_Gaga_720_Poker_Face.1.8m.MP4, xleft=0, yupper=0, duration=8
INT_t QPlaylistData::insertPlayEntry(const QString  strActionUrl,const QString strActionType,
	QString strOtherPar,int iEntryIndex )
{
 	INT_t iOutRet = ERROR_SUCCESS;
	iEntryIndex = 0 ;
	
	do
	{
		QPlaylistEntry::ACTION_TYPE eActionType = getActionTypeFromDesc(strActionType.toUpper());
		QSharedPointer <QPlaylistEntry> PlaylistEntry_sp;
		if(QPlaylistEntry::ACTION_Unknown == eActionType)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		QStringList ParamList;
		ParamList =strOtherPar.split(","); //QVariant(strOtherPar).toStringList();
		if(QPlaylistEntry::ACTION_Video == eActionType)
		{
			QSharedPointer <QPlaylistEntry_Video> PlaylistEntry_Video_sp(new QPlaylistEntry_Video);
			if(PlaylistEntry_Video_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			PlaylistEntry_Video_sp->m_strFilePath = strActionUrl;
			if(PlaylistEntry_Video_sp->m_strFilePath.isEmpty())
			{
				break;
			}
			int PropertyId = 0;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
		
				QStringList KeyValuePairList = strPropertyPair.split('=');
				
				if(2 <= KeyValuePairList.count())
				{
					if("xleft" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("yupper" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("xsize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("ysize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("duration" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("loop" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Video_sp->m_iLoopTimes = KeyValuePairList.at(1).trimmed().toInt();
					}
				}
			}
			/*
			LOG_BLINE("Entry_Video,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d,loop=%d\n", 
				qPrintable(PlaylistEntry_Video_sp->m_strFilePath), PlaylistEntry_Video_sp->m_xLeft,
				PlaylistEntry_Video_sp->m_yUpper, PlaylistEntry_Video_sp->m_xSize,
				PlaylistEntry_Video_sp->m_ySize,PlaylistEntry_Video_sp->m_iDuration,PlaylistEntry_Video_sp->m_iLoopTimes);
			*/
			PlaylistEntry_sp = PlaylistEntry_Video_sp;
		}
		else if(QPlaylistEntry::ACTION_Picture == eActionType)
		{
			
			QSharedPointer <QPlaylistEntry_Picture> PlaylistEntry_Picture_sp(new QPlaylistEntry_Picture);
			if(PlaylistEntry_Picture_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}			
			PlaylistEntry_Picture_sp->m_strFilePath = strActionUrl;
			if(PlaylistEntry_Picture_sp->m_strFilePath.isEmpty())
			{
				break;
			}
			int PropertyId = 0;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{				
				CONST QString & strPropertyPair = ParamList.at(PropertyId);		
				QStringList KeyValuePairList = strPropertyPair.split('=');
				
				if(2 <= KeyValuePairList.count())
				{
					if("xleft" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Picture_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("yupper" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Picture_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("xsize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Picture_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("ysize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Picture_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("duration" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Picture_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
					}
					
				}
			}
			/*
			LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
				qPrintable(PlaylistEntry_Picture_sp->m_strFilePath), PlaylistEntry_Picture_sp->m_xLeft,
				PlaylistEntry_Picture_sp->m_yUpper, PlaylistEntry_Picture_sp->m_xSize,
				PlaylistEntry_Picture_sp->m_ySize,PlaylistEntry_Picture_sp->m_iDuration);
			*/
			PlaylistEntry_sp = PlaylistEntry_Picture_sp;
		}
		else if(QPlaylistEntry::ACTION_Html == eActionType || QPlaylistEntry::ACTION_Browser == eActionType)
		{
			QSharedPointer <QPlaylistEntry_Html> PlaylistEntry_Html_sp(new QPlaylistEntry_Html);
			if(PlaylistEntry_Html_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			PlaylistEntry_Html_sp->m_strFilePath = strActionUrl;
			if(PlaylistEntry_Html_sp->m_strFilePath.isEmpty())
			{
				break;
			}
			int PropertyId = 0;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{				
				CONST QString & strPropertyPair = ParamList.at(PropertyId);		
				QStringList KeyValuePairList = strPropertyPair.split('=');
				
				if(2 <= KeyValuePairList.count())
				{
					if("xleft" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Html_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("yupper" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Html_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("xsize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Html_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("ysize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Html_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("duration" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Html_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
					}
					
				}
			}
			/*
			LOG_BLINE("Entry_Picture,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
				qPrintable(PlaylistEntry_Html_sp->m_strFilePath), PlaylistEntry_Html_sp->m_xLeft,
				PlaylistEntry_Html_sp->m_yUpper, PlaylistEntry_Html_sp->m_xSize,
				PlaylistEntry_Html_sp->m_ySize,PlaylistEntry_Html_sp->m_iDuration);
			*/
			PlaylistEntry_sp = PlaylistEntry_Html_sp;
		}
		else if(QPlaylistEntry::ACTION_Audio == eActionType)
		{
			LOG_BLINE("Entry_Picture,xLeft=%d",ParamList.count());
			
			QSharedPointer <QPlaylistEntry_Audio> PlaylistEntry_Audio_sp(new QPlaylistEntry_Audio);
			if(PlaylistEntry_Audio_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			
			PlaylistEntry_Audio_sp->m_strFilePath = strActionUrl;
			if(PlaylistEntry_Audio_sp->m_strFilePath.isEmpty())
			{
				break;
			}
			int PropertyId = 0;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
		
				QStringList KeyValuePairList = strPropertyPair.split('=');
				
				if(2 <= KeyValuePairList.count())
				{
					if("xleft" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Audio_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("yupper" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Audio_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("xsize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Audio_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("ysize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Audio_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("duration" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Audio_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
					}
					
				}
			}
			/*
			LOG_BLINE("Entry_Audio,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
				qPrintable(PlaylistEntry_Audio_sp->m_strFilePath), PlaylistEntry_Audio_sp->m_xLeft,
				PlaylistEntry_Audio_sp->m_yUpper, PlaylistEntry_Audio_sp->m_xSize,
				PlaylistEntry_Audio_sp->m_ySize,PlaylistEntry_Audio_sp->m_iDuration);
			*/
			PlaylistEntry_sp = PlaylistEntry_Audio_sp;
		}
		else if(QPlaylistEntry::ACTION_Loop == eActionType)
		{
			QSharedPointer <QPlaylistEntry_Loop> PlaylistEntry_Loop_sp(new QPlaylistEntry_Loop);
			if(PlaylistEntry_Loop_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			int PropertyId = 1;
			PlaylistEntry_Loop_sp->m_iTimes = -1;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
				PlaylistEntry_Loop_sp->m_iTimes = strPropertyPair.trimmed().toInt();
			}
			PlaylistEntry_sp = PlaylistEntry_Loop_sp;
			//LOG_BLINE("Entry_Loop\n");
		}
		else if(QPlaylistEntry::ACTION_Volume == eActionType)
		{
			QSharedPointer <QPlaylistEntry_PlayerVolume> PlaylistEntry_Volume_sp(new QPlaylistEntry_PlayerVolume);
			if(PlaylistEntry_Volume_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			int PropertyId = 1;
			PlaylistEntry_Volume_sp->m_iVolumeVaule = -1;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
				PlaylistEntry_Volume_sp->m_iVolumeVaule = strPropertyPair.trimmed().toInt();
			}
			PlaylistEntry_sp = PlaylistEntry_Volume_sp;
			//LOG_BLINE("Entry_Loop\n");
		}
		else if(QPlaylistEntry::ACTION_MuteVolume == eActionType)
		{
			QSharedPointer <QPlaylistEntry_PlayerMuteVolume> PlaylistEntry_MuteVolume_sp(new QPlaylistEntry_PlayerMuteVolume);
			if(PlaylistEntry_MuteVolume_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			int PropertyId = 1;
			PlaylistEntry_MuteVolume_sp->m_iIsMute = -1;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
				PlaylistEntry_MuteVolume_sp->m_iIsMute = strPropertyPair.trimmed().toInt();
			}
			PlaylistEntry_sp = PlaylistEntry_MuteVolume_sp;			
		}
		else if(QPlaylistEntry::ACTION_PlaySavedPlaylist == eActionType)
		{
			QSharedPointer <QPlaylistEntry_PlaySavedPlaylist> PlaylistEntry_PlaySavedPlaylist_sp(new QPlaylistEntry_PlaySavedPlaylist);
			if(PlaylistEntry_PlaySavedPlaylist_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}					
			PlaylistEntry_sp = PlaylistEntry_PlaySavedPlaylist_sp;
			//LOG_BLINE("Entry_PlaySavedPlaylist\n");
		}
		else if(QPlaylistEntry::ACTION_Reboot == eActionType)
		{					
			QSharedPointer <QPlaylistEntry_Reboot> PlaylistEntry_Reboot_sp(new QPlaylistEntry_Reboot);
			if(PlaylistEntry_Reboot_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}					
			PlaylistEntry_sp = PlaylistEntry_Reboot_sp;
		}
		else if(QPlaylistEntry::ACTION_Stream == eActionType)
		{
			if(2 > ParamList.count())
			{
				break;
			}
			QSharedPointer <QPlaylistEntry_Stream> PlaylistEntry_Stream_sp(new QPlaylistEntry_Stream);
			if(PlaylistEntry_Stream_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			PlaylistEntry_Stream_sp->m_strFilePath = ParamList.at(1);
			if(PlaylistEntry_Stream_sp->m_strFilePath.isEmpty())
			{
				break;
			}
			int PropertyId = 2;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{
				
				CONST QString & strPropertyPair = ParamList.at(PropertyId);
				
				QStringList KeyValuePairList = strPropertyPair.split('=');
				
				if(2 <= KeyValuePairList.count())
				{
					if("xleft" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_xLeft = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("yupper" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_yUpper = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("xsize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_xSize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("ysize" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_ySize = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("duration" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_iDuration = KeyValuePairList.at(1).trimmed().toInt();
					}
					else if("loop" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_Stream_sp->m_iLoopTimes = KeyValuePairList.at(1).trimmed().toInt();
					}
				}
			}
			/*
			LOG_BLINE("Entry_Video,File=%s,xLeft=%d,yUpper=%d,xSize=%d,ySize=%d,Duration=%d\n", 
				qPrintable(PlaylistEntry_Video_sp->m_strFilePath), PlaylistEntry_Video_sp->m_xLeft,
				PlaylistEntry_Video_sp->m_yUpper, PlaylistEntry_Video_sp->m_xSize,
				PlaylistEntry_Video_sp->m_ySize,PlaylistEntry_Video_sp->m_iDuration);
			*/
			PlaylistEntry_sp = PlaylistEntry_Stream_sp;
		}
		else if(QPlaylistEntry::ACTION_PlayPlaylist == eActionType)
		{					
			QSharedPointer <QPlaylistEntry_PlayPlaylist> PlaylistEntry_PlayPlaylist_sp(new QPlaylistEntry_PlayPlaylist);
			if(PlaylistEntry_PlayPlaylist_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}					
			PlaylistEntry_sp = PlaylistEntry_PlayPlaylist_sp;
			int PropertyId = 1;
			for(; PropertyId < ParamList.count(); PropertyId++)
			{						
				CONST QString & strPropertyPair = ParamList.at(PropertyId);							
				QStringList KeyValuePairList = strPropertyPair.split('=');
				if(1 <= KeyValuePairList.count())
				{
					if("url" == KeyValuePairList.at(0).trimmed())
					{
						PlaylistEntry_PlayPlaylist_sp->m_strFilePath = KeyValuePairList.at(1).trimmed();
					}
				}
			}
		}
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
		else if(QPlaylistEntry::ACTION_MediaServer_PlayAudio == eActionType)
		{
			if(2 > ParamList.count())
			{
				break;
			}
			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlaylistEntry_MediaServerAudio_sp(new QPlaylistEntry_MediaServerAudio);
			if(PlaylistEntry_MediaServerAudio_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			PlaylistEntry_MediaServerAudio_sp->m_strFilePath = ParamList.at(1);
			if(PlaylistEntry_MediaServerAudio_sp->m_strFilePath.isEmpty())
			{
				break;
			}

			PlaylistEntry_sp = PlaylistEntry_MediaServerAudio_sp;
		}
		/*Media Stop Audio*/
		else if(QPlaylistEntry::ACTION_MediaServer_StopAudio == eActionType)
		{

			QSharedPointer <QPlaylistEntry_MediaServerAudio> PlaylistEntry_MediaServerAudio_sp(new QPlaylistEntry_MediaServerAudio);
			if(PlaylistEntry_MediaServerAudio_sp.isNull())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}

			PlaylistEntry_sp = PlaylistEntry_MediaServerAudio_sp;
		}
#endif
		else
		{
			break;
		}
		if(FALSE == PlaylistEntry_sp.isNull())
		{					
			m_PlaylistEntryList.append(PlaylistEntry_sp);
		}
		if(strActionUrl.trimmed() == "")
		{
			
		}
	}while(false);

	return iOutRet;
}


INT_t QPlaylistData::removePlayEntry(int iEntryIndex)
{
       INT_t iOutRet = ERROR_SUCCESS;
       do
      {
		if(m_PlaylistEntryList.isEmpty())
		{
		       break;
		}
		if(iEntryIndex >= m_PlaylistEntryList.count())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		m_PlaylistEntryList.removeAt(iEntryIndex);                               
       }while(FALSE);

	return iOutRet;
}

