#include "DirectoryManage.h"
#include <BaseErrDef.h>
#include <errno.h>

INT CreateDirectory(LPCSTR pszDirPath)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	int bExist = FALSE;

	do{
		if(NULL == pszDirPath)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = ExistFile(pszDirPath, &bExist);
		if(ERROR_SUCCESS != iRet )
		{
			iOutRet = iRet;
			break;
		}
		
		if(FALSE == bExist)
		{
			iRet = mkdir2(pszDirPath);
			if(-1 == iRet)
			{
				iOutRet = iRet;
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}

INT CreateDirectory2(LPCSTR pszDirPath, mode_t DirMode)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	int bExist = FALSE;

	do{
		if(NULL == pszDirPath)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = ExistFile(pszDirPath, &bExist);
		if(ERROR_SUCCESS != iRet )
		{
			iOutRet = iRet;
			break;
		}
		
		if(FALSE == bExist)
		{
			iRet = mkdir3(pszDirPath, DirMode);
			if(-1 == iRet)
			{
				iOutRet = iRet;
				break;
			}
		}		
	}while(FALSE);

	return iOutRet;
}

INT_t CreateDirIfNotExist(LPCSTR pszDirPath)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bExist;

	do
	{
		bExist = FALSE;
		iRet = ExistDir(pszDirPath, &bExist);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(bExist)
		{
			break;
		}
		iRet = mkdir(pszDirPath, S_IRWXU);
		if(0 != iRet && EEXIST == errno)
		{
		}
		else if(0 != iRet)
		{
			iOutRet = ERR_CREATE_DIR_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

