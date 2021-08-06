;	naskfunc
; TAB=2

[FORMAT "WCOFF"]	;	制作目标文件的模式
[INSTRSET "i486p"]	;	486cpu
[BITS 32]	;	制作32位模式用的机器语言


[FILE "naskfunc.nas"]
;	函数声明
;	需要链接的函数名，前面需要加上_，并且需要使用GLOBAL声明
	GLOBAL _io_hlt,_write_mem8	;	cpu休眠，写内存
	
;	实际函数
[SECTION .text]
_io_hlt:	;	void io_hlt(void)
	HLT
	RET
	
_write_mem8	;	void write_mem8(int addr,int data)
	MOV ECX,[ESP+4]	;	[ESP+4]存放的是地址，将其读取到ECX
	MOV AL,[ESP+8]	;	[ESP+8]存放的是数据，将其读取到AL
	MOV [ECX],AL	;	将数据放到指定地址处
	RET