#include	"dirent2.h"
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include <stdlib.h>
#include	"BaseErrDef.h"
#include	"BaseTypeDef.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ErrPrintHelper.h>

int ExistFile(const char * pFilepath, int * piExist)
{
	int iLen = 0, iOutRet = 0, iExist = 0;
	char *pDir = NULL, *pFilename = NULL, *pRet = NULL;

	do{
		iLen = strlen(pFilepath);
		//定位最后文件名分隔符
		pRet = strrchr(pFilepath, '/');
		if(pRet)
		{
			int iDirLen = 0, iFilenameLen = 0;
			
			iDirLen = pRet-pFilepath;
			if(0 < iDirLen)
			{
				pDir = malloc(iDirLen+1);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				memcpy(pDir, pFilepath, iDirLen);
				pDir[iDirLen] = '\0';
			}
			else if(0 == iDirLen && '/' == pFilepath[0])
			{
				pDir = malloc(2);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				pDir[0] = '/'; pDir[1] = '\0';
			}
			else
			{
				pDir = malloc(2);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				pDir[0] = '.'; pDir[1] = '\0';
			}

			iFilenameLen = iLen - iDirLen - 1;
			if(0 < iFilenameLen)
			{
				pFilename = malloc(iFilenameLen+1);
				if(NULL == pFilename)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}		
				strcpy(pFilename, pRet+1);
			}
			else
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else
		{
			pDir = malloc(2);
			if(NULL == pDir)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			pDir[0] = '.'; pDir[1] = '\0';

			pFilename = malloc(iLen+1);
			if(NULL == pFilename)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}			
			strcpy(pFilename, pFilepath);
		}

		{
			struct dirent **ppNameList = NULL;
			int iRet = 0, iId = 0;

			iRet = scandir(pDir, &ppNameList, 0, alphasort);
			if(-1 == iRet)
			{
				if(ENOENT != errno)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_CALL_SYSTEM_FAILED;
				}
				break;
			}

			for(iId = 0; iId < iRet; iId++)
			{
				if(0 == strcmp(ppNameList[iId]->d_name, pFilename))
				{
					iExist = 1;
					break;
				}
			}
			for(iId = 0; iId < iRet; iId++)
			{
				free(ppNameList[iId]);
				ppNameList[iId] = NULL;
			}
			free(ppNameList);
			ppNameList = NULL;
		}
	}while(FALSE);

	if(pDir)
	{
		free(pDir); pDir = NULL;
	}

	if(pFilename)
	{
		free(pFilename); pFilename = NULL;
	}
	
	*piExist = iExist;

	return iOutRet;
}

INT_t ExistDir(LPCSTR pszDirPath, BOOL_t * pbExist)
{
	INT_t iOutRet = ERROR_SUCCESS, iLen = 0;
	BOOL_t bExist = FALSE;
	char *pDir = NULL, *pFilename = NULL, *pRet = NULL;

	do
	{
		if(NULL == pszDirPath)
		{
			break;
		}
		if('\0' == pszDirPath[0])
		{
			break;
		}
		if(NULL == pbExist)
		{
			break;
		}
		iLen = strlen(pszDirPath);
		//定位最后文件名分隔符
		pRet = strrchr(pszDirPath, '/');
		if(pRet)
		{
			int iDirLen = 0, iFilenameLen = 0;
			
			iDirLen = pRet-pszDirPath;
			if(0 < iDirLen)
			{
				pDir = malloc(iDirLen+1);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				memcpy(pDir, pszDirPath, iDirLen);
				pDir[iDirLen] = '\0';
			}
			else if(0 == iDirLen && '/' == pszDirPath[0])
			{
				pDir = malloc(2);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				pDir[0] = '/'; pDir[1] = '\0';
			}
			else
			{
				pDir = malloc(2);
				if(NULL == pDir)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}
				pDir[0] = '.'; pDir[1] = '\0';
			}

			iFilenameLen = iLen - iDirLen - 1;
			if(0 < iFilenameLen)
			{
				pFilename = malloc(iFilenameLen+1);
				if(NULL == pFilename)
				{
					printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
					iOutRet = ERROR_OUT_OF_MEMORY;
					break;
				}		
				strcpy(pFilename, pRet+1);
			}
			else
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
		else
		{
			pDir = malloc(2);
			if(NULL == pDir)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			pDir[0] = '.'; 
			pDir[1] = '\0';

			pFilename = malloc(iLen+1);
			if(NULL == pFilename)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}			
			strcpy(pFilename, pszDirPath);
		}

		{
			struct dirent **ppNameList = NULL;
			int iRet = 0, iId = 0;

			iRet = scandir(pDir, &ppNameList, 0, alphasort);
			if(-1 == iRet)
			{
				printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
				iOutRet = ERROR_CALL_SYSTEM_FAILED;
				break;
			}

			for(iId = 0; iId < iRet; iId++)
			{
				//LOG_BLINE("Filename:%s,type=%d\n", ppNameList[iId]->d_name, ppNameList[iId]->d_type);
				if(0 == strcmp(ppNameList[iId]->d_name, pFilename) && DT_DIR == ppNameList[iId]->d_type)
				{
					bExist = TRUE;
					break;
				}
			}
			for(iId = 0; iId < iRet; iId++)
			{
				free(ppNameList[iId]);
				ppNameList[iId] = NULL;
			}
			free(ppNameList);
			ppNameList = NULL;
		}
	}while(FALSE);

	if(pDir)
	{
		free(pDir); 
		pDir = NULL;
	}

	if(pFilename)
	{
		free(pFilename); 
		pFilename = NULL;
	}

	if(pbExist)
	{
		*pbExist = bExist;
	}

	return iOutRet;
}

int GetFileLength(LPCSTR pFilePath, off_t * pllFilSize)
{
	int iOutRet = 0, iRet = 0;
	struct stat oSrcFileStat;
	off_t llFileLen = 0;
	
	do{
		if(NULL == pFilePath || NULL == pllFilSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iRet = stat(pFilePath, &oSrcFileStat);
		if(-1 == iRet)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;
			break;
		}
		
		llFileLen = oSrcFileStat.st_size;

		if(0 == llFileLen && S_ISBLK(oSrcFileStat.st_mode))
		{
			int iFdFile = -1;
			off_t lFileOffset = 0;
			int bSuccToGetLen = 0;
			
			do{
				iFdFile = open(pFilePath, O_RDONLY);
				if(-1 == iFdFile)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					break;
				}
				lFileOffset = lseek(iFdFile, 0, SEEK_END);
				if(-1 == lFileOffset)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					break;
				}
				llFileLen = lFileOffset;
				bSuccToGetLen = 1;
			}while(0);
			
			if(-1 != iFdFile)
			{
				iRet = close(iFdFile);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
				}
				iFdFile = -1;
			}

			if(0 == bSuccToGetLen)
			{
				iOutRet = ERROR_FILE_STAT_FAIL;
				break;
			}
		}

		*pllFilSize = llFileLen;
	}while(0);
	
	return iOutRet;
}

