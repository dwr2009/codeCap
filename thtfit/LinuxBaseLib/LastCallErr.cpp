#include <BaseErrDef.h>
#include <LastCallErr.h>

CLastCallErr::CLastCallErr()
{
	m_iLastCallErr = ERROR_SUCCESS;

}

CLastCallErr::~CLastCallErr()
{

}

INT CLastCallErr::getLastCallErr()
{
	return m_iLastCallErr;
}

