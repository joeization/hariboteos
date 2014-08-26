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

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

haribote.sys : haribote.nas Makefile
	$(NASK) haribote.nas haribote.sys haribote.lst

haribote.img : ipl10.bin haribote.sys Makefile
	$(EDIMG)   imgin:../../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		imgout:haribote.img

img :
	$(MAKE) haribote.img

run :haribote.img
	$(QEMU) -fda haribote.img

clean :
	$(DEL) ipl10.bin
	$(DEL) ipl10.lst
	$(DEL) haribote.sys
	$(DEL) haribote.lst

src_only :
	$(MAKE) clean
	$(DEL) haribote.img
