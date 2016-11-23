#ifndef	_MEMORY_MAP_FILE_H_
#define	_MEMORY_MAP_FILE_H_

#include <BaseTypeDef.h>

class CMemoryMapFile
{
public:
	typedef enum
	{
		MAX_SIZE_TO_MAP = (8*1024*1024),
	}RESOURCE_LIMITATION;
public:
	CMemoryMapFile(LPCSTR pszFilePath, size_t FileSizeIfNeedCreate);
	virtual ~CMemoryMapFile();
	virtual void * getMappedAddr();
	virtual INT_t Lock();
	virtual INT_t Unlock();
	virtual BOOL_t FileIsCreatedByMe();
protected:
	VOID UnmapFile();
	VOID CloseFile();
protected:
	int m_iMapFileFd;
	void * m_pMappedAddr;
	INT_t m_iLastErrNo;
	BOOL_t m_bFileIsMapped;
	size_t m_FileSizeToMap;
	BOOL_t m_bFileIsCreatedByMe;
};

#endif	//_MEMORY_MAP_FILE_H_

