
        CYLS    EQU     10
        org     0x7c00
        
        ;以下是fat12磁盘格式设置
        jmp     entry
    	DB		0x90
		DB		"HELLOIPL"		
		DW		512				
		DB		1				
		DW		1				
		DB		2				
		DW		224				
		DW		2880			
		DB		0xf0			
		DW		9				
		DW		18				
		DW		2				
		DD		0				
		DD		2880			
		DB		0,0,0x29		
		DD		0xffffffff		
		DB		"HELLO-OS   "	
		DB		"FAT12   "		
		RESB	18

        ;设置段寄存器
entry:
        mov     ax, 0
        mov     ss, ax
        mov     sp, 0x7c00
        mov     ds, ax
        
        ;磁盘内容读入内存0x8200处
        mov     ax, 0x0820
        mov     es, ax
        mov     cl, 0x2
        mov     ch, 0 & 0xff
        mov     dh, 0
        ;读入18个扇区
readloop:
        mov     si, 0

        ;若出现错误重试三次
retry:
        add     si, 1
        mov     ah, 0x02
        mov     al, 1
        mov     dl, 0
        mov     bx, 0
        int     0x13
        jnc     next
        cmp     si, 3
        jae     error
        mov     ah, 0
        mov     dl, 0
        int     0x13
        jmp     retry
        ;读入下一个扇区       
next:
        mov     ax, es
        add     ax, 0x020
        mov     es, ax
        add     cl, 1
        cmp     cl, 18
        jbe     readloop
        mov     cl, 1
        add     dh, 1
        cmp     dh, 2
        jb      readloop
        mov     dh, 0
        add     ch, 1
        cmp     ch, CYLS
        jb      readloop
        


        ;待机    
fin:
        hlt
        jmp     fin

        ;错误信息
error:
        mov     si,  msg
        ;显示字符串
putloop:
        mov     al, [si]
        add     si, 1
        cmp     al, 0
        je      fin
        mov     ah, 0x0e
        mov     bh, 0
        mov     bl, 1
        int     0x10
        jmp     putloop
        



        ;字符串信息,0结尾
msg:
        DB      0x0a, 0x0a
        DB      "Load Error!"
        DB      0x0a
        DB      0
        ;写满512字节，组后是0x55aa结尾
        resb    0x7dfe-$
        DB      0x55, 0xaa
