#include "NetRouteTable.h"
#include <File.h>
#include <StackBufString.h>
#include <StrOpMacro.h>
#include <linux/route.h>
#include <string_ex.h>
#include <memory.h>
#include <linux/sockios.h>
#include <sys/ioctl.h> 
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <FileCreator.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

VOID DelRouteEntryItem_Init(IN OUT P_DEL_ROUTE_ENTRY_ITEM pDelRouteEntryItem)
{
	if(NULL == pDelRouteEntryItem)
	{
		goto EXIT_PROC;
	}
	pDelRouteEntryItem->uiMatchFlags = 0;
	pDelRouteEntryItem->uiNotMatchFlags = 0;
	pDelRouteEntryItem->szNetIfName[0] = 
		pDelRouteEntryItem->szNetIfName[sizeof(pDelRouteEntryItem->szNetIfName)-1] = '\0';
	pDelRouteEntryItem->uiDestIPv4Addr = 0;
	pDelRouteEntryItem->uiSubnetMaskAddr = 0;
	pDelRouteEntryItem->uiGatewayIPv4Addr = 0;
	pDelRouteEntryItem->bContinueIfDelFail = FALSE;
EXIT_PROC:
	;	
}

CIPv4NetRouteTable::CIPv4NetRouteTable()
{
	m_bSysRouteTableChanged = TRUE;
}

CIPv4NetRouteTable::~CIPv4NetRouteTable()
{
	ClearIPv4RouteTbl();
}

INT CIPv4NetRouteTable::getSysIPv4RouteTable()
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	char szNetIfName[64];
	UINT32_t uiDestIPv4Addr;
	UINT32_t uiGatewayIPv4Addr;
	UINT32_t uiSubnetMaskAddr;
	UINT uiFlags;
	INT iRefCnt, iUse, iMetric, iMTU, iWindows, iIRTT;	
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem = NULL;
	POSITION pPos = NULL;
	char szNetRouteBuf[4*1024];
	size_t bytesRead;
	CArray < LPCSTR, LPCSTR > lineScanArray;
	int iId;

	do{
		ClearIPv4RouteTbl();
		//read from the file
		bytesRead = 0;
		iRet = FileDataToBuffer(KERNEL_IPv4NET_ROUTE_TABLE_FILE_PATH, 
			(PBYTE)szNetRouteBuf, sizeof(szNetRouteBuf), &bytesRead);
		if(ERR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			goto EXIT_PROC;
		}
		if(bytesRead >= sizeof(szNetRouteBuf))
		{
			szNetRouteBuf[sizeof(szNetRouteBuf)-1] = '\0';
		}
		else if(0 <= bytesRead)
		{
			szNetRouteBuf[bytesRead] = '\0';
		}
		else	//err
		{
			iOutRet = ERR_UNKNOWN;
			goto EXIT_PROC;
		}
		//LOG_BLINE("sizeof(szNetRouteBuf)=%d,bytesRead=%d\n", sizeof(szNetRouteBuf), bytesRead);
		//LOG_BLINE("%s\n", szNetRouteBuf);
		//convert '\n' to '\0'
		{
			LPSTR pszScanPos = szNetRouteBuf;
			LPSTR pszFoundPos;
			while(TRUE)
			{
				pszFoundPos = strchr(pszScanPos, '\n');
				if(NULL == pszFoundPos)
				{
					break;
				}
				*pszFoundPos = '\0';
				pszScanPos = pszFoundPos + 1;
				if(pszScanPos >= (szNetRouteBuf + bytesRead))
				{
					break;
				}
				//ignore the first line
				lineScanArray.Add(pszScanPos);
				//LOG_BLINE("pszScanPos=%s\n", pszScanPos);
			}
		}
		//
		iId = 0;
		while((iId) < lineScanArray.GetSize())
		{
			szNetIfName[0] = '\0';
			uiDestIPv4Addr = 0;
			uiGatewayIPv4Addr = 0;
			uiFlags = 0;
			iRefCnt = iUse = iMetric = 0;
			uiSubnetMaskAddr = 0;
			iMTU = iWindows = iIRTT = 0;
			//LOG_BLINE("%s\n", lineScanArray[iId]);
			iRet = sscanf(lineScanArray[iId], "%32s%lx%lx%lu%ld%ld%ld%lx%ld%ld%ld", 
				szNetIfName, &uiDestIPv4Addr, &uiGatewayIPv4Addr, &uiFlags, &iRefCnt, &iUse, &iMetric, 
				&uiSubnetMaskAddr, &iMTU, &iWindows, &iIRTT);
			if(11 != iRet)
			{
				/*
				PRINT_FILE_LINENO;
				printf("iRet=%d\n", iRet);
				//*/
				goto ONE_LINE_SCAN_FINISH;
			}
			if(FALSE == (RTF_UP & uiFlags))
			{
				goto ONE_LINE_SCAN_FINISH;
			}
			if(pIPv4NetRouteTblItem)
			{
				delete pIPv4NetRouteTblItem;
				pIPv4NetRouteTblItem = NULL;
			}
			pIPv4NetRouteTblItem = new IPv4NET_ROUTE_TBL_ITEM;
			if(NULL == pIPv4NetRouteTblItem)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			SAFE_STRNCPY(pIPv4NetRouteTblItem->szNetIfName, szNetIfName);
			pIPv4NetRouteTblItem->uiDestIPv4Addr = ntohl(uiDestIPv4Addr);
			pIPv4NetRouteTblItem->uiGatewayIPv4Addr = ntohl(uiGatewayIPv4Addr);
			pIPv4NetRouteTblItem->uiSubnetMaskAddr = ntohl(uiSubnetMaskAddr);
			pIPv4NetRouteTblItem->uiFlags = uiFlags;
			pPos = m_lstIPv4NetRouteTbl.AddTail(pIPv4NetRouteTblItem);
			if(NULL == pPos)
			{
				iOutRet = ERROR_OUT_OF_MEMORY;
				break;
			}
			pIPv4NetRouteTblItem = NULL;
ONE_LINE_SCAN_FINISH:
			iId++;
		}
	}while(FALSE);

EXIT_PROC:

	if(pIPv4NetRouteTblItem)
	{
		delete pIPv4NetRouteTblItem;
		pIPv4NetRouteTblItem = NULL;
	}

	return iOutRet;
}

INT CIPv4NetRouteTable::MatchRouteEntryItem(P_MATCH_ROUTE_ENTRY_ITEM pMatchRouteEntryItem)
{
	INT iOutRet = ERROR_SUCCESS;
	POSITION pPos = NULL;
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem = NULL;

	do{
		if(NULL == pMatchRouteEntryItem)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pMatchRouteEntryItem->bMatched = FALSE;
		pPos = m_lstIPv4NetRouteTbl.GetHeadPosition();
		while(pPos)
		{
			pIPv4NetRouteTblItem = m_lstIPv4NetRouteTbl.GetNext(pPos);
			if(pIPv4NetRouteTblItem)
			{
				pMatchRouteEntryItem->bMatched = TRUE;
				do{
					if(pMatchRouteEntryItem->uiMatchFlags & ROUTE_MATCH_NET_IF_NAME)
					{
						if(0 != strcmp_safe(pMatchRouteEntryItem->szNetIfName, pIPv4NetRouteTblItem->szNetIfName))
						{
							pMatchRouteEntryItem->bMatched = FALSE;
							break;
						}
					}
					if(pMatchRouteEntryItem->uiMatchFlags & ROUTE_MATCH_DEST_ADDR)
					{
						if(pMatchRouteEntryItem->uiDestIPv4Addr != pIPv4NetRouteTblItem->uiDestIPv4Addr)
						{
							pMatchRouteEntryItem->bMatched = FALSE;
							break;
						}
					}
					if(pMatchRouteEntryItem->uiMatchFlags & ROUTE_MATCH_GATEWAY_ADDR)
					{
						if(pMatchRouteEntryItem->uiGatewayIPv4Addr != pIPv4NetRouteTblItem->uiGatewayIPv4Addr)
						{
							pMatchRouteEntryItem->bMatched = FALSE;
							break;
						}
					}
					if(pMatchRouteEntryItem->uiMatchFlags & ROUTE_MATCH_SUBNETMASK_ADDR)
					{
						if(pMatchRouteEntryItem->uiSubnetMaskAddr != pIPv4NetRouteTblItem->uiSubnetMaskAddr)
						{
							pMatchRouteEntryItem->bMatched = FALSE;
							break;
						}
					}
				}while(FALSE);
			}
			if(pMatchRouteEntryItem->bMatched)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT CIPv4NetRouteTable::AddRouteEntryItem(P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	SOCKET hSck = INVALID_SOCKET;
	struct rtentry oRouteEntry;
	struct sockaddr_in * poSckAddr = NULL;

	do{
		if(NULL == pIPv4NetRouteTblItem)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		hSck = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == hSck)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}
		ZeroMemory(&oRouteEntry, sizeof(oRouteEntry));
		oRouteEntry.rt_flags = pIPv4NetRouteTblItem->uiFlags;
		oRouteEntry.rt_dev = pIPv4NetRouteTblItem->szNetIfName;
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_dst));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiDestIPv4Addr);
		poSckAddr->sin_port = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_genmask));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiSubnetMaskAddr);
		poSckAddr->sin_port = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_gateway));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiGatewayIPv4Addr);
		poSckAddr->sin_port = 0;

		oRouteEntry.rt_metric = 0;
				
		iRet = ioctl(hSck, SIOCADDRT, &oRouteEntry);
		if(-1 == iRet)
		{
			iOutRet = ERROR_SOCKET_IOCTL_FAIL;
			break;
		}
		
	}while(FALSE);

	if(INVALID_SOCKET != hSck)
	{
		iRet = close(hSck);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		hSck = INVALID_SOCKET;
	}

	return iOutRet;
}

INT CIPv4NetRouteTable::DelNetRouteEntryItem(P_DEL_ROUTE_ENTRY_ITEM pDelRouteEntryItem)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	SOCKET hSck = INVALID_SOCKET;
	struct rtentry oRouteEntry;
	struct sockaddr_in * pSckAddr = NULL;
	POSITION pPos = NULL, pCurPos;
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem = NULL;
	BOOL_t bNeedDel = FALSE;

	do{
		if(NULL == pDelRouteEntryItem)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		hSck = socket(AF_INET, SOCK_DGRAM, 0);
		if(INVALID_SOCKET == hSck)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}

		pPos = m_lstIPv4NetRouteTbl.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pIPv4NetRouteTblItem = m_lstIPv4NetRouteTbl.GetNext(pPos);
			if(pIPv4NetRouteTblItem)
			{
				bNeedDel = TRUE;

				do{
					//match
					if(pDelRouteEntryItem->uiMatchFlags & ROUTE_MATCH_NET_IF_NAME)
					{
						if(0 != strcmp_safe(pIPv4NetRouteTblItem->szNetIfName, pDelRouteEntryItem->szNetIfName))
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiMatchFlags & ROUTE_MATCH_DEST_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiDestIPv4Addr != pDelRouteEntryItem->uiDestIPv4Addr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiMatchFlags & ROUTE_MATCH_SUBNETMASK_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiSubnetMaskAddr != pDelRouteEntryItem->uiSubnetMaskAddr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiMatchFlags & ROUTE_MATCH_GATEWAY_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiGatewayIPv4Addr != pDelRouteEntryItem->uiGatewayIPv4Addr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
					//not match
					if(pDelRouteEntryItem->uiNotMatchFlags & ROUTE_MATCH_NET_IF_NAME)
					{
						if(0 == strcmp_safe(pIPv4NetRouteTblItem->szNetIfName, pDelRouteEntryItem->szNetIfName))
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiNotMatchFlags & ROUTE_MATCH_DEST_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiDestIPv4Addr == pDelRouteEntryItem->uiDestIPv4Addr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiNotMatchFlags & ROUTE_MATCH_SUBNETMASK_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiSubnetMaskAddr == pDelRouteEntryItem->uiSubnetMaskAddr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
					if(pDelRouteEntryItem->uiNotMatchFlags & ROUTE_MATCH_GATEWAY_ADDR)
					{
						if(pIPv4NetRouteTblItem->uiGatewayIPv4Addr == pDelRouteEntryItem->uiGatewayIPv4Addr)
						{
							bNeedDel = FALSE;
							break;
						}
					}
				}while(FALSE);

				if(bNeedDel)
				{
					struct in_addr oInAddr;
					DECLARE_CLS_STACK_BUF_STRING(strDestIP, 32);
					DECLARE_CLS_STACK_BUF_STRING(strSubnetMask, 32);
					DECLARE_CLS_STACK_BUF_STRING(strGateway, 32);
					oInAddr.s_addr = htonl(pIPv4NetRouteTblItem->uiDestIPv4Addr);
					strDestIP = inet_ntoa(oInAddr);
					oInAddr.s_addr = htonl(pIPv4NetRouteTblItem->uiSubnetMaskAddr);
					strSubnetMask = inet_ntoa(oInAddr);
					oInAddr.s_addr = htonl(pIPv4NetRouteTblItem->uiGatewayIPv4Addr);
					strGateway = inet_ntoa(oInAddr);
					PRINT_FILE_LINENO;
					printf("del route(%s,dest:%s,mask:%s,gateway:%s)\n", pIPv4NetRouteTblItem->szNetIfName, 
						(LPCSTR)strDestIP, (LPCSTR)strSubnetMask, (LPCSTR)strGateway);
					ZeroMemory(&oRouteEntry, sizeof(oRouteEntry));
					oRouteEntry.rt_dev = pIPv4NetRouteTblItem->szNetIfName;
					oRouteEntry.rt_flags = RTF_UP;
					oRouteEntry.rt_dst.sa_family = AF_INET;

					pSckAddr = (typeof(pSckAddr))(&(oRouteEntry.rt_dst));
					ZeroMemory(pSckAddr, sizeof(*pSckAddr));
					pSckAddr->sin_family = AF_INET;
					pSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiDestIPv4Addr);

					pSckAddr = (typeof(pSckAddr))(&(oRouteEntry.rt_genmask));
					ZeroMemory(pSckAddr, sizeof(*pSckAddr));
					pSckAddr->sin_family = AF_INET;
					pSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiSubnetMaskAddr);

					pSckAddr = (typeof(pSckAddr))(&(oRouteEntry.rt_gateway));
					ZeroMemory(pSckAddr, sizeof(*pSckAddr));
					pSckAddr->sin_family = AF_INET;
					pSckAddr->sin_addr.s_addr = htonl(pIPv4NetRouteTblItem->uiGatewayIPv4Addr);

					iRet = ioctl(hSck, SIOCDELRT, &oRouteEntry);
					if(-1 == iRet)
					{	
						if(pDelRouteEntryItem->bContinueIfDelFail)
						{
							PRINT_FILE_LINENO_CRT_ERRINFO;
						}
						else
						{
							iOutRet = ERROR_SOCKET_IOCTL_FAIL;
							break;
						}
					}
					m_lstIPv4NetRouteTbl.RemoveAt(pCurPos);
				}
			}
		}		
	}while(FALSE);

	if(INVALID_SOCKET != hSck)
	{
		iRet = close(hSck);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		hSck = INVALID_SOCKET;
	}

	return iOutRet;
}

void CIPv4NetRouteTable::ClearIPv4RouteTbl()
{
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem;
	POSITION pPos, pCurPos;

	pPos = m_lstIPv4NetRouteTbl.GetHeadPosition();
	while(pPos)
	{
		pCurPos = pPos;
		pIPv4NetRouteTblItem = m_lstIPv4NetRouteTbl.GetNext(OUT pPos);
		m_lstIPv4NetRouteTbl.RemoveAt(pCurPos);
		if(pIPv4NetRouteTblItem)
		{
			delete pIPv4NetRouteTblItem;
			pIPv4NetRouteTblItem = NULL;
		}
	}
	m_lstIPv4NetRouteTbl.RemoveAll();
}

INT_t CIPv4NetRouteTable::getDefaultGatewayAddr(UINT32_t * pIPv4Addr, BOOL_t bForceReload/* = FALSE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem;
	POSITION pPos, pCurPos;
	BOOL_t bFound;

	do
	{
		if(bForceReload)
		{
			m_bSysRouteTableChanged = TRUE;
		}
		iRet = CheckRouteTableChange();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		bFound = FALSE;
		pPos = m_lstIPv4NetRouteTbl.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pIPv4NetRouteTblItem = m_lstIPv4NetRouteTbl.GetNext(OUT pPos);
			if(pIPv4NetRouteTblItem)
			{
				//LOG_BLINE("Dest=0x%08x,Flags:0x%08x\n", pIPv4NetRouteTblItem->uiDestIPv4Addr, pIPv4NetRouteTblItem->uiFlags);
				if((0 == pIPv4NetRouteTblItem->uiDestIPv4Addr) && (RTF_GATEWAY & pIPv4NetRouteTblItem->uiFlags))
				{
					if(pIPv4Addr)
					{
						*pIPv4Addr = pIPv4NetRouteTblItem->uiGatewayIPv4Addr;
					}
					bFound = TRUE;
					break;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_BUG_STR;
				m_lstIPv4NetRouteTbl.RemoveAt(pCurPos);
			}
		}

		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	return iOutRet;
}
#if 1/*by_su*/
INT_t CIPv4NetRouteTable::getDefaultGatewayAddrByNetIfName(UINT32_t * pIPv4Addr, const char * pszIfName,BOOL_t bForceReload)

{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem;
	POSITION pPos, pCurPos;
	BOOL_t bFound;

	do
	{
		if(NULL == pszIfName)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		if(bForceReload)
		{
			m_bSysRouteTableChanged = TRUE;
		}
		iRet = CheckRouteTableChange();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		
		bFound = FALSE;
		pPos = m_lstIPv4NetRouteTbl.GetHeadPosition();
		while(pPos)
		{
			pCurPos = pPos;
			pIPv4NetRouteTblItem = m_lstIPv4NetRouteTbl.GetNext(OUT pPos);
			if(pIPv4NetRouteTblItem)
			{
				//printf("szNetIfName = %s, pszIfName = %s\n", pIPv4NetRouteTblItem->szNetIfName, (LPCSTR)pszIfName);
				//LOG_BLINE("szNetIfName = %s pszIfName = %s\n", pIPv4NetRouteTblItem->szNetIfName,  (LPCSTR)pszIfName);
				if((0 == strcmp_safe(pIPv4NetRouteTblItem->szNetIfName, pszIfName)) && (RTF_GATEWAY & pIPv4NetRouteTblItem->uiFlags))
				{
					if(pIPv4Addr)
					{
						*pIPv4Addr = pIPv4NetRouteTblItem->uiGatewayIPv4Addr;
					}
					bFound = TRUE;
					break;
				}
			}
			else
			{
				PRINT_BFILE_LINENO_BUG_STR;
				m_lstIPv4NetRouteTbl.RemoveAt(pCurPos);
			}
		}

		if(FALSE == bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
	}while(FALSE);

	return iOutRet;
}
#endif

INT_t CIPv4NetRouteTable::CheckRouteTableChange()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		if(FALSE == m_bSysRouteTableChanged)
		{
			break;
		}
		m_bSysRouteTableChanged = FALSE;

		iRet = getSysIPv4RouteTable();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

