#include "VideoSync.h"
#include <ThreadMessageQueue.h>
#include "MediaPlayerAppIf.h"
#include <QNetworkInterface>
#include "DbgLogSwitchDef.h"
#include <ErrPrintHelper.h>

#define DEBUG
#ifdef DEBUG 
#define LOG_ERROR(fmt,...) LOG_BLINE("ERROR: " fmt,##__VA_ARGS__)
#define LOG_WARNING(fmt,...) LOG_BLINE("WARNING: " fmt,##__VA_ARGS__)
#define LOG_INFO(fmt,...) do{if(Sw_LogVideoSync) LOG_BLINE(fmt,##__VA_ARGS__);}while(0);
#else
#define LOG_INFO(fmt,...)
#endif

using namespace VideoSync;

QVideoSync::QVideoSync(SharedPtr <QMediaSrv> QMediaSrv_sp,QObject *parent)
	:QTcpServer(parent),m_QMediaSrv_sp(QMediaSrv_sp)
{
	bRegState = false;
	SyncSeeking = false;
	bSyncState = bFollowUpState = bRespState = false;
	m_eVideoSyncMode = VidSyncMode_MASTER;
	
	Wirte = new QSemaphore(VIDEO_SYNC_BUFFSIZE);
	if(!Wirte)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO
	}
	
	Read = new QSemaphore;
	if(!Read)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO
	}
	
	SyncState = false;
}

QVideoSync::~QVideoSync(){}

INT_t QVideoSync::InitInstance()
{
	INT_t iOutRet = ERR_SUCCESS, iRet ,argc;
	SharedPtr <CGeneralApp> GeneralApp_sp;
	char ** argv = NULL; 
	bool bRet = false;
	
	/*Read the setting value of VideoSync*/
	{DECLARE_CLS_STACK_BUF_STRING(strVideoSyncMaster, 64);
	iRet = SysProp_get(SysProp_KEY_MpVideoSyncMaster, OUT strVideoSyncMaster);
	if(ERROR_SUCCESS == iRet) 
	{
		m_MasterAddr = QString(strVideoSyncMaster);
	}
	else if(ERROR_NOT_FOUND == iRet) 
	{
		m_MasterAddr = "NOT MASTER";
	}
	else 
		goto EXIT_PROC;}
	
	iRet = SaveSyncMaster(m_MasterAddr);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}

	/*Read the mode of sync */
	{DECLARE_CLS_STACK_BUF_STRING(strValue, 64);
	iRet = SysProp_get(SysProp_KEY_MpVideoSyncMode, OUT strValue);
	if(ERROR_SUCCESS == iRet) 
	{
		m_eVideoSyncMode = (typeof(m_eVideoSyncMode))(INT_t)strValue;
	}
	else if(ERROR_NOT_FOUND == iRet) 
	{
		m_eVideoSyncMode = VidSyncMode_DISABLED;
	}
	else 
		goto EXIT_PROC;}
	
	iRet = SaveSyncMode(m_eVideoSyncMode);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}

	GeneralApp_sp = CGeneralApp::getGeneralApp();
	if(GeneralApp_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	GeneralApp_sp->getCmdLineArgs(OUT argc, OUT argv);

	/*sync*/
	m_SyncTimer_sp = SharedPtr <QTimer> (new QTimer(this));
	if(m_SyncTimer_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_SyncTimer_sp), SIGNAL(timeout()), 
		this, SLOT(PrecisionSync()));
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}

	m_UdpSocket_sp = SharedPtr <QUdpSocket> (new QUdpSocket(this)); 
	if(m_UdpSocket_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = m_UdpSocket_sp->bind(VIDEO_SYNC_PORT);
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_UdpSocket_sp), SIGNAL(readyRead()), 
		this, SLOT(UdpReceiveData()));
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}

	bRet = listen(QHostAddress::Any,VIDEO_SYNC_PORT);
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}

	m_QBufferThread_sp = SharedPtr <BufferThread> (new BufferThread(this));
	if(m_QBufferThread_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_QBufferThread_sp), SIGNAL(finished()), 
		&(*m_QBufferThread_sp), SLOT(deleteLater()));  
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}
	m_QBufferThread_sp->start(); 

	/*The message of synchronization*/
	m_SyncInfoTimer_sp = SharedPtr <QTimer> (new QTimer(this));
	if(m_SyncInfoTimer_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_SyncInfoTimer_sp), SIGNAL(timeout()), 
		this, SLOT(SyncInformation()));
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}

	m_QPrecisionSync_sp = SharedPtr <PrecisionSyncThread> (new PrecisionSyncThread(this));
	if(m_QPrecisionSync_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_QPrecisionSync_sp), SIGNAL(finished()), 
		&(*m_QPrecisionSync_sp), SLOT(deleteLater()));  
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}
	m_QPrecisionSync_sp->start(); 	

	/*test */
	m_TestTimer_sp = SharedPtr <QTimer> (new QTimer(this));
	if(m_TestTimer_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY; 
		goto EXIT_PROC;
	}
	
	bRet = connect(&(*m_TestTimer_sp), SIGNAL(timeout()), this, SLOT(getTest()));
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_FAILED; 
		goto EXIT_PROC;
	}

	//m_TestTimer_sp->start(20);
		
	m_SyncInfoTimer_sp->setSingleShot(TRUE);
	m_SyncInfoTimer_sp->start(3*1000);

EXIT_PROC:
	return iOutRet;
}


VOID QVideoSync::SyncInformation()
{
	INT_t iOutRet = ERR_SUCCESS, iRet; 
	if(VidSyncMode_MASTER == m_eVideoSyncMode)
	{
#if 0
		iRet = SendControlCmd(eVideoSyncCmd_Master_Login);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
#endif		
	}
	else if(VidSyncMode_SLAVE == m_eVideoSyncMode)
	{
		//SendControlCmd(eVideoSyncCmd_Slave_Login);
	}
	
EXIT_PROC:
	return ;	
}

VOID QVideoSync::UdpReceiveData()
{  
	INT_t iOutRet = ERR_SUCCESS, iRet;
	char szRecvBuf[VIDEO_SYNC_MAX_PACKET_LEN];
	QList<QHostAddress> AddrList; 
	p_tVideoSyncCommHead pCommHead;
	p_tPrecisionSyncMsg pSyncMsg;
	p_tVideoSyncCmdMsg pCmd;
	QHostAddress address;
	QString LocalAddr;
	qint64 size;
	quint16 port;

	if(m_UdpSocket_sp->hasPendingDatagrams() && m_eVideoSyncMode != VidSyncMode_DISABLED)
	{  
		AddrList = QNetworkInterface::allAddresses();
		for(int i = 0;i < AddrList.count(); i++)
		{    
			LocalAddr = AddrList.at(i).toString();      
			if(LocalAddr != "127.0.0.1")
				{break;} 
		}
		
		size = m_UdpSocket_sp->readDatagram(szRecvBuf, sizeof(szRecvBuf),&address,&port);
		if(size)
		{
			LOG_INFO("Read the size of the datagram : %lld\n",size);
			pCmd = (p_tVideoSyncCmdMsg)(&szRecvBuf);
			pCommHead = (p_tVideoSyncCommHead)(&szRecvBuf);
			pSyncMsg = (p_tPrecisionSyncMsg)(&szRecvBuf);
		}
		else
		{
			iOutRet = size; 
			goto EXIT_PROC;
		}

		if(LocalAddr != address.toString())
		{
			if(VIDEO_SYNC_CMD == pCommHead->Type)
			{
				if(VidSyncMode_MASTER == m_eVideoSyncMode)
				{
					if(eVideoSyncCmd_Slave_Login == pCommHead->Cmd)
					{
						LOG_INFO("UDP Master ReceiveControlCmd : %d\n",pCommHead->Cmd);
						iRet = SendControlCmd(eVideoSyncCmd_Master_Login,
							QHostAddress(address.toString()));
						if(iRet)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							iOutRet = iRet;
							goto EXIT_PROC;
						}
					}
				}
				else if(VidSyncMode_SLAVE == m_eVideoSyncMode)
				{
					if(eVideoSyncCmd_StartPlay == pCommHead->Cmd 
						&& m_MasterAddr == address.toString())
					{
						iRet = PlaySyncMedia(QString(pCmd->strURL));
						if(iRet)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							iOutRet = iRet; 
							goto EXIT_PROC;
						}
						m_SyncNameURL = pCmd->strURL;/*mark file name */
						m_CurrentURL = pCmd->strURL;
					}
				}
				if(eVideoSyncCmd_Master_Login == pCommHead->Cmd)/*Master ,Slave,Disable*/
				{
					LOG_INFO("UDP Login ReceiveControlCmd : %d\n",pCommHead->Cmd);
					m_MasterAddr = address.toString();
					m_eVideoSyncMode = VidSyncMode_SLAVE;
					
					iRet = SaveSyncMode(m_eVideoSyncMode);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						iOutRet = iRet; 
						goto EXIT_PROC;
					}
					
					iRet = SaveSyncMaster(m_MasterAddr);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						iOutRet = iRet; 
						goto EXIT_PROC;
					}
					
					TcpSendOutDataToMaster(m_MasterAddr);/*Send addr of slave to master*/
				}
				if(eVideoSyncCmd_Master_Logout == pCommHead->Cmd)
				{
					LOG_INFO("UDP Logout ReceiveControlCmd : %d\n",pCommHead->Cmd);
					m_MasterAddr = "NO MASTER";/*Save the addr of master */
					
					iRet = SaveSyncMaster(m_MasterAddr);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						iOutRet = iRet; 
						goto EXIT_PROC;
					}
				}
				if(eVideoSyncCmd_Master_Change == pCommHead->Cmd)
				{
					LOG_INFO("UDP change ReceiveControlCmd : %d\n",pCommHead->Cmd);
					
					iRet = setVideoSyncMode(VidSyncMode_MASTER);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						iOutRet = iRet; 
						goto EXIT_PROC;
					}
					
					iRet = SaveSyncMode(m_eVideoSyncMode);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						iOutRet = iRet; 
						goto EXIT_PROC;
					}
				}
			}
			else if(VIDEO_SYNC_SYNCHRO == pCommHead->Type)
			{
				if(VidSyncMode_MASTER == m_eVideoSyncMode)
				{
					if(eVideoSyncCmd_Delay_Req == pCommHead->Cmd)
					{
						LOG_INFO("master recevie Delay_Req.\n");
						
						iRet = SendControlSync(eVideoSyncCmd_Resp);
						if(iRet)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							iOutRet = iRet;
							goto EXIT_PROC;
						}
						
						uTimestampA = uTimestampD = 0;/*master protocol complete*/
					}
				}
				else if(VidSyncMode_SLAVE == m_eVideoSyncMode)
				{
					if(eVideoSyncCmd_Sync== pCommHead->Cmd) 
					{
						/*Mark the flag of sync state*/
						bSyncState = true;/*slave*/
						LOG_INFO("slave recevie Sync.\n");
						
						iRet = getCurrentTime(uTimestampB);
						if(iRet || 0 == uTimestampB)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							bSyncState = false;
							iOutRet = iRet;
							goto EXIT_PROC;
						}
					}
					else if(eVideoSyncCmd_Follow_Up == pCommHead->Cmd)
					{
						if(bSyncState)/*slave*/
						{
							bFollowUpState = true;
							LOG_INFO("slave recevie Follow_Up.\n");
							
							uTimestampA = pSyncMsg->SyncTime;
							if(0 == uTimestampA)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								bSyncState = false;
								bFollowUpState = false;
								iOutRet = iRet;
								goto EXIT_PROC;
							}
							
							iRet = SendControlSync(eVideoSyncCmd_Delay_Req);
							if(iRet)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								iOutRet = iRet;
								goto EXIT_PROC;
							}
						}
					}
					else if(eVideoSyncCmd_Resp == pCommHead->Cmd)
					{
						if(bReqState)
						{
							LOG_INFO("slave receive Resp.\n");
							bRespState = true;
							
							uTimestampD = pSyncMsg->SyncTime;/*slave*/
							if(0 == uTimestampD)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								bSyncState = false;
								bFollowUpState = false;
								bReqState = false;
								bRespState = false;
								iOutRet = iRet;
								goto EXIT_PROC;
							}
							
							iRet = SyncTimeCalculate();
							if(iRet)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								iOutRet = iRet;
								goto EXIT_PROC;
							}
						}
					}
				}
			}
		}
	}
	
EXIT_PROC:
	return;
}  

INT_t QVideoSync::PlaySyncMedia(QString strURL)
{
	INT_t iOutRet = ERR_SUCCESS, iRet;
	QMediaSrv::PLAY_PARAMS playParams;
	quint32 playbackId = 0;
	INT_t iLoopCnt = 1;
	
	if(m_QMediaSrv_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERR_INV_STATE; 
		goto EXIT_PROC;
	}
	
	iRet = m_QMediaSrv_sp->Stop();
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}
	
	iRet = m_QMediaSrv_sp->setDataSource(strURL);/*set*/
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}
	
	iRet = m_QMediaSrv_sp->Prepare();/*state*/
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}
	
	QMediaSrv::PlayParams_Init(&playParams);
	playParams.LoopCount = iLoopCnt;
	
	iRet = m_QMediaSrv_sp->Play(&playbackId, &playParams);/*play*/
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet; 
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::UdpSendOut(LPCSTR pSendBuf, size_t iBufSize,QHostAddress address)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	qint64 size = m_UdpSocket_sp->writeDatagram(pSendBuf,iBufSize,address, VIDEO_SYNC_PORT);
	if(size && (size == iBufSize))
	{
		LOG_INFO("udp send out sizes = %d\n",iBufSize);
		LOG_INFO("udp send out types = %d\n",((p_tVideoSyncCommHead)&(*pSendBuf))->Type);
		LOG_INFO("udp send out Cmd = %d\n",((p_tVideoSyncCommHead)&(*pSendBuf))->Cmd);
	}
	else 
	{
		iOutRet = ERROR_SOCKET_SEND_FAIL; 
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::StartSync()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	if(VidSyncMode_MASTER == m_eVideoSyncMode)
	{
		iRet = SendControlCmd(eVideoSyncCmd_StartPlay);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		//m_SyncTimer_sp->setSingleShot(TRUE);
		m_SyncTimer_sp->start(20*1000);
		//m_TestTimer_sp->start(20);
#if 1		
		SendControlCmd(eVideoSyncCmd_Master_Login);
#endif
	}else 
		goto EXIT_PROC;
	
EXIT_PROC:
	return iOutRet;	
}

VOID QVideoSync::ReceivedDataFormBuffer(char * buffer)
{
	static unsigned int i;
	
	Wirte->acquire();
	
	strncpy(Buffer[i % VIDEO_SYNC_BUFFSIZE].strAddr,buffer,VIDEO_SYNC_MAX_ADDR_LEN);
	LOG_INFO("write a address :%s, index : %d, available : %d\n",
			Buffer[i % VIDEO_SYNC_BUFFSIZE].strAddr,i,Wirte->available());
	i = ((i + 1) % VIDEO_SYNC_BUFFDATASIZE);
	
	Read->release();
}

VOID QVideoSync::incomingConnection(int socketDescriptor)  
{  
	do{
		TcpThread *thread = new TcpThread(socketDescriptor, this);
		if(!thread) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bool bRet = connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));  
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		thread->start();  
	}while(false);
}  

VOID QVideoSync::TcpSendOutDataToMaster(QString &MasterAddr)
{
	do{
		TcpSocketSlave = new QTcpSocket(this);
		if(!TcpSocketSlave) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bool bRet = connect(TcpSocketSlave, SIGNAL(connected()), 
			this, SLOT(TcpSendOutDataToMasterSlot()));
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bRet = connect(TcpSocketSlave, SIGNAL(disconnected()), 
			this, SLOT(DisconnectedSlaveSocket()));
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bRet = connect(TcpSocketSlave, SIGNAL(readyRead()), 
			this, SLOT(TcpReceiveDataFromMaster()));
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bRet = connect(TcpSocketSlave, SIGNAL(error(QAbstractSocket::SocketError)), 
			this,SLOT(TcpSocketError(QAbstractSocket::SocketError)));
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		TcpSocketSlave->connectToHost(QHostAddress(MasterAddr),VIDEO_SYNC_PORT); 
	}while(false);
}

VOID QVideoSync::TcpSocketError(QAbstractSocket::SocketError)
{
	QString error = TcpSocketSlave->errorString();
	
	LOG_INFO("Tcp Socket error : %s",error.toLatin1().data());
	
	TcpSocketSlave->close();
}

VOID QVideoSync::TcpSendOutDataToMasterSlot()
{
	QString SlaveAddr;
	QList<QHostAddress> AddrList; 
	AddrList = QNetworkInterface::allAddresses();
	
	for(int i = 0;i < AddrList.count(); i++)
	{    
		SlaveAddr = AddrList.at(i).toString();     
		if(SlaveAddr != "127.0.0.1")
			{break;} 
	}
	
	qint64 size = TcpSocketSlave->write(SlaveAddr.toLatin1().data(), SlaveAddr.length());
	if(size && (size == SlaveAddr.length()))
	{
		LOG_INFO("tcp send out sizes : %d\n",SlaveAddr.length());
		LOG_INFO("tcp send out types : %lld\n",size);
		LOG_INFO("tcp send out the addr of slave : %s\n",SlaveAddr.toLatin1().data());
	}else 
		{PRINT_FILE_LINENO_CRT_ERRINFO;}
}

VOID QVideoSync::DisconnectedSlaveSocket()
{
	LOG_INFO("Disconnected the socket of slave.\n");
	TcpSocketSlave->abort();
}

VOID QVideoSync::TcpReceiveDataFromMaster()
{
	QByteArray message = TcpSocketSlave->readAll();
	qDebug()<<"tcp receive the message for master = "<<message;
}

INT_t QVideoSync::SendControlCmd(eVideoSyncCmd Cmd,QHostAddress address)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QList<QHostAddress> AddrList; 
	QString SlaveAddr;
	
	if(eVideoSyncCmd_StartPlay == Cmd)
	{
		tVideoSyncCmdMsg CmdMsg={0};
		PACKET_CONTROL_CMD_HEAD(&CmdMsg,Cmd);
		QString StrURL = m_CurrentURL;/*master*/
		strncpy(CmdMsg.strURL,StrURL.toLatin1().data(),sizeof(CmdMsg.strURL)-1);
		
		iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg));
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		LOG_INFO("SendControlCmd : %s, size: %d, leng :%d\n",CmdMsg.strURL,
			sizeof(CmdMsg.strURL),strlen(CmdMsg.strURL));
	}
	else if(eVideoSyncCmd_Master_Login == Cmd)
	{
		tVideoSyncCommMsg CmdMsg={0};
		PACKET_CONTROL_CMD_HEAD(&CmdMsg,Cmd);
		//m_MasterAddr = QNetworkInterface::allAddresses().first().toString();
		AddrList = QNetworkInterface::allAddresses();
		
		for(int i = 0;i < AddrList.count(); i++)
		{    
			m_MasterAddr = AddrList.at(i).toString();    
			if(m_MasterAddr != "127.0.0.1")
				{break;} 
		}
		
		iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg));
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		LOG_INFO("UDP Master SendControlCmd : %d\n",CmdMsg.CmdHead.Cmd);
	}
	else if(eVideoSyncCmd_Slave_Login == Cmd)
	{
		tVideoSyncCommMsg CmdMsg={0};
		PACKET_CONTROL_CMD_HEAD(&CmdMsg,Cmd);
		
		iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg));
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		LOG_INFO("UDP Slave SendControlCmd : %d\n",CmdMsg.CmdHead.Cmd);
	}
	else if(eVideoSyncCmd_Master_Logout == Cmd)
	{
		tVideoSyncCommMsg CmdMsg={0};
		PACKET_CONTROL_CMD_HEAD(&CmdMsg,Cmd);
		m_MasterAddr = "NO MASTER";
		
		iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg));
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		LOG_INFO("UDP Master Logout SendControlCmd : %d\n",CmdMsg.CmdHead.Cmd);
	}
	else if(eVideoSyncCmd_Master_Change == Cmd)
	{
		tVideoSyncCommMsg CmdMsg={0};
		PACKET_CONTROL_CMD_HEAD(&CmdMsg,Cmd);
		
		iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg),address);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		LOG_INFO("UDP Change SendControlCmd : %d\n",CmdMsg.CmdHead.Cmd);
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::SendControlSync(eVideoSyncCmd Cmd,QHostAddress address)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	tPrecisionSyncMsg CmdMsg={0};
	PACKET_PRECISION_SYNC_HEAD(&CmdMsg,Cmd);

	if(eVideoSyncCmd_Sync == Cmd)
	{
		iRet = getCurrentTime(uTimestampA); /*t1*/
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			uTimestampA = 0;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else if(eVideoSyncCmd_Follow_Up == Cmd)
	{
		CmdMsg.SyncTime = uTimestampA;
		uTimestampA = 0;
	}
	else if(eVideoSyncCmd_Delay_Req == Cmd)
	{
		if(bFollowUpState)/*slave*/
		{
			LOG_INFO("slave send out Delay_Req.\n");
			bReqState = true;
			
			iRet = getCurrentTime(uTimestampC); /*t3*/
			if(iRet || 0 == uTimestampC)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				bSyncState = false;
				bFollowUpState = false;
				bReqState = false;
				iOutRet = iRet;
				goto EXIT_PROC;
			}
		}
	}
	else if(eVideoSyncCmd_Resp == Cmd)
	{
		LOG_INFO("master send out Resp.\n");
			
		iRet = getCurrentTime(uTimestampD); /*t4*/
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			uTimestampD = 0;
			iOutRet = iRet;
			goto EXIT_PROC;
		}	
		
		CmdMsg.SyncTime = uTimestampD;
	}
	
	iRet = UdpSendOut((LPCSTR)&CmdMsg,sizeof(CmdMsg),address);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::setDataSource(const QString & strUrl)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	if(m_QMediaSrv_sp.isNull())
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_OUT_OF_MEMORY;
		goto EXIT_PROC;
	}
	
	if(NULL == strUrl)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = ERROR_INVALID_PARAMETER;
		goto EXIT_PROC;
	}

	/*Video Sync Master Mode*/
	if(VidSyncMode_MASTER == m_eVideoSyncMode)
	{
		iRet = m_QMediaSrv_sp->setDataSource(strUrl);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		m_CurrentURL = strUrl;/*master*/
		
		iRet = StartSync();
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else if(VidSyncMode_SLAVE == m_eVideoSyncMode)/* Slave Mode*/
	{
		iRet = m_QMediaSrv_sp->setDataSource(strUrl);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		
		m_CurrentURL = strUrl;/*slave*/
	}
	else/*Normal*/
	{
		iRet = m_QMediaSrv_sp->setDataSource(strUrl);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

VOID QVideoSync::RegistFromSlave(const QString &SlaveIpAddr)
{
	IP_SLAVE_LIST::iterator itList;
	
	lock.lockForWrite();
	
	if(!(m_SlaveList).contains(SlaveIpAddr))
	{
		m_SlaveList.append(SlaveIpAddr);
	}
	
#if 0
	for (itList = m_SlaveList.begin(); itList != m_SlaveList.end(); ++itList)
	{
		LOG_INFO("regist slave ip address : %s\n",itList->toLatin1().data());
	}
#endif	

	lock.unlock();	

#if 0  /*sync inferface*/
	//ready for precision sync after 5 seconds.
	if(m_SyncTimer_sp->isActive()){m_SyncTimer_sp->stop();} 
	//m_SyncTimer_sp->setSingleShot(TRUE);
	m_SyncTimer_sp->start(1*1000/1);
#endif

	bRegState = true;
}

INT_t QVideoSync::SyncTimeCalculate()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	if(m_SyncNameURL == m_CurrentURL)
	{
		if(!SyncSeeking)
		{
			Offset = ((LONG)(uTimestampB - uTimestampA 
				- uTimestampD + uTimestampC)) >> 1;

			LOG_INFO("t1 = %lu, t2 = %lu, t3 = %lu, t4 = %lu, offset = %ld\n",
				uTimestampA,uTimestampB,uTimestampC,uTimestampD,Offset);

			if(abs(Offset) > VIDEO_SYNC_ERROR)
			{ 
				LOG_INFO("--- sync start ---\n");
				SyncMutex.lock();
				SyncWaitCond.wakeAll();/*start calc*/
				SyncMutex.unlock();
			}
		}
	}
	else
	{
		LOG_INFO("Current URL : %s\n",m_CurrentURL.toLatin1().data());
		LOG_INFO("Sync URL : %s\n",m_SyncNameURL.toLatin1().data());
		LOG_INFO("It's different form master' to the file of slave.\n");
	}

	bSyncState = bFollowUpState = bReqState = bRespState = false;
	/*slave protocol complete*/
	uTimestampA = uTimestampB = uTimestampC = uTimestampD = 0; 
	
EXIT_PROC:
	return iOutRet;
}

VOID QVideoSync::PrecisionSync()
{
	do{ /*avoid block the work thread.*/
		ProtocolThread *thread = new ProtocolThread(this);
		if(!thread) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bool bRet = connect(&(*thread), SIGNAL(SyncComplete()), 
			&(*thread), SLOT(SyncThreadQuit()));
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		bRet = connect(&(*thread), SIGNAL(finished()), 
			&(*thread), SLOT(deleteLater()));  
		if(!bRet) 
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		thread->start();
	}while(false);
}

/*interface*/
INT_t QVideoSync::getVideoSyncMode(OUT qint32 &SyncMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	SyncMode = m_eVideoSyncMode;
	
	return iOutRet;
}

INT_t QVideoSync::setVideoSyncMode(VIDEO_SYNC_MODE_t eSyncMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	/*notify others when be setted master mode*/
	if(m_eVideoSyncMode != eSyncMode)
	{
		if(VidSyncMode_MASTER == m_eVideoSyncMode) 
		{
			if(VidSyncMode_SLAVE == eSyncMode)
			{
				/*slave waitting for master be setted in the local net to sync*/
				m_eVideoSyncMode = eSyncMode;
				
				iRet = SendControlCmd(eVideoSyncCmd_Master_Logout);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = iRet;
					goto EXIT_PROC;
				}
			}
			else if(VidSyncMode_DISABLED == eSyncMode)/*no do*/
			{
				m_eVideoSyncMode = eSyncMode;
				
				iRet = SendControlCmd(eVideoSyncCmd_Master_Logout);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = iRet;
					goto EXIT_PROC;
				}
			}
		}
		else if(VidSyncMode_SLAVE == m_eVideoSyncMode)
		{
			if(VidSyncMode_MASTER == eSyncMode)
			{
				/*change state myself and others*/
				m_eVideoSyncMode = eSyncMode;
				
				iRet = SendControlCmd(eVideoSyncCmd_Master_Login);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = iRet;
					goto EXIT_PROC;
				}
			}
			else if(VidSyncMode_DISABLED == eSyncMode)/*no do*/
			{
				m_eVideoSyncMode = eSyncMode;
			}
		}
		else if(VidSyncMode_DISABLED == m_eVideoSyncMode)
		{
			if(VidSyncMode_MASTER == eSyncMode)
			{
				/*change state myself and others*/
				m_eVideoSyncMode = eSyncMode;
				
				iRet = SendControlCmd(eVideoSyncCmd_Master_Login);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = iRet;
					goto EXIT_PROC;
				}
			}
			else if(VidSyncMode_SLAVE == eSyncMode)
			{
				/*slave waitting for master be setted in the local net to sync*/
				m_eVideoSyncMode = eSyncMode;
				
				iRet = SendControlCmd(eVideoSyncCmd_Slave_Login);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					iOutRet = iRet;
					goto EXIT_PROC;
				}
			}
		}
		/*save mode*/
		iRet = SaveSyncMode(m_eVideoSyncMode);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::getVideoSyncMaster(OUT QString &strMasterAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	/*search master*/
	//sSendControlCmd(eVideoSyncCmd_Slave_Login);
	strMasterAddr = m_MasterAddr;
	
	return iOutRet;
}

INT_t QVideoSync::setVideoSyncMaster(const QString &strMasterAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	QList<QHostAddress> AddrList; 
	QString LocalAddr;
	
	AddrList = QNetworkInterface::allAddresses();
	
	for(int i = 0;i < AddrList.count(); i++)
	{    
		LocalAddr = AddrList.at(i).toString();    
		if(LocalAddr != "127.0.0.1")
			{break;} 
	}
	
	if(LocalAddr == strMasterAddr)
	{
		iRet = setVideoSyncMode(VidSyncMode_MASTER);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	else
	{
		iRet = SendControlCmd(eVideoSyncCmd_Master_Change,
		QHostAddress(strMasterAddr));
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			iOutRet = iRet;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::SaveSyncMode(const VIDEO_SYNC_MODE_t &SyncMode)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = SysProp_setPersistent(SysProp_KEY_MpVideoSyncMode,SyncMode);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::SaveSyncMaster(const QString &strMasterAddr)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = SysProp_setPersistent(SysProp_KEY_MpVideoSyncMaster, 
		strMasterAddr.toLatin1().data());
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::getPlayState(OUT qint32 &PlayState)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_QMediaSrv_sp->getPlayMode(PlayState);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	
EXIT_PROC:
	return iOutRet;
}


INT_t QVideoSync::getCurrentTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_QMediaSrv_sp->getCurrentTime(uTimestamp);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::getLastTime(OUT ULONG & uTimestamp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_QMediaSrv_sp->getLastTime(uTimestamp);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::setSyncTime(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_QMediaSrv_sp->setSyncTime(Offset);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	
EXIT_PROC:
	return iOutRet;
}

INT_t QVideoSync::setSyncSeek(const LONG & Offset)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	
	iRet = m_QMediaSrv_sp->setSyncSeek(Offset);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
		iOutRet = iRet;
		goto EXIT_PROC;
	}	
	
EXIT_PROC:
	return iOutRet;
}

VOID QVideoSync::test()
{
	/*for debug*/
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	ULONG SlaveTime;
	int Mode;
	
	iRet = getCurrentTime(SlaveTime);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
	LOG_INFO("get current timestamp = %lu\n",SlaveTime);
	
	iRet = getLastTime(SlaveTime);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}	
	LOG_INFO("get last timestamp = %lu\n",SlaveTime);
	
	iRet = m_QMediaSrv_sp->getPlayMode(Mode);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}	
	LOG_INFO("get play mode = %d\n",Mode);
	LOG_INFO("\n\n");
}

VOID QVideoSync::getTest()
{
	/*for debug*/
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	ULONG SlaveTime;
	iRet = getCurrentTime(SlaveTime);
	if(iRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
	LOG_INFO("get current timestamp = %lu\n",SlaveTime);
}

TcpThread::TcpThread(int socketDescriptor, QVideoSync *parent)  
	: QThread(parent), socketDescriptor(socketDescriptor),m_QVideoSync_sp(parent){}  

/*receive thread*/
VOID TcpThread::run()  
{  
	TcptSocketMaster = new QTcpSocket;
	if(!TcptSocketMaster)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
		
	if (!TcptSocketMaster->setSocketDescriptor(socketDescriptor))
	{
		emit error(TcptSocketMaster->error());
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
	
	bool bRet = connect(TcptSocketMaster, SIGNAL(readyRead()), 
		this, SLOT(TcpReceiveDataFromSlave()));  
	if(!bRet)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO;
	}
		
  	INT_t iThreadExitCode = exec();
	LOG_INFO("TCP thread exit code : %d\n",iThreadExitCode);
}   

VOID TcpThread::TcpReceiveDataFromSlave()
{
	char szRecvBuf[VIDEO_SYNC_MAX_PACKET_LEN]={0};
	
	qint64 size = TcptSocketMaster->read(szRecvBuf,sizeof(szRecvBuf));
	if(!size)
	{
		PRINT_FILE_LINENO_CRT_ERRINFO; 
		goto EXIT_PROC;
	}
	
	m_QVideoSync_sp->ReceivedDataFormBuffer(szRecvBuf);
	LOG_INFO("TCP Receive Slave ip : %s\n",szRecvBuf);
	
	TcptSocketMaster->abort();
	
    if (TcptSocketMaster->state() == QAbstractSocket::UnconnectedState ||
		TcptSocketMaster->waitForDisconnected(1000))
		LOG_INFO("TCP disconnected.\n");
	
EXIT_PROC:	
	quit();
	wait();
}

VOID TcpThread::TcpSendOutDataToSlave(){}

BufferThread::BufferThread(QVideoSync *parent)
	:QThread(parent),m_QVideoSync_sp(parent){}

VOID BufferThread::run()
{
	static unsigned int i;
	
	while(true)
	{
		m_QVideoSync_sp->Read->acquire();
		
		LOG_INFO("read a address :%s, index : %d, available : %d\n",
			m_QVideoSync_sp->Buffer[i % VIDEO_SYNC_BUFFSIZE].strAddr,i,
			m_QVideoSync_sp->Wirte->available());
		
		m_QVideoSync_sp->RegistFromSlave(QString(QLatin1String(m_QVideoSync_sp
			->Buffer[i % VIDEO_SYNC_BUFFSIZE].strAddr)));
		
		i = ((i + 1) % VIDEO_SYNC_BUFFDATASIZE);
		
		m_QVideoSync_sp->Wirte->release();
	}
}

ProtocolThread::ProtocolThread(QVideoSync *parent)
	:QThread(parent),m_QVideoSync_sp(parent){}

/*sync thread*/
VOID ProtocolThread::run()
{
	INT_t iRet = ERROR_SUCCESS, PlsyState;
	IP_SLAVE_LIST::iterator itList;
	ULONG CurrentTime,LastTime;
	
	m_QVideoSync_sp->lock.lockForRead();
	
	itList = m_QVideoSync_sp->m_SlaveList.begin();
	while(itList != m_QVideoSync_sp->m_SlaveList.end())
	{
		iRet = m_QVideoSync_sp->getPlayState(PlsyState);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		if(MediaPlayer::PlayMode_Normal == PlsyState)
		{
			iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;
			}
			
			iRet = m_QVideoSync_sp->getLastTime(LastTime);			
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;
			}
			
			if(LastTime - CurrentTime < 10000)/*milliseconds*/
			{
				if(m_QVideoSync_sp->m_SyncTimer_sp->isActive())
				{
					/*stop sync*/
					m_QVideoSync_sp->m_SyncTimer_sp->stop();
				}
				
				LOG_INFO("Stop Sync !\n");
				break;
			}

			/*step 1 : Sync*/
			LOG_INFO("sync address : %s\n",itList->toLatin1().data());
			
			iRet = m_QVideoSync_sp->SendControlSync(eVideoSyncCmd_Sync,
				QHostAddress(itList->toLatin1().data()));
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO
				break;
			}
			LOG_INFO("master send out SYNC.\n");
			
			msleep(10);
			
			/*step 2 : Follow_up*/
			iRet = m_QVideoSync_sp->SendControlSync(eVideoSyncCmd_Follow_Up,
				QHostAddress(itList->toLatin1().data()));
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO
				break;
			}

			LOG_INFO("master send out Follow_Up.\n");
				
			++itList;
		}
		else /*trick mode*/
		{
			LOG_INFO("Media is not playing,the state is %d !\n",PlsyState);
			if(MediaPlayer::PlayMode_Stopped == PlsyState)
			{
				if(m_QVideoSync_sp->m_SyncTimer_sp->isActive())
				{
					/*stop sync*/
					m_QVideoSync_sp->m_SyncTimer_sp->stop();
				}
				LOG_INFO("Stop Sync !\n");
			}
			
			break;
		}
	}
	
	m_QVideoSync_sp->lock.unlock();
	
	emit SyncComplete();
	int iThreadExitCode = exec();
	LOG_INFO("SYNC thread exit code : %d\n",iThreadExitCode);
}

VOID ProtocolThread::SyncThreadQuit()
{
	quit();
	wait();
}

PrecisionSyncThread::PrecisionSyncThread(QVideoSync *parent)
	:QThread(parent),m_QVideoSync_sp(parent){}

VOID PrecisionSyncThread::run()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	ULONG SlaveTime, CurrentTime, BaseTime;
	ULONG Delay, DeltaTime;
	LONG DeltaError, DeltaOffset;
	
	while(true)
	{
		m_QVideoSync_sp->SyncMutex.lock();
	
		/*wait for waking*/
		m_QVideoSync_sp->SyncWaitCond.wait(&m_QVideoSync_sp->SyncMutex);

		m_QVideoSync_sp->SyncSeeking = true;

		iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
		if(iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
			goto EXIT_PROC;
		}	
		
		/*mark  time of master*/ 
		BaseTime = CurrentTime - m_QVideoSync_sp->Offset;
		LOG_INFO("BaseTime = %lu\n",BaseTime);
		
		if(abs(m_QVideoSync_sp->Offset) > 3*VIDEO_SYNC_MAX_OFFSET)
		{
			iRet = m_QVideoSync_sp->getCurrentTime(SlaveTime);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				goto EXIT_PROC;
			}	
			
			if(m_QVideoSync_sp->Offset > 0)
			{
				/*Rewind   x10*/	
				iRet = m_QVideoSync_sp->setSyncTime(10*VIDEO_SYNC_MIN_PLAY_SPEED);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					goto EXIT_PROC;
				}	

				for(int i = 1; i < 1000; i++)
				{
					usleep(10*VIDEO_SYNC_SAMPLE_TIME);
					
					iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						goto EXIT_PROC;
					}	
					
					DeltaError = SlaveTime - CurrentTime;
					if((SlaveTime > CurrentTime) && abs(DeltaError) > VIDEO_SYNC_OFFSET_DEVIATION)
					{
						Delay = i*10;
						BaseTime += i*10;
						LOG_INFO("--- RE --- SlaveTime = %lu, CurrentTime = %lu, BaseTime = %lu, DeltaError = %ld, Delay = %lu\n",
							SlaveTime,CurrentTime,BaseTime,DeltaError,Delay);
						
						break;
					}
				}
			}
			else if(m_QVideoSync_sp->Offset < 0)
			{
				/*Fast forward  x10*/
				iRet = m_QVideoSync_sp->setSyncTime(10*VIDEO_SYNC_MAX_PLAY_SPEED);
				if(iRet)
				{
					PRINT_FILE_LINENO_CRT_ERRINFO;
					goto EXIT_PROC;
				}	

				for(int i = 1; i < 1000; i++)
				{
					usleep(10*VIDEO_SYNC_SAMPLE_TIME);
					
					iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						goto EXIT_PROC;
					}	
					
					DeltaError = SlaveTime - CurrentTime;
					if((SlaveTime < CurrentTime) && abs(DeltaError) > VIDEO_SYNC_OFFSET_DEVIATION)
					{
						Delay = i*10;
						BaseTime += i*10;
						LOG_INFO("--- FF --- SlaveTime = %lu, CurrentTime = %lu, BaseTime = %lu, DeltaError = %ld, Delay = %lu\n",
							SlaveTime,CurrentTime,BaseTime,DeltaError,Delay);
						
						break;
					}
				}
			}
			
			while(true)/*Fast seeking*/
			{
				if(m_QVideoSync_sp->Offset > 0)
				{
					usleep(10*VIDEO_SYNC_SAMPLE_TIME);
					
					iRet = m_QVideoSync_sp->getCurrentTime(SlaveTime);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						goto EXIT_PROC;
					}	
					
					BaseTime += 10;
					DeltaOffset = SlaveTime - BaseTime;/*delta = Ts-Tm*/
			
					if(DeltaOffset <= 0)
					{
						/*To find a key frame*/
						LOG_INFO("--- FS > 0 --- To find a key picture : %lu\n",SlaveTime);
						
						iRet = m_QVideoSync_sp->setSyncTime(VIDEO_SYNC_NORMAL_PLAY_SPEED);
						if(iRet)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							goto EXIT_PROC;
						}	
			
						for(int i = 1; i < 1000; i++)
						{
							usleep(10*VIDEO_SYNC_SAMPLE_TIME);
							
							iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
							if(iRet)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								goto EXIT_PROC;
							}	
							
							DeltaError = SlaveTime - CurrentTime;
							if(abs(DeltaError) > VIDEO_SYNC_OFFSET_DEVIATION)
							{
								Delay = i*10;
								LOG_INFO("--- FS > 0 --- key = %lu, CurrentTime = %lu, delta = %ld, Delay = %lu\n",
									SlaveTime,CurrentTime,DeltaError,Delay);
								LOG_INFO("--- FS > 0 --- Playback Start!\n");
								
								break;
							}
						}

						/*-(abs(offset) + delay + error)*/
						m_QVideoSync_sp->Offset = DeltaOffset - Delay - 180;
						
						LOG_INFO("--- FS > 0 --- New Offset = %ld\n",m_QVideoSync_sp->Offset);

						break;
					}
				}
				else if(m_QVideoSync_sp->Offset < 0)
				{
					usleep(10*VIDEO_SYNC_SAMPLE_TIME);
					
					iRet = m_QVideoSync_sp->getCurrentTime(SlaveTime);
					if(iRet)
					{
						PRINT_FILE_LINENO_CRT_ERRINFO;
						goto EXIT_PROC;
					}	
					
					BaseTime += 10;
					DeltaOffset = SlaveTime - BaseTime;/*delta = Ts-Tm*/
			
					if(DeltaOffset >= 0)
					{
						/*To find a key frame*/
						LOG_INFO("--- FS < 0 --- To find a key picture : %lu\n",SlaveTime);
						
						iRet = m_QVideoSync_sp->setSyncTime(VIDEO_SYNC_NORMAL_PLAY_SPEED);
						if(iRet)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
							goto EXIT_PROC;
						}	
			
						for(int i = 1; i < 1000; i++)
						{
							usleep(10*VIDEO_SYNC_SAMPLE_TIME);
							
							iRet = m_QVideoSync_sp->getCurrentTime(CurrentTime);
							if(iRet)
							{
								PRINT_FILE_LINENO_CRT_ERRINFO;
								goto EXIT_PROC;
							}	
							
							DeltaError = SlaveTime - CurrentTime;
							if(abs(DeltaError) > VIDEO_SYNC_OFFSET_DEVIATION)
							{
								Delay = i*10;
								LOG_INFO("--- FS < 0 --- key = %lu, CurrentTime = %lu, delta = %ld, Delay = %lu\n",
									SlaveTime,CurrentTime,DeltaError,Delay);
								LOG_INFO("--- FS < 0 --- Playback Start!\n");
								
								break;
							}
						}

						/*abs(offset) - delay - error*/
						m_QVideoSync_sp->Offset = DeltaOffset - Delay -180;
						
						LOG_INFO("--- FS < 0 --- New Offset = %ld\n",m_QVideoSync_sp->Offset);

						break;
					}
				}
			}
		}

		/*accurate sync*/
		if(m_QVideoSync_sp->Offset > 0)
		{
			iRet = m_QVideoSync_sp->setSyncTime(VIDEO_SYNC_MIN_PLAY_SPEED);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				goto EXIT_PROC;
			}	

			/*Delta Time = abs(Offset / (Vm - Vs))*/
			DeltaTime = abs(m_QVideoSync_sp->Offset)*VIDEO_SYNC_SAMPLE_TIME;
			usleep(DeltaTime);
			
			iRet = m_QVideoSync_sp->setSyncTime(VIDEO_SYNC_NORMAL_PLAY_SPEED);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				goto EXIT_PROC;
			}	
			
			LOG_INFO("--- RE --- Sync Complete!\n");
			m_QVideoSync_sp->SyncSeeking = false;
		}
		else if(m_QVideoSync_sp->Offset < 0) 
		{
			iRet = m_QVideoSync_sp->setSyncTime(0.8*VIDEO_SYNC_MAX_PLAY_SPEED);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				goto EXIT_PROC;
			}	
			
			/*Delta Time = abs(Offset / (Vm - Vs))*/
			DeltaTime = abs(m_QVideoSync_sp->Offset)*VIDEO_SYNC_SAMPLE_TIME / 0.8;
			usleep(DeltaTime);

			iRet = m_QVideoSync_sp->setSyncTime(VIDEO_SYNC_NORMAL_PLAY_SPEED);
			if(iRet)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
				goto EXIT_PROC;
			}	

			LOG_INFO("--- FF --- Sync Complete!\n");
			m_QVideoSync_sp->SyncSeeking = false;
		}
		
EXIT_PROC:
		;
		m_QVideoSync_sp->SyncMutex.unlock();
	}
}

