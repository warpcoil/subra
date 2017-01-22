#include "oslib/assert.h"

void assert(int32_t assertion) {
	if (assertion == 0) {
		message("Assertion Failed, Application Terminated");
		__asm__ __volatile__("hlt");
	}
}
