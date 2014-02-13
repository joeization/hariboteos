harib03
=======

## 一个小记忆点

    10位的内存地址可以表示1KB数据
    20位的内存地址可以表示1MB数据
    30位的内存地址可以表示1GB数据
    32位的内存地址可以表示4GB的数据

## GDTR

GDTR是一个非常特殊的寄存器，给他赋值时，唯一的方法就是给他指定一个特殊的寄存器，从指定的地址中读取6个字节，即命令LGDT。

该寄存器的低16位是段上限，它等于“GDT的有效字节数-1”，剩下的32位是GDT开始的地址。

    load_gdt(0xffff, 0x00270000);

    _load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

栈定本来是FF FF 00 00 00 27 00 00,变成了 FF FF FF FF 00 27 00 00，因而只有用ESP + 6 这些内容 


## set_segmdesc


    void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
    {
        if (limit > 0xfffff) {
            ar |= 0x8000; /* G_bit = 1 */       /*ar 当前为32位，第16位为1*/
            limit /= 0x1000;                    /*limit 段上限，因为当前大于20位，故而要除以12位，即以页的方式计算，变成20位*/
        }
        sd->limit_low    = limit & 0xffff;     
        sd->base_low     = base & 0xffff;
        sd->base_mid     = (base >> 16) & 0xff;
        sd->access_right = ar & 0xff; 

        /*limit 右移16位与0x0f并操作，得到的是limit值高位， 因为limit_hight最高四位还有了ar的4位属性，所以ar左移8位，与 0xf0进行并操作 */
        sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);    
        sd->base_high    = (base >> 24) & 0xff;
        return;
    }

查看一下SEGMNET_DESCRIPTOR的结构

    struct SEGMENT_DESCRIPTOR {
        short limit_low, base_low;
        char base_mid, access_right;
        char limit_high, base_high;
    };


段地址是32位的，这里用了一个base作为名称，low是2个字节，mid是一个字节，high又是一个字节。
段上限其实也是个内存地址，就是说也需要`32`位，但是它不能32位，因为我们只给SEGMENT_DESCRIPTOR分配了64位的地址，因此段上限只分配了20位，但limit_low, limt_high 加起来是24位

英特尔的叔叔们想到了一个方法，20位可以表示1M的数据，再在段属性中设了一个标志位，如果这个是1，就解释为page，即4KB

段属性写在limit_high的前四位，以及access_right这8位中，所以有12位
看上面set_segmdesc的方法签名，用了一个limit，其实只要20位，base是32位，ar是12位的，但都是init。所以进行了一系列的改变。


    base_low + base_mid + base_hight = 16 + 8 + 8 = 32
    limit_low + limt_high = 16 + 8 = 24
    access_right = 8

ar 的高四位是“扩展访问权”，为什么这么讲呢？因为高4位的访问属性在80286时代还不存在，到386位才有的。这4位是由GD00构成的，其中G是指刚才说的 G bit，可以是否解释为page，而D是段的模式，1是指32位模，0是指16位模式。这里说的16位模式只用于运行80286的程序，不能用于调用BIOS，除了80286外，这个D通常都是1。

ar 的低8位表示了什么呢？

    00 00 00 00     0x00    未使用的记录表
    10 01 00 10     0x92    系统专用，可读的段，不可执行
    10 01 10 10     0x9a    系统专用，可执行的段，可读不可写

    11 11 00 10     0xf2    应用程序用，可读写的段，不可执行
    11 11 10 10     0xfa    应用程序用，可执行的段，可读不可写
    
查看一下它的调用过程

    void init_gdtidt(void)
    {
        struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
        struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
        int i;

        for (i = 0; i < 8192; i++) {
            set_segmdesc(gdt + i, 0, 0, 0);
        }
        set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
        set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
        load_gdtr(0xffff, 0x00270000);

        for (i = 0; i < 256; i++) {
            set_gatedesc(idt + i, 0, 0, 0);
        }
        load_idtr(0x7ff, 0x0026f800);

        return;
    }



## 关于PIC

PIC是programmable interrupt controller 的缩写，是指“可编程中断控制器
本书中关于PIC的论述是很简略的，还需要通过其他书籍中进行深入理解，但需要明白的就是，要进行一次PIC的初始化工作

    init_pic(void)    

然后调用`set_gatedesc`，分配中断的相应处理函数

    set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 * 8, AR_INTGATE32);
	set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);


struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};




##_asm_inthandler21

    _asm_inthandler21:
            PUSH	ES
            PUSH	DS
            PUSHAD
            MOV		EAX,ESP
            PUSH	EAX
            MOV		AX,SS
            MOV		DS,AX
            MOV		ES,AX
            CALL	_inthandler21
            POP		EAX
            POPAD
            POP		DS
            POP		ES
            IRETD


为什么不能直接将一个C语言的函数地址直接放入中断处理中去？
因为中断必须使用`IRETD`进行返回


代码中DS，ES 放入SS值，因为C语言自以为是地认为“DS也好，ES也好，SS她好，它们都是指同一个段”，所以如果不按照它的想法设定的话，函数inthandler21就不能够顺利地进行！

但为什么？






