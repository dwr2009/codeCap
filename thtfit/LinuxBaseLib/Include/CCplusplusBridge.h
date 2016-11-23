#ifndef	_C_CPLUSPLUS_BRIDGE_H_
#define	_C_CPLUSPLUS_BRIDGE_H_

#ifdef	__cplusplus
#define	DECLARE_EXTERN_C_BLOCK_START	extern "C" {									
#else
#define	DECLARE_EXTERN_C_BLOCK_START
#endif	//__cplusplus

#ifdef	__cplusplus
#define	DECLARE_EXTERN_C_BLOCK_END		}									
#else
#define	DECLARE_EXTERN_C_BLOCK_END
#endif	//__cplusplus

#endif	//_C_CPLUSPLUS_BRIDGE_H_

