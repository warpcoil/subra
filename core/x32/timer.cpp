#include "kernel.h"
#include "timer.h"
#include "registers.h"

extern "C" void setTicks(registers_t regs);

void setTicks(registers_t regs) {
    //regs=regs;

    //Timer::Increment();

}

void Timer::Increment() {

    //ticks++;

}

uint64_t Timer::GetTicks() {

    //return ticks;

}

void Timer::StopTimer() {

}

void Timer::StartTimer() {

}

void Timer::SetInterval(uint32_t msec) {

}
