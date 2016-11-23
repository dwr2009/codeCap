

#ifndef __USERINPUT_H__
#define __USERINPUT_H__
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "KeycodeMap.h"

class UserInputDevice
{
public:
	UserInputDevice();
	~UserInputDevice();
	int createInputDevice(const char *pInputDeviceName,const struct KeyMap *pKeyMapTabale);
	int destroyInputDevice();
	int reportKey(UINT16 iKeyCode,int iKeyValue);
	static const char g_strUserInputDevice[]; 
protected:
	bool m_bInit;
	int m_hUserInputFd;
	struct uinput_user_dev m_hUserInputDev;
	struct input_event m_hInputEvent;
	
};

#endif




