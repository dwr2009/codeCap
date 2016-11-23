#ifndef	_ERRPRINTHELPER_H_
#define	_ERRPRINTHELPER_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#ifdef	SRC_FILE_NAME
#define	LOG_BLINE(Format,...)		\
	printf("[%s:%d]" Format, SRC_FILE_NAME, __LINE__,##__VA_ARGS__);
#define	LOG_LINE(Format,...)		\
	printf("[%s:%d]" Format, __FILE__, __LINE__,##__VA_ARGS__);
#else	//!defined(SRC_FILE_NAME)
#define	LOG_BLINE(Format,...)		\
	printf("[%s:%d]" Format, basename(__FILE__), __LINE__,##__VA_ARGS__);
#define	LOG_LINE(Format,...)		\
	printf("[%s:%d]" Format, __FILE__, __LINE__,##__VA_ARGS__);
#endif

#define	LOG(Format,...)				printf(Format,##__VA_ARGS__);

#define	LOGP(Format,Params...)		printf(Format, Params);

#if	!defined(ANDROID)
#define	LOGE(Format,...)			fprintf(stderr,Format,##__VA_ARGS__);
#define	AD_LOG(TAG,Format,...)		printf(TAG","Format"\n",##__VA_ARGS__);
#endif	//!ANDROID

#define	PRINT_FILE_LINENO_CRT_ERRINFO 	do{printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));}while(0);	
#define	PRINT_BFILE_LINENO_CRT_ERRINFO 		\
	do	\
	{	\
		LOG_BLINE("%s\n", strerror(errno));	\
	}while(0);	
#define	PRINT_FILE_LINENO_BUG_STR		do{printf("[%s:%d]BUG\n", __FILE__, __LINE__);}while(0);	
#define	PRINT_BFILE_LINENO_BUG_STR		\
	do	\
	{	\
		LOG_BLINE("BUG\n");	\
	}while(0);	
#define	PRINT_FILE_LINENO_IRET_BUG_STR		do{printf("[%s:%d]BUG, iRet=%d\n", __FILE__, __LINE__, iRet);}while(0);	
#define	PRINT_FILE_LINENO_IRET_STR		do{printf("[%s:%d]iRet=%d\n", __FILE__, __LINE__, iRet);}while(0);	
#define	PRINT_BFILE_LINENO_IRET_STR		\
	do	\
	{	\
		LOG_BLINE("iRet=%d\n", iRet);	\
	}while(0);
#define	PRINT_FILE_LINENO_IRET_CRT_ERR	do{printf("[%s:%d]iRet=%d, %s\n", __FILE__, __LINE__, iRet, strerror(errno));}while(0);
#ifdef	SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_IRET_CRT_ERR		\
	do	\
	{	\
		printf("[%s:%d]iRet=%d, %s\n", SRC_FILE_NAME, __LINE__, iRet, strerror(errno));	\
	}while(0);
#else	//!SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_IRET_CRT_ERR		\
	do	\
	{	\
		printf("[%s:%d]iRet=%d, %s\n", basename(__FILE__), __LINE__, iRet, strerror(errno));	\
	}while(0);
#endif	//SRC_FILE_NAME
#define	PRINT_FILE_LINENO_RMSTATUS_ERRINFO 	do{printf("[%s:%d]%s\n", __FILE__, __LINE__, RMstatusToString(eRMstat));}while(0);	
#define	PRINT_FILE_LINENO_RMSTATUS	 	do{printf("[%s:%d]%s\n", __FILE__, __LINE__, RMstatusToString(eRMstatus));}while(0);	

#ifdef	SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_RMSTATUS	 	\
	do	\
	{	\
		printf("[%s:%d]%s\n", SRC_FILE_NAME, __LINE__, RMstatusToString(eRMstatus));	\
	}while(0);	
#else	//!SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_RMSTATUS	 	\
	do	\
	{	\
		printf("[%s:%d]%s\n", basename(__FILE__), __LINE__, RMstatusToString(eRMstatus));	\
	}while(0);	
#endif	//SRC_FILE_NAME

#define	PRINT_FILE_LINENO_RM_STATUS	 	do{printf("[%s:%d]%s\n", __FILE__, __LINE__, RMstatusToString(status));}while(0);	
#define	PRINT_BFILE_LINENO_RM_STATUS	 	\
	do	\
	{	\
		LOG_BLINE("%s\n", RMstatusToString(status));	\
	}while(0);	
#define	PRINT_FILE_LINENO_RMSTATUS_ERR	 	do{printf("[%s:%d]%s\n", __FILE__, __LINE__, RMstatusToString(err));}while(0);	
#ifdef	SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_RMSTATUS_ERR	 	\
	do	\
	{	\
		printf("[%s:%d]%s\n", SRC_FILE_NAME, __LINE__, RMstatusToString(err));	\
	}while(0);	
#else	//!SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_RMSTATUS_ERR	 	\
	do	\
	{	\
		printf("[%s:%d]%s\n", basename(__FILE__), __LINE__, RMstatusToString(err));	\
	}while(0);	
#endif	//SRC_FILE_NAME
#define	PRINT_FILE_LINENO		do{printf("[%s:%d]", __FILE__, __LINE__);}while(0);
#define	PRINT_BFILE_LINENO		\
	do	\
	{	\
		printf("[%s:%d]", basename(__FILE__), __LINE__);	\
	}while(0);
#define	PRINT_BASEFILE_LINENO		do{printf("[%s:%d]", basename(__FILE__), __LINE__);}while(0);
#define	PRINT_FILE_LINENO_NEWLINE		do{printf("[%s:%d]\n", __FILE__, __LINE__);}while(0);
#ifdef	SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_NEWLINE		do{printf("[%s:%d]\n", SRC_FILE_NAME, __LINE__);}while(0);
#else	//!SRC_FILE_NAME
#define	PRINT_BFILE_LINENO_NEWLINE		do{printf("[%s:%d]\n", basename(__FILE__), __LINE__);}while(0);
#endif	//SRC_FILE_NAME
#define	PrintIRetBugIfFail(iRet)	if(ERROR_SUCCESS != iRet){PRINT_FILE_LINENO_IRET_BUG_STR;}

#ifndef	QLOGFL
#define QLOGFL(Format,...)      \
	qDebug("[%s:%d]" Format, SRC_FILE_NAME, __LINE__,##__VA_ARGS__);
#endif	//QLOGFL

#if 1/*2012-11-9 for test how many time is spend in function*/
#define DBG_TIME(fmt,...){\
	struct timeval tvDbgCurrentTick;\
	gettimeofday(&tvDbgCurrentTick,NULL);\
	printf( "[SEC: %ld.%06ld] [%s:%d] " fmt,\
	               tvDbgCurrentTick.tv_sec,tvDbgCurrentTick.tv_usec,\
				   __FILE__,__LINE__,\
		           ##__VA_ARGS__);\
}
#else
#define DBG_TIME(fmt,...)
#endif

#endif	//_ERRPRINTHELPER_H_

