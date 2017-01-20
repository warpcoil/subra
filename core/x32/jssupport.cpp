#include "jssupport.h"
#include "kernel.h"

#define STD_SIZE	0
#define SHORT_SIZE	1
#define LONG_SIZE	2

////- Calls internal message() function from JavaScript
static void js_message(js_State * J) {

    size_t l;

    //varargs
    char * format = (char *)js_tostring(J, 1);

    int len = strlen(format);

    int arg = 2;
    char formatArgs[9];

    for (int i = 0; i<len; i++) {
        if (format[i] == '%') {
            i++;

            switch (format[i]) {
            case '%': { //Modulus
                cputc('%');
                break;
            }
            case 'c': { //Char
                cputc(js_tostring(J, arg)[0]);
                arg++;
                break;
            }
            case 's': { //String
                message("%s", js_tostring(J, arg));
                arg++;
                break;
            }
            case 'i': //Integer
            case 'd': //Integer
            case 'u': //Unsigned Integer
            case 'x': //Hexadecimal
            case 'l': //Modifier - long
            case 'n': //Modifier - short
            case 'h': //Modifier - short
            case '+': //Modifier - +
            case '-': //Modifier - -
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0': {
                //Clear format string
                int j = 0;

                for (j = 0; j < 9; j++)
                    formatArgs[j] = 0;

                j = 1;
                formatArgs[0] = '%';

                //Build up format string upto Non-Modifier
                if ((format[i] != 'i') && (format[i] != 'd') && (format[i] != 'u') && (format[i] != 'x')) {
                    while (((format[i + j] != 'i') || (format[i + j] != 'd') || (format[i + j] != 'u') || (format[i + j] != 'x')) && (j < 8)) {
                        formatArgs[j] = format[i + j];
                        j++;
                    }
                    formatArgs[j] = format[i + j];
                }
                else {
                    formatArgs[j] = format[i];
                    j++;
                }

                message(formatArgs, js_tointeger(J, arg));

                i += j;

                arg++;
                break;
            }
            default:
                //Malformed
                message("Malformed Format Args\n");
            }

        }
        else {
            cputc(format[i]);
        }
    }

    js_pushnumber(J, (double)l);

}

////Structure translation
////Types supported [u]byte, [u]word, [u]doubleword and [u]quadword
//static v7_err js_sizeof(struct v7 * rt, v7_val_t * res) {

//    v7_val_t object = v7_arg(rt, 0);

//    if (!v7_is_object(object)) {
//       message("Malformed Structure Descriptor\n");
//       return V7_EXEC_EXCEPTION;
//    }

//    v7_val_t t = v7_get(rt, object, "0", NULL);

//    if (v7_is_undefined(t)) {
//                message("Undefined Object\n");
//    }

//    if (!v7_is_object(t)) {
//        message("Not An Array\n");
//        return V7_EXEC_EXCEPTION;
//    }

//    t = v7_get(rt, t, "VBEVesaInfo[0][name]", NULL);

//    if (v7_is_undefined(t)) {
//                message("nUndefined Object\n");
//    }

//    if (!v7_is_array(rt, t)) {
//        message("Not An Array\n");
//        return V7_EXEC_EXCEPTION;
//    }

//    //Get array length
//    //unsigned long len = v7_array_length(rt, arr);

//    //message("lenarr: %i\n", len);

////    unsigned long structLength = 0;
////    v7_val_t member;
////    v7_val_t object;
////    int type;

////    size_t temp;

////    for (unsigned long i=0; i<3; i++) {
////        //Each object in array has member type, length and name to bind to
////        member = v7_array_get(rt, arr, i);

////        if (v7_is_object(arr)) {
////            message("Undefined Object\n");
////        }

////        object = v7_get(rt, member, "name", NULL);

////        if (v7_is_undefined(object))
////            message("errrr");

////        char * s = (char *)v7_get_string(rt, &object, &temp);
////        //type = fixint(v7_get_int(rt, object));
////        //type = v7_get_int(rt, object);

////        message("type: %s\n", s);

//////        switch (type) {
//////        //TODO:: Fix Negative Numbers
//////            case -1:
//////                //message("int int8_t");
//////            break;
//////        default:
//////            message("Unknown Base Type %i\n", type);
//////        }
////    }

//    //v7_val_t el0 = v7_array_get(rt, )
//    * res = v7_mk_number(rt, 22);

//    return V7_OK;
//}

////Structure serialisation -- from js object descriptor, create a structure and serialise at addr
//static v7_err js_serialise(struct v7 *rt, v7_val_t * res) {

//}

////Structure deserialisation -- from __attribute__packed typedef, to js object, based on js object descriptor
//static v7_err js_deserialise(struct v7 *rt, v7_val_t * res) {

//}

////Emulated interrupt -- uses vm6 task, to register an interrupt
//static v7_err js_emulated_int(struct v7 *rt, v7_val_t *res) {



//}

JSSupport::Result JSSupport::Init() {

    //Initialise support functions such as in out etc
    if (state == NULL)
        state = js_newstate(NULL, NULL, JS_STRICT); //ES5 strict

    js_newcfunction(state, js_message, "message", MAXARGS);
    js_setglobal(state, "message");
    js_newcfunction(state, js_message, "printf", MAXARGS);
    js_setglobal(state, "printf");

    //Send (in javascript) to console "Hello World!" [test]
    js_dostring(state, "message(\"Initialising JavaScript Runtime Components\n\");");

//    v7_val_t global = v7_get_global(rt);

//    //sizeof(struct descriptor)
//    v7_set_method(rt, global, "sizeof", &js_sizeof);
//    //serialise(js struct to packed type)
//    v7_set_method(rt, global, "serialise", &js_serialise);
//    //deserialise(packed type to js struct)
//    v7_set_method(rt, global, "deserialise", &js_deserialise);
//    //perform interrupt under emulation
//    v7_set_method(rt, global, "emulatedInt", &js_emulated_int);

//    //Just for testing, print a message in printf format, and do some math
//    //It works fine, so we'll just comment it out
//    //rcode = v7_exec(rt, "message(\"%s%i\n\", \"Hello\", 2+2);", &result);

//	if (rcode != V7_OK) {
//        message("Unable to initialise JavaScript Runtime");
//		return FAIL;
//	}

    return OK;
    //v7_destroy(v7); -- TODO:: AT POWERDOWN
}

JSSupport::Result JSSupport::Exec(char * jsCode, uint64_t * result) {

//    //Execute the UTF8/ASCII code within built in engine
//    //Check for success
//    enum v7_err rcode = V7_OK;

//    rcode = v7_exec(rt, jsCode, result);

//    if (rcode != V7_OK) {
//        //Log error
//        message("Support::JSExec: %s\n", v7_get_parser_error(rt));
//        return FAIL;
//    }

//    return OK;
}


