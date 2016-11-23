#ifndef	ALLOW_OS_CODE
#define	ALLOW_OS_CODE
#endif	//ALLOW_OS_CODE

#include "../baseIfce/CBaseTunerIface.h"
#include <BaseErrDef.h>

INT_t CBaseTunerIface::InitVal()
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::OpenTunerModule()
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::SetScanParam()
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::SetNextFreqChannel(UINT32_t *pLock,UINT16_t *pScanProgress)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::SetStandard(IN StandardChannle iStandard,IN INT32_t iConstellation)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::GetStandard(OUT StandardChannle *pOStandard)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::GetChannleInfo(INT32_t *pFreq,UINT8 *pBw)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::GetChannelStrength(UINT16_t *pOutStrength)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::SetChannelInfo(INT32_t Freq)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::WaitForLock(UINT32_t *pLock)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::GetCurrConstellation(OUT UINT32_t *pConstellation)
{
	return ERROR_SUCCESS;
}

INT_t CBaseTunerIface::ReInitModules()
{
	return ERROR_SUCCESS;
}
