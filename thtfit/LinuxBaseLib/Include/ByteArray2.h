#ifndef	_BYTE_ARRAY2_H_
#define	_BYTE_ARRAY2_H_

#include <Array.h>

class CByteArray2 : public CArray <BYTE>
{
public:
	CByteArray2();
	virtual ~CByteArray2();
	virtual INT_t Append(BYTE * pArg, INT_t iCnt);
	virtual INT_t operator = (CONST CByteArray2 & otherByteArray2);
};

#endif	//_BYTE_ARRAY2_H_

