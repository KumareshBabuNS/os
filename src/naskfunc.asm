; naskfunc
; TAB=4

[FORMAT "WCOFF"]				;制作目标文件模式
[BITS 32]						;目标机器语言是32位
[INSTRSET "i486p"]              ;告诉编译器程序将运行于486以上机器，即32位cpu

;制作目标文件信息

[FILE "naskfunc.nas"]			;源文件名

		GLOBAL	_io_hlt,_write_mem8			;程序中包含的函数名


;函数体

[SECTION .text]		;目标文件中写了这些后再写程序

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_write_mem8:
        mov     ecx, [esp+4]
        mov     al, [esp+8]
        mov     [ecx], al
        ret
