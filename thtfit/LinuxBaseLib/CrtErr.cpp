#include	"CrtErr.h"

CCrtErr::CCrtErr()
{
	m_iCrtErrNo = 0;
}

CCrtErr::~CCrtErr()
{
	
}

void CCrtErr::SetCrtErrNo(int iCrtErrNo)
{
	m_iCrtErrNo = iCrtErrNo;
}

int CCrtErr::GetCrtErrNo()
{
	return m_iCrtErrNo;
}

