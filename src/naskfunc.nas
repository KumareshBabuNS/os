; naskfunc
; TAB=4

[FORMAT "WCOFF"]				;����Ŀ���ļ�ģʽ
[BITS 32]						;Ŀ�����������32λ
[INSTRSET "i486p"]              ;���߱���������������486���ϻ�������32λcpu

;����Ŀ���ļ���Ϣ

[FILE "naskfunc.nas"]			;Դ�ļ���
        	;�����а����ĺ�����

		GLOBAL	_io_hlt,_io_cli,_io_sti,_io_stihlt
        GLOBAL  _io_in8,_io_in16,_io_in32
        GLOBAL  _io_out8,_io_out16,_io_out32
        GLOBAL  _io_load_eflags,_io_store_eflags
        GLOBAL  _write_mem8		

;������

[SECTION .text]		;Ŀ���ļ���д����Щ����д����

_io_hlt:	; void io_hlt(void);
		HLT
		RET

;�����ж� void io_cli(void);
_io_cli:
        clt
        ret
; void io_sti(void);  ȡ���ж�����
_io_sti:
        sti
        ret
; void io_stihlt(void) �����ȴ��жϻ���
_io_stihlt:
        sti
        htl
        ret
        
    ; ����������� ;
        
; int io_in8(int port);
_io_in8 :
        mov     edx, [esp+4]
        mov     eax, 0
        in      al, dx
        ret
; int io_in16(int port);
_io_in16 :
        mov     edx, [esp+4]
        mov     eax, 0
        in      ax, dx
        ret
; int io_in32(int port);
_io_in32 :
        mov     edx, [esp+4]
        mov     eax, 0
        in      eax, dx
        ret
; void io_out8(int port,int data);
_io_out8 :
        mov     edx, [esp+4]
        mov     al, [esp+8]
        out     dx, al
        ret
; void io_out16(int port,int data);
_io_out16 :
        mov     edx, [esp+4]
        mov     ax, [esp+8]
        out     dx, ax
        ret
; void io_out32(int port,int data);
_io_out8 :
        mov     edx, [esp+4]
        mov     eax, [esp+8]
        out     dx, eax
        ret

; int io_load_eflags();
_io_load_eflags:
        pushfd
        pop     eax
        ret
; void io_store_eflags();
_io_store_eflags:
        mov     eax, [esp+4]
        push    eax
        popfd
        ret

_write_mem8:
        mov     ecx, [esp+4]
        mov     al, [esp+8]
        mov     [ecx], al
        ret

