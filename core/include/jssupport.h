#ifndef __JSSUPPORT_H__
#define __JSSUPPORT_H__

#include "mujs/mujs.h"
#include "oslib/defs.h"
#include "support.h"

#ifndef MAXARGS
#define MAXARGS 24
#endif

class JSSupport : public Support {
private:
    js_State * state = NULL;
public:
    Result Init();
    Result Exec(char * jsCode, uint64_t * result);
};


#endif // SUPPORT

