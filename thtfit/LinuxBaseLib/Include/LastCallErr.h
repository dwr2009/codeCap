#ifndef	_LAST_CALL_ERR_H_
#define	_LAST_CALL_ERR_H_

#include <BaseTypeDef.h>

class CLastCallErr
{
public:
	CLastCallErr();
	~CLastCallErr();
	virtual INT getLastCallErr();
protected:
	INT m_iLastCallErr;
};

#endif	//_LAST_CALL_ERR_H_
