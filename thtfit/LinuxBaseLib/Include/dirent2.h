#ifndef	_DIRENT2_H_
#define	_DIRENT2_H_

#include	<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <BaseTypeDef.h>

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

int ExistFile(const char * pFilepath, int * piExist);
INT_t ExistDir(LPCSTR pszDirPath, BOOL_t * pbExist);
int GetFileLength(const char * pFilePath, off_t * pllFilSize);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_DIRENT2_H_

