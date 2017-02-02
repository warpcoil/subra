#ifndef REGISTERS_H
#define REGISTERS_H

#include "oslib/stdint.h"

typedef struct registers_t {
    uint32_t params;
    uint32_t ds;
    uint32_t edi, esi, ebp, reserved, ebx, edx, ecx, eax;
    uint32_t interruptNo, errorCode;
    uint32_t eip, cs, eflags, esp, ss;
} registers_t;

#endif // REGISTERS_H

