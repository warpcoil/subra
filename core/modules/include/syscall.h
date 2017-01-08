#ifndef __syscall_h__
#define __syscall_h__

void AddParameter(unsigned long * ParameterList, unsigned long * Parameter)
{

}

void Call(unsigned char * Function, unsigned long * ParameterList)
{
	asm volatile("movl %0, %%edx;"
					"movl %1, %%ecx;"
					"int $ff;"
					:: "d"(Function), "c" (ParameterList));
}

#endif