-include ../common.inc

CROSS := arm-linux-
STRIP := $(CROSS)strip

all:
	@echo "Usage: make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  InstallToRootfs"

mk:
	@make;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	make -f Makefile.Client;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	
