#ifndef	_FILE_H_
#define	_FILE_H_

#include <stdio.h>
#include <BaseTypeDef.h>

#define	OPT_TRIM_TAILING_NEWLINE			0x00000001

class CFile
{
public:
	typedef struct
	{
		int iFieldsScanned;
	}SCANF_PARAM, *P_SCANF_PARAM;
	typedef struct
	{
		INT iBytesWritten;
	}PRINTF_OUT_PARAM, *P_PRINTF_OUT_PARAM;
public:
	CFile();
	virtual ~CFile();
	virtual INT Open(LPCSTR pszFilePath, LPCSTR pszMode);
	virtual INT FlushFileStream();
	virtual INT Close();
	virtual INT ReadLine(LPBYTE pBuf, INT iBufSize, UINT uiOptions = 0);
	virtual INT Read(LPBYTE pBuf, INT iCount, INT * piBytesRead);
	virtual INT Write(LPBYTE pBuf, INT iCount, INT * piBytesWritten);
	virtual INT Seek(LONGLONG llNewPos, int whence);
	virtual INT GetLastCrtErrNo();
	virtual INT Scanf(OUT SCANF_PARAM & oScanfParam, LPCSTR pszFmt, ...);
	virtual INT Printf(OUT PRINTF_OUT_PARAM & oPrintfOutParam, LPCSTR pszFmt, ...);
	virtual BOOL_t IsOpened();
	virtual INT_t getFileSize(OUT off_t & CurFileSize);
protected:
	FILE * m_fpFile;
	INT m_iCrtErrNo;
};

#endif	//_FILE_H_

