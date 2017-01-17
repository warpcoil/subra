#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include "oslib/stdint.h"

class Support {
public:
    enum Result {
        OK, FAIL
    };

    virtual Result Init();
    virtual Result Exec(char * jsCode, uint64_t * result);
};

#endif // SUPPORT_H
