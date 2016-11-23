CROSS := arm-linux-
STRIP := $(CROSS)strip

all:
	@echo "Usage:  make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  clean"
	@echo "  InstallToRootfs"


mk:
	make

clean:
	make clean
	rm -f libSnmpAgent.so.*

InstallToRootfs:
	@ROOTFS_DIR="$${NFS_DBG_DIR}";	\
	if [ -z "$${ROOTFS_DIR}" ]; then	\
		echo "The env var NFS_DBG_DIR is empty";	\
		exit 1;	\
	fi;	\
	echo "ROOTFS_DIR=$${ROOTFS_DIR}"; \
	make InstallToRootfs;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
