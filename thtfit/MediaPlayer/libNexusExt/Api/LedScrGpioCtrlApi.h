#if	!defined(_LedScrGpioCtrlApi_h_)
#define	_LedScrGpioCtrlApi_h_

#include <BaseTypeDef.h>

class CLedScrGpioCtrlIf
{
public:
	enum GpioNo {
		GpioNo_DATA = 0x01,
		GpioNo_CLK = 0x02,
		GpioNo_CS0 = 0x04,
		GpioNo_CS1 = 0x08,
	};
	typedef enum GpioNo GpioNo_t;
public:
	virtual ~CLedScrGpioCtrlIf() {};
	virtual BOOL_t ChkInit() = 0;
	virtual INT_t setGpioLevel(CLedScrGpioCtrlIf::GpioNo_t gpioNo, BOOL_t bLevel) = 0;
	virtual void delayUs(int delayTimeUs) = 0;
};

CLedScrGpioCtrlIf * LedScrGpioCtrl_new();
void LedScrGpioCtrl_delete(CLedScrGpioCtrlIf * pLedScrGpioCtrl);

#endif	//_LedScrGpioCtrlApi_h_

