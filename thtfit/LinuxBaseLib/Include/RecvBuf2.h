#ifndef	_RECVBUF2_H_
#define	_RECVBUF2_H_

#include <String.h>

class CRecvBuf2
{
public:
	CRecvBuf2();
	~CRecvBuf2();
	PBYTE GetBufPtr();
	int SetBufSize(int iSize, BOOL_t bRemainOldMemSizeIfPossible = FALSE);
	int GetBufSize();
	INT CopyData(PBYTE pBuf, INT iSize, BOOL bRemainOldMemSizeIfPossible = FALSE);
	int GetCurReadOff();
	int SetReadStartOff(int iOffset);
	int GetDataWithAutoOff(unsigned char * pBuf, int iSizeToGet);
	int GetDataWithAutoOff(unsigned char * pBuf, unsigned long ulSizeToGet);
	int GetDataWithAutoOff(OUT LPCSTR & pStr, OUT size_t & StrLen);
	INT_t getDataWithAutoOff(INT_t * CONST piBuf);
private:
	static PBYTE NewBuf(size_t sizeNew);
	INT RenewBuf(size_t sizeNew);
	void FreeBuf();
protected:
	int m_iDataOffset;
	PBYTE m_pBuf;
	int m_iBufSize;
	int m_iAllocatedMemSize;
private:
};

#endif	//_RECVBUF2_H_

