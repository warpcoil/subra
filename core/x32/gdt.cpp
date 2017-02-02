#include "kernel.h"
#include "gdt.h"

extern "C" void GdtFlush(uint32_t ptr);

void GDT::GdtSetGate(int32_t Number, uint32_t Base, uint32_t Limit, uint8_t Access, uint8_t Granularity) {

    /* Set Up Descriptor Base Address */
    Gdt[Number].BaseLow = (Base & 0xffff);
    Gdt[Number].BaseMiddle = (Base >> 16) & 0xff;
    Gdt[Number].BaseHigh = (Base >> 24) & 0xff;

    /*Set Up Descriptor Limits */
    Gdt[Number].LimitLow = (Limit & 0xffff);
    Gdt[Number].Granularity = ((Limit >> 16) & 0x0f);

    /* Set Granularity and Access Flags */
    Gdt[Number].Granularity |= Granularity & 0xf0;
    Gdt[Number].Access = Access;

}

void GDT::Initialise() {

    // We must fix the GDT below 1Mb, we use the same GDT for 32 bit and 16 bit modes
    // We'll set it at 0x500, and Real Mode Modules at 0x600
    Gdt = (struct GdtEntry *)0x500;

    uint32_t szGdt = sizeof(struct GdtEntry) * 8; //aka Gdt[8]

    Gpr = (struct GdtPointer *)(szGdt + 0x500);

    /* Set up GdtPointer and Limit */
    Gpr->Limit = (sizeof(struct GdtEntry)* 8) - 1; //Get size of a Gdt Entry mul 3 Entries
    Gpr->Base = (uint32_t)0x500;

    //Set our Null Descriptor
    GdtSetGate(0, 0, 0, 0, 0);

    //Set Second Entry -- Code Segment
    //Base Address is 0
    //Limit is 4 Gb
    //4 Kb Granularity
    //32 Bit OpCode (eax etc . . )
    //CS (Code Segment Descriptor)
    GdtSetGate(1, 0x0, 0xffffffff, 0x9a, 0xcf);

    //Data Segment Descriptor
    GdtSetGate(2, 0x0, 0xffffffff, 0x92, 0xcf);

    //CS (Code Segment Descriptor [16 bit]
    GdtSetGate(3, 0x0, 0xffffffff, 0x9a, 0x0f);

    //DS (Data Segment Descriptor [16 bit]
    GdtSetGate(4, 0x0, 0xffffffff, 0x92, 0x0f);
    //User Mode Code Segment Descriptor
    //GdtSetGate(3, 0x0, 0xffffffff, 0xfA, 0xcf);

    //User Mode Data Segment Descriptor
    //GdtSetGate(4, 0x0, 0xffffffff, 0xf2 ,0xcf);

    //Blanks
    GdtSetGate(5, 0, 0, 0, 0);
    GdtSetGate(6, 0, 0, 0, 0);
    GdtSetGate(7, 0, 0, 0, 0);
    //Flush Out old Gdt and install the new one
    GdtFlush((uint32_t)Gpr);

}
