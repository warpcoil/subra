#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
X16DIR=../obj/x16/
GCCDIR=../obj/libgcc/
OSLDIR=../obj/oslib/
MINIZDIR=../obj/miniz/
MDIR=../obj/libm/
MYDIR=../obj/my/
MUJSDIR=../obj/mujs/

echo -e "Compiling 32 bit main kernel support using Compiler Prefix: $(PREFIX32)"
.SUFFIXES: .o .c .cpp .S .asm .s

#NASMOBJECTS=gdtflush.o
N_OPT= -felf

X16OBJECTS = $(X16DIR)int16.o \
	$(X16DIR)int16elf.o

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
	$(OSLDIR)setjmp.o \
	$(OSLDIR)longjmp.o \
	$(OSLDIR)assert.o

MINIZOBJECTS = $(MINIZDIR)miniz.o 

MOBJECTS = $(MDIR)math.o

MUJSOBJECTS = $(MUJSDIR)jsarray.o \
	$(MUJSDIR)jsboolean.o \
	$(MUJSDIR)jsbuiltin.o \
	$(MUJSDIR)jscompile.o \
	$(MUJSDIR)jsdate.o \
	$(MUJSDIR)jsdtoa.o \
	$(MUJSDIR)jsdump.o \
	$(MUJSDIR)jserror.o \
	$(MUJSDIR)jsfunction.o \
	$(MUJSDIR)jsgc.o \
	$(MUJSDIR)jsintern.o \
	$(MUJSDIR)jslex.o \
	$(MUJSDIR)jsmath.o \
	$(MUJSDIR)jsnumber.o \
	$(MUJSDIR)jsobject.o \
	$(MUJSDIR)json.o \
	$(MUJSDIR)jsparse.o \
	$(MUJSDIR)jsproperty.o \
	$(MUJSDIR)jsregexp.o \
	$(MUJSDIR)jsrun.o \
	$(MUJSDIR)jsstate.o \
	$(MUJSDIR)jsstring.o \
	$(MUJSDIR)jsvalue.o \
	$(MUJSDIR)regexp.o \
	$(MUJSDIR)utf.o \
	$(MUJSDIR)utftype.o

MYOBJECTS = $(MYDIR)my_string.o

OBJECTS = main.o \
	io.o \
	gdt.o \
	gdtflush.o \
	idt.o \
	idtflush.o \
	realmode.o \
	isr.o \
	handler.o \
	timer.o \
	jssupport.o \
	zip.o \
	module.o \
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
	$(LD) $(LD_OPT) -o kernel32.sys $(GCCOBJECTS) $(OSLOBJECTS) $(MINIZOBJECTS) $(MOBJECTS) $(MUJSOBJECTS) $(MYOBJECTS) $(X16OBJECTS) $(OBJECTS)
	$(CP) kernel32.sys ../../iso/boot

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(OBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(OBJECTS)
.asm.o: $(AS) $(N_OPT) $(NASMOBJECTS)

#--start-group -los --end-group -o
