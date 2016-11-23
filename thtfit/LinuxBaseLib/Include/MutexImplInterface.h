#if	!defined(_MUTEX_IMPL_INTERFACE_H_)
#define	_MUTEX_IMPL_INTERFACE_H_

#include <BaseTypeDef.h>

#ifdef	__cplusplus

class CMutexImplInterface
{
public:
	virtual ~CMutexImplInterface() {};
	virtual INT_t Lock() = 0;
	virtual INT_t Unlock() = 0;
	virtual INT_t GetCrtErrNo() = 0;
};

#endif	//__cplusplus

#endif	//_MUTEX_IMPL_INTERFACE_H_

