#ifndef	_INET_ADDR_H_
#define	_INET_ADDR_H_

#include	<BaseTypeDef.h>
#include	<CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

int inet_addr2(const char * pszHost, unsigned int * puiNetAddr);
BOOL_t IsValidIpAddr(LPCSTR pszIpAddr, IN OUT P_SOCKADDR pSockAddr);
BOOL_t IsValidIpV4Addr(LPCSTR pszIpAddr, IN OUT P_SOCKADDR pSockAddr);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_INET_ADDR_H_

