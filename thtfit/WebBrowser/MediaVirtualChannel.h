#ifndef	_MEDIA_VIRTUAL_CHANNEL_H_
#define	_MEDIA_VIRTUAL_CHANNEL_H_

#include <BaseTypeDef.h>
#include <QMap>
#include <QString>
#include <QSharedPointer>
#include <QStringList>

#define	MVC_PropDescName_Enable			"Enable"
#define	MVC_PropDescName_ALIAS			"Alias"
#define	MVC_PropDescName_LabelDispPos	"LabelDispPos"
#define	MVC_PropDescName_DispDuration	"DispDuration"
#define	MVC_PropDescName_MinChId		"MinChId"
#define	MVC_PropDescName_MaxChId		"MaxChId"

class QMediaVirtualChannel
{
public:
	class QVirChInfo
	{
	public:
		BOOL_t m_bEnable;
		QString m_strUrl;
		QString m_strAlias;
		int m_eLabelDispPos;
		int m_DispDuration;
	};
	typedef QMap < int, QSharedPointer <QVirChInfo> > VirChInfo_MAP;
	
public:
	QMediaVirtualChannel();
	virtual ~QMediaVirtualChannel();
	virtual INT_t LoadCfgFromXmlFile(CONST QString & strCfgXmlFilePath);
	virtual INT_t SaveCfgToXmlFile(CONST QString & strCfgXmlFilePath);
	virtual INT_t getVirChInfo(int iId, QSharedPointer <QVirChInfo> & VirChInfo_sp);
	virtual INT_t setVirChInfo(int iId, CONST QSharedPointer <QVirChInfo> VirChInfo_sp);
	virtual INT_t setVirChMin(int iVirChMin);
	virtual INT_t getVirChMin(int & iVirChMin);
	virtual INT_t setVirChMax(int iVirChMax);
	virtual INT_t getVirChMax(int & iVirChMax);
	virtual INT_t getVirChIdList(QStringList & strIdList);
private:
	VirChInfo_MAP m_VirChInfo_Map;
	int m_iVirChMin, m_iVirChMax;
};

#endif	//_MEDIA_VIRTUAL_CHANNEL_H_

