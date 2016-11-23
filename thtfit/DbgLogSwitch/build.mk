CROSS := arm-linux-
STRIP := $(CROSS)strip
THIS_FILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

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
    if [ -f "$${SRC_FILE}" ] && ([ ! -e "$${DST_FILE}" ] || [ "$${SRC_FILE}" -nt "$${DST_FILE}" ] || [ "$${SRC_FILE}" -ot "$${DST_FILE}" ]); then \
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
	@echo "  InstallToRootfs"
	
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
	fi;
	$(call CopyStripFile,Out/DbgLogSwitch,$(ROOTFS_DIR)/usr/bin/DbgLogSwitch)
