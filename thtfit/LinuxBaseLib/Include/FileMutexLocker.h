#ifndef	_FILE_MUTEX_LOCKER_H_
#define	_FILE_MUTEX_LOCKER_H_

#include <BaseTypeDef.h>
#include <String.h>

class CFileMutexLocker
{
public:
	CFileMutexLocker(LPCSTR pszLockerFile);
	~CFileMutexLocker();
	int Lock();
	int Unlock();
	int GetLastErrNo();
	int GetLastCrtErrNo();
private:
	int m_iFdLock;
	int m_iLastErrNo;
	int m_iLastCrtErrNo;
	BOOL_t m_bCreatedLockerFile;
	CString m_strLockerFilePath;
	pid_t m_pidHold;
	UINT_t m_uiLockCount;
};

#endif	//_FILE_MUTEX_LOCKER_H_

