/* �������� */

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start,int end, unsigned char *rgb);

/* ��ʼִ�еĺ��� */

void HariMain(void)
{
    int i;

    for(i=0xa0000; i <= 0xaffff; i++)
        (*(char *)i) = i&0x0f;
    for(;;)
	    io_hlt(); /* ִ��naskfun.nas�е�io_htl����*/ 

}
