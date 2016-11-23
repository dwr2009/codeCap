
#include "ClientKeypad.h"

#include <DateTime.h>




ClientKeypad::ClientKeypad():
	m_hInputClient(NULL),
	m_hEventWait(NULL)
{
}

ClientKeypad::~ClientKeypad()
{
}



Keypad *ClientKeypad::getKeypadInstance()
{
	static ClientKeypad sClientKeypad;
	return static_cast<Keypad *>(&sClientKeypad);
}


int ClientKeypad::waitEvent()
{
	int iErr=-1;
	sleep(4);
	return iErr;
}

int ClientKeypad::connectDevice()
{
	int iErr=-1;
	
	long long lOldTimer=0,lCurTimer=0,lDiffTimer=0;
	const  long long lWaitMaxTimer=20000;
	uint32_t filter = 0;
	NEXUS_InputClientSettings 	settings;
	NxClient_JoinSettings 		joinSettings;
	NxClient_AllocSettings 		allocSettings;	
	
	if(NULL !=m_hInputClient){
		NxClient_Uninit();
		m_hInputClient=NULL;
	}

	NxClient_GetDefaultJoinSettings(&joinSettings);

	
	lOldTimer=GetSysUpTimeMs();
	lCurTimer=GetSysUpTimeMs();

	lDiffTimer=lCurTimer-lOldTimer;
	
	do{
		iErr = NxClient_Join(&joinSettings);
		if(0==iErr){
			printf("%s(%d) NxClient_Join is Ok\n",__func__,__LINE__); 		
			break;
		}		
		sleep(1);
		lCurTimer=GetSysUpTimeMs();
		lDiffTimer=lCurTimer-lOldTimer;
	}while(lDiffTimer<lWaitMaxTimer);
	if(iErr){		
		printf("%s(%d) NxClient_Join is Failed\n",__func__,__LINE__);		
		goto PROC_EXIT;
	}
	
	NxClient_GetDefaultAllocSettings(&allocSettings);
	allocSettings.inputClient = 1;

	
	iErr = NxClient_Alloc(&allocSettings, &m_allocResults);
	if (iErr)
	{    	    		
		goto PROC_EXIT;
	}

	if(!m_allocResults.inputClient[0].id){
		goto FREE_PROC_EXIT;	
	}	
	m_hInputClient = NEXUS_InputClient_Acquire(m_allocResults.inputClient[0].id);
	if(NULL == m_hInputClient)
	{			
		goto FREE_PROC_EXIT;
	}
	iErr = BKNI_CreateEvent(&m_hEventWait);
	if(iErr!=0){
		printf("%s(%d) BKNI_CreateEvent is Failed\n",__func__,__LINE__);
		goto FREE_PROC_EXIT;
	}
	NEXUS_InputClient_GetSettings(m_hInputClient, &settings);
	filter |= 1<<NEXUS_InputRouterDevice_eKeypad;
	settings.filterMask = filter;
	settings.codeAvailable.callback = keypadCallback;
	settings.codeAvailable.context  = static_cast<void *>(this);
	
	iErr = NEXUS_InputClient_SetSettings(m_hInputClient, &settings);
	if(iErr)
	{
		printf("NEXUS_InputClient_SetSettings:%d\n",iErr);
		goto DESTORY_EVENT_EXIT;
		
	}
	return 0;
DESTORY_EVENT_EXIT:
	BKNI_DestroyEvent(m_hEventWait);
	m_hEventWait=NULL;
FREE_PROC_EXIT:
	NxClient_Free(&m_allocResults);

PROC_EXIT:
	return iErr;
}

int ClientKeypad::disconnectDevice()
{
	int iErr=-1;
	
	if( NULL != m_hInputClient){
		NEXUS_InputClient_Release(m_hInputClient);		
		m_hInputClient=NULL;
	}
	
	BKNI_DestroyEvent(m_hEventWait);
	m_hEventWait=NULL;

	NxClient_Free(&m_allocResults);
	
	NxClient_Uninit();
	
	return 0;
}

int ClientKeypad::getKeyEventFromDevice(UINT16 *pKeyCode,int *pKeyValue)
{
	int iRet;
	unsigned  int iReadCodeCount=0;
	NEXUS_InputRouterCode keycode;
	
	

	if(NULL == m_hInputClient){
		return -1;
	}
	
	iRet = NEXUS_InputClient_GetCodes(m_hInputClient, &keycode, 1, &iReadCodeCount);
	if(iReadCodeCount<=0)
		return -1;	

	switch (keycode.deviceType)
	{
	case NEXUS_InputRouterDevice_eKeypad:
		*pKeyCode=keycode.data.keypad.code;
		*pKeyValue=1;
		//printf("%s(%d) keycode:%04x index:%d\n",__func__,__LINE__,keycode.data.keypad.code,keycode.data.keypad.index);
		return 0;
		break;
	case NEXUS_InputRouterDevice_eIrInput:		
	case NEXUS_InputRouterDevice_eEvdev:
	case NEXUS_InputRouterDevice_eUhfInput:
	case NEXUS_InputRouterDevice_eBluetoothRemote:
	default:
		printf("unknown deviceType %d\n", keycode.deviceType);
		break;
	}
	return -1;
}





