#include "support.h"
#include "kernel.h"

static v7_err js_msg(struct v7 *rt, v7_val_t *res) {

    size_t l;

    //varargs
    unsigned long numArgs = v7_argc(rt);
    v7_val_t valFormat = v7_arg(rt, 0);
    const char * format = v7_get_string(rt, &valFormat, &l);

    message("num args %lu", numArgs);

    message("%s", format);

    * res = v7_mk_number(rt, (double)l);

    return V7_OK;
}

Support::Result Support::JSInit() {

    //Initialise support functions such as in out etc
    enum v7_err rcode = V7_OK;
    rt = v7_create();

    v7_val_t result;

    v7_set_method(rt, v7_get_global(rt), "message", &js_msg);

    //Send (in javascript) to console "Hello World!" [test]
    rcode = v7_exec(rt, "message(\"Initialising JavaScript Runtime Components\n\");", &result);
    rcode = v7_exec(rt, "message(\"%s%i\n\", \"Hello\", 1);", &result);



    if (rcode != V7_OK) {
        return FAIL;
    }

    return OK;
    //v7_destroy(v7);
}
