#include "SoftFloatBridge.h"
#include <string.h>

UINT32_t SoftFloatBridge_Add2(UINT32_t uiA, UINT32_t uiB)
{
	float fA, fB, fC;
	UINT32_t uiC;

	memcpy(&fA, &uiA, 4);
	memcpy(&fB, &uiB, 4);	

	fC = fA + fB;
	memcpy(&uiC, &fC, 4);

	return uiC;
}

UINT32_t SoftFloatBridge_ToInt(UINT32_t uiA)
{
	float fA;

	memcpy(&fA, &uiA, 4);
	
	return fA;
}

UINT32_t SoftFloatBridge_IntToFloat(UINT32_t uiA)
{
	float fOutRet;
	UINT32_t uiOutRet;

	fOutRet = uiA;
	memcpy(&uiOutRet, &fOutRet, sizeof(fOutRet));

	return uiOutRet;
}

VOID SoftFloatBridge_FloatToDouble(UINT32_t uiA, UINT64_t * fDest)
{
	float fIn;
	double fOutRet;

	memcpy(&fIn, &uiA, sizeof(fIn));
	fOutRet = fIn;

	memcpy(fDest, &fOutRet, sizeof(fOutRet));
}

