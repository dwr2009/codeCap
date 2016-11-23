#include <stdio.h>
#include <CmdLineArgument.h>
#include <StackBufString.h>
#include "SysBaseSrvApi.h"

#define	SPOA_ExitCode_PARAM_ERR		1
#define	SPOA_ExitCode_OTHER_ERR		2

#define	CMD_PARA_get			"-get"
#define	CMD_PARA_getAll			"-getAll"
#define	CMD_PARA_set			"-set"
#define	CMD_PARA_persistent		"-persistent"
#define	CMD_PARA_del			"-del"

class CSysPropOpsApp
{
public:
	typedef enum
	{
		SysPropOps_NONE,
		SysPropOps_SET_TEMP,
		SysPropOps_SET_PERSISTENT,
		SysPropOps_GET,
		SysPropOps_GET_All,
		SysPropOps_DEL,
	}SysProp_OPS_TYPE, *P_SysProp_OPS_TYPE;
public:
	CSysPropOpsApp(int argc, char * argv[]);
	~CSysPropOpsApp();
	INT_t Exec();
private:
	INT_t ParseCmdLineArgs();
private:
	CCmdLineArgument m_CmdLineArgs;
	SysProp_OPS_TYPE m_eSysPropOpsType;
	CString m_strSysPropKey;
	BOOL_t m_bSysPropKeyIsSet;
	CString m_strSysPropVal;
	BOOL_t m_bSysPropValIsSet;
};

CSysPropOpsApp::CSysPropOpsApp(int argc, char * argv[]) : m_CmdLineArgs(argc, argv)
{
	m_eSysPropOpsType = SysPropOps_NONE;
	m_bSysPropKeyIsSet = FALSE;
	m_bSysPropValIsSet = FALSE;
}

CSysPropOpsApp::~CSysPropOpsApp()
{
}

INT_t CSysPropOpsApp::ParseCmdLineArgs()
{
	INT_t iOutRet = ERROR_SUCCESS;
	INT_t iId;

	do
	{
		if(0 == m_CmdLineArgs.getSize())
		{
			LOG("Too few parameters.\n");
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		for(iId = 0; iId < (typeof(iId))m_CmdLineArgs.getSize(); iId++)
		{
			if(m_CmdLineArgs[iId].Equal(CMD_PARA_get))
			{
				m_eSysPropOpsType = SysPropOps_GET;
			}
			else if(m_CmdLineArgs[iId].Equal(CMD_PARA_getAll))
			{
				m_eSysPropOpsType = SysPropOps_GET_All;
			}
			else if(m_CmdLineArgs[iId].Equal(CMD_PARA_set))
			{
				m_eSysPropOpsType = SysPropOps_SET_TEMP;			
			}
			else if(m_CmdLineArgs[iId].Equal(CMD_PARA_persistent))
			{
				if(SysPropOps_SET_TEMP == m_eSysPropOpsType)
				{
					m_eSysPropOpsType = SysPropOps_SET_PERSISTENT;			
				}
			}
			else if(m_CmdLineArgs[iId].Equal(CMD_PARA_del))
			{
				m_eSysPropOpsType = SysPropOps_DEL;			
			}
			else if(m_CmdLineArgs[iId][0] != '-')
			{
				if(FALSE == m_bSysPropKeyIsSet)
				{
					m_strSysPropKey = m_CmdLineArgs[iId];
					m_bSysPropKeyIsSet = TRUE;
				}
				else if(FALSE == m_bSysPropValIsSet)
				{
					m_strSysPropVal = m_CmdLineArgs[iId];
					m_bSysPropValIsSet = TRUE;
				}
				else
				{
					LOG("Invalid parameter %s.\n", (LPCSTR)(m_CmdLineArgs[iId]));
					iOutRet = ERROR_INVALID_PARAMETER;
					break;
				}
			}
			else
			{
				LOG("Invalid parameter %s.\n", (LPCSTR)(m_CmdLineArgs[iId]));
				iOutRet = ERROR_INVALID_PARAMETER;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CSysPropOpsApp::Exec()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;

	do
	{
		iRet = ParseCmdLineArgs();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		if(SysPropOps_GET == m_eSysPropOpsType)
		{
			if(FALSE == m_bSysPropKeyIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Get needs a key name\n");
				break;
			}
			DECLARE_CLS_STACK_BUF_STRING(strVal, 16*1024);
			iRet = SysProp_get(m_strSysPropKey, strVal);
			if(ERROR_NOT_FOUND == iRet)
			{
				strVal = "";
			}
			else if(ERROR_SUCCESS != iRet)
			{
				LOG("GetOp err %d\n", iRet);
				iOutRet = ERR_GET_ATTR_FAIL;
				break;
			}
			LOG("%s\n", (LPCSTR)strVal);
		}
		else if(SysPropOps_GET_All == m_eSysPropOpsType)
		{
			INT_t iPropCount = 0;
			iRet = SysProp_getCount(&iPropCount);
			if(ERROR_SUCCESS != iRet)
			{
				LOG("GetAllOp err %d\n", iRet);
				iOutRet = ERR_GET_ATTR_FAIL;
				break;
			}
			INT_t iId;
			DECLARE_CLS_STACK_BUF_STRING(strPropName, 4*1024);
			DECLARE_CLS_STACK_BUF_STRING(strPropVal, 16*1024);
			for(iId = 0; iId < iPropCount; iId++)
			{
				iRet = SysProp_get(iId, strPropName, strPropVal);
				if(ERROR_SUCCESS != iRet)
				{
					if(ERROR_NOT_FOUND == iRet)	//is OK
					{
					}
					else
					{
						LOG("GetAllOp err %d\n", iRet);
						iOutRet = ERR_GET_ATTR_FAIL;
					}
					break;
				}
				LOG("%s=%s\n", (LPCSTR)strPropName, (LPCSTR)strPropVal);
			}
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
		}
		else if(SysPropOps_SET_TEMP == m_eSysPropOpsType)
		{
			if(FALSE == m_bSysPropKeyIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Set needs a key name\n");
				break;
			}
			if(FALSE == m_bSysPropValIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Set needs a value\n");
				break;
			}
			iRet = SysProp_setTemp(m_strSysPropKey, m_strSysPropVal);
			if(ERROR_SUCCESS != iRet)
			{
				LOG("SetOp err %d\n", iRet);
				iOutRet = ERR_SET_ATTR_FAIL;
				break;
			}
		}
		else if(SysPropOps_SET_PERSISTENT == m_eSysPropOpsType)
		{
			if(FALSE == m_bSysPropKeyIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Set needs a key name\n");
				break;
			}
			if(FALSE == m_bSysPropValIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Set needs a value\n");
				break;
			}
			iRet = SysProp_setPersistent(m_strSysPropKey, m_strSysPropVal);
			if(ERROR_SUCCESS != iRet)
			{
				LOG("SetOp err %d\n", iRet);
				iOutRet = ERR_SET_ATTR_FAIL;
				break;
			}
		}
		else if(SysPropOps_DEL == m_eSysPropOpsType)
		{
			if(FALSE == m_bSysPropKeyIsSet)
			{
				iOutRet = ERROR_INVALID_PARAMETER;
				LOG("Del needs a key name\n");
				break;
			}
			iRet = SysProp_del(m_strSysPropKey);
			if(ERROR_NOT_FOUND == iRet)
			{
				LOG("Not found(%s)\n", (LPCSTR)m_strSysPropKey);
				iOutRet = ERR_GET_ATTR_FAIL;
				break;
			}
			else if(ERROR_SUCCESS != iRet)
			{
				LOG("DelOp err %d\n", iRet);
				iOutRet = ERR_GET_ATTR_FAIL;
				break;
			}
		}
		else
		{
			iOutRet = ERR_INVALID_CMD;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int main(int argc, char * argv[])
{
	int iProcessExitCode = 0, iRet;
	CSysPropOpsApp SysPropOpsApp(argc, argv);

	do
	{
		iRet = SysPropOpsApp.Exec();
		if(ERROR_INVALID_PARAMETER == iRet)
		{
			iProcessExitCode = SPOA_ExitCode_PARAM_ERR;
		}
		else if(ERROR_SUCCESS != iRet)
		{
			iProcessExitCode = SPOA_ExitCode_OTHER_ERR;			
		}
	}while(FALSE);

	return iProcessExitCode;
}

