Harib05
======


## 关于鼠标

鼠标的中断数据集中到3个时，方为有效。

    buf[0] 是btn
    buf[1] 是x
    buf[2] 是y
    
因为btn的值是`0x08`， `0x18`， `0x28`，`0x38`， `0x0A`， `0x09`，`0x0c`，`0x0B`这些值，所以在进行一下与0x0xc8的并操作，以确保它的确是有效的。否则将进行重置，因为鼠标会有连线不良，断线等问题，所以要进行重新类似重新对齐的操作。

鼠标键的状态放在buf 0 的低三位，如果第1位为1，就是左键`0x09`，如果第二位是1，就是右键，`0x0A`，如果第三位是1，就是中键 `0x0C`。



## 通往32位之路的 asmhead.nas

    MOV		AL,0xff
    OUT		0x21,AL
    NOP						
    OUT		0xa1,AL

    CLI						


这段代码的作用是做以下的事情

    io_out(PIC0_IMR, 0xff); //禁止主PIC的全部中断
    io_out(PIC1_IMR, 0xff); //禁止从PIC的全部中断
    io_cli();   //禁止CPU级别的中断


为了使CPU能够访问16位以上的内存
    

    CALL	waitkbdout
	MOV		AL,0xd1
	OUT		0x64,AL
	CALL	waitkbdout
	MOV		AL,0xdf			
	OUT		0x60,AL
	CALL	waitkbdout




