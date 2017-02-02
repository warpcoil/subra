#include "kernel.h"
#include "idt.h"

extern "C" void Isr0(); //Interrupt Service Routines
extern "C" void Isr1();
extern "C" void Isr2();
extern "C" void Isr3();
extern "C" void Isr4();
extern "C" void Isr5();
extern "C" void Isr6();
extern "C" void Isr7();
extern "C" void Isr8();
extern "C" void Isr9();
extern "C" void Isr10();
extern "C" void Isr11();
extern "C" void Isr12();
extern "C" void Isr13();
extern "C" void Isr14();
extern "C" void Isr15();
extern "C" void Isr16();
extern "C" void Isr17();
extern "C" void Isr18();
extern "C" void Isr19();
extern "C" void Isr20();
extern "C" void Isr21();
extern "C" void Isr22();
extern "C" void Isr23();
extern "C" void Isr24();
extern "C" void Isr25();
extern "C" void Isr26();
extern "C" void Isr27();
extern "C" void Isr28();
extern "C" void Isr29();
extern "C" void Isr30();
extern "C" void Isr31();
extern "C" void Irq0();
extern "C" void Irq1();
extern "C" void Irq2();
extern "C" void Irq3();
extern "C" void Irq4();
extern "C" void Irq5();
extern "C" void Irq6();
extern "C" void Irq7();
extern "C" void Irq8();
extern "C" void Irq9();
extern "C" void Irq10();
extern "C" void Irq11();
extern "C" void Irq12();
extern "C" void Irq13();
extern "C" void Irq14();
extern "C" void Irq15();
//extern void Int255();
extern "C" void IdtFlush(uint32_t ptr);

void IDT::IdtSetGate(uint8_t Number, uint32_t Base, uint16_t SegmentSelector, uint8_t Flags) {

    Idt[Number].BaseLow = Base & 0xffff;
    Idt[Number].BaseHigh = (Base >> 16) & 0xffff;

    Idt[Number].SegmentSelector = SegmentSelector;
    Idt[Number].AlwaysZero = 0;
    Idt[Number].Flags = Flags;

}

void IDT::Initialise() {

    Ipr.Limit = sizeof(struct IdtEntry)* 256 - 1;
    Ipr.Base = (uint32_t)&Idt;

    memset(&Idt, 0, sizeof(struct IdtEntry)* 256); //Clear memory

    //Remap PIC
    IO::OutByte(0x20, 0x11);
    IO::OutByte(0xa0, 0x11);
    IO::OutByte(0x21, 0x20);
    IO::OutByte(0xa1, 0x28);
    IO::OutByte(0x21, 0x04);
    IO::OutByte(0xa1, 0x02);
    IO::OutByte(0x21, 0x01);
    IO::OutByte(0xa1, 0x01);
    IO::OutByte(0x21, 0x00);
    IO::OutByte(0xa1, 0x00);

    IdtSetGate(0, (uint32_t)Isr0, 0x08, 0x8E);
    IdtSetGate(1, (uint32_t)Isr1, 0x08, 0x8E);
    IdtSetGate(2, (uint32_t)Isr2, 0x08, 0x8E);
    IdtSetGate(3, (uint32_t)Isr3, 0x08, 0x8E);
    IdtSetGate(4, (uint32_t)Isr4, 0x08, 0x8E);
    IdtSetGate(5, (uint32_t)Isr5, 0x08, 0x8E);
    IdtSetGate(6, (uint32_t)Isr6, 0x08, 0x8E);
    IdtSetGate(7, (uint32_t)Isr7, 0x08, 0x8E);
    IdtSetGate(8, (uint32_t)Isr8, 0x08, 0x8E);
    IdtSetGate(9, (uint32_t)Isr9, 0x08, 0x8E);
    IdtSetGate(10, (uint32_t)Isr10, 0x08, 0x8E);
    IdtSetGate(11, (uint32_t)Isr11, 0x08, 0x8E);
    IdtSetGate(12, (uint32_t)Isr12, 0x08, 0x8E);
    IdtSetGate(13, (uint32_t)Isr13, 0x08, 0x8E);
    IdtSetGate(14, (uint32_t)Isr14, 0x08, 0x8E);
    IdtSetGate(15, (uint32_t)Isr15, 0x08, 0x8E);
    IdtSetGate(16, (uint32_t)Isr16, 0x08, 0x8E);
    IdtSetGate(17, (uint32_t)Isr17, 0x08, 0x8E);
    IdtSetGate(18, (uint32_t)Isr18, 0x08, 0x8E);
    IdtSetGate(19, (uint32_t)Isr19, 0x08, 0x8E);
    IdtSetGate(20, (uint32_t)Isr20, 0x08, 0x8E);
    IdtSetGate(21, (uint32_t)Isr21, 0x08, 0x8E);
    IdtSetGate(22, (uint32_t)Isr22, 0x08, 0x8E);
    IdtSetGate(23, (uint32_t)Isr23, 0x08, 0x8E);
    IdtSetGate(24, (uint32_t)Isr24, 0x08, 0x8E);
    IdtSetGate(25, (uint32_t)Isr25, 0x08, 0x8E);
    IdtSetGate(26, (uint32_t)Isr26, 0x08, 0x8E);
    IdtSetGate(27, (uint32_t)Isr27, 0x08, 0x8E);
    IdtSetGate(28, (uint32_t)Isr28, 0x08, 0x8E);
    IdtSetGate(29, (uint32_t)Isr29, 0x08, 0x8E);
    IdtSetGate(30, (uint32_t)Isr30, 0x08, 0x8E);
    IdtSetGate(31, (uint32_t)Isr31, 0x08, 0x8E);
    //Hardware Interrupts
    IdtSetGate(32, (uint32_t)Irq0, 0x08, 0x8E);
    IdtSetGate(33, (uint32_t)Irq1, 0x08, 0x8E);
    IdtSetGate(34, (uint32_t)Irq2, 0x08, 0x8E);
    IdtSetGate(35, (uint32_t)Irq3, 0x08, 0x8E);
    IdtSetGate(36, (uint32_t)Irq4, 0x08, 0x8E);
    IdtSetGate(37, (uint32_t)Irq5, 0x08, 0x8E);
    IdtSetGate(38, (uint32_t)Irq6, 0x08, 0x8E);
    IdtSetGate(39, (uint32_t)Irq7, 0x08, 0x8E);
    IdtSetGate(40, (uint32_t)Irq8, 0x08, 0x8E);
    IdtSetGate(41, (uint32_t)Irq9, 0x08, 0x8E);
    IdtSetGate(42, (uint32_t)Irq10, 0x08, 0x8E);
    IdtSetGate(43, (uint32_t)Irq11, 0x08, 0x8E);
    IdtSetGate(44, (uint32_t)Irq12, 0x08, 0x8E);
    IdtSetGate(45, (uint32_t)Irq13, 0x08, 0x8E);
    IdtSetGate(46, (uint32_t)Irq14, 0x08, 0x8E);
    IdtSetGate(47, (uint32_t)Irq15, 0x08, 0x8E);
    //Software Interrupts
    //None Yet
    //IdtSetGate(255, (uint32_t)Int255, 0x08, 0x8E);

    IdtFlush((uint32_t)&Ipr);

}

void IDT::StopInterrupts() {

    __asm__ __volatile__("cli");

}

void IDT::StartInterrupts() {

    __asm__ __volatile__("sti");

}
