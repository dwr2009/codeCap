#ifndef	_RUA_MEMORY_H_
#define	_RUA_MEMORY_H_

#include <BaseTypeDef.h>
#include <Array.h>

typedef struct {
	int iMemBankId;
	UINT32_t uiSerialNumber;
	PVOID pMemAddr;
	UINT32_t uiSize;
}RUA_MEMORY_SNAPSHOT, *P_RUA_MEMORY_SNAPSHOT;

typedef CArray <RUA_MEMORY_SNAPSHOT, RUA_MEMORY_SNAPSHOT &> RUA_MEMORY_SNAPSHOT_COLLECTION;
	
class CRuaMemory
{
public:
	CRuaMemory();
	~CRuaMemory();
	INT get_RuaMemorySnapshot(RUA_MEMORY_SNAPSHOT_COLLECTION & RuaMemSnapshotCollection);
	BOOL_t ExistRuaMemAddr(PVOID pRuaMemAddr, int * piMemBankId);
};

#endif	//_RUA_MEMORY_H_

