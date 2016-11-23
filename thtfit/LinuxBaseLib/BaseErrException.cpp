#include <BaseErrException.h>
#include <BaseErrDef.h>

CBaseErrException::CBaseErrException()
{
	m_iBaseErr = ERROR_SUCCESS;
}

CBaseErrException::CBaseErrException(INT_t iBaseErr)
{
	m_iBaseErr = iBaseErr;
}

CBaseErrException::~CBaseErrException()
{

}

INT_t CBaseErrException::getBaseErr()
{
	return m_iBaseErr;
}

