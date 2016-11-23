#ifndef	_GLOBALSHAREMEM_H_
#define	_GLOBALSHAREMEM_H_

#include <sys/ipc.h>
#include <BaseTypeDef.h>

typedef struct
{
	LPCSTR pszFilePath;
	int iId;
}IPC_KEY_VAL_IN, *P_IPC_KEY_VAL_IN;

class CGlobalShareMem
{
public:
	CGlobalShareMem();
	~CGlobalShareMem();
	int CreateOrGetShareMem(LPCSTR pszKeyFilePath, int iSize);
	INT CreateOrGetShareMem(CONST P_IPC_KEY_VAL_IN pIpcKeyValIn, CONST INT iSize);
	int CreateOrGetShareMem(key_t iKey, int iSize);
	void * GetShareMemAddr();
	bool GetIsCreated();
	int DetachOrDestroyShareMem();
	int GetLastCrtErrNo();
	INT_t setRemoveShmIdIfCreatedByMe(CONST BOOL_t bNeedRemove);
private:
	int m_iShmId;
	BOOL m_bCreated;
	BOOL_t m_bRemoveShmIdIfCreatedByMe;
	void * m_pShmAddr;
	int m_iLastCrtErrNo;
};

#endif	//_GLOBALSHAREMEM_H_

