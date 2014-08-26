TOOLPATH=../../z_tools/
INCPATH=../../z_tools/
MAKE = make -r
NASK = $(TOOLPATH)nask.exe
CC1 = $(TOOLPATH)cc1 -I $(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask -a
OBJ2BIM = $(TOOLPATH)obj2bim
BIN2OBJ = $(TOOLPATH)bin2obj
BIM2HRB = $(TOOLPATH)bim2hrb
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG = $(TOOLPATH)edimg.exe
IMGTOL = $(TOOLPATH)imgtol.com
MAKEFONT = $(TOOLPATH)makefont
GOLIB = $(TOOLPATH)golib00
COPY = cp
DEL = rm -f
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

run : haribote.img
	$(QEMU) -fda haribote.img	

clean :
	-$(DEL) ipl.bin
	-$(DEL) ipl.lst

src_only :
	$(MAKE) clean
	$(DEL) haribote.img
