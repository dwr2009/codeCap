#****************************************************************************
#
# Makefile for TinyXml test.
# Lee Thomason
# www.grinninglizard.com
#
# This is a GNU make (gmake) makefile
#****************************************************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# TINYXML_USE_STL can be used to turn on STL support. NO, then STL
# will not be used. YES will include the STL files.
TINYXML_USE_STL := NO

#****************************************************************************

CROSS_COMPILER := arm-linux-

CC     := ${CROSS_COMPILER}gcc
CXX    := ${CROSS_COMPILER}g++
LD     := ${CROSS_COMPILER}g++
AR     := ${CROSS_COMPILER}ar rc
RANLIB := ${CROSS_COMPILER}ranlib

#DEBUG_CFLAGS     := -Wall -Wno-format -g -DDEBUG -mips32r2 -Wa,-mips32r2
#RELEASE_CFLAGS   := -O2 -mips32r2 -Wa,-mips32r2 -mtune=24kf -pipe -L/sdb1/xrq/Project/smp86xx_android2.2/cs_rootfs_1.2.19/cross_rootfs/lib -I/sdb1/xrq/Project/smp86xx_android2.2/cs_rootfs_1.2.19/cross_rootfs/include -Wall -Wno-unknown-pragmas -Wno-format -O3

LIBS		 :=

DEBUG_CXXFLAGS   := ${DEBUG_CFLAGS} 
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
   CFLAGS       := ${DEBUG_CFLAGS}
   CXXFLAGS     := ${DEBUG_CXXFLAGS}
   LDFLAGS      := ${DEBUG_LDFLAGS}
else
   CFLAGS       := ${RELEASE_CFLAGS}
   CXXFLAGS     := ${RELEASE_CXXFLAGS}
   LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
   CFLAGS   := ${CFLAGS} -pg -O3
   CXXFLAGS := ${CXXFLAGS} -pg -O3
   LDFLAGS  := ${LDFLAGS} -pg
endif

#****************************************************************************
# Preprocessor directives
#****************************************************************************

ifeq (YES, ${TINYXML_USE_STL})
  DEFS := -DTIXML_USE_STL
else
  DEFS :=
endif

#****************************************************************************
# Include paths
#****************************************************************************

#INCS := -I/usr/include/g++-2 -I/usr/local/include
INCS :=


#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := ${CFLAGS}   ${DEFS}
CXXFLAGS := ${CXXFLAGS} ${DEFS}

#****************************************************************************
# Targets of the build
#****************************************************************************

OUTPUT := xmltest
XMLLIB := libtinyxml.a
XMLLIB_SH := libtinyxml.so

all: $(XMLLIB) $(XMLLIB_SH)


#****************************************************************************
# Source files
#****************************************************************************

# SRCS := tinyxml.cpp tinyxmlparser.cpp xmltest.cpp tinyxmlerror.cpp tinystr.cpp
SRCS := tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp

# Add on the sources for libraries
SRCS := ${SRCS}

OBJS := $(addsuffix .o,$(basename ${SRCS}))
OBJS_SH := $(addsuffix .pic.o,$(basename ${SRCS}))

#****************************************************************************
# Output
#****************************************************************************

${OUTPUT}: ${OBJS} xmltest.o
	${LD} -o $@ ${LDFLAGS} xmltest.o ${OBJS} ${LIBS} ${EXTRA_LIBS}

${XMLLIB_SH}: ${OBJS_SH}
	${LD} -shared -o $@ ${OBJS_SH}

${XMLLIB}: ${OBJS}
	${AR} $@ ${OBJS}
	$(RANLIB) $@

#****************************************************************************
# common rules
#****************************************************************************

# Rules for compiling source files to object files
# NOTE: Rules order is important!
%.pic.o : %.cpp
	${CXX} -c -fPIC ${CXXFLAGS} ${INCS} $< -o $@

%.pic.o : %.c
	${CC} -c -fPIC ${CFLAGS} ${INCS} $< -o $@

%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

install:
	-cp -f *.h ${prefix}/include
	-cp -f ${XMLLIB} ${XMLLIB_SH} ${prefix}/lib

dist:
	bash makedistlinux

clean:
	-rm -f core ${OBJS} ${OBJS_SH} ${OUTPUT} $(XMLLIB) $(XMLLIB_SH) xmltest.o

depend:
	#makedepend ${INCS} ${SRCS}

tinyxml.o: tinyxml.h tinystr.h
tinyxmlparser.o: tinyxml.h tinystr.h
xmltest.o: tinyxml.h tinystr.h
tinyxmlerror.o: tinyxml.h tinystr.h
