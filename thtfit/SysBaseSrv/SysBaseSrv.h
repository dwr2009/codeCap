#ifndef	_SYS_BASE_SRV_H_
#define	_SYS_BASE_SRV_H_

#include <UnixDatagramSocket.h>
#include <EventApp.h>
#include <SharedPtr.h>
#include "SysBaseSrvApi.h"
#include <Array.h>
#include <RecvBuf2.h>
#include <SendBuf2.h>
#include "SysPropOps.h"

using namespace CppBase;

class CSysBaseSrvApp : public CEventApp
{
public:
	CSysBaseSrvApp();
	~CSysBaseSrvApp();
 	static void SigHandler(int iSigNo);
	virtual INT EventProc(UINT uiEvtId, UINT uiParam, PVOID pParam);
	INT_t SysProp_Set(LPCSTR pszKey, size_t KeyLen, LPCSTR pszVal, size_t ValueLen, BOOL_t bPersistent);
	INT_t SysProp_Get(LPCSTR pszKey, OUT CStackBufString & strValue);
	INT_t SysProp_getById(INT_t iPropId, OUT CStackBufString & strPropName, OUT CStackBufString & strPropVal);
	INT_t SysProp_getCount(OUT INT_t & iPropCount);
	INT_t SysProp_Del(LPCSTR pszKey, size_t KeyLen);
private:
	virtual INT InitInstance(int argc, char * argv[]);	
	virtual INT ExitInstance();
	VOID OnTimer(UINT uiTimerId);
	INT_t OnMethodCall(CRecvBuf2 & SrvSckReadBuf, IN OUT CSendBuf2 & SrvSckSendBuf);
	INT_t ProcessRequestReply(CRecvBuf2 & SrvSckReadBuf, IN OUT CSendBuf2 & SrvSckSendBuf);
	VOID OnSrvSckRead();
	virtual VOID OnFdRead(int iFd);
	VOID UnregTimer_SaveSysPropData();
	VOID RegTimer_SaveSysPropData();
	INT_t SaveSysPropData();
private:
	SharedPtr <CUnixDatagramSocket> m_oSrvSck_sp;
	CRecvBuf2 m_SrvSckReadBuf;
	CSendBuf2 m_SrvSckSendBuf;
	SharedPtr <CSysPropOps> m_oSysPropOps_sp;
	UINT_t m_TimerId_SaveSysPropData;
};

#endif	//_SYS_BASE_SRV_H_

