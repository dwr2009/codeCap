#if	!defined(_ProcessRunningInfo_h_)
#define	_ProcessRunningInfo_h_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#ifdef	__cplusplus

class CProcessRunningInfo
{
public:
	static BOOL_t NxserverIsRunning();
};

#endif	//__cplusplus

DECLARE_EXTERN_C_BLOCK_START

BOOL_t NxserverIsRunning(void);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_ProcessRunningInfo_h_

