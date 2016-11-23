#ifndef	_SMP865X_SOFT_FLOAT_H_
#define	_SMP865X_SOFT_FLOAT_H_

#include <CCplusplusBridge.h>

//libgcc soft-float declaration
extern float __addsf3 (float a, float b);
extern double __extendsfdf2 (float a);
extern int __fixsfsi (float a);
extern float __floatsisf(int i);

DECLARE_EXTERN_C_BLOCK_START

float __addsf4 (float a, float b);
double __extendsfdf3 (float a);
int __fixsfs2 (float a);
float __floatsis2(int i);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_SMP865X_SOFT_FLOAT_H_

