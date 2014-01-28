; naskfunc
; TAB=4

[FORMAT "WCOFF"]        ; 制作目标文件的模式
[INSTRSET "i486p"]       ;告诉nask这个程序是486的机器语言
[BITS 32]			    ; 制作32位模式的机器语言			
[FILE "naskfunc.nas"]			

		GLOBAL	_io_hlt,_write_mem8

[SECTION .text]	

_io_hlt:	
		HLT
		RET

_write_mem8:        ; write_mem8(int addr, int data)

        MOV     ECX,[ESP+4]
        MOV     AL,[ESP+8]
        MOV     [ECX],AL
        RET
