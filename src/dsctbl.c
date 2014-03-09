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