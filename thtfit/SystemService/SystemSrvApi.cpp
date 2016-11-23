#include "SystemSrvApi.h"
#include <stdio.h>
#include <string.h>

namespace SystemSrv
{

NetCfg_ActionScope_T getNetCfgActionScopeFromDesc(LPCSTR pszNetCfgActionScopeDesc)
{
	NetCfg_ActionScope_T eNetCfgActionType = NetCfg_ActionScope_UNKNOWN;

	if(NULL == pszNetCfgActionScopeDesc)
	{		
	}
	else if(0 == strcmp(PARAM_VALUE_Current, pszNetCfgActionScopeDesc))
	{
		eNetCfgActionType = NetCfg_ActionScope_CURRENT;
	}
	else if(0 == strcmp(PARAM_VALUE_SavedCfg, pszNetCfgActionScopeDesc))
	{
		eNetCfgActionType = NetCfg_ActionScope_SavedCfg;
	}

	return eNetCfgActionType;
}

}

