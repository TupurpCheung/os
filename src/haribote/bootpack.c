/*����c���������и������ڱ���ļ���ǵ��Լ�����*/
void io_hlt(void);
void write_mem8(int addr,int data);

void HariMain(void) {
	int i;	/**32λ����������4���ֽ�DWORD*/
	char *p;	/**char���͵�ָ�룬����Byte�͵�ַ*/
	
	for	(i = 0xa0000;	i <= 0xaffff;	i++)	{
		p = i;
		*p = i | 0x0f;
	}
	
	for (;;) {
		io_hlt();
	}
}
