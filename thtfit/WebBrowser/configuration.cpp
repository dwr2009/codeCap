#include <QFile>
#include <QStringList>
#include <QApplication>
#include "configuration.h"
#include <QtDebug>
#include <ErrPrintHelper.h>

configuration::configuration(QString str_name)
{
    QString strLockFileName=str_name+".lock";
    m_isValidConfig = false;
    mstr_file = str_name;
    file  = new QFile (str_name);
    if(QFile::exists(strLockFileName))
    {
        LOG_BLINE("remove file %s\n", qPrintable(strLockFileName));
        QFile::remove(strLockFileName);
    }
}

configuration::~configuration()
{
// 	m_settings->clear();
	if (m_isValidConfig)
	{
		delete m_settings;
	}
	if(file)
	{
		file->close();
		delete file;
		file=NULL;
	}
}

bool configuration::open_file()
{
	if (QFile::exists(mstr_file))
	{
		m_isValidConfig = true;
		m_settings = new QSettings(mstr_file, QSettings::IniFormat);
		QStringList allKeys = m_settings->allKeys();
		foreach(QString key, allKeys)
		{
			//LOG_BLINE("Key=%s,Val=%s\n", qPrintable(key), qPrintable((m_settings->value(key)).toString()));
			m_hash.insert(key, (m_settings->value(key)).toString());	
		}
		return true;
	}
	else
	{		
		if (file->open(QIODevice::ReadWrite | QIODevice::Text))
		{
			m_isValidConfig = true;
			m_settings = new QSettings(mstr_file, QSettings::IniFormat);
			QStringList allKeys = m_settings->allKeys();
			foreach(QString key, allKeys)
			{
				m_hash.insert(key, (m_settings->value(key)).toString());	
			}
			return true;
		}
		else
		{
			return false;
		}
	}
}

QString configuration::getConfigValue(QString key)
{
	if (!m_isValidConfig) return "";

	QString configValue = ""; 

	if (m_hash.contains(key))
		configValue = m_hash.value(key);
	return configValue;
}

bool configuration::isValidConfig()
{
	return m_isValidConfig;
}

void configuration::setConfigValue(QString key, QString value)
{
    
    
    if (!m_isValidConfig) return;
    
    m_hash.insert(key, value);    
    m_settings->setValue(key, value);    
    m_settings->sync();
    
}

void configuration::removeValue(QString key)
{
	if (!m_isValidConfig) return;
	m_hash.remove(key);
	m_settings->remove(key);
}

void configuration::close_file()
{	
	if(file->isOpen())
	{
		 file->close();
	}
	else
	{
		return;
	}
}

