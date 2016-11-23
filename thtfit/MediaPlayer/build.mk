THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
CROSS := arm-linux-
STRIP := $(CROSS)strip

define CopyStripFile
    @ \
    ROOTFS_DIR="$${NFS_DBG_DIR}";   \
    if [ -z "$${ROOTFS_DIR}" ]; then    \
        echo "The env var NFS_DBG_DIR is empty";    \
        exit 1; \
    fi; \
	SrcFile="$(1)"; \
	DstFile="$(2)"; \
    SRC_FILE=$${SrcFile}; \
    DST_FILE=$${DstFile}; \
    if [ ! -e "$${DST_FILE}" ] || [ "$${SRC_FILE}" -nt "$${DST_FILE}" ] || [ "$${SRC_FILE}" -ot "$${DST_FILE}" ]; then \
        DstDir=`dirname "$${DST_FILE}"`; \
        ProcessExitCode=$$?;    \
        if [ $${ProcessExitCode} -ne 0 ]; then  \
           exit 1; \
        fi; \
        if [ ! -d "$${DstDir}" ]; then \
            mkdir -p $${DstDir}; \
            ProcessExitCode=$$?;    \
            if [ $${ProcessExitCode} -ne 0 ]; then  \
                exit 1; \
            fi; \
        fi; \
        echo "Copy: $${SRC_FILE} => $${DST_FILE}";  \
        cp -fa "$${SRC_FILE}" $${DST_FILE}; \
        ProcessExitCode=$$?;    \
        if [ $${ProcessExitCode} -ne 0 ]; then  \
            exit 1; \
        fi; \
        echo "Target strip: $${DST_FILE}";  \
        $(STRIP) --strip-debug $${DST_FILE};    \
        ProcessExitCode=$$?;    \
        if [ $${ProcessExitCode} -ne 0 ]; then  \
            exit 1; \
        fi; \
    fi;
endef

ROOTFS_DIR := $(NFS_DBG_DIR)

all:
	@echo "Usage: make -f $(THIS_FILE) <sub_mk>"
	@echo "  GenerateMakefile"
	@echo "  InstallToRootfs"

GenerateMakefile:
	@QMAKE=$(QTDIR)/qtbase/bin/qmake;	\
	if [ ! -f "$${QMAKE}" ]; then	\
		echo "Not found qmake($${QMAKE})";	\
		exit 1;	\
	fi;	\
	echo "$${QMAKE} -o Makefile MediaPlayer.pro";	\
	$${QMAKE} -o Makefile MediaPlayer.pro;	\
	
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
	# main;	\
	SRC_FILE="MediaPlayer";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/MediaPlayer";	\
	echo "$${SRC_FILE} => $${DST_FILE}";	\
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
	fi;
	$(call CopyStripFile,libNexusExt/Out/libNexusExt.so,$(ROOTFS_DIR)/lib/libNexusExt.so)
