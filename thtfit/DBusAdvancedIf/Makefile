# module defintion including
MAKE_WITH_PRIVATE := 1
#SMART_CARD_MODULE := 1

WORK_DIR := $(shell pwd)


OPENSOURCE ?= $(WORK_DIR)/../../opensource


-include ${OPENSOURCE}/dbus/libdbus_ver.inc


SHELL=/bin/sh
CROSS=arm-linux-
CC=$(CROSS)gcc
TARGET_AR=$(CROSS)ar
LD=$(CROSS)ld
TARGET_STRIP=$(CROSS)strip
MAKEFILE_NAME := Makefile
OUT_DIR=Out
OUT_OBJ_DIR := $(OUT_DIR)/Obj

# libdbus
DBUS_SRC_DIR := $(WORK_DIR)/../../opensource/dbus-1.8.0/

# libLinuxBaseLib
LINUX_BASE_LIB_DIR := $(WORK_DIR)/../LinuxBaseLib

CFLAGS := -shared -fPIC -Wall -IInclude -I$(DBUS_SRC_DIR) -I$(LINUX_BASE_LIB_DIR)/Include	\
	-I$(COMMON_LIB_DIR) -I$(ZLIB_DIR) -I$(COMMON_LIB_DIR)/Thread	\
	-I$(COMMON_LIB_DIR)/String -I$(COMMON_LIB_DIR)/NetLayer -I$(COMMON_LIB_DIR)/DebugConfig	\
	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE	\
	-D_BSD_SOURCE -D_XOPEN_SOURCE -DUNIX -D_GNU_SOURCE $(LIBDBUS_CFLAGS)
CPPFLAGS := -fcheck-new $(LIBDBUS_CFLAGS)
LINKFLAGS= -shared -fPIC $(LIBDBUS_LDFLAGS) -lstdc++


# Find the path of libLinuxBaseLib
LINUXBASELIB_DIR := "$(WORK_DIR)/../LinuxBaseLib"
LINUXBASELIB_DIR_EXIST = $(shell test -d "$(LINUXBASELIB_DIR)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
#$(warning LINUXBASELIB_DIR_EXIST=$(LINUXBASELIB_DIR_EXIST))
ifeq ($(LINUXBASELIB_DIR_EXIST),1)
    ifeq '$(findstring "-I$(LINUXBASELIB_DIR)/Include",$(CFLAGS))' ''
        CFLAGS += -I$(LINUXBASELIB_DIR)/Include
    endif
	ifeq '$(findstring " -L$(LINUXBASELIB_DIR)/Out",$(LDFLAGS))' ''
		LINKFLAGS += -L$(LINUXBASELIB_DIR)/Out
	endif
	ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
		LINKFLAGS += -lLinuxBaseLib
	endif
else
    $(error Not found the directory LinuxBaseLib)
endif



ifeq '$(findstring " -g",$(CFLAGS))' ''
	CFLAGS += -ggdb
endif
ifeq '$(findstring " -g",$(LINKFLAGS))' ''
	LINKFLAGS += -ggdb
endif

LIBDIR=.
LIB=
TARGET=libDBusAdvancedIf.so

SRC := DBusParameter.cpp DBusParameterList.cpp DBusMessage.cpp	\
	DbusConnection.cpp
ifeq '$(SMART_CARD_MODULE)' '1'
  SRC += SmartCard.c
endif
ifeq '$(MAKE_WITH_PRIVATE)' '1'
  ifeq '$(SMART_CARD_MODULE)' '1'
    SRC += Smp865xSoftFloat.c SoftFloatBridge.c
  endif
else
  PRIVATE_OBJS += Smp865xSoftFloat.o SoftFloatBridge.o
endif
#$(wildcard *.c *.cpp)

SRC_CPP=$(filter %.cpp, $(SRC))
SRC_C=$(filter %.c, $(SRC))

ifeq '$(findstring " -O",$(CFLAGS))' ''
	CFLAGS += -O2
endif

all:$(OUT_DIR)/$(TARGET)
	@if [ ! -d $(OUT_DIR) ]; then	\
		echo "MkDir: $(OUT_DIR)";	\
		mkdir $(OUT_DIR);	\
	fi;	\
	echo "Copy: $(OUT_DIR)/$(TARGET) => $(OUT_OBJ_DIR)/";	\
	cp -fa $(OUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/;	\
	echo "Target strip: $(OUT_DIR)/$(TARGET)";	\
	$(TARGET_STRIP) --strip-debug $(OUT_DIR)/$(TARGET);

$(SRC):$(MAKEFILE_NAME)
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
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< >> $@;\
	echo '	$$(CC) $$(CFLAGS) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/%.d:%.c
	if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		mkdir -p $(OUT_OBJ_DIR);	\
	fi;	\
	set -e;\
	echo -n "$(OUT_OBJ_DIR)/" > $@;	\
	$(CC) -MM $(CFLAGS) $< >> $@;\
	echo '	$$(CC) $$(CFLAGS) -c -o $$@ $$<' >> $@

DEPEND_FILES := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d))

include $(DEPEND_FILES)

OBJS := $(addprefix $(OUT_OBJ_DIR)/,$(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o))
#${warning OBJS=$(OBJS)}

$(OUT_DIR)/$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $^ $(PRIVATE_OBJS)

.PHONY : clean
clean:
	rm -fr $(OUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/$(TARGET) $(OBJS) $(DEPEND_FILES) \
		$(OUT_OBJ_DIR)/*.d $(OUT_DIR)
	
.PHONY : remake
remake:
	make clean
	make
	
.PHONY:cleanall
cleanall:clean

