#include "BrcmIrBlaster.h"
#include "nxclient.h"
#include <stdlib.h>
#include <string.h>
#include <DateTime.h>

BrcmIrBlaster::BrcmIrBlaster()
{
	m_bIsWork=false;
	m_bEnableHeaderPulse=true;
	m_iIrBlasterMode=NEXUS_IrBlasterMode_eNec;
	startIrBlaster();
}

BrcmIrBlaster::~BrcmIrBlaster()
{
	stopIrBlaster();
}

IrBlasterInterface *BrcmIrBlaster::getIrBlasterInstance()
{
	static BrcmIrBlaster sBrcmIrBlaster;
	return &sBrcmIrBlaster;
}

int BrcmIrBlaster::sendIrCodeOut(int iIRCode)
{
	bool bHeaderPulse = true;
	unsigned char iTrailer=0;
	unsigned char iNumBits = 16,iMode=m_iIrBlasterMode;
	NEXUS_IrBlasterStatus irBStatus;
	int iErr=0;
	unsigned int iIRCodeBuffer[4];


	memset(iIRCodeBuffer,0x00,sizeof(iIRCodeBuffer));

	if(!m_bIsWork){
		printf("BrcmIrBlaster isn't init\n");
		return -1;
	}
	bHeaderPulse=true;
	switch(m_iIrBlasterMode){
	case NEXUS_IrBlasterMode_eRC6:
		iNumBits=16;
		break;
	case NEXUS_IrBlasterMode_eSony:
		iNumBits=12;
		break;
	case NEXUS_IrBlasterMode_eGI:
		iNumBits=17;
		break;
	case NEXUS_IrBlasterMode_eNec:
		bHeaderPulse=true;
		iNumBits=33;
		break;
	default:
		iNumBits=16;
		break;
	}
	iIRCodeBuffer[0]=iIRCode;
	printf("%s(%d) iIRCode:%08x bits:%d iMode:%d\n",__func__,__LINE__,iIRCode,iNumBits,iMode);
	if(NEXUS_IrBlasterMode_eRC6==m_iIrBlasterMode){
		 iErr = NEXUS_IrBlaster_SendRC6(m_hIrBlaster, reinterpret_cast<unsigned int*>(iIRCodeBuffer), iNumBits,iMode /* mode */, iTrailer ^= 1);
	}else{
		 iErr = NEXUS_IrBlaster_Send(m_hIrBlaster, reinterpret_cast<unsigned int*>(iIRCodeBuffer), iNumBits, bHeaderPulse);
	}

	if (iErr) {
            printf("Failed to send data %08x !\n",iIRCode);
            return iErr;
        } 

	iErr = NEXUS_IrBlaster_GetStatus(m_hIrBlaster, &irBStatus);
	if (iErr) {
                printf("Get Status failed iErr:%d!\n",iErr);
  		return -1;
	}
    
	if (!irBStatus.busy) {
		printf("Data sent...\n");
               return -1;
	}
	return 0;
}

int BrcmIrBlaster::sendIrLevelOut(char *pIRLevelCode)
{
	return 0;
}

int BrcmIrBlaster::startIrBlaster()
{
	int iErr=0;

	long lOldTimer=0,lCurTimer=0,lDiffTimer=0;
	const  long lWaitMaxTimer=20000;/*Ms*/
	NEXUS_IrBlasterHandle irBHandle;
	NEXUS_IrBlasterSettings irBSettings;
	NEXUS_IrBlasterStatus irBStatus;
	NxClient_JoinSettings joinSettings;

	if(m_bIsWork)
		return 0;	
	
	NxClient_GetDefaultJoinSettings(&joinSettings);

 	lOldTimer=GetSysUpTimeMs();
	lCurTimer=GetSysUpTimeMs();	  
	lDiffTimer=lCurTimer-lOldTimer;
 
	do{
		iErr = NxClient_Join(&joinSettings);
		if(0==iErr){
			//printf("%s(%d) NxClient_Join is Ok\n",__func__,__LINE__);               
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
	
	NEXUS_IrBlaster_GetDefaultSettings(&m_setIrBlaster);
	m_setIrBlaster.mode = m_iIrBlasterMode;
	 irBHandle = NEXUS_IrBlaster_Open(0, &m_setIrBlaster);
	if (!irBHandle) {
		printf("Failed to open irBlaster\n");
		return -1;
	}
	iErr = NEXUS_IrBlaster_GetStatus(irBHandle, &irBStatus);
	if (iErr){
		printf("Get Status failed!\n");
		goto close_irblaster;
		
	}else{
  		printf("Initial Blaster status is %sbusy!\n", irBStatus.busy?"":"not ");
	}
	m_hIrBlaster=irBHandle;	
	m_bIsWork=true;
	printf("BrcmBlaster Init works\n");
	return 0;
close_irblaster:
	NEXUS_IrBlaster_Close(irBHandle);
client_out:
	NxClient_Uninit();
PROC_EXIT:
	return iErr;
}

int BrcmIrBlaster::stopIrBlaster()
{
	if(!m_bIsWork)
		return 0;
	if(m_hIrBlaster!=NULL){
		NEXUS_IrBlaster_Close(m_hIrBlaster);
		m_hIrBlaster=NULL;
	}
	m_bIsWork=false;
	NxClient_Uninit();
}

IrBlasterInterface *getBrcmIrBlasterInstance()
{
	return BrcmIrBlaster::getIrBlasterInstance();
}




