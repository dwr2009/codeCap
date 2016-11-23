#include <LedScrGpioCtrl.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>
#include <DateTime.h>

CLedScrGpioCtrlIf * LedScrGpioCtrl_new()
{
	return (new CLedScrGpioCtrl);
}

void LedScrGpioCtrl_delete(CLedScrGpioCtrlIf * pLedScrGpioCtrl)
{
	if(pLedScrGpioCtrl)
	{
		delete pLedScrGpioCtrl;
		pLedScrGpioCtrl = NULL;
	}
}

CLedScrGpioCtrl::CLedScrGpioCtrl()
{
	INT_t iRet;
	NEXUS_GpioSettings gpioSettings;

	iRet = CommonConstruct();
	if(ERROR_SUCCESS != iRet)
	{
		goto EXIT_PROC;
	}

	NEXUS_Gpio_GetDefaultSettings(NEXUS_GpioType_eStandard, &gpioSettings);
	gpioSettings.mode = NEXUS_GpioMode_eOutputPushPull;
	gpioSettings.value = NEXUS_GpioValue_eHigh;
	gpioSettings.interruptMode = NEXUS_GpioInterrupt_eDisabled;
	gpioSettings.interrupt.callback = NULL;
	gpioSettings.interrupt.context = NULL;
	gpioSettings.interrupt.param = NULL;
	gpioSettings.maskEdgeInterrupts = FALSE;

	//DATA
	m_hGpio_Data = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_Data, &gpioSettings);
	if(NULL == m_hGpio_Data)
	{
		LOG_BLINE("open m_hGpio_Data ERROR!\n");
		goto EXIT_PROC;
	}
	usleep(1000*1000);
	//CLK
	m_hGpio_Clk = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_Clk, &gpioSettings);
	if(NULL == m_hGpio_Clk)
	{
		LOG_BLINE("open m_hGpio_Clk ERROR!\n");
		goto EXIT_PROC;
	}
	usleep(1000*1000);
	//CS0
	m_hGpio_CS0 = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_CS0, &gpioSettings);
	if(NULL == m_hGpio_CS0)
	{
		LOG_BLINE("open m_hGpio_CS0 ERROR!\n");
		goto EXIT_PROC;
	}
	usleep(1000*1000);
	//CS1
	m_hGpio_CS1 = NEXUS_Gpio_Open(NEXUS_GpioType_eStandard, BrcmGpioNo_CS1, &gpioSettings);
	if(NULL == m_hGpio_CS1)
	{
		LOG_BLINE("open m_hGpio_CS1 ERROR!\n");
		goto EXIT_PROC;
	}
	usleep(1000*1000);
	
	m_bInited = TRUE;
	
EXIT_PROC:
	;
}

INT_t CLedScrGpioCtrl::CommonConstruct()
{
	INT_t iOutRet = ERROR_SUCCESS;

	m_bInited = FALSE;
	m_hGpio_Data = NULL;
	m_hGpio_Clk = NULL;
	m_hGpio_CS0 = NULL;
	m_hGpio_CS1 = NULL;	

EXIT_PROC:
	return iOutRet;
}

CLedScrGpioCtrl::~CLedScrGpioCtrl()
{
	if(m_hGpio_Data)
	{
		NEXUS_Gpio_Close(m_hGpio_Data);
		m_hGpio_Data = NULL;
	}
	if(m_hGpio_Clk)
	{
		NEXUS_Gpio_Close(m_hGpio_Clk);
		m_hGpio_Clk = NULL;
	}
	if(m_hGpio_CS0)
	{
		NEXUS_Gpio_Close(m_hGpio_CS0);
		m_hGpio_CS0 = NULL;
	}
	if(m_hGpio_CS1)
	{
		NEXUS_Gpio_Close(m_hGpio_CS1);
		m_hGpio_CS1 = NULL;
	}
	m_bInited = FALSE;
}

BOOL_t CLedScrGpioCtrl::ChkInit()
{
	return m_bInited;
}

INT_t CLedScrGpioCtrl::setGpioLevel(CLedScrGpioCtrlIf::GpioNo_t gpioNo, BOOL_t bLevel)
{
	INT_t iOutRet = ERR_SUCCESS;
	NEXUS_Error nexusErr;
	NEXUS_GpioHandle pGpioHandle = NULL;
	NEXUS_GpioSettings gpioSettings;

	if(GpioNo_DATA == gpioNo)
	{
		pGpioHandle = m_hGpio_Data;
	}
	else if(GpioNo_CLK == gpioNo)
	{
		pGpioHandle = m_hGpio_Clk;
	}
	else if(GpioNo_CS0 == gpioNo)
	{
		pGpioHandle = m_hGpio_CS0;
	}
	else if(GpioNo_CS1 == gpioNo)
	{
		pGpioHandle = m_hGpio_CS1;
	}
	else
	{
		iOutRet = ERR_INV_PARAM;
		goto EXIT_PROC;
	}

	NEXUS_Gpio_GetSettings(pGpioHandle, &gpioSettings);
	if(bLevel)
	{
		gpioSettings.value = NEXUS_GpioValue_eHigh;
	}
	else
	{
		gpioSettings.value = NEXUS_GpioValue_eLow;
	}	
	nexusErr = NEXUS_Gpio_SetSettings(pGpioHandle, &gpioSettings);
	if(NEXUS_SUCCESS != nexusErr)
	{
		iOutRet = ERR_SETUP_FAIL;
		goto EXIT_PROC;
	}

EXIT_PROC:
	return iOutRet;
}

void CLedScrGpioCtrl::delayUs(int delayTimeUs)
{
	INT64_t monotonicStartTimeUs, monotonicCurTimeUs;

	monotonicStartTimeUs = getClkMonotonicTimeUs();
	while(TRUE) 
	{
		monotonicCurTimeUs = getClkMonotonicTimeUs();
		if(monotonicCurTimeUs > monotonicStartTimeUs)
		{
			if(monotonicCurTimeUs - monotonicStartTimeUs >= delayTimeUs)
			{
				break;
			}
		}
		else if(monotonicCurTimeUs < monotonicStartTimeUs)
		{
			if((((UINT64_t)(-1)) - (monotonicStartTimeUs - monotonicCurTimeUs) + 1) >= delayTimeUs)
			{
				break;
			}
		}
	}
}

