#include "Api/QNetDhcp.h"
#include <BaseErrDef.h>
#include <QSharedPointer>
#include <ProcessList.h>
#include <QStringList>

#define	PROG_NAME_dhcpc			"udhcpc"

INT_t QNetDhcp::ChkDhcpcProcess(IN CONST QString & strNetIfName, OUT BOOL_t & bIsRunning, OUT pid_t & pidDhcpc)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet, iId;
	QSharedPointer <CProcessList> m_ProcessList_sp(new CProcessList);
	POSITION pPos;
	P_PROCESS_ITEM_INFO pProcessItemInfo;

	bIsRunning = FALSE;

	do
	{
		if(m_ProcessList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		iRet = m_ProcessList_sp->ScanSysProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		CONST PROCESS_ITEM_INFO_LIST & ProcessInfoList = m_ProcessList_sp->getProcessList();
		pPos = ProcessInfoList.GetHeadPosition();
		while(pPos)
		{
			pProcessItemInfo = ProcessInfoList.GetNext(OUT pPos);
			do
			{
				if(NULL == pProcessItemInfo)
				{
					break;				
				}
				QString strCmdLine = (LPCSTR)(pProcessItemInfo->strCmdLine);
				QStringList CmdFieldList = strCmdLine.split(' ');
				if(2 > CmdFieldList.size())
				{
					break;
				}
				if(PROG_NAME_dhcpc != CmdFieldList[0])
				{
					break;
				}
				for(iId = 0; iId < CmdFieldList.size(); iId++)
				{
					if(((QString)"--interface=")+strNetIfName == CmdFieldList[iId])
					{
						bIsRunning = TRUE;
						pidDhcpc = pProcessItemInfo->uiProcessId;
						break;
					}
				}
				if(FALSE == bIsRunning)
				{
					break;
				}
				bIsRunning = TRUE;
				break;
			}while(FALSE);
			if(bIsRunning)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

