

#ifndef __INDEPENDENTKEYPAD_H__
#define __INDEPENDENTKEYPAD_H__
#include "Keypad.h"


class IndependentKeypad:public Keypad
{
protected:
	IndependentKeypad();
public:
	static Keypad *getKeypadInstance();
	virtual ~IndependentKeypad();
protected:
	virtual int connectDevice();
	virtual int disconnectDevice();
	virtual int getKeyEventFromDevice(UINT16 *pKeyCode,int *pKeyValue);
	NEXUS_KeypadHandle m_hKeypad;
	NEXUS_KeypadSettings m_sKeypadSettings;
	NEXUS_PlatformSettings m_sPlatformSettings;
	BKNI_EventHandle m_hEvent;
private:

};


#endif


