
#include "Keypad.h"
#include "KeycodeMap.h"
#include <time.h>  
#include <unistd.h> 
#include "config.h"
#include <DateTime.h>


const struct KeyMap Keypad::m_sKeycodeMapTable[]=
{
	{KEY_SW1_SCANCODE,KEY_DOWN},
	{KEY_SW2_SCANCODE,KEY_UP},
	{KEY_SW3_SCANCODE,KEY_LEFT},
	{KEY_SW4_SCANCODE,KEY_RIGHT},
	{KEY_SW5_SCANCODE,KEY_ENTER},
	{KEY_SW6_SCANCODE,KEY_F2},
	{KEY_SW7_SCANCODE,KEY_F6},	
	{0x00,0x00},
};

Keypad::Keypad():	
	m_bInit(false),
	m_pUserInputDevice(NULL),	
	m_pKeyMapTable(NULL)
{
	memset((char *)m_iKeyStatus,0x00,sizeof(m_iKeyStatus));
	pthread_mutex_init(&m_lockInit, NULL);
	pthread_mutex_init(&m_lockKeyUp, NULL);
	pthread_cond_init(&m_condKeyUp, NULL);
	m_iLastKeyCode=0x00;
	m_bLastKeyUp=true;
	m_bThreadExit=false;
	m_llLastKeyTimerMs=0;	
	m_iKeypadMode=KEYPAD_CLIENT_MODE;
	m_iKeyMapTableSize=0;
	m_llRepeatKeyFilterTimer=200;
	memset(m_strKeyMapFileName,0x00,sizeof(m_strKeyMapFileName));
}

Keypad::~Keypad()
{
	exitKeypad();
	pthread_mutex_destroy(&m_lockKeyUp);
	pthread_cond_destroy(&m_condKeyUp);
	pthread_mutex_destroy(&m_lockInit);
	
}

int Keypad::loadKeyMapTable()
{
	int iErr=0;
	int iKeyCounts=0;
	if(NULL!=m_pKeyMapTable){
		delete m_pKeyMapTable;
		m_pKeyMapTable=NULL;
	}
	iKeyCounts=sizeof(m_sKeycodeMapTable)/sizeof(m_sKeycodeMapTable[0]);
	m_pKeyMapTable=new KeyMap_s[iKeyCounts];
	memcpy(m_pKeyMapTable,m_sKeycodeMapTable,sizeof(m_sKeycodeMapTable));
	return iErr;
}

int Keypad::unloadKeyMapTable()
{
	int iErr=0;
	int iKeyCounts=0;
	if(NULL!=m_pKeyMapTable){
		delete m_pKeyMapTable;
		m_pKeyMapTable=NULL;
	}
	return iErr;
}

bool Keypad::isRun()
{
	bool bRun=false;
	pthread_mutex_lock(&m_lockInit);
	do{
		bRun=m_bInit;
	}while(0);
	pthread_mutex_unlock(&m_lockInit);
	return bRun;
}

unsigned long Keypad::getCurrentTimerUs()
{
	unsigned long llCurTimer=0;
	struct timeval tvCurTimer;
	gettimeofday(&tvCurTimer,NULL);
	llCurTimer=tvCurTimer.tv_sec;
	llCurTimer*=1000000;
	llCurTimer+=tvCurTimer.tv_usec;
	return llCurTimer;	
}

int Keypad::onLastKeyUp()
{
	int iErr=0;
	unsigned long llCurTimer=0,llDiffTimer=0;
	pthread_mutex_lock(&m_lockKeyUp);
	if(m_bLastKeyUp)
		pthread_cond_wait(&m_condKeyUp, &m_lockKeyUp);
	
	do{
		if(m_bThreadExit)
			break;
		if(m_bLastKeyUp)
			break;
		if(0x00==m_iLastKeyCode){
			m_bLastKeyUp=true;
			break;
		}
		llCurTimer=GetSysUpTimeMs();
		//printf("%s(%d) llCurTimer %ld us \n",__func__,__LINE__,llDiffTimer);
		llDiffTimer=llCurTimer-m_llLastKeyTimerMs;
		if(llDiffTimer<m_llRepeatKeyFilterTimer){
			//printf("%s(%d) wait %ld us \n",__func__,__LINE__,llDiffTimer);
			pthread_mutex_unlock(&m_lockKeyUp);
			usleep(llDiffTimer);
			pthread_mutex_lock(&m_lockKeyUp);
			continue;
		}		
		//printf("%s(%d) %ld ms \n",__func__,__LINE__,llDiffTimer);
		onKeyUpEvent(m_iLastKeyCode);
		m_iLastKeyCode=0x00;
		m_bLastKeyUp=true;
	}while(!m_bLastKeyUp);	
	pthread_mutex_unlock(&m_lockKeyUp);
	return iErr;
}


int Keypad::onKeyUpEvent(UINT16 iKeyCode)
{
	int iErr=0;
	//printf("%s(%d) keycode %d Up Event\n",__func__,__LINE__,iKeyCode);
	pthread_mutex_lock(&m_lockInit);
	if(!m_bInit)
		goto exit;
	if(m_pUserInputDevice!=NULL){
		m_pUserInputDevice->reportKey(iKeyCode,0x0);
	}
exit:
	pthread_mutex_unlock(&m_lockInit);
	return iErr;
}

int Keypad::onKeyDownEvent(UINT16 iKeyCode)
{
	int iErr=0;
	//printf("%s(%d) keycode %d Down Event\n",__func__,__LINE__,iKeyCode);
	pthread_mutex_lock(&m_lockInit);
	if(!m_bInit)
		goto exit;
	if(m_pUserInputDevice!=NULL){
		m_pUserInputDevice->reportKey(iKeyCode,0x1);
	}
exit:
	pthread_mutex_unlock(&m_lockInit);
	return iErr;
}

int Keypad::onKeyPressingEvent(UINT16 iKeyCode)
{
	int iErr=0;
	//printf("%s(%d) keycode %d pressing event\n",__func__,__LINE__,iKeyCode);
	return iErr;
}
int Keypad::waitEvent()
{
	sleep(4);
	return 0;
}
int Keypad::addLastKeycode(UINT16 iKeyCode,int iKeyValue)
{
	int iErr=0;
	//printf("%s(%d) \n",__func__,__LINE__);
	pthread_mutex_lock(&m_lockKeyUp);
	do{		
		if(m_bLastKeyUp){			
			break;
		}
		if(0x00==m_iLastKeyCode){
			m_bLastKeyUp=true;
			break;
		}
		if(iKeyCode==m_iLastKeyCode){						
			onKeyPressingEvent(iKeyCode);
			m_llLastKeyTimerMs=GetSysUpTimeMs();
			pthread_cond_signal(&m_condKeyUp);
			break;
		}		
		onKeyUpEvent(m_iLastKeyCode);
		m_iLastKeyCode=0x00;
		m_bLastKeyUp=true;
	}while(0);
	if(iKeyCode!=m_iLastKeyCode){
		m_iLastKeyCode=iKeyCode;
		m_bLastKeyUp=false;
		onKeyDownEvent(iKeyCode);
		m_llLastKeyTimerMs=GetSysUpTimeMs();
		pthread_cond_signal(&m_condKeyUp);
	}
	pthread_mutex_unlock(&m_lockKeyUp);
	//printf("%s(%d) \n",__func__,__LINE__);
	return iErr;
}
int Keypad::onKeyUpThreadDetect()
{
	while(!m_bThreadExit){
		//printf("%s(%d) \n",__func__,__LINE__);
		onLastKeyUp();
	}
	return 0;
}

void *Keypad::threadKeyUpDetect(void *pParameter)
{
	//printf("%s(%d) \n",__func__,__LINE__);
	Keypad *pKeypadObject=static_cast<Keypad *>(pParameter);
	if(NULL==pKeypadObject){
		printf("%s(%d)  pParameter is null \n",__func__,__LINE__);
		return NULL;
	}
	pKeypadObject->onKeyUpThreadDetect();
	return NULL;
}
int Keypad::setKeyStatus(UINT16 iKeyCode)
{

	int iByteOffset=0,iBitOffset=0;
	
	iBitOffset=iKeyCode&0x7;		
	iByteOffset=(iKeyCode>>3);
	m_iKeyStatus[iByteOffset]|=(1<<iBitOffset);
	return 0;

}

int Keypad::getKeyStatus(UINT16 iKeyCode)
{

	int iByteOffset=0,iBitOffset=0;
	
	iBitOffset=iKeyCode&0x7;		
	iByteOffset=(iKeyCode>>3);
	if((m_iKeyStatus[iByteOffset]&(1<<iBitOffset))){
		return 1;
	}
	return 0;

}
void Keypad::setKeyMapFileName(char *pFileName)
{
	if(NULL==pFileName)
		return;
	strncpy(m_strKeyMapFileName,pFileName,sizeof(m_strKeyMapFileName));
	return;
}

int Keypad::clearKeyStatus(UINT16 iKeyCode)
{
	int iByteOffset=0,iBitOffset=0;
	
	iBitOffset=iKeyCode&0x7;		
	iByteOffset=(iKeyCode>>3);
	m_iKeyStatus[iByteOffset]&=~(1<<iBitOffset);
	return 0;
}

int Keypad::onKeypadCallback(int iParameter)
{
	
	int iErr=-1;
	UINT16 iKeyCode=0;
	int iKeyValue=0;
	struct KeyMap *pKeyMapTable=m_pKeyMapTable;

	//printf("%s(%d) \n",__func__,__LINE__);
	pthread_mutex_lock(&m_lockInit);
	if(!m_bInit)
		goto fail_out;
	iErr=getKeyEventFromDevice(&iKeyCode,&iKeyValue);
	if(!m_bInit)
		goto fail_out;
	pthread_mutex_unlock(&m_lockInit);
	for(;(pKeyMapTable->iKeyScanCode!=0x00);pKeyMapTable++){
	
		if(iKeyCode!=pKeyMapTable->iKeyScanCode){
			continue;
		}
		addLastKeycode(pKeyMapTable->iKeyCode,iKeyValue);			
		break;

	}
	return 0;
fail_out:
	pthread_mutex_unlock(&m_lockInit);
	return -1;	
	
}

void Keypad::keypadCallback(void *pKeypad,int iParameter)
{
	
	Keypad *pKeypadObject=static_cast<Keypad *>(pKeypad);	       
	int iErr=-1;

	if(NULL==pKeypadObject){
		printf("%s(%d) pKeypad is NULL\n",__func__,__LINE__);
		return;
	}
	iErr=pKeypadObject->onKeypadCallback(iParameter);
	
	return;    
}

int Keypad::initKeypad()
{
	int iRet=-1;
	int iErr=-1;
	

	
	pthread_mutex_lock(&m_lockInit);
	
	if(m_bInit){
		iRet=0;
		printf("%s(%d) Already init\n",__func__,__LINE__);
		goto exit_out;
	}
	if(NULL==m_pUserInputDevice){
		printf("%s(%d) m_pUserInputDevice is null\n",__func__,__LINE__);
		goto exit_out;
	}
	iErr=loadKeyMapTable();
	if(iErr){
		printf("%s(%d) loadKeyMapTable failed\n",__func__,__LINE__);
		goto exit_out;	
	} 
	iErr=m_pUserInputDevice->createInputDevice("Keypad",m_pKeyMapTable);
	if(iErr){
		printf("%s(%d) createInputDevice failed\n",__func__,__LINE__);
		goto exit_unload;
	}

	iErr=connectDevice();
	if(iErr){
		printf("%s(%d) connectDevice failed\n",__func__,__LINE__);
		goto exit_destroy;
	}
	m_bThreadExit=false;
	iErr=pthread_create(&m_threadIdKeyUp,NULL,threadKeyUpDetect,static_cast<void *>(this));
	if(iErr){
		printf("%s(%d) pthread_create failed\n",__func__,__LINE__);
		goto exit_disconnect;
	}
	m_bInit=true;
	pthread_mutex_unlock(&m_lockInit);
	printf("%s(%d) init success\n",__func__,__LINE__);	
	return 0;
exit_disconnect:
	disconnectDevice();
exit_destroy:
	m_pUserInputDevice->destroyInputDevice();
exit_unload:
	unloadKeyMapTable();
exit_out:
	pthread_mutex_unlock(&m_lockInit);	
	return iRet;
}

int Keypad::signalExit(const int iSignalId)
{
	int iErr=-1;
	exit(-1);
	return iErr;
}
int Keypad::exitKeypad()
{
	int iRet=0;
	

	pthread_mutex_lock(&m_lockInit);
	if(!m_bInit){
		goto exit_out;
	}
	m_bThreadExit=true;
	m_bInit=false;
	
	pthread_mutex_lock(&m_lockKeyUp);
		
	pthread_cond_signal(&m_condKeyUp);
		
	pthread_mutex_unlock(&m_lockKeyUp);
	
	pthread_join(m_threadIdKeyUp,NULL); 
	
	disconnectDevice();
	if(m_pUserInputDevice!=NULL)
		m_pUserInputDevice->destroyInputDevice();
	unloadKeyMapTable();
	
exit_out:
	pthread_mutex_unlock(&m_lockInit);
	return 0;
	
}









