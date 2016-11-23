#ifndef	_GPIO_BTN_INPUT_APP_H_
#define	_GPIO_BTN_INPUT_APP_H_

#include <EventApp.h>
#include <SharedPtr.h>
#include <list>
#include <dbus/dbus.h>
#include <Array.h>
#include <DBusParameterList.h>
#include <nxclient.h>

//ProcessExitCodes
#define	GBI_PEC_ERROR			1

#define	BTN_INPUT_MCP23017_I2C_ADDR			0x48
#define	I2C_MODULE_INDEX_SOFTWARE			0
#define	I2C_MODULE_INDEX_HARDWARE			1
#define	I2C_MODULE_INDEX_INTERNAL			2

#define	GPIO_BTN_INPUT_DEV_PATH			"/dev/Mp7xxGpioBtnInputDevice"

#define	GPIO_IN_EVT_DATA_QUEUE_MAX_LEN		200

using namespace std;
using namespace CppBase;

typedef struct
{
	BYTE PacketData[4];
	size_t LeftSizeToSend;
}GPIO_BTN_EVT_DATA_PACKET, *P_GPIO_BTN_EVT_DATA_PACKET;

class CGpioInputEventData
{
public:
	CGpioInputEventData();
	virtual ~CGpioInputEventData();
public:
	int m_GpioBtnStatusChangeData;
};

typedef list < SharedPtr<CGpioInputEventData> >	GpioInputEventData_LIST;

static dbus_bool_t DBusAddWatchCb(DBusWatch * pDbusWatch, void * pData);
static void DBusRemoveWatchCb(DBusWatch * pDbusWatch, void * pData);

class CGpioOutputTimingData
{
public:
	CGpioOutputTimingData();
	~CGpioOutputTimingData();
public:
	INT_t m_iOutputGpioId;
	INT_t m_iPulseWidthMs;
	BOOL_t m_bHighPulse;
	UINT_t m_uiTimerId;
	INT_t m_iWaveStage;	//start from 1
};

typedef list < SharedPtr <CGpioOutputTimingData> > GpioOutputTimingData_LIST;

class CGpioBtnInputApp : public CEventApp
{
public:
	CGpioBtnInputApp();
	virtual ~CGpioBtnInputApp();
 	static void SigHandler(int iSigNo);
	virtual INT EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam);
	virtual VOID OnFdRead(int iFd);
	virtual VOID OnFdWrite(int iFd);
	void chkGpioBtnInput();
	dbus_bool_t DBusAddWatch(DBusWatch * pDbusWatch);
	void DBusRemoveWatch(DBusWatch * pDbusWatch);
	static void BtnInterruptCb(void * pContext, int param);
	void onBtnInterrupt(int param);
private:
	virtual INT InitInstance(int argc, char * argv[]);	
	virtual INT ExitInstance();
	INT_t ProcessDBusMsg(DBusMessage * pDBusMsg);
	INT_t ProcessDBusMsg_DefaultIfCall(DBusMessage * pDBusMsg);
	INT_t ProcessDBusMsg_DBusIntrospectableIfCall(DBusMessage * pDBusMsg);
	INT_t ProcessDBusMsg_DBusDBusIfCall(DBusMessage * pDBusMsg);
	INT_t ProcessDBusMsg_MethodCall(DBusMessage * pDBusMsg);
	INT_t DBusIntrospectableIf_MethodCall(DBusMessage * pDBusMsg);
	VOID OnTimer(UINT uiTimerId);
	INT_t ReopenGpioBtnInDevFile();
	INT_t Driver_GpioClrEvt();
	INT_t Driver_GpioEnableInterrupt();
	virtual VOID On_BtnDataOutPipe_Write();
	INT_t ConnectDBus();
	INT_t disconnectDBus();
	INT_t On_DbusMsg();
	INT_t OnDBusCall_getGpioInterruptSource(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_getGpioDirection(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_setGpioDirection(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_setGpioOutputData(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_setGpioOutputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_selectInputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_getSelectedInputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_selectOutputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_getSelectedOutputGpioId(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_readGpioInputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_setGpioOutputSingleBit(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_setGpioOutputPulseWidth(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_getGpioOutputPulseWidth(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_getGpioInputStatus(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t OnDBusCall_GenerateGpioOutputTiming(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t DBusIntrospectableIf_Introspect(SharedPtr <CDBusParameterList> & DBusInParamList_sp,
		SharedPtr <CDBusParameterList> & DBusOutParamList_sp);
	INT_t SendGpioInputInterruptDbusSig(int GpioNo, int bPressDown);
	INT_t SendDbusSig_ServiceStarted();
	INT_t setGpioDirection(UINT32_t GpioLogicalDirection);
	INT_t setGpioOutputData(UINT32_t GpioOutputData);
	INT_t setGpioOutputStatus(UINT32_t GpioOutputData, UINT32_t BitMask = 0xFFFFFFFF);
	INT_t getGpioInputStatus(OUT UINT32_t & uiGpioInputStatus);
private:
	UINT8_t m_OldGpioStatus;
	int m_BtnDataOutPipeFd;
	GpioInputEventData_LIST m_lstGpioInputEventData;
	GPIO_BTN_EVT_DATA_PACKET m_GpioBtnEvtDataPacket;
	DBusConnection * m_pDBusConn;
	BOOL_t m_bDBusNeedShutdown;
	CIntArray m_DbusFdArray;
	INT_t m_LastGpioInterruptSource;
	UINT32_t m_GpioLogicalDirection;
	INT32_t m_iSelectedInputGpioId, m_iSelectedOutputGpioId;
	INT32_t m_iGpioOutputPulseWidth;
	GpioOutputTimingData_LIST m_lstGpioOutputTimingData;
	BOOL_t m_bNxClientJoined;	
	NEXUS_GpioHandle m_hGpio_BtnInt;
	NEXUS_I2cHandle m_hNexusI2c;
	UINT_t m_GpioInterruptCnt;
	UINT64_t m_GpioInterruptStartTimeMs;
};

#endif	//_GPIO_BTN_INPUT_APP_H_

