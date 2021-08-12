### DOSBox使用
+ 进入`DOSBox-0.74-3`目录
+ 运行`DOSBox 0.74-3 Options.bat`即可

### debug.exe命令
+ R：查看，改变cpu寄存器的内容
  + R;cpu当前寄存器的内容
  + R AX;改变ax寄存器的内容
  + R CS;改变当前cpu代码段寄存器的值
  + R IP;改变指令偏移地址


+ D：查看内存中的内容
  + D CS:IP;查看对应的cs:ip地址的二进制指令数据


+ U：命令将内存中的机器指令翻译成汇编指令
  + U CS:IP;查看对应的CS:IP地址的汇编指令数据


+ E：改写内存中的内容
  + E CS:IP 十六进制数据 十六进制数据 十六进制数据


+ A：以汇编的指令的格式在内存中写入一条机器指令
+ T：执行一条机器指令

### 8086汇编
#### 1、寄存器
##### 1.1、通用（数据）寄存器
+ AX：累加器，一般是用于计算
+ BX：基址寄存器，用来存放存储器偏移地址，和DS配合使用
+ CX：计数器，循环操作中常用，与LOOP指令配合使用
+ DX：数据寄存器

##### 1.2、段寄存器
+ CS：指令段，和IP配合找到下一条指令的位置
+ DS：数据段，和BX配合找到内存中的数据
+ ES：附加段，和DI配合用于串指令。
+ SS；堆栈段，和SP配合寻找栈顶

##### 1.3、指针寄存器
+ IP：指令指针寄存器
+ SP：堆栈指针寄存器，用于存放栈顶指针的位置
+ BP：基址指针寄存器，用于寻找栈内的元素


##### 1.4、变址寄存器
+ SI：源寄存器	
+ DI：目的寄存器
##### 1.5、标志位寄存器
+ CF：进位标志
+ ZF：零标志
+ SF：符号标志
+ PF：奇偶标志
+ OF：溢出标志
+ DF：方向标志（控制）
+ IF：中断允许标志（控制）
+ TF：陷阱标志（控制）

#### 2、内存寻址
+ 8086中最小的内存单元为1个字节。一个字（WORD）为2个字节，DB表示两个字，即4个字节32位
+ 任何2个地址连续的内存单元，N号单元和N+1单元，可以视作2个独立的内存单元，也可以看做1个地址为N的字单元，其中N为低位，N+1为高位
+ DB（define byte）一个字节,DW（define word）一个字
+ 由于8086地址总线有20条，但寄存器只有16位，为了能使得寻址能力最大化，所以采取了物理地址 = 段地址*16+偏移地址的方式

#### 3、栈
+ 一段空间被申请用作栈，想象为一个木桶，物品先进后出，后进先出
+ 木桶底部，也即栈底是高字节
+ 栈顶的位置是会变化的，在栈里面没有数据时，栈顶的位置是：最高的字节+1
+ 通常使用SS:SP来表达栈顶位置，随着栈数据的出栈进栈，SP会自动加减。而SS则不会变化
+ 栈顶是指向最上面的数据，而不是最上面数据的的上面（空空的空气）或者两个数据之间的夹缝

#### 4、一些指令
##### 4.1、MOV
+ MOV 寄存器 数据
+ MOV 寄存器 寄存器

##### 4.2、LOOP
+ 从内存单元ffff:0006位置取一个单元的数据，乘以3，结果放在DX中
```asm
assume cs:codesg

codesg segment
	start:mov ax,0ffffh
			mov ds,ax
			mov bx,0006h,
			mov al,[bx]
			mov ah,0
			mov dx,0
			mov cx,3
	  s:add dx,ax		
			loop s
			mov ax,4c00H
			int 21H
codesg ends	
end start			
```

##### 4.3、PUSH和POP


#### 5、一个包含多个段的程序
```asm
;一个包含多个段的程序，把数据（data）段的字型数据依次入栈
assume cs:code,ds:data,ss:stack	;	指定代码段，数据段，栈段

data segment
	dw 0123H,0456H,0789H,0abcH,0defH,0fedH,0cbaH,0987H
data ends

stack segment
	dw 0,0,0,0,0,0,0,0,0,0
stack ends

code segment
	start:mov ax:stack
		mov ss,ax
		mov sp,16	;	设置栈顶ss:sp指向stack:16，即栈底部的下一个高位
		
		mov ax,data
		mov ds,ax
		mov bx,0	;	设置数据段地址
		
		mov cx,8
	s0:push [bx]
		add bx,2
		loop s0	;	将data段0-15单元的8个字型数据依次入栈 
		
		mov bx,0
		mov cx,8
	s1:pop [bx]
		add bx,2
		loop s1	;	将stack段中的8个字型数据依次出栈到data段的0-15单元中
		
		mov ax,4c00h
		int 21h
code ends
end start
```
