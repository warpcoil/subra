#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CPP=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p

.SUFFIXES: .o .c .cpp .S .asm

NASMOBJECTS=gdtflush.o
N_OPT= -felf

MYOBJECTS = my_string.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -fno-exceptions -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -m32 -I../include -I../include/oslib -I../include/my
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

compile: $(MYOBJECTS)
	$(MKDIR) ../obj/my
	$(CP) *.o ../obj/my/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(MYOBJECTS)
.cpp.o: $(CPP) $(CXXFLAGS) -c $(MYOBJECTS)
.asm.o: $(AS) $(N_OPT) $(NASMOBJECTS)

#--start-group -los --end-group -o
