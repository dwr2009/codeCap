#!/bin/sh

#--------------------------
#function definitions
#--------------------------
function ExitProcess()
{
	exit $1;
}

# Parse command line arguments
ArgCnt=$#
#echo Args=$@
bCompressRootfs=1
for OneArg in "$@"
do
	if [ ${OneArg} = "-NotCompress" ]; then
		bCompressRootfs=0
	fi
done
#echo bCompressRootfs=${bCompressRootfs}

#--------------------------
#environment variables
#--------------------------
WORK_DIR=${PWD}
ROOTFS_DIR=${NFS_DBG_DIR}
ROOTFS_DIR=`${WORK_DIR}/../CorrectPath.pl ${ROOTFS_DIR}`
ROOTFS_DIR=`${WORK_DIR}/../CorrectPath.pl ${ROOTFS_DIR}`
ROOTFS_DIR=`${WORK_DIR}/../CorrectPath.pl ${ROOTFS_DIR}`
echo "ROOTFS_DIR=${ROOTFS_DIR}"
if [ ! -d "${ROOTFS_DIR}" ]; then
	echo "Not found \"${ROOTFS_DIR}\"";
	ExitProcess 1;
fi
ROOTFS_TEMP_DIR="${WORK_DIR}/Temp/rootfs"

#--------------------------
# Create ROOTFS_TEMP_DI${R} if not exist
#--------------------------
if [ ! -d "${ROOTFS_TEMP_DIR}" ]; then
	mkdir -p ${ROOTFS_TEMP_DIR};
	ProcessExitCode=$?
	if [ ${ProcessExitCode} -ne 0 ]; then
		echo "Failed to create \"${ROOTFS_TEMP_DIR}\"";		
		ExitProcess 1;
	fi
else
	rm -fr ${ROOTFS_TEMP_DIR}/*
	ProcessExitCode=$?
	if [ ${ProcessExitCode} -ne 0 ]; then
		echo "Failed to delete \"${ROOTFS_TEMP_DIR}/*\"";		
		ExitProcess 1;
	fi
	echo -n;
fi

#--------------------------
# Create a directory Out
#--------------------------
if [ ! -d Out ]; then
	mkdir Out;
	ProcessExitCode=$?
	if [ ${ProcessExitCode} -ne 0 ]; then
		echo "Failed to create the dir \"Out\"";		
		ExitProcess 1;
	fi
fi

#--------------------------
# Copy rootfs/*
#--------------------------
echo "Copy: ${ROOTFS_DIR}/* => ${ROOTFS_TEMP_DIR}/"
cp -fra ${ROOTFS_DIR}/* ${ROOTFS_TEMP_DIR}/;
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Failed to create the dir \"Out\"";		
	ExitProcess 1;
fi

#--------------------------
# Delete unnecessary files
#--------------------------
while read OneLine
do
	#echo "OneLine=${OneLine}"
	if [ ! -z "${OneLine}" ]; then
		echo "Delete: ${ROOTFS_TEMP_DIR}/${OneLine}"
		rm -fr "${ROOTFS_TEMP_DIR}/${OneLine}";
		ProcessExitCode=$?
		if [ ${ProcessExitCode} -ne 0 ]; then
			echo "Failed to delete ${ROOTFS_TEMP_DIR}/${OneLine}";		
			ExitProcess 1;
		fi
		rm -fr ${ROOTFS_TEMP_DIR}/${OneLine};
		ProcessExitCode=$?
		if [ ${ProcessExitCode} -ne 0 ]; then
			echo "Failed to delete ${ROOTFS_TEMP_DIR}/${OneLine}";		
			ExitProcess 1;
		fi
	fi
done < MP7xx_FilesToDelete.txt

#--------------------------
# Strip dynamical library(*.so)
#--------------------------
echo "StripDebug all .so files";
find ${ROOTFS_TEMP_DIR} -name "*.so*" | xargs arm-linux-strip --strip-debug

#--------------------------
# Strip binary files(executable)
#--------------------------
while read OneLine
do
	#echo "OneLine=${OneLine}"
	if [ ! -z "${OneLine}" ]; then
		if [ -f "${ROOTFS_TEMP_DIR}/${OneLine}" ]; then
			echo "StripAll: ${ROOTFS_TEMP_DIR}/${OneLine}"
			arm-linux-strip --strip-all "${ROOTFS_TEMP_DIR}/${OneLine}";
			ProcessExitCode=$?
			if [ ${ProcessExitCode} -ne 0 ]; then
				echo "Failed to StripAll ${ROOTFS_TEMP_DIR}/${OneLine}";		
				ExitProcess 1;
			fi
		fi
	fi
done < MP7xx_FilesToStripAll.txt


#--------------------------
# Change file permissions
#--------------------------

if [ ${bCompressRootfs} -ne 0 ]; then
	#--------------------------
	#Create a tar package
	#--------------------------
	echo "Creating a compressed package..."
	cd "${ROOTFS_TEMP_DIR}";
	tar -czf "${WORK_DIR}/Out/rootfs.tgz" .;
	ProcessExitCode=$?;
	if [ ${ProcessExitCode} -ne 0 ]; then
		echo "Failed to create a tar package \"${WORK_DIR}/Out/rootfs.tgz\"";		
		ExitProcess 1;
	fi
fi

cd "${WORK_DIR}";

echo "Success"
