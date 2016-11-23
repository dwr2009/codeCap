

#ifndef __CLIENTKEYPAD_H__
#define __CLIENTKEYPAD_H__
#include "Keypad.h"
#include "bkni.h"

class ClientKeypad:public Keypad
{
protected:
	ClientKeypad();
public:
	static Keypad *getKeypadInstance();
	virtual ~ClientKeypad();
	virtual int waitEvent();
protected:
	virtual int connectDevice();
	virtual int disconnectDevice();
	virtual int getKeyEventFromDevice(UINT16 *pKeyCode,int *pKeyValue);
	NEXUS_InputClientHandle m_hInputClient;
	NxClient_AllocResults m_allocResults;	
	BKNI_EventHandle m_hEventWait;
private:
	

};


#endif


