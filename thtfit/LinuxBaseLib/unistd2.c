#include "unistd2.h"
#include <sys/stat.h>
#include <sys/types.h>

int mkdir2(const char * pPathname)
{
	return mkdir(pPathname, S_IRWXU | S_IRWXG | S_IRWXO);
}

int mkdir3(const char * pPathname, mode_t DirMode)
{
	return mkdir(pPathname, DirMode);
}

