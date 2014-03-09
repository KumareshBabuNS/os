#include <stdio.h>
#define TRUE 			0x1
#define FALSE 			0x0

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
void io_stihlt(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void init_screen(char *, int, int);
void set_palette(int start,int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

/* ÏÔÊ¾×Ö·ûÒÔ¼°×Ö·û´® */
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfont8_asc(char *vram, int xsize, int x, int y, char c, char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize);

/* GDTºÍIDTÉèÖÃ */
struct SEGMENT_DESCRIPTOR
{
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR
{
   	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

void init_gdtidt();
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd,unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e
/* ¿ªÊ¼Ö´ÐÐµÄº¯Êý */
struct BOOTINFO
{
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

/***************** int.c ***************************/

void init_pic();
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
void asm_inthandler21();
void asm_inthandler27();
void asm_inthandler2c();

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/***************** fifo.c ***************************/
struct Queue8
{
	unsigned char *buf;
	int toReadLoc, toWriteLoc, size, freeSpace, flag, empty;
};
void queue8_init(struct Queue8 *fifo, int size, unsigned char *buf);
int queue8_push(struct Queue8 *fifo, unsigned char data);
int queue8_pop(struct Queue8 *fifo);
int queue8_status(struct Queue8 *fifo);