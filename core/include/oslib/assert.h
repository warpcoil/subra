#ifndef ASSERT
#define ASSERT

#include "error.h"
#include "types.h"

void assert(int32_t assertion) {
    if (assertion == 0) {
        message("Assertion Failed, Application Terminated");
        asm volatile("hlt");
    }
}

#endif // ASSERT

