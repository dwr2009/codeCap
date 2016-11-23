CROSS := arm-linux-
STRIP := $(CROSS)strip

WORK_DIR    := $(shell pwd)
THIS_FILE   := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
ROOTFS_DIR  := $(NFS_DBG_DIR)
QMAKE       := $(QTHOSTDIR)/bin/qmake
BROWSER_PRO := QTUpgradeServer.pro

help:
	@echo "Usage: make -f $(THIS_FILE) <sub_mk>"
	@echo "  GenerateMakefile     generate Makefile"
	@echo "  mk                   make"
	@echo "  InstallToRootfs      install to $(ROOTFS_DIR)"

.PHONY:  GenerateMakefile mk InstallToRootfs

all: GenerateMakefile mk InstallToRootfs

GenerateMakefile:
	@QMAKE=$(QMAKE);	\
	if [ ! -f "$${QMAKE}" ]; then	\
		QMAKE=$(QTDIR)/qtbase/bin/qmake;	\
		if [ ! -f "$${QMAKE}" ]; then	\
			echo "Not found $${QMAKE}";	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "$${QMAKE} -o Makefile $(BROWSER_PRO)";	\
	$${QMAKE} -o Makefile $(BROWSER_PRO);	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\

mk:
	make

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
	# QUpgradeServer;	\
	SRC_FILE="QUpgradeServer";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/QUpgradeServer";	\
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
