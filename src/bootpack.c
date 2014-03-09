#include "bootpack.h"

void HariMain(void)
{
    struct BOOTINFO *binfo;
    char *vram = (char *)0xa0000;
    int xsize = 320;
    int ysize = 200;
    char mcursor[256];
    int mx,my;

    init_palette();
    init_gdtidt();
    init_pic();
    binfo = (struct BOOTINFO*)0x0ff0;
    xsize = binfo->scrnx;
    ysize = binfo->scrny;
    vram = binfo->vram;
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    /* Ãè»æUI */
    init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
    init_mouse_cursor8(mcursor,COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    // char str[100]; 
    // sprintf(str,"%x xiaomengmeng",0x20);
    // putfont8_asc(binfo->vram,binfo->scrnx,10,10,COL8_FFFFFF,str);

    io_out8(PIC0_IMR, 0xf9);  /* 允许1号键盘中断和2号来自从PIC的中断 */
    io_out8(PIC1_IMR, 0xef);  /* 允许12号鼠标中断信号 */

    for(;;)
	    io_stihlt(); /* Ö´ÐÐnaskfun.nasÖÐµÄio_htlº¯Êý*/ 
    //io_stihlt();
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
    int i,temp,counter;
    char d;

    for(i = 0; i < 16; i++)
    {
        d = font[i];
        temp = 0x80;
        counter = 0;
        
        /* ÒÆÎ»¼ì²âÏñËØ */
        while(temp>0)
        {
            if((d&temp) != 0)
                vram[(y+i)*xsize + x + counter] = c;

            temp = temp >> 1;
            counter++;
        }

    }
}
void putfont8_asc(char *vram, int xsize, int x, int y, char c, char *s)
{
    extern char hankaku[4096];
    int i = 0;
    for(; *s != '\0'; s++, i++)
    {
        putfont8(vram,xsize,x + i * 8,y,c,hankaku + *s * 16);

    }
}

void init_screen(char *vram, int xsize, int ysize)
{
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
    return;
}
void  init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:ºÚ */
		0xff, 0x00, 0x00,	/*  1:ºì */
		0x00, 0xff, 0x00,	/*  2:ÂÌ */
		0xff, 0xff, 0x00,	/*  3:»Æ */
		0x00, 0x00, 0xff,	/*  4:À¶ */
		0xff, 0x00, 0xff,	/*  5:×Ï */
		0x00, 0xff, 0xff,	/*  6:Ç³À¶ */
		0xff, 0xff, 0xff,	/*  7:°× */
		0xc6, 0xc6, 0xc6,	/*  8:»Ò */
		0x84, 0x00, 0x00,	/*  9:°µºì */
		0x00, 0x84, 0x00,	/* 10:°µÂÌ */
		0x84, 0x84, 0x00,	/* 11:°µ»Æ */
		0x00, 0x00, 0x84,	/* 12:°µÇà */
		0x84, 0x00, 0x84,	/* 13:°µ×Ï */
		0x00, 0x84, 0x84,	/* 14:Ç³°µÀ¶ */
		0x84, 0x84, 0x84	/* 15:°µ»Ò */
	};
    set_palette(0,15,table_rgb);
    return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for(i = start; i <= end; i++)
    {
        io_out8(0x03c9, rgb[0]/4);
        io_out8(0x03c9, rgb[1]/4);
        io_out8(0x03c9, rgb[2]/4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

/* 描绘矩形 */
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int i,j;
    for(i = x0; i <= x1; i++)
        for(j = y0; j <= y1; j++)
        {
            vram[j*xsize + i] = c;
        }

    return;
}

void init_mouse_cursor8(char *mouse, char bc)
/* Êó±êÖ¸Õë³õÊ¼»¯ */
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR*) 0x00270000;
    struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR*) 0x0026f800;
    int i;

    /* Set gdt */
    for (i = 0; i < 8192; ++i)
    {
        /* code */
        set_segmdesc(gdt+i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
    load_gdtr(0xffff, 0x00270000);

    /* set idt 设置*/
    for (i = 0; i < 256; ++i)
    {
        /* code */
        set_gatedesc(idt+i, 0, 0, 0);
    }
    set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 << 3, AR_INTGATE32);
    set_gatedesc(idt + 0x27, (int) asm_inthandler27, 2 << 3, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 << 3, AR_INTGATE32);
    load_idtr(0x7ff, 0x0026f800);
    return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff)
    {
        /* code */
        ar |= 0x8000;
        limit /= 0x1000;
    }

    sd->limit_low   = limit & 0xffff;
    sd->base_low    = base & 0xffff;
    sd->base_mid    = (base >> 16) & 0xff;
    sd->access_right= ar & 0xff;
    sd->limit_high  = ((limit >> 16) & 0xffff) | ((ar >> 8) & 0xf0);
    sd->base_high   = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;
    return;
}

/*************** int.c ******************/
void init_pic()
{
    io_out8(PIC0_IMR,  0xff  ); /* 禁止所有中断 */
    io_out8(PIC1_IMR,  0xff  ); /* 禁止所有中断 */

    io_out8(PIC0_ICW1, 0x11  ); /* 电气属性：边沿触发(猜测是上升沿) */
    io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT20-27接收 */
    io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2接收 */
    io_out8(PIC0_ICW4, 0x01  ); /* 无缓冲区 */

    io_out8(PIC1_ICW1, 0x11  ); /* 电气属性：边沿触发(猜测是上升沿) */
    io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15由INT28-2f接收 */
    io_out8(PIC1_ICW3, 2     ); /* PIC1由IRQ2接收 */
    io_out8(PIC1_ICW4, 0x01  ); /* 无缓冲区 */

    io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1以外中断全部禁止 */
    io_out8(PIC1_IMR,  0xff  ); /* 11111111 禁止所有中断 */

    return;
}

void inthandler21(int *esp)
{
    /* 键盘中断处理函数 */
    struct BOOTINFO *binfo = (struct BOOTINFO*) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
    for (;;)
    {
        /* code */
        io_hlt();
    }
}

void inthandler2c(int *esp)
{
    /* 鼠标中断处理函数 */
    struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;) {
        io_hlt();
    }
}

void inthandler27(int *esp)                                
{
    io_out8(PIC0_OCW2, 0x67); /* 兼容性设置 */
    return;
}