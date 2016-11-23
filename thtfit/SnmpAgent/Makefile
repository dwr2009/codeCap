WORK_DIR := $(shell pwd)

BChipVer := $(shell echo $(BCHP_VER) | tr '[A-Z]' '[a-z]')

#--------------------------------------------------------------------
CC    := arm-linux-gcc
CXX   := arm-linux-g++
STRIP := arm-linux-strip
LINK  := arm-linux-ld
RM    := /bin/rm -f

CFLAGS  = -O2 -Wall -fPIC
CFLAGS += -I. -I${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/Out/usr/local/include/ \
	-I${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/include \
	-I${PRJ_TOP_DIR}/AppLibs/target/${NEXUS_PLATFORM}${BChipVer}.arm-linux.debug/usr/local/include \
	-I${PRJ_TOP_DIR}/AppLibs/target/${NEXUS_PLATFORM}${BChipVer}.arm-linux.debug/include/dbus-1.0	\
	-I${PRJ_TOP_DIR}/AppLibs/opensource/dbus-1.8.0 \
	-I${PRJ_TOP_DIR}/AppLibs/opensource/wireless/wireless_tools.29 \

SOFLAGS = -L${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/Out/usr/local/lib \
	-shared -fPIC \
	-L${PRJ_TOP_DIR}/AppLibs/target/${NEXUS_PLATFORM}${BChipVer}.arm-linux.debug/usr/local/lib \
	-L${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/out/usr/local/lib \
	-L${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/snmplib/.libs	\
	-L${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/agent/.libs	\
	-L${PRJ_TOP_DIR}/AppLibs/opensource/net-snmp-5.7.2.1/agent/helpers/.libs	\
	-lnetsnmp -lnetsnmpmibs -lnetsnmpagent -lnetsnmphelpers -ldbus-1 \

LDFLAGS = 
ifeq '$(findstring "-lstdc++",$(LDFLAGS))' ''
    LDFLAGS := ${LDFLAGS} -lstdc++
endif


# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(WORK_DIR)/../LinuxBaseLib"
LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(CFLAGS))' ''
        CFLAGS += -I$(LINUXBASELIB_DIR)/Include
    endif
	ifeq '$(findstring " -L$(LINUXBASELIB_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS += -L$(LINUXBASELIB_DIR)/Out
	endif
	ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
		LDFLAGS += -lLinuxBaseLib
	endif
else
    $(error Not found the directory LinuxBaseLib)
endif

# Find the path of MediaPlayer
MediaPlayer_DIR := "$(WORK_DIR)/../MediaPlayer"
MediaPlayer_DIR_EXIST = $(shell test -d "$(MediaPlayer_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(MediaPlayer_DIR_EXIST),1)
    ifeq '$(findstring "-I$(MediaPlayer_DIR)",$(CFLAGS))' ''
        CFLAGS += -I$(MediaPlayer_DIR)
    endif
else
    $(error Not found the directory MediaPlayer)
endif

# Find the path of WebBrowser
WebBrowser_DIR := "$(WORK_DIR)/../WebBrowser"
WebBrowser_DIR_EXIST = $(shell test -d "$(WebBrowser_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(WebBrowser_DIR_EXIST),1)
    ifeq '$(findstring "-I$(WebBrowser_DIR)",$(CFLAGS))' ''
        CFLAGS += -I$(WebBrowser_DIR) -I$(WebBrowser_DIR)/Api
    endif
else
    $(error Not found the directory WebBrowser)
endif

#ifeq '$(findstring " -ggdb",$(LDFLAGS))' ''
#	LDFLAGS += -ggdb
#endif

#ifeq '$(findstring " -ggdb",$(LDFLAGS))' ''
#	LDFLAGS += -ggdb
#endif

# Find the libDBusAdvancedIf
libDBusAdvancedIf_DIR := "$(WORK_DIR)/../DBusAdvancedIf"
libDBusAdvancedIf_DIR_EXIST = $(shell test -d "$(libDBusAdvancedIf_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning libDBusAdvancedIf_DIR_EXIST=$(libDBusAdvancedIf_DIR_EXIST))
ifeq ($(libDBusAdvancedIf_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libDBusAdvancedIf_DIR)",$(CFLAGS))' ''
        CFLAGS += -I$(libDBusAdvancedIf_DIR)
    endif
	ifeq '$(findstring " -L$(libDBusAdvancedIf_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS += -L$(libDBusAdvancedIf_DIR)/Out
	endif
	ifeq '$(findstring " -lDBusAdvancedIf",$(LDFLAGS))' ''
		LDFLAGS += -lDBusAdvancedIf
	endif
else
	$(error Not found the directory DBusAdvancedIf)
endif

# Find the libMp7xxCommon
libMp7xxCommon_DIR := "$(WORK_DIR)/../libMp7xxCommon"
libMp7xxCommon_DIR_EXIST := $(shell test -d "$(libMp7xxCommon_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning libMp7xxCommon_DIR_EXIST=$(libMp7xxCommon_DIR_EXIST))
ifeq ($(libMp7xxCommon_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libMp7xxCommon_DIR)/Api",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(libMp7xxCommon_DIR)/Api
    endif
	ifeq '$(findstring " -L$(libMp7xxCommon_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(libMp7xxCommon_DIR)/Out
	endif
	ifeq '$(findstring " -lMp7xxCommon",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lMp7xxCommon
	endif
else
	$(error Not found the directory libMp7xxCommon)
endif

# Find the path of GpioBtnInput
GpioBtnInput_DIR := "$(WORK_DIR)/../GpioBtnInput"
GpioBtnInput_DIR_EXIST = $(shell test -d "$(GpioBtnInput_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning GpioBtnInput_DIR_EXIST=$(GpioBtnInput_DIR_EXIST))
ifeq ($(GpioBtnInput_DIR_EXIST),1)
    ifeq '$(findstring "-I$(GpioBtnInput_DIR)",$(CFLAGS))' ''
        CFLAGS += -I$(GpioBtnInput_DIR)
    endif
else
	$(error Not found the directory GpioBtnInput)
endif

# Find the path of mrua
#MruaSrc_DIR := "$(RUA_DIR)"
#MruaSrc_DIR_EXIST = $(shell test -d "$(MruaSrc_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#ifeq ($(MruaSrc_DIR_EXIST),1)
#    ifeq '$(findstring "-I$(MruaSrc_DIR)",$(CFLAGS))' ''
#        CFLAGS := $(CFLAGS) -I$(MruaSrc_DIR)
#    endif
#    ifeq '$(findstring "$(RMCFLAGS)",$(CFLAGS))' ''
#        CFLAGS := $(CFLAGS) $(RMCFLAGS)
#    endif
#    ifeq '$(findstring "-L$(RUA_DIR)/lib",$(LDFLAGS))' ''
#        LDFLAGS := $(LDFLAGS) -L$(RUA_DIR)/lib
#    endif
#    ifeq '$(findstring "-lrua",$(LDFLAGS))' ''
#        LDFLAGS := $(LDFLAGS) -lrua
#    endif
#    ifeq '$(findstring "-lrua",$(LDFLAGS))' ''
#        LDFLAGS := $(LDFLAGS) -lllad -lrmcore
#    endif
#else
#    $(error Not found the directory MruaSrc)
#endif

#--------------------------------------------------------------------
# Find the libGlobalStatus
libGlobalStatus_DIR = "$(WORK_DIR)/../libGlobalStatus"
libGlobalStatus_DIR_TO_TEST = $(libGlobalStatus_DIR)
libGlobalStatus_DIR_EXIST = $(shell test -d "$(libGlobalStatus_DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libGlobalStatus_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libGlobalStatus_DIR_TO_TEST)",$(RMCFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(libGlobalStatus_DIR_TO_TEST)/Api
    endif
	ifeq '$(findstring " -L$(libGlobalStatus_DIR_TO_TEST)/Out",$(LDFLAGS))' ''
        #${warning DIR_TO_TEST=$(DIR_TO_TEST)}
		LDFLAGS := $(LDFLAGS) -L$(libGlobalStatus_DIR_TO_TEST)/Out
	endif
	ifeq '$(findstring " -lGlobalStatus",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lGlobalStatus
	endif
else
	$(error Not found the libGlobalStatus directory )
endif
#-----------------

SRCS = subagent.cpp dbus.cpp \
	DbgLogSwitchDef.cpp \
	data_access/network.cpp \
 	data_access/system.cpp \
 	data_access/gpio.cpp \
 	data_access/player.cpp \
 	data_access/monitor.cpp \
 	data_access/media.cpp \
 	data_access/tuner.cpp

 SRCS += \
 	fileListing/fileListingTable.cpp \
 	fileListing/fileListingTable_data_access.cpp \
 	fileListing/fileListingTable_data_get.cpp \
 	fileListing/fileListingTable_data_set.cpp \
 	fileListing/fileListingTable_interface.cpp

 SRCS += \
 	channelTable/channelTable.cpp \
 	channelTable/channelTable_data_access.cpp \
 	channelTable/channelTable_data_get.cpp \
 	channelTable/channelTable_data_set.cpp \
 	channelTable/channelTable_interface.cpp


SRCS += traps.cpp

TARGET = libSnmpAgent.so

OBJS := $(addsuffix .o, $(basename $(SRCS)))
DEPS := $(addsuffix .d, $(basename $(SRCS)))

#-include $(DEPS)
#--------------------------------------------------------------------

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(SOFLAGS) $(LDFLAGS) $^ -o $@

InstallToRootfs:
	@ROOTFS_DIR="$${NFS_DBG_DIR}"; \
	if [ -z "$${ROOTFS_DIR}" ]; then \
	    echo "The env var NFS_DBG_DIR is empty"; \
	    exit 1; \
	fi; \
	if [ ! -d "$${ROOTFS_DIR}/usr/local/lib" ]; then	\
		mkdir -p "$${ROOTFS_DIR}/usr/local/lib";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "Copy: $(TARGET) => $${ROOTFS_DIR}/usr/local/lib/$(TARGET)"; \
	cp -fa $(TARGET) "$${ROOTFS_DIR}/usr/local/lib/";	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	DST_FILE="$${ROOTFS_DIR}/usr/local/lib/$(TARGET)";	\
	echo "Target strip: $${DST_FILE}";	\
	$(STRIP) --strip-debug $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi	\

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET)

#--------------------------------------------------------------------
%.o : %.c
	$(CC) -c -ggdb $(CFLAGS) -MD -MF $*.d -c $^ -o $@

%.o : %.cpp
	$(CXX) -c -ggdb $(CFLAGS) -MD -MF $*.d -c $^ -o $@

