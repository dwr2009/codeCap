#ifndef	_TIMER_LISTENER_INTERFACE_H_
#define	_TIMER_LISTENER_INTERFACE_H_

#include <BaseTypeDef.h>

namespace CppBase
{

class ITimerListener
{
public:
	virtual ~ITimerListener() {};
	virtual VOID OnTimer(UINT_t TimerId) = 0;
};

}

#endif	//_TIMER_LISTENER_INTERFACE_H_

