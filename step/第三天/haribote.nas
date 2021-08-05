; haribote-os
;	tab=2
	ORG 0xc200	;	这个程序将被加载到内存的什么地方
	
	MOV AL,0x13	;	vga显卡，320*200*8位彩色
	MOV AH,0x00
	INT 0x10
	
fin:
	HLT
	JMP	fin