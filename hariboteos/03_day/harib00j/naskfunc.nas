; naskfunc
; TAB=4

[FORMAT "WCOFF"]     ; 制作目标文件的模式
[BITS 32]			 ; 制作32位模式的机器语言			


[FILE "naskfunc.nas"]			
		GLOBAL	_io_hlt			

[SECTION .text]	

_io_hlt:	
		HLT
		RET
