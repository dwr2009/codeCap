#ifndef	_UNISTD2_H_
#define	_UNISTD2_H_

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

#include <sys/stat.h>
#include <sys/types.h>

int mkdir2(const char * pPathname);
int mkdir3(const char * pPathname, mode_t DirMode);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_UNISTD2_H_

