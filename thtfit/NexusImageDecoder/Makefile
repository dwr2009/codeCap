PWD := $(shell pwd)
WORK_DIR := $(PWD)
THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(THIS_FILE)
CROSS=arm-linux-
CC=$(CROSS)gcc
TARGET_AR=$(CROSS)ar
LD=$(CROSS)ld
TARGET_STRIP=$(CROSS)strip
OUTPUT_DIR=$(WORK_DIR)/Out
OUT_OBJ_DIR := $(OUTPUT_DIR)/Obj
CPPFLAGS += -DENABLE_DTV

NEXUS_PLATFORM ?= $(PLATFORM)
ifndef NEXUS_PLATFORM
	$(error NEXUS_PLATFORM is not defined)
endif

# Find the path of nexus
nexus_DIR := $(PRJ_TOP_DIR)/nexus
nexus_DIR_EXIST = $(shell test -d "$(nexus_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(nexus_DIR_EXIST),1)
	NEXUS_TOP := $(nexus_DIR)
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
	$(error Not found the directory nexus($(nexus_DIR)))
endif

# Find the path of libLinuxBaseLib
libLinuxBaseLib_DIR := $(PWD)/../LinuxBaseLib
libLinuxBaseLib_DIR_EXIST = $(shell test -d "$(libLinuxBaseLib_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libLinuxBaseLib_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libLinuxBaseLib_DIR)/Include",$(CPPFLAGS))' ''
        CFLAGS := $(CPPFLAGS) -I$(libLinuxBaseLib_DIR)/Include
    endif
	ifeq '$(findstring " -L$(libLinuxBaseLib_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(libLinuxBaseLib_DIR)/Out
	endif
	ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lLinuxBaseLib
	endif
else
    ${error Not found the directory libLinuxBaseLib($(libLinuxBaseLib_DIR))}
endif

# Find the path of 
libGlobalStatus_DIR := $(PWD)/../libGlobalStatus
libGlobalStatus_DIR_EXIST = $(shell test -d "$(libGlobalStatus_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(libGlobalStatus_DIR_EXIST),1)
    ifeq '$(findstring "-I$(libGlobalStatus_DIR)/Api",$(CPPFLAGS))' ''
        CFLAGS := $(CPPFLAGS) -I$(libGlobalStatus_DIR)/Api
    endif
        ifeq '$(findstring " -L$(libGlobalStatus_DIR)/Out",$(LDFLAGS))' ''
                LDFLAGS := $(LDFLAGS) -L$(libGlobalStatus_DIR)/Out
        endif

        ifeq '$(findstring " -lGlobalStatus",$(LDFLAGS))' ''
                LDFLAGS := $(LDFLAGS) -lGlobalStatus
        endif
else
    ${error Not found the directory libGlobalStatus($(libGlobalStatus_DIR))}
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
include $(B_LIB_TOP)/playback_ip/b_playback_ip_lib.inc
CFLAGS += $(PLAYBACK_IP_CFLAGS) 
LDFLAGS += $(PLAYBACK_IP_LDFLAGS) 

# Include nexus definitions
#include $(NEXUS_TOP)/build/nexus_defs.inc
#NEXUS_PLATFORM_BUILDDIR=$(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build
#include $(NEXUS_TOP)/platforms/common/build/nexus_platforms.inc

#${warning NEXUS_PLATFORM_DEFINES=$(NEXUS_PLATFORM_DEFINES)}

#${warning B_REFSW_OS=$(B_REFSW_OS)}
#${warning NEXUS_APP_INCLUDE_PATHS=$(NEXUS_APP_INCLUDE_PATHS)}

CFLAGS := $(CFLAGS) -shared -fPIC -Wall -IInclude -I$(COMMON_INC_DIR) -I$(COMMON_LIB_DIR)/libc2	\
	-I$(COMMON_LIB_DIR) -I$(ZLIB_DIR) -I$(COMMON_LIB_DIR)/Thread	\
	-I$(COMMON_LIB_DIR)/String -I$(COMMON_LIB_DIR)/NetLayer -I$(COMMON_LIB_DIR)/DebugConfig	\
	-I$(NEXUS_TOP)/modules/core/include -I$(NEXUS_TOP)/nxclient/include \
	-I$(PRJ_TOP_DIR)/obj.97445/nexus/bin/include -I$(PRJ_TOP_DIR)/rockford/middleware/platform/nexus \
	-I$(WORK_DIR)/ 	 \
	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE	\
	-D_BSD_SOURCE -D_XOPEN_SOURCE -DUNIX -D_GNU_SOURCE 

CPPFLAGS := -fcheck-new
ifeq ($(findstring -shared,$(LDFLAGS)),)
	LDFLAGS := $(LDFLAGS) -shared
endif

ifeq ($(findstring -fPIC,$(LDFLAGS)),)
	LDFLAGS := $(LDFLAGS) -fPIC
endif

#${warning NEXUS_EXTRA_LDFLAGS=$(NEXUS_EXTRA_LDFLAGS)}

ifeq '$(findstring " -g",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) -ggdb3
endif
ifeq '$(findstring " -g",$(LDFLAGS))' ''
	LDFLAGS := $(LDFLAGS) -ggdb3
endif

LIBDIR=.
LIB=
TARGET := libNexusDecoder.so

SRC := NexusDecoder.cpp  namevalue.c dvr_crypto.c \
       botf_bcmindexer.c wav_file.c tsindexer.c picdecoder.c tspsimgr3.c psip_vct.c \
       parse_userdata.c

SRC_CPP=$(filter %.cpp, $(SRC))
SRC_C=$(filter %.c, $(SRC))

ifeq '$(findstring " -O",$(CFLAGS))' ''
	CFLAGS += -O2
endif

ifeq '$(findstring "-I Api",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) -I Api
endif

#NexusPlaybackPublicIncludes := $(NEXUS_PLAYBACK_PUBLIC_INCLUDES)
#${warning NexusPlaybackPublicIncludes=$(NexusPlaybackPublicIncludes)}
#ifeq '$(findstring "$(NexusPlaybackPublicIncludes)",$(CFLAGS))' ''
#	CFLAGS := $(CFLAGS) $(NexusPlaybackPublicIncludes)
#endif

# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(PRJ_TOP_DIR)/AppLibs/thtfit/LinuxBaseLib"
LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(CPPFLAGS))' ''
        CPPFLAGS += -I$(LINUXBASELIB_DIR)/Include
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

# Find the path of RTC
RTC_DIR := "$(PRJ_TOP_DIR)/AppLibs/thtfit/Rtc"
RTC_DIR_EXIST = $(shell test -d "$(RTC_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(RTC_DIR_EXIST),1)
    ifeq '$(findstring "-I$(RTC_DIR)",$(CPPFLAGS))' ''
        CPPFLAGS += -I$(RTC_DIR)
        CFLAGS += -I$(RTC_DIR)
    endif
 
   ifeq '$(findstring " -L$(RTC_DIR)/Out",$(LDFLAGS))' ''
        LDFLAGS += -L$(RTC_DIR)/Out
   endif

   ifeq '$(findstring " -lrtc",$(LDFLAGS))' ''
        LDFLAGS += -lrtc
   endif
else
        $(error Not found the directory rtc)
endif

ifeq '$(findstring "-I$(WORK_DIR)",$(CFLAGS))' ''
	CFLAGS += -I$(WORK_DIR)
endif

ifeq '$(findstring "-I$(NEXUS_TOP)/../BSEAV/lib/bcmplayer/include",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) -I$(NEXUS_TOP)/../BSEAV/lib/bcmplayer/include 
endif

ifeq '$(findstring "-I$(NEXUS_TOP)/../BSEAV/lib/dcc/include",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(NEXUS_TOP)/../BSEAV/lib/dcc/include -I$(NEXUS_TOP)/../magnum/commonutils/udp/include -I$(NEXUS_TOP)/../magnum/commonutils/avc/include
endif

ifeq '$(findstring " -DCMDLINE_TEST",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS)  -DCMDLINE_TEST
endif

MediaPlayer_Dir := $(WORK_DIR)/..
ifeq '$(findstring "-I$(MediaPlayer_Dir)/Api",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) -I$(MediaPlayer_Dir)/Api
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

$(SRC):$(MAKEFILE_NAME)
	@ \
	touch $(SRC) $(NEXUS_TOP)/../magnum/commonutils/udp/src/budp_dccparse.c $(NEXUS_TOP)/../magnum/commonutils/udp/src/budp_bitread.c; \
	ProcessExitCode=$$?;	\
	if [ $$ProcessExitCode != 0 ]; then	\
		exit 1;	\
	fi;	

CFLAGS := $(filter-out -D__KERNEL__ -I$(PRJ_TOP_DIR)/linux/arch/arm/include, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/arch/arm/include/generated, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/include, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/arch/arm/include/uapi, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/arch/arm/include/generated/uapi, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/include/uapi, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/include/generated/uapi, $(CFLAGS))
CFLAGS := $(filter-out -I$(PRJ_TOP_DIR)/linux/include/generated/uapi, $(CFLAGS))
CFLAGS := $(filter-out -fno-pic, $(CFLAGS))
CFLAGS := $(filter-out -std=c89, $(CFLAGS))

$(OUT_OBJ_DIR)/Process.o: override CFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CFLAGS))
$(OUT_OBJ_DIR)/Process.o: override CPPFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CPPFLAGS))
ifeq '$(findstring -msoft-float,$(CFLAGS))' ''
  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CFLAGS += -msoft-float
endif
ifeq '$(findstring -msoft-float,$(CPPFLAGS))' ''
  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CPPFLAGS += -msoft-float
endif

$(OUT_OBJ_DIR)/%.d:%.cpp 
	@if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		mkdir -p $(OUT_OBJ_DIR);	\
		ProcessExitCode=$$?; \
		if [ $${ProcessExitCode} -ne 0 ]; then \
			exit 1; \
		fi; \
	fi;
	echo -n "$(OUT_OBJ_DIR)/" > $@;	\
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< >> $@;\
	iProcessExitCode=$$?;	\
	if [ $${iProcessExitCode} -ne 0 ]; then	\
		echo "Del: $@"; \
		rm -f $@;	\
		exit 1;	\
	fi;	\
	echo '	$$(CC) $$(CFLAGS) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/%.d:%.c 
	@if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		echo "MkDir: $(OUT_OBJ_DIR)"; \
		mkdir -p $(OUT_OBJ_DIR);	\
		ProcessExitCode=$$?; \
		if [ $${ProcessExitCode} -ne 0 ]; then \
			exit 1; \
		fi; \
	fi;  \
	echo -n "$(OUT_OBJ_DIR)/" > $@;  \
	$(CC) -MM $(CFLAGS) $< >> $@;\
	iProcessExitCode=$$?;	\
	if [ $${iProcessExitCode} -ne 0 ]; then	\
		echo "Del: $@"; \
		rm -f $@;	\
		exit 1;	\
	fi;	\
	echo '	$$(CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/budp_dccparse.d:$(NEXUS_TOP)/../magnum/commonutils/udp/src/budp_dccparse.c 
	@if [ ! -d $(OUT_OBJ_DIR) ]; then       \
                echo "MkDir: $(OUT_OBJ_DIR)"; \
                mkdir -p $(OUT_OBJ_DIR);        \
                ProcessExitCode=$$?; \
                if [ $${ProcessExitCode} -ne 0 ]; then \
                        exit 1; \
                fi; \
        fi;   \
        echo -n "$(OUT_OBJ_DIR)/" > $@;  \
        $(CC) -MM $(CFLAGS) $< >> $@;\
        iProcessExitCode=$$?;   \
        if [ $${iProcessExitCode} -ne 0 ]; then \
                echo "Del: $@"; \
                rm -f $@;       \
                exit 1; \
        fi;     \
        echo '	$$(CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/budp_bitread.d:$(NEXUS_TOP)/../magnum/commonutils/udp/src/budp_bitread.c
	@if [ ! -d $(OUT_OBJ_DIR) ]; then       \
                echo "MkDir: $(OUT_OBJ_DIR)"; \
                mkdir -p $(OUT_OBJ_DIR);        \
                ProcessExitCode=$$?; \
                if [ $${ProcessExitCode} -ne 0 ]; then \
                        exit 1; \
                fi; \
        fi;   \
        echo -n "$(OUT_OBJ_DIR)/" > $@;  \
        $(CC) -MM $(CFLAGS) $< >> $@;\
        iProcessExitCode=$$?;   \
        if [ $${iProcessExitCode} -ne 0 ]; then \
                echo "Del: $@"; \
                rm -f $@;       \
                exit 1; \
        fi;     \
        echo '	$$(CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

DEPEND_FILES := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d))
DEPEND_FILES += $(OUT_OBJ_DIR)/budp_dccparse.d 
DEPEND_FILES += $(OUT_OBJ_DIR)/budp_bitread.d

include $(DEPEND_FILES)

OBJS := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o))
OBJS += $(OUT_OBJ_DIR)/budp_dccparse.o
OBJS += $(OUT_OBJ_DIR)/budp_bitread.o

$(OUTPUT_DIR)/$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(PRIVATE_OBJS)

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
