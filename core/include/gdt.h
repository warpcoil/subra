#ifndef GDT_H
#define GDT_H

#include "oslib/stdint.h"

class GDT {

private:
    struct GdtEntry
    {
        uint16_t LimitLow;
        uint16_t BaseLow;
        uint8_t BaseMiddle;
        uint8_t Access;
        uint8_t Granularity;
        uint8_t BaseHigh;
    } __attribute__((packed));

    struct GdtPointer //A stucture of Pointers
    {
        uint16_t Limit;
        uint32_t Base;
    } __attribute__((packed));

    //Create 3 GDT Entries and 1 Pointer for Entry [0]
    //struct GdtEntry Gdt[8];
    //struct GdtPointer Gpr;

    struct GdtEntry * Gdt;
    struct GdtPointer * Gpr;

private:
    void GdtSetGate(int32_t Number, uint32_t Base, uint32_t Limit, uint8_t Access, uint8_t Granularity);
public:
    void Initialise();

};

#endif // GDT_H

