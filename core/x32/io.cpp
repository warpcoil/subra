#include "kernel.h"
#include "io.h"

void IO::OutByte(uint16_t port, uint8_t value) {

    __asm__ __volatile__("outb %%al, %%dx" : : "d" (port), "a" (value));

}

void IO::OutWord(uint16_t port, uint16_t value) {

    __asm__ __volatile__("outw %%ax, %%dx" : : "d" (port), "a" (value));

}

void IO::OutDoubleWord(uint16_t port, uint32_t value) {

    __asm__ __volatile__("outl %%eax, %%dx" : : "d" (port), "a" (value));

}

uint8_t IO::InByte(uint16_t port) {

    uint8_t returnvalue;
    __asm__ __volatile__("inb %1, %0" : "=a" (returnvalue) : "Nd" (port));
    return returnvalue;

}

uint16_t IO::InWord(uint16_t port) {

    uint16_t returnvalue;
    __asm__ __volatile__("inw %1, %0" : "=a" (returnvalue) : "Nd" (port));
    return returnvalue;

}

uint32_t IO::InDoubleWord(uint16_t port) {

    uint32_t returnvalue;
    __asm__ __volatile__("inl %1, %0" : "=a" (returnvalue) : "Nd" (port));
    return returnvalue;

}
