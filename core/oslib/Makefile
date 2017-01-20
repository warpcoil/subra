# Standard library for ELF applications
# Makefile for GNU MAKE & GCC 6 or later
# Untested with earlier versions.  Must use xxx-elf-gcc/ld NEVER xxx-linux-elf-gcc/ld

CXX=$(PREFIX32)g++
CC=$(PREFIX32)gcc
AR=$(PREFIX32)ar
MKDIR=mkdir -p
RM=rm
CP=cp

echo -e "Compiling low level support using Compiler Prefix: $(PREFIX32)"

COMMON_OBJS = xinfo.o \
	x1.o \
	xsystab.o \
	xconv.o \
	xdosf.o \
	xdosm.o \
	ccpu.o \
	fpu.o \
	irq.o \
	ctxsw.o \
	xinit.o \
	idtinit.o \
	vm86.o \
	losrt.o \
	xbios.o \
	time.o \
	setjmp.o \
	assert.o

GNU_S_OBJS = x0.o xsys0.o cpu2.o exc.o ctx.o mem.o

OBJS = $(GNU_S_OBJS) $(GNU_C_OBJS) $(COMMON_OBJS)

C_OPT= $(EXTRAFLAGS) -Wall -O -finline-functions -fno-builtin -fno-stack-protector -nostdinc -ffreestanding -D__LINUX__ -D__VIRCSW__ -m32 -std=c99 -I../include -I../include/oslib

ASM_OPT = -x assembler-with-cpp -D__LINUX__ -D__VIRCSW__ -ffreestanding -m32

.s.o:
	$(CXX) $(ASM_OPT) -c $<

.c.o:
	$(CC) $(C_OPT) -c $<

.c.s:
	$(CC) $(C_OPT) -S $<

all: oslib

oslib: $(OBJS)
	$(MKDIR) ../obj/oslib
	$(CP) *.o ../obj/oslib/

