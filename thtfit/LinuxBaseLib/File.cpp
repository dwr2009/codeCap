#include	"File.h"
#include	<ErrPrintHelper.h>
#include	<BaseErrDef.h>
#include	<stdio.h>
#include	<errno.h>
#include	<string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

CFile::CFile()
{
	m_fpFile = NULL;
	m_iCrtErrNo = 0;
}

CFile::~CFile()
{
	int iRet = 0;
	
	if(m_fpFile)
	{
		iRet = fclose(m_fpFile);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_fpFile = NULL;
	}
}

INT CFile::Open(LPCSTR pszFilePath, LPCSTR pszMode)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	m_iCrtErrNo = 0;

	if(NULL == pszFilePath || NULL == pszMode) {return ERROR_INVALID_PARAMETER;}

	do{
		if(m_fpFile)
		{
			iRet = fclose(m_fpFile);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			m_fpFile = NULL;
		}

		m_fpFile = fopen(pszFilePath, pszMode);
		if(NULL == m_fpFile)
		{
			m_iCrtErrNo = errno;
			//PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}		
	}while(false);

	return iOutRet;
}

INT CFile::FlushFileStream()
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	do{
		m_iCrtErrNo = ERROR_SUCCESS;

		if(NULL == m_fpFile)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		iRet = fflush(m_fpFile);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERROR_CALL_SYSTEM_FAILED;
			m_iCrtErrNo = errno;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CFile::Close()
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;

	if(m_fpFile)
	{
		iRet = fclose(m_fpFile);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		m_fpFile = NULL;
	}	

	return iOutRet;
}

INT CFile::ReadLine(LPBYTE pBuf, INT iBufSize, UINT uiOptions/*0*/)
{
	INT iOutRet = 0;
	char * pszRet = NULL;

	if(NULL == m_fpFile) {return ERROR_INVALID_STATE;}
	if(NULL == pBuf || 0 >= iBufSize) {return ERROR_INVALID_PARAMETER;}

	pszRet = fgets((char *)pBuf, iBufSize, m_fpFile);
	if(NULL == pszRet)
	{
		m_iCrtErrNo = errno;
		if(ERROR_SUCCESS == m_iCrtErrNo || feof(m_fpFile))
		{
			return ERR_END_OF_FILE;
		}
		else
		{
			return ERROR_FILE_READ_FAIL;
		}
	}

	if(OPT_TRIM_TAILING_NEWLINE & uiOptions)
	{
		INT iLen = 0;
		iLen = strlen((char *)pBuf);
		if(1 <= iLen)
		{
			if('\r' == pBuf[iLen-1] || '\n' == pBuf[iLen-1])
			{
				pBuf[iLen-1] = '\0';
			}
		}
		if(2 <= iLen)
		{
			if('\r' == pBuf[iLen-2] || '\n' == pBuf[iLen-2])
			{
				pBuf[iLen-2] = '\0';
			}
		}
	}
	pBuf[iBufSize-1] = '\0';

	return iOutRet;
}

INT CFile::Read(LPBYTE pBuf, INT iCount, INT * piBytesRead)
{
	INT iOutRet = ERROR_SUCCESS;
	size_t sizeRead = 0;

	m_iCrtErrNo = 0;

	do{
		if(NULL == pBuf || 0 >= iCount || NULL == piBytesRead)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(NULL == m_fpFile)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		sizeRead = fread(pBuf, 1, iCount, m_fpFile);
		if(piBytesRead)
		{
			*piBytesRead = sizeRead;
		}
		m_iCrtErrNo = errno;
		if(0 == sizeRead && !feof(m_fpFile))
		{
			iOutRet = ERROR_FILE_READ_FAIL;
			break;
		}
	}while(false);

	return iOutRet;
}

INT CFile::Write(LPBYTE pBuf, INT iCount, INT * piBytesWritten)
{
	INT iOutRet = ERROR_SUCCESS;
	size_t sizeWritted = 0;

	do{
		if(NULL == pBuf || 0 >= iCount || NULL == piBytesWritten)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(NULL == m_fpFile)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		sizeWritted = fwrite(pBuf, 1, iCount, m_fpFile);
		if(piBytesWritten)
		{
			*piBytesWritten = sizeWritted;
		}
		m_iCrtErrNo = errno;
		if(0 == sizeWritted && !feof(m_fpFile))
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(false);

	return iOutRet;
}

INT CFile::Seek(LONGLONG llNewPos, int whence)
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0;

	m_iCrtErrNo = 0;

	do{
		if(NULL == m_fpFile)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		
		iRet = fseek(m_fpFile, llNewPos, whence);
		if(0 != iRet)
		{
			m_iCrtErrNo = errno;
			iOutRet = ERROR_FILE_SEEK_FAIL;
			break;
		}
	}while(false);

	return iOutRet;
}

INT CFile::GetLastCrtErrNo()
{
	return m_iCrtErrNo;
}

INT CFile::Scanf(OUT SCANF_PARAM & oScanfParam, LPCSTR pszFmt, ...)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == m_fpFile)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		va_list VarList;
		
		va_start(VarList, pszFmt);
		oScanfParam.iFieldsScanned = vfscanf(m_fpFile, pszFmt, VarList);				
		va_end(VarList);

		if(EOF == oScanfParam.iFieldsScanned)
		{
			iOutRet = ERR_SCANF_FAILURE;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT CFile::Printf(OUT PRINTF_OUT_PARAM & oPrintfOutParam, LPCSTR pszFmt, ...)
{
	INT iOutRet = ERROR_SUCCESS;

	do{
		if(NULL == m_fpFile)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		va_list VarList;
		
		va_start(VarList, pszFmt);
		oPrintfOutParam.iBytesWritten = vfprintf(m_fpFile, pszFmt, VarList);				
		va_end(VarList);

		if(0 > oPrintfOutParam.iBytesWritten)
		{
			iOutRet = ERR_PRINTF_FAILURE;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

BOOL_t CFile::IsOpened()
{
	return (NULL != m_fpFile);
}

INT_t CFile::getFileSize(OUT off_t & CurFileSize)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	struct stat oFsStat;

	CurFileSize = -1;

	do
	{
		if(NULL == m_fpFile)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
		iRet = fstat(fileno(m_fpFile), &oFsStat);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERROR_FILE_STAT_FAIL;			
			break;
		}
		CurFileSize = oFsStat.st_size;
	}while(FALSE);

	return iOutRet;
}

