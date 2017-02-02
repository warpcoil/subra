#ifndef TIMER
#define TIMER

#include "oslib/stdint.h"

//NOTE: Doe not use PIT

class Timer {
private:
    static uint64_t ticks;
    static uint32_t interval;
public:
    static void Increment();
    static uint64_t GetTicks();
    static void StopTimer(); //Very importantly stops Interrupts occuring during a critical phase
    static void StartTimer();
    static void SetInterval(uint32_t msec);
};

#endif // TIMER

