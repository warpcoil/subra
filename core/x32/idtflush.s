[BITS 32]

[GLOBAL IdtFlush]

IdtFlush:
mov eax, [esp+4] ;Get Idt Parameter Passed
lidt [eax] ;Load IDT
ret
