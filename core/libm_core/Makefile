#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p
RM=rm

.SUFFIXES: .o .c .cpp

MOBJECTS = math.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-sign-compare -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -D__STDC_LIMIT_MACROS -DSUBRACORE -DARCH=$(ARCH) -m32 -I../include -I../include/oslib -I../include/libm_core
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

compile: $(MOBJECTS)
	$(MKDIR) ../obj/libm
	$(CP) *.o ../obj/libm/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(MOBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(MOBJECTS)

#--start-group -los --end-group -o
