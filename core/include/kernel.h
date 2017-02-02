#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>

#include "oslib/x-bios.h"
#include "oslib/types.h"
#include "oslib/cons.h"
#include "oslib/oslibstdlib.h"
#include "oslib/oslibstdio.h"
#include "oslib/losrt.h"
#include "oslib/hw-func.h"
#include "oslib/error.h"
#include "oslib/tss-ctx.h"

//Real Mode
#include "realmode.h"

//Ports (io)
#include "io.h"

//Basic Descriptor Tables
#include "gdt.h"
#include "idt.h"

//Simple linked list malloc and free support with new and delete
#include "memory.h"
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);
void operator delete(void *p, size_t sz);
void operator delete[](void *p, size_t sz);

//Global access
//extern static GDT * gdt;
//extern static IDT * idt;
//extern static Support * support;
//extern static Module * module;
//extern static RealMode * realMode;

extern "C" void set_vm86_register_value(uint32_t reg, uint16_t value);
extern "C" uint16_t get_vm86_register_value(uint32_t reg);
extern "C" void vm86_call(int bind, int service);

//Zip File Processing [i.e. modules]
//#include "zip.h"

//Module Loading [multiboot spec]
#include "module.h"

//Shell / Execution Environment

//JavaScript Runtime Support Utilities
#include "jssupport.h"

//Command line processing strict key=value format
//#include "commandline.h"

//LLS - Low Level Structures [and tables] - but only the ones that are relevant today
//#include "lls.h"

//Basic console font
//#include "font.h"

//Low Level Graphics Primitives
//#include "guiprimitive.h"

//Basic Graphics Driver[s] (of type GUIPrimitive)
//#include "vesalfb.h"

#endif
