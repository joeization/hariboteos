; haribote-ipl
; TAB=4

CYLS	EQU		10				

		ORG		0x7c00		    ; IPL规定必须加载到0x7c00这一地址	

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		
		DW		512				
		DB		1				
		DW		1				
		DB		2				
		DW		224				
		DW		2880			
		DB		0xf0			
		DW		9				
		DW		18				
		DW		2				
		DD		0				
		DD		2880			
		DB		0,0,0x29		
		DD		0xffffffff		
		DB		"HARIBOTEOS "	
		DB		"FAT12   "		
		RESB	18				


entry:
		MOV		AX,0		        ; 初始化寄存器为0	
		MOV		SS,AX               ; SS 为0
		MOV		SP,0x7c00           ; SP 
		MOV		DS,AX

		MOV		AX,0x0820
		MOV		ES,AX               ; ES = 0x0820
		MOV		CH,0		        ; 柱面0	
		MOV		DH,0			    ; 磁头0
		MOV		CL,2		        ; 扇面2	
readloop:
		MOV		SI,0		        ; if si > 5 go error	
retry:
		MOV		AH,0x02		        ; int 0x13时，AH为读盘	
		MOV		AL,1		        ; AL 处理对象的扇区数	
		MOV		BX,0                ; ES:BX为缓冲区地址
		MOV		DL,0x00			    ; 启动器号,因为只有一个磁盘，因而为0
		INT		0x13			
		JNC		next		        ; 成功则到标号next处，错误则重新读盘	
		ADD		SI,1			
		CMP		SI,5			
		JAE		error			
		MOV		AH,0x00             ; int 0x13 AH 0为磁盘复位，DL为磁盘号
		MOV		DL,0x00			
		INT		0x13			
		JMP		retry
next:
		MOV		AX,ES		         
		ADD		AX,0x0020           ; 一个扇区有512个字节 0x200 = 512 
		MOV		ES,AX			    ; ES = 0x0820 + 0x20 + 0x20 + 0x20 
		ADD		CL,1		        ; CL = CL + 1 initial CL is 2	
		CMP		CL,18		        ; to the 18 	
		JBE		readloop		    ;   继续读，直接读到第18个扇区
		MOV		CL,1
		ADD		DH,1                
		CMP		DH,2
		JB		readloop		
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		

		MOV		[0x0ff0],CH		
		JMP		0xc200

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			
		MOV		BX,15			
		INT		0x10			
		JMP		putloop
fin:
		HLT						
		JMP		fin				
msg:
		DB		0x0a, 0x0a		
		DB		"load error"
		DB		0x0a			
		DB		0

		RESB	0x7dfe-$		

		DB		0x55, 0xaa

