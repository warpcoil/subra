#include "kernel.h"
#include "registers.h"

extern "C" void IsrHandler(registers_t regs);

void IsrHandler(registers_t regs) {

    if (regs.interruptNo != 0)
        message("INT:: %i\n");

}
