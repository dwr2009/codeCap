#if	!defined(_LedScrGpioCtrl_h_)
#define	_LedScrGpioCtrl_h_

#include <BaseTypeDef.h>
#include <LedScrGpioCtrlApi.h>
#include <nexus_base_types.h>
#include <nexus_gpio.h>

class CLedScrGpioCtrl : public CLedScrGpioCtrlIf
{
public:
	enum tagBrcmGpioNo {
		BrcmGpioNo_Data = 45,
		BrcmGpioNo_Clk = 43,
		BrcmGpioNo_CS0 = 46,
		BrcmGpioNo_CS1 = 44,
	};
	typedef enum tagBrcmGpioNo BrcmGpioNo_t;
public:
	CLedScrGpioCtrl();
	virtual ~CLedScrGpioCtrl();
	virtual BOOL_t ChkInit();
	virtual INT_t setGpioLevel(CLedScrGpioCtrlIf::GpioNo_t gpioNo, BOOL_t bLevel);
	virtual void delayUs(int delayTimeUs);
private:
	INT_t CommonConstruct();
private:
	BOOL_t m_bInited;
	NEXUS_GpioHandle m_hGpio_Data;
	NEXUS_GpioHandle m_hGpio_Clk;
	NEXUS_GpioHandle m_hGpio_CS0;
	NEXUS_GpioHandle m_hGpio_CS1;
};

#endif	//_LedScrGpioCtrl_h_

