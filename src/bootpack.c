#include<stdio.h>
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
/* º¯ÊýÉùÃ÷ */

void io_hlt(void);
void io_cli(void);
void io_iosti(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void init_screen(char *, int, int);
void set_palette(int start,int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char c, char *s);
/* ¿ªÊ¼Ö´ÐÐµÄº¯Êý */
struct BOOTINFO
{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};

void HariMain(void)
{
    extern char hankaku[4096];
    struct BOOTINFO *binfo;
    int i;
    char *vram = (char *)0xa0000;
    int xsize = 320;
    int ysize = 200;
    
    init_palette();
    binfo = (struct BOOTINFO*)0x0ff0;
    xsize = binfo->scrnx;
    ysize = binfo->scrny;
    vram = binfo->vram;
    /* Ãè»æUI */
    init_screen(binfo->vram,binfo->scrnx,binfo->scrny);

    char str[100]; 
    sprintf(str,"%x xiaomengmeng",0x20);
    putfont8_asc(binfo->vram,binfo->scrnx,10,10,COL8_FFFFFF,str);
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
        putfont8(vram,xsize,x + i * 10,y,c,hankaku + *s * 16);

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

/* Ãè»æ¾ØÐÎ */
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
