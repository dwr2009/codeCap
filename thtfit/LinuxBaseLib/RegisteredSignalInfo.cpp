#include "RegisteredSignalInfo.h"

CRegisteredSignalInfo::CRegisteredSignalInfo()
{
	m_SigNo = 0;
	m_pOldSigHandler = NULL;
}

CRegisteredSignalInfo::CRegisteredSignalInfo(int SigNo)
{
	m_SigNo = SigNo;
	m_pOldSigHandler = NULL;
}

