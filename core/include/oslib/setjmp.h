#ifndef SETJMP
#define SETJMP

#include "oslib/hw-data.h"

typedef struct _jmp_buf { unsigned char _jb[109]; } jmp_buf[1];

int setjmp(jmp_buf __jmpb);
void longjmp(jmp_buf __jmpb, int __ret);

#endif // SETJMP

