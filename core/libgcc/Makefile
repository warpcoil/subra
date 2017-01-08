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

#The specific warnings should be disabled, ordinarily all warnings are treated except in the case of sqlite (amalgamtion)
#SFLAGS = -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unused-but-set-variable

OBJECTS = __divdi3.o \
	__divmoddi4.o \
	__moddi3.o \
	__udivdi3.o \
	__udivmoddi4.o \
	__umoddi3.o \
	implicit.o

#C_OPT= $(EXTRAFLAGS) -Wall -O -ffreestanding -finline-functions -fno-builtin -nostdinc -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -m32
CFLAGS = $(EXTRAFLAGS) -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-stack-protector -nostdinc -ffreestanding -Wno-implicit-function-declaration -m32 -I../include -I../include/libgcc
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

compile: $(OBJECTS)
	$(MKDIR) ../obj/libgcc
	$(CP) *.o ../obj/libgcc/

.s.o: $(AS) $<
.c.o: $(CC) $(C_OPT) -c $(OBJECTS)
.cpp.o: $(CXX) $(C_OPT) -c $(OBJECTS)
.asm.o: $(AS) $(N_OPT) $(NASMOBJECTS)

#--start-group -los --end-group -o
