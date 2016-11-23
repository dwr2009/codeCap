WORK_DIR=$(shell pwd)
-include ../common.inc

CROSS := arm-linux-
CC := $(CROSS)gcc
CXX := $(CROSS)g++
TARGET_STRIP := $(CROSS)strip
THISFILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
#${warning THISFILE=$(THISFILE)}
OUTPUT_DIR=$(WORK_DIR)/Out

COMMON_DIR := $(ARM_LINUX_BASE_DIR)/common
LIBC2_DIR := $(COMMON_DIR)/libc2
LIB_NETLAYER_DIR := $(COMMON_DIR)/NetLayer
LIB_SIGNAL_DIR := $(COMMON_DIR)/Signal
LIB_STRING_DIR := $(COMMON_DIR)/String

CPPFLAGS := -Wall -D_GNU_SOURCE -D_BSD_SOURCE -DWITH_MONO -D_FILE_OFFSET_BITS=64	\
	-D_LARGEFILE64_SOURCE 	\
	-I$(COMMON_DIR) -I$(LIBC2_DIR) -I$(COMMON_DIR)/include 	\
	-I$(LIB_NETLAYER_DIR) -I$(LIB_SIGNAL_DIR) -I$(COMMON_DIR)/Socket -I$(COMMON_DIR)/String	\
	-I$(COMMON_DIR)/File -I$(COMMON_DIR)/BufManage -I$(COMMON_DIR)/DateTime	\
	-I$(RUA_DIR) -I$(ARM_LINUX_BASE_DIR)/BaseLib/Include -I$(ZLIB_DIR)	\
	-I$(RUA_DIR)/MPlayer $(RMCFLAGS) \
	-I${WORK_DIR}/../../../nexus/nxclient/include/
	
LDFLAGS := -L$(TOOLCHAIN_PATH)/lib -lpthread -lstdc++

CPPFLAGS += -ggdb
LDFLAGS += -ggdb

ifeq '$(findstring " -O",$(CPPFLAGS))' ''
	CPPFLAGS += -O2
endif

ZLIB_STATIC := 
LIB = 
TARGET := SysPropOps
MAIN_SRC := SysPropOpsApp.cpp
SRC := 

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
	LDFLAGS +=  -lrt
endif

# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(WORK_DIR)/../LinuxBaseLib"
LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(CPPFLAGS))' ''
        CPPFLAGS += -I$(LINUXBASELIB_DIR)/Include
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

# Find the path of SysBaseSrv
SysBaseSrv_DIR := "$(WORK_DIR)/../SysBaseSrv"
SysBaseSrv_EXIST = $(shell test -d "$(SysBaseSrv_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning SysBaseSrv_DIR_EXIST=$(SysBaseSrv_DIR_EXIST))
ifeq ($(SysBaseSrv_EXIST),1)
    ifeq '$(findstring "-I$(SysBaseSrv_DIR)",$(RMCFLAGS))' ''
        RMCFLAGS += -I$(SysBaseSrv_DIR)
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

all: "$(OUTPUT_DIR)/$(TARGET_NAME)"
	
"$(OUTPUT_DIR)/$(TARGET_NAME)": $(TARGET_NAME)
	@if [ ! -d "$(OUTPUT_DIR)" ]; then	\
		mkdir "$(OUTPUT_DIR)";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "Copy: $< => $@";	\
	cp -fa $< $@;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	chmod a+x Out/$(TARGET_NAME);	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target strip: $@";	\
	$(TARGET_STRIP) --strip-all $@;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;

$(SRC):$(THISFILE)
	touch $(SRC)
	
%.d:%.cpp 
	set -e;\
	$(CC) -MM $(CPPFLAGS) $< > $@;\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo '	$$(CC) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

%.d:%.c 
	set -e;\
	$(CC) -MM $(CPPFLAGS) $< > $@;\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo '	$$(CC) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

include $(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d)

LINKER := $(CC)

ifeq ('', $(SRC_CPP))
	LINKER := $(CC)
else
	LINKER := $(CC)
endif

OBJ_FILES := $(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o)

$(TARGET_NAME):$(OBJ_FILES) $(LIB)
	$(LINKER) $(LDFLAGS) 	\
	-Wl,--start-group	\
	$^ $(LIB) 	\
	-Wl,--end-group	\
	-o $@

$(ZLIB_STATIC):

.PHONY : clean
clean:
	rm -f *.o *.obj $(TARGET_NAME) *.d Out/Obj/*.d
	
.PHONY : remake
remake:
	make clean -f $(THISFILE)
	make -f $(THISFILE)
	
