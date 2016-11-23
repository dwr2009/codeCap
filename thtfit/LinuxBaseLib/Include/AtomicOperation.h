#ifndef	_ATOMIC_OPERATION_H_
#define	_ATOMIC_OPERATION_H_

#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

typedef struct
{
	int Counter;
}ATOMIC_T, *P_ATOMIC_T;

#define	INITED_ATOMIC	\
	{	\
		Counter: 0,	\
	}

void InitAtomic(P_ATOMIC_T pAtomic);
int atomic_read(P_ATOMIC_T pAtomic);
void atomic_set(P_ATOMIC_T pAtomic, int iToSet);
int atomic_add_return(P_ATOMIC_T pAtomic, int iToAdd);
int atomic_sub_return(P_ATOMIC_T pAtomic, int iToSub);
	
DECLARE_EXTERN_C_BLOCK_END

#endif	//_ATOMIC_OPERATION_H_

