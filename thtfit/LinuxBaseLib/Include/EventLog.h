#ifndef EVENT_LOG_H__
#define EVENT_LOG_H__

#include <BaseTypeDef.h>
#include <FileMutexLocker.h>
#include <String.h>

#define OLD_EVENT_LOG_FILE_PATH1 	"/data/EventLog1.data"
#define OLD_EVENT_LOG_FILE_PATH2 	"/data/EventLog2.data"

#define EVENT_LOG_FILE_PATH  "/data/EventLog.txt"
#define EVT_LOG_FILE_BAKE_PATH "/tmp/EventLogTmp.txt"
#define MBYTE_TO_BYTE (1024*1024)
#define FILE_SIZE_BLANKING	(512*1024)
#define LOG_DIR_MIN_FREE_SIZE_MB	2
#define LOG_FILE_MAX_SIZE_MB	8
#define STATIC_MAX_BUF_SIZE	(16*1024)

typedef enum
{
	Reserved_EventLog= 0,
	TYPE_LOG_SystemStart = 1,
	TYPE_LOG_SystemOff = 2,
	TYPE_LOG_ResolutionChange = 3,
	TYPE_LOG_VolumeChange = 4,
	TYPE_LOG_Format = 5,
	TYPE_LOG_Boot,
	TYPE_LOG_SystemErrors,
	TYPE_LOG_SystemAudioOutPutMode,
	TYPE_LOG_RS232,
	TYPE_LOG_CEC,
	TYPE_LOG_IPChange,
	TYPE_LOG_NetWorkLoss,
	TYPE_LOG_WDTReset,
	TYPE_LOG_DisplayIsConnect,
	TYPE_LOG_UsbIsPlug,
	TYPE_LOG_SdInsert,
	TYPE_LOG_SdRemove,
	TYPE_LOG_GpioInput = 18,
	TYPE_LOG_GpioOutput,
	TYPE_LOG_IRInput,
	TYPE_LOG_IROutPut,
	TYPE_LOG_PushButton,
	TYPE_LOG_XappRun,
	TYPE_LOG_Transfer=101,
	TYPE_LOG_TransferError,
	TYPE_LOG_Delete,
	TYPE_LOG_ReadFileError,
	TYPE_LOG_StartPlay=201,
	TYPE_LOG_PlaybackEos = 203,
}TpyeOfEventLog;


class CEventLog
{
public:
	CEventLog();
	~CEventLog();
	INT_t WEventLog(CString::STD_STRING_LIST & wStrList);
	INT_t GetLastErrNo();
	INT_t NotifyEvtLog(BOOL_t bStopWEvt);
	INT_t ReInitEvtLog();
private:
	INT_t InitVal();
	INT_t OpenEventLogFile();
	INT_t GetLogFileSize(UINT_t *pFileSize);
	BOOL_t IsEventLogFileExist();
	INT_t CheckLogDirFreeSize(UINT_t *pFreeSizeMB);
	INT_t LostHalfEvtLogFile(UINT_t LogFileSize);
	INT_t InverEscapeSequence(LPCSTR pInStr, UINT32_t inSize, LPSTR pOutStr, UINT32_t outSize, 
		OUT size_t & BytesWrittenWithNull);
private:
	CFileMutexLocker m_FileLocker;
	INT_t m_iLastErrNo;
	INT_t m_LogFd;
	BOOL_t m_bStopEvtLog;
	UINT_t m_PreBlankSize;
};

#endif

