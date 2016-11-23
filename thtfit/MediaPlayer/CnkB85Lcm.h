#ifndef	_CNK_B85_LCM_H_
#define	_CNK_B85_LCM_H_

#include "LedScrGpioCtrlApi.h"
#include <SharedPtr.h>

using namespace CppBase;

#define	LOGICAL_LEVEL_HIGH	1
#define	LOGICAL_LEVEL_LOW	0

#define	LCM_CHIP_COUNT			2
#define	LCM_CHIP_SELECT_0		0
#define	LCM_CHIP_SELECT_1		1

#define	LCM_HALF_CLK_PERIOD_US		2

#define	DATA_SEGMENT_CNT		28

#define	LCM_CHIP_CHAR_COUNT			7

class CCnkB85Lcm
{
public:
	CCnkB85Lcm();
	virtual ~CCnkB85Lcm();
	virtual BOOL_t ChkInit();
	virtual INT_t InitLCD();
	//MSB sent first
	virtual INT_t sendData(INT_t ChipSelect, PBYTE pData, size_t BitsCnt);
	virtual INT_t ClearScreen();
	virtual INT_t FullScreen();
	virtual INT_t DisplayChar(CONST INT_t CharPos, CONST char CharToDisplay, CONST BOOL_t bWithDot = FALSE);
	virtual INT_t DisplayStr(LPCSTR pszStrToDisp, CONST INT_t StartPos = 1);
	virtual VOID DelayUs(UINT_t TimeUs);
protected:
	virtual INT_t setData(BOOL_t bValue);
	virtual INT_t setClk(BOOL_t bValue);
	virtual INT_t setCs0(BOOL_t bValue);
	virtual INT_t setCs1(BOOL_t bValue);
private:
	BOOL_t m_bInited;
	SharedPtr <CLedScrGpioCtrlIf> m_GpioCtrl;
};

#endif	//_CNK_B85_LCM_H_

