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

/*	For accessing BIOS calls returning in Real Mode, or using VM86	*/

#ifndef __X_BIOS_H__
#define __X_BIOS_H__

#include "defs.h"
BEGIN_DEF

#include "hw-data.h"
#include "hw-instr.h"

#define DOS_OFF(x)      ((DWORD)(x) & 0x000F)
#define DOS_SEG(x)      (((DWORD)(x) & 0xFFFF0) >> 4)

typedef union x_regs16 {
    struct {
	WORD ax __attribute__ ((packed));
	WORD bx __attribute__ ((packed));
	WORD cx __attribute__ ((packed));
	WORD dx __attribute__ ((packed));
	WORD si __attribute__ ((packed));
	WORD di __attribute__ ((packed));
	WORD cflag __attribute__ ((packed));
	WORD _pad __attribute__ ((packed));
    } x;
    struct {
	BYTE al,ah;
	BYTE bl,bh;
	BYTE cl,ch;
	BYTE dl,dh;
    } h;
} X_REGS16;

typedef struct x_sregs16 {
	WORD es __attribute__ ((packed));
	WORD cs __attribute__ ((packed));
	WORD ss __attribute__ ((packed));
	WORD ds __attribute__ ((packed));
} X_SREGS16;

typedef struct {
    /* The GDT linear address inheritable from X */ 
    DWORD GDT_base;
    /* These are used for BIOS calling 	*/
    X_REGS16 ir;
    X_REGS16 rr;
    X_SREGS16 sr;
    DWORD irqno;
    /* This is the X version 		*/
    DWORD ver;
} X_CALLBIOS;

X_CALLBIOS * x_bios_address(void);
void X_meminfo(LIN_ADDR *b1,DWORD *s1,LIN_ADDR *b2,DWORD *s2);
void X_callBIOS(int service,X_REGS16 *in,X_REGS16 *out,X_SREGS16 *s);
void vm86_init();
TSS *vm86_get_tss(void);
int vm86_callBIOS(int service,X_REGS16 *in,X_REGS16 *out,X_SREGS16 *s);

END_DEF

#endif
