TOOLPATH=../../z_tools/
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

haribote.sys : haribote.nas Makefile
	$(NASK) haribote.nas haribote.sys haribote.lst

haribote.img : ipl.bin haribote.sys Makefile
	$(EDIMG)   imgin:../../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

img :
	$(MAKE) haribote.img

run :haribote.img
	$(QEMU) -fda haribote.img

clean :
	$(DEL) ipl.bin
	$(DEL) ipl.lst
	$(DEL) haribote.sys
	$(DEL) haribote.lst

src_only :
	$(MAKE) clean
	$(DEL) haribote.img
