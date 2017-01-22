#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p

.SUFFIXES: .o .c .cpp

N_OPT= -felf

MINIZOBJECTS = miniz.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -Wno-varargs -fno-exceptions -fno-stack-protector -Wno-strict-aliasing -Wno-misleading-indentation -D__LINUX__ -D__VIRCSW__ -m32 -I../include -I../include/oslib
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

compile: $(MINIZOBJECTS)
	$(MKDIR) ../obj/miniz
	$(CP) *.o ../obj/miniz/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(MINIZOBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(MINIZOBJECTS)

#--start-group -los --end-group -o
