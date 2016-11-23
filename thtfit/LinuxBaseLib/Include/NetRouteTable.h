#ifndef	_NET_ROUTER_TABLE_H_
#define	_NET_ROUTER_TABLE_H_

#include <BaseTypeDef.h>
#include <BaseList.h>

typedef struct
{
	char szNetIfName[32];
	UINT uiFlags;
	UINT32_t uiDestIPv4Addr;
	UINT32_t uiSubnetMaskAddr;
	UINT32_t uiGatewayIPv4Addr;
}IPv4NET_ROUTE_TBL_ITEM, *P_IPv4NET_ROUTE_TBL_ITEM;

#define	ROUTE_MATCH_DEST_ADDR			(1<<0)
#define	ROUTE_MATCH_GATEWAY_ADDR		(1<<1)
#define	ROUTE_MATCH_SUBNETMASK_ADDR		(1<<2)
#define	ROUTE_MATCH_NET_IF_NAME			(1<<3)

typedef struct
{
	UINT uiMatchFlags;
	char szNetIfName[32];
	UINT32_t uiDestIPv4Addr;
	UINT32_t uiGatewayIPv4Addr;
	UINT32_t uiSubnetMaskAddr;
	OUT BOOL_t bMatched;
}MATCH_ROUTE_ENTRY_ITEM, *P_MATCH_ROUTE_ENTRY_ITEM;

typedef struct
{
	UINT uiMatchFlags, uiNotMatchFlags;
	char szNetIfName[32];
	UINT32_t uiDestIPv4Addr;
	UINT32_t uiSubnetMaskAddr;
	UINT32_t uiGatewayIPv4Addr;
	BOOL_t bContinueIfDelFail;
}DEL_ROUTE_ENTRY_ITEM, *P_DEL_ROUTE_ENTRY_ITEM;

typedef CList <P_IPv4NET_ROUTE_TBL_ITEM, P_IPv4NET_ROUTE_TBL_ITEM &> IPv4NET_ROUTE_TBL_LIST;

#define	KERNEL_IPv4NET_ROUTE_TABLE_FILE_PATH	"/proc/net/route"

VOID DelRouteEntryItem_Init(IN OUT P_DEL_ROUTE_ENTRY_ITEM pDelRouteEntryItem);

class CIPv4NetRouteTable
{
public:
	CIPv4NetRouteTable();
	~CIPv4NetRouteTable();
	INT getSysIPv4RouteTable();
	INT MatchRouteEntryItem(P_MATCH_ROUTE_ENTRY_ITEM pMatchRouteEntryItem);
	INT AddRouteEntryItem(P_IPv4NET_ROUTE_TBL_ITEM pIPv4NetRouteTblItem);
	INT DelNetRouteEntryItem(P_DEL_ROUTE_ENTRY_ITEM pDelRouteEntryItem);
	INT_t getDefaultGatewayAddr(UINT32_t * pIPv4Addr, BOOL_t bForceReload = FALSE);
#if 1/*by_su*/
	INT_t getDefaultGatewayAddrByNetIfName(UINT32_t * pIPv4Addr, const char * pszIfName,BOOL_t bForceReload);
#endif
private:
	void ClearIPv4RouteTbl();
	INT_t CheckRouteTableChange();
private:
	IPv4NET_ROUTE_TBL_LIST m_lstIPv4NetRouteTbl;
	BOOL_t m_bSysRouteTableChanged;
};

#endif	//_NET_ROUTER_TABLE_H_

