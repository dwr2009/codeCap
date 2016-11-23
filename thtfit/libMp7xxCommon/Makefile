WORK_DIR := $(shell pwd)
CROSS := arm-linux-
TARGET_CC := $(CROSS)gcc
TARGET_AR := $(CROSS)ar
TARGET_LD := $(CROSS)ld
TARGET_STRIP := $(CROSS)strip
THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
#${warning THIS_FILE=$(THIS_FILE)}
OUTPUT_DIR := $(WORK_DIR)/Out
OUT_OBJ_DIR := $(OUTPUT_DIR)/Obj

CFLAGS := -shared -fPIC -Wall -IInclude -I$(COMMON_INC_DIR) -I$(COMMON_LIB_DIR)/libc2	\
	-I$(COMMON_LIB_DIR) -I$(ZLIB_DIR) -I$(COMMON_LIB_DIR)/Thread	\
	-I$(COMMON_LIB_DIR)/String -I$(COMMON_LIB_DIR)/NetLayer -I$(COMMON_LIB_DIR)/DebugConfig	\
	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE	\
	-D_BSD_SOURCE -D_XOPEN_SOURCE -DUNIX -D_GNU_SOURCE $(RMCFLAGS)	\
	-I../../../nexus/nxclient/include/
CPPFLAGS := -fcheck-new
LINKFLAGS= -shared -fPIC -lstdc++

ifeq '$(findstring "-ggdb3",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) -ggdb3
endif
ifeq '$(findstring "-ggdb3",$(LINKFLAGS))' ''
	LINKFLAGS := $(LINKFLAGS) -ggdb3
endif

LIBDIR=.
LIB=
TARGET := libMp7xxCommon.so

SRC := SnmpCmdConv.cpp AppFilePathManage.cpp DevConnectionStatus.cpp LedStatusIndicator.cpp	\
	GuiOsd2FrameBufferCtrl.cpp RmDbgImplementation.cpp ProcessRunningInfo.cpp

SRC_CPP=$(filter %.cpp, $(SRC))
SRC_C=$(filter %.c, $(SRC))

ifeq '$(findstring " -O",$(CFLAGS))' ''
	CFLAGS += -O2
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

ifeq '$(findstring "-I$(WORK_DIR)",$(CFLAGS))' ''
	CFLAGS += -I$(WORK_DIR)
endif

ifneq ($(PLATFORM),)
	ifeq '$(findstring "-DPLATFORM=$(PLATFORM)",$(CFLAGS))' ''
		CFLAGS := $(CFLAGS) -DPLATFORM=$(PLATFORM)
	endif	
endif

# Find the path of libGlobalStatus
libGlobalStatus_DIR := "$(WORK_DIR)/../libGlobalStatus"
libGlobalStatus_DIR_EXIST = $(shell test -d "$(libGlobalStatus_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libGlobalStatus_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libGlobalStatus_DIR)/Api",$(CPPFLAGS))' ''
        CPPFLAGS += -I$(libGlobalStatus_DIR)/Api
    endif
	ifeq '$(findstring " -L$(libGlobalStatus_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS += -L$(libGlobalStatus_DIR)/Out
	endif
	ifeq '$(findstring " -lGlobalStatus",$(LDFLAGS))' ''
		LDFLAGS += -lGlobalStatus
	endif
else
	$(error Not found the directory libGlobalStatus)
endif

# Find the path of libMp7xxCommon
libMp7xxCommon_DIR := "$(WORK_DIR)/../libMp7xxCommon"
libMp7xxCommon_DIR_EXIST := $(shell test -d "$(libMp7xxCommon_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libMp7xxCommon_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libMp7xxCommon_DIR)/Api",$(CPPFLAGS))' ''
        CPPFLAGS := $(CPPFLAGS) -I$(libMp7xxCommon_DIR)/Api
    endif
else
	$(error Not found the directory libGlobalStatus)
endif

# Find the path of SysBaseSrv
SysBaseSrv_DIR := "$(WORK_DIR)/../SysBaseSrv"
SysBaseSrv_EXIST := $(shell test -d "$(SysBaseSrv_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(SysBaseSrv_EXIST),1)
    ifeq '$(findstring "-I$(SysBaseSrv_DIR)",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(SysBaseSrv_DIR)
    endif
	ifeq '$(findstring " -L$(SysBaseSrv_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(SysBaseSrv_DIR)/Out
	endif
	ifeq '$(findstring " -lSysBaseSrv",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lSysBaseSrv
	endif
else
    ${error Not found the directory SysBaseSrv}
endif

ifeq '$(findstring "-I$(WORK_DIR)",$(CFLAGS))' ''
	CFLAGS += -I$(WORK_DIR)
endif

# Find the path of MediaPlayer
MediaPlayer_DIR := "$(WORK_DIR)/../MediaPlayer"
MediaPlayer_DIR_EXIST := $(shell test -d "$(MediaPlayer_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(MediaPlayer_DIR_EXIST),1)
    ifeq '$(findstring "-I$(MediaPlayer_DIR)",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(MediaPlayer_DIR)
    endif
else
    $(error Not found the directory MediaPlayer)
endif

# Find the path of WebBrowser
WebBrowser_DIR := "$(WORK_DIR)/../WebBrowser"
WebBrowser_DIR_EXIST := $(shell test -d "$(WebBrowser_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(WebBrowser_DIR_EXIST),1)
    ifeq '$(findstring "-I$(WebBrowser_DIR)",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(WebBrowser_DIR)
    endif
    ifeq '$(findstring "-I$(WebBrowser_DIR)/Api",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(WebBrowser_DIR)/Api
    endif
else
    $(error Not found the directory WebBrowser)
endif

# RUA_DIR
ifeq '$(findstring "-I$(RUA_DIR)",$(CFLAGS))' ''
    CFLAGS := $(CFLAGS) -I$(RUA_DIR)
endif

all:$(OUTPUT_DIR)/$(TARGET)
	@if [ ! -d $(OUTPUT_DIR) ]; then	\
		mkdir $(OUTPUT_DIR);	\
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode != 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	cp -fa $(OUTPUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/;	\
	ProcessExitCode=$$?;	\
	if [ $$ProcessExitCode != 0 ]; then	\
		exit 1;	\
	fi;	\
	$(TARGET_STRIP) --strip-debug $(OUTPUT_DIR)/$(TARGET);	\
	ProcessExitCode=$$?;	\
	if [ $$ProcessExitCode != 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target: $(OUTPUT_DIR)/$(TARGET)"

$(SRC):$(THIS_FILE)
	touch $(SRC)

$(OUT_OBJ_DIR)/Process.o: override CFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CFLAGS))
$(OUT_OBJ_DIR)/Process.o: override CPPFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CPPFLAGS))
ifeq '$(findstring -msoft-float,$(CFLAGS))' ''
  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CFLAGS += -msoft-float
endif
ifeq '$(findstring -msoft-float,$(CPPFLAGS))' ''
  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CPPFLAGS += -msoft-float
endif

$(OUT_OBJ_DIR)/%.d:%.cpp
	if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		mkdir -p $(OUT_OBJ_DIR);	\
	fi;	\
	set -e;\
	echo -n "$(OUT_OBJ_DIR)/" > $@;	\
	$(TARGET_CC) -MM $(CFLAGS) $(CPPFLAGS) $< >> $@;\
	iProcessExitCode=$$?;	\
	if [ $${iProcessExitCode} -ne 0 ]; then	\
		rm -f $@;	\
		exit 1;	\
	fi;	\
	echo '	$$(TARGET_CC) $$(CFLAGS) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/%.d:%.c
	if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		mkdir -p $(OUT_OBJ_DIR);	\
	fi;	\
	set -e;\
	echo -n "$(OUT_OBJ_DIR)/" > $@;	\
	$(TARGET_CC) -MM $(CFLAGS) $< >> $@;\
	iProcessExitCode=$$?;	\
	if [ $${iProcessExitCode} -ne 0 ]; then	\
		rm -f $@;	\
		exit 1;	\
	fi;	\
	echo '	$$(TARGET_CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

DEPEND_FILES := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d))

include $(DEPEND_FILES)

OBJS := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o))
#${warning OBJS=$(OBJS)}

$(OUTPUT_DIR)/$(TARGET): $(OBJS)
	$(TARGET_CC) $(LINKFLAGS) $(LDFLAGS) -o $@ $^ $(PRIVATE_OBJS)

.PHONY : clean
clean:
	rm -fr $(OUTPUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/$(TARGET) $(OBJS) $(DEPEND_FILES) \
		$(OUT_OBJ_DIR)/*.d $(OUTPUT_DIR)
	
.PHONY : remake
remake:
	make clean
	make
	
.PHONY:cleanall
cleanall:clean
