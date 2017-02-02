[BITS 32]
[GLOBAL GdtFlush]

GdtFlush:

mov eax, [esp+4] ;GDT Pointer Gpr passed a param
lgdt [eax] ;Load GDT Address

mov ax, 0x10
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
jmp 0x8:flush

flush:
	ret
