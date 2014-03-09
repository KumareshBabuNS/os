#include "bootpack.h"
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

