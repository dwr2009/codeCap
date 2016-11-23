#include <SmartCard.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <string.h>

int main(int argc, char * argv[])
{
	P_SMART_CARD_INST pSmartCardInst = NULL;
	INT iRet = 0;

	do
	{
		float fTime, fTime2, fSum;
		unsigned int uiTime, uiTime2, uiSum;
		uiTime = 0x4e61b53a;
		uiTime2 = 0x439f3333;
		memcpy(&fTime, &uiTime, 4);
		memcpy(&fTime2, &uiTime2, 4);
		fSum = fTime + fTime2;
		memcpy(&uiSum, &fSum, 4);
		printf("fTime=%f, fTime2=%f, fSum=%f, fSum_mem=0x%08x\n", fTime, fTime2, fSum, uiSum);
		break;
		
		iRet = SmartCard_New(&pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}
		if(NULL == pSmartCardInst)
		{
			PRINT_BFILE_LINENO;
			printf("BUG\n");
			break;
		}
		SCARD_SEL_MATCH_PARAM oSCardSelMatchParam = {0, };
		oSCardSelMatchParam.bSCardInstId_Valid = TRUE;
		oSCardSelMatchParam.iSCardInstId = 0;
		iRet = SmartCard_Open(pSmartCardInst, &oSCardSelMatchParam);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}		
		iRet = SmartCard_PingPong(pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
			break;
		}		
	}while(FALSE);

	if(pSmartCardInst)
	{
		iRet = SmartCard_Close(pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		iRet = SmartCard_Del(pSmartCardInst);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_CRT_ERR;
		}
		pSmartCardInst = NULL;
	}

	return 0;
}

