/*告诉c编译器，有个函数在别的文件里，记得自己找下*/
void io_hlt(void);

void HariMain(void) {
	fin:
		io_hlt();
		goto fin;
}
