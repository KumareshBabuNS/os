; naskfunc
; TAB=4

[FORMAT "WCOFF"]				;����Ŀ���ļ�ģʽ
[BITS 32]						;Ŀ�����������32λ


;����Ŀ���ļ���Ϣ

[FILE "naskfunc.nas"]			;Դ�ļ���

		GLOBAL	_io_hlt			;�����а����ĺ�����


;������

[SECTION .text]		;Ŀ���ļ���д����Щ����д����

_io_hlt:	; void io_hlt(void);
		HLT
		RET
