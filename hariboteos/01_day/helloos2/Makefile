TOOLPATH=../../z_tools/
INCPATH=../../z_tools/

MAKE = make -r
NASK = $(TOOLPATH)nask.exe
DEL = rm -f
QEMU = /usr/bin/qemu-system-i386

default :
	$(MAKE) img

helloos.img: helloos.nas
	$(NASK) helloos.nas helloos.img

img :
	$(MAKE) helloos.img

run : helloos.img
	$(QEMU) -fda helloos.img

src_only :
	$(DEL) *.img
