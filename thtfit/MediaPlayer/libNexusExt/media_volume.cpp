#include "media_volume.h"
#include <stdio.h>
#include "BaseErrDef.h"

INT_t GetVolumeTableIndexSize()
{
	return (sizeof(VolumeTable) / sizeof(VolumeTable[0]));
}

INT_t GetVolumeVal(INT_t idVolume)
{
	if(0 > idVolume) 
	{
		idVolume = 0;
	}
	else if(GetVolumeTableIndexSize() - 1 < idVolume) 
	{
		idVolume = GetVolumeTableIndexSize() - 1;
	}
		
	return VolumeTable[idVolume];
}

INT_t getPercentFromVolVal(INT_t uiVolVal)
{
	INT_t iOutVolPercent = 0;
	int iId = 0;

	for(iId = 0; iId < GetVolumeTableIndexSize(); iId++)
	{
		if(uiVolVal <= VolumeTable[iId])
		{
			iOutVolPercent = ((iId+1) * 100) / GetVolumeTableIndexSize();
			break;
		}
	}
	if(uiVolVal > VolumeTable[GetVolumeTableIndexSize() - 1])
	{
		iOutVolPercent = 100;
	}
	if(0 == iId)
	{
		iOutVolPercent = 0;
	}

	return iOutVolPercent;
}

INT_t getVolValFromPercent(INT_t iVolPercent)
{
	INT_t uiOutVolVal = 0;
	INT_t iVolId = 0;

	do
	{
		if(0 > iVolPercent)
		{
			iVolPercent = 0;
		}
		if(100 < iVolPercent)
		{
			iVolPercent = 100;
		}
		iVolId = GetVolumeTableIndexSize() * iVolPercent / 100 - 1;
		
		if(0 > iVolId)
		{
			iVolId = 0;
		}
		uiOutVolVal = VolumeTable[iVolId];
	}while(FALSE);

	return uiOutVolVal;
}


INT_t getVolumeLevel(CAudioVolumeExt& ChAudioVolume)
{
	INT_t iOutRet = ERROR_SUCCESS;
	NxClient_AudioSettings audioSettings;
	NxClient_GetAudioSettings(&audioSettings);
	ChAudioVolume.m_left=audioSettings.leftVolume;	
	ChAudioVolume.m_right=audioSettings.rightVolume;
	goto EXIT_PROC;
	
EXIT_PROC:
	
	return iOutRet; 
}

INT_t setVolumeLevel(CAudioVolumeExt& ChAudioVolume)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	NxClient_AudioSettings audioSettings;
	NxClient_GetAudioSettings(&audioSettings);
	audioSettings.volumeType  = NEXUS_AudioVolumeType_eLinear;
	audioSettings.rightVolume = ChAudioVolume.m_right; 
	audioSettings.leftVolume  = ChAudioVolume.m_left;
	iRet = NxClient_SetAudioSettings(&audioSettings);
	if(iRet)
	{
		iOutRet=iRet;
		goto EXIT_PROC;
	}
	
EXIT_PROC:

	return iOutRet; 
}

