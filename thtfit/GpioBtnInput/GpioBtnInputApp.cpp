#include "GpioBtnInputApp.h"
#include <SignalInfo.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <memory.h>
#include "Mcp23008RegDef.h"
#include "GpioBtnInputEventDef.h"
#include "GpioBtnInputApi.h"
#include "DBusMessage.h"
#include "DbgLogSwitchDef.h"
#include <InvArgException.h>
#include <BaseErrException.h>
#include <SysBaseSrvApi.h>
#include <BitOpsMacro.h>
#include <math2.h>
#include <DateTime.h>
#include "Mp7xxGlobalStatus.h"
#include <BaseErrDef.h>
#include <nexus_i2c.h>
#include "nexus_platform.h"
#include <ProcessRunningInfo.h>
#include <stdlib.h> 

#define	BrcmGpioNo_BtnInterrupt				        (65)
#define	I2cGpio_I2cChannelNo				        (4)
#if	PLATFORM == 97252
#define	I2cGpio_I2cDevAddr					(0x20)
#elif	PLATFORM == 97445
#define	I2cGpio_I2cDevAddr					(0x20)
#elif   PLATFORM == 97439
#define I2cGpio_I2cDevAddr                                      (0x20)
#else
#error No def, stop.
#endif

#define	EvtId_GpioInterrupt					(EVT_ID_USER+100)

static LPCSTR g_pszGpioBtnInput_Introspect = 
	DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
	"<node name=\"" DBUS_GpioBtnInput_OBJECT_PATH "\">\n"
	"	<interface name=\"org.freedesktop.DBus.Introspectable\">\n"
	"		<method name=\"Introspect\">\n"
	"			<arg name=\"data\" direction=\"out\" type=\"s\"/>\n"
	"		</method>\n"
	"	</interface>\n"
	"	<interface name=\"" DBUS_GpioBtnInput_OBJECT_INTERFACE "\">\n"
	"		<method name=\"getGpioInterruptSource\">\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"			<arg name=\"IntSrcGpioNo\" type=\"u\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"getGpioInputStatus\">\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"			<arg name=\"GpioInputStatus\" type=\"u\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"getGpioDirection\">\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"			<arg name=\"GpioDirection\" type=\"u\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"setGpioDirection\">\n"
	"			<arg name=\"GpioDirection\" type=\"u\" direction=\"in\" />\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"setGpioOutputStatus\">\n"
	"			<arg name=\"GpioOutputStatus\" type=\"s\" direction=\"in\" />\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"setGpioOutputData\">\n"
	"			<arg name=\"GpioOutputData\" type=\"u\" direction=\"in\" />\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"		</method>\n"
	"		<method name=\"getSelectedInputGpioId\">\n"
	"			<arg name=\"iOutRet\" type=\"i\" direction=\"out\" />\n"
	"			<arg name=\"SelectedInputGpioId\" type=\"u\" direction=\"out\" />\n"
	"		</method>\n"
	"		<signal name=\"ServiceStarted\">\n"
	"		</signal>\n"
	"		<signal name=\"GpioInputInterrupt\">\n"
	"			<arg name=\"GpioNo\" direction=\"out\" type=\"i\"/>\n"
	"			<arg name=\"PressDown\" direction=\"out\" type=\"i\"/>\n"
	"		</signal>\n"
	"	</interface>\n"
	"</node>\n";	

CGpioInputEventData::CGpioInputEventData()
{
	m_GpioBtnStatusChangeData = 0;
}

CGpioInputEventData::~CGpioInputEventData()
{

}

CGpioOutputTimingData::CGpioOutputTimingData()
{
	m_iPulseWidthMs = 10;
	m_bHighPulse = FALSE;
	m_uiTimerId = 0;
	m_iOutputGpioId = -1;
	m_iWaveStage = 0;	//invalid
}

CGpioOutputTimingData::~CGpioOutputTimingData()
{
}

CGpioBtnInputApp::CGpioBtnInputApp()
{
	INT_t iRet;
	
	m_BtnDataOutPipeFd = INVALID_FILE_DESCRIPTOR;
	m_OldGpioStatus = 0xFF;
	m_GpioBtnEvtDataPacket.LeftSizeToSend = 0;
	m_pDBusConn = NULL;
	m_bDBusNeedShutdown = FALSE;
	m_LastGpioInterruptSource = -1;
	m_GpioLogicalDirection = GPIO_LOGICAL_DIR_ALL_INPUT;
	m_iSelectedInputGpioId = -1;	//invalid
	m_iSelectedOutputGpioId = -1;	//invalid
	m_iGpioOutputPulseWidth = 0;
	m_bNxClientJoined = FALSE;
	m_hGpio_BtnInt = NULL;
	m_hNexusI2c = NULL;
	m_GpioInterruptCnt = 0;
	m_GpioInterruptStartTimeMs = GetSysUpTimeMs();

	iRet = RegDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

CGpioBtnInputApp::~CGpioBtnInputApp()
{
	int iRet;

	if(m_hGpio_BtnInt)
	{
		NEXUS_Gpio_Close(m_hGpio_BtnInt);
		m_hGpio_BtnInt = NULL;
	}

	if(m_hNexusI2c)
	{
		NEXUS_I2c_Close(m_hNexusI2c);
		m_hNexusI2c = NULL;
	}

	iRet = disconnectDBus();
	if(ERROR_SUCCESS != iRet)
	{			
		PRINT_BFILE_LINENO_IRET_STR;
	}

	iRet = remove(BTN_IN_SRV_PIPE_PATH);
	if(0 != iRet)
	{
		if(ENOENT != errno)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
	}

	if(INVALID_FILE_DESCRIPTOR != m_BtnDataOutPipeFd)
	{
		iRet = close(m_BtnDataOutPipeFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
		}
		m_BtnDataOutPipeFd = INVALID_FILE_DESCRIPTOR;
	}

	if(m_bDBusNeedShutdown)
	{
		m_bDBusNeedShutdown = FALSE;
		dbus_shutdown();
	}

	iRet = UnregDbgLogSwitch();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

void CGpioBtnInputApp::SigHandler(int iSigNo)
{
	INT iRet;
	CGpioBtnInputApp * pApp = (typeof(pApp))GetApp();

	printf("Signal:%s\n", CSignalInfo::GetSingalDescFromSigNo(iSigNo));
	if(SIGINT == iSigNo || SIGTERM == iSigNo || SIGQUIT == iSigNo)
	{
		if(pApp)
		{
			iRet = pApp->PostMessage(EVT_ID_QUIT_EVENT_LOOP, 0, NULL);
		}
	}
}

static dbus_bool_t DBusAddWatchCb(DBusWatch * pDbusWatch, void * pData)
{
	dbus_bool_t bDbusRet = TRUE;
	CGpioBtnInputApp * pApp = (typeof(pApp))pData;

	do
	{
		if(NULL == pDbusWatch || NULL == pApp)
		{
			bDbusRet = FALSE;
			break;
		}
		bDbusRet = pApp->DBusAddWatch(pDbusWatch);
	}while(FALSE);

	return bDbusRet;
}

dbus_bool_t CGpioBtnInputApp::DBusAddWatch(DBusWatch * pDbusWatch)
{
	int iRet;
	dbus_bool_t bDbusRet = TRUE, bRet;
	int iFd;
	unsigned int WatchFlags;
	EVENT_SELECT_FLAG SelectFlags = EVENT_SELECT_NONE;

	do
	{
		iFd =  dbus_watch_get_unix_fd(pDbusWatch);
		if(0 > iFd)
		{
			bDbusRet = FALSE;
			break;
		}
		iRet = UnregisterFdEventSelect(iFd);
		if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
		bRet = dbus_watch_get_enabled(pDbusWatch);
		if(FALSE == bRet)	//disabled already
		{
			break;
		}
		WatchFlags = dbus_watch_get_flags(pDbusWatch);
		if(WatchFlags & DBUS_WATCH_READABLE)
		{
			SelectFlags = (typeof(SelectFlags))(SelectFlags | EVENT_SELECT_READ);
		}
		if(WatchFlags & DBUS_WATCH_WRITABLE)
		{
			SelectFlags = (typeof(SelectFlags))(SelectFlags | EVENT_SELECT_WRITE);
		}
		iRet = RegisterFdEventSelect(iFd, SelectFlags);
		if(ERROR_SUCCESS != iRet)
		{
			bDbusRet = FALSE;
			break;
		}
		iRet = m_DbusFdArray.AddIfNotExist(iFd);
		if(ERROR_SUCCESS != iRet)
		{
			bDbusRet = FALSE;
			break;
		}
	}while(FALSE);

	if(FALSE == bDbusRet)
	{
		iRet = UnregisterFdEventSelect(iFd);
		if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
	}
	
	return bDbusRet;
}

static void DBusRemoveWatchCb(DBusWatch * pDbusWatch, void * pData)
{
	CGpioBtnInputApp * pApp = (typeof(pApp))pData;

	do
	{
		if(NULL == pDbusWatch || NULL == pApp)
		{
			break;
		}
		pApp->DBusRemoveWatch(pDbusWatch);
	}while(FALSE);
}

void CGpioBtnInputApp::DBusRemoveWatch(DBusWatch * pDbusWatch)
{
	int iFd, iRet;

	do
	{
		iFd =  dbus_watch_get_unix_fd(pDbusWatch);
		if(0 > iFd)
		{
			break;
		}
		iRet = UnregisterFdEventSelect(iFd);
		if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
		iRet = m_DbusFdArray.RemoveIfExist(iFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
			break;
		}
	}while(FALSE);
}

/* called from other threads */
void CGpioBtnInputApp::BtnInterruptCb(void * pContext, int param)
{
	CGpioBtnInputApp * pThis = (typeof(pThis))pContext;
	pThis->onBtnInterrupt(param);
}

/* called from other threads */
void CGpioBtnInputApp::onBtnInterrupt(int param)
{
	INT_t iRet;
	//LOG_BLINE("onBtnInterrupt\n");
	iRet = PostMessage(EvtId_GpioInterrupt, 0, 0);
	if(ERR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;
	}
}

INT CGpioBtnInputApp::InitInstance(int argc, char * argv[])
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	NEXUS_Error nexusErr;

	do
	{
		//Catch SIGINT and SIGTERM signals
		{
			sighandler_t pOldSigHandler = NULL;
			
			pOldSigHandler = signal(SIGINT, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pOldSigHandler = signal(SIGTERM, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
			pOldSigHandler = signal(SIGQUIT, SigHandler);
			if(SIG_ERR == pOldSigHandler)
			{
				PRINT_FILE_LINENO_CRT_ERRINFO;
			}
		}

		{
			if(FALSE == dbus_threads_init_default())
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			m_bDBusNeedShutdown = TRUE;
		}

		iRet = ConnectDBus();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			LOG("ConnectDBus err(%d)\n", iRet);
			break;
		}

		{
			UINT64_t uSysUpTimeMs, uSysUpStartTimeMs;
			NxClient_JoinSettings oNxJoinSettings;
			NxClient_GetDefaultJoinSettings(&oNxJoinSettings);
			snprintf(oNxJoinSettings.name, sizeof(oNxJoinSettings.name), "%s", "GpioBtnInput");
			oNxJoinSettings.name[sizeof(oNxJoinSettings.name)-1] = '\0';
			oNxJoinSettings.tunnelCapable = FALSE;
			oNxJoinSettings.timeout = 0;
			oNxJoinSettings.session = 0;
			uSysUpStartTimeMs = GetSysUpTimeMs();
RETRY_NX_JOIN:
			nexusErr = NxClient_Join(&oNxJoinSettings);
			if(NEXUS_SUCCESS != nexusErr)
			{
				//LOG_BLINE("nexusErr=%d\n", nexusErr);
				uSysUpTimeMs = GetSysUpTimeMs();
				if((15*1000) > (uSysUpTimeMs - uSysUpStartTimeMs))
				{
					if(TRUE == NxserverIsRunning())
					{
						//LOG_BLINE("nxserver is running, retry.\n");
						sleep(1);
						goto RETRY_NX_JOIN;
					}
				}
				iOutRet = ERR_NxClientJoin_FAILED;
				goto EXIT_PROC;
			}
			m_bNxClientJoined = TRUE;
		}

		if(NULL == m_hGpio_BtnInt)
		{			
			NEXUS_GpioSettings gpioSettings;
			NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eStandard, &gpioSettings);
			gpioSettings.mode = NEXUS_GpioMode_eInput;
			gpioSettings.value = NEXUS_GpioValue_eHigh;
			gpioSettings.interruptMode = NEXUS_GpioInterrupt_eLow;
			gpioSettings.interrupt.callback = BtnInterruptCb;
			gpioSettings.interrupt.context = this;
			gpioSettings.interrupt.param = NULL;
			gpioSettings.maskEdgeInterrupts = FALSE;			
			/*
			gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
			gpioSettings.value = NEXUS_GpioValue_eLow;
			gpioSettings.interruptMode = NEXUS_GpioInterrupt_eLow;
			gpioSettings.interrupt.callback = BtnInterruptCb;
			gpioSettings.interrupt.context = this;
			gpioSettings.interrupt.param = NULL;
			gpioSettings.maskEdgeInterrupts = FALSE;			
			*/
			m_hGpio_BtnInt = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_BtnInterrupt, &gpioSettings);
			if(NULL == m_hGpio_BtnInt)
			{
				iOutRet = ERR_OpenNexusGpio_FAIL;
				goto EXIT_PROC;
			}
		}

		{
			NEXUS_I2cSettings nexusI2cSettings;
			NEXUS_I2c_GetDefaultSettings(&nexusI2cSettings);
			nexusI2cSettings.clockRate = NEXUS_I2cClockRate_e100Khz;
			nexusI2cSettings.interruptMode = true;
			nexusI2cSettings.burstMode = false;
			nexusI2cSettings.timeout = 0;	//use defalt
			nexusI2cSettings.softI2c = false;
			nexusI2cSettings.fourByteXferMode = false;
			nexusI2cSettings.use5v = false;
			m_hNexusI2c = NEXUS_I2c_Open(I2cGpio_I2cChannelNo, &nexusI2cSettings);
			if(NULL == m_hNexusI2c)
			{
				iOutRet = ERR_OpenNexusI2c_FAIL;
				goto EXIT_PROC;
			}
		}

		//Create a named pipe
		iRet = remove(BTN_IN_SRV_PIPE_PATH);
		if(0 != iRet)
		{
			if(ENOENT != errno)
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				iOutRet = ERR_SYS_remove_FAIL;
				break;
			}
		}
		iRet = mkfifo(BTN_IN_SRV_PIPE_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if(-1 == iRet)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			iOutRet = ERR_SYS_mkfifo_FAIL;
			break;
		}
		m_BtnDataOutPipeFd = open(BTN_IN_SRV_PIPE_PATH, O_RDWR | O_NONBLOCK);
		if(0 > m_BtnDataOutPipeFd)
		{
			PRINT_BFILE_LINENO_CRT_ERRINFO;
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		//Get m_GpioLogicalDirection from setting
		{
			DECLARE_CLS_STACK_BUF_STRING(strValue, 64);
			iRet = SysProp_get(SysProp_KEY_GpioLogicalDir, OUT strValue);
			if(ERROR_SUCCESS == iRet)
			{
				CStackBufString::SCANF_PARAM oScanfParam;
				CStackBufString::ScanfParam_Init(&oScanfParam);
				iRet = strValue.Scanf(IN OUT oScanfParam, "%x", &m_GpioLogicalDirection);
				if(ERROR_SUCCESS == iRet)
				{
				}
				else
				{
					m_GpioLogicalDirection = GPIO_LOGICAL_DIR_ALL_INPUT;
				}
			}
			else
			{
				if(ERROR_NOT_FOUND == iRet)
				{
				}
				else
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				m_GpioLogicalDirection = GPIO_LOGICAL_DIR_ALL_INPUT;
			}
		}

		iRet = setGpioDirection(m_GpioLogicalDirection);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			LOG_BLINE("setGpioDir err %d\n", iRet);
			goto EXIT_PROC;
		}

		//Send a dbus signal ServiceStarted
		iRet = SendDbusSig_ServiceStarted();
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

EXIT_PROC:

	return iOutRet;
}

INT CGpioBtnInputApp::EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam)
{
	INT iOutRet = ERROR_SUCCESS;

	switch(uiEvtId)
	{
		case EVT_ID_FD_READ:
		{
			int iFd = uiParam;
			OnFdRead(iFd);
			break;
		}
		case EVT_ID_FD_WRITE:
		{
			int iFd = uiParam;
			OnFdWrite(iFd);
			break;
		}
		case EVT_ID_TIMER:
		{
			UINT uiTimerId = uiParam;
			/*
			PRINT_FILE_LINENO;
			printf("Timer Id:%u, CurTime:%llu ms\n", uiTimerId, GetCurTimeMs());
			*/	
			OnTimer(uiTimerId);

			break;
		}
		case EvtId_GpioInterrupt:
		{
			chkGpioBtnInput();
			break;
		}
		default:
		{
			iOutRet = ERROR_NOT_SUPPORTED;
		}
	}
 
	return iOutRet;
}

VOID CGpioBtnInputApp::OnFdRead(int iFd)
{
	if(m_DbusFdArray.Exist(iFd))
	{
		On_DbusMsg();
	}
	else	//BUG
	{
		PRINT_BFILE_LINENO_BUG_STR;
	}
}

VOID CGpioBtnInputApp::OnFdWrite(int iFd)
{
	if(iFd == m_BtnDataOutPipeFd)
	{
		On_BtnDataOutPipe_Write();
	}
}

void CGpioBtnInputApp::chkGpioBtnInput()
{
	INT_t iRet;
	NEXUS_Error nexusErr;
	UINT8_t NewGpioStatus;
	BOOL_t bPressDown;
	BOOL_t bErrInterrupt = FALSE;

	m_GpioInterruptCnt++;

	//Reading GPIOA results in clear the interrupt line INTA
	nexusErr = NEXUS_I2c_Read(m_hNexusI2c, I2cGpio_I2cDevAddr, MCP23008_RegAddr_GPIO, &NewGpioStatus, 1);
	if(NEXUS_SUCCESS != nexusErr)
	{
		LOG_BLINE("i2c R err %d\n", nexusErr);
		goto EXIT_PROC;
	}
	if(TRUE == Sw_LogGpioButton)
	{
		LOG_BLINE("NewGpioVal=0x%02x\n", NewGpioStatus);
	}
	if(NewGpioStatus != m_OldGpioStatus)
	{
		//LOG_BLINE("OldGpio:0x%02x,NewGpio:0x%02x\n", m_OldGpioStatus, NewGpioStatus);
		do
		{
			try
			{
				if((0 == (m_GpioLogicalDirection & (1<<0))) && ((NewGpioStatus & (1<<0)) != (m_OldGpioStatus & (1<<0))))	//Bit0
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<0))? GPIO_BTN_0_UP : GPIO_BTN_0_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<0))? "GPIO_BTN_0_UP" : "GPIO_BTN_0_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 0;
					if(GPIO_BTN_0_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<1))) && ((NewGpioStatus & (1<<1)) != (m_OldGpioStatus & (1<<1))))	//Bit1
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<1))? GPIO_BTN_1_UP : GPIO_BTN_1_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<1))? "GPIO_BTN_1_UP" : "GPIO_BTN_1_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 1;
					if(GPIO_BTN_1_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<2))) && ((NewGpioStatus & (1<<2)) != (m_OldGpioStatus & (1<<2))))	//Bit2
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<2))? GPIO_BTN_2_UP : GPIO_BTN_2_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<2))? "GPIO_BTN_2_UP" : "GPIO_BTN_2_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 2;
					if(GPIO_BTN_2_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<3))) && ((NewGpioStatus & (1<<3)) != (m_OldGpioStatus & (1<<3))))	//Bit3
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<3))? GPIO_BTN_3_UP : GPIO_BTN_3_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<3))? "GPIO_BTN_3_UP" : "GPIO_BTN_3_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 3;
					if(GPIO_BTN_3_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<4))) && ((NewGpioStatus & (1<<4)) != (m_OldGpioStatus & (1<<4))))	//Bit4
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<4))? GPIO_BTN_4_UP : GPIO_BTN_4_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<4))? "GPIO_BTN_4_UP" : "GPIO_BTN_4_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 4;
					if(GPIO_BTN_4_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<5))) && ((NewGpioStatus & (1<<5)) != (m_OldGpioStatus & (1<<5))))	//Bit5
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<5))? GPIO_BTN_5_UP : GPIO_BTN_5_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<5))? "GPIO_BTN_5_UP" : "GPIO_BTN_5_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 5;
					if(GPIO_BTN_5_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<6))) && ((NewGpioStatus & (1<<6)) != (m_OldGpioStatus & (1<<6))))	//Bit6
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<6))? GPIO_BTN_6_UP : GPIO_BTN_6_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<6))? "GPIO_BTN_6_UP" : "GPIO_BTN_6_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 6;
					if(GPIO_BTN_6_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				if((0 == (m_GpioLogicalDirection & (1<<7))) && ((NewGpioStatus & (1<<7)) != (m_OldGpioStatus & (1<<7))))	//Bit7
				{
					SharedPtr<CGpioInputEventData> GpioInputEventData_sp(new CGpioInputEventData);
					if(GpioInputEventData_sp.isNull())
					{
						break;
					}
					GpioInputEventData_sp->m_GpioBtnStatusChangeData = 
						((NewGpioStatus & (1<<7))? GPIO_BTN_7_UP : GPIO_BTN_7_DOWN);
					//LOG_BLINE("%s\n", ((NewGpioStatus & (1<<7))? "GPIO_BTN_7_UP" : "GPIO_BTN_7_DOWN"));
					m_lstGpioInputEventData.push_back(GpioInputEventData_sp);
					m_LastGpioInterruptSource = 7;
					if(GPIO_BTN_7_DOWN == GpioInputEventData_sp->m_GpioBtnStatusChangeData)
					{
						bPressDown = TRUE;
					}
					else
					{
						bPressDown = FALSE;
					}
					iRet = SendGpioInputInterruptDbusSig(m_LastGpioInterruptSource, bPressDown);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
			}
			catch(std::bad_alloc & BadAllocEx)
			{
				break;
			}
			//lmitation checking
			while(GPIO_IN_EVT_DATA_QUEUE_MAX_LEN < m_lstGpioInputEventData.size())
			{
				m_lstGpioInputEventData.erase(m_lstGpioInputEventData.begin());
			}

			if(0 < m_GpioBtnEvtDataPacket.LeftSizeToSend || 0 < m_lstGpioInputEventData.size())
			{					
				iRet = RegisterFdEventSelect(m_BtnDataOutPipeFd, EVENT_SELECT_WRITE);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
					break;
				}
			}
		}while(FALSE);
		m_OldGpioStatus = NewGpioStatus;
	}

EXIT_PROC:

	if(1000 <= m_GpioInterruptCnt)
	{
		UINT64_t curTimeMs = GetSysUpTimeMs();
		UINT64_t elapsedTimeMs = (curTimeMs - m_GpioInterruptStartTimeMs);
		if(1000 <= elapsedTimeMs)
		{
			UINT64_t interruptTimesPerSecond = m_GpioInterruptCnt * 1000 / elapsedTimeMs;
			if(50 <= interruptTimesPerSecond)
			{
				LOG_BLINE("Err interrupt,will be disabled.\n");
				bErrInterrupt = TRUE;
			}
		}
	}

	if(FALSE == bErrInterrupt)
	{
		nexusErr = NEXUS_Gpio_ClearInterrupt(m_hGpio_BtnInt);
		if(NEXUS_SUCCESS != nexusErr)
		{
			LOG_BLINE("i2c R err %d\n", nexusErr);
		}
	}
}

VOID CGpioBtnInputApp::On_BtnDataOutPipe_Write()
{
	INT_t iRet;
	ssize_t sizeWritten;
	SharedPtr<CGpioInputEventData> GpioInputEventData_sp;

	while(TRUE)
	{
		while(TRUE)
		{
			if(0 < m_GpioBtnEvtDataPacket.LeftSizeToSend)
			{
				sizeWritten = write(m_BtnDataOutPipeFd, 
					m_GpioBtnEvtDataPacket.PacketData+(sizeof(m_GpioBtnEvtDataPacket.PacketData)-m_GpioBtnEvtDataPacket.LeftSizeToSend),
					m_GpioBtnEvtDataPacket.LeftSizeToSend);
				if(0 < sizeWritten)
				{
					//LOG_BLINE("Sent %d\n", sizeWritten);
					m_GpioBtnEvtDataPacket.LeftSizeToSend -= sizeWritten;
				}
				else if(0 == sizeWritten)
				{
					LOG_BLINE("0 written\n");
					break;
				}
				else
				{
					if(EAGAIN == errno)
					{
						break;
					}
					else
					{
						LOG_BLINE("%d,%s\n", errno, strerror(errno));
						m_GpioBtnEvtDataPacket.LeftSizeToSend = 0;
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
		if(0 < m_GpioBtnEvtDataPacket.LeftSizeToSend)
		{
			break;
		}

		if(0 < m_lstGpioInputEventData.size())
		{
			GpioInputEventData_sp = *(m_lstGpioInputEventData.begin());
			m_lstGpioInputEventData.pop_front();
			if(GpioInputEventData_sp.isValid())
			{
				m_GpioBtnEvtDataPacket.LeftSizeToSend = sizeof(m_GpioBtnEvtDataPacket.PacketData);
				UINT16_t PacketSize = sizeof(m_GpioBtnEvtDataPacket.PacketData);
				memcpy(&(m_GpioBtnEvtDataPacket.PacketData[0]), &PacketSize, sizeof(PacketSize));
				m_GpioBtnEvtDataPacket.PacketData[2] = MSG_ID_GPIO_INPUT_EVENT_DATA;
				m_GpioBtnEvtDataPacket.PacketData[3] = GpioInputEventData_sp->m_GpioBtnStatusChangeData;
			}
		}
		else
		{
			break;
		}
	}

	if(0 >= m_GpioBtnEvtDataPacket.LeftSizeToSend && 0 >= m_lstGpioInputEventData.size())
	{
		iRet = UnregisterFdEventSelect(m_BtnDataOutPipeFd);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_IRET_CRT_ERR;
		}
	}
}

INT_t CGpioBtnInputApp::ConnectDBus()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DBusError DBusErr;
	dbus_bool_t bDbusRet;
	
	do
	{
		//Server connection
		dbus_error_init(&DBusErr);
		m_pDBusConn = dbus_bus_get_private(DBUS_BUS_SYSTEM, &DBusErr);
		if(NULL == m_pDBusConn)
		{
			iOutRet = ERR_CONNECTION_FAIL;
			break;
		}
		if(dbus_error_is_set(&DBusErr))
		{			
			iOutRet = ERR_CONNECTION_FAIL;
			dbus_error_free(&DBusErr);
			break;
		}

		iRet = dbus_bus_request_name(m_pDBusConn, DBUS_GpioBtnInput_ServiceName, 0, &DBusErr);
		if(-1 == iRet)
		{
			iOutRet = ERR_SETUP_FAIL;
			dbus_error_free(&DBusErr);
			break;
		}
		else if(2 == iRet)
		{
			iOutRet = ERR_SETUP_FAIL;
			LOG_BLINE("Srv \"%s\" reg fail\n", DBUS_GpioBtnInput_ServiceName);
			break;
		}

		dbus_connection_set_exit_on_disconnect(m_pDBusConn, FALSE);
		//Register fd event callbacks
		bDbusRet = dbus_connection_set_watch_functions(m_pDBusConn, DBusAddWatchCb, DBusRemoveWatchCb, NULL, this, NULL);
		if(FALSE == bDbusRet)
		{
			iOutRet = ERR_CONNECTION_FAIL;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::disconnectDBus()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	dbus_bool_t bDbusRet;
	DBusError DBusErr;

	do
	{
		dbus_error_init(&DBusErr);

		if(m_pDBusConn)
		{
			iRet = dbus_bus_release_name(m_pDBusConn, DBUS_GpioBtnInput_ServiceName, &DBusErr);
			if(-1 == iRet)
			{
				iOutRet = ERR_REMOVE_REG_INFO_FAIL;
				dbus_error_free(&DBusErr);
				break;
			}

			//no more watching
			bDbusRet = dbus_connection_set_watch_functions(m_pDBusConn, NULL, NULL, NULL, this, NULL);
			if(FALSE == bDbusRet)
			{
				LOG_BLINE("DbusConnSetWatchFunc(NULL) failed\n");
			}

			dbus_connection_close(m_pDBusConn);
			dbus_connection_unref(m_pDBusConn);
			m_pDBusConn = NULL;
		}
	}while(FALSE);
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::On_DbusMsg()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	dbus_bool_t bDBusRet;
	DBusMessage * pDBusMsg = NULL;
	int TimeoutMs;

	do
	{
		TimeoutMs = 0;
		bDBusRet = dbus_connection_read_write(m_pDBusConn, TimeoutMs);
		if(FALSE == bDBusRet)	//the connection is closed
		{
			iOutRet = ERR_SCK_CONN_CLOSED_BY_PEER;
			break;
		}
		while(TRUE)
		{
			pDBusMsg = dbus_connection_pop_message(m_pDBusConn);
			if(pDBusMsg)
			{
				/*
				LOG_BLINE("dbus_message,type:%d,path:%s,interface:%s,member:%s,errorname:%s,destination:%s,sender:%s\n", 
					dbus_message_get_type(pDBusMsg), dbus_message_get_path(pDBusMsg),
					dbus_message_get_interface(pDBusMsg), dbus_message_get_member(pDBusMsg),
					dbus_message_get_error_name(pDBusMsg),	dbus_message_get_destination(pDBusMsg),
					dbus_message_get_sender(pDBusMsg));
				*/
				iRet = ProcessDBusMsg(pDBusMsg);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				dbus_message_unref(pDBusMsg);
				pDBusMsg = NULL;
			}
			else	//no more data
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getGpioInterruptSource(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		
	}while(FALSE);

	(*DBusOutParamList_sp) << iOutRet;
	(*DBusOutParamList_sp) << (INT32_t)m_LastGpioInterruptSource;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getGpioDirection(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		
	}while(FALSE);

	(*DBusOutParamList_sp) << iOutRet;
	(*DBusOutParamList_sp) << m_GpioLogicalDirection;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_setGpioDirection(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet = ERR_SUCCESS;
	UINT32_t GpioLogicalDirection;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		GpioLogicalDirection = ((*DBusInParamList_sp)[0])->toUint32();
		iRet = setGpioDirection(GpioLogicalDirection);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_setGpioOutputData(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet;
	UINT32_t GpioOutputData;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		GpioOutputData = ((*DBusInParamList_sp)[0])->toUint32();
		iRet = setGpioOutputData(GpioOutputData);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_setGpioOutputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet, iId;
	UINT32_t GpioOutputData;
	DECLARE_CLS_STACK_BUF_STRING(strGpioOutputStatus, 32);

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		strGpioOutputStatus = ((*DBusInParamList_sp)[0])->toString();
		GpioOutputData = 0;
		for(iId = 0; iId < GET_MIN_VAL(strGpioOutputStatus.getStringLength(), (8 * sizeof(GpioOutputData))); iId++)
		{
			GpioOutputData |= (('0' != (LPCSTR(strGpioOutputStatus))[iId])? (1 << iId) : 0);
		}
		iRet = setGpioOutputStatus(GpioOutputData);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_selectInputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_iSelectedInputGpioId = ((*DBusInParamList_sp)[0])->toInt32();
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getSelectedInputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS;

	do
	{
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	(*DBusOutParamList_sp) << m_iSelectedInputGpioId;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_selectOutputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_iSelectedOutputGpioId = ((*DBusInParamList_sp)[0])->toInt32();
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getSelectedOutputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS;

	do
	{
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	(*DBusOutParamList_sp) << m_iSelectedOutputGpioId;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_readGpioInputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS, iRet;
	UINT32_t uiGpioInputStatus = 0x00000000;
	INT32_t bGpioInputStatus = 0;

	do
	{
		iRet = getGpioInputStatus(OUT uiGpioInputStatus);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
		if(0 > m_iSelectedInputGpioId || 31 < m_iSelectedInputGpioId)
		{
			iReplyRet = ERROR_INVALID_STATE;
			break;
		}
		bGpioInputStatus = (uiGpioInputStatus & (1 << m_iSelectedInputGpioId))?1:0;
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	(*DBusOutParamList_sp) << bGpioInputStatus;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_setGpioOutputSingleBit(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS, iRet;
	BOOL_t bGpioOutputBitStatus = 0;
	UINT32_t uiGpioOutputData = 0;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 > m_iSelectedOutputGpioId || 31 < m_iSelectedOutputGpioId)
		{
			iReplyRet = ERROR_INVALID_STATE;
			break;
		}
		bGpioOutputBitStatus = ((*DBusInParamList_sp)[0])->toInt32();
		if(bGpioOutputBitStatus)
		{
			uiGpioOutputData = (1 << m_iSelectedOutputGpioId);
		}
		iRet = setGpioOutputStatus(uiGpioOutputData, (1 << m_iSelectedOutputGpioId));
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_setGpioOutputPulseWidth(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS, iRet;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		m_iGpioOutputPulseWidth = ((*DBusInParamList_sp)[0])->toInt32();
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getGpioOutputPulseWidth(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iReplyRet = ERROR_SUCCESS, iRet;

	do
	{
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;
	(*DBusOutParamList_sp) << m_iGpioOutputPulseWidth;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_getGpioInputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet = ERROR_SUCCESS;
	UINT32_t uiGpioInputStatus = 0x00000000;
	DECLARE_CLS_STACK_BUF_STRING(strGpioInputStatus, 64);

	do
	{
		iRet = getGpioInputStatus(OUT uiGpioInputStatus);
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
		iRet = strGpioInputStatus.Format("%d%d%d%d%d%d%d%d", BIT_MASK_SHIFT0(uiGpioInputStatus,7),
			BIT_MASK_SHIFT0(uiGpioInputStatus,3), BIT_MASK_SHIFT0(uiGpioInputStatus,6), 
			BIT_MASK_SHIFT0(uiGpioInputStatus,2), BIT_MASK_SHIFT0(uiGpioInputStatus,5), 
			BIT_MASK_SHIFT0(uiGpioInputStatus,1), BIT_MASK_SHIFT0(uiGpioInputStatus,4),
			BIT_MASK_SHIFT0(uiGpioInputStatus,0));
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);

	(*DBusOutParamList_sp) << iReplyRet;	
	(*DBusOutParamList_sp) << (LPCSTR)strGpioInputStatus;
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::OnDBusCall_GenerateGpioOutputTiming(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iReplyRet = ERROR_SUCCESS;
	BOOL_t bHighTiming = FALSE;
	BOOL_t bFound = FALSE;
	GpioOutputTimingData_LIST::iterator itGpioOutputTimingData;
	SharedPtr <CGpioOutputTimingData> GpioOutputTimingData_sp;
	BOOL_t bRescan;

	do
	{
		if(1 > DBusInParamList_sp->getSize())
		{
			iReplyRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(0 > m_iSelectedOutputGpioId || 31 < m_iSelectedOutputGpioId)
		{
			iReplyRet = ERROR_INVALID_STATE;
			break;
		}
		bHighTiming = ((*DBusInParamList_sp)[0])->toInt32();
		bFound = FALSE;
		bRescan = TRUE;
		while(bRescan)
		{
			bRescan = FALSE;
			for(itGpioOutputTimingData = m_lstGpioOutputTimingData.begin(); itGpioOutputTimingData != m_lstGpioOutputTimingData.begin(); itGpioOutputTimingData++)
			{
				GpioOutputTimingData_sp = (*itGpioOutputTimingData);
				if(GpioOutputTimingData_sp.isNull())
				{
					m_lstGpioOutputTimingData.erase(itGpioOutputTimingData);
					bRescan = TRUE;
					break;
				}
				if(GpioOutputTimingData_sp->m_iOutputGpioId == m_iSelectedOutputGpioId)
				{
					GpioOutputTimingData_sp->m_iWaveStage = 1;
					GpioOutputTimingData_sp->m_iPulseWidthMs = (m_iGpioOutputPulseWidth * 100);
					GpioOutputTimingData_sp->m_bHighPulse = bHighTiming;
					if(0 < GpioOutputTimingData_sp->m_uiTimerId)
					{
						iRet = UnregisterTimer(GpioOutputTimingData_sp->m_uiTimerId);
						if(ERROR_SUCCESS != iRet)
						{
							PRINT_BFILE_LINENO_IRET_STR;
						}
						GpioOutputTimingData_sp->m_uiTimerId = 0;
					}
					iRet = RegisterTimer(GpioOutputTimingData_sp->m_iPulseWidthMs, OUT GpioOutputTimingData_sp->m_uiTimerId);
					if(ERROR_SUCCESS != iRet)
					{
						m_lstGpioOutputTimingData.erase(itGpioOutputTimingData);
						iReplyRet = iRet;
					}
					bFound = TRUE;
					break;
				}
			}
		}
		if(ERROR_SUCCESS != iReplyRet)
		{
			break;
		}
		if(FALSE == bFound)
		{
			GpioOutputTimingData_sp = SharedPtr <CGpioOutputTimingData> (new CGpioOutputTimingData);
			if(GpioOutputTimingData_sp.isValid())
			{
				GpioOutputTimingData_sp->m_iWaveStage = 1;
				GpioOutputTimingData_sp->m_iOutputGpioId = m_iSelectedOutputGpioId;
				GpioOutputTimingData_sp->m_iPulseWidthMs = (m_iGpioOutputPulseWidth * 100);
				GpioOutputTimingData_sp->m_bHighPulse = bHighTiming;
				iRet = RegisterTimer(GpioOutputTimingData_sp->m_iPulseWidthMs, OUT GpioOutputTimingData_sp->m_uiTimerId);
				if(ERROR_SUCCESS == iRet)
				{
					try
					{
						m_lstGpioOutputTimingData.push_back(GpioOutputTimingData_sp);
					}
					catch(std::bad_alloc & BadAllocEx)
					{
						iReplyRet = ERROR_OUT_OF_MEMORY;
					}
				}
				else
				{
					iReplyRet = iRet;
				}
			}
		}
		if(GpioOutputTimingData_sp.isNull())
		{
			iReplyRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		if(ERROR_SUCCESS != iReplyRet)
		{
			break;
		}
		//set the level in wave stage 1
		iRet = setGpioOutputStatus(
			((bHighTiming) ? 0x00000000 : (1 << GpioOutputTimingData_sp->m_iOutputGpioId)), 
			(1 << GpioOutputTimingData_sp->m_iOutputGpioId));
		if(ERROR_SUCCESS != iRet)
		{
			iReplyRet = iRet;
			break;
		}
	}while(FALSE);
	
	(*DBusOutParamList_sp) << iReplyRet;	
	
	return iOutRet;
}

INT_t CGpioBtnInputApp::ExitInstance()
{
	INT iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = disconnectDBus();
		if(ERROR_SUCCESS != iRet)
		{			
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(m_bDBusNeedShutdown)
		{
			m_bDBusNeedShutdown = FALSE;
			dbus_shutdown();
		}
	}while(FALSE);

	if(m_hNexusI2c)
	{
		NEXUS_I2c_Close(m_hNexusI2c);
		m_hNexusI2c = NULL;
	}

	if(m_hGpio_BtnInt)
	{
		NEXUS_Gpio_Close(m_hGpio_BtnInt);
		m_hGpio_BtnInt = NULL;
	}

	if(m_bNxClientJoined)
	{
		NxClient_Uninit();
		m_bNxClientJoined = FALSE;
	}

	return iOutRet;
}

INT_t CGpioBtnInputApp::DBusIntrospectableIf_Introspect(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
	SharedPtr <CDBusParameterList> & DBusOutParamList_sp)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DBusError DBusErr;
	SharedPtr <CDBusParameter_STRING> DBusParam_IntrospectXml_sp(new CDBusParameter_STRING);

	do
	{
		if(DBusParam_IntrospectXml_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		DBusParam_IntrospectXml_sp->setValue(g_pszGpioBtnInput_Introspect);
		{
			SharedPtr<CDBusParameter> DBusParam_sp = DBusParam_IntrospectXml_sp;
			iRet = DBusOutParamList_sp->addParameter(DBusParam_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::SendGpioInputInterruptDbusSig(int GpioNo, int bPressDown)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DBusMessage * pDBusMsg = NULL;
	dbus_bool_t bDBusRet;
	SharedPtr <CDBusParameterList> DBusReplyParameterList_sp(new CDBusParameterList);

	do
	{		
		if(NULL == m_pDBusConn)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusReplyParameterList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		pDBusMsg = dbus_message_new_signal(DBUS_GpioBtnInput_OBJECT_PATH, DBUS_GpioBtnInput_OBJECT_INTERFACE, 
			DBUS_SIG_GpioInputInterrupt);
		if(NULL == pDBusMsg)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		(*DBusReplyParameterList_sp) << GpioNo;
		(*DBusReplyParameterList_sp) << bPressDown;
		
		(*DBusReplyParameterList_sp) >> pDBusMsg;
		bDBusRet = dbus_connection_send(m_pDBusConn, pDBusMsg, NULL);
		if(FALSE == bDBusRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	if(pDBusMsg)
	{
		dbus_message_unref(pDBusMsg);
		pDBusMsg = NULL;
	}

	return iOutRet;
}

INT_t CGpioBtnInputApp::SendDbusSig_ServiceStarted()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DBusMessage * pDBusMsg = NULL;
	dbus_bool_t bDBusRet;
	SharedPtr <CDBusParameterList> DBusReplyParameterList_sp(new CDBusParameterList);

	do
	{		
		if(NULL == m_pDBusConn)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusReplyParameterList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		pDBusMsg = dbus_message_new_signal(DBUS_GpioBtnInput_OBJECT_PATH, DBUS_GpioBtnInput_OBJECT_INTERFACE, 
			DBUS_SIG_ServiceStarted);
		if(NULL == pDBusMsg)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		
		(*DBusReplyParameterList_sp) >> pDBusMsg;
		bDBusRet = dbus_connection_send(m_pDBusConn, pDBusMsg, NULL);
		if(FALSE == bDBusRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	if(pDBusMsg)
	{
		dbus_message_unref(pDBusMsg);
		pDBusMsg = NULL;
	}

	return iOutRet;
}

INT_t CGpioBtnInputApp::setGpioDirection(UINT32_t GpioLogicalDirection)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strGpioLogicalDir, 32);
	NEXUS_Error nexusErr;

	//Cache the setting
	m_GpioLogicalDirection = GpioLogicalDirection;
	//Save the setting to persistent storage
	iRet = strGpioLogicalDir.Format("0x%08x", m_GpioLogicalDirection);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}
	iRet = SysProp_setPersistent(SysProp_KEY_GpioLogicalDir, (LPCSTR)strGpioLogicalDir);
	if(ERROR_SUCCESS != iRet)
	{
		iOutRet = iRet;
		goto EXIT_PROC;
	}

	//MCP23008 IODIR
	{
		BYTE gpioDirData = (~m_GpioLogicalDirection);
		nexusErr = NEXUS_I2c_Write(m_hNexusI2c, I2cGpio_I2cDevAddr, MCP23008_RegAddr_IODIR, &gpioDirData, 1);
		if(NEXUS_SUCCESS != nexusErr)
		{
			iOutRet = ERR_NexusI2cWrite_FAIL;
			goto EXIT_PROC;
		}
	}

	//enable MCP23008_GPIO interrupts
	{
		BYTE gpioInterruptEnableData = (~m_GpioLogicalDirection);
		nexusErr = NEXUS_I2c_Write(m_hNexusI2c, I2cGpio_I2cDevAddr, MCP23008_RegAddr_GPINTEN, &gpioInterruptEnableData, 1);
		if(NEXUS_SUCCESS != nexusErr)
		{
			iOutRet = ERR_NexusI2cWrite_FAIL;
			goto EXIT_PROC;
		}
	}
	
EXIT_PROC:
	return iOutRet;
}

INT_t CGpioBtnInputApp::setGpioOutputData(UINT32_t GpioOutputData)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	return iOutRet;
}

INT_t CGpioBtnInputApp::setGpioOutputStatus(UINT32_t GpioOutputData, UINT32_t BitMask/* = 0xFFFFFFFF*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	UINT32_t uiOldGpioOutputStatus = 0;


	return iOutRet;
}

INT_t CGpioBtnInputApp::getGpioInputStatus(OUT UINT32_t & uiGpioInputStatus)
{
	INT_t iOutRet = ERROR_SUCCESS;
	NEXUS_Error nexusErr;
	BYTE newGpioStatue;

	nexusErr = NEXUS_I2c_Read(m_hNexusI2c, I2cGpio_I2cDevAddr, MCP23008_RegAddr_GPIO, &newGpioStatue, 1);
	if(NEXUS_SUCCESS != nexusErr)
	{
		LOG_BLINE("i2c R err %d\n", nexusErr);
		uiGpioInputStatus = 0x00000000;
		goto EXIT_PROC;
	}
	uiGpioInputStatus = newGpioStatue;

EXIT_PROC:
	return iOutRet;
}

INT_t CGpioBtnInputApp::ProcessDBusMsg(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	const char * pszInterfaceName = NULL;
		
	do
	{
		pszInterfaceName = dbus_message_get_interface(pDBusMsg);
		//LOG_BLINE("DbusIfName=%s\n", pszInterfaceName);
		if(0 == strcmp(DBUS_GpioBtnInput_OBJECT_INTERFACE, pszInterfaceName))
		{
			iRet = ProcessDBusMsg_DefaultIfCall(pDBusMsg);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_INTERFACE_INTROSPECTABLE, pszInterfaceName))
		{
			iRet = ProcessDBusMsg_DBusIntrospectableIfCall(pDBusMsg);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_INTERFACE_DBUS, pszInterfaceName))
		{
			iRet = ProcessDBusMsg_DBusDBusIfCall(pDBusMsg);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else
		{
			iOutRet = CDBusMessage::SendErrReply(m_pDBusConn, pDBusMsg, 
				DBUS_ERROR_UNKNOWN_INTERFACE, "Unsupported interface call");
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::ProcessDBusMsg_DefaultIfCall(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int DBusMsgType;
		
	do
	{
		DBusMsgType = dbus_message_get_type(pDBusMsg);
		if(DBUS_MESSAGE_TYPE_METHOD_CALL == DBusMsgType)
		{
			iRet = ProcessDBusMsg_MethodCall(pDBusMsg);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(DBUS_MESSAGE_TYPE_SIGNAL == DBusMsgType)
		{
			const char * pszMethodame = NULL;
			pszMethodame = dbus_message_get_member(pDBusMsg);
			if(NULL == pszMethodame)
			{
				pszMethodame = "";
			}
			//LOG_BLINE("DbusSig,%s\n", pszMethodame);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::ProcessDBusMsg_DBusIntrospectableIfCall(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	int DBusMsgType;
		
	do
	{
		DBusMsgType = dbus_message_get_type(pDBusMsg);
		if(DBUS_MESSAGE_TYPE_METHOD_CALL == DBusMsgType)
		{
			iRet = DBusIntrospectableIf_MethodCall(pDBusMsg);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(DBUS_MESSAGE_TYPE_SIGNAL == DBusMsgType)
		{
			const char * pszMethodame = NULL;
			pszMethodame = dbus_message_get_member(pDBusMsg);
			if(NULL == pszMethodame)
			{
				pszMethodame = "";
			}
			LOG_BLINE("DBUS_MESSAGE_TYPE_SIGNAL,%s\n", pszMethodame);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::ProcessDBusMsg_DBusDBusIfCall(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS;
	int DBusMsgType;

	do
	{
		DBusMsgType = dbus_message_get_type(pDBusMsg);
		if(DBUS_MESSAGE_TYPE_SIGNAL == DBusMsgType)
		{
			const char * pszMethodame = NULL;
			pszMethodame = dbus_message_get_member(pDBusMsg);
			if(NULL == pszMethodame)
			{
				pszMethodame = "";
			}
			if(0 == strcmp("NameOwnerChanged", pszMethodame))
			{
				/*
				const char *pszArg0 = NULL, *pszArg1 = NULL, *pszArg2 = NULL;
				dbus_error_init(&DBusErr);
				bDBusRet =  dbus_message_get_args(pDBusMsg, &DBusErr, DBUS_TYPE_STRING, &pszArg0,
					DBUS_TYPE_STRING, &pszArg0, DBUS_TYPE_STRING, &pszArg1, DBUS_TYPE_STRING, &pszArg2,
					DBUS_TYPE_INVALID);
				if(false == bDBusRet)
				{
					if (dbus_error_is_set(&DBusErr)) 
					{
						dbus_error_free(&err);
					}
					//LOG_BLINE("can't get dbus_msg args\n");
				}
				//LOG_BLINE("DBUS_MESSAGE_TYPE_SIGNAL,%s,Args:%s,%s,%s\n", pszMethodame, pszArg0, pszArg1, pszArg2);
				*/
			}
			else if(0 == strcmp("NameAcquired", pszMethodame))
			{
			}
			else
			{
				LOG_BLINE("DBUS_MESSAGE_TYPE_SIGNAL,%s\n", pszMethodame);				
			}
		}
		else if(DBUS_MESSAGE_TYPE_METHOD_CALL == DBusMsgType)
		{
			iOutRet = CDBusMessage::SendErrReply(m_pDBusConn, pDBusMsg, 
				DBUS_ERROR_UNKNOWN_METHOD, "Unsupported method call");
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CGpioBtnInputApp::ProcessDBusMsg_MethodCall(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	const char * pszMethodame = NULL;
	SharedPtr <CDBusParameterList> DBusInParamList_sp(new CDBusParameterList);
	SharedPtr <CDBusParameterList> DBusOutParamList_sp(new CDBusParameterList);
	DBusMessage * pDBusMsgRet = NULL;
	dbus_bool_t bDBusRet;
		
	do
	{
		pDBusMsgRet = dbus_message_new_method_return(pDBusMsg);
		if(NULL == pDBusMsgRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}		

		if(NULL == DBusInParamList_sp.get() || NULL == DBusOutParamList_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pszMethodame = dbus_message_get_member(pDBusMsg);
		if(NULL == pszMethodame)
		{
			break;
		}
		if(Sw_LogDbusCmd)
		{
			LOG("Method: %s\n", pszMethodame);
		}
#ifdef	__EXCEPTIONS
		try
#endif	//__EXCEPTIONS
		{
			(*DBusInParamList_sp) << pDBusMsg;
		}
#ifdef	__EXCEPTIONS
		catch(CInvArgException & InvArgEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		catch(CBaseErrException & BaseErrEx)
		{
			iOutRet = BaseErrEx.getBaseErr();
			break;
		}
#endif	//__EXCEPTIONS
		
		if(0 == strcmp(DBUS_METHOD_getGpioInterruptSource, pszMethodame))
		{
			iRet = OnDBusCall_getGpioInterruptSource(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_getGpioDirection, pszMethodame))
		{
			iRet = OnDBusCall_getGpioDirection(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_setGpioDirection, pszMethodame))
		{
			iRet = OnDBusCall_setGpioDirection(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_setGpioOutputData, pszMethodame))
		{
			iRet = OnDBusCall_setGpioOutputData(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_setGpioOutputStatus, pszMethodame))
		{
			iRet = OnDBusCall_setGpioOutputStatus(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_selectInputGpioId, pszMethodame))
		{
			iRet = OnDBusCall_selectInputGpioId(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_getSelectedInputGpioId, pszMethodame))
		{
			iRet = OnDBusCall_getSelectedInputGpioId(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_selectOutputGpioId, pszMethodame))
		{
			iRet = OnDBusCall_selectOutputGpioId(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_getSelectedOutputGpioId, pszMethodame))
		{
			iRet = OnDBusCall_getSelectedOutputGpioId(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_readGpioInputStatus, pszMethodame))
		{
			iRet = OnDBusCall_readGpioInputStatus(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_setGpioOutputSingleBit, pszMethodame))
		{
			iRet = OnDBusCall_setGpioOutputSingleBit(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_setGpioOutputPulseWidth, pszMethodame))
		{
			iRet = OnDBusCall_setGpioOutputPulseWidth(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_getGpioOutputPulseWidth, pszMethodame))
		{
			iRet = OnDBusCall_getGpioOutputPulseWidth(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_getGpioInputStatus, pszMethodame))
		{
			iRet = OnDBusCall_getGpioInputStatus(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else if(0 == strcmp(DBUS_METHOD_GenerateGpioOutputTiming, pszMethodame))
		{
			iRet = OnDBusCall_GenerateGpioOutputTiming(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else
		{
			iOutRet = CDBusMessage::SendErrReply(m_pDBusConn, pDBusMsg, 
				DBUS_ERROR_UNKNOWN_METHOD, "Unsupported method call");
			break;
		}

#ifdef	__EXCEPTIONS
		try
#endif	//__EXCEPTIONS
		{
			(*DBusOutParamList_sp) >> pDBusMsgRet;
		}
#ifdef	__EXCEPTIONS
		catch(CInvArgException & InvArgEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		catch(CBaseErrException & BaseErrEx)
		{
			iOutRet = BaseErrEx.getBaseErr();
			break;
		}
		catch(...)
		{
			LOG_BLINE("BUG,uncaught exception\n");
		}
#endif	//__EXCEPTIONS

		bDBusRet = dbus_connection_send(m_pDBusConn, pDBusMsgRet, NULL);
		if(TRUE != bDBusRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	if(pDBusMsgRet)
	{
		dbus_message_unref(pDBusMsgRet);
		pDBusMsgRet = NULL;
	}

	return iOutRet;
}

INT_t CGpioBtnInputApp::DBusIntrospectableIf_MethodCall(DBusMessage * pDBusMsg)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	const char * pszMethodame = NULL;
	SharedPtr <CDBusParameterList> DBusInParamList_sp(new CDBusParameterList);
	SharedPtr <CDBusParameterList> DBusOutParamList_sp(new CDBusParameterList);
	DBusMessage * pDBusMsgRet = NULL;
	dbus_bool_t bDBusRet;
		
	do
	{
		pDBusMsgRet = dbus_message_new_method_return(pDBusMsg);
		if(NULL == pDBusMsgRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}		

		if(NULL == DBusInParamList_sp.get() || NULL == DBusOutParamList_sp.get())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		pszMethodame = dbus_message_get_member(pDBusMsg);
		if(NULL == pszMethodame)
		{
			break;
		}
		if(Sw_LogDbusCmd)
		{
			LOG("Method: %s\n", pszMethodame);
		}
#ifdef	__EXCEPTIONS
		try
#endif	//__EXCEPTIONS
		{
			(*DBusInParamList_sp) << pDBusMsg;
		}
#ifdef	__EXCEPTIONS
		catch(CInvArgException & InvArgEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		catch(CBaseErrException & BaseErrEx)
		{
			iOutRet = BaseErrEx.getBaseErr();
			break;
		}
#endif	//__EXCEPTIONS
		
		if(0 == strcmp(DBUS_METHOD_Introspect, pszMethodame))
		{
			iRet = DBusIntrospectableIf_Introspect(DBusInParamList_sp, DBusOutParamList_sp);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
			}
		}
		else
		{
			iOutRet = CDBusMessage::SendErrReply(m_pDBusConn, pDBusMsg, 
				DBUS_ERROR_UNKNOWN_METHOD, "Unsupported method call");
			break;
		}

#ifdef	__EXCEPTIONS
		try
#endif	//__EXCEPTIONS
		{
			(*DBusOutParamList_sp) >> pDBusMsgRet;
		}
#ifdef	__EXCEPTIONS
		catch(CInvArgException & InvArgEx)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		catch(CBaseErrException & BaseErrEx)
		{
			iOutRet = BaseErrEx.getBaseErr();
			break;
		}
#endif	//__EXCEPTIONS

		bDBusRet = dbus_connection_send(m_pDBusConn, pDBusMsgRet, NULL);
		if(TRUE != bDBusRet)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
	}while(FALSE);

	if(pDBusMsgRet)
	{
		dbus_message_unref(pDBusMsgRet);
		pDBusMsgRet = NULL;
	}

	return iOutRet;
}

VOID CGpioBtnInputApp::OnTimer(UINT_t uiTimerId)
{
	INT_t iRet;
	GpioOutputTimingData_LIST::iterator itGpioOutputTimingData;
	SharedPtr <CGpioOutputTimingData> GpioOutputTimingData_sp;
	BOOL_t bRescan;

	bRescan = TRUE;
	while(bRescan)
	{
		bRescan = FALSE;
		for(itGpioOutputTimingData = m_lstGpioOutputTimingData.begin(); itGpioOutputTimingData != m_lstGpioOutputTimingData.end(); itGpioOutputTimingData++)
		{
			GpioOutputTimingData_sp = *itGpioOutputTimingData;
			if(GpioOutputTimingData_sp.isNull())
			{
				continue;
			}
			if(GpioOutputTimingData_sp->m_uiTimerId == uiTimerId)
			{
				if(1 == GpioOutputTimingData_sp->m_iWaveStage)	//about to stage 2
				{
					//HighPulse -- low->high, LowPulse -- high->low
					iRet = setGpioOutputStatus(((GpioOutputTimingData_sp->m_bHighPulse) ? (1 << GpioOutputTimingData_sp->m_iOutputGpioId) : 0x00000000), 
						(1 << GpioOutputTimingData_sp->m_iOutputGpioId));
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					GpioOutputTimingData_sp->m_iWaveStage = 2;
					if(0 < GpioOutputTimingData_sp->m_uiTimerId)
					{
						iRet = UnregisterTimer(GpioOutputTimingData_sp->m_uiTimerId);
						if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
						{
							PRINT_BFILE_LINENO_IRET_STR;
						}
						GpioOutputTimingData_sp->m_uiTimerId = 0;
					}
					iRet = RegisterTimer(GpioOutputTimingData_sp->m_iPulseWidthMs, OUT GpioOutputTimingData_sp->m_uiTimerId);
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
				}
				else if(2 == GpioOutputTimingData_sp->m_iWaveStage)	//about to stage 3
				{
					//HighPulse -- high->low, LowPulse -- low->high
					iRet = setGpioOutputStatus(((GpioOutputTimingData_sp->m_bHighPulse) ? 0x00000000 : (1 << GpioOutputTimingData_sp->m_iOutputGpioId)), 
						(1 << GpioOutputTimingData_sp->m_iOutputGpioId));
					if(ERROR_SUCCESS != iRet)
					{
						PRINT_BFILE_LINENO_IRET_STR;
					}
					GpioOutputTimingData_sp->m_iWaveStage = 3;
					if(0 < GpioOutputTimingData_sp->m_uiTimerId)
					{
						iRet = UnregisterTimer(GpioOutputTimingData_sp->m_uiTimerId);
						if(ERROR_SUCCESS != iRet && ERROR_NOT_FOUND != iRet)
						{
							PRINT_BFILE_LINENO_IRET_STR;
						}
						GpioOutputTimingData_sp->m_uiTimerId = 0;
					}
					//erase, rescan
					m_lstGpioOutputTimingData.erase(itGpioOutputTimingData);
					bRescan = TRUE;
					break;
				}
				else
				{
					//erase, rescan
					m_lstGpioOutputTimingData.erase(itGpioOutputTimingData);
					bRescan = TRUE;					
					break;
				}
			}
		}
	}
}

int main(int argc, char * argv[])
{
    int iProcessExitCode = 0, iRet;
    

    pid_t iPid=-1;
    
        
    printf("start service %s ...\n",argv[0]);
    iPid=fork();
    if(iPid>0)
    {
        printf("parent exit\n");
        exit(0);
    }
    if(iPid<0)
    {
        printf("%s fork failed\n",argv[0]);
        exit(-1);
    }
    printf("service %s init\n",argv[0]);
    setsid();
#if 0
    do
    {
        int iNullFd=-1;
        int iRetFd=-1;
        const int iStdInFd=0;
        const int iStdOutFd=1;
        const int iStdErrFd=2;
        iNullFd=open("/dev/null", O_RDWR);
        if(iNullFd<0)
        {
            printf("%s open /dev/null failed\n",argv[0]);
            break;
        }

        iRetFd=dup2(iNullFd, iStdInFd);
        if(iRetFd<0)
        {
            printf("%s dup2 stdin failed\n",argv[0]);
            break;
        }
        iRetFd=dup2(iNullFd, iStdOutFd);
        iRetFd=dup2(iNullFd, iStdErrFd);

        if(iNullFd>2)
        {
            close(iNullFd);
        }           
    }while(false);
#endif

    {
        CGpioBtnInputApp oGpioBtnInputApp;
        iRet = oGpioBtnInputApp.Run(argc, argv);
        if(ERROR_SUCCESS != iRet)
        {
            iProcessExitCode = GBI_PEC_ERROR;
        }
        printf("service %s exit\n",argv[0]);
    }

    return iProcessExitCode;
}


