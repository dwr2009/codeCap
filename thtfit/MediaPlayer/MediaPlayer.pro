TEMPLATE = app
TARGET = MediaPlayer
CONFIG += release
DEPENDPATH += . GeneratedFiles
MOC_DIR += build
OBJECTS_DIR += build
UI_DIR += build
PRJ_TOP_DIR = $(PRJ_TOP_DIR)
isEmpty(QTDIR) {
	QTDIR = $(QT_DIR)
}
exists($(QTDIR)) {
	INCLUDEPATH += $(QTDIR)/qtbase/include
} else {
	error("QTDIR($${QTDIR}) not exist.")
}
#message($${PWD})
WorkDir = $${PWD}
#message($${WorkDir})
QT += core widgets dbus network

DEFINES += ENABLE_DTV 

contains(QMAKE_CXXFLAGS, -ggdb) {
} else {
	QMAKE_CXXFLAGS += -ggdb
}

contains(QMAKE_LFLAGS, -ggdb) {
} else {
	QMAKE_LFLAGS += -ggdb
}

HEADERS = MediaPlayerApp.h MediaSrv.h TransactionMsgData.h MPGenericMsgId.h \
	MediaPlayerAppIf.h NexusPlayer.h SerialComPort.h TransactionMsgId.h \
    MediaPlayerAppDef.h VideoSync.h CnkB85Lcm.h DbgLogSwitchDef.h
SOURCES = Main.cpp MediaPlayerApp.cpp MediaSrv.cpp TransactionMsgData.cpp \
	MediaPlayerAppIf.cpp NexusPlayer.cpp CnkB85Lcm.cpp DbgLogSwitchDef.cpp\
	SerialComPort.cpp VideoSync.cpp

RESOURCES = 



#work dir includes
contains(INCLUDEPATH, $${WorkDir}/Api) {
} else {
	INCLUDEPATH += $${WorkDir}/Api
}
#message($${INCLUDEPATH})

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

# glib
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
	glib_DIR = $${WorkDir}/../../opensource/glib/glib-2.38.2
} else {
	glib_DIR = $(PRJ_TOP_DIR)/AppLibs/opensource/glib/glib-2.38.2
}
#message($${glib_DIR})
exists($${glib_DIR}) {
	contains(INCLUDEPATH, $${glib_DIR}/glib) {
	} else {
		INCLUDEPATH += $${glib_DIR}/glib
	}
	contains(INCLUDEPATH, $${glib_DIR}) {
	} else {
		INCLUDEPATH += $${glib_DIR}
	}
	contains(INCLUDEPATH, $${glib_DIR}/gmodule) {
	} else {
		INCLUDEPATH += $${glib_DIR}/gmodule
	}
} else {
	error("Not found glib($${glib_DIR})")
}

# libGlobalStatus
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
	libGlobalStatus_DIR = $${WorkDir}/../libGlobalStatus
} else {
	libGlobalStatus_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/libGlobalStatus
}
#message($${libGlobalStatus_DIR})
exists($${libGlobalStatus_DIR}) {
	contains(INCLUDEPATH, $${libGlobalStatus_DIR}/Api) {
	} else {
		INCLUDEPATH += $${libGlobalStatus_DIR}/Api
	}
	contains(LIBS, -L$${libGlobalStatus_DIR}/Out) {
	} else {
		LIBS += -L$${libGlobalStatus_DIR}/Out
	}	
	contains(LIBS, -lGlobalStatus) {
	} else {
		LIBS += -lGlobalStatus
	}	
} else {
	error("Not found libGlobalStatus($${libGlobalStatus_DIR})")
}

# DomCodecAccess
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        DomCodecAccess_DIR = $${WorkDir}/../DomCodecAccess
} else {
        DomCodecAccess_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/DomCodecAccess
}
#message($${DomCodecAccess_DIR})
exists($${DomCodecAccess_DIR}) {
        contains(INCLUDEPATH, $${DomCodecAccess_DIR}) {
        } else {
                INCLUDEPATH += $${DomCodecAccess_DIR}
        }
        contains(LIBS, -L$${DomCodecAccess_DIR}/Out) {
        } else {
                LIBS += -L$${DomCodecAccess_DIR}/Out
        }
        contains(LIBS, -lDomCodecAccess) {
        } else {
                LIBS += -lDomCodecAccess
        }
} else {
        error("Not found libGlobalStatus($${libGlobalStatus_DIR})")
}


# tinyxml-2.5.3
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        tinyxml_DIR = $${WorkDir}/../tinyxml-2.5.3
} else {
        tinyxml_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/tinyxml-2.5.3
}
#message($${tinyxml_DIR})
exists($${tinyxml_DIR}) {
        contains(INCLUDEPATH, $${tinyxml_DIR}) {
        } else {
                INCLUDEPATH += $${tinyxml_DIR}
        }
        contains(LIBS, -L$${tinyxml_DIR}) {
        } else {
                LIBS += -L$${tinyxml_DIR}
        }
        contains(LIBS, -ltinyxml) {
        } else {
                LIBS += -ltinyxml
        }
} else {
        error("Not found libGlobalStatus($${libGlobalStatus_DIR})")
}

# SysBaseSrv
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        SysBaseSrv_DIR = $${WorkDir}/../SysBaseSrv
} else {
        SysBaseSrv_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/SysBaseSrv
}
#message($${SysBaseSrv_DIR})
exists($${SysBaseSrv_DIR}) {
        contains(INCLUDEPATH, $${SysBaseSrv_DIR}) {
        } else {
                INCLUDEPATH += $${SysBaseSrv_DIR}
				INCLUDEPATH += ../../../nexus/nxclient/include/
				INCLUDEPATH += $${WorkDir}/../libMp7xxCommon/Api
				INCLUDEPATH += $${WorkDir}/../WebBrowser/Api
				INCLUDEPATH += $${WorkDir}/../mp7xxTunerMn88436/include
				INCLUDEPATH += $${WorkDir}/../mp7xxTunerMn88436/baseIfce
        }
        contains(LIBS, -L$${SysBaseSrv_DIR}/Out) {
        } else {
                LIBS += -L$${SysBaseSrv_DIR}/Out
		LIBS += -L$${WorkDir}/../mp7xxTunerMn88436/src/Out
        }
        contains(LIBS, -lSysBaseSrv) {
        } else {
                LIBS += -lSysBaseSrv
		LIBS += -lDMDTuner
        }
} else {
        error("Not found libSysBaseSrv($${libSysBaseSrv_DIR})")
}
# SystemService
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        SystemService_DIR = $${WorkDir}/../SystemService
} else {
        SystemService_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/SystemService
}
#message($${SystemService_DIR})
exists($${SystemService_DIR}) {
        contains(INCLUDEPATH, $${SystemService_DIR}) {
        } else {
                INCLUDEPATH += $${SystemService_DIR}
        }
} else {
        error("Not found SystemService$${SystemService_DIR})")
}


# libMp7xxCommon
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        libMp7xxCommon_DIR = $${WorkDir}/../libMp7xxCommon
} else {
        libMp7xxCommon_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/libMp7xxCommon
}
#message($${libMp7xxCommon_DIR})
exists($${libMp7xxCommon_DIR}) {
        contains(INCLUDEPATH, $${libMp7xxCommon_DIR/Api}) {
        } else {
				INCLUDEPATH += $${WorkDir}/../libMp7xxCommon/Api
        }
        contains(LIBS, -L$${libMp7xxCommon_DIR}/Out) {
        } else {
                LIBS += -L$${libMp7xxCommon_DIR}/Out
        }
        contains(LIBS, -lMp7xxCommon) {
        } else {
                LIBS += -lMp7xxCommon
        }
} else {
        error("Not found libMp7xxCommon($${libMp7xxCommon_DIR})")
}

# DBusAdvancedIf
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        DBusAdvancedIf_DIR = $${WorkDir}/../DBusAdvancedIf
} else {
        DBusAdvancedIf_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/DBusAdvancedIf
}
#message($${DBusAdvancedIf_DIR})
exists($${DBusAdvancedIf_DIR}) {
        contains(INCLUDEPATH, $${DBusAdvancedIf_DIR}) {
        } else {
                INCLUDEPATH += $${DBusAdvancedIf_DIR}
        }
        contains(LIBS, -L$${DBusAdvancedIf_DIR}/Out) {
        } else {
                LIBS += -L$${DBusAdvancedIf_DIR}/Out
        }
        contains(LIBS, -lDBusAdvancedIf) {
        } else {
                LIBS += -lDBusAdvancedIf
        }
} else {
        error("Not found libDBusAdvancedIf($${libDBusAdvancedIf_DIR})")
}


# libNexusExt
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
	libNexusExt_DIR = $${WorkDir}/libNexusExt
} else {
	libNexusExt_DIR = $${WorkDir}/libNexusExt
}
#message($${libNexusExt_DIR})
exists($${libNexusExt_DIR}) {
	contains(INCLUDEPATH, $${libNexusExt_DIR}/Api) {
	} else {
		INCLUDEPATH += $${libNexusExt_DIR}/Api
	}
	contains(LIBS, -L$${libNexusExt_DIR}/Out) {
	} else {
		LIBS += -L$${libNexusExt_DIR}/Out
	}	
	contains(LIBS, -lNexusExt) {
	} else {
		LIBS += -lNexusExt
	}	
} else {
	error("Not found libNexusExt($${libNexusExt_DIR})")
}

PRE_TARGETDEPS += libNexusExt/Out/libNexusExt.so
libNexusExt.target = libNexusExt/Out/libNexusExt.so
libNexusExt.commands = @echo Make: libNexusExt; make -C libNexusExt
libNexusExt.depends = FORCE
QMAKE_EXTRA_TARGETS += libNexusExt

# libnexus.so
isEmpty(PRJ_TOP_DIR) {
	NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
	NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
	contains(LIBS, -L$${NexusBinDir}) {
	} else {
		LIBS += -L$${NexusBinDir}
	}	
	contains(LIBS,$${NexusBinDir}/libnexus.so) {
	} else {
		LIBS += $${NexusBinDir}/libnexus.so
	}	
} else {
	error("Not found NexusBinDir($${NexusBinDir})")
}


#libb_playback_ip.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libb_playback_ip.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libb_playback_ip.so
        }
        contains(LIBS, -lb_playback_ip) {
        } else {
                LIBS += -lb_playback_ip
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libssl.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libssl.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libssl.so
        }
        contains(LIBS, -lssl) {
        } else {
                LIBS += -lssl
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libb_os.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libb_os.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libb_os.so
        }
        contains(LIBS, -lb_os) {
        } else {
                LIBS += -lb_os
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libbip_base.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libbip_base.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libbip_base.so
        }
        contains(LIBS, -lbip_base) {
        } else {
                LIBS += -lbip_base
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libb_psip.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libb_psip.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libb_psip.so
        }
        contains(LIBS, -lb_psip) {
        } else {
                LIBS += -lb_psip
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libliveMedia.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libliveMedia.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libliveMedia.so
        }
        contains(LIBS, -lliveMedia) {
        } else {
                LIBS += -lliveMedia
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libgroupsock.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libgroupsock.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libgroupsock.so
        }
        contains(LIBS, -lgroupsock) {
        } else {
                LIBS += -lgroupsock
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libUsageEnvironment.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libUsageEnvironment.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libUsageEnvironment.so
        }
        contains(LIBS, -lUsageEnvironment) {
        } else {
                LIBS += -lUsageEnvironment
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libBasicUsageEnvironment.so
isEmpty(PRJ_TOP_DIR) {
        NexusBinDir = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/nexus/bin
} else {
        NexusBinDir = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/nexus/bin
}
exists($${NexusBinDir}) {
        contains(LIBS, -L$${NexusBinDir}/libBasicUsageEnvironment.so) {
        } else {
                LIBS += -L$${NexusBinDir}/libBasicUsageEnvironment.so
        }
        contains(LIBS, -lBasicUsageEnvironment) {
        } else {
                LIBS += -lBasicUsageEnvironment
        }

} else {
        error("Not found NexusBinDir($${NexusBinDir})")
}

#libblive_ext.a
isEmpty(PRJ_TOP_DIR) {
        BSEAV_LIB_DIR = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/BSEAV/lib/livemedia/blive_ext/lib/arm-linux.debug
} else {
        BSEAV_LIB_DIR = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/BSEAV/lib/livemedia/blive_ext/lib/arm-linux.debug
}
exists($${BSEAV_LIB_DIR}) {
        contains(LIBS, -L$${BSEAV_LIB_DIR}/libblive_ext.a) {
        } else {
                LIBS += -L$${BSEAV_LIB_DIR}
        }
	
        contains(LIBS, -lblive_ext) {
        } else {
                LIBS += -lblive_ext
        }

} else {
        error("Not found BSEAV_LIB_DIR($${BSEAV_LIB_DIR})")
}

#libb_dcc.a
isEmpty(PRJ_TOP_DIR) {
        BSEAV_LIB_DIR_CC = $${WorkDir}/../../../obj.$(NEXUS_PLATFORM)/BSEAV/bin
} else {
        BSEAV_LIB_DIR_CC = $(PRJ_TOP_DIR)/obj.$(NEXUS_PLATFORM)/BSEAV/bin
}
exists($${BSEAV_LIB_DIR_CC}) {
        contains(LIBS, -L$${BSEAV_LIB_DIR_CC}/libb_dcc.a) {
        } else {
                LIBS += -L$${BSEAV_LIB_DIR_CC}
        }

        contains(LIBS, -lb_dcc) {
        } else {
                LIBS += -lb_dcc
        }

} else {
        error("Not found BSEAV_LIB_DIR_CC($${BSEAV_LIB_DIR_CC})")
}


# gstreamer
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
	gstreamer_DIR = $${WorkDir}/../../opensource/gstreamer/gstreamer-0.10.36
} else {
	gstreamer_DIR = $(PRJ_TOP_DIR)/AppLibs/opensource/gstreamer/gstreamer-0.10.36
}
#message($${gstreamer_DIR})
exists($${gstreamer_DIR}) {
} else {
	isEmpty(PRJ_TOP_DIR) {
    	gstreamer_DIR = $${WorkDir}/../../opensource/gstreamer/gstreamer-0.10.36
	} else {
    	gstreamer_DIR = $(PRJ_TOP_DIR)/AppLibs/opensource/gstreamer/gstreamer-0.10.36
}
}
exists($${gstreamer_DIR}) {
	contains(INCLUDEPATH, $${gstreamer_DIR}) {
	} else {
		INCLUDEPATH += $${gstreamer_DIR}
	}
	contains(LIBS, -L$${gstreamer_DIR}/gst/.libs) {
	} else {
		LIBS += -L$${libGlobalStatus_DIR}/gst/.libs
	}	
	contains(LIBS, -lgstreamer-0.10) {
	} else {
		LIBS += -lgstreamer-0.10
	}	
} else {
	error("Not found gstreamer($${gstreamer_DIR})")
}


# libdbus
contains(LIBS,-ldbus-1) {
} else {
	LIBS = $${LIBS} -ldbus-1
}

#Rtc
#message($(PRJ_TOP_DIR))
isEmpty(PRJ_TOP_DIR) {
        Rtc_DIR = $${WorkDir}/../Rtc
} else {
        Rtc_DIR = $(PRJ_TOP_DIR)/AppLibs/thtfit/Rtc
}
#message($${Rtc_DIR})
exists($${Rtc_DIR}) {
        contains(INCLUDEPATH, $${Rtc_DIR}) {
        } else {
                INCLUDEPATH += $${Rtc_DIR}
        }
        contains(LIBS, -L$${Rtc_DIR}/Out) {
        } else {
                LIBS += -L$${Rtc_DIR}/Out
        }
        contains(LIBS, -lrtc) {
        } else {
                LIBS += -lrtc
        }
} else {
        error("Not found librtc($${Rtc_DIR})")
}	



#message(LIBS=$${LIBS})
