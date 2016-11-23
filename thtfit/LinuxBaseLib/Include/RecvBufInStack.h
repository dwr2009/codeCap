#ifndef	_RECV_BUF_IN_STACK_H_
#define	_RECV_BUF_IN_STACK_H_

#include <String.h>

class CRecvBufInStack
{
public:
	CRecvBufInStack(PBYTE pBufInStack, INT_t BufSize);
	~CRecvBufInStack();
	PBYTE GetBufPtr();
	int SetBufSize(int iSize);
	int GetBufSize();
	INT CopyData(PBYTE pBuf, INT iSize);
	int GetCurReadOff();
	int SetReadStartOff(int iOffset);
	int GetDataWithAutoOff(PBYTE pBuf, int iSizeToGet);
	INT_t getDataWithAutoOff(INT_t * CONST piData);
	int GetDataWithAutoOff(PBYTE pBuf, ULONG_t ulSizeToGet);
	int GetDataWithAutoOff(OUT LPCSTR & pStr, OUT size_t & StrLen);
private:

protected:
	int m_iDataOffset;
	PBYTE m_pBuf;
	int m_iBufSize;
	int m_iAllocatedMemSize;
private:
};

#define	DECLARE_CLS_RecvBufInStack(ObjName,BufSize)	BYTE sz##ObjName[BufSize]; CRecvBufInStack ObjName(sz##ObjName, BufSize);

#endif	//_RECV_BUF_IN_STACK_H_

