base_dir := $(RUA_DIR)
WORK_DIR := $(shell pwd)

TARGET_TYPE := EXECUTABLE
DISABLE_WARNINGS := 1
include $(base_dir)/scripts/inc.Target

MAIN_SRC := TunerMn88436Test.cpp

SRC = dbgimplementation.c \
    rmmmimplementation.c

# Find the path of libTunerMn88436
libTunerMn88436_DIR := $(WORK_DIR)/..
DIR_TO_TEST := $(libTunerMn88436_DIR)
#${warning DIR_TO_TEST=$(DIR_TO_TEST)}
TEST_DIR_EXIST := $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(TEST_DIR_EXIST),1)
    ifeq '$(findstring "-I$(DIR_TO_TEST)/include",$(RMCFLAGS))' ''
        RMCFLAGS := $(RMCFLAGS) -I$(DIR_TO_TEST)/include
    endif
    ifeq '$(findstring " -L$(DIR_TO_TEST)/src",$(LDFLAGS))' ''
            LDFLAGS := $(LDFLAGS) -L$(DIR_TO_TEST)/src
    endif
    ifeq '$(findstring " -lDMDTuner",$(LDFLAGS))' ''
            LDFLAGS := $(LDFLAGS) -lDMDTuner
    endif
else
    ${error Not found the directory libTunerMn88436_DIR}
endif

lib_list += \
    librua \
	librmcw \
	librmcore \
	libllad \
	libdcc
	
COMPILKIND += withoutdbg

RMCFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1
RMCFLAGS += \
    -DRMVDEMUX_LOCAL_RMOBJECT=1 \
    -DBASIC_MPEG4_FRAMEWORK=1


LDFLAGS += -ldl -lz -lrt
USE_STD_LIB = 1
NO_LIB_SUBDIR := 1
STATIC_LINKS_WITH := $(lib_list:%=$(base_dir)/lib/%.$(lib_ext))

# debug for gdb
ifeq '$(findstring "-ggdb",$(RMCFLAGS))' ''
    RMCFLAGS += -ggdb
endif
ifeq '$(findstring "-ggdb",$(LDFLAGS))' ''
	LDFLAGS += -ggdb
endif
LDFLAGS += -lDMDTuner

ifeq '$(findstring "-L../",$(LDFLAGS))' ''
        LDFLAGS := $(LDFLAGS) -L../
endif

# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(WORK_DIR)/../../LinuxBaseLib"
LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(RMCFLAGS))' ''
        RMCFLAGS += -I$(LINUXBASELIB_DIR)/Include
    endif
    ifeq '$(findstring " -L$(LINUXBASELIB_DIR)/Out",$(LDFLAGS))' ''
            LDFLAGS += -L$(LINUXBASELIB_DIR)/Out
    endif
    ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
            LDFLAGS += -lLinuxBaseLib
    endif
else
    LINUXBASELIB_DIR := "$(RUA_DIR)/../../LinuxBaseLib"
    LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
    #$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
    ifeq ($(LINUXBASELIB_DIR_EXIST),1)
        ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(RMCFLAGS))' ''
            RMCFLAGS += -I$(LINUXBASELIB_DIR)/Include
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
endif

# Find the path of MruaDevSdk
MruaDevSdk_DIR := "$(TOPDIR)/../MruaDevSdk"
MruaDevSdk_DIR_EXIST := $(shell test -d "$(MruaDevSdk_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning MruaDevSdk_DIR_EXIST=$(MruaDevSdk_DIR_EXIST))
ifeq ($(MruaDevSdk_DIR_EXIST),0)
	MruaDevSdk_DIR := "$(QTDIR)/../MruaDevSdk"
	MruaDevSdk_DIR_EXIST := $(shell test -d "$(MruaDevSdk_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
endif
ifeq ($(MruaDevSdk_DIR_EXIST),1)
    ifeq '$(findstring "-I$(MruaDevSdk_DIR)/Include",$(RMCFLAGS))' ''
        RMCFLAGS += -I$(MruaDevSdk_DIR)/Include
    endif
    ifeq '$(findstring "-I$(MruaDevSdk_DIR)/Api",$(RMCFLAGS))' ''
        RMCFLAGS += -I$(MruaDevSdk_DIR)/Api
    endif
	ifeq '$(findstring " -L$(MruaDevSdk_DIR)",$(LDFLAGS))' ''
		LDFLAGS += -L$(MruaDevSdk_DIR)
	endif
	ifeq '$(findstring " -lMruaDevSdk",$(LDFLAGS))' ''
		LDFLAGS += -lMruaDevSdk
	endif
else
    ${error Not found the directory MruaDevSdk}
endif

include $(base_dir)/scripts/inc.Makefile
