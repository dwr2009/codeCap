
TARGET_PROJECT =Md5-Base64-Permision.pro
TARGET_EXE =Md5-Base64-Permision
QMAKE =/usr/lib/i386-linux-gnu/qt5/bin/qmake

all: Makefile  $(TARGET_EXE)
	echo "compile end"

Makefile:
	$(QMAKE) -o $@ $(TARGET_PROJECT)

$(TARGET_EXE):Makefile
	make

clean:
	rm Makefile $(TARGET_EXE)
