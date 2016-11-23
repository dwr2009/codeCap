WORK_DIR=$(shell pwd)
CROSS := arm-linux-
OUT_DIR=$(WORK_DIR)/Out
CC := $(CROSS)gcc
CXX := $(CROSS)g++
LD := $(CROSS)ld
STRIP := $(CROSS)strip
MIBS=host agentx smux \
     ucd-snmp/diskio tcp-mib udp-mib mibII/mta_sendmail \
     ip-mib/ipv4InterfaceTable ip-mib/ipv6InterfaceTable \
     ip-mib/ipAddressPrefixTable/ipAddressPrefixTable \
     ip-mib/ipDefaultRouterTable/ipDefaultRouterTable \
     ip-mib/ipv6ScopeZoneIndexTable ip-mib/ipIfStatsTable \
     sctp-mib rmon-mib etherlike-mib

export CC
export CXX
export LD
export CFLAGS

all:
	@echo "Usage: make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  clean"
	@echo "  InstallToRootfs	;Install to $(NFS_DBG_DIR)"
	
mk:
	@cd src;	\
	make -f Makefile.linux

clean:
	@ \
	cd src; \
	make -f Makefile.linux clean;
	
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
	\
	SRC_FILE="lib/libsnmp++.so.2.0.0";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libsnmp++.so.2.0.0";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target strip: $${DST_FILE}";	\
	${STRIP} --strip-debug "$${DST_FILE}";	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	\
	SRC_FILE="lib/libsnmp++.so.2";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libsnmp++.so.2";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	\
	SRC_FILE="lib/libsnmp++.so";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libsnmp++.so";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	
