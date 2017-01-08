#ifndef COMMANDLINE
#define COMMANDLINE

/* The kernel accepts command line arguments, they are NOT required except for overrides */

//Problem - computer screen resolution approx 1280 x xxx, qemu 2560(ish) x xxx
//Override, as the OS will detect the best possible mode

//Command line arguments are formed of key value pairs (always!)

//  gfx=vesalfb
//  gfxmode=280 (or other official VBE mode)

#include <stdint.h>

class CommandLine {
private:
    int getSplitPoint(char * argv);
public:
    void RegisterCommandLine(int argc, char * argv[]);
    int HasArgument(char * arg);
    char * GetValue(uint16_t index);

};

#endif // COMMANDLINE

