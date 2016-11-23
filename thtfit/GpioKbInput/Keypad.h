


#include <stdio.h>
#include "nexus_types.h"
#include "nexus_platform.h"
#include "nexus_led.h"
#include "nexus_keypad.h"
#include "bstd.h"
#include "bkni.h"
#include <string.h>
#include <pthread.h>
#include "KeycodeMap.h"
#include "UserInputDevice.h"
#include "nxclient.h"
#include "nexus_input_client.h"
#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#define	KEY_STATUS_LENGTH		0x300
#define FILE_NAME_LENGTH_MAX		128

class Keypad
{
protected:
	Keypad();
public:	
	virtual ~Keypad();
	//static Keypad *getKeypadInstance();
	static const struct KeyMap m_sKeycodeMapTable[];
	static void *threadKeyUpDetect(void *pParameter);
	static void keypadCallback(void *pKeypad,int iParameter);
	virtual int initKeypad();
	virtual int exitKeypad();
	virtual int waitEvent();		
	bool isRun();//{return !m_bInit;};
	void setKeyMapFileName(char *pFileName);
	int setUserInputDevice(UserInputDevice *pUserInputDevice){m_pUserInputDevice=pUserInputDevice;return 0;};
	virtual int signalExit(const int iSignalId);
protected:
	int onKeyUpThreadDetect();
	int onKeypadCallback(int iParameter);
	int setKeyStatus(UINT16 iKeyCode);
	int getKeyStatus(UINT16 iKeyCode);
	int clearKeyStatus(UINT16 iKeyCode);
	unsigned long getCurrentTimerUs();

	virtual int connectDevice()=0;
	virtual int disconnectDevice()=0;
	virtual int getKeyEventFromDevice(UINT16 *pKeyCode,int *pKeyValue)=0;
	virtual int loadKeyMapTable();
	virtual int unloadKeyMapTable();
	
	int onKeyDownEvent(UINT16 iKeyCode);	
	int onKeyUpEvent(UINT16 iKeyCode);
	int onKeyPressingEvent(UINT16 iKeyCode);
	int onLastKeyUp();
	int addLastKeycode(UINT16 iKeyCode,int KeyValue);
	
	unsigned char m_iKeyStatus[KEY_STATUS_LENGTH>>3];

	pthread_mutex_t m_lockInit;
	int m_bInit;

	pthread_mutex_t m_lockKeyUp;
	pthread_cond_t	m_condKeyUp;
	unsigned short m_iLastKeyCode;
	unsigned long m_llLastKeyTimerMs;
	bool m_bLastKeyUp;

	pthread_t m_threadIdKeyUp;
	bool m_bThreadExit;

	unsigned long m_llRepeatKeyFilterTimer;
	UserInputDevice *m_pUserInputDevice;
	
	int m_iKeypadMode;
	struct KeyMap *m_pKeyMapTable;
	int m_iKeyMapTableSize;
	char m_strKeyMapFileName[FILE_NAME_LENGTH_MAX];

private:

};

#endif



