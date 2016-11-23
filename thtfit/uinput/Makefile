SHELL := /bin/sh
CROSS_COMPILE := arm-linux-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
STRIP := $(CROSS_COMPILE)strip
LD := $(CROSS_COMPILE)ld
THISFILE := Makefile
PWD := $(shell pwd)
WORK_DIR := $(PWD)
GccVerIs4_8 := $(shell ScanRes=`arm-linux-gcc --version | grep "arm-linux-gcc" | grep "stbgcc-4.8-0.4"`;ProcessExitCode=$$?;if [ $${ProcessExitCode} -eq 0 ] && [ ! -z "$${ScanRes}" ]; then echo "1"; else echo "0"; fi;)
#${warning GccVerIs4_8=$(GccVerIs4_8)}
#${error stop here.}
NEXUS_TOP ?= $(PRJ_TOP_DIR)/nexus

include $(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build/platform_app.inc

LDFLAGS += $(NEXUS_LDFLAGS) $(NEXUS_LD_LIBRARIES) -lm

CFLAGS += $(NEXUS_CFLAGS) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES))
include $(NEXUS_TOP)/nxclient/include/nxclient.inc
LDFLAGS += $(NXCLIENT_LDFLAGS)

CFLAGS := $(CFLAGS) -Wall -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE \
			-I${WORK_DIR}/../DomCodecAccess	\
			-I${WORK_DIR}/../tinyxml-2.5.3		\
			-I$(NEXUS_TOP)/../magnum/portinginterface/xvd/include	\
			-I$(NEXUS_TOP)/nxclient/include	

BChipVer := $(shell echo $(BCHP_VER) | tr '[A-Z]' '[a-z]')

LDFLAGS := $(LDFLAGS) -L$(NEXUS_TOP)/../AppLibs/target/97445$(BChipVer).arm-linux.debug/usr/local/lib/ -lnexus	\
	  -lstdc++
ifneq ($(findstring -g,$(CFLAGS)),)
	CFLAGS := $(filter-out -g,$(CFLAGS))
endif
ifeq ($(findstring -ggdb,$(CFLAGS)),)
	CFLAGS := $(CFLAGS) -ggdb
endif
ifneq ($(findstring -g,$(LDFLAGS)),)
    LDFLAGS := $(filter-out -g,$(LDFLAGS))
endif
ifeq ($(findstring -ggdb,$(LDFLAGS)),)
    LDFLAGS := $(LDFLAGS) -ggdb
endif
ifneq ($(findstring -O,$(CFLAGS)),)
    CFLAGS := $(filter-out -O,$(CFLAGS))
endif
ifeq ($(findstring -O2,$(CFLAGS)),)
    CFLAGS := $(CFLAGS) -O2
endif
# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(WORK_DIR)/../LinuxBaseLib"
LINUXBASELIB_DIR_EXIST := $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(RMCFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(LINUXBASELIB_DIR)/Include
    endif

    ifeq '$(findstring " -L$(LINUXBASELIB_DIR)/Out",$(LDFLAGS))' ''
        LDFLAGS := $(LDFLAGS) -L$(LINUXBASELIB_DIR)/Out
    endif
  
    ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
        LDFLAGS := $(LDFLAGS) -lLinuxBaseLib
    endif
else
	$(error Not found the directory LinuxBaseLib)
endif

ZLIB_STATIC := 
BASE_LIB_STATIC := 
LIB = 
TARGET := 
MAIN_SRC := uinput.cpp
SRC := DbgLogSwitchDef.cpp
#$(wildcard *.c *.cpp)

SRC_CPP := $(filter %.cpp, $(SRC)) $(filter %.cpp, $(MAIN_SRC))
SRC_C := $(filter %.c, $(SRC)) $(filter %.c, $(MAIN_SRC))
SRC := $(SRC_C) $(SRC_CPP)

TARGET := $(strip $(TARGET))
MAIN_SRC := $(strip $(MAIN_SRC))
TARGET_NAME = $(TARGET)
ifeq '$(TARGET_NAME)' ''
	TARGET_NAME = $(basename $(MAIN_SRC))
endif

ifeq '$(findstring " -lrt",$(LDFLAGS))' ''
	LDFLAGS += -lrt
endif

# libtinyxml
TINYXML_SRC_DIR := $(WORK_DIR)/../tinyxml-2.5.3
DIR_TO_TEST := $(TINYXML_SRC_DIR)
DIR_EXIST = $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)",$(CFLAGS))' ''
        CFLAGS += -I$(DIR_TO_TEST)
    endif

    ifeq '$(findstring " -L$(DIR_TO_TEST)",$(LDFLAGS))' ''
        LDFLAGS += -L$(DIR_TO_TEST)
    endif

    ifeq '$(findstring " -ltinyxml",$(LDFLAGS))' ''
        LDFLAGS += -ltinyxml
    endif

endif



# Find the libDomCodecAccess
libDomCodecAccess_DIR := "$(WORK_DIR)/../DomCodecAccess"
DIR_TO_TEST := $(libDomCodecAccess_DIR)
DIR_EXIST = $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)",$(CFLAGS))' ''
        CFLAGS += -I$(DIR_TO_TEST)
    endif
	
    ifeq '$(findstring " -L$(DIR_TO_TEST)/Out",$(LDFLAGS))' ''
        LDFLAGS += -L$(DIR_TO_TEST)/Out
    endif

    ifeq '$(findstring " -lDomCodecAccess",$(LDFLAGS))' ''
        LDFLAGS += -lDomCodecAccess

    endif

else
	$(error Not found the libDomCodecAccess directory )
endif

# Find the libMp7xxCommon
libMp7xxCommon_DIR := "$(WORK_DIR)/../libMp7xxCommon"
DIR_TO_TEST := $(libMp7xxCommon_DIR)
DIR_EXIST := $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)/Api",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(DIR_TO_TEST)/Api
    endif
	ifeq '$(findstring " -L$(DIR_TO_TEST)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(DIR_TO_TEST)/Out
	endif
	ifeq '$(findstring " -lDomCodecAccess",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lMp7xxCommon
	endif
else
    $(error Not found the directory libMp7xxCommon)
endif

# Find the libGlobalStatus
libGlobalStatus_DIR := "$(WORK_DIR)/../libGlobalStatus"
DIR_TO_TEST := $(libGlobalStatus_DIR)
DIR_EXIST := $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)/Api",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(DIR_TO_TEST)/Api
    endif
	ifeq '$(findstring " -L$(DIR_TO_TEST)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(DIR_TO_TEST)/Out
	endif
	ifeq '$(findstring " -lDomCodecAccess",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lGlobalStatus
	endif
else
    $(error Not found the directory libGlobalStatus)
endif

# Find the SysBaseSrv
SysBaseSrv_DIR := "$(WORK_DIR)/../SysBaseSrv"
DIR_TO_TEST := $(SysBaseSrv_DIR)
DIR_EXIST := $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)/Api",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(DIR_TO_TEST)/Api
    endif
	ifeq '$(findstring " -L$(DIR_TO_TEST)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(DIR_TO_TEST)/Out
	endif
	ifeq '$(findstring " -lSysBaseSrv",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lSysBaseSrv
	endif
else
    $(error Not found the directory SysBaseSrv)
endif

ifeq '$(findstring "-lpthread",$(LDFLAGS))' ''
	LDFLAGS := $(filter-out -lpthread,$(LDFLAGS))
endif

all:$(TARGET_NAME)
	@if [ ! -d Out ]; then	\
		mkdir Out;	\
	fi;	\
	cp -fa $(TARGET_NAME) Out/;	\
	if [ $$? -eq 0 ]; then	\
		echo "Copy: $(TARGET_NAME) => Out/";		\
		chmod a+x Out/$(TARGET_NAME);	\
	fi;	\
	$(STRIP) --strip-debug Out/$(TARGET_NAME);	\
	if [ $$? -eq 0 ]; then	\
		echo "Target strip: Out/$(TARGET_NAME)";		\
	fi;	\

$(SRC):$(THISFILE)
	touch $(SRC)
	
%.d:%.cpp 
	set -e;\
	$(CXX) -MM $(CFLAGS) $< > $@;\
	echo '	$$(CXX) $$(CFLAGS) -c -o $$@ $$<' >> $@

%.d:%.c 
	set -e;\
	$(CC) -MM $(CFLAGS) $< > $@;\
	echo '	$$(CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

include $(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d)

LINKER := $(CC)

ifeq ('', $(SRC_CPP))
	LINKER := $(CC)
else
	LINKER := $(CXX)
endif

LINKER := $(CC)
#${warning LINKER=$(LINKER)}
#${warning CC=$(CC)}
#${warning CXX=$(CXX)}
#${error stop here.}

OBJ_FILES := $(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o)

$(TARGET_NAME):$(OBJ_FILES) $(LIB)
	$(LINKER) $(LDFLAGS) 	\
	-Wl,--start-group	\
	$^ $(LIB) 	\
	-Wl,--end-group	\
	-o $@

$(ZLIB_STATIC):

.PHONY : $(BASE_LIB_STATIC)
$(BASE_LIB_STATIC):
	make -C $(BASE_LIB_DIR)

.PHONY : clean
clean:
	rm -f *.o *.obj $(TARGET_NAME) *.d Out/Obj/*.d Out/$(TARGET_NAME)
	
.PHONY : remake
remake:
	make clean -f $(THISFILE)
	make -f $(THISFILE)
	
