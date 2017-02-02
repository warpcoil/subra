#include "realmode.h"
#include "kernel.h"

//Embedded Modules
#include "x16/int16.h"

bool RealMode::isModuleLoaded(RealModeModule module) {

    if (loadedModule == module)
        return true;

    return false;

}

void RealMode::loadModule(RealModeModule module) {

    switch (module) {
    case NONE:
        message("RealMode::\"None\" Module Invalid\n\n");
        break;
    case INT16: {

        //Copy wherever int16.o is and put it into memory starting 0x700
        //It does not matter if it crosses the 0x7c00 boundary
        unsigned char * where = (unsigned char *)0x7c00;
        for (long int i=0; i<int16_o_size; i++) {
            * where = int16_o[i];
            where++;
        }

        loadedModule = INT16;
        break;
    }
    default:
        message("RealMode::Unknown Real Mode Module\n\n");
        break;
    }

}

extern "C" uint32_t int16elf(uint32_t interruptNo, uint32_t regsPtr);
#define int16 ((void (*)(uint8_t intnum, regs16_t *regs))0x7c00)

void RealMode::interrupt(uint8_t interruptNo) {

    if (!isModuleLoaded(INT16))
        loadModule(INT16);

    message("out");

    //uint32_t x = int16elf((uint32_t)interruptNo, (uint32_t)&regs);
    //int16(interruptNo, &regs);

    regs.ax = 0x0013;
        int16(0x10, &regs);

    //int16(&regs, interruptNo); params RTL
//    __asm__ __volatile__("movl %0, %%eax\n"
//                         "movl %1, %%ebx\n"
//                         "pushl %%eax\n"
//                         "pushl %%ebx\n"
//                         "jmp %2"
//                         : : "r"((uint32_t)interruptNo), "r"(&regs), "m"(where) : "%eax", "%ebx");


    message("valueOf: %i %i\n", regs.ax);
    for (;;);
}

void RealMode::setRegister(uint32_t reg, uint16_t value) {

    switch (reg) {
    case 97120: //'ax'
        regs.ax = value;
        break;
    case 98120: //'bx'
        regs.bx = value;
        break;
    case 99120: //'cx'
        regs.cx = value;
        break;
    case 100120: //'dx'
        regs.dx = value;
        break;
    case 115105: //'si'
        regs.si = value;
        break;
    case 100105: //'di'
        regs.di = value;
        break;
    case 101102: //'ef' aka eflags
        regs.ef = value;
        break;
    case 101115: //'es'
        regs.es = value;
        break;
    case 102115: //'fs'
        regs.fs = value;
        break;
    case 103115: //'gs'
        regs.gs = value;
        break;
    case 100115: //'ds'
        regs.ds = value;
        break;
    case 98112: //'bp'
        regs.bp = value;
        break;
    case 115112: //'sp'
        regs.sp = value;
        break;
    default:
        message("Register: %ui does not exist!", reg);
    }

}

uint16_t RealMode::getRegister(uint32_t reg) {

    uint16_t value = 0;

    switch (reg) {
    case 97120: //'ax'
        value = regs.ax;
        break;
    case 98120: //'bx'
        value = regs.bx;
        break;
    case 99120: //'cx'
        value = regs.cx;
        break;
    case 100120: //'dx'
        value = regs.dx;
        break;
    case 115105: //'si'
        value = regs.si;
        break;
    case 100105: //'di'
        value = regs.di;
        break;
    case 101102: //'ef' aka eflags
        value = regs.ef;
        break;
    case 101115: //'es'
        value = regs.es;
        break;
    case 102115: //'fs'
        value = regs.fs;
        break;
    case 103115: //'gs'
        value = regs.gs;
        break;
    case 100115: //'ds'
        value = regs.ds;
        break;
    case 98112: //'bp'
        value = regs.bp;
        break;
    case 115112: //'sp'
        value = regs.sp;
        break;
    default:
        message("Register: %ui does not exist!", reg);
    }

    return value;
}
