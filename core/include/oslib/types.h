/* Project:     OSLib
 * Description: The OS Construction Kit
 * Date:                1.6.2000
 * Idea by:             Luca Abeni & Gerardo Lamastra
 *
 * OSLib is an SO project aimed at developing a common, easy-to-use
 * low-level infrastructure for developing OS kernels and Embedded
 * Applications; it partially derives from the HARTIK project but it
 * currently is independently developed.
 *
 * OSLib is distributed under GPL License, and some of its code has
 * been derived from the Linux kernel source; also some important
 * ideas come from studying the DJGPP go32 extender.
 *
 * We acknowledge the Linux Community, Free Software Foundation,
 * D.J. Delorie and all the other developers who believe in the
 * freedom of software and ideas.
 *
 * For legalese, check out the included GPL license.
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include "hw-data.h"

//#define size_t uint32_t
//#define ssize_t int32_t
typedef long unsigned int size_t;
typedef long signed int ssize_t;
#define va_list void*

#define u_int uint32_t
#define u_char BYTE
#define u_short WORD
#define u_long DWORD

/* unsigned integers */
typedef BYTE         u_int8_t;
typedef WORD         u_int16_t;
typedef DWORD        u_int32_t;

/* signed integers */
typedef signed char  int8_t;
typedef short int    int16_t;
typedef int          int32_t;

#ifndef __cplusplus
typedef int8_t bool;
#endif

#define true 1
#define false 0

#define offsetof(st, m) ((size_t)&(((st *)0)->m))

#endif
