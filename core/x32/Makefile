#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
GCCDIR=../obj/libgcc/
OSLDIR=../obj/oslib/
V7DIR=../obj/v7/
MYDIR=../obj/my/

echo -e "Compiling 32 bit main kernel support using Compiler Prefix: $(PREFIX32)"
.SUFFIXES: .o .c .cpp .S .asm

NASMOBJECTS=gdtflush.o
N_OPT= -felf

GCCOBJECTS = $(GCCDIR)__divdi3.o \
	$(GCCDIR)__divmoddi4.o \
	$(GCCDIR)__moddi3.o \
	$(GCCDIR)__udivdi3.o \
	$(GCCDIR)__udivmoddi4.o \
	$(GCCDIR)__umoddi3.o \
	$(GCCDIR)implicit.o

OSLOBJECTS = $(OSLDIR)xinfo.o \
	$(OSLDIR)x0.o \
	$(OSLDIR)x1.o \
	$(OSLDIR)xsystab.o \
	$(OSLDIR)xconv.o \
	$(OSLDIR)xdosf.o \
	$(OSLDIR)xdosm.o \
	$(OSLDIR)ccpu.o \
	$(OSLDIR)fpu.o \
	$(OSLDIR)irq.o \
	$(OSLDIR)ctxsw.o \
	$(OSLDIR)xinit.o \
	$(OSLDIR)idtinit.o \
	$(OSLDIR)vm86.o \
	$(OSLDIR)xbios.o \
	$(OSLDIR)xsys0.o \
	$(OSLDIR)cpu2.o \
	$(OSLDIR)exc.o \
	$(OSLDIR)ctx.o \
	$(OSLDIR)mem.o \
	$(OSLDIR)losrt.o \
	$(OSLDIR)time.o \
	$(OSLDIR)math.o \
	$(OSLDIR)setjmp.o

V7OBJECTS = $(V7DIR)v7.o

MYOBJECTS = $(MYDIR)my_string.o

OBJECTS = main.o \
	support.o \
	stacksmash.o \
	commandline.o \
	lls.o \
	kernel8font.o \
	kernel12font.o \
	kernel16font.o \
	memory.o \
	font.o \
	guiprimitive.o \
	vesalfb.o

#datamanager.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -fno-exceptions -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -DLACKS_STDLIB_H -DDEBUG -m32 -I../include -I../include/oslib -I../include/my -I../include/warp
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: link

link: $(OBJECTS) link.ld
	$(LD) $(LD_OPT) -o kernel32.sys $(GCCOBJECTS) $(OSLOBJECTS) $(V7OBJECTS) $(MYOBJECTS) $(OBJECTS)
	$(CP) kernel32.sys ../../iso/boot

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(OBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(OBJECTS)
.asm.o: $(AS) $(N_OPT) $(NASMOBJECTS)

#--start-group -los --end-group -o
