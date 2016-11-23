#include	"memory.h"
#include	<ErrPrintHelper.h>
#include	<BaseTypeDef.h>

void ZeroMemory(void * pMemAddr, size_t sizeMem)
{
	do{
		if(NULL == pMemAddr)
		{
			PRINT_FILE_LINENO_BUG_STR;
			break;
		}

		memset(pMemAddr, 0x00, sizeMem);
	}while(FALSE);
}

