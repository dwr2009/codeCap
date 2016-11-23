#include "NetIF.h"
#include <linux/if.h>
#include <sys/ioctl.h>
#include <linux/if_arp.h>
#include <BaseErrDef.h>
#include	<BaseTypeDef.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include <unistd.h>
#include <linux/route.h>
#include <ErrPrintHelper.h>
#include	<math2.h>
#include <memory.h>
#include <stdlib.h>

VOID NetIfHwAddrInfo_Init(P_NET_IF_HW_ADDR_INFO pNetIfHwAddrInfo)
{
	if(pNetIfHwAddrInfo)
	{
		ZeroMemory(pNetIfHwAddrInfo, sizeof(*pNetIfHwAddrInfo));
	}
}

//Get hardware address of a specified interface
int GetIfHWAddr(const int sockfd, const char * pIfName, LPBYTE pHWAddr)
{
	int nRet; struct ifreq IfInfo;

	if(NULL == pIfName || NULL == pHWAddr)
	{
		return ERROR_INVALID_PARAMETER;
	}

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	nRet = ioctl(sockfd, SIOCGIFHWADDR, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;

	if(ARPHRD_ETHER != IfInfo.ifr_hwaddr.sa_family)
		return ERROR_IF_FAMILY_MISMATCH;

	memcpy(pHWAddr, IfInfo.ifr_hwaddr.sa_data, sizeof(IfInfo.ifr_hwaddr.sa_data));

	return ERROR_SUCCESS;
}

INT GetNetIfHWAddr(const char * pszNetIfName, LPBYTE pHWAddr, const INT iBufLen)
{
	INT iOutRet = ERROR_SUCCESS, iRet = 0;
	INT iSckFd = INVALID_SOCKET;
	
	do{
		if(NULL == pszNetIfName || NULL == pHWAddr || 6 > iBufLen)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iSckFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iRet = GetIfHWAddr(iSckFd, pszNetIfName, pHWAddr);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}
	
	return iOutRet;
}

INT GenerateMACStr(const LPBYTE pMACAddr, const INT iBufLen, char * pszMACStrBuf,
	const INT iMACStrBufLen, P_INT piCharsWritten, P_MAC_STR_GENERATE_PARA pMacStrGeneratePara)
{
	INT iOutRet = 0, iRet = 0, iId = 0, iPrintStartOff = 0;
	BYTE byteData;

	do{
		if(NULL == pMACAddr || 0 >= iBufLen || NULL == pszMACStrBuf || 0 >= iMACStrBufLen)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iPrintStartOff = 0;
		for(iId=0; iId<iBufLen; iId++)
		{
			byteData = pMACAddr[iId];
			if(0 < iId && pMacStrGeneratePara->bAddColonBetweenByte)
			{
				pszMACStrBuf[iPrintStartOff] = ':';
				iPrintStartOff++;
				if(iPrintStartOff > iMACStrBufLen - 2)
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					break;
				}
			}

			iRet = snprintf(pszMACStrBuf+iPrintStartOff, iMACStrBufLen-iPrintStartOff, "%02x", byteData);
			iPrintStartOff += iRet;
			if(iPrintStartOff > iMACStrBufLen - 2)
			{
				iOutRet = ERROR_NOT_ENOUGH_BUFFER;
				break;
			}
		}
		pszMACStrBuf[iMACStrBufLen-1] = '\0';
		iPrintStartOff++;

		if(piCharsWritten)
		{
			*piCharsWritten = iPrintStartOff;
		}
	}while(FALSE);

	return iOutRet;
}

INT GetEthMacAddrFromStr(LPCSTR pszMacAddr, PBYTE pBuf, size_t sizeBuf, size_t * psizeWritten)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	UINT_t uiMacVal0, uiMacVal1, uiMacVal2, uiMacVal3, uiMacVal4, uiMacVal5; 
	size_t sizeWritten = 0;

	do{
		if(NULL == pszMacAddr || NULL == pBuf)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		if(ETH_MAC_ADDR_LEN > sizeBuf)
		{
			iOutRet = ERROR_NOT_ENOUGH_BUFFER;
			break;
		}
		iRet = sscanf(pszMacAddr, "%x:%x:%x:%x:%x:%x", &uiMacVal0, &uiMacVal1, &uiMacVal2, &uiMacVal3, 
			&uiMacVal4, &uiMacVal5);
		if(ETH_MAC_ADDR_LEN != iRet)
		{
			iOutRet = ERR_CONVERT_FAIL;
			break;
		}
		sizeWritten = ETH_MAC_ADDR_LEN;
		pBuf[0] = uiMacVal0;
		pBuf[1] = uiMacVal1;
		pBuf[2] = uiMacVal2;
		pBuf[3] = uiMacVal3;
		pBuf[4] = uiMacVal4;
		pBuf[5] = uiMacVal5;
	}while(FALSE);

	if(psizeWritten)
	{
		*psizeWritten = sizeWritten;
	}

	return iOutRet;
}

INT EthMacAddrInvertSeq(PBYTE pMacAddr, UINT uiLen)
{
	INT iOutRet = ERROR_SUCCESS;
	BYTE abMacAddrTmp[ETH_MAC_ADDR_LEN];

	do{
		if(NULL == pMacAddr || ETH_MAC_ADDR_LEN != uiLen)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		memcpy(abMacAddrTmp, pMacAddr, ETH_MAC_ADDR_LEN);

		pMacAddr[0] = abMacAddrTmp[5];
		pMacAddr[1] = abMacAddrTmp[4];
		pMacAddr[2] = abMacAddrTmp[3];
		pMacAddr[3] = abMacAddrTmp[2];
		pMacAddr[4] = abMacAddrTmp[1];
		pMacAddr[5] = abMacAddrTmp[0];
	}while(FALSE);

	return iOutRet;
}

int GetNetInterfaceHwAddr(int sockfd, CONST char * pIfName, P_NET_IF_HW_ADDR_INFO pNetIfHwAddrInfo)
{
	int nRet = 0; 
	struct ifreq IfInfo;

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	nRet = ioctl(sockfd, SIOCGIFHWADDR, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;

	pNetIfHwAddrInfo->iHwAddrFamily = IfInfo.ifr_hwaddr.sa_family;
	memcpy(pNetIfHwAddrInfo->acHwAddr, IfInfo.ifr_hwaddr.sa_data, 
		GetMinVal(sizeof(IfInfo.ifr_hwaddr.sa_data), sizeof(pNetIfHwAddrInfo->acHwAddr)));
	if(ARPHRD_ETHER == pNetIfHwAddrInfo->iHwAddrFamily)
	{
		pNetIfHwAddrInfo->iHwAddrLen = ETH_ALEN;
	}
	else 
	{
		pNetIfHwAddrInfo->iHwAddrLen = 
			GetMinVal(sizeof(IfInfo.ifr_hwaddr.sa_data), sizeof(pNetIfHwAddrInfo->acHwAddr));
	}

	return ERROR_SUCCESS;
}

int SetIfHwAddr(LPCSTR pIfName, LPBYTE pHwAddr, int iHwAddrLen)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;
	struct ifreq IfInfo;

	do{
		if(6 != iHwAddrLen)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
	
		iSckFd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
			break;
		}

		memset(&IfInfo, 0x00, sizeof(IfInfo));
		strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
		IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';
		IfInfo.ifr_hwaddr.sa_family = ARPHRD_ETHER;
		memcpy(IfInfo.ifr_hwaddr.sa_data, pHwAddr, iHwAddrLen); 

		iRet = ioctl(iSckFd, SIOCSIFHWADDR, &IfInfo);
		if(0 > iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			printf("[%s:%d]%s\n", __FILE__, __LINE__, strerror(errno));
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

int SetIfStatus(LPCSTR pIfName, enum NET_IF_STATUS iNetIfStatus)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{	
		iSckFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iOutRet = SetIfStatusWithSckFd(iSckFd, pIfName, iNetIfStatus);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

int GetIfStatus(const char * pIfName, short * psiIfFlags)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{	
		if(NULL == pIfName || NULL == psiIfFlags)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iSckFd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iOutRet = GetIfStatusWithSckFd(iSckFd, pIfName, psiIfFlags);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

int GetIfStatusWithSckFd(int iSockFd, const char * pIfName, short * psiIfFlags)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	struct ifreq IfInfo;

	do{
		if(NULL == pIfName || NULL == psiIfFlags)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		memset(&IfInfo, 0x00, sizeof(IfInfo));
		strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
		IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';

		//get flags
		iRet = ioctl(iSockFd, SIOCGIFFLAGS, &IfInfo);
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}

		*psiIfFlags = IfInfo.ifr_flags;
	}while(0);

	return iOutRet;
}

int SetIfStatusWithSckFd(int iSockFd, LPCSTR pIfName, enum NET_IF_STATUS iNetIfStatus)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	struct ifreq IfInfo;

	do{
		memset(&IfInfo, 0x00, sizeof(IfInfo));
		strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
		IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';

		//get flags
		iRet = ioctl(iSockFd, SIOCGIFFLAGS, &IfInfo);
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		//set flags
		if(STATUS_NETIF_DOWN == iNetIfStatus)
		{
			IfInfo.ifr_flags &= ~IFF_UP;
		}
		else if(STATUS_NETIF_UP == iNetIfStatus)
		{
			IfInfo.ifr_flags |= (IFF_UP | IFF_RUNNING);
		}
		iRet = ioctl(iSockFd, SIOCSIFFLAGS, &IfInfo, sizeof(IfInfo));
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
	}while(0);

	return iOutRet;
}

//Get address of a interface
int GetIfAddr(const int sockfd, const char * pIfName, UINT * puiIPAddr)
{
	int nRet; struct ifreq IfInfo; struct sockaddr_in * sin; 

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	nRet = ioctl(sockfd, SIOCGIFADDR, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	
	sin = (struct sockaddr_in *)(&(IfInfo.ifr_addr));
	*puiIPAddr = ntohl(sin->sin_addr.s_addr);

	return ERROR_SUCCESS; 
}

int GetIfAddrIPV4(LPCSTR pszIfName, UINT32_t * puiIPAddr)
{
	int iOutRet = 0, iRet = 0, iFdSck = INVALID_SOCKET;

	do{
		iFdSck = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(INVALID_SOCKET == iFdSck)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;			
			break;
		}

		iRet = GetIfAddr(iFdSck, pszIfName, puiIPAddr);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
		}
	}while(FALSE);

	if(INVALID_SOCKET != iFdSck)
	{
		iRet = close(iFdSck);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iFdSck = INVALID_SOCKET;
	}

	return iOutRet;
}

//delay 
int Sleep2(long nuSec)
{
	struct timeval timeVal; int nRet;

	timeVal.tv_sec = 0; timeVal.tv_usec = nuSec;
	nRet = select(0, 0, 0, 0, &timeVal);

	if(-1 == nRet)
		return ERROR_SELECT_CALL_FAILED;
	
	return ERROR_SUCCESS; 
}

//Get broadcast address of a specified interface
int GetIfBroadcastAddr(int sockfd, char * pIfName, UINT * puiBroadcastAddr)
{
	int nRet; struct ifreq IfInfo; struct sockaddr_in * sin; 

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	sin = (struct sockaddr_in *)(&(IfInfo.ifr_broadaddr));
	sin->sin_family = AF_INET;
	nRet = ioctl(sockfd, SIOCGIFBRDADDR, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	
	*puiBroadcastAddr = ntohl(sin->sin_addr.s_addr);

	return ERROR_SUCCESS; 
}

int SetIfBroadcastAddr(const int iSockFd, const char * pszIfName, const UINT uiBroadcastAddr)
{
	int nRet; struct ifreq IfInfo; struct sockaddr_in * sin; 

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pszIfName, sizeof(IfInfo.ifr_name));
	IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';
	sin = (struct sockaddr_in *)(&(IfInfo.ifr_broadaddr));
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = htonl(uiBroadcastAddr);
	nRet = ioctl(iSockFd, SIOCSIFBRDADDR, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	
	return ERROR_SUCCESS; 
}

//Get netmask
int GetIfNetmask(const SOCKET sockfd, LPCSTR pIfName, UINT32_t * puiNetmask)
{
	int nRet; struct ifreq IfInfo; struct sockaddr_in * sin; 

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	nRet = ioctl(sockfd, SIOCGIFNETMASK, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	
	sin = (struct sockaddr_in *)(&(IfInfo.ifr_netmask));
	*puiNetmask = ntohl(sin->sin_addr.s_addr);

	return ERROR_SUCCESS; 
}

INT getNetIfSubnetMask(LPCSTR pszNetIfName, OUT UINT32_t * puiSubnetMask)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{
		if(NULL == pszNetIfName || NULL == puiSubnetMask)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		iSckFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}

		iOutRet = GetIfNetmask(iSckFd, pszNetIfName, puiSubnetMask);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

//Set subnet-mask
INT SetIfNetmask(LPCSTR pszNetIfName, CONST UINT32_t uiSubnetMask)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	int iSck = INVALID_SOCKET;

	do{
		iSck = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSck)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}
		
		iOutRet = SetIfNetmaskWithSckFd(iSck, pszNetIfName, uiSubnetMask);
	}while(FALSE);

	if(INVALID_SOCKET != iSck)
	{
		iRet = close(iSck);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSck = INVALID_SOCKET;
	}
	
	return iOutRet;
}

INT SetIfNetmaskWithSckFd(CONST SOCKET hSck, LPCSTR pszNetIfName, CONST UINT32_t uiSubnetMask)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	struct ifreq IfInfo; 
	struct sockaddr_in * sin; 

	do{
		if(NULL == pszNetIfName)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		ZeroMemory(&IfInfo, sizeof(IfInfo));
		strncpy(IfInfo.ifr_name, pszNetIfName, sizeof(IfInfo.ifr_name));
		IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';
		sin = (struct sockaddr_in *)(&(IfInfo.ifr_netmask));
		sin->sin_family = PF_INET;
		sin->sin_addr.s_addr = htonl(uiSubnetMask);
		iRet = ioctl(hSck, SIOCSIFNETMASK, &IfInfo, sizeof(IfInfo));
		if(0 > iRet)
		{
			iOutRet = ERROR_SOCKET_IOCTL_FAIL;
			break;
		}
	}while(FALSE);
	
	return iOutRet;
}

int SetIfNetmask2(char * pIfName, const char * pszSubnetMask)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS; 
	UINT uiSubnetMask = 0;
	struct in_addr addrIn;

	do{
		iRet = inet_aton(pszSubnetMask, &addrIn);
		if(0 == iRet)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		uiSubnetMask = ntohl(addrIn.s_addr);
		iOutRet = SetIfNetmask(pIfName, uiSubnetMask);
	}while(0);

	return iOutRet;
}

//Set the ip address of specified interface
int SetIPAddr(const int sockfd, const char * pIfName, UINT uiIPAddr)
{
	int nRet; struct ifreq IfInfo; struct sockaddr_in sin; 

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';
	sin.sin_family = AF_INET; sin.sin_port = 0; sin.sin_addr.s_addr = htonl(uiIPAddr);
	memcpy(&(IfInfo.ifr_addr), &sin, sizeof(IfInfo.ifr_addr));
	//set ip
	nRet = ioctl(sockfd, SIOCSIFADDR, &IfInfo, sizeof(IfInfo));
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;

	/*
	//get flags
	nRet = ioctl(sockfd, SIOCGIFFLAGS, &IfInfo);
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	//set flags
	IfInfo.ifr_flags |= (IFF_UP);
	nRet = ioctl(sockfd, SIOCSIFFLAGS, &IfInfo, sizeof(IfInfo));
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	*/

	return ERROR_SUCCESS; 
}

int SetIPAddr2(SOCKET sockfd, LPCSTR pIfName, LPCSTR sIPAddr)
{
	int nRet; 
	struct ifreq IfInfo; 
	struct sockaddr_in sin;

	memset(&IfInfo, 0x00, sizeof(IfInfo));
	strncpy(IfInfo.ifr_name, pIfName, sizeof(IfInfo.ifr_name));
	sin.sin_family = AF_INET; sin.sin_port = 0;
	sin.sin_addr.s_addr = inet_addr(sIPAddr);
	if(INADDR_NONE == sin.sin_addr.s_addr)
		return ERROR_INVALID_IP_FORMAT;
	memcpy(&(IfInfo.ifr_addr), &sin, sizeof(IfInfo.ifr_addr));
	//set ip
	nRet = ioctl(sockfd, SIOCSIFADDR, &IfInfo, sizeof(IfInfo));
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	//get flags
	nRet = ioctl(sockfd, SIOCGIFFLAGS, &IfInfo, sizeof(IfInfo));
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;
	//set flags
	IfInfo.ifr_flags |= (IFF_UP);
	nRet = ioctl(sockfd, SIOCSIFFLAGS, &IfInfo, sizeof(IfInfo));
	if(nRet < 0)
		return ERROR_IOCTL_FAILED;

	return ERROR_SUCCESS; 
}

INT setNetIfIPv4Addr2(SOCKET hSck, LPCSTR pszNetIfName, CONST UINT32_t uiIPv4Addr)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	struct ifreq IfInfo; 
	struct sockaddr_in sin;

	do{
		if(NULL == pszNetIfName)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		ZeroMemory(&IfInfo, sizeof(IfInfo));
		strncpy(IfInfo.ifr_name, pszNetIfName, sizeof(IfInfo.ifr_name));
		IfInfo.ifr_name[sizeof(IfInfo.ifr_name)-1] = '\0';
		sin.sin_family = AF_INET; 
		sin.sin_port = 0;
		sin.sin_addr.s_addr = htonl(uiIPv4Addr);
		memcpy(&(IfInfo.ifr_addr), &sin, sizeof(IfInfo.ifr_addr));
		//set ip
		iRet = ioctl(hSck, SIOCSIFADDR, &IfInfo, sizeof(IfInfo));
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		//get flags
		iRet = ioctl(hSck, SIOCGIFFLAGS, &IfInfo, sizeof(IfInfo));
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
		//set flags
		IfInfo.ifr_flags |= (IFF_UP);
		iRet = ioctl(hSck, SIOCSIFFLAGS, &IfInfo, sizeof(IfInfo));
		if(iRet < 0)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int SetIPAddr3(char * pIfName, char * sIPAddr)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{
		iSckFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iOutRet = SetIPAddr2(iSckFd, pIfName, sIPAddr);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

INT setNetIfIPv4Addr(LPCSTR pszNetIfName, CONST UINT32_t uiIPv4Addr)
{
	INT iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{
		iSckFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			break;
		}

		iOutRet = setNetIfIPv4Addr2(iSckFd, pszNetIfName, uiIPv4Addr);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

int SetSysNetCoreRecvBufMaxMem(int iSize)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS;
	FILE * fpProc = NULL;

	if(0 > 64*1024 || 2*1024*1024 < iSize) 
	{
		return ERROR_INVALID_PARAMETER;
	}

	do{
		fpProc = fopen("/proc/sys/net/core/rmem_max", "wt");
		if(NULL == fpProc)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iRet = fprintf(fpProc, "%d", iSize);
		if(0 >= iRet)
		{
			iOutRet = ERROR_FILE_WRITE_FAIL;
			PRINT_FILE_LINENO_BUG_STR;
		}
	}while(0);

	if(fpProc)
	{
		iRet = fclose(fpProc);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		fpProc = NULL;
	}

	return iOutRet;
}

int SetIPAddrNetmask(const char * pszIfName, const char * pszIpAddr, const char * pszNetmask)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, iSckFd = INVALID_SOCKET;

	do{
		iSckFd = socket(PF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}

		iOutRet = SetIPAddrNetmaskWithSckFd(iSckFd, pszIfName, pszIpAddr, pszNetmask);
	}while(0);

	if(INVALID_SOCKET != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = INVALID_SOCKET;
	}

	return iOutRet;
}

int SetIPAddrNetmaskWithSckFd(int iSockFd, const char * pszIfName, const char * pszIpAddr, 
	const char * pszNetmask)
{
	int iOutRet = ERROR_SUCCESS, iRet = ERROR_SUCCESS, bRet = 0, bSucc = 0;
	UINT uiIpAddr = {0, }, uiNetmask = {0, }, uiBroadcastAddr = {0, };

	//if up
	iRet = SetIfStatusWithSckFd(iSockFd, pszIfName, STATUS_NETIF_UP);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}

	do{
		//get ip addr
		bSucc = 0;
		do{
			if(pszIpAddr)
			{
				struct in_addr oInAddr = {0, };
				bRet = inet_aton(pszIpAddr, &oInAddr);
				if(1 == bRet)	// success
				{
					uiIpAddr = ntohl(oInAddr.s_addr);
					bSucc = 1;
					break;
				}
			}

			//get the current ip addr
			iRet = GetIfAddr(iSockFd, pszIfName, &uiIpAddr);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			bSucc = 1;
		}while(0);
		if(0 == bSucc)
		{
			break;
		}

		//get netmask
		bSucc = 0;
		do{
			if(pszNetmask)
			{
				struct in_addr oInAddr = {0, };
				bRet = inet_aton(pszNetmask, &oInAddr);
				if(1 == bRet)	// success
				{
					uiNetmask = ntohl(oInAddr.s_addr);
					bSucc = 1;
					break;
				}
			}

			//get the current ip addr
			iRet = GetIfNetmask(iSockFd, pszIfName, &uiNetmask);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			bSucc = 1;
		}while(0);
		if(0 == bSucc)
		{
			break;
		}

		//calculate the broadcast
		uiBroadcastAddr = ~uiNetmask | (uiIpAddr & uiNetmask);

		//the 3 addres is ok, then set it.
		iRet = SetIPAddr(iSockFd, pszIfName, uiIpAddr);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SetIfNetmaskWithSckFd(iSockFd, pszIfName, uiNetmask);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = SetIfBroadcastAddr(iSockFd, pszIfName, uiBroadcastAddr);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(0);

	//if up
	iRet = SetIfStatusWithSckFd(iSockFd, pszIfName, STATUS_NETIF_UP);
	if(ERROR_SUCCESS != iRet)
	{
		PRINT_FILE_LINENO_IRET_CRT_ERR;
	}
	
	return iOutRet;
}

int DelDefaultGateway()
{
	int iOutRet = 0, iRet = 0;
	int iSckFd = -1;
	struct rtentry oRouteEntry;
	struct sockaddr_in * pSockAddr;

	do{
		iSckFd = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		memset(&oRouteEntry, 0x00, sizeof(oRouteEntry));
		oRouteEntry.rt_flags = RTF_UP;
		pSockAddr = (typeof(pSockAddr))(&(oRouteEntry.rt_dst));
		pSockAddr->sin_family = AF_INET;
		iRet = ioctl(iSckFd, SIOCDELRT, &oRouteEntry);
		if(-1 == iRet && ESRCH != errno && ENETUNREACH != errno)
		{
			iOutRet = ERROR_SOCKET_IOCTL_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(0);

	if(-1 != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = -1;
	}

	return iOutRet;
}

int DelDefaultGatewayIfc(const char *ifname)
{
	int iOutRet = 0, iRet = 0;
	int iSckFd = -1;
	struct rtentry oRouteEntry;
	struct sockaddr_in * pSockAddr;

	do{
		iSckFd = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		memset(&oRouteEntry, 0x00, sizeof(oRouteEntry));
		oRouteEntry.rt_dev = (void *)ifname;
		oRouteEntry.rt_flags = RTF_UP | RTF_GATEWAY;
		pSockAddr = (typeof(pSockAddr))(&(oRouteEntry.rt_dst));
		pSockAddr->sin_family = AF_INET;
		pSockAddr->sin_port = 0;
		pSockAddr->sin_addr.s_addr = 0;
		iRet = ioctl(iSckFd, SIOCDELRT, &oRouteEntry);
		if(-1 == iRet && ESRCH != errno && ENETUNREACH != errno)
		{
			iOutRet = ERROR_SOCKET_IOCTL_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(0);

	if(-1 != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = -1;
	}

	return iOutRet;
}

int AddDefaultGateway(LPCSTR pszGatewayIpAddr, LPCSTR pszDevName)
{
	int iOutRet = 0, iRet = 0;
	struct in_addr addrIn;

	do{
		if(NULL == pszGatewayIpAddr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iRet = inet_aton(pszGatewayIpAddr, &addrIn);
		if(0 == iRet)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		iOutRet = AddDefaultGateway2(ntohl(addrIn.s_addr), pszDevName);
	}while(0);

	return iOutRet;
}

int AddDefaultGateway2(CONST UINT32_t uiGatewayIpAddr, LPCSTR pszDevName)
{
	int iOutRet = 0, iRet = 0;
	struct rtentry oRouteEntry;
	int iSckFd = -1;
	struct sockaddr_in * poSckAddr = NULL;

	do{
		iSckFd = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		memset(&oRouteEntry, 0x00, sizeof(oRouteEntry));

		oRouteEntry.rt_flags = RTF_UP | RTF_GATEWAY;
		if(pszDevName)
		{
			oRouteEntry.rt_dev = (char *)pszDevName;
		}

		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_dst));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_port = 0;
		poSckAddr->sin_addr.s_addr = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_genmask));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_port = 0;
		poSckAddr->sin_addr.s_addr = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_gateway));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_port = 0;
		poSckAddr->sin_addr.s_addr = htonl(uiGatewayIpAddr);

		oRouteEntry.rt_metric = 0;
				
		iRet = ioctl(iSckFd, SIOCADDRT, &oRouteEntry);
		if(-1 == iRet)
		{
			if(ENETUNREACH != errno)
			{
				iOutRet = ERROR_SOCKET_IOCTL_FAIL;
				PRINT_FILE_LINENO_CRT_ERRINFO;
				break;
			}
		}
	}while(0);

	if(-1 != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = -1;
	}

	return iOutRet;
}

int RouteAddDefault(LPCSTR pszDevName)
{
	int iOutRet = 0, iRet = 0;
	struct rtentry oRouteEntry;
	int iSckFd = -1;
	struct sockaddr_in * poSckAddr = NULL;

	do{
		iSckFd = socket(AF_INET, SOCK_DGRAM, 0);
		if(-1 == iSckFd)
		{
			iOutRet = ERROR_CREATE_SOCKET_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
		
		memset(&oRouteEntry, 0x00, sizeof(oRouteEntry));
		oRouteEntry.rt_flags = RTF_UP/* | RTF_GATEWAY*/;
		if(pszDevName)
		{
			oRouteEntry.rt_dev = (char *)pszDevName;
		}

		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_dst));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = 0;
		poSckAddr->sin_port = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_genmask));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = 0;
		poSckAddr->sin_port = 0;
		
		poSckAddr = (struct sockaddr_in *)(&(oRouteEntry.rt_gateway));
		poSckAddr->sin_family = AF_INET;
		poSckAddr->sin_addr.s_addr = 0;
		poSckAddr->sin_port = 0;

		oRouteEntry.rt_metric = 0;
				
		iRet = ioctl(iSckFd, SIOCADDRT, &oRouteEntry);
		if(-1 == iRet)
		{
			iOutRet = ERROR_SOCKET_IOCTL_FAIL;
			PRINT_FILE_LINENO_CRT_ERRINFO;
			break;
		}
	}while(0);

	if(-1 != iSckFd)
	{
		iRet = close(iSckFd);
		if(-1 == iRet)
		{
			PRINT_FILE_LINENO_CRT_ERRINFO;
		}
		iSckFd = -1;
	}

	return iOutRet;
}

INT getIPv4AddrBySck(CONST SOCKET hSck, OUT UINT32_t * puiIPv4Addr)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	SOCKADDR_IN sckIPv4Addr;
	socklen_t sckAddrLen = sizeof(sckIPv4Addr);

	do{
		if(NULL == puiIPv4Addr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		iRet = getsockname(hSck, (SOCKADDR *)(&sckIPv4Addr), &sckAddrLen);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = ERR_GETSOCKNAME_FAIL;
			break;
		}

		if(AF_INET != sckIPv4Addr.sin_family)
		{
			iOutRet = ERR_SOCK_NOT_IPv4ADDR;
			break;
		}

		*puiIPv4Addr = ntohl(sckIPv4Addr.sin_addr.s_addr);
	}while(FALSE);

	return iOutRet;
}

INT IPv4AddrInSysNetIfList(CONST SOCKET hSck, CONST UINT32_t uiIPv4Addr, OUT BOOL_t * pbAvailable)
{
	INT iOutRet = ERROR_SUCCESS, iRet;
	struct ifconf oIfConf;
	struct ifreq *pIfReq = NULL;
	INT iId = 0;

	ZeroMemory(&oIfConf, sizeof(oIfConf));

	do{
		if(NULL == pbAvailable)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		*pbAvailable = FALSE;

		oIfConf.ifc_len = sizeof(struct ifreq) * 50;
		oIfConf.ifc_buf = malloc(oIfConf.ifc_len);
		if(NULL == oIfConf.ifc_buf)
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		iRet = ioctl(hSck, SIOCGIFCONF, &oIfConf);
		if(0 > iRet)
		{
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}

		pIfReq = oIfConf.ifc_req;
		for(iId=0; iId<oIfConf.ifc_len/sizeof(struct ifreq); iId++)
		{
			SOCKADDR * pSckAddr = (typeof(pSckAddr))(&(pIfReq->ifr_addr));
			if('\0' == pIfReq->ifr_name[0])
			{
				break;
			}
			if(AF_INET == pSckAddr->sa_family)
			{
				SOCKADDR_IN * pIPv4SckAddr = (typeof(pIPv4SckAddr))pSckAddr;
				/*
				PRINT_FILE_LINENO;
				printf("IfName:%s, IPv4Addr:%s\n", pIfReq->ifr_name, inet_ntoa(pIPv4SckAddr->sin_addr));
				*/
				if(pIPv4SckAddr->sin_addr.s_addr == htonl(uiIPv4Addr))
				{
					*pbAvailable = TRUE;
					break;
				}
			}
			pIfReq++;
		}
	}while(FALSE);

	if(oIfConf.ifc_buf)
	{
		free(oIfConf.ifc_buf);
		oIfConf.ifc_buf = NULL;
	}

	return iOutRet;
}

INT getIPv4AddrFromStr(LPCSTR pszIPv4Addr, OUT UINT32_t * puiIPv4Addr)
{
	INT iOutRet = ERROR_SUCCESS;
	BOOL_t bRet;
	struct in_addr oInAddr;

	do{
		if(NULL == pszIPv4Addr || NULL == puiIPv4Addr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		bRet = inet_aton(pszIPv4Addr, &oInAddr);
		if(FALSE == bRet)
		{
			iOutRet = ERR_INVALID_IPv4ADDR;
			break;
		}

		*puiIPv4Addr = ntohl(oInAddr.s_addr);
	}while(FALSE);

	return iOutRet;
}

INT_t convertIPv4Addr_BinToStr(UINT32_t uiIPv4Addr, char * pszBuf, size_t BufSize)
{
	INT_t iOutRet = ERROR_SUCCESS;
	const char * pszRet = NULL;
	struct in_addr NetAddr;

	do
	{
		if(NULL == pszBuf || 16 > BufSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		pszBuf[0] = '\0';
		NetAddr.s_addr = htonl(uiIPv4Addr);
		pszRet = inet_ntop(AF_INET, &NetAddr, pszBuf, BufSize);
		if(NULL == pszRet)
		{
			iOutRet = ERR_CONVERT_FAIL;
			break;
		}
		pszBuf[BufSize-1] = '\0';
	}while(FALSE);

	return iOutRet;
}

