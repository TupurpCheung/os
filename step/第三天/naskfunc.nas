;	naskfunc
; TAB=2

[FORMAT "WCOFF"]	;	制作目标文件的模式
[BITS 32]	;	制作32位模式用的机器语言

;	函数声明
[FILE "naskfunc.nas"]
	GLOBAL _io_hlt	;	需要链接的函数名，前面需要加上_，并且需要使用GLOBAL声明
	
;	实际函数
[SECTION .text]
_io_hlt:
	HLT
	RET