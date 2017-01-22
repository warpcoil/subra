## musl core

This directory only contains a limited set of header files.

As we do not need or want the entire musl libc within the kernel, we have constructed a limited set of header files that use small portions of libm to support the JavaScript Runtime as necessary.

Required math support preprocessor definitions


Required math support functions
* acos
* asin
* atan
* atan2
* ceil
* cos
* exp
* fabs
* floor
* fmod
* frexp
* ldexp
* log
* pow
* round
* sin
* sqrt
* tan

The existing musl/*.h directory is not currently needed but maybe needed in future when we consider using a libc/libm user space.
