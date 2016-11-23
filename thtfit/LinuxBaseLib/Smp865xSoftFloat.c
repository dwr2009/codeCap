#include <Smp865xSoftFloat.h>
#include <ErrPrintHelper.h>
#include "SoftFloatBridge.h"
#include <string.h>

/*
If FPU exists and is disabled, the return value of soft float is put into FLOAT regs($f0).
If NO FPU, the return value of soft float is put into NORMAL regs($v0).
*/

//#define	PRINT_DBG_INFO

float __addsf4 (float a, float b)
{
	float fSum = 0;
	UINT32_t uiA, uiB, uiC;
#ifdef	PRINT_DBG_INFO
	unsigned int uiData_a, uiData_b, uiData_Sum;
#endif

#ifdef	PRINT_DBG_INFO
	memcpy(&uiData_a, &a, 4);
	memcpy(&uiData_b, &b, 4);	
#endif
	memcpy(&uiA, &a, 4);
	memcpy(&uiB, &b, 4);		
	
	uiC = SoftFloatBridge_Add2(uiA, uiB);
	memcpy(&fSum, &uiC, 4);

#ifdef	PRINT_DBG_INFO
	memcpy(&uiData_Sum, &fSum, 4);
	PRINT_BFILE_LINENO;
	printf("__addsf4, a=%f, m=0x%08x, b=%f, m=0x%08x, sum=%f, m=0x%08x\n", a, uiData_a, b, uiData_b, fSum,
		uiData_Sum);
#endif

	return fSum;
}

double __extendsfdf3 (float a)
{
	double fOutRet;
	UINT32_t uiA;
	UINT64_t uiOutRet;
	
	memcpy(&uiA, &a, sizeof(uiA));
	SoftFloatBridge_FloatToDouble(uiA, &uiOutRet);
	memcpy(&fOutRet, &uiOutRet, sizeof(fOutRet));

	return fOutRet;
}

int __fixsfs2 (float a)
{
	int iOutRet = 0;
	UINT32_t uiA;

	memcpy(&uiA, &a, 4);
	iOutRet = SoftFloatBridge_ToInt(uiA);

#ifdef	PRINT_DBG_INFO
	PRINT_BFILE_LINENO;
	printf("__fixsfs2, a=%f, iRet=%d\n", a, iOutRet);
#endif

	return iOutRet;
}

float __floatsis2(int i)
{
	UINT32_t uiOutRet;
	float fOutRet;
	
	uiOutRet = SoftFloatBridge_IntToFloat(i);
	memcpy(&fOutRet, &uiOutRet, sizeof(fOutRet));

	return fOutRet;
}

