#ifndef	_LIST_H_
#define	_LIST_H_

#include	<BaseTypeDef.h>

#ifdef	__cplusplus
extern "C"
{
#endif

typedef struct _LIST LIST;
typedef LIST * P_LIST;

P_LIST List_new();
void List_delete(P_LIST pThis);
POSITION List_AddTail(P_LIST pThis, void * pData);
int List_GetItemCount(P_LIST pThis);
void List_RemoveAll(P_LIST pThis);
POSITION List_GetHeadPosition(P_LIST pThis);
POSITION List_GetTailPosition(P_LIST pThis);
PVOID List_GetNext(P_LIST pThis, POSITION * ppPos);
PVOID List_GetAt(P_LIST pThis, POSITION pPos);
void List_RemoveAt(P_LIST pThis, POSITION pPos);
int List_ExistTargetData(P_LIST pThis, PVOID pVoidData);

#ifdef	__cplusplus
}
#endif

#endif	//_LIST_H_

