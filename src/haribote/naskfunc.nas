;	naskfunc
; TAB=2

[FORMAT "WCOFF"]	;	制作目标文件的模式
[INSTRSET "i486p"]	;	486cpu
[BITS 32]	;	制作32位模式用的机器语言


[FILE "naskfunc.nas"]
;	函数声明
;	需要链接的函数名，前面需要加上_，并且需要使用GLOBAL声明
	
	GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt	;;	cpu休眠，写内存，设置中断位为0，设置中断位为1，..
	GLOBAL	_io_in8,  _io_in16,  _io_in32        
	GLOBAL	_io_out8, _io_out16, _io_out32       
	GLOBAL	_io_load_eflags, _io_store_eflags
	GLOBAL 	_load_gdtr,_load_idtr    

	
;	实际函数
[SECTION .text]

;	cpu休眠
_io_hlt:	;	void io_hlt(void)
	HLT
	RET

;	写内存，第一个参数为内存地址，第二个参数为要写入的数据	
_write_mem8	;	void write_mem8(int addr,int data)
	MOV ECX,[ESP+4]	;	[ESP+4]存放的是地址，将其读取到ECX
	MOV AL,[ESP+8]	;	[ESP+8]存放的是数据，将其读取到AL
	MOV [ECX],AL	;	将数据放到指定地址处
	RET

;	清空中断标志位，即将标志位设置为0，禁止中断
_io_cli:	; void io_cli(void);
		CLI
		RET

;	设置中断标志位，即将标志位设置为1，允许中断
_io_sti:	; void io_sti(void);
		STI
		RET

; 允许中断并cpu休眠
_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

;	
_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

;
_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

; 读入
_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

;
_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

;
_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

;	写出
_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS 
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS 
		RET
		
_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET
	