SHELL=/bin/sh
CROSS=arm-linux-
CC=$(CROSS)gcc
TARGET_AR=$(CROSS)ar
LD=$(CROSS)ld
TARGET_STRIP=$(CROSS)strip
MAKEFILE_NAME := Makefile
OUT_DIR=Out
OUT_OBJ_DIR := $(OUT_DIR)/Obj

CFLAGS := -shared -fPIC -Wall -IInclude -D_GNU_SOURCE
CPPFLAGS := -fcheck-new
LINKFLAGS= -shared -fPIC -lstdc++

#ifeq '$(findstring " -g",$(CFLAGS))' ''
	CFLAGS += -ggdb
#endif
#ifeq '$(findstring " -g",$(LINKFLAGS))' ''
#	LINKFLAGS += -ggdb
#endif

LIBDIR=.
LIB=
TARGET=libLinuxBaseLib.so

SRC := string_ex.c String.cpp math2.c memory.c DateTime.c FileMutexLocker.cpp dirent2.c	\
	LastCallErr.cpp	\
	LinuxSignalHandler.cpp LinuxSignalHandler_C.cpp EventThread.cpp EventApp.cpp	\
	EventCenter.cpp Mutex2.cpp CrtErr.cpp UnixDatagramSocket.cpp UnixSocket.cpp	\
	LinuxSocket.cpp SignalInfo.cpp Thread.cpp Process.c string2.c StackBufString.cpp	\
	RuaMemory.cpp File.cpp NetIF.c FileCreator.c RefCounter.cpp AtomicOperation.c	\
	MemoryDevice.cpp AutoLock.cpp Exception.cpp InvArgException.cpp BaseErrException.cpp	\
	GenericMsgData.cpp ThreadMessageQueue.cpp EventEx.cpp MsgThread.cpp Object.cpp	\
	LinuxSignalHandlerEx.cpp RegisteredSignalInfo.cpp SignalHandlerItem.cpp NetRouteTable.cpp	\
	inetaddr.c SystemTimezone.cpp CmdLineArgument.cpp UrlInfo.cpp DbgLogSwitch.cpp	\
	GlobalShareMem.cpp ProcessList.cpp DirectoryManage.c unistd2.c	\
	UdpSocket.cpp Socket2.cpp MediaDataPacket.cpp UrlParser.cpp     \
	ThreadMutex.c UniqueId.c ThreadSpecific.c List.c Checksum.c	\
	RecvBuf2.cpp SendBuf2.cpp UrlHandle.c SendBufInStack.cpp RecvBufInStack.cpp	\
	FileCopyOps.cpp storageManage.cpp ThreadData.cpp FdEvtNotifier.cpp	\
	ByteArray2.cpp EventLog.cpp Base64.c MemoryMapFile.cpp GenericTimer.cpp	\
	EncDecrypt_RC4.c ThreadSleep.cpp NetAddrUtils.cpp ErrChkMutex.cpp AutoScopeLock.cpp  \
	
SRC_CPP=$(filter %.cpp, $(SRC))
SRC_C=$(filter %.c, $(SRC))

ifeq '$(findstring " -O",$(CFLAGS))' ''
	CFLAGS += -O2
endif

all:$(OUT_DIR)/$(TARGET)
	@if [ ! -d $(OUT_DIR) ]; then	\
		mkdir $(OUT_DIR);	\
		ProcessExitCode=$$?;	\
		if [ $$ProcessExitCode != 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	cp -fa $(OUT_DIR)/$(TARGET) $(OUT_OBJ_DIR)/;	\
	ProcessExitCode=$$?;	\
	if [ $$ProcessExitCode != 0 ]; then	\
		exit 1;	\
	fi;	\
	$(TARGET_STRIP) --strip-debug $(OUT_DIR)/$(TARGET);	\
	ProcessExitCode=$$?;	\
	if [ $$ProcessExitCode != 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target: $(OUT_DIR)/$(TARGET)"

$(SRC):$(MAKEFILE_NAME)
	touch $(SRC)

$(OUT_OBJ_DIR)/Process.o: override CFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CFLAGS))
$(OUT_OBJ_DIR)/Process.o: override CPPFLAGS := $(filter-out -O -O1 -O2 -O3 , $(CPPFLAGS))

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
#${warning OBJS=$(OBJS)}

$(OUT_DIR)/$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) -o $@ $^
	#$(LD) $(LINKFLAGS) -o $@ $^

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

