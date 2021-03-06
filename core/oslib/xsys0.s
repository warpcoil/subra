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

/*	Provides the _exit function for escaping from PM!!!	*/

#include "../include/oslib/linkage.h"
#include "../include/oslib/defs.h"
#include "../include/oslib/sel.h"
#include "../include/oslib/defs.h"

.data

ASMFILE(X0-Sys)

.text

.globl SYMBOL_NAME(_x_callBIOS)

#ifdef __NO_INLINE_PORT__
.globl SYMBOL_NAME(outp)
.globl SYMBOL_NAME(inp)
.globl SYMBOL_NAME(outpw)
.globl SYMBOL_NAME(inpw)
.globl SYMBOL_NAME(outpd)
.globl SYMBOL_NAME(inpd)
#endif

/* Invoke 16 bit BIOS function from PM application */

/* void _x_callBIOS(void) */

SYMBOL_NAME_LABEL(_x_callBIOS)
		.byte	0x09a		   /* Direct gate call */
		.long	0		  
		.word	X_CALLBIOS_GATE
		ret

#ifdef __NO_INLINE_PORT__
/* void outp(int port,char value) */

SYMBOL_NAME_LABEL(outp)
		movl	4(%esp),%edx
		movl	8(%esp),%eax
		outb	%al,%dx
		ret

/* char inp(int port) */

SYMBOL_NAME_LABEL(inp)
		movl	4(%esp),%edx
		inb	%dx,%al
		movzb	%al,%eax
		ret

/* void outpw(int port,unsigned short value) */

SYMBOL_NAME_LABEL(outpw)
		movl	4(%esp),%edx
		movl	8(%esp),%eax
		outw	%ax,%dx
		ret

/* unsigned short inpw(int port) */

SYMBOL_NAME_LABEL(inpw)
		movl	4(%esp),%edx
		inw	%dx,%ax
		movzwl	%ax,%eax
		ret
/* void outpd(int port,unsigned long value) */

SYMBOL_NAME_LABEL(outpd)
		movl	4(%esp),%edx
		movl	8(%esp),%eax
		outl	%eax,%dx
		ret

/* unsigned long inpd(int port) */

SYMBOL_NAME_LABEL(inpd)
		movl	4(%esp),%edx
		inl	%dx,%eax
		ret
#endif	/* __NO_INLINE_PORTS__ */	
