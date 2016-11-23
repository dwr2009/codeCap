TEMPLATE = lib
TARGET = SnmpAgent
VERSION = 1.0
CONFIG += release
DEPENDPATH += . GeneratedFiles
isEmpty(QTDIR) {
	QTDIR = $(QT_DIR)
}

# Qt base include
INCLUDEPATH += $${QTDIR}/include
INCLUDEPATH += . $$quote($(PWD)/../LinuxBaseLib/Include)
LINUXBASELIB_DIR = $$quote($$(PWD)/../LinuxBaseLib/Out)

# DBus
INCLUDEPATH += $${QTDIR}/../dbus/out/usr/local/include/dbus-1.0
QT_LIB_DIR = $${QTDIR}/../dbus/out/lib

# net-snmp
INCLUDEPATH += $$quote($(PWD)/../net-snmp/include)
LIB_NETSNMP_DIR = $$quote($$(PWD)/../net-snmp/out/usr/local/lib)

# source files
HEADERS +=
SOURCES += data_access/network.cpp

QT += core dbus

LIBS += -L$${LINUXBASELIB_DIR} \
	-lLinuxBaseLib \
	-ldbus-1 \
	-L$${LIB_NETSNMP_DIR} -lnetsnmp -lnetsnmpmibs -lnetsnmpagent -lnetsnmphelpers
