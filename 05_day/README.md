harib02
=======


## GDT 

GDT 是 global(segment) descriptor table 的缩写，即全局段号记录表，这些数据整齐在排列在内存中的某个地方，然后内存的起始地址和有效设定个数被放在了CPU中`GDTR`这一特殊的寄存器当中。

## GDTR

GDT register

GDTR 是一个`48`位的寄存器，包含一个32位基地址和一个16位的段限。

## 什么是分段

在16位时没有分段的概念，但在32位的处理器时，分段就有必要了。它将内存分为一段一段，每段都当作4G来处理，这样，任何一段程序都可以加上一个ORG 0，这个就极大地方便了编译。

看下面一个程序，它的DS是段地址，CPU会往EBX中加上某个值来计算地址。而不是说像8086那样，DS要剩以16。 

    MOV AL, [DS:EBX]

## segment selector的计算方式是什么？



## 表示一个段有哪些信息

- 段的大小 
- 段的起始地址
- 段的管理属性

CPU需要用`8`个字节的数据来表示这个信息，但段寄存器在i386时，并没有增长到32位，它依然是16位，是没有EDS, ECS 这种寄存器的！
那怎么办？

0 0000 0000 0000


- 先将一个段号(segment selector)放入段寄存器里，然后预先设定段号与段的对应关系

- 段寄存器是16位的，但由于CPU设计上的原因，段寄存器的低3位是不能使用的，所以能够用的段号才有13位，那么，能表示的段就只有0 - 8191

- 因为可以表示8192个段，表示一个段的信息需要有 `8` 个字节，那么要存储的信息有 8192 * 8 = 65 536字节，即64KB

- 这64KB的信息就是GDT,将这些数据整齐地排列在内存中固定的某一个地方，即可了，再用一个GDTR这个特殊的寄存器来存储内在的起始地址和有效的设定个数

- 前面论述过，GDTR是48位的寄存器，32位基地址肯定是表示内存地址的，16位的段限是有效个数，前面讨论过，其实只有`13`位有效


## IDT

Interrupt descriptor table中断记录表

要使用鼠标，键盘，网卡等设备，就必须要有中断，我们就必须设定IDT，IDT记录了0-255号中断号码与对应的函数关系，其设定方法与GDT非常相似（也许是因为使用同样的方法能够简化CPU电路设计）


## harib操作系统中GDT与IDT的分布

    gdt     0x270000 - 0x27ffff
    idt     0x26f800 - 0x26ffff


## 关于本次程序的论述

    void init_gdtidt(void)
    {
        struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
        struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
        int i;
        //最多可表示8192个段
        for (i = 0; i < 8192; i++) {
            set_segmdesc(gdt + i, 0, 0, 0);
        }

        set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
        set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
        load_gdtr(0xffff, 0x00270000);

        // 0 - 255号中断
        for (i = 0; i < 256; i++) {
            set_gatedesc(idt + i, 0, 0, 0);
        }
        load_idtr(0x7ff, 0x0026f800);

        return;
    }


