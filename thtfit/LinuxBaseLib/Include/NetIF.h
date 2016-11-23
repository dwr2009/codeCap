#ifndef _NETIF_H_
#define _NETIF_H_

#include <CCplusplusBridge.h>
#include <BaseTypeDef.h>

#define	ETH_MAC_ADDR_LEN				6

#define	ERROR_NETIF_BASE				1000
#define	ERROR_IF_FAMILY_MISMATCH		(ERROR_NETIF_BASE+1)
#define	ERROR_SELECT_CALL_FAILED		(ERROR_NETIF_BASE+2)
#define	ERROR_INVALID_IP_FORMAT			(ERROR_NETIF_BASE+3)

enum NET_IF_STATUS
{
	STATUS_NETIF_DOWN,
	STATUS_NETIF_UP
};

typedef struct
{
	BOOL_t bAddColonBetweenByte;
}MAC_STR_GENERATE_PARA, *P_MAC_STR_GENERATE_PARA;

typedef struct
{
	INT iHwAddrFamily;
	BYTE acHwAddr[16];
	INT iHwAddrLen;
}NET_IF_HW_ADDR_INFO, *P_NET_IF_HW_ADDR_INFO; 

DECLARE_EXTERN_C_BLOCK_START

VOID NetIfHwAddrInfo_Init(P_NET_IF_HW_ADDR_INFO pNetIfHwAddrInfo);
int GetIfHWAddr(const int sockfd, const char * pIfName, LPBYTE pHWAddr);
INT GetNetIfHWAddr(const char * pszNetIfName, LPBYTE pHWAddr, const INT iBufLen);
int GetNetInterfaceHwAddr(int sockfd, const char * pIfName, P_NET_IF_HW_ADDR_INFO pNetIfHwAddrInfo);
INT GenerateMACStr(const LPBYTE pMACAddr, const INT iBufLen, char * pszMACStrBuf,
	const INT iMACStrBufLen, P_INT piCharsWritten, P_MAC_STR_GENERATE_PARA pMacStrGeneratePara);
INT GetEthMacAddrFromStr(LPCSTR pszMacAddr, PBYTE pBuf, size_t sizeBuf, size_t * psizeWritten);
INT EthMacAddrInvertSeq(PBYTE pMacAddr, UINT uiLen);
int SetIfHwAddr(const char * pIfName, LPBYTE pHwAddr, int iHwAddrLen);
int GetIfAddr(const int sockfd, const char * pIfName, UINT * puiIPAddr);
int GetIfAddrIPV4(const char * pszIfName, UINT32 * puiIPAddr);
int Sleep2(long nuSec);
int GetIfBroadcastAddr(int sockfd, char * pIfName, UINT * puiBroadcastAddr);
int SetIfBroadcastAddr(const int iSockFd, const char * pszIfName, const UINT uiBroadcastAddr);
int GetIfNetmask(const int sockfd, const char * pIfName, UINT * puiNetmask);
INT getNetIfSubnetMask(LPCSTR pszNetIfName, OUT UINT32_t * puiSubnetMask);
INT SetIfNetmask(LPCSTR pszNetIfName, CONST UINT32_t uiSubnetMask);
INT SetIfNetmaskWithSckFd(CONST SOCKET hSck, LPCSTR pszNetIfName, CONST UINT32_t uiSubnetMask);
int SetIfNetmask2(char * pIfName, const char * pszSubnetMask);
int SetIPAddr(const int sockfd, const char * pIfName, UINT uiIPAddr);
int SetIPAddr2(SOCKET sockfd, LPCSTR pIfName, LPCSTR sIPAddr);
INT setNetIfIPv4Addr2(SOCKET hSck, LPCSTR pszNetIfName, CONST UINT32_t uiIPv4Addr);
int SetIPAddr3(char * pIfName, char * sIPAddr);
INT setNetIfIPv4Addr(LPCSTR pszNetIfName, CONST UINT32_t uiIPv4Addr);
int SetIPAddrNetmask(const char * pszIfName, const char * pszIpAddr, const char * pszNetmask);
int SetIPAddrNetmaskWithSckFd(int iSockFd, const char * pszIfName, const char * pszIpAddr, 
	const char * pszNetmask);
int SetIfStatus(const char * pIfName, enum NET_IF_STATUS iNetIfStatus);
int SetIfStatusWithSckFd(int iSockFd, const char * pIfName, enum NET_IF_STATUS iNetIfStatus);
int GetIfStatus(const char * pIfName, short * psiIfFlags);
int GetIfStatusWithSckFd(int iSockFd, const char * pIfName, short * psiIfFlags);
int SetSysNetCoreRecvBufMaxMem(int iSize);
int DelDefaultGateway();
int DelDefaultGatewayIfc(const char *ifname);
int AddDefaultGateway(LPCSTR pszGatewayIpAddr, LPCSTR pszDevName);
int RouteAddDefault(LPCSTR pszDevName);
int AddDefaultGateway2(CONST UINT32_t uiGatewayIpAddr, LPCSTR pszDevName);
INT getIPv4AddrBySck(CONST SOCKET hSck, OUT UINT32_t * puiIPv4Addr);
INT IPv4AddrInSysNetIfList(CONST SOCKET hSck, CONST UINT32_t uiIPv4Addr, OUT BOOL_t * pbAvailable);
INT getIPv4AddrFromStr(LPCSTR pszIPv4Addr, OUT UINT32_t * puiIPv4Addr);
INT_t convertIPv4Addr_BinToStr(UINT32_t uiIPv4Addr, char * pszBuf, size_t BufSize);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_NETIF_H_ 

