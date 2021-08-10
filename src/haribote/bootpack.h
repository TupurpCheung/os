/**定义宏*/
#define ADR_BOOTINFO	0x00000ff0

//IDT起始位置
#define ADR_IDT			0x0026f800
//IDT大小
#define LIMIT_IDT		0x000007ff

//GDT起始位置
#define ADR_GDT			0x00270000
//GDT限制
#define LIMIT_GDT		0x0000ffff

//bootpack起始位置
#define ADR_BOTPAK		0x00280000
//bootpack大小
#define LIMIT_BOTPAK	0x0007ffff


#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a

//颜色
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

//GDT
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
//IDT
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};


/*告诉c编译器，有个函数在别的文件里，记得自己找下，这些都是汇编方法*/
/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

//下面这些都是C语言的方法
/* graphic.c */
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
//初始化鼠标图像
void init_mouse_cursor8(char *mouse, char bc);
//画块状的物体
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);

/* dsctbl.c */
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);	
