
#include "IndependentKeypad.h"






IndependentKeypad::IndependentKeypad():
	m_hKeypad(NULL)
{
}

IndependentKeypad::~IndependentKeypad()
{
}



Keypad *IndependentKeypad::getKeypadInstance()
{
	static IndependentKeypad sIndependentKeypad;
	return static_cast<Keypad *>(&sIndependentKeypad);
}



int IndependentKeypad::connectDevice()
{
	int iErr=-1;
	NEXUS_Platform_GetDefaultSettings(&m_sPlatformSettings);
	m_sPlatformSettings.openFrontend = false;
	iErr=NEXUS_Platform_Init(&m_sPlatformSettings);
	if(iErr)
		return iErr;
	BKNI_CreateEvent(&m_hEvent);
	NEXUS_Keypad_GetDefaultSettings(&m_sKeypadSettings);
	m_sKeypadSettings.dataReady.callback = keypadCallback;
	m_sKeypadSettings.dataReady.context = static_cast<void *>(this);	
	m_hKeypad = NEXUS_Keypad_Open(0, &m_sKeypadSettings);
	if(NULL==m_hKeypad)
		return -1;
	return 0;
}

int IndependentKeypad::disconnectDevice()
{
	if(m_hKeypad!=NULL)
		NEXUS_Keypad_Close(m_hKeypad);	
	BKNI_SetEvent((BKNI_EventHandle)m_hEvent);
	m_hKeypad=NULL;	
	BKNI_DestroyEvent(m_hEvent);
	NEXUS_Platform_Uninit();
	return 0;	
}

int IndependentKeypad::getKeyEventFromDevice(UINT16 *pKeyCode,int *pKeyValue)
{
	NEXUS_Error nError;
	const int iKeyEvents=1;
        NEXUS_KeypadEvent keypadEvent[iKeyEvents];	
	size_t iKeyEventRead=0,iKeyEventIndex=0;
	bool bOverflow=false,bRepeat=false;
	

	
	if(!m_bInit)
		return -1;
	
	BKNI_SetEvent((BKNI_EventHandle)m_hEvent);
	
        nError=NEXUS_Keypad_GetEvents(m_hKeypad, keypadEvent, iKeyEvents, &iKeyEventRead, &bOverflow);
        BDBG_ASSERT(!nError);
	if(iKeyEventRead<=0){		
		return -1;
	}	
	*pKeyCode=keypadEvent[0].code;
	*pKeyValue=1;	
	return 0;
}





