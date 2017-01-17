#ifndef SUPPORT
#define SUPPORT

#include "v7/v7.h"

class Support {
private:
    struct v7 * rt;
public:
    enum Result {
        OK,
        FAIL
    };
    Result Init();
    Result Exec(char * jsCode, uint64_t * result);
};


#endif // SUPPORT

