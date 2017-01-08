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

//Simple linked list malloc and free support with new and delete
#include "memory.h"
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);
void operator delete(void *p, size_t sz);
void operator delete[](void *p, size_t sz);


//Shell
#include "v7/v7.h"

//Command line processing strict key=value format
#include "commandline.h"

//LLS - Low Level Structures [and tables] - but only the ones that are relevant today
#include "lls.h"

//Basic console font
#include "font.h"

//Low Level Graphics Primitives
#include "guiprimitive.h"

//Basic Graphics Driver[s] (of type GUIPrimitive)
#include "vesalfb.h"

#endif
