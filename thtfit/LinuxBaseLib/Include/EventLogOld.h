#ifndef EVENT_LOG_H__
#define EVENT_LOG_H__

#include <BaseTypeDef.h>
#include <FileMutexLocker.h>

#define EVENT_LOG_FILE_PATH1 "/data/EventLog1.data"
#define EVENT_LOG_FILE_PATH2 "/data/EventLog2.data"

#define MAX_SIZE_OF_EVENTLOG_FILE 	(2*1024*1024)

typedef enum{
	TYPE_LOG_UNKNOW,
	TYPE_LOG_ContentStarted = 0,
	TYPE_LOG_PowerStateChange,
	TYPE_LOG_InputChange,
	TYPE_LOG_VolumeChange,
	TYPE_LOG_ResolutionChange,
	TYPE_LOG_PowerStateChangeNoTicks,
	TYPE_LOG_InputChangeNoTicks,
	TYPE_LOG_VolumeChangeNoTicks,
	TYPE_LOG_StartOfEachPlaylist,
	TYPE_LOG_StatusOfPlayerBoot,
	TYPE_LOG_StreamDetect,
	TYPE_LOG_StreamLost,
	TYPE_LOG_FileNameMapID = 128,
	TYPE_LOG_PlayListNameMapId = 129,
	TYPE_LOG_ErrorEvent = 255,
}TpyeOfEventLog;

//0
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	UINT32_t fileNumber:24;
	UINT16_t playListId;
}ContentStartedLog,*pContentStartedLog;

// 1
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	BYTE powerState;
}PowerStateChangeLog,*pPowerStateChangeLog;

// 2
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;
	BYTE Input;
}InputChangeLog,*pInputChangeLog;

// 3
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	BYTE monitorVolume;
	BYTE playerVolume;
}VolumeChangeLog,*pVolumeChangeLog;

// 4
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	BYTE resolution;
}ResolutionChangeLog,*pResolutionChangeLog;

// 5
typedef struct{
	BYTE headerByte;
	BYTE offset;
	BYTE powerState;
}PowerStateChangeNoTicksLog,*pPowerStateChangeNoTicksLog;

//6
typedef struct{
	BYTE headerByte;
	BYTE offset;
	BYTE Input;
}InputChangeNoTicksLog,*pInputChangeNoTicksLog;

//7
typedef struct{
	BYTE headerByte;
	BYTE offset;
	BYTE monitorVolume;
	BYTE playerVolume;
}VolumeChangeNoTicksLog,*pVolumeChangeNoTicksLog;

//8
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	UINT16_t playListId;
	BYTE powerState;
	BYTE Input;
	BYTE monitorVolume;
	BYTE playerVolume;
}StartOfEachPlaylistLog,*pStartOfEachPlaylistLog;

//9
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
	BYTE powerState;
	BYTE Input;
	BYTE monitorVolume;
	BYTE playerVolume;
	BYTE resolution;
}StatusOfPlayerBootLog,*pStatusOfPlayerBootLog;

// 10-11
typedef struct{
	BYTE headerByte;
	BYTE offset;
	INT_t eventLogTime;//function time() return (second) 
}StreamInfoLog,*pStreamInfoLog;

// 128
typedef struct{
	BYTE headerByte;
	UINT32_t fileNameId:24;
	LPSTR pFileName;
}FileNameMapIDLog,*pFileNameMapIDLog;

//129
typedef struct{
	BYTE headerByte;
	UINT16_t playListId;
	LPSTR playListName;
}PlayListNameMapIdLog,*pPlayListNameMapIdLog;

//255
typedef struct{
	BYTE headerByte;
	BYTE offset;
	UINT32_t eventLogTime;//function time() return (second) 
	UINT16_t errorID;
	LPSTR extraInfo;
}ErrorEventLog,*pErrorEventLog;

class CEventLog
{
public:
	CEventLog();
	~CEventLog();
	INT_t WEventLog(TpyeOfEventLog LogType,void* LogData,UINT_t LogDataSize);
	INT_t GetLastErrNo();
	INT_t NotifyEvtLog(BOOL_t bStopWEvt);
	INT_t ReInitEvtLog();
private:
	INT_t InitVal();
	BOOL_t IsFirstLogFile();
	BOOL_t IsSecondLogFile();
	INT_t OpenEventLogFile(BOOL_t reCreate);
	INT_t GetLogFileSize(UINT_t *pFileSize);
private:
	CFileMutexLocker m_FileLocker;
	INT_t m_iLastErrNo;
	INT_t m_LogFd;
	BOOL_t m_bStopEvtLog;
	BOOL_t m_bFirstLogFile;
};

#endif

