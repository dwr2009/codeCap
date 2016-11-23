#ifndef	_INV_ARG_EXCEPTION_H_
#define	_INV_ARG_EXCEPTION_H_

#include <Exception.h>

class CInvArgException : public CException
{
public:
	CInvArgException();
	virtual ~CInvArgException();
};

#endif	//_INV_ARG_EXCEPTION_H_
