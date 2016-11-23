THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
CROSS := arm-linux-
STRIP := $(CROSS)strip

all:
	@echo "make -f $(THIS_FILE) <sub_mk>"
	@echo "  GenerateMakefile"
	@echo "  InstallToRootfs"
	
GenerateMakefile:
	@QMAKE=$(QTDIR)/qtbase/bin/qmake;	\
	if [ ! -f "$${QMAKE}" ]; then	\
		QMAKE=$(QTDIR)/bin/qmake;	\
		if [ ! -f "$${QMAKE}" ]; then	\
			echo "Not found qmake";	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "$${QMAKE} -o Makefile SystemService.pro";	\
	$${QMAKE} -o Makefile SystemService.pro;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\

InstallToRootfs:
	@ROOTFS_DIR="$${NFS_DBG_DIR}";	\
	if [ -z "$${ROOTFS_DIR}" ]; then	\
		echo "The env var NFS_DBG_DIR is empty";	\
		exit 1;	\
	fi;	\
	echo "ROOTFS_DIR=$${ROOTFS_DIR}";	\
	# Create $(ROOTFS_DIR)/MP7XX if needed;	\
	if [ ! -d "$${ROOTFS_DIR}/MP7XX" ]; then	\
		echo "Create the directory $${ROOTFS_DIR}/MP7XX";	\
		mkdir -p "$${ROOTFS_DIR}/MP7XX";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "copy libNexusForSysService/Out/libNexusForSysService.so ==> $${ROOTFS_DIR}/lib/"; \
	cp libNexusForSysService/Out/libNexusForSysService.so $${ROOTFS_DIR}/lib/; \
	# SystemService;	\
	SRC_FILE="SystemService";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/SystemService";	\
	mkdir -p "$${ROOTFS_DIR}/MP7XX";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target strip: $${DST_FILE}";	\
	$(STRIP) --strip-all $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi	\
