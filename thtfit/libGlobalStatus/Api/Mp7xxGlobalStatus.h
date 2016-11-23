#ifndef	_MP7XX_GLOBAL_STATUS_H_
#define	_MP7XX_GLOBAL_STATUS_H_

#ifdef	__cplusplus	//C++

#include "GlobalStatus.h"

/*added by lxj 2012-8-2 for Device Key Bit Mask,
    this copy from tsi_agent_new/Tnetapp.h */
#if 1
#define DK_DEFAULT_ENABLED  DK_ALL_ENABLED
typedef enum{
	DK_ALL_ENABLED          = 0xffffffff,/*Initial*/

    DK_ADOBE_FLASH_ENALBED  = 0x20000,
    DK_WMA_ENABLED          = 0x10000,

    DK_MEDIA_ENABLE			= 0x8000,/*no nead for MP700*/
    DK_MEDIA_OVERRIDE_0		= 0x4000,/*no need for MP700*/
    DK_MEDIA_OVERRIDE_1		= 0x0100,/*no need for MP700*/
    DK_MEDIA_OVERRIDE		= 0x4100,/*no need for MP700*/
    DK_RESERVED_1			= 0x2000,
    DK_BROWSER_ENABLE		= 0x1000,/*no need for MP700*/
    DK_RESERVED_2			= 0x0800,
    DK_PLAYER_ENABLE		= 0x0400,
    DK_RESERVED_3			= 0x0200,
    DK_RESERVED_4			= 0x0080,
    DK_RESERVED_5			= 0x0040,
    DK_CMDR_ENABLE			= 0x0020,/*not used*/
    DK_SCMOD_ENABLED		= 0x0010,
    DK_DTS_ENABLE			= 0x0008,
    DK_DOLBY_ENABLE         = 0x0004,
    DK_VIRTUAL_CHAN_ENABLE	= 0x0002,
    DK_ALPHA_SET			= 0x0001,
}eDeviceKeyBitMask;
#endif

//Playing status, only for MediaSrv not indicates actual playing state
#define PLAY_STAT_UNKNOWN               0
#define	PLAY_STAT_IDLE					1
#define	PLAY_STAT_PREPARED				2
#define	PLAY_STAT_PLAYING				3
#define PLAY_STAT_TUNER_SCANNING        4

class CMp7xxGlobalStatus : public CGlobalStatus
{
public:
	typedef struct
	{
		BOOL_t bAutoGetIP;
		UINT32_t uiIpV4Addr;
		UINT32_t uiSubnetMaskV4;
	}CUR_ETH_SETTINGS, *P_CUR_ETH_SETTINGS;	

	struct tagSECOND_OSD_INFO {
		struct tagUSING_CNT {
			pid_t processId;
			INT_t refCnt;
		};
		typedef struct tagUSING_CNT USING_CNT;
		
		UINT32_t surfaceAddr;
		UINT32_t surfaceLumaAddr;
		UINT32_t surfaceLumaSize;
		UINT32_t surfaceWidth;
		UINT32_t surfaceHeight;
		USING_CNT usingCnt[8];
	};
	typedef struct tagSECOND_OSD_INFO SECOND_OSD_INFO;

	typedef struct
	{
		size_t sizeOfThisStruct;
		CUR_ETH_SETTINGS CurEthSettings;
		BOOL_t bEnableCeLabsLogging;
		BOOL_t bUsbIsMounted;
		BOOL_t bSdIsMounted;
		BOOL_t bHddIsMounted;
		BOOL_t bWritingFs;
		time_t tmLastTimeToWriteFs;
		BOOL_t bLedIndicator_HwInited;
		INT_t iLedBusyCounter;
#if 1/*added by lxj 2012-8-2 for enabled features*/
        UINT32_t uiEnabledFeatures;
#endif
#if 1/*added by lxj 2012-9-29 for irControl*/
		INT_t bIrControlEnabled;
#endif
#if 1/*added by lxj 2012-11-20*/
		INT_t iPlayStatus;
#endif
		UINT32_t curGuiFbSurfaceAddr;
		SECOND_OSD_INFO secondOsdInfo;
	}GLOBAL_STATUS_INFO, *P_MP7XX_GLOBAL_STATUS_INFO;
public:
	CMp7xxGlobalStatus();
	virtual ~CMp7xxGlobalStatus();
	virtual INT_t Lock();
	virtual INT_t Unlock();
	virtual CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO getStatusInfo(CONST BOOL_t bNeedLock = TRUE);
	virtual INT_t setCurEthSettings(BOOL_t bAutoGetIP, UINT32_t uiIpV4Addr, UINT32_t uiSubnetMaskV4);
	virtual INT_t getCurEthSettings(OUT BOOL_t & bAutoGetIP, OUT UINT32_t & uiIpV4Addr, OUT UINT32_t & uiSubnetMaskV4);
	virtual INT_t setEnableLogging(BOOL_t bEnableCeLabsLogging);
	virtual INT_t getEnableLogging(OUT BOOL_t & bEnableCeLabsLogging);
	virtual INT_t set_UsbIsMounted(CONST BOOL_t bMounted);
	virtual INT_t get_UsbIsMounted(OUT BOOL_t & bMounted);
	virtual INT_t set_SdIsMounted(CONST BOOL_t bMounted);
	virtual INT_t get_SdIsMounted(OUT BOOL_t & bMounted);
	virtual INT_t set_HddIsMounted(CONST BOOL_t bMounted);
	virtual INT_t get_HddIsMounted(OUT BOOL_t & bMounted);
	virtual INT_t get_WritingFs_Info(BOOL_t * CONST pbWritingFs, time_t * CONST ptmLastTimeToWriteFs);
	virtual INT_t set_WritingFs_Info(CONST BOOL_t bWritingFs, CONST time_t tmLastTimeToWriteFs);
	virtual INT_t TestAndSet_LedIndicatorHwInited(CONST BOOL_t bInited, CONST BOOL_t bNewVal, BOOL_t * CONST pbOldVal);
	virtual INT_t set_LedBusyCounter(CONST INT_t iIncrement, OUT INT_t & iCurVal);
#if 1/*added by lxj 2012-8-2 for enabled features*/
    virtual INT_t setEnabledFeatures(CONST UINT32_t uiEnabledFeatures);
	virtual INT_t getEnabledFeatures(OUT int &uiEnabledFeatures);
    virtual INT_t getEnablePlayer(OUT BOOL_t &bEnablePlayer);
    virtual INT_t getEnableDolby(OUT BOOL_t &bEnableDolby);
    virtual INT_t getEnableWMA(OUT BOOL_t &bEnableWMA);
    virtual INT_t getEnableAdobeFlash(OUT BOOL_t &bEnableAdobeFlash);
#endif
#if 1/*added by lxj 2012-9-29 for irControl*/
	virtual INT_t setIrControlEnabled(CONST INT_t bEnabled);
	virtual INT_t getIrControlEnabled(OUT INT_t &bEnabled);
#endif
#if 1/*added by lxj 2012-11-20*/
	virtual INT_t setMpSrvPlayStatus(CONST INT_t iPlayStatus);
	virtual INT_t getMpSrvPlayStatus(INT_t * CONST pPlayStatus);
#endif
	virtual INT_t setCurGuiFbSurfaceAddr(CONST UINT32_t curGuiFbSurfaceAddr);
	virtual INT_t getCurGuiFbSurfaceAddr(OUT UINT32_t & curGuiFbSurfaceAddr);
protected:

};

INT_t Mp7xxGlobalStatus_Lock();
INT_t Mp7xxGlobalStatus_Unlock();
CMp7xxGlobalStatus::P_MP7XX_GLOBAL_STATUS_INFO Mp7xxGlobalStatus_getStatusInfo(CONST BOOL_t bNeedLock = TRUE);

#endif	//__cplusplus

INT_t get_UsbIsMounted(BOOL_t * CONST pbMounted);
INT_t set_UsbIsMounted(CONST BOOL_t bMounted);
INT_t get_SdIsMounted(BOOL_t * CONST pbMounted);
INT_t set_SdIsMounted(CONST BOOL_t bMounted);
INT_t get_HddIsMounted(BOOL_t * CONST pbMounted);
INT_t set_HddIsMounted(CONST BOOL_t bMounted);
INT_t TestAndSet_LedIndicatorHwInited(CONST BOOL_t bInited, CONST BOOL_t bNewVal, BOOL_t * CONST pbOldVal);
INT_t set_LedBusyCounter(CONST INT_t iIncrement, INT_t * CONST piCurVal);

#if 1/*added by lxj 2012-8-2 for enabled features, export by function*/
INT_t getEnablePlayer(BOOL_t * CONST bEnablePlayer);
INT_t getEnableDolby(BOOL_t * CONST bEnableDolby);
INT_t getEnableWMA(BOOL_t * CONST bEnableWMA);
INT_t getEnableAdobeFlash(BOOL_t * CONST bEnableAdobeFlash);
#endif

#if 1/*added by lxj 2012-9-29 for irControl*/
INT_t setIrControlEnabled(CONST INT_t bEnabled);
INT_t getIrControlEnabled(INT_t * CONST bEnabled);
#endif

#if 1/*added by lxj 2012-11-11*/
INT_t setMpSrvPlayStatus(CONST INT_t iPlayStatus);
INT_t getMpSrvPlayStatus(INT_t * CONST pPlayStatus);
#endif

INT_t setCurGuiFbSurfaceAddr(CONST UINT32_t curGuiFbSurfaceAddr);
INT_t getCurGuiFbSurfaceAddr(OUT UINT32_t & curGuiFbSurfaceAddr);

#endif	//_MP7XX_GLOBAL_STATUS_H_

