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

cp -fa Out/rootfs.tgz NetworkUpdate/rootfs/SystemUpgrade/
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Copy(Out/rootfs.tgz=>NetworkUpdate/rootfs/SystemUpgrade/) err ${ProcessExitCode}";		
	ExitProcess 1;
fi

cd NetworkUpdate/rootfs/SystemUpgrade
rm -fr md5sum
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Del(NetworkUpdate/rootfs/SystemUpgrade/md5sum) err ${ProcessExitCode}";		
	ExitProcess 1;
fi
md5sum * > md5sum
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "md5sum(NetworkUpdate/rootfs/SystemUpgrade/*) err ${ProcessExitCode}";		
	ExitProcess 1;
fi

cd "${WORK_DIR}";
cd NetworkUpdate/rootfs/
tar -czf SystemUpgrade.tgz SystemUpgrade
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Tar(NetworkUpdate/rootfs/SystemUpgrade/) err ${ProcessExitCode}";		
	ExitProcess 1;
fi

echo "Target: NetworkUpdate/rootfs/SystemUpgrade.tgz"

cd "${WORK_DIR}";

echo "Success"
