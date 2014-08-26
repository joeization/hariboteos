TOOLPATH=../../z_tools/
INCPATH=../../z_tools/

MAKE = make -r
NASK = $(TOOLPATH)nask.exe
DEL = rm -f
QEMU = /usr/bin/qemu-system-i386

default:
	$(MAEK) img

helloos.img:
	$(NASK) helloos.nas helloos.img

img:
	$(MAKE) helloos.img

run:helloos.img
	$(QEMU) helloos.img

src_only:
	$(DEL) *.img
