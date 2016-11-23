#include <stdio.h>
#include <MN_DMD_driver.h>
#include <memory2.h>
#include <ErrPrintHelper.h>
#include <StackBufString.h>

#define	CMD_TUNE	"1"
#define	CMD_QUIT	"Q"

static LPSTR Stdin_getLine(char * pszLineBuf, size_t lineBufSize)
{
	LPSTR pszRet;
	pszRet = fgets(pszLineBuf, lineBufSize, stdin);
	if(pszRet)
	{		
		pszLineBuf[lineBufSize-1] = '\0';
	}
	else if(EOF == (typeof(EOF))pszRet)
	{
		pszRet = NULL;
	}
	return pszRet;
}

static BOOL_t Cmd_isQuit(LPCSTR pszCmd)
{
	return (0 == strcmp(CMD_QUIT, pszCmd) || 0 == strcmp(CMD_QUIT "\n", pszCmd));
}

static BOOL_t Cmd_isTune(LPCSTR pszCmd)
{
	return (0 == strcmp(CMD_TUNE, pszCmd) || 0 == strcmp(CMD_TUNE "\n", pszCmd));
}

int main(int argc, char * argv[])
{
	DMD_PARAMETER_t dmdParam;
	DMD_ERROR_t dmdErr;
	char szCmdLine[256];
	char * pszRet = NULL;

	ZeroMemory(&dmdParam, sizeof(dmdParam));

	dmdErr = DMD_open(&dmdParam);
	if(DMD_E_OK != dmdErr)
	{
		LOG_BLINE("DMD_open err %d\n", dmdErr);
		goto EXIT_PROC;
	}

	//Select device ID
	dmdParam.devid = 0;

	dmdErr = DMD_init(&dmdParam);
	if(DMD_E_OK != dmdErr)
	{
		LOG_BLINE("DMD_init err %d\n", dmdErr);
		goto EXIT_PROC;
	}

	dmdParam.system = DMD_E_QAMB_256QAM;
	
	printf("Menu\n");
	printf("1. Tune\n");
	printf("Q. Exit\n");
	while(TRUE)
	{
		pszRet = Stdin_getLine(szCmdLine, sizeof(szCmdLine));
		if(NULL == pszRet)
		{
			break;
		}
		if(Cmd_isQuit(szCmdLine))
		{
			break;
		}
		else if(Cmd_isTune(szCmdLine))
		{
			printf("Freq(KHz): ");
			pszRet = Stdin_getLine(szCmdLine, sizeof(szCmdLine));
			if(NULL == pszRet)
			{
				break;
			}
			DECLARE_CLS_STACK_BUF_STRING(strLine, 256);
			strLine = szCmdLine;
			INT_t freqKHz = strLine;
			if(0 >= freqKHz)
			{
				printf("Invalid freq %d\n", freqKHz);
				goto MAIN_CMD_END;
			}
			printf("Freq=%dKHz\n", freqKHz);
			dmdParam.freq = freqKHz;
			dmdParam.funit = DMD_E_KHZ;
			MXL603_CHAN_TUNE_CFG_T mxl603ChannelTuneCfg;
			mxl603ChannelTuneCfg.freqInHz = freqKHz*1000;
			mxl603ChannelTuneCfg.signalMode = MXL603_DIG_J83B;
			mxl603ChannelTuneCfg.startTune = MXL_START_TUNE;
			mxl603ChannelTuneCfg.bandWidth = MXL603_CABLE_BW_6MHz;
			mxl603ChannelTuneCfg.xtalFreqSel = MXL603_XTAL_24MHz;
			dmdErr = DMD_scan(&dmdParam, &mxl603ChannelTuneCfg);
			if(DMD_E_OK != dmdErr)
			{
				LOG_BLINE("DMD_scan err %d\n", dmdErr);
			}
		}
MAIN_CMD_END:
		;
	}

EXIT_PROC:

	dmdErr = DMD_close(&dmdParam);
	if(DMD_E_OK != dmdErr)
	{
		LOG_BLINE("DMD_close err %d\n", dmdErr);
	}
	
	return 0;
}

