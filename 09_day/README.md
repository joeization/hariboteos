Harib06
=======

## 关于内存检查

最初启动时，BIOS肯定要做检查内存容量，所以只要我们一问BIOS，就知道内存有多大。但问题是，如果那样做的话，一方面asmhead.nas肯定会变得很长，另一方面，BIOS版本不同，BIOS函数的调用方法也不相同。因而只能自己去做内存检查。

在内存检查时，将缓存设为OFF，先查看CPU是不是在486以上，如果是，就将缓存设为OFF。






## memtest 



    unsigned int memtest(unsigned int start, unsigned int end)
    {
        char flg486 = 0;
        unsigned int eflg, cr0, i;

        
        eflg = io_load_eflags();    //加载EFLAGS
        eflg |= EFLAGS_AC_BIT;      //将AC位高为1 
        io_store_eflags(eflg);      //放进EFLAGS中云
        eflg = io_load_eflags();    //再次拿出
        if ((eflg & EFLAGS_AC_BIT) != 0) {  //如果是386，即使是AC为1，也会自动回到0
            flg486 = 1;
        }
        eflg &= ~EFLAGS_AC_BIT;     // 将EFLAGS的AC位高为0 
        io_store_eflags(eflg);

        if (flg486 != 0) {
            cr0 = load_cr0();       // 禁止缓存
            cr0 |= CR0_CACHE_DISABLE; 
            store_cr0(cr0);
        }

        i = memtest_sub(start, end);

        if (flg486 != 0) {          // 允许缓存
            cr0 = load_cr0();
            cr0 &= ~CR0_CACHE_DISABLE; 
            store_cr0(cr0);
        }

        return i;
    }



## memtest_sub


    unsigned int memtest_sub(unsigned int start, unsigned int end)
    {
        unsigned int i, *p, old, pat0 = 0xaa55aa55, pat1 = 0x55aa55aa;
        for (i = start; i <= end; i += 0x1000) {
            p = (unsigned int *) (i + 0xffc);
            old = *p;			
            *p = pat0;			
            *p ^= 0xffffffff;	
            if (*p != pat1) {	
    not_memory:
                *p = old;
                break;
            }
            *p ^= 0xffffffff;	
            if (*p != pat0) {	
                goto not_memory;
            }
            *p = old;	
        }
        return i;
    }


这个程序调查从`start` 到 `end` 的内存空间内。首先将p指定为一个指针，用old来保存之前的p这一内存空间中的值,然后p所代表的内存中赋值 `0xaa55aa55`，再进行反转，如果是可以的，再进行反转，以确保可以这一内存地址是真实存在的，否则则跳出，此是i就是最大的内存空间。


但实际运行中，这段程序却是 `错误的！！`，为什么呢？因为编译器太强大了，这段程序的生成的汇编代码优化了检查的部分，导致了不能出来正确的结果 


    376 00000409                                 _memtest_sub:
    377 00000409 55                              	PUSH	EBP
    378 0000040A 89 E5                           	MOV	EBP,ESP
    379 0000040C 8B 55 0C                        	MOV	EDX,DWORD [12+EBP]
    380 0000040F 8B 45 08                        	MOV	EAX,DWORD [8+EBP]
    381 00000412 39 D0                           	CMP	EAX,EDX
    382 00000414 77 09                           	JA	L30
    383 00000416                                 L36:
    384 00000416                                 L34:
    385 00000416 05 00001000                     	ADD	EAX,4096
    386 0000041B 39 D0                           	CMP	EAX,EDX
    387 0000041D 76 F7                           	JBE	L36
    388 0000041F                                 L30:
    389 0000041F 5D                              	POP	EBP
    390 00000420 C3                              	RET

## 挑战内存管理

内存管理的的基础，一个是内存分配，一个是内存释放。

windows的内存管理，是以512字节为单位进行管理的，管理表用bit来构成。

第二种方法，是把类似于“从XXX号地址开始的YYY字节的空间是空着的”这些信息存储在表里

 
    struct FREEINFO {	
        unsigned int addr, size;
    };

    struct MEMMAN {	
        int frees, maxfrees, lostsize, losts;
        struct FREEINFO free[1000];
    };


如下面的用法

    struct MEMAN memman;
    memman.frees = 1;
    memman.free[0].addr = 0x00400000;
    memman.free[0].size = 0x07c00000;


比如现在需要有100KB的空间，只要memman中free的善，从中找到100MB以上的可用空间就行了。

    for(i = 0; i< memman.frees; i++){
        if(memman.free[i].size > 100 * 1024){
            //找到了可用空间
            //从地址memman.free[i].addr 开始的100KB空间
        }
    }

这一新方法的优点，首先就是占用的内存少，memman是 `8*1000+4=8004`，还不到8KB






