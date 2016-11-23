TEMPLATE = app
TARGET = SystemService
CONFIG += release
DEPENDPATH += . GeneratedFiles
PRJ_TOP_DIR = $(PRJ_TOP_DIR)
WorkDir = $${PWD}
isEmpty(QTDIR) {
	QTDIR = $(QT_DIR)
}
# Qt base include
INCLUDEPATH += $${QTDIR}/include $${QTDIR}/include/QtWidgets $${QTDIR}/include/QtCore

INCLUDEPATH += . $$quote($$(PWD)/../LinuxBaseLib/Include) 
LINUXBASELIB_DIR = $$quote($$(PWD)/../LinuxBaseLib/Out)
#SYSTEM_CMD=$$quote(echo LINUXBASELIB_DIR=$${LINUXBASELIB_DIR})
#system($${SYSTEM_CMD})
#QMAKE_RPATHDIR += $$quote($${LINUXBASELIB_DIR})
# libQtExt
INCLUDEPATH += $$quote($$(PWD)/../QtExt) $$quote($$(PWD)/../QtExt/Api) 
LIB_QTEXT_DIR = $$quote($$(PWD)/../QtExt)
# QtDBus
INCLUDEPATH += $${QTDIR}/include/QtDBus
QT_LIB_DIR = $${QTDIR}/lib
# SysBaseSrv
SysBaseSrv_DIR = $$quote($$(PWD)/../SysBaseSrv)
INCLUDEPATH += $${SysBaseSrv_DIR}
INCLUDEPATH += ../../../nexus/nxclient/include/
# MediaPlayer
MediaPlayer_DIR = $$quote($$(PWD)/../MediaPlayer)
INCLUDEPATH += $${MediaPlayer_DIR}
# libGlobalStatus
libGlobalStatus_DIR = $$quote($$(PWD)/../libGlobalStatus)
INCLUDEPATH += $${libGlobalStatus_DIR}/Api
# libMp7xxCommon
libMp7xxCommon_DIR = $$quote($$(PWD)/../libMp7xxCommon)
INCLUDEPATH += $${libMp7xxCommon_DIR}/Api

# libiw
libiw_DIR = $$quote($$(PWD)/../../opensource/wireless/wireless_tools.29)
INCLUDEPATH += $${libiw_DIR}

# lib-dbus
libDBus_DIR = $$quote($$(PWD)/../dbus)
INCLUDEPATH += $${libDBus_DIR}

# libwpa_ctrl
libwpa_ctrl_DIR = $$quote($$(PWD)/../../opensource/wireless/wpa_supplicant-0.7.3/src/common)
INCLUDEPATH += $${libwpa_ctrl_DIR}
INCLUDEPATH += $$quote($$(PWD)/../../opensource/wireless/wpa_supplicant-0.7.3/src/utils)

# WebBrowser
INCLUDEPATH += $$quote($$(PWD)/../WebBrowser/Api)

#nexus
#INCLUDEPATH += $$(PRJ_TOP_DIR)/nexus/platforms/common/include
#INCLUDEPATH += $$(PRJ_TOP_DIR)/nexus/modules/core/include
#INCLUDEPATH += $$(PRJ_TOP_DIR)/nexus/base/include
#INCLUDEPATH += $$(PRJ_TOP_DIR)/nexus/base/include/public
#INCLUDEPATH += $$(PRJ_TOP_DIR)/nexus/base/include/wince


# libNexusForSysService
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
	libNexusForSysService_DIR = $${WorkDir}/libNexusForSysService
} else {
	libNexusForSysService_DIR = $${WorkDir}/libNexusForSysService
}
#message($${libNexusForSysService_DIR})
exists($${libNexusForSysService_DIR}) {
	contains(INCLUDEPATH, $${libNexusForSysService_DIR}/Api) {
	} else {
		INCLUDEPATH += $${libNexusForSysService_DIR}/Api
	}
	contains(LIBS, -L$${libNexusForSysService_DIR}/Out) {
	} else {
		LIBS += -L$${libNexusForSysService_DIR}/Out
	}	
	contains(LIBS, -lNexusForSysService) {
	} else {
		LIBS += -lNexusForSysService
	}	
} else {
	error("Not found libNexusForSysService($${libNexusForSysService_DIR})")
}

PRE_TARGETDEPS += libNexusForSysService/Out/libNexusForSysService.so
libNexusForSysService.target = libNexusForSysService/Out/libNexusForSysService.so
libNexusForSysService.commands = @echo Make: libNexusForSysService; make -C libNexusForSysService
libNexusForSysService.depends = FORCE
QMAKE_EXTRA_TARGETS += libNexusForSysService


# libnexus.so
isEmpty(PRJ_TOP_DIR) {
	NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
	NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
	contains(LIBS, -L$${NexusBinDir}) {
	} else {
		LIBS += -L$${NexusBinDir}
	}	
	contains(LIBS,$${NexusBinDir}/libnexus.so) {
	} else {
		LIBS += $${NexusBinDir}/libnexus.so
	}	
} else {
	error("Not found NexusBinDir($${NexusBinDir})")
}



# MRUA
#INCLUDEPATH += $$quote($(RUA_DIR))
#QMAKE_CXXFLAGS += $(RMCFLAGS)
# mrua libs
#mrua_DIR = ../mrua
#exists($${mrua_DIR}) {
#	LIBS += -L${RUA_DIR}/lib -lrua -lllad -lrmcore
#} else {
#	error("Not found mrua")
#}

# cs_rootfs
cs_rootfs_DIR = $$(PWD)/../cs_rootfs
exists($${cs_rootfs_DIR}) {
} else {
	cs_rootfs_DIR = 
}
isEmpty(cs_rootfs_DIR) {
	cs_rootfs_DIR = $$(PWD)/../cs_rootfs_1.2.19
}
isEmpty(cs_rootfs_DIR) {
	error("Not found the cs_rootfs directory")
}
INCLUDEPATH += $${cs_rootfs_DIR}/package/target-shell/busybox/busybox-1.15.3/include

# source files
HEADERS += SystemServiceApp.h	\
	NetworkManagerSrv.h		\
	NetMgrEthernet.h	\
    NetMgrWifi.h    \
	SystemServiceAppIf.h	\
	SystemSrvApi.h	\
	DbgLogSwitchDef.h	\
	SystemEvent.h	\
	IrInputCtrl.h	\
	BackendTransactionThread.h \
	WpasCtrl.h \
	HwWatchDog.h \
	PowerMate.h \
	NetDevListReader.h 
	
	
SOURCES += SystemServiceApp.cpp	\
	NetworkManagerSrv.cpp	\
	NetMgrEthernet.cpp	\
    NetMgrWifi.cpp  \
	SystemServiceAppIf.cpp	\
	SystemSrvApi.cpp	\
	DbgLogSwitchDef.cpp	\
	SystemEvent.cpp	\
	IrInputCtrl.cpp	\
	BackendTransactionThread.cpp \
	WpasCtrl.cpp \
	HwWatchDog.cpp \
	PowerMate.cpp \
	NetDevListReader.cpp 
	
		
QT += core dbus

LIBS += -L$${LINUXBASELIB_DIR} -lLinuxBaseLib \
	-L$${LIB_QTEXT_DIR} -lQtExt	\
	-L$${QT_LIB_DIR} -lQt5DBus -lQt5Widgets	\
	-L$${SysBaseSrv_DIR}/Out -lSysBaseSrv	\
	-L$${libGlobalStatus_DIR}/Out -lGlobalStatus	\
	-L$${libDBus_DIR}/out/lib -ldbus-1 \
	-L$${libwpa_ctrl_DIR} -lwpa_ctrl	\
	-L$${libMp7xxCommon_DIR}/Out/Obj -lMp7xxCommon	\

contains(QMAKE_CXXFLAGS, -ggdb) {
} else {
	QMAKE_CXXFLAGS += -ggdb
}

contains(QMAKE_LFLAGS, -ggdb) {
} else {
	QMAKE_LFLAGS += -ggdb
}
