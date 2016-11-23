#include "Api/DevConnectionStatus.h"
#include "Api/AppFilePathManage.h"
#include <BaseErrDef.h>
#include <Mp7xxGlobalStatus.h>
#include <unistd.h>
#include <StackBufString.h>
#include <SysBaseSrvApi.h>

INT_t getDevMountStatus_Usb(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszUsbSymLinkFilePath = "";

	do
	{
		iRet = get_UsbIsMounted(pbMounted);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(pbMounted && *pbMounted)	//mounted
		{
			pszUsbSymLinkFilePath = Mp7xx_getMountSymLinkFilePath(Mp7xxCommon::MOUNT_DEV_USB);
			if(NULL == pszUsbSymLinkFilePath || '\0' == pszUsbSymLinkFilePath[0])
			{
				*pbMounted = FALSE;
				break;
			}
			if(0 != access(pszUsbSymLinkFilePath, F_OK))	//file can not be accessed
			{
				*pbMounted = FALSE;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t getDevMountStatus_Sd(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszSdSymLinkFilePath = "";

	do
	{
		iRet = get_SdIsMounted(pbMounted);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(pbMounted && *pbMounted)	//mounted
		{
			pszSdSymLinkFilePath = Mp7xx_getMountSymLinkFilePath(Mp7xxCommon::MOUNT_DEV_SD);
			if(NULL == pszSdSymLinkFilePath || '\0' == pszSdSymLinkFilePath[0])
			{
				*pbMounted = FALSE;
				break;
			}
			if(0 != access(pszSdSymLinkFilePath, F_OK))	//file can not be accessed
			{
				*pbMounted = FALSE;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t getDevMountStatus_Hdd(BOOL_t * CONST pbMounted)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	LPCSTR pszHddSymLinkFilePath = "";

	do
	{
		iRet = get_HddIsMounted(pbMounted);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(pbMounted && *pbMounted)	//mounted
		{
			pszHddSymLinkFilePath = Mp7xx_getMountSymLinkFilePath(Mp7xxCommon::MOUNT_DEV_HDD);
			if(NULL == pszHddSymLinkFilePath || '\0' == pszHddSymLinkFilePath[0])
			{
				*pbMounted = FALSE;
				break;
			}
			if(0 != access(pszHddSymLinkFilePath, F_OK))	//file can not be accessed
			{
				*pbMounted = FALSE;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t UpdateDeviceMountStatusFromSysProp()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strPropVal, 32);

	do
	{
		//usb
		iRet = SysProp_get(SysProp_KEY_DevConn_UsbMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = set_UsbIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = set_UsbIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//sd
		iRet = SysProp_get(SysProp_KEY_DevConn_SdMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = set_SdIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = set_SdIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//Hdd
		iRet = SysProp_get(SysProp_KEY_DevConn_HddMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = set_HddIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = set_HddIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iOutRet;
}

