SHELL := /bin/sh
PWD := $(shell pwd)
WORK_DIR := $(PWD)
CROSS_COMPILE := arm-linux-
CC := $(CROSS_COMPILE)g++
CXX := $(CROSS_COMPILE)g++
TARGET_STRIP := ${CROSS_COMPILE}strip
THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
OUT_DIR := Out

define CFLAGS_add
ifeq '$(findstring "$(1)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(1)
endif
endef

define LDFLAGS_add
ifeq '$(findstring "$(1)",$(LDFLAGS))' ''
	LDFLAGS := $(LDFLAGS) $(1)
endif
endef

$(eval $(call CFLAGS_add,-O2))
$(eval $(call CFLAGS_add,-ggdb))

$(eval $(call LDFLAGS_add,-lpthread))
$(eval $(call LDFLAGS_add,-lstdc++))
$(eval $(call LDFLAGS_add,-lrt))
$(eval $(call LDFLAGS_add,-ggdb))

TARGET := SyncRtcToSystemTime
TARGET := $(strip $(TARGET))
MAIN_SRC := Main.cpp
MAIN_SRC := $(strip $(MAIN_SRC))
SRC :=  CNexusI2cBus.cpp CRtcIsl1208Device.cpp 
SRC_CPP := $(filter %.cpp, $(SRC)) $(filter %.cpp, $(MAIN_SRC))
SRC_C := $(filter %.c, $(SRC)) $(filter %.c, $(MAIN_SRC))
SRC := $(SRC_C) $(SRC_CPP)

TARGET_NAME = $(TARGET)
ifeq '$(TARGET_NAME)' ''
	TARGET_NAME = $(basename $(MAIN_SRC))
endif

NEXUS_PLATFORM ?= $(PLATFORM)
ifndef NEXUS_PLATFORM
	$(error NEXUS_PLATFORM is not defined)
endif

# Find the path of nexus
nexus_DIR := $(PRJ_TOP_DIR)/nexus
nexus_DIR_EXIST = $(shell test -d "$(nexus_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(nexus_DIR_EXIST),1)
	NEXUS_TOP := $(nexus_DIR)
else
	$(error Not found the directory nexus($(nexus_DIR)))
endif

ifeq ($(NEXUS_PREBUILT_BINARY),y)
# do "make api" and "make nexus_headers" first, then "make NEXUS_PREBUILT_BINARY=y"
include $(NEXUS_BIN_DIR)/include/platform_app.inc
Q_ ?= @
else
# include cross-compiler definitions
include $(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build/platform_app.inc
endif

ifeq '$(findstring "$(NEXUS_CFLAGS)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(NEXUS_CFLAGS)
endif

NexusAppIncPaths := $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS))
#${warning NexusAppIncPaths=$(NexusAppIncPaths)}
ifeq '$(findstring "$(NexusAppIncPaths)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(NexusAppIncPaths)
endif

NexusAppDefines := $(addprefix -D,$(NEXUS_APP_DEFINES))
ifeq '$(findstring "$(NexusAppDefines)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(NexusAppDefines)
endif

# NXCLIENT flags
include $(NEXUS_TOP)/nxclient/include/nxclient.inc
ifeq '$(findstring "$(NXCLIENT_CFLAGS)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(NXCLIENT_CFLAGS)
endif
ifeq '$(findstring "$(NXCLIENT_LDFLAGS)",$(LDFLAGS))' ''
	LDFLAGS := $(LDFLAGS) $(NXCLIENT_LDFLAGS)
endif

B_LIB_TOP = $(NEXUS_TOP)/lib
include $(B_LIB_TOP)/os/b_os_lib.inc

# Include nexus definitions
#include $(NEXUS_TOP)/build/nexus_defs.inc
#NEXUS_PLATFORM_BUILDDIR=$(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build
#include $(NEXUS_TOP)/platforms/common/build/nexus_platforms.inc

#${warning NEXUS_PLATFORM_DEFINES=$(NEXUS_PLATFORM_DEFINES)}

#${warning B_REFSW_OS=$(B_REFSW_OS)}
#${warning NEXUS_APP_INCLUDE_PATHS=$(NEXUS_APP_INCLUDE_PATHS)}

# Find the path of libLinuxBaseLib
LinuxBaseLib_DIR := "$(WORK_DIR)/../LinuxBaseLib"
DirToTest := $(LinuxBaseLib_DIR)
DirToTest_EXIST = $(shell test -d "$(DirToTest)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DirToTest_EXIST),1)
    ifeq '$(findstring "-I$(LinuxBaseLib_DIR)/Include",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(LinuxBaseLib_DIR)/Include
    endif
	ifeq '$(findstring " -L$(LinuxBaseLib_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(LinuxBaseLib_DIR)/Out
	endif
	ifeq '$(findstring "-lLinuxBaseLib",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lLinuxBaseLib
	endif
else
	$(error Not found the directory libLinuxBaseLib)
endif

# Find the libMp7xxCommon 
libMp7xxCommon_DIR = "$(WORK_DIR)/../libMp7xxCommon"
libMp7xxCommon_DIR_TO_TEST = $(libMp7xxCommon_DIR)
libMp7xxCommon_DIR_EXIST = $(shell test -d "$(libMp7xxCommon_DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libMp7xxCommon_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libMp7xxCommon_DIR_TO_TEST)",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(libMp7xxCommon_DIR_TO_TEST)/Api
    endif
#	ifeq '$(findstring " -L$(libMp7xxCommon_DIR_TO_TEST)/Out",$(LDFLAGS))' ''
#        #${warning DIR_TO_TEST=$(DIR_TO_TEST)}
#		LDFLAGS += -L$(libMp7xxCommon_DIR_TO_TEST)/Out
#	endif
#	ifeq '$(findstring " -lMp7xxCommon",$(LDFLAGS))' ''
#		LDFLAGS += -lMp7xxCommon
#	endif
else
	$(error Not found the libMp7xxCommon directory )
endif

# Find the path of SysBaseSrv
SysBaseSrv_DIR := "$(WORK_DIR)/../SysBaseSrv"
SysBaseSrv_DIR_EXIST = $(shell test -d "$(SysBaseSrv_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning SysBaseSrv_DIR_EXIST=$(SysBaseSrv_DIR_EXIST))
ifeq ($(SysBaseSrv_DIR_EXIST),1)
    ifeq '$(findstring "-I$(SysBaseSrv_DIR)",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(SysBaseSrv_DIR)
    endif
	ifeq '$(findstring " -L$(SysBaseSrv_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS += -L$(SysBaseSrv_DIR)/Out
	endif
	ifeq '$(findstring " -lSysBaseSrv",$(LDFLAGS))' ''
		LDFLAGS += -lSysBaseSrv
	endif
else
    ${error Not found the directory SysBaseSrv}
endif

CFLAGS := $(CFLAGS) -DLIB_CONFIG_CAPACITY=1

OBJ_FILES := $(addprefix $(OUT_DIR)/,$(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o))
#${warning OBJ_FILES=$(OBJ_FILES)}

define ProduceSrcDepends

$(OUT_DIR)/$(basename $(1)).d: $(1)
	@ \
	if [ ! -d "$(OUT_DIR)" ]; then \
		echo "MkDir $(OUT_DIR)"; \
		mkdir -p $(OUT_DIR); \
		ProcessExitCode=$$$$?; \
		if [ $$$${ProcessExitCode} -ne 0 ]; then \
			exit $$$${ProcessExitCode}; \
		fi; \
	fi;
	$(CXX) -MM $(CFLAGS) -Wp,-MT,$(OUT_DIR)/$(basename $(1)).o $(1) > $$@; \
    ProcessExitCode=$$$$?; \
    if [ $$$${ProcessExitCode} -ne 0 ]; then \
		rm -f $$@; \
		exit 1; \
	fi; \
	echo '	$$(CXX) $$(CFLAGS) -c -o $(OUT_DIR)/$(basename $(1)).o $(1)' >> $$@;
	
endef

all: $(OUT_DIR)/$(TARGET_NAME)

$(SRC): $(THIS_FILE)
	touch $(SRC)

$(eval $(foreach OneSrcFile,$(SRC_C),$(call ProduceSrcDepends,$(OneSrcFile))))
$(eval $(foreach OneSrcFile,$(SRC_CPP),$(call ProduceSrcDepends,$(OneSrcFile))))

DependFiles := $(addprefix $(OUT_DIR)/,$(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d))
#${warning DependFiles=$(DependFiles)}

-include $(DependFiles)

$(OUT_DIR)/$(TARGET_NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) 	\
	-Wl,--start-group	\
	$^ 	\
	-Wl,--end-group	\
	-o $@	
	
.PHONY : clean
clean:
	rm -fr *.o *.obj $(TARGET_NAME) *.d Out/Obj/*.d Out/*
	
.PHONY : remake
remake:
	make clean -f $(THIS_FILE)
	make -f $(THIS_FILE)
	
