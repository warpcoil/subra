#include "kernel.h"

#include "oslib/oslibstdio.h"

extern "C" void kernel_main(int argc, char * argv[]);

FILE * stdin;
FILE * stdout;
FILE * stderr;

static GUIPrimitive * gfxDriver;

//message function is called from javascript

static v7_err js_msg(struct v7 *v7, v7_val_t *res) {

    size_t l;

    v7_val_t m = v7_arg(v7, 0);

    const char * s = v7_get_string(v7, &m, &l);

    message("%s", s);

    * res = v7_mk_number(v7, (double)l);

    return V7_OK;
}


void kernel_main(int argc, char * argv[]) {

    struct multiboot_info * MultiBootInfo = (multiboot_info *)l1_init();

    if (MultiBootInfo == NULL)
    {
            message("Error in Initialisation Code");
            //l1_exit (-1);
    }

    //WORD c = ll_context_save();
    ll_context_save();

    message("Attempting Set up of SubraX\n\n");

    FILE cin;
    cin.filename = (char *)"::in";
    FILE cout;
    cout.filename = (char *)"::out";
    FILE cerr;
    cerr.filename = (char *)"::err";

    stdin = &cin;
    stdout = &cout;
    stderr = &cerr;

/*** Region -- Memory Manager ***/
//    Memory::init();
/*** End Region -- Memory Manager ***/

    //Register the command line [if any]
    CommandLine commandLine;

    commandLine.RegisterCommandLine(argc, argv);

    enum v7_err rcode = V7_OK;
    struct v7 *v7 = v7_create();
    v7_val_t result;

    v7_set_method(v7, v7_get_global(v7), "message", &js_msg);

    //Send (in javascript) to console "Hello World!" [test]
    rcode = v7_exec(v7, "message(\"Hello World!\");", &result);

    if (rcode != V7_OK) {
        message("exec error: %i\n", (int)rcode);
    }

    v7_destroy(v7);



/*** Region -- Data Management initialisation ***/
//    DataManager x;
//    x.Test();

/*** End Region -- Data Management initialisation ***/
/*** Region -- RSDP/ACPI for system information ***/

//    LLS * lls = new LLS();

//    lls->ReadStructures();


/*** End Region -- Basic Level GUI ***/
/*** Region -- Basic Level GUI for information presentation ***/

    //Check for gfx command line [forced], if not use autodetection [recommended]
    VesaLFB * vesaLfb = new VesaLFB();

    int arg = commandLine.HasArgument((char *)"gfx");
    bool useVesaLfb = false;
    if (arg > 0) {
        char * displayDriver = commandLine.GetValue(arg);
        message("Using forced %s driver\n\n", displayDriver);

        if (strcmp(displayDriver, "vesalfb") == 0) {
            useVesaLfb = true;
        } else { //possible future vbe or other, for now only vesalfb
            message("Unknown forced %s driver, falling back to other options", displayDriver);
        }
    } else {
        //Check vesalfb is supported
        useVesaLfb = vesaLfb->IsSupported();
    }

    //Prioritise driver based on performance
    if (useVesaLfb == true) {

        gfxDriver = vesaLfb; //gfxDriver is generic, vesalfb is linear frame buffer, polymorph

        //Check for mode override (basically don't use maximums
        arg = commandLine.HasArgument((char*)"vesalfb");

        uint16_t mode;

        if (arg > 0) { //The user specified the mode they wish to use during the boot process

            char * modechr = commandLine.GetValue(arg);
            mode = strtoul(modechr, 0, 10);

            message("mode: %x", mode);

        } else { //Use autodetection

            mode = vesaLfb->GetMaximum(true); //Favour 32 bit depth over resolution

        }

        mode = vesaLfb->GetMaximum(true); //Favour 32 bit depth over resolution
        message("mode: %x", mode);

        //Set mode
        //vesaLfb->SetResolution(mode);

    } else {

        message("A graphics driver has either not been specified or is unavailable, halting");
        asm volatile ("cli\nhlt");

    }

/*** End Region -- Basic Level GUI ***/

    //If we have got this far, we now have capabilities to feed information to the user
    //Continue boot process

    //Font::Character * chr = new Font::Character();

    //message("chr @ %x", chr);

    GUIPrimitive::RGB colour;

    colour.Red = 255;
    colour.Blue = 255;
    colour.Green = 255;

    for (int i=0; i<20; i++) {
        //gfxDriver->DrawPixel(10 + i, 20, &colour);
        //vesaLfb->DrawPixel(10 + i, 20, &colour);
    }
}

/* C++ support functions for new and delete */

void *operator new(size_t size)
{
    return dlmalloc((uint16_t)size);
}

void *operator new[](size_t size)
{
    return dlmalloc((uint16_t)size);
}

inline void *operator new(size_t, void* __p)
{
    return __p;
}

inline void *operator new[](size_t, void* __p)
{
    return __p;
}

inline void operator delete(void *, void *) { }

inline void operator delete[](void *, void *) { }

void operator delete(void *p)
{
    dlfree(p);
}

void operator delete[](void *p)
{
    dlfree(p);
}

void operator delete(void *p, size_t sz)
{
    sz = sz;
    dlfree(p);
}

void operator delete[](void *p, size_t sz)
{
    sz = sz;
    dlfree(p);
}
