
######################
#before compile thtfit,Please compile 
# dbus,wireless
#
##########################

SHELL=/bin/sh
CROSS=arm-linux-
CC=$(CROSS)gcc
TARGET_AR=$(CROSS)ar
LD=$(CROSS)ld
TARGET_STRIP=$(CROSS)strip
MAKEFILE_NAME := Makefile
OUT_DIR=Out
OUT_OBJ_DIR := $(OUT_DIR)/Obj

CFLAGS := -shared -fPIC -Wall -IInclude -D_GNU_SOURCE
CPPFLAGS := -fcheck-new
LINKFLAGS= -shared -fPIC -lstdc++

strBaseName=

objecs	= LinuxBaseLib.so DbgLogSwitch.so  libMp7xxCommon.so libGlobalStatus.so 
objecs += SysBaseSrv.so SystemService.so QtExt.so DBusAdvancedIf.so DomCodecAccess.so
objecs += tinyxml-2.5.3.so MediaPlayer.so WebBrowser.so snmp++.so
objecs += GpioBtnInput.so  GpioKbInput.so

objecs_clean = LinuxBaseLib.clean DbgLogSwitch.clean  libMp7xxCommon.clean libGlobalStatus.clean 
objecs_clean += SysBaseSrv.clean SystemService.clean QtExt.clean DBusAdvancedIf.clean DomCodecAccess.clean
objecs_clean += tinyxml-2.5.3.clean MediaPlayer.clean WebBrowser.so snmp++.clean

objecs_clean += GpioBtnInput.clean GpioKbInput.clean


all: $(objecs)
	@echo "Compile end"

clean:$(objecs_clean)
	@echo "Clean end"; 

WebBrowser.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

tslib.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

tinyxml-2.5.3.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean


SystemService.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

SysBaseSrv.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

snmp++.clean:
	@echo "Clean $@"
	make -C $(basename $@) -f build.mk clean
QtExt.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

MediaPlayer.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

LinuxBaseLib.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

libMp7xxCommon.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

libGlobalStatus.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean
DomCodecAccess.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean
DBusAdvancedIf.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

DbgLogSwitch.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean
	

GpioBtnInput.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean

GpioKbInput.clean:
	@echo "Clean $@"
	make -C $(basename $@) clean
	

	
snmp++.so:
	@echo "Compile $@ enter $(basename $@) "	
	make -C $(basename $@) -f build.mk mk
	make -C $(basename $@) -f build.mk InstallToRootfs

WebBrowser.so:snmp++.so
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk GenerateMakefile 
	make -C $(basename $@) -f build.mk mk
	make -C $(basename $@) -f build.mk InstallToRootfs
MediaPlayer.so:tinyxml-2.5.3.so
	make -C $(basename $@) -f build.mk GenerateMakefile 
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs
	
GpioBtnInput.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs

GpioKbInput.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs

DomCodecAccess.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs



DBusAdvancedIf.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs
	
SysBaseSrv.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk mk 
	make -C $(basename $@) -f build.mk InstallToRootfs

QtExt.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk all
	

SystemService.so:QtExt.so
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk GenerateMakefile 
	make -C $(basename $@) 
	make -C $(basename $@) -f build.mk InstallToRootfs

libGlobalStatus.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk mk 
	make -C $(basename $@) -f build.mk InstallToRootfs

libMp7xxCommon.so:SysBaseSrv.so  libGlobalStatus.so
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk mk 
	make -C $(basename $@) -f build.mk InstallToRootfs

tinyxml-2.5.3.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f Makefile
	make -C $(basename $@) -f build.mk InstallToRootfs

uinput.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f Makefile
	make -C $(basename $@) -f build.mk InstallToRootfs

DbgLogSwitch.so:
	@echo "Compile $@ enter $(basename $@) "
	make -C $(basename $@) -f build.mk InstallToRootfs
	
tslib.so:
	@echo "Compile $@ enter $(basename $@) "
	mkdir -p out	
	make -C $(basename $@) -f build.mk AutoGenConfigure 
	make -C $(basename $@) -f build.mk cfg 
	make -C $(basename $@) -f build.mk mk 
	make -C $(basename $@) -f build.mk install
	make -C $(basename $@) -f build.mk InstallToRootfs
	
	
	
%.so:		
	@echo "Compile $@ enter $(basename $@) "
	mkdir -p out	
	make -C $(basename $@) -f build.mk mk
	make -C $(basename $@) -f build.mk InstallToRootfs
	



