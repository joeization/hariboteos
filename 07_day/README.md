Harib04
=======


## 如何攻取按键码


    void inthandler21(int *esp)
    {
        struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
        unsigned char data, s[4];
        io_out8(PIC0_OCW2, 0x61);	
        data = io_in8(PORT_KEYDAT);
        putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "hello,Leo");

        boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
        putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
        return;
    }



这段程序中`io_out8(PIC0_OCW2, 0x61)`，0x61是指“0x60 + IRQ号码”，执行结束以后PIC将继续监视IRQ1中断是否发生，如果没有这句，那么PIC就不会再监视IRQ1中断

那么按键的信息从哪些获取的呢，编号0x0060的设备是键盘，所以直接从中读取data，就知道按键码了。








