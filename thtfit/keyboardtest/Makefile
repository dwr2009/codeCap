SHELL := /bin/sh
PWD := $(shell pwd)
WORK_DIR := $(PWD)
CROSS_COMPILE := arm-linux-
CC := $(CROSS_COMPILE)gcc
TARGET_STRIP := ${CROSS_COMPILE}strip
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
$(eval $(call CFLAGS_add,-ggdb))

$(eval $(call LDFLAGS_add,-lpthread))
#$(eval $(call LDFLAGS_add,-lstdc++))
$(eval $(call LDFLAGS_add,-lrt))
$(eval $(call LDFLAGS_add,-ggdb))

TARGET := keyboardtest
TARGET := $(strip $(TARGET))
MAIN_SRC := event_device_test.c
MAIN_SRC := $(strip $(MAIN_SRC))
SRC :=  
SRC_CPP := $(filter %.cpp, $(SRC)) $(filter %.cpp, $(MAIN_SRC))
SRC_C := $(filter %.c, $(SRC)) $(filter %.c, $(MAIN_SRC))
SRC := $(SRC_C) $(SRC_CPP)


$(OUT_DIR)/$(TARGET):$(MAIN_SRC)
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $<


.PHONY : clean
clean:
	rm -fr *.o *.obj $(TARGET_NAME) *.d Out/Obj/*.d Out/*

