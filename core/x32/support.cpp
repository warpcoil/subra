#include "support.h"
#include "kernel.h"

#define STD_SIZE	0
#define SHORT_SIZE	1
#define LONG_SIZE	2
static v7_err js_msg(struct v7 *rt, v7_val_t *res) {

    size_t l;

    //varargs
    unsigned long numArgs = v7_argc(rt);
    v7_val_t valFormat = v7_arg(rt, 0);
    const char * format = v7_get_string(rt, &valFormat, &l);

    int len = strlen(valFormat);

    unsigned parsing = 0;
    int w = 0, arg = 1, x = 0; size = 0;
    v7_val_t val;

    for (int i=0; i<len; i++) {
        if (format[i] == '%') {
            i++;

            parsing = 1;
            w = 10;
            /*prec = 4;*/
            size = STD_SIZE;
            flag = 0;

            switch (format[i]) {
            case '%':
                cputc('%');
                parsing = 0;
                break;
            case 'c':
                val = v7_arg(rt, arg);
                cputc(v7_get_string(rt, &val, &x)[0]);
                parsing = 0;
                arg++;
            case 'i':
            case 'd':
                val = v7_arg(rt, arg);
                message("%i", v7_get_int(rt, &val));
                arg++;
                break;
            case 'u':
                val = v7_arg(rt, arg);
                message("%u", (unsigned)v7_get_int(rt, &val));
                arg++;
                break;
            case 'x':
                val = v7_arg(rt, arg);
                message("%x", (unsigned)v7_get_int(rt, &val));
                arg++;
                break;
            case 's':
                val = v7_arg(rt, arg);
                message("%s", v7_get_string(rt, &val, &x));
                arg++;
                break;
            case 'l':
            case 'n':
            case 'h':
                break;

            default:
                break;
            }

        } else {
            cputc(format[i]);
        }
    }

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
