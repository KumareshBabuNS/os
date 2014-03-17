[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
	EXTERN	_init_palette
	EXTERN	_init_gdtidt
	EXTERN	_init_pic
	EXTERN	_keyBoardBuffer
	EXTERN	_queue8_init
	EXTERN	_mouseBuffer
	EXTERN	_init_screen
	EXTERN	_init_mouse_cursor8
	EXTERN	_putblock8_8
	EXTERN	_io_out8
	EXTERN	_io_cli
	EXTERN	_queue8_status
	EXTERN	_queue8_pop
	EXTERN	_boxfill8
	EXTERN	_sprintf
	EXTERN	_putfont8_asc
	EXTERN	_io_stihlt
	EXTERN	_io_in8
[FILE "bootpack.c"]
[SECTION .data]
LC0:
	DB	"%02X",0x00
[SECTION .text]
	GLOBAL	_HariMain
_HariMain:
	PUSH	EBP
	MOV	EBP,ESP
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	MOV	EBX,2
	SUB	ESP,648
	CALL	_init_palette
	CALL	_init_gdtidt
	CALL	_init_pic
	CALL	_init_keyboard
	CALL	_enable_mouse
	LEA	EAX,DWORD [-44+EBP]
	PUSH	EAX
	PUSH	32
	PUSH	_keyBoardBuffer
	CALL	_queue8_init
	LEA	EAX,DWORD [-172+EBP]
	PUSH	EAX
	PUSH	128
	PUSH	_mouseBuffer
	CALL	_queue8_init
	MOVSX	EAX,WORD [4084]
	MOVSX	EDX,WORD [4086]
	LEA	ECX,DWORD [-16+EAX]
	MOV	DWORD [-656+EBP],EDX
	MOV	EAX,ECX
	MOV	ESI,DWORD [-656+EBP]
	CDQ
	IDIV	EBX
	SUB	ESI,44
	MOV	EDI,EAX
	MOV	EAX,ESI
	CDQ
	IDIV	EBX
	PUSH	DWORD [-656+EBP]
	MOV	ESI,EAX
	LEA	EBX,DWORD [-428+EBP]
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_init_screen
	ADD	ESP,36
	PUSH	14
	PUSH	EBX
	CALL	_init_mouse_cursor8
	PUSH	16
	PUSH	EBX
	PUSH	ESI
	PUSH	EDI
	PUSH	16
	PUSH	16
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putblock8_8
	ADD	ESP,40
	PUSH	249
	PUSH	33
	CALL	_io_out8
	PUSH	239
	PUSH	161
	CALL	_io_out8
	ADD	ESP,16
L2:
	CALL	_io_cli
	PUSH	_keyBoardBuffer
	CALL	_queue8_status
	PUSH	_mouseBuffer
	MOV	EBX,EAX
	CALL	_queue8_status
	LEA	EAX,DWORD [EAX+EBX*1]
	POP	EBX
	POP	ESI
	TEST	EAX,EAX
	JLE	L11
	PUSH	_keyBoardBuffer
	CALL	_queue8_status
	POP	ECX
	TEST	EAX,EAX
	JG	L12
	PUSH	_mouseBuffer
	CALL	_queue8_status
	POP	EDX
	TEST	EAX,EAX
	JLE	L2
	PUSH	_mouseBuffer
	CALL	_queue8_pop
	PUSH	35
	PUSH	31
	MOV	EBX,EAX
	PUSH	20
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	ADD	ESP,32
	PUSH	EBX
	LEA	EBX,DWORD [-652+EBP]
	PUSH	LC0
	PUSH	EBX
	CALL	_sprintf
	PUSH	EBX
	PUSH	3
	PUSH	20
L10:
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8_asc
	ADD	ESP,36
	JMP	L2
L12:
	PUSH	_keyBoardBuffer
	CALL	_queue8_pop
	PUSH	15
	PUSH	31
	MOV	EBX,EAX
	PUSH	0
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	ADD	ESP,32
	PUSH	EBX
	LEA	EBX,DWORD [-540+EBP]
	PUSH	LC0
	PUSH	EBX
	CALL	_sprintf
	PUSH	EBX
	PUSH	3
	PUSH	0
	JMP	L10
L11:
	CALL	_io_stihlt
	JMP	L2
	GLOBAL	_wait_KBC_sendready
_wait_KBC_sendready:
	PUSH	EBP
	MOV	EBP,ESP
L14:
	PUSH	100
	CALL	_io_in8
	POP	EDX
	AND	EAX,2
	JNE	L14
	LEAVE
	RET
	GLOBAL	_init_keyboard
_init_keyboard:
	PUSH	EBP
	MOV	EBP,ESP
	CALL	_wait_KBC_sendready
	PUSH	96
	PUSH	100
	CALL	_io_out8
	CALL	_wait_KBC_sendready
	PUSH	71
	PUSH	96
	CALL	_io_out8
	LEAVE
	RET
	GLOBAL	_enable_mouse
_enable_mouse:
	PUSH	EBP
	MOV	EBP,ESP
	CALL	_wait_KBC_sendready
	PUSH	212
	PUSH	100
	CALL	_io_out8
	CALL	_wait_KBC_sendready
	PUSH	244
	PUSH	96
	CALL	_io_out8
	LEAVE
	RET
