#include "jssupport.h"
#include "kernel.h"
#include "oslib/string.h"
#include "x16/registers.h"

#define STD_SIZE	0
#define SHORT_SIZE	1
#define LONG_SIZE	2

////- Calls internal message() function from JavaScript
static void js_message(js_State * state) {

    //varargs
    char * format = (char *)js_tostring(state, 1);

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
                cputc(js_tostring(state, arg)[0]);
                arg++;
                break;
            }
            case 's': { //String
                message("%s", js_tostring(state, arg));
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

                message(formatArgs, js_tointeger(state, arg));

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

    js_pushundefined(state);

}

static void js_malloc(js_State * state) {

    int32_t amt = js_toint32(state, 1);
    uint32_t where = (uint32_t)dlmalloc((size_t)amt);
    js_pushnumber(state, (double)where);

}

static void js_free(js_State * state) {

    uint32_t addr = js_touint32(state, 1);
    dlfree((void *)addr);
    js_pushundefined(state);

}

static void js_realloc(js_State * state) {

    uint32_t addr = js_touint32(state, 1);
    int32_t newSize = js_toint32(state, 2);
    uint32_t where = (uint32_t)dlrealloc((void *)addr, (size_t)newSize);
    js_pushnumber(state, (double)where);

}

static void js_writeInt8_t(js_State * state) {

    uint32_t where = js_touint32(state, 1);
    int8_t * w = (int8_t *)where;
    * w = (int8_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_writeInt16_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    int16_t * w = (int16_t *)where;
    * w = (int16_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_writeInt32_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    int32_t * w = (int32_t *)where;
    * w = (int32_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_writeUint8_t(js_State * state) {

    uint32_t where = js_touint32(state, 1);
    uint8_t * w = (uint8_t *)where;
    * w = (uint8_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_writeUint16_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    uint16_t * w = (uint16_t *)where;
    * w = (uint16_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_writeUint32_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    uint32_t * w = (uint32_t *)where;
    * w = (uint32_t)js_toint32(state, 2);
    js_pushundefined(state);

}

static void js_readInt8_t(js_State * state) {

    uint32_t where = js_touint32(state, 1);
    int8_t * w = (int8_t *)where;
    js_pushnumber(state, (double) * w);

}

static void js_readInt16_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    int16_t * w = (int16_t *)where;
    js_pushnumber(state, (double) * w);

}

static void js_readInt32_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    int32_t * w = (int32_t *)where;
    js_pushnumber(state, (double) * w);

}

static void js_readUint8_t(js_State * state) {

    uint32_t where = js_touint32(state, 1);
    uint8_t * w = (uint8_t *)where;
    js_pushnumber(state, (double) * w);

}

static void js_readUint16_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    uint16_t * w = (uint16_t *)where;
    js_pushnumber(state, (double) * w);

}

static void js_readUint32_t(js_State *state) {

    uint32_t where = js_touint32(state, 1);
    uint32_t * w = (uint32_t *)where;
    js_pushnumber(state, (double) * w);

}

RealMode * realMode;

static void js_real_set_register_value(js_State *state) {

    //register id
    uint32_t reg = js_touint32(state, 1);

    //16 bit value
    uint16_t value = js_touint16(state, 2);

    realMode->setRegister(reg, value);

    js_pushundefined(state);
}

static void js_real_get_register_value(js_State *state) {
    //register id
    uint32_t reg = js_touint32(state, 1);

    //16 bit value
    uint16_t value = realMode->getRegister(reg);

    js_pushnumber(state, (double)value);
}

static void js_real_call_bios(js_State *state) {

    int interrupt = js_toint32(state, 1);

    __asm__ __volatile("cli");
    realMode->interrupt(interrupt);
    __asm__ __volatile("hlt");
    __asm__ __volatile("sti");

    message("hereRWRG: %i ", interrupt);

    js_pushundefined(state);

    __asm__ __volatile__("hlt");

}

//    js_getindex(state, 1, 0);
//    if(!js_hasproperty(state, -1, "name")) {
//        message("Unknown Descriptor\n");
//    }
//    const char * name = js_tostring(state, -1);
//    //js_pop(state, -1);

//    js_getindex(state, 1, 0);
//    if(!js_hasproperty(state, -1, "type")) {
//        message("Unknown Descriptor\n");
//    }
//    int type = js_toint32(state, -1);
//    //js_pop(state, -1);

//    js_getindex(state, 1, 0);
//    if(!js_hasproperty(state, -1, "amount")) {
//        message("Unknown Descriptor\n");
//    }
//    int amount = js_toint32(state, -1);
//    //js_pop(state, -1);


////Emulated interrupt -- uses vm6 task, to register an interrupt
//static v7_err js_emulated_int(struct v7 *rt, v7_val_t *res) {



//}

JSSupport::Result JSSupport::Init() {

    //Initialise support functions such as in out etc
    if (state == NULL)
        state = js_newstate(NULL, NULL, JS_STRICT); //ES5 strict

    //Global dependency initialisation
    realMode = new RealMode();

    //Legacy mode message/printf [VGA]
    js_newcfunction(state, js_message, "message", MAXARGS);
    js_setglobal(state, "message");

    js_newcfunction(state, js_message, "printf", MAXARGS);
    js_setglobal(state, "printf");

    //malloc, free and realloc et al
    js_newcfunction(state, js_malloc, "malloc", 1);
    js_setglobal(state, "malloc");

    js_newcfunction(state, js_free, "free", 1);
    js_setglobal(state, "free");

    js_newcfunction(state, js_realloc, "realloc", 2);
    js_setglobal(state, "realloc");

    //mem writes and reads from js
    js_newcfunction(state, js_writeInt8_t, "writeInt8_t", 2);
    js_setglobal(state, "writeInt8_t");

    js_newcfunction(state, js_writeInt16_t, "writeInt16_t", 2);
    js_setglobal(state, "writeInt16_t");

    js_newcfunction(state, js_writeInt32_t, "writeInt32_t", 2);
    js_setglobal(state, "writeInt32_t");

    js_newcfunction(state, js_writeUint8_t, "writeUint8_t", 2);
    js_setglobal(state, "writeUint8_t");

    js_newcfunction(state, js_writeUint16_t, "writeUint16_t", 2);
    js_setglobal(state, "writeUint16_t");

    js_newcfunction(state, js_writeUint32_t, "writeUint32_t", 2);
    js_setglobal(state, "writeUint32_t");

    js_newcfunction(state, js_readInt8_t, "readInt8_t", 1);
    js_setglobal(state, "readInt8_t");

    js_newcfunction(state, js_readInt16_t, "readInt16_t", 1);
    js_setglobal(state, "readInt16_t");

    js_newcfunction(state, js_readInt32_t, "readInt32_t", 1);
    js_setglobal(state, "readInt32_t");

    js_newcfunction(state, js_readUint8_t, "readUint8_t", 1);
    js_setglobal(state, "readUint8_t");

    js_newcfunction(state, js_readUint16_t, "readUint16_t", 1);
    js_setglobal(state, "readUint16_t");

    js_newcfunction(state, js_readUint32_t, "readUint32_t", 1);
    js_setglobal(state, "readUint32_t");

    //real mode basic utilities [from modified oslib]

    js_newcfunction(state, js_real_set_register_value, "real_set_register_value", 2);
    js_setglobal(state, "real_set_register_value");

    js_newcfunction(state, js_real_get_register_value, "real_get_register_value", 1);
    js_setglobal(state, "real_get_register_value");

    js_newcfunction(state, js_real_call_bios, "real_call_bios", 1);
    js_setglobal(state, "real_call_bios");

    //Send (in javascript) to console "Javascript Runtime Message!" [test] Note:  You cannot embed \n
    js_dostring(state, "message('Initialising JavaScript Runtime Components' + String.fromCharCode(10));");

    //sizeof(struct descriptor)
//    js_newcfunction(state, js_sizeof, "sizeof", 1);
//    js_setglobal(state, "sizeof");

//    //serialise(js struct to packed type)
//    js_newcfunction(state, js_serialise, "serialise", 2);
//    js_setglobal(state, "serialise");

//    //deserialise(packed type to js struct)
//    js_newcfunction(state, js_deserialise, "deserialise", 2);
//    js_setglobal(state, "deserialise");
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

JSSupport::Result JSSupport::Exec(char * jsCode) {

    //Execute the UTF8/ASCII code within built in engine

    js_dostring(state, jsCode);

    return OK;
}


