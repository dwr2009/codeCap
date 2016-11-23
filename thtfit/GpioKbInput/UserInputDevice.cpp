

#include "UserInputDevice.h"


const char UserInputDevice::g_strUserInputDevice[]="/dev/uinput"; 

UserInputDevice::UserInputDevice()
	:m_bInit(false),
	m_hUserInputFd(-1)
{
}

UserInputDevice::~UserInputDevice()
{
	destroyInputDevice();
}

int UserInputDevice::createInputDevice(const char *pInputDeviceName,const struct KeyMap *pKeyMapTabale)
{
	int iErr=-1;
	if(m_bInit)
		return 0;
	if(NULL==pKeyMapTabale){
		printf("[%s(%d)] pKeyMapTabale is null\n",__func__,__LINE__);
		return iErr;
	}
	memset(&m_hUserInputDev, 0, sizeof(m_hUserInputDev));
	strncpy(m_hUserInputDev.name,"Simulated Input", UINPUT_MAX_NAME_SIZE);
	m_hUserInputDev.id.version = 1; 
	m_hUserInputDev.id.bustype = BUS_VIRTUAL;
	m_hUserInputFd=open(g_strUserInputDevice, O_WRONLY | O_NDELAY);
	if(m_hUserInputFd<0){
		printf("[%s(%d)] open %s failed\n",__func__,__LINE__,g_strUserInputDevice);
		return iErr;
	}
	ioctl(m_hUserInputFd, UI_SET_EVBIT, EV_KEY);
	ioctl(m_hUserInputFd, UI_SET_EVBIT, EV_REP);
	
	for(;pKeyMapTabale->iKeyCode!=0;pKeyMapTabale++){
		iErr = ioctl(m_hUserInputFd, UI_SET_KEYBIT, pKeyMapTabale->iKeyCode);
	}
	write(m_hUserInputFd, &m_hUserInputDev, sizeof(m_hUserInputDev));
	iErr=ioctl(m_hUserInputFd, UI_DEV_CREATE);
	if(iErr){
		printf("Unable to create UINPUT device\n");
		close(m_hUserInputFd);
		m_hUserInputFd=-1;
		return -1;
	}
	m_bInit=true;
	return 0;
}


int UserInputDevice::destroyInputDevice()
{
	int iErr=-1;
	if(!m_bInit)
		return iErr;
	if(m_hUserInputFd<0){
		printf("[%s(%d)] hUserInputFd err\n",__func__,__LINE__);
		return iErr;
	}
	iErr = ioctl(m_hUserInputFd, UI_DEV_DESTROY);
	if(0 != iErr)
	{
		printf("[%s(%d)] UI_DEV_DESTROY err\n",__func__,__LINE__);
		return iErr;
	}
	iErr = close(m_hUserInputFd);
	if(0 != iErr)
	{
		printf("[%s(%d)] close err\n",__func__,__LINE__);
		return iErr;
	}
	m_hUserInputFd = -1;	
	m_bInit=false;
}
int UserInputDevice::reportKey(UINT16 iKeyCode,int iKeyValue)
{
	struct input_event inputEvent;
	int iErr=-1;

	if(!m_bInit)
		return iErr;
	memset(&inputEvent, 0, sizeof(inputEvent));
	gettimeofday(&inputEvent.time, NULL);

	inputEvent.type = EV_KEY;
	inputEvent.code = iKeyCode;
	inputEvent.value = iKeyValue;
	write(m_hUserInputFd, &inputEvent, sizeof(inputEvent));

	inputEvent.type = EV_SYN;
	inputEvent.code = SYN_REPORT;
	inputEvent.value = 0;
	write(m_hUserInputFd, &inputEvent, sizeof(inputEvent));

	return 0;
	
}




