; naskfunc
; TAB=4

[FORMAT "WCOFF"]				;����Ŀ���ļ�ģʽ
[BITS 32]						;Ŀ�����������32λ
[INSTRSET "i486p"]              ;���߱���������������486���ϻ�������32λcpu

;����Ŀ���ļ���Ϣ

[FILE "naskfunc.nas"]			;Դ�ļ���

		GLOBAL	_io_hlt,_write_mem8			;�����а����ĺ�����


;������

[SECTION .text]		;Ŀ���ļ���д����Щ����д����

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_write_mem8:
        mov     ecx, [esp+4]
        mov     al, [esp+8]
        mov     [ecx], al
        ret
