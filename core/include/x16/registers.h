#ifndef REGISTERS
#define REGISTERS

typedef struct __attribute__ ((packed)) {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, ef;
} regs16_t;

#endif // REGISTERS

