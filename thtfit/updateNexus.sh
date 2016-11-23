#!/bin/bash

PWD=`pwd`
NEXUS_DIR="${PWD}/../../nexus"
NEXUS_LIB_DIR="${PWD}/../../obj.97439/nexus/bin"
NEXUS_APP_DIR="${PWD}/../target/97439b0.arm-linux.debug/usr/local/lib"


if [ ! -f "${NEXUS_DIR}/build/Makefile" ]; then
   echo "nexus lib is not exist"
fi

if [ ! -d "${NEXUS_LIB_DIR}" ]; then
   if [ ! -f "${NEXUS_LIB_DIR}/libnexus.so" ]; then
	echo "remove current nexus libs"
        rm -rf ${NEXUS_LIB_DIR}/libnexus.so
        rm -rf ${NEXUS_LIB_DIR}/libnxclient.so
		rm -rf ${NEXUS_LIB_DIR}/libnxserver.a
   fi
fi

echo "begin to compile nexus"
cd "${NEXUS_DIR}/build"
make

if [ ! -d "${NEXUS_APP_DIR}" ]; then
   if [ ! -f "${NEXUS_APP_DIR}/libnexus.so" ]; then
	echo "remove current NEXUS APP lib"
        rm -rf ${NEXUS_APP_DIR}/libnexus.so
        rm -rf ${NEXUS_APP_DIR}/libnxclient.so
		rm -rf ${NEXUS_APP_DIR}/libnxserver.a
   fi
fi

echo "copy the lib to APP"
cp -r ${NEXUS_LIB_DIR}/libnexus.so ${NEXUS_APP_DIR}
cp -r ${NEXUS_LIB_DIR}/libnxclient.so ${NEXUS_APP_DIR}
cp -r ${NEXUS_LIB_DIR}/libnxserver.a ${NEXUS_APP_DIR}
