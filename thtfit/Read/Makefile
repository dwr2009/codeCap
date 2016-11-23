SHELL := /bin/sh
PWD := $(shell pwd)
WORK_DIR := $(PWD)
CROSS_COMPILE := arm-linux-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)gcc
TARGET_STRIP := ${CROSS_COMPILE}strip
THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
OUT_DIR := Out

define CFLAGS_add
ifeq '$(findstring "$(1)",$(CFLAGS))' ''
	CFLAGS := $(CFLAGS) $(1)
endif
endef

define LDFLAGS_add
ifeq '$(findstring "$(1)",$(LDFLAGS))' ''
	LDFLAGS := $(LDFLAGS) $(1)
endif
endef

$(eval $(call CFLAGS_add,-O2))
$(eval $(call CFLAGS_add,-ggdb3))

$(eval $(call LDFLAGS_add,-lpthread))
$(eval $(call LDFLAGS_add,-lstdc++))
$(eval $(call LDFLAGS_add,-lrt))
$(eval $(call LDFLAGS_add,-ggdb3))

TARGET := 
TARGET := $(strip $(TARGET))
MAIN_SRC := Read.cpp
MAIN_SRC := $(strip $(MAIN_SRC))
SRC := 
SRC_CPP := $(filter %.cpp, $(SRC)) $(filter %.cpp, $(MAIN_SRC))
SRC_C := $(filter %.c, $(SRC)) $(filter %.c, $(MAIN_SRC))
SRC := $(SRC_C) $(SRC_CPP)

TARGET_NAME = $(TARGET)
ifeq '$(TARGET_NAME)' ''
	TARGET_NAME = $(basename $(MAIN_SRC))
endif

# Find the path of libLinuxBaseLib
LinuxBaseLib_DIR := "$(WORK_DIR)/../LinuxBaseLib"
DirToTest := $(LinuxBaseLib_DIR)
DirToTest_EXIST = $(shell test -d "$(DirToTest)";if [ $$? -eq 0 ]; then echo 1; else echo 0; fi)
ifeq ($(DirToTest_EXIST),1)
    ifeq '$(findstring "-I$(LinuxBaseLib_DIR)/Include",$(CFLAGS))' ''
        CFLAGS := $(CFLAGS) -I$(LinuxBaseLib_DIR)/Include
    endif
	ifeq '$(findstring " -L$(LinuxBaseLib_DIR)/Out",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -L$(LinuxBaseLib_DIR)/Out
	endif
	ifeq '$(findstring "-lLinuxBaseLib",$(LDFLAGS))' ''
		LDFLAGS := $(LDFLAGS) -lLinuxBaseLib
	endif
else
	$(error Not found the directory libLinuxBaseLib)
endif

OBJ_FILES := $(addprefix $(OUT_DIR)/,$(SRC_CPP:.cpp=.o) $(SRC_C:.c=.o))
#${warning OBJ_FILES=$(OBJ_FILES)}

define ProduceSrcDepends
#${warning SrcFile=$(1)}
#${warning DepFile=$(OUT_DIR)/$(basename $(1)).d}
$(OUT_DIR)/$(basename $(1)).d: $(1)
	@ \
	if [ ! -d "$(OUT_DIR)" ]; then \
		echo "MkDir $(OUT_DIR)"; \
		mkdir -p $(OUT_DIR); \
		ProcessExitCode=$$$$?; \
		if [ $$$${ProcessExitCode} -ne 0 ]; then \
			exit $$$${ProcessExitCode}; \
		fi; \
	fi;
	$(CXX) -MM $(CFLAGS) -Wp,-MT,$(OUT_DIR)/$(basename $(1)).o $(1) > $$@; \
    ProcessExitCode=$$$$?; \
    if [ $$$${ProcessExitCode} -ne 0 ]; then \
		rm -f $$@; \
		exit 1; \
	fi; \
	echo '	$$(CXX) $$(CFLAGS) -c -o $(OUT_DIR)/$(basename $(1)).o $(1)' >> $$@;
	
endef

all: $(OUT_DIR)/$(TARGET_NAME)

$(SRC): $(THIS_FILE)
	touch $(SRC)

$(eval $(foreach OneSrcFile,$(SRC_C),$(call ProduceSrcDepends,$(OneSrcFile))))
$(eval $(foreach OneSrcFile,$(SRC_CPP),$(call ProduceSrcDepends,$(OneSrcFile))))

DependFiles := $(addprefix $(OUT_DIR)/,$(SRC_CPP:.cpp=.d) $(SRC_C:.c=.d))
#${warning DependFiles=$(DependFiles)}

-include $(DependFiles)

$(OUT_DIR)/$(TARGET_NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) 	\
	-Wl,--start-group	\
	$^ 	\
	-Wl,--end-group	\
	-o $@	
	
.PHONY : clean
clean:
	rm -fr *.o *.obj $(TARGET_NAME) *.d Out/Obj/*.d Out/*
	
.PHONY : remake
remake:
	make clean -f $(THIS_FILE)
	make -f $(THIS_FILE)
	
