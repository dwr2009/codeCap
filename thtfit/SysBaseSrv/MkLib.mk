WORK_DIR=$(shell pwd)
-include ../common.inc

CROSS=arm-linux-
CC=$(CROSS)gcc
TARGET_AR=$(CROSS)ar
LD=$(CROSS)ld
TARGET_STRIP=$(CROSS)strip
THISFILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
#${warning THISFILE=$(THISFILE)}
OUTPUT_DIR=$(WORK_DIR)/Out
OUT_OBJ_DIR := $(OUTPUT_DIR)/Obj

CFLAGS := -shared -fPIC -Wall -IInclude -I$(COMMON_INC_DIR) -I$(COMMON_LIB_DIR)/libc2	\
	-I$(COMMON_LIB_DIR) -I$(ZLIB_DIR) -I$(COMMON_LIB_DIR)/Thread	\
	-I$(COMMON_LIB_DIR)/String -I$(COMMON_LIB_DIR)/NetLayer -I$(COMMON_LIB_DIR)/DebugConfig	\
	-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE	\
	-D_BSD_SOURCE -D_XOPEN_SOURCE -DUNIX -D_GNU_SOURCE \
	-I${WORK_DIR}/../../../nexus/nxclient/include/

CPPFLAGS := -fcheck-new
LINKFLAGS= -shared -fPIC -lstdc++

ifeq '$(findstring " -g",$(CFLAGS))' ''
	CFLAGS += -ggdb
endif
ifeq '$(findstring " -g",$(LINKFLAGS))' ''
	LINKFLAGS += -ggdb
endif

LIBDIR=.
LIB=
TARGET=libSysBaseSrv.so

SRC := SysPropOpsClient.cpp

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
		LINKFLAGS += -L$(LINUXBASELIB_DIR)/Out
	endif
	ifeq '$(findstring " -lLinuxBaseLib",$(LDFLAGS))' ''
		LINKFLAGS += -lLinuxBaseLib
	endif
else
	$(error Not found the directory LinuxBaseLib)
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
	iProcessExitCode=$$?;	\
	if [ $${iProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
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
${warning OBJS=$(OBJS)}

$(OUTPUT_DIR)/$(TARGET): $(OBJS)
	@echo "$@, <== $<   |  $(OBJS)"
	$(CC) $(LINKFLAGS) -o $@ $^ $(PRIVATE_OBJS)

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
