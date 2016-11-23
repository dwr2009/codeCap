#include "Api/LedStatusIndicator.h"
#if defined(__mips__)
#include <emhwlib_hal/include/tango3/emhwlib_registers_tango3.h>
#endif
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include <Mp7xxGlobalStatus.h>
#include <AutoLock.h>

namespace Mp7xxCommon
{

CLedStatusIndicator::CLedStatusIndicator()
{

#if defined(__mips__)
	INT_t iRet=0;

	m_pUart0GpioDir_Addr = NULL;

	do
	{
		if(CPU_UART_GPIODATA-CPU_UART_GPIODIR != 4 || CPU_UART_GPIOMODE-CPU_UART_GPIODIR != 8)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}
		
		iRet = m_GbusMapper.MapPhyMem((PVOID)(REG_BASE_cpu_block+CPU_UART0_base+CPU_UART_GPIODIR), 12, &m_pUart0GpioDir_Addr);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			m_pUart0GpioDir_Addr = NULL;
			break;
		}

		BOOL_t bHwInited = FALSE;
		iRet = TestAndSet_LedIndicatorHwInited(FALSE, TRUE, &bHwInited);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(FALSE == bHwInited)
		{
			UINT32_t RegVal;
			//mode
			RegVal = *((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIOMODE-CPU_UART_GPIODIR)));
			RegVal |= 0x8080;	//SPI_CLK as a GPIO
			*((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIOMODE-CPU_UART_GPIODIR))) = RegVal;
			//direction
			RegVal = *((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODIR-CPU_UART_GPIODIR)));
			RegVal |= 0x8080;	//SPI_CLK is set to OUTPUT
			*((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODIR-CPU_UART_GPIODIR))) = RegVal;
			TurnOffLed();
		}
	}while(FALSE);
#endif
}

CLedStatusIndicator::~CLedStatusIndicator()
{

}

INT_t CLedStatusIndicator::BeginBusy()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iCurVal;

	CAutoLock AutoRangeLocker(&m_mutexCommon);

#if	(PLATFORM==97445 || PLATFORM==97252)
	goto EXIT_PROC;
#endif

	do
	{		
		iRet = set_LedBusyCounter(1, &iCurVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(0 < iCurVal)
		{
			if(NULL == m_pUart0GpioDir_Addr)
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			TurnOnLed();
		}
	}while(FALSE);

EXIT_PROC:
	return iOutRet;
}

INT_t CLedStatusIndicator::EndBusy()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iCurVal;

	CAutoLock AutoRangeLocker(&m_mutexCommon);

#if	(PLATFORM==97445 || PLATFORM==97252)
	goto EXIT_PROC;
#endif

	do
	{
		iRet = set_LedBusyCounter(-1, &iCurVal);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(0 >= iCurVal)
		{
			if(NULL == m_pUart0GpioDir_Addr)
			{
				iOutRet = ERROR_INVALID_STATE;
				break;
			}
			TurnOffLed();
		}
	}while(FALSE);

EXIT_PROC:
	return iOutRet;
}

VOID CLedStatusIndicator::TurnOnLed()
{
#if defined(__mips__)
	UINT32_t RegVal;
	//data
	RegVal = *((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODATA-CPU_UART_GPIODIR)));
	RegVal |= 0x8000;	//SPI_CLK bit
	*((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODATA-CPU_UART_GPIODIR))) = RegVal;
#endif
}

VOID CLedStatusIndicator::TurnOffLed()
{
#if defined(__mips__)
	UINT32_t RegVal;
	//data
	RegVal = *((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODATA-CPU_UART_GPIODIR)));
	RegVal |= 0x8080;	//SPI_CLK bit
	*((UINT32_t *)(((PBYTE)m_pUart0GpioDir_Addr)+(CPU_UART_GPIODATA-CPU_UART_GPIODIR))) = RegVal;
#endif
}

CAutoLedBusyIndicator::CAutoLedBusyIndicator()
{
	INT_t iRet;
	
	iRet = Mp7xxCommon::LedIndicator_BeginBusy();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;	
	}
}

CAutoLedBusyIndicator::~CAutoLedBusyIndicator()
{
	INT_t iRet;
	
	iRet = Mp7xxCommon::LedIndicator_EndBusy();
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_BFILE_LINENO_IRET_STR;	
	}
}

static CLedStatusIndicator & getLedStatusIndicator()
{
	static CLedStatusIndicator g_LedStatusIndicator;
	return g_LedStatusIndicator;
}

INT_t LedIndicator_BeginBusy()
{
	INT_t iOutRet = ERROR_SUCCESS;
	CLedStatusIndicator & LedStatusIndicator = getLedStatusIndicator();

	do
	{
		iOutRet = LedStatusIndicator.BeginBusy();
	}while(FALSE);

	return iOutRet;
}

INT_t LedIndicator_EndBusy()
{
	INT_t iOutRet = ERROR_SUCCESS;
	CLedStatusIndicator & LedStatusIndicator = getLedStatusIndicator();

	do
	{
		iOutRet = LedStatusIndicator.EndBusy();
	}while(FALSE);

	return iOutRet;
}

}	//namespace Mp7xxCommon

