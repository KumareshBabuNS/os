
        
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
        mov     es, ax
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
        
        ;待机    
fin:
        hlt
        jmp     fin
        ;字符串信息,0结尾
msg:
        DB      0x0a, 0x0a
        DB      "Hello egg!"
        DB      0x0a
        DB      0
        ;写满512字节，组后是0x55aa结尾
        resb    0x7dfe-$
        DB      0x55, 0xaa
