CROSS := arm-linux-
STRIP := $(CROSS)strip

QMAKE := $(QTDIR)/qtbase/bin/qmake

.PHONY: all GenerateMakefile mk InstallToRootfs

help:
	@echo "make -f build.mk <sub_mk>"
	@echo "  all                    make all"
	@echo "  GenerateMakefile       generate Makefile"
	@echo "  mk                     make"
	@echo "  InstallToRootfs        install to $(NFS_DBG_DIR)"

all: GenerateMakefile mk InstallToRootfs

GenerateMakefile:
	@QMAKE=$(QMAKE);	\
	if [ ! -f "$${QMAKE}" ]; then	\
		QMAKE=$(QTHOSTDIR)/bin/qmake;	\
		if [ ! -f "$${QMAKE}" ]; then	\
			echo "Not found qmake";	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "$${QMAKE} -o Makefile QtExt.pro";	\
	$${QMAKE} -o Makefile QtExt.pro;	\
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
	# libQtExt.so.1.0.0;	\
	SRC_FILE="libQtExt.so.1.0.0";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libQtExt.so.1.0.0";	\
	mkdir -p "$${ROOTFS_DIR}/lib";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target strip: $${DST_FILE}";	\
	$(STRIP) --strip-debug $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi	\
	# libQtExt.so;	\
	SRC_FILE="libQtExt.so";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libQtExt.so";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	# libQtExt.so.1;	\
	SRC_FILE="libQtExt.so.1";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libQtExt.so.1";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
