#ifndef IDT_H
#define IDT_H

/* Fault Types
0	Divide By Zero or DIV/0
1	Debug Exception
2	Non Maskable Interrupt
3	Breakpoint Exception
4	Into Detected Overload
5	Out of Bounds Exception
6	Invalid OpCode Exception
7	No CoProcessor Exception
8	Double Fault ###
9	CoProcessor Segment Overrun
10	Bad TSS ?? Task Segment Scheduler??? ###
11	Segment Not Present ###
12	Stack Fault ###
13	General Protection Fault
14	Page Fault ###
15	Unknown Interrupt ###
16	CoProcessor Fault
17	Alignment Check Exception
18	Machine Check Exception
19-31	Reserved

###		Pushes Error Code
*/

#include "oslib/stdint.h"

//This is because IRQs are remapped
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

class IDT {

private:
    struct IdtEntry
    {
        uint16_t BaseLow;
        uint16_t SegmentSelector;
        uint8_t AlwaysZero;
        uint8_t Flags;
        uint16_t BaseHigh;
    } __attribute__((packed));

    struct IdtPointer
    {
        uint16_t Limit;
        uint32_t Base;
    } __attribute__((packed));

    struct IdtEntry Idt[256];
    struct IdtPointer Ipr;

    void IdtSetGate(uint8_t Number, uint32_t Base, uint16_t SegmentSelector, uint8_t Flags);

public:

    void Initialise();
    void StopInterrupts();
    void StartInterrupts();

};

#endif // IDT_H

