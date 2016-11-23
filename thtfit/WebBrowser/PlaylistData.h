#ifndef	_PLAY_LIST_DATA_H_
#define	_PLAY_LIST_DATA_H_

#include <QXmlDocument.h>
#include <QObject>
#include <QList>
#include <QSharedPointer>

#define	DEFAULT_PLAYLIST_VERSION		"2.0"

//layer type desc
#define		LAYER_TYPE_DESC_Unknown		"Unknown"
#define		LAYER_TYPE_DESC_Video		"Video"
#define		LAYER_TYPE_DESC_Browser		"Browser"

//Action desc
#define		ACTION_TYPE_DESC_Unknown	"Unknown"
#define		ACTION_TYPE_DESC_Video		"VIDEO"
#define		ACTION_TYPE_DESC_Picture	"PICTURE"
#define		ACTION_TYPE_DESC_Audio		"AUDIO"
#define		ACTION_TYPE_DESC_Html		"HTML"
#define		ACTION_TYPE_DESC_Loop		"LOOP"
#define		ACTION_TYPE_DESC_PlaySavedPlaylist		"PlaySavedPlaylist"
#define		ACTION_TYPE_DESC_Volume		"PLAYERVOLUME"
#define		ACTION_TYPE_DESC_MuteVolume		"PLAYERMUTE"
#define		ACTION_TYPE_DESC_Reboot		"REBOOT"
#define		ACTION_TYPE_DESC_Browser		"BROWSER"
#define		ACTION_TYPE_DESC_Stream		"STREAM"
#define		ACTION_TYPE_DESC_PlayPlaylist		"playPlaylist"
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
#define     ACTION_TYPE_DESC_MEDIASERVER_PALY_AUDIO "MediaServer_PlayAudio"
#define     ACTION_TYPE_DESC_MEDIASERVER_STOP_AUDIO "MediaServer_StopAudio"
#endif

#define	DEFAULT_PICTURE_DURATION		10
#define	DEFAULT_PLAY_DURATION_TIME		-1
#define	SNMP_DEFAULT_PLAY_DURATION_TIME		0
#define	SNMP_DEFAULT_LOOP_TIMES		0

class QPlaylistEntry : public QObject
{
	Q_OBJECT
public:
	typedef enum
	{
		ACTION_Unknown,
		ACTION_Video,
		ACTION_Picture,
		ACTION_Html,
		ACTION_Audio,
		ACTION_PlaySavedPlaylist,
		ACTION_Loop,
		ACTION_Volume,
		ACTION_MuteVolume,
		ACTION_Reboot,
		ACTION_Browser,
		ACTION_Stream,
		ACTION_PlayPlaylist,
#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
		ACTION_MediaServer_PlayAudio,
		ACTION_MediaServer_StopAudio,
#endif
	}ACTION_TYPE;
public:
	QPlaylistEntry();
	virtual ~QPlaylistEntry();
	virtual ACTION_TYPE getActionType();
protected:
	ACTION_TYPE m_eActionType;
};

class QPlaylistEntry_Video : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Video();
	virtual ~QPlaylistEntry_Video();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
	int m_iAlpha;
	int m_iLoopTimes;
	int m_iHasLoopTimes;
};

class QPlaylistEntry_Picture : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Picture();
	virtual ~QPlaylistEntry_Picture();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
	int m_iAlpha;
};

class QPlaylistEntry_Audio : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Audio();
	virtual ~QPlaylistEntry_Audio();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
};

class QPlaylistEntry_Html: public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Html();
	virtual ~QPlaylistEntry_Html();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
};

class QPlaylistEntry_Browser: public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Browser();
	virtual ~QPlaylistEntry_Browser();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
};


class QPlaylistEntry_PlaySavedPlaylist: public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_PlaySavedPlaylist();
	virtual ~QPlaylistEntry_PlaySavedPlaylist();
};


class QPlaylistEntry_Loop : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Loop();
	virtual ~QPlaylistEntry_Loop();
public:
	int m_iTimes;
	int m_iHasLoopTimes;
};


class QPlaylistEntry_PlayerVolume : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_PlayerVolume();
	virtual ~QPlaylistEntry_PlayerVolume();
public:
	int m_iVolumeVaule;
};

class QPlaylistEntry_PlayerMuteVolume : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_PlayerMuteVolume();
	virtual ~QPlaylistEntry_PlayerMuteVolume();
public:
	int m_iIsMute;
};

class QPlaylistEntry_Reboot: public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Reboot();
	virtual ~QPlaylistEntry_Reboot();
};

class QPlaylistEntry_Stream : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_Stream();
	virtual ~QPlaylistEntry_Stream();
public:
	QString m_strFilePath;
	int m_xLeft;
	int m_yUpper;
	int m_xSize;
	int m_ySize;
	int m_iDuration;
	int m_iAlpha;
	int m_iLoopTimes;
	int m_iHasLoopTimes;
};


class QPlaylistEntry_PlayPlaylist: public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_PlayPlaylist();
	virtual ~QPlaylistEntry_PlayPlaylist();
public:
	QString m_strFilePath;
};

#if 1/*added by lxj 2012-8-9 for MediaServer Audio Play/Stop on alsa*/
class QPlaylistEntry_MediaServerAudio : public QPlaylistEntry
{
	Q_OBJECT
		
public:
	QPlaylistEntry_MediaServerAudio();
	virtual ~QPlaylistEntry_MediaServerAudio();
public:
	QString m_strFilePath;
};
#endif

typedef QList < QSharedPointer <QPlaylistEntry> > PLAYLIST_ENTRY_LIST;

class QPlaylistData
{
public:
	typedef enum
	{
		LAYER_UNKNOWN,
		LAYER_Video,
		LAYER_Browser,
	}LAYER_TYPE;
	int m_keydownflag;
public:
	QPlaylistData();
	virtual ~QPlaylistData();
	virtual INT_t LoadFromXmlFile(QString * pstrFilePath);
	virtual INT_t LoadFromIniFile(CONST QString * pstrFilePath);
	QSharedPointer <QPlaylistEntry> MoveToNextPlayEntry(BOOL bMoveNext = true);
	QSharedPointer <QPlaylistEntry> MoveToFirstPlayEntry();
	QSharedPointer <QPlaylistEntry> MoveToPreHeadPlayEntry();
	QSharedPointer <QPlaylistEntry> CurrPlayEntry();
	int getCurPlayEntryId();
	int PlayEntryCount();
	BOOL_t IsLastEntry();
	void ClearPlayListData();
	BOOL isEmpty();
	QString getCurrentPlayListName();
	INT_t insertPlayEntry(const QString strActionUrl,const QString  strActionType = ACTION_TYPE_DESC_Video,QString strOtherPar = "",int iEntryIndex = 0);
	INT_t removePlayEntry(int iEntryIndex = 0);
private:
	LAYER_TYPE getLayerTypeFromDesc(QString & strLayerType);
	QPlaylistEntry::ACTION_TYPE getActionTypeFromDesc(CONST QString & strAction);
private:
	QXmlDocument m_XmlDoc;
	QString m_strPlaylistVersion;
	LAYER_TYPE m_eLayerType;
	PLAYLIST_ENTRY_LIST m_PlaylistEntryList;
	int m_CurEntryId;
	QString m_strPlaylistControlName;
};

#endif	//_PLAY_LIST_DATA_H_

