ifneq '$(filter -DEM86XX_MODE=EM86XX_MODEID_STANDALONE,$(RMCFLAGS))' ''

RMCFLAGS += -DEM86XX_REMOTE=1

ifneq '$(filter -DEM86XX_CHIP=EM86XX_CHIPID_TANGO3,$(RMCFLAGS))' ''
CROSS      := mipsel-linux-
RMPLATFORM := RMPLATFORMID_TANGO3
COMPILKIND += withdl
lib_ext    := so
else # EM86XX_CHIPID_TANGO3
ifneq '$(filter -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2,$(RMCFLAGS))' ''

CROSS      := mipsel-linux-
RMPLATFORM := RMPLATFORMID_TANGO2
COMPILKIND += withdl
lib_ext    := so

else # EM86XX_CHIPID_TANGO2

CROSS      := arm-elf-
RMPLATFORM := RMPLATFORMID_JASPERMAMBO
RMCFLAGS   += -DLLAD_DIRECT -D__arm__
LDFLAGS    += -Wl,-elf2flt="-s32768"
lib_ext    := a

endif # EM86XX_CHIPID_TANGO2
endif # EM86XX_CHIPID_TANGO3

else # EM86XX_MODEID_STANDALONE
ifneq '$(filter -DEM86XX_MODE=EM86XX_MODEID_WITHHOST,$(RMCFLAGS))' ''

COMPILKIND += withdl
lib_ext := so

else # EM86XX_MODEID_WITHHOST

$(error EM86XX_MODE is not set in RMCFLAGS: refer to emhwlib/include/emhwlib_modes.h)

endif # EM86XX_MODEID_WITHHOST
endif # EM86XX_MODEID_STANDALONE

base_dir := ${RUA_DIR}

TARGET_TYPE := EXECUTABLE
include $(base_dir)/scripts/inc.Target

COMPILKIND  += implicit
MAIN_SRC := SmartCardTest.cpp

SRC :=  

lib_list :=  

EXTERNAL_STATIC_LINKS_WITH := $(lib_list:%=$(base_dir)/lib/lib%.$(lib_ext))	\
	$(LINUX_BASE_LIB)/libLinuxBaseLib.so

LINKS_WITH := $(RUA_DIR)/emhwlib_hal/i2c/src/libi2chal.a

ifeq '$(findstring " -g",$(RMCFLAGS))' ''
	RMCFLAGS += -ggdb
endif

ifeq '$(findstring " -g",$(LDFLAGS))' ''
	LDFLAGS += -ggdb
endif

ifeq '$(findstring " -I$(DMS_SMARTCARD_OPERATION_LIB_DIR)/include",$(RMCFLAGS))' ''
	RMCFLAGS += -I$(DMS_SMARTCARD_OPERATION_LIB_DIR)/include
endif

ifeq '$(findstring " -I$(LINUX_BASE_LIB)/Include",$(RMCFLAGS))' ''
	RMCFLAGS += -I$(LINUX_BASE_LIB)/Include
endif

ifeq '$(findstring " -L$(RUA_DIR)/lib",$(LDFLAGS))' ''
	LDFLAGS += -L$(RUA_DIR)/lib
endif

export RMPLATFORM
export RMCFLAGS
export CROSS

USE_STD_LIB := 1

include $(base_dir)/scripts/inc.Makefile

all: $(TARGET)
	cp -fa $(TARGET) /sdb1/xrq/nfs/smp865x/$(TARGET)
	
