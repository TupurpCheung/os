#include "bootpack.h"
#include <stdio.h>


void HariMain(void) {
	//使用16个字节来表示字母A,比特位为1时，设置像素点颜色
	static char font_A[16] = {
		0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,
		0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00
	};	
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	char s[40], mcursor[256];
		//初始化调色板
	init_palette();
	//初始化屏幕
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	//打印A
	putfont8(binfo->vram, binfo->scrnx, 10, 10, COL8_FFFFFF, font_A);
	
	//打印两遍会有立体字的效果
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_C6C6C6,31, 31, "Haribote OS.");
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_FFFFFF,30, 30, "Haribote OS.");
	
	
	//初始化鼠标
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, 180, 30, mcursor, 16);	
		
	sprintf(s,"scrnx = %d",binfo->scrnx);
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_FFFFFF,80, 30,s);
	
	
	
	for (;;) {
		io_hlt();
	}
}

