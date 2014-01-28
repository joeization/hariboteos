TOOLPATH = ../../z_tools/
MAKE     = make -r
NASK     = $(TOOLPATH)nask.exe
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = cp
DEL      = rm -f
QEMU = /usr/bin/qemu-system-i386

default :
	$(MAKE) img

ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

haribote.img : ipl.bin Makefile
	$(EDIMG)   imgin:../../z_tools/fdimg0at.tek wbinimg src:ipl.bin len:512 from:0 to:0   imgout:haribote.img


asm :
	$(MAKE) ipl.bin

img :
	$(MAKE) haribote.img

run :haribote.img
	$(QEMU) -fda haribote.img
	
clean :
	$(DEL) ipl.bin
	$(DEL) ipl.lst

src_only :
	$(MAKE) clean
	$(DEL) haribote.img

