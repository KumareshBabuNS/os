#include "bootpack.h"

extern struct Queue8 keyBoardBuffer;
extern struct Queue8 mouseBuffer;
void init_keyboard();
void enable_mouse();

void HariMain(void)
{
    struct BOOTINFO *binfo;
    char *vram = (char *)0xa0000;
    unsigned char keyBuf[32]; 
    unsigned char mouseBuf[128];
    int xsize = 320;
    int ysize = 200;
    char mcursor[256];
    int mx,my;

    init_palette();
    init_gdtidt();
    init_pic();
    init_keyboard();
    enable_mouse();

    queue8_init(&keyBoardBuffer, 32, keyBuf);
    queue8_init(&mouseBuffer, 128, mouseBuf);
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
        if(queue8_status(&keyBoardBuffer) + queue8_status(&mouseBuffer) <= 0)
            io_stihlt();
        else
        {
            if (queue8_status(&keyBoardBuffer) > 0)
            {
                int data;
                char ch[100];

                data = queue8_pop(&keyBoardBuffer);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 32 - 1, 16 - 1);
                sprintf(ch, "%02X", data);
                putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFF00, ch);
            }
            
            else if (queue8_status(&mouseBuffer) > 0)
            {
                /* code */
                int data;
                char ch[100];

                data = queue8_pop(&mouseBuffer);
                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 20, 32 - 1, 20 + 16 - 1);
                sprintf(ch, "%02X", data);
                putfont8_asc(binfo->vram, binfo->scrnx, 0, 20, COL8_FFFF00, ch);
            }
             
        }

    }
    //io_stihlt();
}

#define PORT_KEYDAT             0x0060
#define PORT_KEYSTA             0x0064
#define PORT_KEYCMD             0x0064
#define KEYSTA_SEND_NOTREADY    0x02
#define KEYCMD_WRITE_MODE       0x60
#define KBC_MODE                0x47

void wait_KBC_sendready(void)
{
    /* 等待键盘电路 */
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void)
{
    /* 初始化键盘模式 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

#define KEYCMD_SENDTO_MOUSE     0xd4
#define MOUSECMD_ENABLE         0xf4

void enable_mouse(void)
{
    /* 激活鼠标 */
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return; 
}