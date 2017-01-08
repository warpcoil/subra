#include "oslib/math.h"
#include "oslib/hw-instr.h"
#include "oslib/error.h"
#include "oslib/stdint.h"

int isinf(double x) {
    union { uint64_t u; double f; } ieee754;
    ieee754.f = x;
    return ( (unsigned)(ieee754.u >> 32) & 0x7fffffff ) == 0x7ff00000 &&
           ( (unsigned)ieee754.u == 0 );
}

int isnan(double x) {
    union { uint64_t u; double f; } ieee754;
    ieee754.f = x;
    return ( (unsigned)(ieee754.u >> 32) & 0x7fffffff ) +
           ( (unsigned)ieee754.u != 0 ) > 0x7ff00000;
}

int signbit(double x) {
    union { uint64_t u; double f; } ieee754;
    ieee754.f = x;
    if (ieee754.u & 0x8000000000000000) //Check signed bit 64 for double
        return 1;

    return 0;
}
