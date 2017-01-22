#include "jssupport.h"
#include "kernel.h"
#include "oslib/string.h"

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

void emulate_int(uint32_t intnum, struct registers r) {

    TSS *vm86_tss;
    DWORD *bos;
    DWORD isr_cs, isr_eip;
    WORD *old_esp;
    DWORD *IRQTableEntry;
    CONTEXT c = get_TR();

    vm86_tss = vm86_get_tss();
    bos = (DWORD *)vm86_tss->esp0;
    if (c == X_VM86_TSS)
    {
            old_esp = (WORD *)(*(bos - 6) + (*(bos - 5) << 4));
            r.flags = CPU_FLAG_VM | CPU_FLAG_IOPL;
            *(old_esp - 2) = (WORD)(*(bos - 8));
            *(old_esp - 3) = (WORD)(*(bos - 9));
            *(bos - 6) -= 6;

            IRQTableEntry = (DWORD *)(0L); //intnum -- 0x6d
            isr_cs = ((IRQTableEntry[0x6d]) & 0xffff0000) >> 16;
            isr_eip = ((IRQTableEntry[0x6d]) & 0x0000ffff);

            *(bos - 8) = isr_cs;
            *(bos - 9) = isr_eip;
    }

}

static void js_vm86_init(js_State *state) {

    vm86_init();

    //Get interrupt to bind or undefined
    if (!js_isundefined(state, 1))
    {
        int interrupt = js_toint32(state, 1);
        l1_int_bind(interrupt /*interrupt*/, (void *)emulate_int);
    }

    js_pushundefined(state);

}

X_REGS16 v86_regs_out;
X_REGS16 v86_regs_in;
X_SREGS16 v86_sregs;

static void js_vm86_set_register_value(js_State *state) {

    //register id
    uint32_t regId = js_touint32(state, 1);
    //16 bit value
    uint16_t value = js_touint16(state, 2);

    switch (regId) {
    case 97120: //'ax'
        v86_regs_out.x.ax = value;
        break;
    case 98120: //'bx'
        v86_regs_out.x.bx = value;
        break;
    case 99120: //'cx'
        v86_regs_out.x.cx = value;
        break;
    case 100120: //'dx'
        v86_regs_out.x.dx = value;
        break;
    case 115105: //'si'
        v86_regs_out.x.si = value;
        break;
    case 100105: //'di'
        v86_regs_out.x.di = value;
        break;
    case 99102: //'cf' aka cflag
        v86_regs_out.x.cflag = value;
        break;
    case 97108: //'al'
        v86_regs_out.h.al = (uint8_t)value;
        break;
    case 97104: //'ah'
        v86_regs_out.h.ah = (uint8_t)value;
        break;
    case 98108: //'bl'
        v86_regs_out.h.bl = (uint8_t)value;
        break;
    case 98104: //'bh'
        v86_regs_out.h.bh = (uint8_t)value;
        break;
    case 99108: //'cl'
        v86_regs_out.h.cl = (uint8_t)value;
        break;
    case 99104: //'ch'
        v86_regs_out.h.ch = (uint8_t)value;
        break;
    case 100108: //'dl'
        v86_regs_out.h.dl = (uint8_t)value;
        break;
    case 100104: //'dh'
        v86_regs_out.h.dh = (uint8_t)value;
        break;
    case 101115: //'es'
        v86_sregs.es = value;
        break;
    case 99115: //'cs'
        v86_sregs.cs = value;
        break;
    case 115115: //'ss'
        v86_sregs.ss = value;
        break;
    case 100115: //'ds'
        v86_sregs.ds = value;
        break;
    default:
        message("Register: %ui does not exist!", regId);
    }

    js_pushundefined(state);
}

static void js_vm86_get_register_value(js_State *state) {
    //register id
    uint32_t regId = js_touint32(state, 1);
    //16 bit value
    uint16_t value = 0;

    switch (regId) {
    case 97120: //'ax'
        value = v86_regs_in.x.ax;
        break;
    case 98120: //'bx'
        value = v86_regs_in.x.bx;
        break;
    case 99120: //'cx'
        value = v86_regs_in.x.cx;
        break;
    case 100120: //'dx'
        value = v86_regs_in.x.dx;
        break;
    case 115105: //'si'
        value = v86_regs_in.x.si;
        break;
    case 100105: //'di'
        value = v86_regs_in.x.di;
        break;
    case 99102: //'cf' aka cflag
        value = v86_regs_in.x.cflag;
        break;
    case 97108: //'al'
        value = (uint16_t)v86_regs_in.h.al;
        break;
    case 97104: //'ah'
        value = (uint16_t)v86_regs_in.h.ah;
        break;
    case 98108: //'bl'
        value = (uint16_t)v86_regs_in.h.bl;
        break;
    case 98104: //'bh'
        value = (uint16_t)v86_regs_in.h.bh;
        break;
    case 99108: //'cl'
        value = (uint16_t)v86_regs_in.h.cl;
        break;
    case 99104: //'ch'
        value = (uint16_t)v86_regs_in.h.ch;
        break;
    case 100108: //'dl'
        value = (uint16_t)v86_regs_in.h.dl;
        break;
    case 100104: //'dh'
        value = (uint16_t)v86_regs_in.h.dh;
        break;
    case 101115: //'es'
        value = v86_sregs.es;
        break;
    case 99115: //'cs'
        value = v86_sregs.cs;
        break;
    case 115115: //'ss'
        value = v86_sregs.ss;
        break;
    case 100115: //'ds'
        value = v86_sregs.ds;
        break;
    default:
        message("Register: %ui does not exist!", regId);
    }

    js_pushnumber(state, (double)value);
}

static void js_vm86_call_bios(js_State *state) {

    int inta = js_toint32(state, 1);
    int intb = js_toint32(state, 2);

    vm86_init();
    l1_int_bind(0x6d, (void *)emulate_int);

    X_REGS16 ir, rr;
    X_SREGS16 sr;

    //Get video mode
    ir.x.ax = 0x4f00;
    ir.x.di=0;
    sr.ds=0x3a;
    sr.es=0x3a;

    vm86_callBIOS(0x10, &ir, &rr, &sr);

    message("here %i", inta);
    //vm86_callBIOS(interrupt, &v86_regs_out, &v86_regs_in, &v86_sregs);

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

    //vm86 basic utilities [from modified oslib]
    js_newcfunction(state, js_vm86_init, "vm86_init", 1);
    js_setglobal(state, "vm86_init");

    js_newcfunction(state, js_vm86_set_register_value, "vm86_set_register_value", 2);
    js_setglobal(state, "vm86_set_register_value");

    js_newcfunction(state, js_vm86_get_register_value, "vm86_get_register_value", 1);
    js_setglobal(state, "vm86_get_register_value");

    js_newcfunction(state, js_vm86_call_bios, "vm86_call_bios", 1);
    js_setglobal(state, "vm86_call_bios");

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


