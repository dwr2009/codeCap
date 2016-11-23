#ifndef	_SIGNALINFO_H_
#define	_SIGNALINFO_H_

#include	<BaseTypeDef.h>

class CSignalInfo
{
public:
	static LPCSTR GetSingalDescFromSigNo(int iSigNo);
private:
	static LPCSTR m_ppSignalDesc[]; 
};

#endif	//_SIGNALINFO_H_

