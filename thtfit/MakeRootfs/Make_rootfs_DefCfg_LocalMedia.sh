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
echo "Create ${WORK_DIR}/Out/rootfs.tgz"
cd "${ROOTFS_TEMP_DIR}";
tar --exclude=./MP7XX/Config/* --exclude=./MP7XX/LocalMedia/* -czf "${WORK_DIR}/Out/rootfs.tgz" .;
ProcessExitCode=$?;
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Failed to create a tar package \"${WORK_DIR}/Out/rootfs.tgz\"";		
	ExitProcess 1;
fi
cd "${WORK_DIR}";

# Mp7xxDefCfg.tgz
DST_FILE="${WORK_DIR}/Out/Mp7xxDefCfg.tgz"
echo "Create ${DST_FILE}"
cd "${ROOTFS_TEMP_DIR}";
tar -czf "${DST_FILE}" ./MP7XX/Config/*;
ProcessExitCode=$?;
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Failed to create a tar package \"${DST_FILE}\"";		
	ExitProcess 1;
fi
cd "${WORK_DIR}";

# Mp7xxLocalMedia.tgz
DST_FILE="${WORK_DIR}/Out/Mp7xxLocalMedia.tgz"
echo "Create ${DST_FILE}"
cd "${ROOTFS_TEMP_DIR}";
tar -czf "${DST_FILE}" ./MP7XX/LocalMedia/*;
ProcessExitCode=$?;
if [ ${ProcessExitCode} -ne 0 ]; then
	echo "Failed to create a tar package \"${DST_FILE}\"";		
	ExitProcess 1;
fi
cd "${WORK_DIR}";

echo "Success"
