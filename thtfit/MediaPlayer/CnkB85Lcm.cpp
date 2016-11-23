#ifndef	ALLOW_OS_CODE
#define	ALLOW_OS_CODE
#endif

#include "CnkB85Lcm.h"
#include <BaseErrDef.h>
#include <unistd.h>
#include <BaseTypeDef.h>
#include <DateTime.h>

CCnkB85Lcm::CCnkB85Lcm()
{
	INT_t iRet = ERROR_SUCCESS;
	m_bInited = FALSE;

	do
	{
		m_GpioCtrl = SharedPtr <CLedScrGpioCtrlIf> (LedScrGpioCtrl_new());
		if(NULL == m_GpioCtrl.get())
		{
			break;
		}
		if(FALSE == m_GpioCtrl->ChkInit())
		{
			break;
		}
		iRet = setCs0(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		iRet = setCs1(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		iRet = setData(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		iRet = setClk(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			break;
		}
		m_bInited = TRUE;
	}while(FALSE);
}

CCnkB85Lcm::~CCnkB85Lcm()
{
#if	0
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE DataBuf[2];

	do
	{
		//CS0
			//Cmd 02H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x40;
		iRet = sendData(LCM_CHIP_SELECT_0, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//CS1
			//Cmd 02H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x40;
		iRet = sendData(LCM_CHIP_SELECT_1, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);
#endif
	m_GpioCtrl.Clear();
}

BOOL_t CCnkB85Lcm::ChkInit()
{
	return m_bInited;
}

INT_t CCnkB85Lcm::InitLCD()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE DataBuf[2];

	do
	{
		if(FALSE == m_bInited)
		{
			iOutRet = ERR_NOT_INITIALIZED;
			break;
		}
		//CS0
			//Cmd 18H
		DataBuf[0] = 0x83;
		DataBuf[1] = 0x00;
		iRet = sendData(LCM_CHIP_SELECT_0, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 01H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x20;
		iRet = sendData(LCM_CHIP_SELECT_0, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 03H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x60;
		iRet = sendData(LCM_CHIP_SELECT_0, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 29H
		DataBuf[0] = 0x85;
		DataBuf[1] = 0x20;
		iRet = sendData(LCM_CHIP_SELECT_0, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		//CS1
			//Cmd 18H
		DataBuf[0] = 0x83;
		DataBuf[1] = 0x00;
		iRet = sendData(LCM_CHIP_SELECT_1, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 01H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x20;
		iRet = sendData(LCM_CHIP_SELECT_1, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 03H
		DataBuf[0] = 0x80;
		DataBuf[1] = 0x60;
		iRet = sendData(LCM_CHIP_SELECT_1, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
			//Cmd 29H
		DataBuf[0] = 0x85;
		DataBuf[1] = 0x20;
		iRet = sendData(LCM_CHIP_SELECT_1, DataBuf, 12);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		/*
		{
			UINT64_t PrevSysUpTimeMs, CurSysUpTimeMs;
			PrevSysUpTimeMs = GetSysUpTimeMs();
			m_GpioCtrl->DelayUs(1000);
			CurSysUpTimeMs = GetSysUpTimeMs();
			LOG_BLINE("PrevTime:%lld,CurTime:%lld\n", PrevSysUpTimeMs, CurSysUpTimeMs);
		}
		*/

		iRet = ClearScreen();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CCnkB85Lcm::setData(BOOL_t bValue)
{
	return m_GpioCtrl->setGpioLevel(CLedScrGpioCtrlIf::GpioNo_DATA, bValue);
}

INT_t CCnkB85Lcm::setClk(BOOL_t bValue)
{
	return m_GpioCtrl->setGpioLevel(CLedScrGpioCtrlIf::GpioNo_CLK, bValue);
}

INT_t CCnkB85Lcm::setCs0(BOOL_t bValue)
{
	return m_GpioCtrl->setGpioLevel(CLedScrGpioCtrlIf::GpioNo_CS0, bValue);
}

INT_t CCnkB85Lcm::setCs1(BOOL_t bValue)
{
	return m_GpioCtrl->setGpioLevel(CLedScrGpioCtrlIf::GpioNo_CS1, bValue);
}

INT_t CCnkB85Lcm::sendData(INT_t ChipSelect, PBYTE pData, size_t BitsCnt)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	size_t BitsCntSent;

	do
	{
		if(NULL == pData || 0 >= BitsCnt)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(LCM_CHIP_SELECT_0 == ChipSelect)
		{
			iRet = setCs0(LOGICAL_LEVEL_LOW);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else if(LCM_CHIP_SELECT_1 == ChipSelect)
		{
			iRet = setCs1(LOGICAL_LEVEL_LOW);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		else
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		//LOG("SendData: ");

		BitsCntSent = 0;
		INT_t DataByteOff = 0;
		INT_t BitId = 7;
		INT_t BitVal = LOGICAL_LEVEL_LOW;
		while(TRUE)
		{
			iRet = setClk(LOGICAL_LEVEL_LOW);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if((pData[DataByteOff] >> BitId) & 0x01)
			{
				BitVal = LOGICAL_LEVEL_HIGH;
			}
			else
			{
				BitVal = LOGICAL_LEVEL_LOW;
			}

			//LOG("%d", BitVal);

			iRet = setData(BitVal);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			DelayUs(LCM_HALF_CLK_PERIOD_US);

			iRet = setClk(LOGICAL_LEVEL_HIGH);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			DelayUs(LCM_HALF_CLK_PERIOD_US);

			BitId--;
			BitsCntSent++;
			if(0 > BitId)
			{
				BitId = 7;
				DataByteOff++;
			}

			if(BitsCnt <= BitsCntSent)
			{
				break;
			}
		}
		if(ERROR_SUCCESS != iOutRet)
		{
			break;
		}

		//LOG("\n");
	}while(FALSE);

	if(LCM_CHIP_SELECT_0 == ChipSelect)
	{
		iRet = setCs0(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			if(ERROR_SUCCESS == iOutRet)
			{
				iOutRet = iRet;
			}
		}
	}
	else if(LCM_CHIP_SELECT_1 == ChipSelect)
	{
		iRet = setCs1(LOGICAL_LEVEL_HIGH);
		if(ERROR_SUCCESS != iRet)
		{
			if(ERROR_SUCCESS == iOutRet)
			{
				iOutRet = iRet;
			}
		}
	}

	return iOutRet;
}

VOID CCnkB85Lcm::DelayUs(UINT_t TimeUs)
{
	INT_t iRet;

	if((4*1000) <= TimeUs)
	{
		while(TRUE)
		{
			iRet = usleep(TimeUs);
			if(0 == iRet)
			{
				break;
			}
			else if(-1 == iRet && EINTR == errno)
			{
			}
			else
			{
				PRINT_BFILE_LINENO_CRT_ERRINFO;
				break;
			}
		}
	}
	else
	{
		m_GpioCtrl->delayUs(TimeUs);
	}
}

INT_t CCnkB85Lcm::ClearScreen()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE CmdToSend[2];
	INT_t AddrCnt = DATA_SEGMENT_CNT, AddrId;

	do
	{
		for(AddrId = 0; AddrId < AddrCnt; AddrId++)
		{
			CmdToSend[0] = 0xA0;
			CmdToSend[0] |= (AddrId >> 1);
			CmdToSend[1] = (AddrId << 7);
			iRet = sendData(LCM_CHIP_SELECT_0, CmdToSend, 13);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		for(AddrId = 0; AddrId < AddrCnt; AddrId++)
		{
			CmdToSend[0] = 0xA0;
			CmdToSend[0] |= (AddrId >> 1);
			CmdToSend[1] = (AddrId << 7);
			iRet = sendData(LCM_CHIP_SELECT_1, CmdToSend, 13);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t CCnkB85Lcm::FullScreen()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BYTE CmdToSend[2];
	INT_t AddrCnt = DATA_SEGMENT_CNT, AddrId;

	do
	{
		for(AddrId = 0; AddrId < AddrCnt; AddrId++)
		{
			CmdToSend[0] = 0xA0;
			CmdToSend[0] |= (AddrId >> 1);
			CmdToSend[1] = (AddrId << 7);
			CmdToSend[1] |= (0x0F << 3);
			iRet = sendData(LCM_CHIP_SELECT_0, CmdToSend, 13);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
		for(AddrId = 0; AddrId < AddrCnt; AddrId++)
		{
			CmdToSend[0] = 0xA0;
			CmdToSend[0] |= (AddrId >> 1);
			CmdToSend[1] = (AddrId << 7);
			CmdToSend[1] |= (0x0F << 3);
			iRet = sendData(LCM_CHIP_SELECT_1, CmdToSend, 13);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

/*added by lxj_com 2012-7-30 for LED char table index by ASCII*/
#define MAX_CHAR_TABLES 128
static const unsigned int charTables[MAX_CHAR_TABLES] = {
	/*the LED's segment totals = 15
	  segment1-4:  E,N,G,F    in 0-3   bit
	  segment5-8:  D,M,I,H    in 8-11  bit
	  segment9-12: L,K,J,A    in 16-19 bit
	  segment13-15: DOT,C,B   in 24-26 bit */
			
	/*control char*/
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,

	/*0-9,A-Z,a-z*/
	0x00000000/*SP*/,0x00000000/*!*/,0x00000C00/*"*/,0x00000000/*#*/,0x00000000/*$*/,0x00000000/*%*/,0x00000000/*&*/,0x00000400/*'*/,
	0x00000000/*(*/ ,0x00000000/*)*/,0x00070806/***/,0x00020604/*+*/,0x00000002/*,*/,0x00020004/*-*/,0x01000000/*.*/,0x00040002/*/*/,
	0x06080109/*0*/ ,0x00000009/*1*/,0x040A0105/*2*/,0x060A0104/*3*/,0x0602000C/*4*/,0x020A010C/*5*/,0x020A010D/*6*/,0x06080000/*7*/,
	0x060A010D/*8*/ ,0x060A010C/*9*/,0x00000600/*:*/,0x00000000/*;*/,0x00000000/*<*/,0x00000000/*=*/,0x00000000/*>*/,0x00000000/*?*/,
	0x00000000/*@*/ ,0x060A000D/*A*/,0x060A010D/*B*/,0x00080109/*C*/,0x06080700/*D*/,0x000A010D/*E*/,0x000A000D/*F*/,0x020A0109/*G*/,
	0x0602000D/*H*/ ,0x00080700/*I*/,0x00090500/*J*/,0x0005000D/*K*/,0x00000109/*L*/,0x06040A09/*M*/,0x06010809/*N*/,0x06080109/*O*/,
	0x040A000D/*P*/ ,0x06090109/*Q*/,0x040B000D/*R*/,0x020A010C/*S*/,0x00080600/*T*/,0x06000109/*U*/,0x0004000B/*V*/,0x0601040B/*W*/,
	0x00050802/*X*/ ,0x00040A00/*Y*/,0x000C0102/*Z*/,0x00080109/*[*/,0x00010800/*\*/,0x06080100/*]*/,0x00000000/*^*/,0x00000100/*_*/,
	0x00000000/*`*/ ,0x00000000/*a*/,0x0202010d/*b*/,0x00020105/*c*/,0x06020105/*d*/,0x00000000/*e*/,0x00000000/*f*/,0x00000000/*g*/,
	0x0202000D/*h*/ ,0x00000000/*i*/,0x00000000/*j*/,0x00050600/*k*/,0x00000000/*l*/,0x02020205/*m*/,0x02020005/*n*/,0x02020105/*o*/,
	0x00000000/*p*/ ,0x00000000/*q*/,0x00000005/*r*/,0x00000000/*s*/,0x00000000/*t*/,0x00000000/*u*/,0x00000000/*v*/,0x00000000/*w*/,
	0x00050802/*x*/ ,0x00000000/*y*/,0x00000000/*z*/,0x00000000/*{*/,0x00000600/*|*/,0x00000000/*}*/,0x00000000/*~*/,0x00000000/* */,
};

INT_t CCnkB85Lcm::DisplayChar(CONST INT_t CharPos, CONST char CharToDisplay, CONST BOOL_t bWithDot/* = FALSE*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	INT_t ChipSelId = LCM_CHIP_SELECT_0;
	INT_t StartAddrId;
	BYTE DataAtAddrId0 = 0, DataAtAddrId1 = 0, DataAtAddrId2 = 0, DataAtAddrId3 = 0;
	BYTE CmdToSend[2];

	do
	{
		//LOG_LINE("CharPos=%d,DisplayChar=%c,bWithDot=%d\n",CharPos,CharToDisplay,bWithDot);
		if(CharPos <= 0 || CharPos > (LCM_CHIP_CHAR_COUNT * LCM_CHIP_COUNT))
		{
			iOutRet = ERROR_OUT_OF_RANGE;
			break;
		}

		ChipSelId = (CharPos <= LCM_CHIP_CHAR_COUNT)? LCM_CHIP_SELECT_0 : LCM_CHIP_SELECT_1;
		StartAddrId = (4 * ((CharPos - 1) % LCM_CHIP_CHAR_COUNT));
		CmdToSend[0] = 0xA0;

#if 1/*modify by lxj 2012-7-30, search by ASCII*/
		if( (unsigned char)(CharToDisplay) < MAX_CHAR_TABLES )
		{
			DataAtAddrId0 = (charTables[CharToDisplay] >> 0) & 0xff;  /*segment1-4:  E,N,G,F  */
			DataAtAddrId1 = (charTables[CharToDisplay] >> 8) & 0xff;  /*segment5-8:  D,M,I,H  */
			DataAtAddrId2 = (charTables[CharToDisplay] >> 16) & 0xff; /*segment9-12: L,K,J,A  */
			DataAtAddrId3 = (charTables[CharToDisplay] >> 24) & 0xff; /*segment13-15: DOT,C,B */
		}
#else
		if('.' == CharToDisplay)
		{
			DataAtAddrId3 |= 0x01;
		}
		else if('_' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x00;
		}
		else if('+' == CharToDisplay)
		{
			DataAtAddrId0 = 0x04;
			DataAtAddrId1 = 0x06;
			DataAtAddrId2 = 0x02;
			DataAtAddrId3 = 0x00;
		}
		else if('-' == CharToDisplay)
		{
			DataAtAddrId0 = 0x04;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x02;
			DataAtAddrId3 = 0x00;
		}
		else if('*' == CharToDisplay)
		{
			DataAtAddrId0 = 0x06;
			DataAtAddrId1 = 0x08;
			DataAtAddrId2 = 0x07;
			DataAtAddrId3 = 0x00;
		}
		else if('/' == CharToDisplay)
		{
			DataAtAddrId0 = 0x02;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x04;
			DataAtAddrId3 = 0x00;
		}
		else if(' ' == CharToDisplay)
		{
		}
		else if('A' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x06;
		}
		else if('B' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x06;
		}
		else if('C' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x00;
		}
		else if('D' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x07;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x06;
		}
		else if('E' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0d;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x00;
		}
		else if('e' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0d;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x04;
		}
		else if('F' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0d;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x00;
		}
		else if('G' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x02;
		}
		else if('H' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0d;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x02;
			DataAtAddrId3 = 0x06;
		}
		else if('I' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x07;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x00;
		}
		else if('i' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x06;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x00;
		}
		else if('J' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x05;
			DataAtAddrId2 = 0x09;
			DataAtAddrId3 = 0x00;
		}
		else if('K' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x05;
			DataAtAddrId3 = 0x00;
		}
		else if('L' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x00;
		}
		else if('M' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x0A;
			DataAtAddrId2 = 0x04;
			DataAtAddrId3 = 0x06;
		}
		else if('N' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x08;
			DataAtAddrId2 = 0x01;
			DataAtAddrId3 = 0x06;
		}
		else if('O' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x06;
		}
		else if('P' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x04;
		}
		else if('Q' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x09;
			DataAtAddrId3 = 0x06;
		}
		else if('R' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0d;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x0b;
			DataAtAddrId3 = 0x04;
		}
		else if('r' == CharToDisplay)
		{
			DataAtAddrId0 = 0x05;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x00;
		}
		else if('S' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0c;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x02;
		}
		else if('T' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x06;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x00;
		}
		else if('U' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x06;
		}
		else if('V' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0b;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x04;
			DataAtAddrId3 = 0x00;
		}
		else if('W' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0b;
			DataAtAddrId1 = 0x04;
			DataAtAddrId2 = 0x01;
			DataAtAddrId3 = 0x06;
		}
		else if('X' == CharToDisplay)
		{
			DataAtAddrId0 = 0x02;
			DataAtAddrId1 = 0x08;
			DataAtAddrId2 = 0x05;
			DataAtAddrId3 = 0x00;
		}
		else if('Y' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x0A;
			DataAtAddrId2 = 0x04;
			DataAtAddrId3 = 0x00;
		}
		else if('Z' == CharToDisplay)
		{
			DataAtAddrId0 = 0x02;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0C;
			DataAtAddrId3 = 0x00;
		}
		else if('0' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x06;
		}
		else if('1' == CharToDisplay)
		{
			DataAtAddrId0 = 0x09;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x00;
			DataAtAddrId3 = 0x00;
		}
		else if('2' == CharToDisplay)
		{
			DataAtAddrId0 = 0x05;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x04;
		}
		else if('3' == CharToDisplay)
		{
			DataAtAddrId0 = 0x04;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x06;
		}
		else if('4' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0C;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x02;
			DataAtAddrId3 = 0x06;
		}
		else if('5' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0c;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x02;
		}
		else if('6' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0a;
			DataAtAddrId3 = 0x02;
		}
		else if('7' == CharToDisplay)
		{
			DataAtAddrId0 = 0x00;
			DataAtAddrId1 = 0x00;
			DataAtAddrId2 = 0x08;
			DataAtAddrId3 = 0x06;
		}
		else if('8' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0D;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x06;
		}
		else if('9' == CharToDisplay)
		{
			DataAtAddrId0 = 0x0C;
			DataAtAddrId1 = 0x01;
			DataAtAddrId2 = 0x0A;
			DataAtAddrId3 = 0x06;
		}
#endif
		else
		{
			iOutRet = ERROR_NOT_SUPPORTED;
			break;
		}

		if(bWithDot)
		{
			DataAtAddrId3 |= 0x01;
		}

		CmdToSend[0] |= (StartAddrId >> 1);
		CmdToSend[1] = (StartAddrId << 7);
		CmdToSend[1] |= (DataAtAddrId0 << 3);
		iRet = sendData(ChipSelId, CmdToSend, 13);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		CmdToSend[0] |= ((StartAddrId+1) >> 1);
		CmdToSend[1] = ((StartAddrId+1) << 7);
		CmdToSend[1] |= (DataAtAddrId1 << 3);
		iRet = sendData(ChipSelId, CmdToSend, 13);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		CmdToSend[0] |= ((StartAddrId+2) >> 1);
		CmdToSend[1] = ((StartAddrId+2) << 7);
		CmdToSend[1] |= (DataAtAddrId2 << 3);
		iRet = sendData(ChipSelId, CmdToSend, 13);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		CmdToSend[0] |= ((StartAddrId+3) >> 1);
		CmdToSend[1] = ((StartAddrId+3) << 7);
		CmdToSend[1] |= (DataAtAddrId3 << 3);
		iRet = sendData(ChipSelId, CmdToSend, 13);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

	}while(FALSE);

	return iOutRet;
}

INT_t CCnkB85Lcm::DisplayStr(LPCSTR pszStrToDisp, CONST INT_t StartPos/* = 1*/)
{
	INT_t iOutRet = ERROR_SUCCESS, iId;
	size_t StrLen;
	INT_t CurCharPos;

	do
	{
		if(NULL == pszStrToDisp)
		{
			break;
		}
		if('\0' == pszStrToDisp[0])
		{
			break;
		}

		StrLen = strlen(pszStrToDisp);
		CurCharPos = StartPos;
		for(iId = 0; iId < (typeof(iId))StrLen; iId++)
		{
			BOOL_t bWithDot = FALSE;
			if(iId < (typeof(iId))StrLen - 1)	//not the last char
			{
				if('.' == pszStrToDisp[iId+1])
				{
					bWithDot = TRUE;
				}
				else
				{
				}
			}
			else
			{
			}

#if 1
			iOutRet = DisplayChar(CurCharPos, pszStrToDisp[iId], bWithDot);
			if(ERROR_SUCCESS != iOutRet)
			{
				break;
			}
#else/* added by lxj 2012-7-30 for test all char tables */
			for(unsigned char i=' ';i<128;i++)
			{
				iOutRet = DisplayChar(1, i, 0);
				if(ERROR_SUCCESS != iOutRet)
				{
					break;
				}
				sleep(3);
			}
#endif

			if(bWithDot)
			{
				iId++;
			}
			CurCharPos++;
		}
	}while(FALSE);

	return iOutRet;
}


