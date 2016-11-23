#ifndef	_TUNER_SRV_H_
#define	_TUNER_SRV_H_

#include <CBaseTunerIface.h>
#include <String.h>
#include <Mutex2.h>
#include <DtvType.h>
#include "GenericTimer.h"
#include <EventEx.h>
#include <Mp7xxGlobalStatus.h>
#include <ErrChkMutex.h>
#include <nexus_i2c.h>
#include <SharedPtr.h>
#include "media_player.h"


#define PROGRAM_TABLE_FILE	"/data/Program.data"
#define MAX_NUMBER_OF_CHANNELS 100
#define MAJOR_NO_VCT_DEFAULT	800

#define	SysProp_KEY_TunerStdMode		"TunerStdMode"
#define	SysProp_KEY_TunerScanStatus		"TunerCurrScanStatus"
#define	SysProp_KEY_TunerChannelMode	"TunerChannelMode"

typedef void (*PFN_TuScanProgressCallback)(void* pContext,OUT UINT16_t *pPgmNum,OUT UINT16_t *pProgress);

typedef struct
{
	PFN_TuScanProgressCallback TuScanProgressCallback;
	void *context;
}TunerScanProgressCk,*pTunerScanProgressCk;

typedef struct{
	UINT32_t MajorlNum;
	UINT32_t MinorlNum;
}TunerUrlInfo,*pTunerUrlInfo;


class CTunerSrv
{
public:
	CTunerSrv(media_player_t MediaPlayer);
	 virtual ~CTunerSrv();
	 INT_t InitInstance();
	 virtual INT_t OpenTuner();
	 virtual INT_t CloseTuner();
	 virtual INT_t OnScanProgram();
	 virtual INT_t OnSetUrl(LPCSTR pszChStrIndex,bool *pbFoundInChTab);
	 virtual INT_t OnChannelUp();
	 virtual INT_t OnChannelDown();
	 virtual INT_t OnPsfSetPmt();
	 virtual INT_t OnSetChannelInfo();
	 virtual INT_t OnClosePsfDemux();
	 virtual INT_t OnEventTuneProgram();
	 virtual BOOL_t OnCheckInit();
	 virtual INT_t OnGetStrength(OUT UINT32_t *pStrength);
	 virtual INT_t OnSetScanProgressCb(pTunerScanProgressCk pCbTuScanProgress);
	 virtual INT_t OnWaitLock(UINT32_t *pLock);
	 virtual INT_t OnReInitTunerModules();
	 virtual INT_t OnReloadProgram();
	 virtual INT_t OnGetChannelInfo(INT32_t iEntryId,INT32_t *pChannelNum,INT32_t *pNumofSubChannel,char *pStrPids);
	 virtual INT_t OnGetChannelNumber(OUT UINT32_t *pMajor,OUT UINT32_t *pMinor);
	 virtual INT_t OnSetTunerMode(INT32_t *pStdMode);
	 virtual INT_t OnGetTunerMode(INT32_t *pChStdMode);
	 virtual INT_t OnGetCurrStdMode(INT32_t *pStdMode);
	 virtual INT_t OnSetTunerChannelMode(INT32_t *pInChMode);
	 virtual INT_t OnGetTunerChannelMode(INT32_t *pOutChMode);
	 virtual void  SetMediaStartSetting(media_player_start_settings* mediaPlayStartSet);
	INT_t FindChannelIndexInFixedTable(LPCSTR pChStrIndex,bool *pFound);
private:
	INT_t ParseChannelInfoUrl(LPCSTR pStrChannelUrl);
	INT_t LoadProgramTable();
	INT_t SaveProgramTable();
	pProgramInfo ProgramInfoNew(UINT16_t numProgram);
	INT_t ProgramInfoDel(pProgramInfo pPGInfo);
	INT_t FillProgramInfo(UINT16_t numProgram,UINT32_t offset,tspsimgr_program* pPGInfo);
	INT_t RestoreProgramInfo();
	INT_t FindVirtualChannelNumInProgramTable(pTunerUrlInfo pUrlInfo,bool *pbFound);
	INT_t FillChannelInfo(tspsimgr_program* pPgmInfo,INT32_t freq,BandwidthChannel typeBW,StandardChannle typeStandard,UINT32_t Index,UINT16_t numProgram,UINT32_t Constellation);
	INT_t SetChannelStandMode(INT32_t *pIStdModeType);
	INT_t SysPropChannelParaValid();
	void DumpProgInfo(ProgramInfo* pProg);
	void DumpChannelInfo();
	
private:
	TunerUrlInfo m_TunerUrlInfo;
	SharedPtr<CBaseTunerIface> m_TunerIface_sp;
	BOOL_t m_bDtvChannelLoadSucc;
	ChannelProgramInfo m_ChProgInfo[MAX_NUMBER_OF_CHANNELS];
	UINT32_t m_TotalChannel;
	UINT32_t m_currChIdx;
	UINT32_t m_currPgIdx;
	DtvChannelCfg m_DtvProgramCfg;
	UINT32_t m_currProgramIndex;
	BOOL_t m_bNeedSetTunerMode;
	UINT32_t	m_DefaultMajorNo;
	INT32_t	m_FixedFreqKHz;
	BOOL_t m_StopTunerFlag;
	NEXUS_ParserBand m_parserBand;
	
protected:
	INT_t m_iTuPlayStatus;
	CMp7xxGlobalStatus m_Mp7xxGlobalStatusData;
	CEventEx m_PlayStatChangedEvent;
	BOOL_t m_bInited;
	CString m_strChIndexUrl;
	CMutex2 m_SharedDataLock;
	pTunerScanProgressCk m_pCbScanProgress;
	media_player_t m_MediaPlayer;
	media_player_start_settings m_playsetting;
};

#endif	//_TUNER_SRV_H_

