[bits 32]
[org 0x0800]
[global int16]
jmp int16

; This structure is an assembler representation of the one defined in the header file regs16_t
struc regs16_t
    .di resw 1
    .si resw 1
    .bp resw 1
    .sp resw 1
    .bx resw 1
    .dx resw 1
    .cx resw 1
    .ax resw 1
    .ss resw 1
    .gs resw 1
    .fs resw 1
    .es resw 1
    .ds resw 1
    .cs resw 1
    .ef resw 1
    .size:
endstruc
		;Store 32 Bit IDT (With Initialiser)
interrupt	db		0x00		;Store Interrupt Number To Call
regs16ptr	dd		0x00000000	;Store regs16_t pointer

regs16: resb regs16_t.size

%define CODEBASE    0x0800 ;Ok, code for each real mode module starts executing at 0x600
%define DATABASE    0x0600 ;We, reserve 512 bytes for data [fixed] (0x0800 - 0x0600)
;Remember the GDT is stored at 0x500!
;This could be used for VESA data or whatever

copyStruct: use32

    mov ax, [ecx + regs16_t.di]
    mov [edx + regs16_t.di], ax
    
    mov ax, [ecx + regs16_t.si]
    mov [edx + regs16_t.si], ax
    
    mov ax, [ecx + regs16_t.bp]
    mov [edx + regs16_t.bp], ax
    
    mov ax, [ecx + regs16_t.sp]
    mov [edx + regs16_t.sp], ax
    
    mov ax, [ecx + regs16_t.bx]
    mov [edx + regs16_t.bx], ax
    
    mov ax, [ecx + regs16_t.dx]
    mov [edx + regs16_t.dx], ax
    
    mov ax, [ecx + regs16_t.cx]
    mov [edx + regs16_t.cx], ax
    
    mov ax, [ecx + regs16_t.ax]
    mov [edx + regs16_t.ax], ax
    
    mov ax, [ecx + regs16_t.ss]
    mov [edx + regs16_t.ss], ax
    
    mov ax, [ecx + regs16_t.gs]
    mov [edx + regs16_t.gs], ax
    
    mov ax, [ecx + regs16_t.fs]
    mov [edx + regs16_t.fs], ax
    
    mov ax, [ecx + regs16_t.ds]
    mov [edx + regs16_t.ds], ax
    
    mov ax, [ecx + regs16_t.cs]
    mov [edx + regs16_t.cs], ax
    
    mov ax, [ecx + regs16_t.ef] ; eflags btw
    mov [edx + regs16_t.ef], ax
    
    ret

int16: use32
    cli					; Stop Interrupts
    push	ebp             ; Save ebp on the stack
    mov	ebp, esp        ; Replace ebp with esp since we will be using
                        ; ebp as the base ponter for the functions
                        ; stack.
                        ;
                        ; The arguments start at ebp+8 since calling the
                        ; the function places eip on the stack and the
                        ; function places ebp on the stack as part of
                        ; the preamble.

    mov	eax, [ebp+12]	; regs16 ptr (packed)
    mov	[regs16ptr], eax
    mov	eax, [ebp+8]	; Interrupt Number
    mov	[interrupt], al
    ;push edi			; Store index registers
    ;push esi

;Copy data from ptr, to our struc (below 1Mb), regs16ptr is GUARANTEED to be higher than 1Mb

    mov edx, regs16	    ;Our struct [fixed < 1M] - destination
    mov ecx, [regs16ptr]  ;The ptr of the passed struct [could be anywhere] - source

    call copyStruct
    
    sidt [idt32_ptr]

;jmp to 16 bit protected mode
    mov ebx, regs16
    mov ax, [ebx + regs16_t.cs]
;    jmp ax:protectedMode16

;Set 16 bit data segment
protectedMode16: use16
    mov bx, regs16
    mov ax, [bx + regs16_t.ss]
    mov ss, ax
    
    mov ax, [bx + regs16_t.gs]
    mov gs, ax
    
    mov ax, [bx + regs16_t.fs]
    mov fs, ax
    
    mov ax, [bx + regs16_t.es]
    mov es, ax
    
    mov ax, [bx + regs16_t.ds]
    mov ds, ax

    mov eax, cr0
    and  al,  ~0x01                        ; mask off PE bit to turn off protected mode
    mov  cr0, eax                          ; set cr0 to result
    jmp word 0x0000:realMode16
    
realMode16: use16
    xor  ax, ax                            ; set ax to zero
    mov  ds, ax                            ; set ds so we can access idt16
    mov  ss, ax                            ; set ss so they the stack is valid
;    lidt idt16_ptr
    mov  bx, 0x0870                        ; master 8 and slave 112
    call resetpic                          ; set pic's the to real-mode settings
    
    
resetpic:                                  ; reset's 8259 master and slave pic vectors
    push ax                                ; expects bh = master vector, bl = slave vector
    mov  al, 0x11                          ; 0x11 = ICW1_INIT | ICW1_ICW4
    out  0x20, al                          ; send ICW1 to master pic
    out  0xA0, al                          ; send ICW1 to slave pic
    mov  al, bh                            ; get master pic vector param
    out  0x21, al                          ; send ICW2 aka vector to master pic
    mov  al, bl                            ; get slave pic vector param
    out  0xA1, al                          ; send ICW2 aka vector to slave pic
    mov  al, 0x04                          ; 0x04 = set slave to IRQ2
    out  0x21, al                          ; send ICW3 to master pic
    shr  al, 1                             ; 0x02 = tell slave its on IRQ2 of master
    out  0xA1, al                          ; send ICW3 to slave pic
    shr  al, 1                             ; 0x01 = ICW4_8086
    out  0x21, al                          ; send ICW4 to master pic
    out  0xA1, al                          ; send ICW4 to slave pic
    pop  ax                                ; restore ax from stack
    ret                                    ; return to caller
    
;Perform the interrupt

;Set 32 bit code segment

;Set 32 bit data segment:flush

;Set values in the regs16ptr > 1Mb

    mov edx, [regs16ptr]
    mov ecx, regs16
        
    call copyStruct
        
;return
    ;pop esi
    ;pop edi
    mov eax, [regs16]
    pop ebp		;
    ret

stack32_ptr:                               ; address in 32bit stack after we
    dd 0x00000000                          ;   save all general purpose registers
         
idt32_ptr:                                 ; IDT table pointer for 32bit access
        dw 0x0000                              ; table limit (size)
        dd 0x00000000                          ; table base address
         
;gdt32_ptr:                                 ; GDT table pointer for 32bit access
;        dw 0x0000                              ; table limit (size)
;        dd 0x00000000                          ; table base address
         
idt16_ptr:                                 ; IDT table pointer for 16bit access
        dw 0x03FF                              ; table limit (size)
        dd 0x00000000                          ; table base address
         
;gdt16_base:                                ; GDT descriptor table
;    .null:                                 ; 0x00 - null segment descriptor
;        dd 0x00000000                      ; must be left zero'd
;        dd 0x00000000                      ; must be left zero'd
;             
;    .code32:                               ; 0x01 - 32bit code segment descriptor 0xFFFFFFFF
;        dw 0xFFFF                          ; limit  0:15
;        dw 0x0000                          ; base   0:15
;        db 0x00                            ; base  16:23
;        db 0x9A                            ; present, iopl/0, code, execute/read
;        db 0xCF                            ; 4Kbyte granularity, 32bit selector; limit 16:19
;        db 0x00                            ; base  24:31
;             
;    .data32:                               ; 0x02 - 32bit data segment descriptor 0xFFFFFFFF
;        dw 0xFFFF                          ; limit  0:15
;        dw 0x0000                          ; base   0:15
;        db 0x00                            ; base  16:23
;        db 0x92                            ; present, iopl/0, data, read/write
;        db 0xCF                            ; 4Kbyte granularity, 32bit selector; limit 16:19
;        db 0x00                            ; base  24:31
;             
;    .code16:                               ; 0x03 - 16bit code segment descriptor 0x000FFFFF
;        dw 0xFFFF                          ; limit  0:15
;        dw 0x0000                          ; base   0:15
;        db 0x00                            ; base  16:23
;        db 0x9A                            ; present, iopl/0, code, execute/read
;        db 0x0F                            ; 1Byte granularity, 16bit selector; limit 16:19
;        db 0x00                            ; base  24:31
;             
;    .data16:                               ; 0x04 - 16bit data segment descriptor 0x000FFFFF
;        dw 0xFFFF                          ; limit  0:15
;        dw 0x0000                          ; base   0:15
;        db 0x00                            ; base  16:23
;        db 0x92                            ; present, iopl/0, data, read/write
;        db 0x0F                            ; 1Byte granularity, 16bit selector; limit 16:19
;        db 0x00                            ; base  24:31
;             
;gdt16_ptr:                                 ; GDT table pointer for 16bit access
;        dw gdt16_ptr - gdt16_base - 1          ; table limit (size)
;        dd gdt16_base                          ; table base address