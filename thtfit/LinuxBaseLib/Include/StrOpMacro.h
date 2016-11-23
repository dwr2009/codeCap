#ifndef	_STROPMACRO_H_
#define	_STROPMACRO_H_

#define	SAFE_STRNCPY(dest, src)	do{if(src){strncpy(dest, src, sizeof(dest));dest[sizeof(dest)-1]='\0';}}while(0)

#endif	//_STROPMACRO_H_

