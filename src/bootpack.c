#include "bootpack.h"
extern struct Queue8 keyBoardBuffer;
void HariMain(void)
{
    struct BOOTINFO *binfo;
    char *vram = (char *)0xa0000;
    unsigned char keyBuf[32]; 
    int xsize = 320;
    int ysize = 200;
    char mcursor[256];
    int mx,my;

    init_palette();
    init_gdtidt();
    init_pic();

    queue8_init(&keyBoardBuffer, 32, keyBuf);
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

    io_out8(PIC0_IMR, 0xf9);  /* 允许1号键盘中断和2号来自从PIC的中断 */
    io_out8(PIC1_IMR, 0xef);  /* 允许12号鼠标中断信号 */

    for(;;)
	{
        io_cli();
        if(queue8_status(&keyBoardBuffer) <= 0)
            io_stihlt();
        else
        {
            int data;
            char ch[100];

            data = queue8_pop(&keyBoardBuffer);
            boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 32 - 1, 16 - 1);
            sprintf(ch, "%02X", data);
            putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFF00, ch);
             // boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 18, 32 * 5 - 1, 18 + 16 - 1);
             // sprintf(ch, "%02X %02X %02X %02X", keyBoardBuffer.size, 
             //     keyBoardBuffer.freeSpace, keyBoardBuffer.toReadLoc, keyBoardBuffer.toWriteLoc);
             // putfont8_asc(binfo->vram, binfo->scrnx, 0, 18, COL8_FFFFFF, ch);
        }

    }
    //io_stihlt();
}

