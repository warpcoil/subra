#ifndef REALMODE
#define REALMODE

#include "oslib/stdint.h"
#include "x16/registers.h"

class RealMode {
private:

    enum RealModeModule {
        NONE = 0,
        INT16 = 1
    };

    RealModeModule loadedModule = NONE; //No modules are loaded at first

    bool isModuleLoaded(RealModeModule module);
    void loadModule(RealModeModule module);

    regs16_t regs;

public:

    void interrupt(uint8_t interruptNo);
    void setRegister(uint32_t reg, uint16_t value);
    uint16_t getRegister(uint32_t reg);

};

#endif // REALMODE

