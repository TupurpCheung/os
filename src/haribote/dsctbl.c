
/**
*关于GDT,IDT初始化的 descriptor table的处理
*/
#include "bootpack.h"

void init_gdtidt(void) {
	//gdt从内存的0x00270000开始
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	//idt从内存的0x0026f8000开始
	struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;
	int i;

	/* 初始化GDT */
	for (i = 0; i < 8192; i++) {
		//设置8192个段的默认初始值为0
		set_segmdesc(gdt + i, 0, 0, 0);
	}
	//设置第一个段的对应的内存空间为4GB，即32位系统的上限
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
	//设置第二个段大小为512Kb,内存起始位置为0x00280000，存放的是bootpack.ha
	set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
	//
	load_gdtr(0xffff, 0x00270000);

	/* 初始化IDT */
	for (i = 0; i < 256; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}
	load_idtr(0x7ff, 0x0026f800);

	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar) {
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	sd->limit_low    = limit & 0xffff;
	sd->base_low     = base & 0xffff;
	sd->base_mid     = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high    = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar) {
	gd->offset_low   = offset & 0xffff;
	gd->selector     = selector;
	gd->dw_count     = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high  = (offset >> 16) & 0xffff;
	return;
}
