#ifndef	_LED_STATUS_INDICATOR_H_
#define	_LED_STATUS_INDICATOR_H_

#include <BaseTypeDef.h>
#include <Mutex2.h>
#include <MemoryDevice.h>

#ifdef	__cplusplus

namespace Mp7xxCommon
{

class CLedStatusIndicator
{
public:
	CLedStatusIndicator();
	~CLedStatusIndicator();
	INT_t BeginBusy();
	INT_t EndBusy();
private:
	VOID TurnOnLed();
	VOID TurnOffLed();
private:
	CMutex2 m_mutexCommon;
	CMemDevMapper m_GbusMapper;
	PVOID m_pUart0GpioDir_Addr;
};

class CAutoLedBusyIndicator
{
public:
	CAutoLedBusyIndicator();
	~CAutoLedBusyIndicator();
};

#define	DECLARE_WORK_BUSY_SECTION		Mp7xxCommon::CAutoLedBusyIndicator oAutoLedBusySection;

INT_t LedIndicator_BeginBusy();
INT_t LedIndicator_EndBusy();

}

#endif	//__cplusplus

#endif	//_LED_STATUS_INDICATOR_H_

