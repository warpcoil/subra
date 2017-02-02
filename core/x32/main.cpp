#include "kernel.h"
#include "oslib/oslibstdio.h"

//16 bit objects
#include "x16/int16.h"

extern "C" void kernel_main(int argc, char * argv[]);
extern "C" void emulate_int(uint32_t intnum, struct registers r);

FILE * stdin;
FILE * stdout;
FILE * stderr;

//static GUIPrimitive * gfxDriver;
static GDT * gdt;
static IDT * idt;
static Support * support; //-- Generic Interface, currently only JS
static Module * module;
static RealMode * realMode;

void kernel_main(int argc, char * argv[]) {
    argc=argc;
    argv=argv;

    //DWORD stackPtr = get_SP();
    struct multiboot_info * MultiBootInfo = (multiboot_info *)l1_init();

    if (MultiBootInfo == NULL)
    {
            message("Error in Initialisation Code");
            __asm__ __volatile__("hlt");
    }

    ll_context_save();

    //Copy embedded 16 bit code to org 0x600
    uint32_t begin = (uint32_t)&int16_o[0];
    uint32_t size = (uint32_t)int16_o_size;

    message("beg %u sz %u\n", begin, size);

    //Enable 16 bit mode
    //RealMode real;
    //realMode = &real;

    //Set our own GDT, don't use oslib, we need to extend later
    GDT globalDescriptor;
    gdt = &globalDescriptor;
    gdt->Initialise();

    //Set our own IDT, we can create services and handle them as necessary
    IDT interruptDescriptor;
    idt = &interruptDescriptor;
    idt->StopInterrupts();
    idt->Initialise();
    idt->StartInterrupts();

    //X_REGS16 ri;
    //X_REGS16 ro;
    //X_SREGS16 rs;

    //regs_in = &ri;
    //regs_out = &ro;
    //regs_segments = &rs;

    //vm86_init();
    //l1_int_bind(0x6d, (void *)emulate_int);

    //Get video mode
    //regs_in->x.ax = 0x4f00;
    //regs_in->x.di=0;
    //regs_segments->ds=0x3a;
    //regs_segments->es=0x3a;

    //vm86_call(0x6d, 0x10);

    //vm86_callBIOS(0x10, regs_in, regs_out, regs_segments);

/*** Region -- Modules Initialisation (zip files) ***/
    //Load modules -- contains kernel runtime encoded as JavaScript
    //Everything is written in JS
    Module mod;
    if (!mod.Initialise(MultiBootInfo)) {
        message("Unable to load modules, system halted\n");
        __asm__ __volatile__("hlt");
    }
    module = &mod;

/*** End Region -- Modules Initialisation (zip files) ***/
/*** Region -- Virtual stdin, stdout and stderr for JS Runtime ***/

    FILE cin;
    cin.filename = (char *)"::in";
    FILE cout;
    cout.filename = (char *)"::out";
    FILE cerr;
    cerr.filename = (char *)"::err";

    stdin = &cin;
    stdout = &cout;
    stderr = &cerr;

/*** End Region -- Virtual stdin, stdout and stderr for JS Runtime ***/
/*** Region -- Early JS Runtime ***/

    JSSupport supp;
    supp.Init();
    support = &supp;

/*** End Region -- Early JS Runtime ***/
/*** Region -- Physical Module Loading ***/
    //Firstly get all loadable (JavaScript) files
    //There can be n zips containing amongst other files, .js files
    //We're interested in *.js at the minute
    warp::Vector<Module::ModuleFile> * jsFiles = new warp::Vector<Module::ModuleFile>();

    if (!module->ListFilesOfType(".JS", jsFiles)) {
        message("Platform Initialisation Error, No Modules To Load!\n\nSystem Halted\n");
        __asm__ __volatile__ ("hlt");
    }

    for (size_t i=0; i<jsFiles->size(); i++) {
        Module::ModuleFile * modFile = jsFiles->at(i);
        message("Executing: %s\n", modFile->Filename.c_str());
        if (!module->Exec(modFile->ModuleId, modFile->Filename, support)) {
            message("Error Loading %s, continuing as normal\n", modFile->Filename.c_str());
        }
    }

    for(;;);
/*** End Region -- Physical Module Loading ***/

    //Register the command line [if any]
//    CommandLine commandLine;

//    commandLine.RegisterCommandLine(argc, argv);


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
//    VesaLFB * vesaLfb = new VesaLFB();

//    int arg = commandLine.HasArgument((char *)"gfx");
//    bool useVesaLfb = false;
//    if (arg > 0) {
//        char * displayDriver = commandLine.GetValue(arg);
//        message("Using forced %s driver\n\n", displayDriver);

//        if (strcmp(displayDriver, "vesalfb") == 0) {
//            useVesaLfb = true;
//        } else { //possible future vbe or other, for now only vesalfb
//            message("Unknown forced %s driver, falling back to other options", displayDriver);
//        }
//    } else {
//        //Check vesalfb is supported
//        useVesaLfb = vesaLfb->IsSupported();
//    }

//    //Prioritise driver based on performance
//    if (useVesaLfb == true) {

//        gfxDriver = vesaLfb; //gfxDriver is generic, vesalfb is linear frame buffer, polymorph

//        //Check for mode override (basically don't use maximums
//        arg = commandLine.HasArgument((char*)"vesalfb");

//        uint16_t mode;

//        if (arg > 0) { //The user specified the mode they wish to use during the boot process

//            char * modechr = commandLine.GetValue(arg);
//            mode = strtoul(modechr, 0, 10);

//            message("mode: %x", mode);

//        } else { //Use autodetection

//            mode = vesaLfb->GetMaximum(true); //Favour 32 bit depth over resolution

//        }

//        mode = vesaLfb->GetMaximum(true); //Favour 32 bit depth over resolution
//        message("mode: %x", mode);

        //Set mode
        //vesaLfb->SetResolution(mode);

//    } else {

//        message("A graphics driver has either not been specified or is unavailable, halting");
//        asm volatile ("cli\nhlt");

//    }

/*** End Region -- Basic Level GUI ***/

    //If we have got this far, we now have capabilities to feed information to the user
    //Continue boot process

    //Font::Character * chr = new Font::Character();

    //message("chr @ %x", chr);

//    GUIPrimitive::RGB colour;

//    colour.Red = 255;
//    colour.Blue = 255;
//    colour.Green = 255;

//    for (int i=0; i<20; i++) {
//        //gfxDriver->DrawPixel(10 + i, 20, &colour);
//        //vesaLfb->DrawPixel(10 + i, 20, &colour);
//    }
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
