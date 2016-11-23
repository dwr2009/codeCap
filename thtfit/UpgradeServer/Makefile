SHELL := /bin/sh
CROSS_COMPILE := arm-linux-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
STRIP := $(CROSS_COMPILE)strip
LD := $(CROSS_COMPILE)ld
THISFILE := Makefile
PWD := $(shell pwd)
WORK_DIR := $(PWD)
NEXUS_TOP ?= $(PRJ_TOP_DIR)/nexus

include $(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build/platform_app.inc

LDFLAGS += $(NEXUS_LDFLAGS) $(NEXUS_LD_LIBRARIES) -lm

CFLAGS += $(NEXUS_CFLAGS) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES))

CFLAGS := $(CFLAGS) -Wall -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE 

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

ZLIB_STATIC := 
BASE_LIB_STATIC := 
LIB = 
TARGET := 
MAIN_SRC := UpgradeServer.cpp
SRC := bfont.c
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
	
