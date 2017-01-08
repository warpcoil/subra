#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CPP=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p

.SUFFIXES: .o .c .cpp

N_OPT= -felf

V7OBJECTS = v7.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -Wno-varargs -fno-exceptions -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -D__STDC_LIMIT_MACROS -DSUBRACORE -DCS_PLATFORM=CS_P_CUSTOM -DV7_BUILD_PROFILE=1 -DV7_NO_FS -DV7_ENABLE__Math=1 -DV7_ENABLE__RegExp=1 -m32 -I../include -I../include/oslib -I../include/my -I../include/v7
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

compile: $(V7OBJECTS)
	$(MKDIR) ../obj/v7
	$(CP) *.o ../obj/v7/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(V7OBJECTS)
.cpp.o: $(CPP) $(CXXFLAGS) -c $(V7OBJECTS)

#--start-group -los --end-group -o
