#ifndef SETJMP
#define SETJMP

#include "oslib/hw-data.h"

//Ported from musl
typedef unsigned long __jmp_buf[6];

typedef struct __jmp_buf_tag {
    __jmp_buf __jb;
    unsigned long __fl;
    unsigned long __ss[128/sizeof(long)];
} jmp_buf[1];

int setjmp (jmp_buf);
__attribute__((__noreturn__)) void longjmp (jmp_buf, int);

#endif // SETJMP

