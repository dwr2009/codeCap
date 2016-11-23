#ifndef SUBAGENT_H
#define SUBAGENT_H

#include "data_access/network.h"
#include "data_access/system.h"
#include "data_access/gpio.h"
#include "data_access/player.h"
#include "data_access/monitor.h"
#include "data_access/media.h"
#include "data_access/tuner.h"
#include "fileListing/fileListingTable.h"
#include "channelTable/channelTable.h"
#include "dbus.h"
#include "traps.h"
#include <WebBrowserAppDef.h>

#define UNUSED(x) (void)x

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
//    NetworkConfig network;
    MonitorConfig monitor;
    PlayerConfig player;
    MediaConfig media;
    SystemConfig system;
    GpioConfig gpio;
    TunerConfig tuner;
} MP7xxSetting;

typedef enum {
    SPValue_Integer, SPValue_Integer64, SPValue_String, SPValue_String_Array
} SPValueType;

typedef enum {
    SPBus_ControlCenter_UiSrv = 0,
    SPBus_WebBrowser_DataCommSrv,
    SPBus_SystemService_NetworkManager,
    SPBus_SystemService_IrInputCtrl,
    SPBus_MPlayer,
    SPBus_GpioBtnInput
} SPBusType;

typedef struct {
    SPValueType eType;
    union {
        int integer;
        const char *pString;
        const char **pArray;
    };
} SPArgs;

int init_libSnmpAgent(void);
int deinit_libSnmpAgent(void);

const char *stringActieMediaType(ActiveMedia_TYPE type);
const char *ParseUrlFilename(const char *url);

int DBus_setProperty(SPBusType busType, const char *pFuncName, SPArgs *pGetArgsExt, int first_arg_type, ...);
int DBus_getProperty(SPBusType busType, const char *pFuncName, SPArgs *pSetArgsExt, int first_arg_type, ...);

#ifdef __cplusplus
}
#endif

#endif /* SUBAGENT_H */
