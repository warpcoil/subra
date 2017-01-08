#include "oslib/setjmp.h"
#include "oslib/error.h"

int setjmp(jmp_buf __jmpb) {
    __jmpb = __jmpb;
    message("STUB: Set Jump Called\n");
    return 0;
}

void longjmp(jmp_buf __jmpb, int __ret) {
    __jmpb = __jmpb;
    __ret = __ret;
    message("STUB: Long Jump Called\n");
}
