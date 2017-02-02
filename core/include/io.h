#ifndef IO_H
#define IO_H

#include "oslib/stdint.h"

class IO {

public:

    static void OutByte(uint16_t port, uint8_t value);
    static void OutWord(uint16_t port, uint16_t value);
    static void OutDoubleWord(uint16_t port, uint32_t value);
    static uint8_t InByte(uint16_t port);
    static uint16_t InWord(uint16_t port);
    static uint32_t InDoubleWord(uint16_t port);

};

#endif // IO_H

