haribote-day01
=============

## 关于汇编的一些知识

    DB      date byte
    RESB    reserve byte, filled with 0
    DW      date word
    RESB    0x1fe-$ 

## FAT32格式

Windows或者MS-DOS格式化的软盘就是这一种格式


## 启动区

计算机读取软盘时，是512个字节为一个扇区进行读写的，必须保证第512个字节是55 AA,这是最初的设计者所规定的。

## IPL

`initial program loader`，启动程序加载器。启动区只有512个空间，操作系统是完全装不下去的，所以把加载操作系统的程序放在启动区里

## boot

bootstrap

