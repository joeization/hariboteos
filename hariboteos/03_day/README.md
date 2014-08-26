Haribote-day02
==============

## ORG指令

ORG是汇编伪指令，用以使汇编语言生成相应的机器码

## ORG 7c00

最初的计算机设计者规定，操作系统的启动区内容装载地址为 0x00007c00 - 0x00007dff

## .bin是用以链接的文件, .lst是列表文件

## ASSEMBLY LANGUAGE

    AX  accumulator
    CX  counter
    DX  data
    BX  base

    SP  stack pointer
    BP  base pointer
    SI  source index
    DI  destination index

    EAX
    ECX
    EDX
    EBX

    ESP
    EBP
    ESI
    EDI

    ES  extra segment
    CS  code segment
    CC  stack segment
    DS  date segment
    FS
    GS

## MOV CX, [1234]

    其实是 MOV CX,[DX:1234]


## 磁盘映像工具

    edimg.exe

## IPL 

IPL启动区位于C0-H0-S1，下一个扇区为C0-H0-S2

## 缓冲区地址

这个内存地址，表明我们要把软盘上读出的数据装载到内在的哪个位置上。一般来讲如果能用一个寄存器表示内在地址的话，当然很方便，但BX只能表示到0x0000 - 0xffff的值,最大才64K，所以就有了EBX这个寄存器，就可以处理4G内存了。


##为什么是0x7c00

内存中以0开始的部分，是BIOS用来实现不同功能的地方，是不能使用的

另外，0xf0000号地址附近，还存放着BIOS程序本身，也不能随意使用

所以IBM的大叔们将0x00007c00 - 0x00007d00规定为装载启动区内容的地址

## 扇区

一个圆环有18个扇区,柱面以0开头，磁头以0开头，扇区以1开头

    C0-H0-S1 柱面0,磁头0,扇区1
    C0-H0-S2
    C0-H0-S3
    C0-H0-S4
    C0-H0-S5
    C0-H0-S6
    C0-H0-S7
    C0-H0-S8
    C0-H0-S9
    C0-H0-S10
    C0-H0-S11
    C0-H0-S12
    C0-H0-S13
    C0-H0-S14
    C0-H0-S15
    C0-H0-S16
    C0-H0-S17
    C0-H0-S18

    C0-H1-S1 柱面0,磁头0,扇区1
    C0-H1-S2

# 如何读盘

    AH = 0x20
    CH 柱面号
    CL 扇区
    DH 磁头
    DL 驱动器号
    ES:BX 缓冲地址 如ES为0x0820,BX为0,则加载到0x8200 - 0x83ff这一段内存当中

    返回值:
    FLAG.CF == 0  没有错误，AH为0
    FLAG.CF == 1  错误      AH为错误码

# 如何复位磁盘

    AH = 0x20
    DL 驱动器号

## 内存分布

    0x7C00 - 0x7DFF是用于启动区的，要将内容读取到这个地方

    0x7e00 - 0x9fbff这段内在操作系统可以随意使用

    0x8000 - 0x81ff这512个字节中装着启动区的内容

    0x8200 - 0x83ff会开始装载着软盘中的数据

在ipl10中，最初的CL是2,就是说，是从第二个扇区加载的，因而0x8000-0x81ff区域是没有内容的，起码没有被读盘读入其中，而且最初的IPL是在0x7C00 - 0x7DFF 之中的

## img文件中分布情况

    0x002600 处会写上文件名
    0x004200 处会有文件内容（操作系统内容）

    因而haribote.sys那段内容在内存中的起始地址是0x8000 + 0x4200 = 0xc200

## edimg

磁盘映像管理工具

先读入一个磁盘映像文件，然后在开头的位置写上ipl.bin的内容，最后输出为hellos.img磁盘映像文件。


## asmhead.nas 由haribotes.nas进化而来，但其中加了100多行的汇编代码，作者在本节并末说明原因


## gas 汇编语言

.gas 文件是gas汇编语言的文件，本书中是用CC1将C语言改为gas文件的

## nas

GAS2NASK是将.gas文件改为.nas文件

## bim

bim 是作者川合秀实创造的一种文件格式，只表明将各部分链接到一起了，做成了一个完整的机器语言文件，但为了实际应用，我们还需要针对每一个不同操作系统的要求进行一系列的修改，比如说加上识别用的头文件，压缩等

