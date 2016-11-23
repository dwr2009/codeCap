#ifndef configuration_H
#define configuration_H

#include <QObject>
#include <QSettings>
#include <QHash>
#include <QFile>

class configuration : public QObject
{
	Q_OBJECT

public:
	configuration(QString str_name);
	virtual ~configuration();
protected:
	QHash<QString, QString> m_hash;
	QSettings * m_settings;
	bool m_isValidConfig;
	QString mstr_file;
private:
	typedef enum
	{ 
		FILE_EXITS,
		FILE_CREATE_SUCCESS,
		FILE_OPEN_FAILE
	}FILE_TYPE;

 	QFile *file;
public:
	QString getConfigValue(QString key);
	bool isValidConfig();
	void setConfigValue(QString key, QString value);
	void removeValue(QString key);
	bool open_file();
	void close_file();
};

#endif // configuration_H
