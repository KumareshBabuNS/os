#include "bootpack.h"
/*************** int.c ******************/
struct Queue8 keyBoardBuffer;
struct Queue8 mouseBuffer;
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
#define PORT_KEYDAT     0x0060


void inthandler21(int *esp)
{
    /* 键盘中断处理函数 */
    char data;
    char StrNum[5];
    
    struct BOOTINFO *binfo = (struct BOOTINFO*) ADR_BOOTINFO;
    io_out8(PIC0_OCW2, 0x61); /* 通知CPU中断请求已经处理完 */
    data = io_in8(PORT_KEYDAT);
    queue8_push(&keyBoardBuffer, data);
    //boxfill8(binfo->vram, binfo->scrnx, COL8_000084, 0, 0, 32 - 1, 16 - 1);
    //init_screen(binfo->vram,binfo->scrnx,binfo->scrny);
    //sprintf(StrNum, "%02X", data);
    //putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, StrNum);
    
    //putfont8_asc(binfo->vram, binfo->scrnx, 0, 10, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
    return;
}

void inthandler2c(int *esp)
{
    /* 鼠标中断处理函数 */
    /*struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
    putfont8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");*/
    unsigned char data;
    io_out8(PIC1_OCW2, 0x64);
    io_out8(PIC0_OCW2, 0x62);
    data = io_in8(PORT_KEYDAT);
    queue8_push(&mouseBuffer, data);
    return;
    
}

void inthandler27(int *esp)                                
{
    io_out8(PIC0_OCW2, 0x67); /* 兼容性设置 */
    return;
}