#ifndef	_PROCESS_H_
#define	_PROCESS_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#include <BaseTypeDef.h>

#define	EXPECT_STR_START		1
#define	SINGLE_STR_START		2

typedef struct tagPROCESS_ENVI_INFO
{
	BOOL bPrintErrInfo;
	INT iLastCrtErrNo;
}PROCESS_ENVI_INFO, * P_PROCESS_ENVI_INFO;

typedef struct tagRUN_PROCESS_PARA
{
	IN LPCSTR pszCmd;
	OUT int iProcessStatus;
}RUN_PROCESS_PARA, *P_RUN_PROCESS_PARA;

typedef struct
{
	IN LPCSTR pszCmd;
}RUN_PROCESS_PARA_IN, *P_RUN_PROCESS_PARA_IN;

typedef struct
{
	OUT int iProcessStatus;
}RUN_PROCESS_PARA_OUT, *P_RUN_PROCESS_PARA_OUT;

INT CreateProcess(pid_t * pProcessId, const char * pszCmd, P_PROCESS_ENVI_INFO pEnvInfo);
INT RunProcess(P_RUN_PROCESS_PARA pRunProcessPara);
INT RunProcess2(P_RUN_PROCESS_PARA_IN pRunProcParaIn, P_RUN_PROCESS_PARA_OUT pRunProcParaOut);

#ifdef	__cplusplus
}
#endif

#endif	//_PROCESS_H_

