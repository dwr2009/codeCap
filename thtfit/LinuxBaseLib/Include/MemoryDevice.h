#ifndef	_MEMORY_DEVICE_H_
#define	_MEMORY_DEVICE_H_

#include <BaseTypeDef.h>
#include <CrtErr.h>

#define	MEMORY_DEVICE_DEV_PATH		"/dev/mem"

class CMemoryDevice : public CCrtErr
{
public:
	CMemoryDevice();
	virtual ~CMemoryDevice();
	virtual INT_t ReadMemory(void * pMemAddr, size_t sizeToRead, PBYTE pDataBuf);	
protected:
	virtual VOID UnmapCur();
protected:
	BOOL_t mInited;
	int mMemDevFd;
	int mMemoryPageSize;
	size_t mMappedSize;
	int mMappedProtectFlag;
	off_t mMappedMemPhyAddr;
	void * mMappedMemVirAddr;
};

class CMemDevMapper : public CMemoryDevice
{
public:
	CMemDevMapper();
	virtual ~CMemDevMapper();
	virtual INT_t ReadPhyMem(PVOID pPhyMemAddr, size_t sizeToRead, PBYTE pDataBuf);
	virtual INT_t MapPhyMem(PVOID pPhyMemAddr, size_t sizeToMap, PVOID * ppMappedVirAddr);
	virtual PVOID getMappedVirAddr();
};
	
#endif	//_MEMORY_DEVICE_H_

