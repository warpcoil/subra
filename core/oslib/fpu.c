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

/*
   FPU Context switch & management functions!
   Generic 32 bit module
*/

#include "../include/oslib/hw-data.h"
#include "../include/oslib/hw-instr.h"
#include "../include/oslib/hw-func.h"
#include "../include/oslib/mem.h"
#include "../include/oslib/tss-ctx.h"

FILE(FPU);

extern TSS main_tss;

BYTE LL_FPU_savearea[FPU_CONTEXT_SIZE]; /* Global FPU scratch SaveArea */
#ifdef __FPU_DEBUG__
    long int ndp_called = 0,ndp_switched = 0;
#endif

/* FPU context management */
static TSS *LL_has_FPU = &main_tss;

/* As the 8086 does not have an hardware mechanism to support task      */
/* switch, also the FPU context switch is implemented via software.     */
/* When a preemption occurs, if the task is marked as a MATH task, the  */
/* preemption routine will save/restore the FPU context.		*/
/* The hook is called whenever a FPU context switch is necessarty	*/

void ll_FPU_hook(void)
{
    CONTEXT current;
    TSS *base;

    current = get_TR();
    base = (TSS *)GDT_read(current, NULL, NULL, NULL);

    clts();
    #ifdef __FPU_DEBUG__
	ndp_called++;
    #endif
    if (LL_has_FPU == base) return;
    #ifdef __FPU_DEBUG__
	ndp_switched++;
    #endif

#if 0
    LL_FPU_save();
    memcpy(TSS_table[LL_has_FPU].ctx_FPU,LL_FPU_savearea,FPU_CONTEXT_SIZE);
#else 
    save_fpu(LL_has_FPU);
#endif

    LL_has_FPU = base;

#if 1
    memcpy(LL_FPU_savearea, base->ctx_FPU, FPU_CONTEXT_SIZE);
    LL_FPU_restore();
#else 
    restore_fpu(&(TSS_table[LL_has_FPU]));
#endif
    return;
}

TSS *LL_FPU_get_task(void)
{
    return(LL_has_FPU);
}
