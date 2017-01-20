#ifndef __JSSUPPORT_H__
#define __JSSUPPORT_H__

#include "mujs/mujs.h"
#include "support.h"

class JSSupport : public Support {
private:
    struct v7 * rt;
public:
    Result Init();
    Result Exec(char * jsCode, uint64_t * result);
};


#endif // SUPPORT

