CROSS := arm-linux-
CROSS_GNU := arm-linux-gnu-
STRIP := $(CROSS)strip
STRIP := $(shell which "$(STRIP)")
ifeq ($(STRIP),)
  STRIP := $(CROSS_GNU)strip
  STRIP := $(shell which "$(STRIP)")
  #${warning STRIP=$(STRIP)}
endif
#${warning STRIP=$(STRIP)}

all:
	@echo "Usage: make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  InstallToRootfs"

mk:
	@make -C src -f build.mk mk;	\
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
	make -C src -f build.mk InstallToRootfs;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
