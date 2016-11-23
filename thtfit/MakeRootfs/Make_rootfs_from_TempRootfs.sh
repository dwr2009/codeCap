#!/bin/sh

#--------------------------
#function definitions
#--------------------------
function ExitProcess()
{
	exit $1;
}

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
cd "${WORK_DIR}";

echo "Success"
