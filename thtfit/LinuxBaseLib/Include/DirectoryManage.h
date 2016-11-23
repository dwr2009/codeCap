#ifndef	_DIRECTORY_MANAGE_H_
#define	_DIRECTORY_MANAGE_H_

#include <dirent2.h>
#include <unistd2.h>
#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

INT CreateDirectory(LPCSTR pszDirPath);
INT CreateDirectory2(LPCSTR pszDirPath, mode_t DirMode);
INT_t CreateDirIfNotExist(LPCSTR pszDirPath);

DECLARE_EXTERN_C_BLOCK_END
	
#endif	//_DIRECTORY_MANAGE_H_
