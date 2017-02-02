[BITS 32]
[EXTERN IsrHandler]
;[GLOBAL Int255]
;Int255:
;cli
;push byte 0
;push 255
;jmp ServiceHandler

%macro ISRNOERRORCODE 1
	[GLOBAL Isr%1]
	Isr%1:
	cli
	push byte 0			;Dummy Error Code 0
	push %1				;Interrupt Identifier
	jmp IsrCommonStub
%endmacro

%macro ISRERRORCODE 1
	[GLOBAL Isr%1]
	Isr%1:
	cli
	push byte 0			;Interrupt Identifier (Err Code already on Stack
	jmp IsrCommonStub
%endmacro

%macro ISHARDWAREINT 2
	[GLOBAL Irq%1]
	Irq%1:
	cli
	push byte 0
	push byte %2
	jmp IsrCommonStub
%endmacro

ISRNOERRORCODE 0
ISRNOERRORCODE 1
ISRNOERRORCODE 2
ISRNOERRORCODE 3
ISRNOERRORCODE 4
ISRNOERRORCODE 5
ISRNOERRORCODE 6
ISRNOERRORCODE 7
ISRERRORCODE 8
ISRNOERRORCODE 9
ISRERRORCODE 10
ISRERRORCODE 11
ISRERRORCODE 12
ISRERRORCODE 13
ISRERRORCODE 14
ISRNOERRORCODE 15
ISRNOERRORCODE 16
ISRERRORCODE 17
ISRNOERRORCODE 18
ISRNOERRORCODE 19
ISRNOERRORCODE 20
ISRNOERRORCODE 21
ISRNOERRORCODE 22
ISRNOERRORCODE 23
ISRNOERRORCODE 24
ISRNOERRORCODE 25
ISRNOERRORCODE 26
ISRNOERRORCODE 27
ISRNOERRORCODE 28
ISRNOERRORCODE 29
ISRERRORCODE 30
ISRNOERRORCODE 31
ISHARDWAREINT 0, 32
ISHARDWAREINT 1, 33
ISHARDWAREINT 2, 34
ISHARDWAREINT 3, 35
ISHARDWAREINT 4, 36
ISHARDWAREINT 5, 37
ISHARDWAREINT 6, 38
ISHARDWAREINT 7, 39
ISHARDWAREINT 8, 40
ISHARDWAREINT 9, 41
ISHARDWAREINT 10, 42
ISHARDWAREINT 11, 43
ISHARDWAREINT 12, 44
ISHARDWAREINT 13, 45
ISHARDWAREINT 14, 46
ISHARDWAREINT 15, 47

IsrCommonStub:
	pusha	;Push edi esi ebp esp ebx edx ecx eax on to stack

	mov ax, ds	;Move Lower 16 bits of ds into ax
	push eax	;Push 32 bit (ds -data segment descr) onto stack

	mov ax, 0x10 ;As with gdtflush
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	;mov eax, cr3
	;push eax
	;mov eax, KernelPageAddr
	;mov cr3, eax

	call IsrHandler

	;pop eax
	;mov cr3, eax

	pop eax	;Retreive dss
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8	;Cleans up pushed error code and pushed error no
	sti	;	Restart Interrupts
	iret	;Pops CS, EIP, EFLAGS, SS and ESP return

IrqCommonStub:
	pusha	;Push edi esi ebp esp ebx edx ecx eax on to stack

	mov ax, ds	;Move Lower 16 bits of ds into ax
	push eax	;Push 32 bit (ds -data segment descr) onto stack

	mov ax, 0x10 ;As with gdtflush
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call IsrHandler

	pop eax	;Retreive dss
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8	;Cleans up pushed error code and pushed error no
	sti	;	Restart Interrupts
	iret	;Pops CS, EIP, EFLAGS, SS and ESP return
