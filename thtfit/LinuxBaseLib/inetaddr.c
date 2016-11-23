#include	"inetaddr.h"
#include	<BaseErrDef.h>
#include	<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "math2.h"

int inet_addr2(LPCSTR pszHost, unsigned int * puiNetAddr )
{
	int iOutRet = ERROR_SUCCESS;
	unsigned int uiNetAddr = 0;
	struct hostent * pHostEnt = NULL;

	do{
		if(NULL == pszHost || NULL == puiNetAddr)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}	
		
		uiNetAddr = inet_addr(pszHost);
		if(INADDR_NONE != uiNetAddr)
		{
			break;
		}

		pHostEnt = gethostbyname(pszHost);
		if(NULL == pHostEnt)
		{
			iOutRet = ERROR_CALL_SYSTEM_FAILED;
			break;
		}
		memcpy(&uiNetAddr, pHostEnt->h_addr, GetMinVal(pHostEnt->h_length, sizeof(uiNetAddr)));
	}while(0);

	if(ERROR_SUCCESS == iOutRet)
	{
		*puiNetAddr = uiNetAddr;
	}

	return iOutRet;
}

BOOL_t IsValidIpAddr(LPCSTR pszIpAddr, IN OUT P_SOCKADDR pSockAddr)
{
	struct in_addr oInAddr;
	struct in_addr * pInAddr = &oInAddr;

	if(NULL == pszIpAddr)
	{
		return FALSE;
	}

	if(pSockAddr)
	{
		pInAddr = &(((P_SOCKADDR_IN)pSockAddr)->sin_addr);
	}

	return (inet_aton(pszIpAddr, pInAddr));
}

BOOL_t IsValidIpV4Addr(LPCSTR pszIpAddr, IN OUT P_SOCKADDR pSockAddr)
{
	return IsValidIpAddr(pszIpAddr, pSockAddr);
}

