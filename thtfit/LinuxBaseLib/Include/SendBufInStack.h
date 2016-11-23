#ifndef	_SEND_BUF_IN_STACK_H_
#define	_SEND_BUF_IN_STACK_H_

#include <BaseTypeDef.h>
#include <StackBufString.h>

class CSendBufInStack
{
public:
	CSendBufInStack(PBYTE pBufInStack, INT_t BufSize);
	~CSendBufInStack();
	PBYTE GetBufPtr();
	int GetBufSize();
	int SetBufSize(int iSize);
	int GetCurWriteOff();
	int SetWriteStartOff(int iOffset);
	int WriteDataWithAutoOff(CONST PBYTE pBuf, int iSizeToWrite);
	int WriteDataWithAutoOff(INT_t iVal);
	int WriteDataWithAutoOff(UINT_t uiVal);
	int WriteDataWithAutoOff(BYTE ucVal);
	int WriteDataWithAutoOff(void * pVal);
	INT_t WriteDataWithAutoOff(CStackBufString & strValue);
	INT_t WriteDataWithAutoOff(LPCSTR pszVal);
protected:
	int m_iDataOffset;
	PBYTE m_pBuf;
	int m_iBufSize;
	int m_iAllocatedMemSize;
private:
};

#define	DECLARE_CLS_SendBufInStack(ObjName,BufSize)	BYTE sz##ObjName[BufSize]; CSendBufInStack ObjName(sz##ObjName, BufSize);

#endif	//_SENDBUF2_H_

