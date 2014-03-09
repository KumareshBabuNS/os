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

