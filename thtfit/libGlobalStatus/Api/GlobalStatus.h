#ifndef	_GLOBAL_STATUS_H_
#define	_GLOBAL_STATUS_H_

#include <String.h>
#include <MemoryMapFile.h>

class CGlobalStatus
{
public:
	CGlobalStatus(LPCSTR pszStatusFilePath, size_t sizeStatusData);
	virtual ~CGlobalStatus();
protected:
	CString m_strStatusFilePath;
	CMemoryMapFile m_StatusMapFile;
};

#endif	//_GLOBAL_STATUS_H_

