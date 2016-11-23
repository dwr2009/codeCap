#include <GenericMsgData.h>

// CGenericMsgData

BEGIN_IMPL_RUNTIME_CLASS(CGenericMsgData);
END_IMPL_RUNTIME_CLASS();

CGenericMsgData::CGenericMsgData()
{
	m_uiMsgId = 0;
	m_uiParam = 0;
	
	m_iOutRet = 0;
}

CGenericMsgData::~CGenericMsgData()
{

}

