#ifndef	_COMPILER_H_
#define	_COMPILER_H_

#if	defined(ANDROID)

#include <cutils/compiler.h>

#else	//!ANDROID

/*
 * helps the compiler's optimizer predicting branches
 */

#ifdef __cplusplus
#   define CC_LIKELY( exp )    (__builtin_expect( !!(exp), true ))
#   define CC_UNLIKELY( exp )  (__builtin_expect( !!(exp), false ))
#else
#   define CC_LIKELY( exp )    (__builtin_expect( !!(exp), 1 ))
#   define CC_UNLIKELY( exp )  (__builtin_expect( !!(exp), 0 ))
#endif

#endif	//ANDROID

#define	C_UNUSED(x)		{(void)x;}

#endif	//_COMPILER_H_

