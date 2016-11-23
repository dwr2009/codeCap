#ifndef	_Q_TAR_PACKAGE_READER_H_
#define	_Q_TAR_PACKAGE_READER_H_

#include <BaseTypeDef.h>
#include <QStringList>

class QTarPackageReader
{
public:
	QTarPackageReader();
	virtual ~QTarPackageReader();
	virtual INT_t OpenTarFile(const QString & strTarFilePath);
	virtual INT_t CloseTarFile();
private:
	BOOL_t m_bFileIsOpened;
	QStringList m_FileListInTarFile;
};

#endif	//_Q_TAR_PACKAGE_READER_H_

