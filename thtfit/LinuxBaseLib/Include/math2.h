#ifndef	_MATH2_H_
#define	_MATH2_H_

#include	<BaseTypeDef.h>

#ifdef	__cplusplus
extern "C"
{
#endif	//__cplusplus

#define	GET_MIN_VAL(a, b)			((a)<(b)?(a):(b))

int GetMinVal(int a, int b);
UINT getMinVal_UINT(UINT a, UINT b);
int GetMaxVal(int a, int b);
INT ConvertValFromStr(LPCSTR pszStr, UINT * puiVal/*IN OUT*/);
BOOL_t IsInRange(CONST UINT32_t uiTest, CONST UINT32_t uiMinVal, CONST UINT32_t uiMaxVal);
BOOL_t IsNotInRange(CONST UINT32_t uiTest, CONST UINT32_t uiMinVal, CONST UINT32_t uiMaxVal);

#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_MATH2_H_

