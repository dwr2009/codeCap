#ifndef	_MP_VIDEO_SYNC_H_
#define	_MP_VIDEO_SYNC_H_

#include <BaseTypeDef.h>
#include <SharedPtr.h>
#include <SnmpCmdConv.h>
#include "MediaSrv.h"
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QReadWriteLock>
#include <QSemaphore>
#include <StackBufString.h>
#include "SysBaseSrvApi.h"
#include <QTimer>
#include <QWaitCondition>
#include <QMutex>

using namespace Mp7xxCommon;
#ifdef	__cplusplus
namespace VideoSync{
#endif

#define	VIDEO_SYNC_PORT 				(5005)
#define VIDEO_SYNC_MAGIC                (0xAAAA5555)
#define VIDEO_SYNC_MAX_URL_PATH_LEN     (256)
#define VIDEO_SYNC_MAX_ADDR_LEN         (256)
#define VIDEO_SYNC_MAX_PACKET_LEN       (1024)//(8*1024)
#define	VIDEO_SYNC_BUFFDATASIZE 		(512)
#define VIDEO_SYNC_BUFFSIZE 			(512)
#define VIDEO_SYNC_CMD                  (0)
#define VIDEO_SYNC_SYNCHRO              (1)

/*for sync calc*/
#define VIDEO_SYNC_ERROR				(30)
#define VIDEO_SYNC_MAX_PLAY_SPEED		(1000)
#define VIDEO_SYNC_MIN_PLAY_SPEED		(-1000)
#define VIDEO_SYNC_NORMAL_PLAY_SPEED	(0)
#define VIDEO_SYNC_MAX_OFFSET			(1000)
#define VIDEO_SYNC_MIN_OFFSET			(-1000)
#define VIDEO_SYNC_OFFSET_DEVIATION		(20)
#define VIDEO_SYNC_SAMPLE_TIME			(1000)/*ms*/




typedef enum{
	eVideoSyncState_Idle,
	eVideoSyncState_PrepareToPlay,
	eVideoSyncState_ReadyToPlay,
	eVideoSyncState_Running,
}eVideoSyncState;

typedef enum{
	eVideoSyncCmd_Nothing,
	eVideoSyncCmd_Reg,
	eVideoSyncCmd_UnReg,
	eVideoSyncCmd_PrepareToPlay,
	eVideoSyncCmd_ReadyToPlay,
	eVideoSyncCmd_StartPlay,
	eVideoSyncCmd_PlayingInfo,
	eVideoSyncCmd_StopPlay,
	eVideoSyncCmd_Master_Login,
	eVideoSyncCmd_Slave_Login,
	eVideoSyncCmd_Master_Logout,
	eVideoSyncCmd_Master_Change,
	/*sync protocol*/
	eVideoSyncCmd_Sync,
	eVideoSyncCmd_Follow_Up,
	eVideoSyncCmd_Delay_Req,
	eVideoSyncCmd_Resp
}eVideoSyncCmd;

typedef struct{
	UINT32_t Type;
	UINT32_t Magic;                
	UINT32_t DataLen;             
	UINT32_t PacketSequenceNum;   
	eVideoSyncCmd Cmd;         
}tVideoSyncCommHead,*p_tVideoSyncCommHead;

#define PACKET_CONTROL_CMD_HEAD(pCmdHead,cmd){ \
	(pCmdHead)->CmdHead.Type= VIDEO_SYNC_CMD; \
	(pCmdHead)->CmdHead.Magic = VIDEO_SYNC_MAGIC; \
	(pCmdHead)->CmdHead.DataLen = sizeof(*(pCmdHead)) - sizeof(tVideoSyncCommHead); \
	(pCmdHead)->CmdHead.PacketSequenceNum = m_VidSyncPacketSequenceNum++; \
	(pCmdHead)->CmdHead.Cmd = cmd; \
}

#define PACKET_PRECISION_SYNC_HEAD(pCmdHead,cmd){ \
	(pCmdHead)->CmdHead.Type= VIDEO_SYNC_SYNCHRO; \
	(pCmdHead)->CmdHead.Magic = VIDEO_SYNC_MAGIC; \
	(pCmdHead)->CmdHead.DataLen = sizeof(*(pCmdHead)) - sizeof(tVideoSyncCommHead); \
	(pCmdHead)->CmdHead.PacketSequenceNum = m_VidSyncPacketSequenceNum++; \
	(pCmdHead)->CmdHead.Cmd = cmd; \
}

typedef struct{
	tVideoSyncCommHead CmdHead;
}tVideoSyncCommMsg,*p_tVideoSyncCommMsg;

typedef struct{
	tVideoSyncCommHead CmdHead;
	INT8 strURL[VIDEO_SYNC_MAX_URL_PATH_LEN];
}tVideoSyncCmdMsg,*p_tVideoSyncCmdMsg;

typedef struct{
	tVideoSyncCommHead CmdHead;
	INT8 strAddr[VIDEO_SYNC_MAX_ADDR_LEN];
}tVideoSyncAddrMsg,*p_tVideoSyncAddrMsgMsg;

typedef struct{
	tVideoSyncCommHead CmdHead;
	UINT SyncTime;
}tPrecisionSyncMsg,*p_tPrecisionSyncMsg;

typedef struct{
	tVideoSyncCommHead CmdHead;
	eVideoSyncState SyncState;
}tVideoSyncStateMsg,*p_tVideoSyncStateMsg;

typedef QList<QString> IP_SLAVE_LIST;

class BufferThread;
class PrecisionSyncThread;

class QVideoSync : public QTcpServer
{
	Q_OBJECT
public:
	QVideoSync(SharedPtr <QMediaSrv> QMediaSrv_sp,QObject * parent);
	virtual ~QVideoSync();
	INT_t InitInstance();
	INT_t StartSync();
	INT_t SyncTimeCalculate();
	INT_t UdpSendOut(LPCSTR pSendBuf, size_t iBufSize,QHostAddress address = QHostAddress::Broadcast);
	INT_t SendControlCmd(eVideoSyncCmd Cmd,QHostAddress address = QHostAddress::Broadcast);
	INT_t SendControlSync(eVideoSyncCmd Cmd,QHostAddress address = QHostAddress::Broadcast);
	VOID  incomingConnection(int socketDescriptor);
	VOID  TcpSendOutDataToMaster(QString &MasterAddr);
	VOID  ReceivedDataFormBuffer(char * buffer);
	VOID  RegistFromSlave(const QString &SlaveIpAddr);
	INT_t SaveSyncMode(const VIDEO_SYNC_MODE_t &SyncMode);
	INT_t SaveSyncMaster(const QString &strMasterAddr);
	INT_t getCurrentTime(OUT ULONG & uTimestamp);
	INT_t getLastTime(OUT ULONG & uTimestamp);
	INT_t setSyncTime(const LONG & Offset);
	INT_t setSyncSeek(const LONG & Offset);
	INT_t setDataSource(const QString & strUrl);
	INT_t PlaySyncMedia(QString strURL);
	INT_t getPlayState(OUT qint32 &PlayState);
	INT_t getVideoSyncMode(OUT qint32 &SyncMode);
	INT_t setVideoSyncMode(VIDEO_SYNC_MODE_t SyncMode);
	INT_t getVideoSyncMaster(OUT QString &strMasterAddr);
	INT_t setVideoSyncMaster(const QString &strMasterAddr);

public slots:
	VOID  SyncInformation();
	VOID  UdpReceiveData();
	VOID  TcpSendOutDataToMasterSlot();
	VOID  DisconnectedSlaveSocket();
	VOID  TcpReceiveDataFromMaster();
	VOID  PrecisionSync();
	VOID  TcpSocketError(QAbstractSocket::SocketError);
	VOID  getTest();//for debug
	VOID  test();

public:
	WeakPtr <QVideoSync> m_this_wp;
	tVideoSyncAddrMsg Buffer[VIDEO_SYNC_BUFFSIZE];
	VIDEO_SYNC_MODE_t m_eVideoSyncMode;
	SharedPtr <QTimer> m_SyncTimer_sp;
	SharedPtr <QTimer> m_SyncInfoTimer_sp;
	SharedPtr <QTimer> m_TestTimer_sp;
	IP_SLAVE_LIST m_SlaveList;
	QWaitCondition SyncWaitCond;
	QMutex SyncMutex;
	QSemaphore *Wirte;
	QSemaphore *Read;
	QReadWriteLock lock;
	bool SyncSeeking;
	LONG Offset;

private:
	pthread_t m_threadId;
	Qt::HANDLE m_hThisThreadId;
	UINT16_t m_VidSyncPacketSequenceNum;
	SharedPtr <PrecisionSyncThread> m_QPrecisionSync_sp;
	SharedPtr <BufferThread> m_QBufferThread_sp;
	SharedPtr <QUdpSocket> m_UdpSocket_sp;
	SharedPtr <QTcpServer> m_TcpMaster_sp;
	SharedPtr <QMediaSrv> m_QMediaSrv_sp;
	ULONG uTimestampA,uTimestampB;
	ULONG uTimestampC,uTimestampD;
	bool bSyncState,bFollowUpState;
	bool bReqState,bRespState;
	QTcpSocket *TcpSocketSlave;
	QTcpServer *TcpMaster;
	QString m_MasterAddr;
	QString m_SyncNameURL;
	QString m_CurrentURL;
	bool bRegState;
	bool SyncState;
};

class TcpThread : public QThread 
{ 
	Q_OBJECT
public: 
	TcpThread(int socketDescriptor, QVideoSync * parent);
	INT_t RegistFromSlave(const QString &SlaveIpAddr);
	VOID  TcpSendOutDataToSlave();
	VOID  run(); 
private slots:
	VOID  TcpReceiveDataFromSlave();
signals:
	VOID  error(QTcpSocket::SocketError socketError);
private:
	INT_t socketDescriptor;
	QVideoSync *m_QVideoSync_sp;
	QTcpSocket *TcptSocketMaster;
}; 

class BufferThread : public QThread 
{ 
	Q_OBJECT
public: 
	BufferThread(QVideoSync * parent);
	VOID  run(); 
private:
	QVideoSync *m_QVideoSync_sp;	
}; 

class ProtocolThread : public QThread 
{ 
	Q_OBJECT
public: 
	ProtocolThread(QVideoSync * parent);
	VOID  run(); 
public slots:	 
	VOID  SyncThreadQuit();
signals:
	VOID  SyncComplete();
private: 
	QVideoSync *m_QVideoSync_sp;	
}; 

class PrecisionSyncThread : public QThread 
{ 
	Q_OBJECT
public: 
	PrecisionSyncThread(QVideoSync * parent);
	VOID  run(); 

private: 
	QVideoSync *m_QVideoSync_sp;	
}; 

#ifdef	__cplusplus
}
#endif
#endif	//_MP_VIDEO_SYNC_H_
