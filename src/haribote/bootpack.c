#include <stdio.h>
/*告诉c编译器，有个函数在别的文件里，记得自己找下*/
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

/**初始化调色板*/
void init_palette(void);
/**设置调色板*/
void set_palette(int start, int end, unsigned char *rgb);
/**画矩形*/
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
//初始化屏幕
void init_screen(char *vram,int xsize,int ysize);
//显示8*16的字符
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);	


/**定义宏*/
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

//定义bootinfo信息结构体
struct BOOTINFO {
	char cyls,leds,vmode,reserve;
	short scrnx,scrny;	//屏幕分辨率,注意类型不要写成Int,因为在asmhead.nas中定义时，像素的x和y长度只占两个字节，如果写成int就会变成四字节，对不上了。
	char *vram;	//显存指针
};


void HariMain(void) {
	//使用16个字节来表示字母A,比特位为1时，设置像素点颜色
	static char font_A[16] = {
		0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,
		0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00
	};	
	struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
	char s[40];
		//初始化调色板
	init_palette();
	//初始化屏幕
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	//打印A
	putfont8(binfo->vram, binfo->scrnx, 10, 10, COL8_FFFFFF, font_A);
	
	//打印两遍会有立体字的效果
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_C6C6C6,31, 31, "Haribote OS.");
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_FFFFFF,30, 30, "Haribote OS.");
	
		
	sprintf(s,"scrnx = %d",binfo->scrnx);
	putfonts8_asc(binfo->vram, binfo->scrnx, COL8_FFFFFF,50, 30,s);
	
	for (;;) {
		io_hlt();
	}
}


/**
初始化调色板，也就是系统中就只有这些颜色
每三个字节为一组，每组表示一个颜色，共16组
*/
void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黑 */
		0xff, 0x00, 0x00,	/*  1:亮红 */
		0x00, 0xff, 0x00,	/*  2:亮绿 */
		0xff, 0xff, 0x00,	/*  3:亮黄 */
		0x00, 0x00, 0xff,	/*  4:亮蓝 */
		0xff, 0x00, 0xff,	/*  5:亮紫 */
		0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
		0x84, 0x00, 0x00,	/*  9:暗红 */
		0x00, 0x84, 0x00,	/* 10:暗绿 */
		0x84, 0x84, 0x00,	/* 11:暗黄 */
		0x00, 0x00, 0x84,	/* 12:暗青 */
		0x84, 0x00, 0x84,	/* 13:暗紫 */
		0x00, 0x84, 0x84,	/* 14:浅暗蓝 */
		0x84, 0x84, 0x84	/* 15:暗灰 */
	};
	
	//设置调色板
	set_palette(0, 15, table_rgb);
	return;

	
}

//将调色板数据记录到内存中
void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 先备份eflags的值 */
	io_cli(); 					/* 设置不允许中断 */
	//向端口0x03c8输入的为调色板序号
	io_out8(0x03c8, start);
	//接着向0x03c9输入颜色，如果还要接着输入下一序号的颜色，可以直接接着输入颜色，而不用输入序号
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	

	io_store_eflags(eflags);	/* 恢复eflags的现场 */
	return;
}

void init_screen(char *vram,int xsize,int ysize) {
	boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
	boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
	boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

	boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
	boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
	boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
	boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
	boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
	
}

/**画矩形
* 当前系统使用的分辨率是320*200的
* 像素点在显存的位置为从左到右边，从上到下
* 显存地址 = 0xa0000 + x + y * 20
* x0,y0 为左上角
* x1,y1 为右下角
*/
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}


/**
*@param vram 显存地址
*@param xsize 像素长度
*@param x x轴起始点，左上角为[0,0]
*@param y y轴起始点，左上角为[0,0]
*@param c 颜色
*@param 字符数组8*16位
*/
void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	//共计需要16个字节
	for (i = 0; i < 16; i++) {
		//求每个字节的第一个位的指针
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

//显示ascii字符
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for (; *s != 0x00; s++) {
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}

