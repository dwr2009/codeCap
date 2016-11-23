#ifndef	_SMART_CARD_H_
#define	_SMART_CARD_H_

#include <BaseTypeDef.h>
#include <CCplusplusBridge.h>

DECLARE_EXTERN_C_BLOCK_START

typedef	struct _SMART_CARD_INST SMART_CARD_INST;
typedef	SMART_CARD_INST * P_SMART_CARD_INST;

typedef struct
{
	BOOL_t bSCardInstId_Valid;
	int iSCardInstId;
}SCARD_SEL_MATCH_PARAM, *P_SCARD_SEL_MATCH_PARAM;	

INT SmartCard_New(P_SMART_CARD_INST * ppSmartCardInst);
INT SmartCard_Open(P_SMART_CARD_INST pSmartCardInst,
	CONST P_SCARD_SEL_MATCH_PARAM pSCardSelMatchParam);
INT SmartCard_PingPong(P_SMART_CARD_INST pSmartCardInst);
INT SmartCard_ResetCard(P_SMART_CARD_INST pSmartCardInst);
INT SmartCard_Write(P_SMART_CARD_INST pSmartCardInst, PBYTE pData, size_t sizeData);
INT SmartCard_Read(P_SMART_CARD_INST pSmartCardInst, PBYTE pDataBuf, size_t sizeDataBuf,
	size_t * psizeRead);
INT SmartCard_WaitData(P_SMART_CARD_INST pSmartCardInst, INT iTimeoutMs);
INT SmartCard_ClearReadBuf(P_SMART_CARD_INST pSmartCardInst);
INT SmartCard_Close(P_SMART_CARD_INST pSmartCardInst);
INT SmartCard_Del(P_SMART_CARD_INST pSmartCardInst);

DECLARE_EXTERN_C_BLOCK_END

#endif	//_SMART_CARD_H_

