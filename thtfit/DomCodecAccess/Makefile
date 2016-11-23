# module defintion including
MAKE_WITH_PRIVATE := 1
#SMART_CARD_MODULE := 1

WORK_DIR := $(shell pwd)
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
DBUS_SRC_DIR := $(WORK_DIR)/../../opensource/dbus/dbus
# libtinyxml
TINYXML_SRC_DIR := $(WORK_DIR)/../tinyxml-2.5.3
DIR_TO_TEST := $(TINYXML_SRC_DIR)
DIR_EXIST = $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DIR_EXIST),0)
  TINYXML_SRC_DIR := $(WORK_DIR)/../tinyxml-2.5.3
  DIR_TO_TEST := $(TINYXML_SRC_DIR)
  DIR_EXIST = $(shell test -d "$(DIR_TO_TEST)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
  ifeq ($(DIR_EXIST),0)
    ${error Not found libtinyxml source dir}
  endif
endif
#${warning TINYXML_SRC_DIR=$(TINYXML_SRC_DIR)}

# libLinuxBaseLib
LINUX_BASE_LIB_DIR := $(WORK_DIR)/../LinuxBaseLib

CFLAGS := -shared -fPIC -Wall -IInclude -I$(DBUS_SRC_DIR) -I$(LINUX_BASE_LIB_DIR)/Include	\
	-I$(TINYXML_SRC_DIR)	\

CPPFLAGS := -fcheck-new
LINKFLAGS= -shared -fPIC

ifeq '$(findstring " -g",$(CFLAGS))' ''
	CFLAGS += -ggdb
endif
ifeq '$(findstring " -g",$(LINKFLAGS))' ''
	LINKFLAGS += -ggdb
endif

LIBDIR = .
#LIB = -ltinyxml
TARGET=libDomCodecAccess.so

SRC := TiXmlDoc2.cpp
#ifeq '$(SMART_CARD_MODULE)' '1'
#  SRC += SmartCard.c
#endif
#ifeq '$(MAKE_WITH_PRIVATE)' '1'
#  ifeq '$(SMART_CARD_MODULE)' '1'
#    SRC += Smp865xSoftFloat.c SoftFloatBridge.c
#  endif
#else
#  PRIVATE_OBJS += Smp865xSoftFloat.o SoftFloatBridge.o
#endif
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
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		echo "MkDir: $(OUT_OBJ_DIR)";	\
		mkdir $(OUT_OBJ_DIR);	\
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "Copy: $(OUT_DIR)/$(TARGET) => $(OUT_OBJ_DIR)/";	\
	cp -fa $(OUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/;	\
	echo "Target strip: $(OUT_DIR)/$(TARGET)";	\
	$(TARGET_STRIP) --strip-debug $(OUT_DIR)/$(TARGET);

$(SRC):$(MAKEFILE_NAME)
	touch $(SRC)

#$(OUT_OBJ_DIR)/Process.o: override CFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CFLAGS))
#$(OUT_OBJ_DIR)/Process.o: override CPPFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CPPFLAGS))
#ifeq '$(findstring -msoft-float,$(CFLAGS))' ''
#  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CFLAGS += -msoft-float
#endif
#ifeq '$(findstring -msoft-float,$(CPPFLAGS))' ''
#  $(OUT_OBJ_DIR)/Smp865xSoftFloat.o: override CPPFLAGS += -msoft-float
#endif

$(OUT_OBJ_DIR)/%.d:%.cpp
	@if [ ! -d $(OUT_OBJ_DIR) ]; then	\
		mkdir -p $(OUT_OBJ_DIR);	\
	fi;	\
	set -e;\
	echo -n "$(OUT_OBJ_DIR)/" > $@;	\
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< >> $@;\
	echo '	$$(CC) $$(CFLAGS) $$(CPPFLAGS) -c -o $$@ $$<' >> $@

$(OUT_OBJ_DIR)/%.d:%.c
	@if [ ! -d $(OUT_OBJ_DIR) ]; then	\
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
	@if [ ! -d $(OUT_DIR) ]; then	\
		echo "MkDir: $(OUT_DIR)";	\
		mkdir $(OUT_DIR);	\
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	if [ ! -d "$(OUT_OBJ_DIR)" ]; then	\
		echo "MkDir: $(OUT_OBJ_DIR)";	\
		mkdir $(OUT_OBJ_DIR);	\
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	$(CC) $(LINKFLAGS) -o $@ $^ $(PRIVATE_OBJS) $(LIB);

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

