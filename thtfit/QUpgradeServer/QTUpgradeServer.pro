######################################################################
# Automatically generated by qmake (2.01a) ??? ??? 29 11:47:12 2011
######################################################################

TEMPLATE = app
TARGET = QUpgradeServer
CONFIG += release
DEPENDPATH += . GeneratedFiles
isEmpty(QTDIR) {
	QTDIR = $(QT_DIR)
}
# Qt base include
INCLUDEPATH += $${QTDIR}/include
INCLUDEPATH += . ../LinuxBaseLib/Include
LINUXBASELIB_DIR = ../LinuxBaseLib/Out
#SYSTEM_CMD=$$quote(echo LINUXBASELIB_DIR=$${LINUXBASELIB_DIR})
#system($${SYSTEM_CMD})
#QMAKE_RPATHDIR += $$quote($${LINUXBASELIB_DIR})

# LinuxBaseLib
LinuxBaseLib_DIR = ../LinuxBaseLib
exists($${LinuxBaseLib_DIR}) {
        #SYSTEM_CMD=$$quote(echo LinuxBaseLib_DIR=$${LinuxBaseLib_DIR})
        #system($${SYSTEM_CMD})
        INCLUDEPATH += $${LinuxBaseLib_DIR}/Include
        #SYSTEM_CMD=$$quote(echo INCLUDEPATH=$${INCLUDEPATH})
        #system($${SYSTEM_CMD})
        contains(LIBS, $${LinuxBaseLib_DIR}/Out) {
        } else {
                LIBS += -L $${LinuxBaseLib_DIR}/Out
        }
        contains(LIBS, -lLinuxBaseLib) {
        } else {
                LIBS += -lLinuxBaseLib
        }
} else {
        error("Not found libLinuxBaseLib")
}

# Input
HEADERS += QUpgradeViewServer.h
SOURCES += QUpgradeViewServer.cpp
QT +=core \
     gui \
     dbus \
     xml \
     webkit \	
     webkitwidgets \

LIBS += -L$${LINUXBASELIB_DIR} -lLinuxBaseLib	\

ICON = qml.icns

contains(INCLUDEPATH, $${QTDIR}/qtbase/include) {
} else {
	INCLUDEPATH += $${QTDIR}/qtbase/include
}

contains(INCLUDEPATH, $${QTDIR}/qtwebkit/include) {
} else {
	INCLUDEPATH += $${QTDIR}/qtwebkit/include
}

contains(INCLUDEPATH, $${QTDIR}/qtwebkit/include/QtWebKit) {
} else {
	INCLUDEPATH += $${QTDIR}/qtwebkit/include/QtWebKit
}

contains(LIBS, $${QTDIR}/qtwebkit/lib) {
} else {
	LIBS += -L$${QTDIR}/qtwebkit/lib
}

contains(QMAKE_CXXFLAGS, -ggdb) {
} else {
	QMAKE_CXXFLAGS += -ggdb
}

contains(QMAKE_LFLAGS, -ggdb) {
} else {
	QMAKE_LFLAGS += -ggdb
}

#message(LIBS=$${LIBS})
