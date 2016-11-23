CROSS := arm-linux-
STRIP := $(CROSS)strip

all:
	@echo "Usage: make -f build.mk <sub_mk>"
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
	fi;	\
	# GpioBtnInput;	\
	SRC_FILE="Out/GpioBtnInput";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/GpioBtnInput";	\
	echo "$${SRC_FILE} => $${DST_FILE}";	\
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
