/*告诉c编译器，有个函数在别的文件里，记得自己找下*/
void io_hlt(void);
void write_mem8(int addr,int data);

void HariMain(void) {
	int i;	/**32位的整数，即4个字节DWORD*/
	char *p;	/**char类型的指针，用于Byte型地址*/
	
	for	(i = 0xa0000;	i <= 0xaffff;	i++)	{
		p = i;
		*p = i | 0x0f;
	}
	
	for (;;) {
		io_hlt();
	}
}
