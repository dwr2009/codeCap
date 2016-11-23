#ifndef	_GENERIC_MSG_DATA_H_
#define	_GENERIC_MSG_DATA_H_

#include <BaseTypeDef.h>
#include <Object.h>
#include <WeakPtr.h>
#include <Object.h>

using namespace CppBase;

class CGenericMsgData
{
	DECLARE_RUNTIME_CLASS(CGenericMsgData);
public:
	//IN
	UINT m_uiMsgId;
	UINT m_uiParam;
	WeakPtr <CObject> m_objParam_wp;
	//OUT
	INT_t m_iOutRet;
public:
	CGenericMsgData();
	virtual ~CGenericMsgData();	
};

#endif	//_GENERIC_MSG_DATA_H_

