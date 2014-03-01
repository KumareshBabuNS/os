; naskfunc
; TAB=4

[FORMAT "WCOFF"]				;制作目标文件模式
[BITS 32]						;目标机器语言是32位


;制作目标文件信息

[FILE "naskfunc.nas"]			;源文件名

		GLOBAL	_io_hlt			;程序中包含的函数名


;函数体

[SECTION .text]		;目标文件中写了这些后再写程序

_io_hlt:	; void io_hlt(void);
		HLT
		RET
