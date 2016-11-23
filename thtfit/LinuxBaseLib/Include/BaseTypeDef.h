#ifndef	_BASETYPEDEF_H_
#define	_BASETYPEDEF_H_

#ifdef _WIN32

#else	//nodef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include	"CommonDataType.h"
#include	"SocketTypeDef.h"
#include	"BoolTypeDef.h"

typedef char INT8;

typedef char * LPSTR;
typedef const char * LPCSTR;
typedef	unsigned char * LPBYTE;
typedef	unsigned char * PBYTE;
typedef	unsigned char BYTE;
typedef unsigned short WORD;
typedef	unsigned long DWORD;
typedef DWORD * LPDWORD;

#ifndef NULL
/** pointer not defined */
#define NULL (0)
#endif // NULL

#ifndef	_HAVE_DEFINED_COLORREF_
  typedef	DWORD COLORREF;
  #define	_HAVE_DEFINED_COLORREF_
#endif	//_HAVE_DEFINED_COLORREF_

#ifndef	_HAVE_DEFINED_INT8_t_
	typedef INT8 INT8_t;
	#define UINT8_t unsigned char
	#define	_HAVE_DEFINED_INT8_t_
#endif	

typedef	int	INT;
typedef	unsigned int UINT;

#ifndef	_HAVE_DEFINED_INT16_t
#define	_HAVE_DEFINED_INT16_t
	typedef signed short int INT16_t;
#endif	//_HAVE_DEFINED_INT16_t

#ifndef	_HAVE_DEFINED_INT_t
	#define _HAVE_DEFINED_INT_t
	typedef INT INT_t;
#endif	//_HAVE_DEFINED_INT_t

#ifndef	_HAVE_DEFINED_UINT_t
	#define _HAVE_DEFINED_UINT_t
	typedef UINT UINT_t;
#endif	//_HAVE_DEFINED_UINT_t

#ifndef	_HAVE_DEFINED_UINT32_
  typedef UINT UINT32;
  #define	_HAVE_DEFINED_UINT32_
#endif	//_HAVE_DEFINED_UINT32_  
typedef	unsigned int UINT32_t;

#define	CONST	const

typedef	int	INT32_t;

typedef INT * PINT;
typedef INT * P_INT;
typedef unsigned int * P_UINT;

typedef	long		LONG;
#ifdef	_WIN32
typedef __int64	LONGLONG;
#else	//nodef _WIN32
typedef long long LONGLONG;
#ifndef	_HAVE_DEFINED_UINT64_
	typedef unsigned long long UINT64;
	#define	_HAVE_DEFINED_UINT64_
#endif
#endif
typedef unsigned long ULONG;
#ifndef	_HAVE_DEFINED_ULONG_t
	#define	_HAVE_DEFINED_ULONG_t
	typedef ULONG ULONG_t;
#endif	//_HAVE_DEFINED_ULONG_t

#ifndef	_HAVE_DEFINED_UINT64_t
	#define	_HAVE_DEFINED_UINT64_t
	typedef unsigned long long UINT64_t;
#endif	//_HAVE_DEFINED_UINT64_t

#ifndef	_HAVE_DEFINED_INT64_t
	#define	_HAVE_DEFINED_INT64_t
	typedef signed long long INT64_t;
#endif	//_HAVE_DEFINED_INT64_t

#ifdef	__cplusplus
  #ifndef	_HAVE_DEFINED_VOID_
    #define	VOID	void
    #define	_HAVE_DEFINED_VOID_
  #endif	//_HAVE_DEFINED_VOID_
#else	//nodef __cplusplus
  #ifndef	_HAVE_DEFINED_VOID_
    typedef void VOID;
    #define	_HAVE_DEFINED_VOID_
  #endif	//_HAVE_DEFINED_VOID_
#endif	//__cplusplus

typedef void * PVOID;
#ifndef	_HAVE_DEFINED_HANDLE
	typedef PVOID HANDLE;
	#define	_HAVE_DEFINED_HANDLE
#endif	//_HAVE_DEFINED_HANDLE
typedef PVOID POSITION;

typedef struct sockaddr_in		SOCKADDR_IN;
typedef	struct sockaddr		SOCKADDR;
typedef SOCKADDR * P_SOCKADDR;
typedef SOCKADDR_IN * P_SOCKADDR_IN;

#define INVALID_SOCKET  (SOCKET)(~0)

#ifndef	TRUE
#define	TRUE	1	
#endif

#ifndef	FALSE
#define	FALSE	0
#endif

#define	INVALID_FILE_DESCRIPTOR				(-1)
#define	INV_FILE_DESC						(-1)

#define	IN
#define	OUT

#define	PRIVATE		static

#ifdef	ANDROID
	#define	interface	class
#else	//!ANDROID
	#define	INTERFACE	class
#endif

#define	GENERALIZE_PERCENT(Value)		if(0 > Value) {Value = 0;} else if(100 < Value) {Value = 100;}

typedef struct _GUID {          // size is 16
    DWORD Data1;
    WORD   Data2;
    WORD   Data3;
    BYTE  Data4[8];
} GUID;

#endif

