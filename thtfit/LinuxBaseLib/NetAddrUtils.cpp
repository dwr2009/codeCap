#include "Include/NetAddrUtils.h"
#include <NetIF.h>

CString getIpAddrStrFromBin(UINT32_t uiIpAddr)
{
	INT_t iRet;
	CString strIpAddr;

	do
	{
		iRet = strIpAddr.SetStrLength(32);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
		iRet = convertIPv4Addr_BinToStr(uiIpAddr, strIpAddr.GetBufPtr(), strIpAddr.GetBufLength());
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
	}while(FALSE);

	return strIpAddr;
}

