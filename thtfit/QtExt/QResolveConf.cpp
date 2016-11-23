#include "QResolveConf.h"
#include <BaseErrDef.h>
#include <QSharedPointer>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

#define	KEY_DOMAIN				"domain"
#define	KEY_NAMESERVER			"nameserver"
#define	KEY_SEARCH				"search"

const char * QResolveConf::RESOLVE_CONF_FILE = "/etc/resolv.conf";

QResolveConf::QResolveConf()
{
}

QResolveConf::~QResolveConf()
{
}

INT_t QResolveConf::Reset()
{
	m_strDomain.clear();
	m_listNameServer.clear();
	
	return ERROR_SUCCESS;
}

INT_t QResolveConf::ReloadConf()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	bool bRet;
	QSharedPointer <QFile> ResolveFile_sp(new QFile(RESOLVE_CONF_FILE));
	QSharedPointer <QTextStream> ResolveStream_sp;

	do
	{		
		if(ResolveFile_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = Reset();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		bRet = ResolveFile_sp->open(QIODevice::ReadOnly);
		if(false == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		ResolveStream_sp = QSharedPointer <QTextStream> (new QTextStream(&(*ResolveFile_sp)));
		if(ResolveStream_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		QString OneLineContent;
		while(TRUE)
		{
			OneLineContent = ResolveStream_sp->readLine();
			if(OneLineContent.isNull())
			{
				break;
			}
			//qDebug() << "OneLineContent:" << OneLineContent;
			QStringList WordList = OneLineContent.split(' ');
			//qDebug() << "WordList:" << WordList;
			if(1 <= WordList.count())
			{
				if(KEY_DOMAIN == WordList[0])
				{
					if(2 <= WordList.count())
					{
						m_strDomain = WordList[1];
					}
				}
				else if(KEY_NAMESERVER == WordList[0])
				{
					if(2 <= WordList.count())
					{
						m_listNameServer << WordList[1];
					}
				}
				else if(KEY_SEARCH == WordList[0])
				{
					if(2 <= WordList.count())
					{
						m_strSearch = WordList[1];
					}
				}
			}
		}
	}while(FALSE);

	ResolveStream_sp.clear();
	if(FALSE == ResolveFile_sp.isNull())
	{
		ResolveFile_sp->close();
	}
	ResolveFile_sp.clear();

	return iOutRet;
}

INT_t QResolveConf::clearNameServer()
{
	m_listNameServer.clear();
	
	return ERROR_SUCCESS;
}

INT_t QResolveConf::setDomainName(const QString & strDomainName)
{
	INT_t iOutRet = ERROR_SUCCESS;

	do
	{
		m_strDomain = strDomainName;
	}while(FALSE);

	return iOutRet;
}

INT_t QResolveConf::setNameServer(QStringList & listNameServer)
{
	m_listNameServer = listNameServer;

	return ERROR_SUCCESS;
}

INT_t QResolveConf::SaveConf()
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	QSharedPointer <QFile> ResolveFile_sp(new QFile(RESOLVE_CONF_FILE));
	QSharedPointer <QTextStream> ResolveStream_sp;

	do
	{
		if(ResolveFile_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = ResolveFile_sp->open(QIODevice::WriteOnly);
		if(false == bRet)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		ResolveStream_sp = QSharedPointer <QTextStream> (new QTextStream(&(*ResolveFile_sp)));
		if(ResolveStream_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		if(FALSE == m_strDomain.isEmpty())
		{
			//qDebug() << "m_strDomain:" << m_strDomain;
			(*ResolveStream_sp) << KEY_DOMAIN << " " << m_strDomain << "\n";
		}
		if(FALSE ==  m_strSearch.isEmpty())
		{
			//qDebug() << "m_strSearch:" << m_strSearch;
			(*ResolveStream_sp) << KEY_SEARCH << " " << m_strSearch << "\n";
		}
		foreach(const QString & strNameServer, m_listNameServer)
		{
			if(FALSE ==  strNameServer.isEmpty())
			{
				//qDebug() << "strNameServer:" << strNameServer;
				(*ResolveStream_sp) << KEY_NAMESERVER << " " << strNameServer << "\n";
			}
		}
	}while(FALSE);

	ResolveStream_sp.clear();
	if(FALSE == ResolveFile_sp.isNull())
	{
		ResolveFile_sp->close();
	}
	ResolveFile_sp.clear();

	return iOutRet;
}

