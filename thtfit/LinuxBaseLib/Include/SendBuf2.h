#ifndef	_SENDBUF2_H_
#define	_SENDBUF2_H_

#include <BaseTypeDef.h>
#include <StackBufString.h>

class CSendBuf2
{
public:
	CSendBuf2();
	~CSendBuf2();
	unsigned char * GetBufPtr();
	int GetBufSize();
	int SetBufSize(int iSize, bool bRemainOldMemSizeIfPossible = false);
	int GetCurWriteOff();
	int SetWriteStartOff(int iOffset);
	int WriteDataWithAutoOff(CONST PBYTE pBuf, int iSizeToWrite);
	int WriteDataWithAutoOff(int iVal);
	int WriteDataWithAutoOff(unsigned int uiVal);
	int WriteDataWithAutoOff(unsigned char ucVal);
	int WriteDataWithAutoOff(void * pVal);
	INT_t WriteDataWithAutoOff(CStackBufString & strValue);
	static int getObjCnt();
protected:
	int m_iDataOffset;
	unsigned char * m_pBuf;
	int m_iBufSize;
	int m_iAllocatedMemSize;
private:
	static int m_iObjCnt;
};

#endif	//_SENDBUF2_H_

