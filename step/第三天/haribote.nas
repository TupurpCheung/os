; haribote-os
;	tab=2
	ORG 0xc200	;	������򽫱����ص��ڴ��ʲô�ط�
	
	MOV AL,0x13	;	vga�Կ���320*200*8λ��ɫ
	MOV AH,0x00
	INT 0x10
	
fin:
	HLT
	JMP	fin