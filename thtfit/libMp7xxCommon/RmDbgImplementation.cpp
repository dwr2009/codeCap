#if	!defined(ALLOW_OS_CODE)
#define	ALLOW_OS_CODE
#endif	//ALLOW_OS_CODE


#include <stdio.h>
#include <stdlib.h>
#if defined(__mips__)
#include <rmdef/rmdef.h>
#include <rmdef/rmmm.h>
#else
#define RMuint32 unsigned int
#define RMuint8 unsigned char
#endif
#include <string.h>

int verbose_stderr = 0;

void *RMMalloc(RMuint32 size)
{
	return malloc(size);
}

void RMFree(void *ptr)
{
	free(ptr);
}

void *RMMemset(void *buffer, RMuint8 byte, RMuint32 size)
{
	return memset(buffer, byte, size);
}

void *RMCalloc(RMuint32 elements, RMuint32 elementSize)
{
	void *ptr = RMMalloc(elements * elementSize);
	if (ptr != NULL)
		RMMemset(ptr, 0, elements * elementSize);
	return ptr;
}

void *RMMemcpy(void *dest, const void *src, RMuint32 size)
{
	return memcpy(dest, src, size);
}

