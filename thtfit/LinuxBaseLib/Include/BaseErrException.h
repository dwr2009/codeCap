#ifndef	_BASE_ERR_EXCEPTION_H_
#define	_BASE_ERR_EXCEPTION_H_

#include <Exception.h>
#include <BaseTypeDef.h>

class CBaseErrException : public CException
{
public:
	CBaseErrException();
	CBaseErrException(INT_t iBaseErr);
	virtual ~CBaseErrException();
	virtual INT_t getBaseErr();
protected:
	INT_t m_iBaseErr;
};

#endif	//_BASE_ERR_EXCEPTION_H_

