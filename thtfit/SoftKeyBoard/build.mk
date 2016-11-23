CROSS := arm-linux-
STRIP := $(CROSS)strip
QMAKE := $(QTDIR)/qtbase/bin/qmake

all:
	@echo "make -f build.mk <sub_mk>"
	@echo "  GenerateMakefile"
	@echo "  mk"
	@echo "  InstallToRootfs"
	
GenerateMakefile:
	@QMAKE=$(QMAKE);	\
	if [ ! -f "$${QMAKE}" ]; then	\
		QMAKE=$(QTHOSTDIR)/bin/qmake;	\
		if [ ! -f "$${QMAKE}" ]; then	\
			echo "Not found qmake";	\
			exit 1;	\
		fi;	\
	fi;	\
	echo "$${QMAKE} -o Makefile libime_english.pro";	\
	$${QMAKE} -o Makefile libime_english.pro;	\

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
	# Create the dir /plugins/inputmethods if needed;	\
	if [ ! -d "$${ROOTFS_DIR}/plugins/inputmethods" ]; then	\
		mkdir -p "$${ROOTFS_DIR}/plugins/inputmethods";	\
	fi;	\
	# Del "$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1.0.0";	\
	DST_FILE="$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1.0.0";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${DST_FILE}";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Del "$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so";	\
	DST_FILE="$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${DST_FILE}";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Del "$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1";	\
	DST_FILE="$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${DST_FILE}";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Del "$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1.0";	\
	DST_FILE="$${ROOTFS_DIR}/plugins/inputmethods/libime_english.so.1.0";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${DST_FILE}";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Out/libime_english.so.1.0.0;	\
	DST_FILE="$${ROOTFS_DIR}/lib/libime_english.so.1.0.0";	\
	if [ -f "$${ROOTFS_DIR}/lib/libime_english.so.1.0.0" -o -h "$${ROOTFS_DIR}/lib/libime_english.so.1.0.0" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${SRC_FILE}" $${DST_FILE};	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Out/libime_english.so;	\
	SRC_FILE="Out/libime_english.so";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libime_english.so";	\
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
	# Out/libime_english.so.1;	\
	DST_FILE="$${ROOTFS_DIR}/lib/libime_english.so.1";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${SRC_FILE}" $${DST_FILE};	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Out/libime_english.so.1.0;	\
	DST_FILE="$${ROOTFS_DIR}/lib/libime_english.so.1.0";	\
	if [ -f "$${DST_FILE}" -o -h "$${DST_FILE}" ]; then	\
		echo "Del: $${DST_FILE}";	\
		rm -f "$${SRC_FILE}" $${DST_FILE};	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Arabic.qm;	\
	SRC_FILE="Arabic.qm";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/Arabic.qm";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	# English.qm;	\
	SRC_FILE="English.qm";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/English.qm";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	# Spanish.qm;	\
	SRC_FILE="Spanish.qm";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/Spanish.qm";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	# py.txt;	\
	SRC_FILE="py.txt";	\
	DST_FILE="$${ROOTFS_DIR}/MP7XX/py.txt";	\
	echo "Copy: $${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
