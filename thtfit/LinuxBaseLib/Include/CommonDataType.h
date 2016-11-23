#ifndef	_COMMON_DATA_TYPE_H_
#define	_COMMON_DATA_TYPE_H_

#include	"BoolTypeDef.h"

typedef char CHAR;
#ifndef	_HAVE_DEFINDED_UINT8_
  typedef unsigned char UINT8;
  #define	_HAVE_DEFINDED_UINT8_
#endif	//_HAVE_DEFINDED_UINT8_
typedef unsigned char UINT8_t;
#ifndef	_HAVE_DEFINDED_UINT16_
  typedef unsigned short int UINT16;
  #define	_HAVE_DEFINDED_UINT16_
#endif	//_HAVE_DEFINDED_UINT16_

#ifndef	_HAVE_DEFINDED_UINT16_t
  typedef unsigned short int UINT16_t;
  #define	_HAVE_DEFINDED_UINT16_t
#endif	//_HAVE_DEFINDED_UINT16_t

#ifndef	_HAVE_DEFINDED_INT16_
  typedef signed short int INT16;
  #define	_HAVE_DEFINDED_INT16_
#endif	//_HAVE_DEFINDED_INT16_

#ifndef	_HAVE_DEFINDED_INT32_
  typedef long int INT32;
  #define	_HAVE_DEFINDED_INT32_
#endif	//_HAVE_DEFINDED_INT32_

#endif	//_COMMON_DATA_TYPE_H_

