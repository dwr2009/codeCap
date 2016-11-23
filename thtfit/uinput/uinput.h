#ifndef _UINPUT_H_
#define _UINPUT_H_

#define NEXUS_PLATFORM 97445
#define NEXUS_PLATFORM_VERSION_MAJOR 13  
#define NEXUS_PLATFORM_VERSION_MINOR  4

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "Api/UinputProcessExitCode.h"
#include <ThreadData.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <GenericTimer.h>
#include <EventEx.h>
#include <Mutex2.h>
#include <SharedPtr.h>
#include <GenericTimer.h>
#include <ErrPrintHelper.h>
#include "DbgLogSwitchDef.h"
#include <BaseErrDef.h>
#include <LedStatusIndicator.h>
#include <TimerListenerInterface.h>
#include <dirent2.h>
#include <map>
#include <sys/time.h>
#include <Mp7xxGlobalStatus.h>
#include <TiXmlDoc2.h>
#include <AutoLock.h>
#include "nxclient.h"
#include "nexus_ir_input.h"
#include "nexus_input_client.h"
#include <DateTime.h>

#define ALWAYS_ENABLED_KEY_MAX       		50
#define ALWAYS_ENABLED_REPEAT_KEY_MAX   	30
#define ALWAYS_ENABLED_KEY_MENU         	KEY_F1
#define ALWAYS_ENABLED_KEY_OK           	KEY_ENTER
#define ALWAYS_ENABLED_KEY_UP           	KEY_UP
#define ALWAYS_ENABLED_KEY_DOWN         	KEY_DOWN
#define ALWAYS_ENABLED_KEY_LEFT         	KEY_LEFT
#define ALWAYS_ENABLED_KEY_RIGHT        	KEY_RIGHT
unsigned int always_enabled_key_count=0;
typedef unsigned int tUnDisableKey;
static  tUnDisableKey 						always_enabled_key_table[ALWAYS_ENABLED_KEY_MAX] = {0};
static  tUnDisableKey 						always_enabled_repeat_key_table[ALWAYS_ENABLED_REPEAT_KEY_MAX]={0};
static 	int 								repeatkeycount=0;

#define MAX_REPEAT_CNT		5
#define UINPUT_MAX_NAME_SIZE	80



#define	UINPUT_XML_CFG_FILE				"/MP7XX/Config/Uinput.xml"
using namespace std;
using namespace CppBase;

typedef struct
{
	unsigned int remote_code;						/* Remote code */
	unsigned int key_code;							/* Mapped key code */
	unsigned int repeat;							/* Repeatable? */
	const char * desc;								/* Description */
} IR_KEY;

typedef struct
{
	unsigned int remote_code;						/* Remote code */
	int type;										/* 0: rel_pos, 1: key */
	union {
		short rel_pos[2];							/* Mapped X and Y offset */
		unsigned int key_code;						/* Key code if any */
	};
	const char * desc;								/* Description */
} IR_POINTER;

typedef struct
{
	unsigned int fip_code;							/* Front panel code */
	unsigned int key_code;							/* Mapped key code */
	const char * desc;								/* Description */
} FIP_KEY;

/* Key table for reference NEC remote */

static IR_KEY nec_key_table[] = {
	//MP7XX
	{ 0x22ddff00, KEY_MUTE, 	0, "MUTE" },		//KEY_MUTE
	{ 0x29d6ff00, KEY_FASTFORWARD, 0, "FFWD" },
	{ 0x2dd2ff00, KEY_DOWN,		0, "DOWN" },
	{ 0x31ceff00, KEY_ENTER, 	0, "ENTER" },
	{ 0x33ccff00, KEY_3,		0, "3" },
	{ 0x35caff00, KEY_UP,		0, "UP" },
	{ 0x37c8ff00, KEY_6,		0, "6" },
	{ 0x3ac5ff00, KEY_F10,		0, "F10"},      	//USB video switch, add by lxj 2012-10-8
	{ 0x3bc4ff00, KEY_9,		0, "9" },
	{ 0x3cc3ff00, KEY_F7, 0, "PAUSE" },     		//PAUSE, add by lxj 2012-7-24
	{ 0x3ec1ff00, KEY_RIGHT, 	0, "RIGHT" },
	{ 0x609fff00, KEY_MODE, 0, "TV GUIDE" },			//TV MODE
	{ 0x629dff00, KEY_ESC,		0, "ESC" },			//Esc
	{ 0x639cff00, KEY_POWER,	0, "POWER"},    /* POWER */
	{ 0x6699ff00, KEY_LEFT, 	0, "LEFT" },
	{ 0x6a95ff00, KEY_F1, 		0, "MENU" },			//MENU
	{ 0x6c93ff00, KEY_2,		0, "2" },
	{ 0x6d92ff00, KEY_1, 		0, "1" },
	{ 0x708fff00, KEY_5,		0, "5" },
	{ 0x718eff00, KEY_4,		0, "4" },
	{ 0x728dff00, KEY_F3,		0, "STOP" },			//STOP
	{ 0x748bff00, KEY_8,		0, "8" },
	{ 0x758aff00, KEY_7,		0, "7" },
	{ 0x7788ff00, KEY_BACKSPACE,0, "BACKSPACE" },	//Delete
	{ 0x7887ff00, KEY_0,		0, "0" },
	{ 0x7a85ff00, KEY_F8, 0, "CH+" },      //CH+
	{ 0x7986ff00, KEY_F9, 0, "CH-" },      	//CH-
	{ 0x7c83ff00, KEY_F2,		0, "PLAY" },			//PLAY
    { 0x7d82ff00, KEY_DOT,		0, "DOT" },			//Dot, "."
	{ 0x7e81ff00, KEY_F5, 1, "VOLUME-" },			//VOLUME-
	{ 0x7f80ff00, KEY_F4,		1, "VOLUME+" },			//VOLUME+
    { 0x25daff00, KEY_REWIND, 0, "REW" },
    { 0x26d9ff00, KEY_VIDEO, 0, "VOD"},
    { 0x2fd0ff00, KEY_SETUP, 0, "SETTINGS"},
    { 0x32cdff00, KEY_RECORD, 0, "REC"},
    { 0x619eff00, KEY_CHANNEL,0, "CHANNELS"},
    { 0x6996ff00, KEY_PC, 0, "PC"},
    { 0x6e91ff00, KEY_F14, 0, "SPEED"},
    { 0x7689ff00, KEY_FAVORITES, 0, "FAV"},
    { 0x7b84ff00, KEY_F13, 0, "QUARTO"},

	/* Sigma Designs' DVD remote */
	{ 0xbc43e608, KEY_POWER, 	0, "POWER" },
	{ 0xb847e608, KEY_EJECTCLOSECD, 0, "EJECT" },
	{ 0xbd42e608, KEY_1, 		0, "1" },
	{ 0xbe41e608, KEY_2, 		0, "2" },
	{ 0xbf40e608, KEY_3, 		0, "3" },
	{ 0xb946e608, KEY_4, 		0, "4" },
	{ 0xba45e608, KEY_5, 		0, "5" },
	{ 0xbb44e608, KEY_6, 		0, "6" },
	{ 0xb54ae608, KEY_7, 		0, "7" },
	{ 0xb649e608, KEY_8, 		0, "8" },
	{ 0xb748e608, KEY_9, 		0, "9" },
	{ 0xb24de608, KEY_0,		0, "0" },
	{ 0xb14ee608, KEY_CLEAR, 	0, "CLEAR" },
	{ 0xb34ce608, KEY_SELECT, 	0, "SELECT" },
	{ 0xb44be608, KEY_MODE,		0, "TV/MODE" },
	{ 0xe31ce608, KEY_DISPLAYTOGGLE,0, "VIEW/DISPLAY" },
	{ 0xe51ae608, KEY_SETUP,	0, "SETUP" },
	{ 0xe619e608, KEY_MUTE,		0, "MUTE" },
	{ 0xef10e608, KEY_VOLUMEDOWN,	1, "VOL DOWN" },
	{ 0xe718e608, KEY_VOLUMEUP,	1, "VOL UP" },
	{ 0xeb14e608, KEY_UP,		1, "UP" },
	{ 0xe21de608, KEY_LEFT,		1, "LEFT" },
	{ 0xee11e608, KEY_RIGHT,	1, "RIGHT" },
	{ 0xe916e608, KEY_DOWN,		1, "DOWN" },
	{ 0xea15e608, KEY_ENTER,	0, "ENTER" },
	{ 0xe11ee608, KEY_RESTART,	0, "RETURN" },
	{ 0xe01fe608, KEY_AB,		0, "A-B" },
	{ 0xe41be608, KEY_PREVIOUSSONG,	1, "PREV" },
	{ 0xec13e608, KEY_NEXTSONG,	1, "NEXT" },
	{ 0xa35ce608, KEY_REDO, 	0, "REPEAT" },
	{ 0xa758e608, KEY_BACK,		1, "REV" },
	{ 0xab54e608, KEY_STOPCD,	0, "STOP" },
	{ 0xaf50e608, KEY_FORWARD,	1, "FWD" },
	{ 0xa25de608, KEY_INFO,		0, "INFO" },
	{ 0xa659e608, KEY_PREVIOUS,	1, "SLOW REV" },
	{ 0xaa55e608, KEY_PAUSECD,	0, "PAUSE/STEP" },
	{ 0xae51e608, KEY_NEXT,		1, "SLOW FWD" },
	{ 0xa15ee608, KEY_EDIT,		0, "EDIT/PROGRAM" },
	{ 0xa55ae608, KEY_TITLE, 	0, "TITLE" },
	{ 0xa956e608, KEY_SUBTITLE,	0, "SUBTITLE" },
	{ 0xad52e608, KEY_AUDIO,	0, "AUDIO" },
	{ 0xa05fe608, KEY_SHUFFLE,	0, "SHUFFLE" },
	{ 0xa45be608, KEY_ANGLE,	0, "ANGLE/ROTATE" },
	{ 0xa857e608, KEY_SEARCH,	0, "SEARCH" },
	{ 0xac53e608, KEY_ZOOM,		0, "ZOOM" },
	/* Popcorn remote */
	{ 0xb54acb04, KEY_POWER, 	0, "POWER" },
	{ 0xb748cb04, KEY_FILE,		0, "FILE MODE" },
	{ 0xf00fcb04, KEY_SETUP,	0, "SETUP" },
	{ 0xb24dcb04, KEY_SUSPEND, 	0, "SUSPEND" },
	{ 0xb14ecb04, KEY_PAGEUP,	0, "PAGE UP" },
	{ 0xbb44cb04, KEY_EJECTCLOSECD,	0, "EJECT" },
	{ 0xec13cb04, KEY_MODE,		0, "TV/MODE" },
	{ 0xae51cb04, KEY_MUTE,		0, "MUTE" },
	{ 0xad52cb04, KEY_PAGEDOWN,	0, "PAGE DOWN" },
	{ 0xbe41cb04, KEY_1,		0, "1" },
	{ 0xfc03cb04, KEY_2,		0, "2" },
	{ 0xbd42cb04, KEY_3,		0, "3" },
	{ 0xba45cb04, KEY_4,		0, "4" },
	{ 0xf807cb04, KEY_5,		0, "5" },
	{ 0xb946cb04, KEY_6,		0, "6" },
	{ 0xaa55cb04, KEY_7,		0, "7" },
	{ 0xe817cb04, KEY_8,		0, "8" },
	{ 0xa956cb04, KEY_9,		0, "9" },
	{ 0xe41bcb04, KEY_0,		0, "0" },
	{ 0xa659cb04, KEY_DELETE,	1, "DELETE" },
	{ 0xa55acb04, KEY_NUMLOCK,	0, "CAP/NUM" },
	{ 0xb847cb04, KEY_RESTART,	0, "RETURN" },
	{ 0xfa05cb04, KEY_SELECT,	0, "SOURCE" },
	{ 0xbc43cb04, KEY_LEFT,		1, "LEFT" },
	{ 0xfe01cb04, KEY_RIGHT,	1, "RIGHT" },
	{ 0xf906cb04, KEY_UP,		1, "UP" },
	{ 0xf50acb04, KEY_DOWN,		1, "DOWN" },
	{ 0xfd02cb04, KEY_ENTER, 	0, "ENTER" },
	{ 0xb44bcb04, KEY_INFO,		0, "INFO" },
	{ 0xf609cb04, KEY_HOME,		0, "HOME" },
	{ 0xac53cb04, KEY_MENU,		0, "MENU" },
	{ 0xed12cb04, KEY_PREVIOUSSONG,	1, "PREV" },
	{ 0xaf50cb04, KEY_PLAYCD,	0, "PLAY" },
	{ 0xee11cb04, KEY_NEXTSONG,	1, "NEXT" },
	{ 0xb04fcb04, KEY_TITLE, 	0, "TITLE" },
	{ 0xf10ecb04, KEY_BACK,		1, "REV" },
	{ 0xb34ccb04, KEY_STOPCD,	0, "STOP" },
	{ 0xf20dcb04, KEY_FORWARD,	1, "FWD" },
	{ 0xa857cb04, KEY_REDO, 	0, "REPEAT" },
	{ 0xe916cb04, KEY_ANGLE,	0, "ANGLE" },
	{ 0xab54cb04, KEY_PAUSECD,	0, "PAUSE" },
	{ 0xea15cb04, KEY_NEXT,		1, "SLOW" },
	{ 0xa45bcb04, KEY_TIME,		0, "TIME SEEK" },
	{ 0xe51acb04, KEY_AUDIO,	0, "AUDIO" },
	{ 0xa758cb04, KEY_SUBTITLE,	0, "SUBTITLE" },
	{ 0xe619cb04, KEY_ZOOM,		0, "ZOOM" },
	{ 0xa05fcb04, KEY_FN_F1, 	0, "RED FN" },
	{ 0xe11ecb04, KEY_FN_F2,	0, "GREEN FN" },
	{ 0xa35ccb04, KEY_FN_F3,	0, "YELLOW FN" },
	{ 0xe21dcb04, KEY_FN_F4,	0, "BLUE FN" },
};

static IR_POINTER nec_pointer_table[] = {
	/* Sigma Designs' DVD remote */
	{ 0xeb14e608, 0, {{ 0, -1 }},	"UP" },
	{ 0xe21de608, 0, {{ -1, 0 }},	"LEFT" },
	{ 0xee11e608, 0, {{ 1, 0 }},	"RIGHT" },
	{ 0xe916e608, 0, {{ 0, 1 }},	"DOWN" },
	{ 0xea15e608, 1, {{ BTN_LEFT}},	"ENTER" },
	/* Popcorn remote */
	{ 0xf906cb04, 0, {{ 0, -1 }},	"UP" },
	{ 0xbc43cb04, 0, {{ -1, 0 }},	"LEFT" },
	{ 0xfe01cb04, 0, {{ 1, 0 }},	"RIGHT" },
	{ 0xf50acb04, 0, {{ 0, 1 }},	"DOWN" },
	{ 0xfd02cb04, 1, {{ BTN_LEFT }},	"ENTER" },
};

#if 0
static FIP_KEY ref1_table[] = {
	{ FIP_KEY_EJECT, KEY_EJECTCLOSECD,	"EJECT" },
	{ FIP_KEY_PREV, KEY_PREVIOUSSONG,	"PREV" },
	{ FIP_KEY_NEXT, KEY_NEXTSONG,		"NEXT" },
	{ FIP_KEY_FBWD, KEY_BACK,		"REV" },
	{ FIP_KEY_FFWD, KEY_FORWARD,		"FWD" },
	{ FIP_KEY_PLAYPAUSE, KEY_PLAYPAUSE,	"PLAY/PAUSE" },
	{ FIP_KEY_STOP, KEY_STOPCD,		"STOP" },
};
#endif

enum{
	MODE_KEY=0,
	MODE_POINTER	
};

class CUinput:public CppBase::ITimerListener
{
public:
	class CIrKeyMapItem
	{
	public:
		CIrKeyMapItem();
		virtual ~CIrKeyMapItem();
	public:
		UINT16_t	m_LinuxInputKeyCode;
		BOOL_t 		m_bRepeat;
	};
	typedef map < UINT32_t, SharedPtr <CIrKeyMapItem> > IR_KEY_MAP;

	
public:
	CUinput();
	virtual ~CUinput();
	INT_t					setup_uinput_device(void);
	INT_t					LoadIrKeyMapTable();
	void					loadIrRepeatKeyTable();
	IR_KEY_MAP & 			getIrKeyMapTable();
	INT_t					open();
	void					close();
	INT_t 					On_IrInput();
	virtual VOID 			OnTimer(UINT_t TimerId);
	INT_t 					getNextTimerTimeoutMs(OUT INT_t & NextTimeoutMs);
	INT_t 					CheckAndTrigTimer();
	INT_t 					waitIrInputEventTimeout(INT_t iTimeoutMs);
	INT_t 					resetIrInputEvent();
	INT_t 					processEvent();
	INT_t					processIrEvent(NEXUS_InputRouterCode& ircode);
	void 					notifyIrInput(void);
protected:
	unsigned int	m_iEventMode;
public:
	WeakPtr <CUinput> m_this_wp;
	
private:	
	NEXUS_InputClientHandle 	client;
	CppBase::CGenericTimer 		m_oTimerList;
	UINT_t					 	m_idLedBusyTimer;
	BOOL_t						m_bLedInBusy;
	IR_KEY_MAP 					m_IrKeyMap;
	CMutex2 					m_IrinputMutex;
	CEventEx 					m_EventOnIrInput;
	CMutex2 					m_IrKeyMapMutex;
	//limit repeatable keys not too fast
	UINT32_t m_LastIrCode;
	UINT64_t m_RepeatableKeyChkStartTimeMs;
};

#endif

