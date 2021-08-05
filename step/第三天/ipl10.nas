; hello-os
; TAB=2

CYLS	EQU	10	;	定义常量10，用于柱面计数
	ORG 0x7c00	;	指明程序的装载地址 7c00-7dff 共512字节内存空间，用于装载启动区的内容
	
; 以下的记述用于标准FAT12格式的软盘的专用代码（2面*80扇道*18扇区*512bytes扇区的大小=1440*1024bytes=1440KB=1.44MB）

	JMP	entry
	DB	0x90
	DB	"HARIBOTE"	;	启动区的内容，可以时任意的字符，8字节
	DW	512	;	每个扇区的(sector)的大小，必须为512字节
	DB	1	;	簇(cluster)的大小，必须为1个扇区
	DW	1	;	FAT的起始位置，一般从第一个扇区开始
	DB	2	;	FAT的个数，必须为2
	DW	224	;根目录的大小，一般设置为224项
	DW	2880	;	该磁盘的大小，必须是2880扇区
	DB	0xf0	;	磁盘的种类，必须是0xf0
	DW	9	;	FAT的长度，必须是9扇区
	DW	18	;	1个磁道(track)有几个扇区，必须是18
	DW	2	;	磁头数，必须是2
	DD	0	;	不使用分区，必须是0
	DD	2880	;	重写一次磁盘大小
	DB	0,0,0X29	;	意义不明，固定
	DD	0Xfffffff	;	可能是卷标号码
	DB	"HARIBOTEOS "	;	磁盘的名称，11字节
	DB	"FAT12   "	;	磁盘格式名称，8字节
	RESB	18	;	先空出18字节
	
;	上面的部分，是定死的，只要是FAT12格式的软盘，必定以上面的内容开始

;	程序核心
;	先初始化寄存器，把寄存器的值清0或赋默认值
;	程序入口，由于最开始指定了装载程序的地址时0x7c00，然后立刻就跳到了此处，所以entry的地址就是0x7c00

entry:
	MOV	AX,0
	MOV	SS,AX
	MOV	SP,0x7c00
	MOV	DS,AX
	
;读磁盘	
	MOV AX,0x0820
	MOV ES,AX
	MOV CH,0	;	柱面0
	MOV DH,0	;	磁头0
	MOV CL,2	; 扇区2
;	下面随着程序的运行，柱面、磁头和扇区的值都会发生变化	
	
;上面也可以从扇区1开始加载，加载到内存0x8000处


;	加载软盘上初始的10个柱面，每个柱面18个扇区，共计大小 2 * 10 * 18 * 512 = 180KB
; 骗人的啦，根本没有加载第一个扇区，只加载了179个扇区，实际加载的大小为 179.5KB

readloop:
	MOV SI,0	;记录失败次数的寄存器
	
retry:
	MOV AH,0x02	;	读入磁盘
	MOV AL,1	;	1个扇区
	MOV BX,0
	MOV DL,0X00	;	A驱动器
	INT 0x13	;	调用磁盘BIOS
	JNC next	;	没出错时跳转到next
	ADD SI,1	;	SI加1，失败次数加1
	CMP SI,5	;	比较SI与5
	JAE error	;	SI >= 5时，跳转到error
	MOV AH,0x00
	MOV DL,0x00	;	A驱动器
	INT	0x13	;	重置驱动器
	JMP retry	; 重试
	

next:
	MOV AX,ES	;	把内存地址后移0x200=512字节
	ADD AX,0x0020
	MOV ES,AX	;
	ADD CL,1	;	CL加1，即扇区加1
	CMP CL,18	; 比较CL与18
	JBE readloop	;	如果CL <=18，即当前柱面内的扇区还没有读取结束，则跳转至readloop，继续读当前柱面的CL扇区
	
;	如果当前柱面的一面的全部扇区都读完了，则读另外一面，即使用另一个磁头读取数据
	MOV CL,1	;	将扇区设置为1，另一面肯定从第一个扇区开始读噻
	ADD DH,1	; 磁头加1，注意，一共就两个磁头哦，0和1
	CMP DH,2
	JB readloop	;	如果DH < 2，即两面都还么有读结束，则跳转到readloop，继续读
	
;	如果当前柱面的两面都读完了，则准备读下一个柱面		
	MOV DH,0	; 磁头归0
	ADD CH,1	; 柱面加1
	CMP CH,CYLS	;	比较是否读够了指定的柱面数 CYLS=cylinders
	JB readloop	;	如果CH < CYLS，则跳转到readloop，继续读

;

	MOV	[0x0ff0],CH	;	此时CH的值就是CYLS=10，写到了内存0x0ff0处，我也不知道为啥是写到这个地址
	JMP	0xc200	;	跳转到真正的操作系统处，启动区（IPL）的使命完成了
	
; BIOS加载IPL，IPL加载操作系统


error:
	MOV	SI,msg
	
putloop:
	MOV	AL,[SI]
	ADD	SI,1	; 
	CMP	AL,0
	JE	fin
	MOV	AH,0x0e	;
	MOV	BX,15 ;
	INT	0x10	;
	JMP	putloop
fin:
	HLT	; cpu休息
	JMP	fin	; 继续休息
msg:
	DB	0x0a, 0x0a	;
	DB	"load error"
	DB	0x0a			; 
	DB	0

	RESB	0x7dfe-$	; 填充0到内存0x7dfe

	DB	0x55, 0xaa	;512个字节最后俩个字节是55aa