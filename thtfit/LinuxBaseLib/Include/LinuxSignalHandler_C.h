#ifndef	_LINUX_SIGNAL_HANDLER_C_H_
#define	_LINUX_SIGNAL_HANDLER_C_H_

#include <CCplusplusBridge.h>
#include <pthread.h>
#include <BaseTypeDef.h>

DECLARE_EXTERN_C_BLOCK_START

typedef enum
{
	SIG_RECV_THIS_THREAD = 1,
}SIG_RECV_TARGET_TYPE, *P_SIG_RECV_TARGET_TYPE;

typedef struct
{
	int iSigNo;
}LINUX_SIG_INFO, *P_LINUX_SIG_INFO;

typedef VOID (*PFN_SIG_HANDLER_CALLBACK)(PVOID pContext, P_LINUX_SIG_INFO pLinuxSigInfo);

typedef struct
{
	int iSigNo;
	SIG_RECV_TARGET_TYPE eSigRecvTargetType;
	pthread_t pidTargetThread;	
	PFN_SIG_HANDLER_CALLBACK pfnSigHandlerCallback;
	PVOID pCallbackContext;
}LINUX_SIG_HANDLER_ITEM, *P_LINUX_SIG_HANDLER_ITEM;

typedef struct
{
	int iSigNo;
	SIG_RECV_TARGET_TYPE eSigRecvTargetType;
	PFN_SIG_HANDLER_CALLBACK pfnSigHandlerCallback;
	PVOID pCallbackContext;
}REG_SIG_HANDLER_PARAM_IN, *P_REG_SIG_HANDLER_PARAM_IN;

typedef struct
{
	int iSigNo;
}UNREG_SIG_HANDLER_PARAM_IN, *P_UNREG_SIG_HANDLER_PARAM_IN;

INT LinuxSigHandler_Init();
INT LinuxSigHandler_RegSigHandler(P_REG_SIG_HANDLER_PARAM_IN pRegSigHandlerParamIn);
INT LinuxSigHandler_UnregSigHandler(P_UNREG_SIG_HANDLER_PARAM_IN pUnregSigHandlerParamIn);
INT LinuxSigHandler_Uninit();

DECLARE_EXTERN_C_BLOCK_END

#endif	//_LINUX_SIGNAL_HANDLER_C_H_

