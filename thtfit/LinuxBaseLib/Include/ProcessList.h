#ifndef	_PROCESS_LIST_H_
#define	_PROCESS_LIST_H_

#include <BaseTypeDef.h>
#include <String.h>
#include <BaseList.h>

typedef struct tagPROCESS_ITEM_INFO
{
	UINT_t uiProcessId;
	CString strCmdLine;
}PROCESS_ITEM_INFO, *P_PROCESS_ITEM_INFO;

typedef CList <P_PROCESS_ITEM_INFO, P_PROCESS_ITEM_INFO &> PROCESS_ITEM_INFO_LIST;

class CProcessList
{
public:
	CProcessList();
	~CProcessList();
	INT ScanSysProcess();
	const PROCESS_ITEM_INFO_LIST & getProcessList();
	static BOOL_t existProcess(pid_t processId);
	VOID ClearProcessList();
private:
protected:
	PROCESS_ITEM_INFO_LIST m_lstProcessItemInfo;
};

#endif	//_PROCESS_LIST_H_

