#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
BIN2C=$(PREFIX32)bin2c
ASFLAT=nasm -fbin
ASELF=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p

.SUFFIXES: .o .c .cpp .s

RMOBJECTS = int16.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -Wno-varargs -fno-exceptions -fno-stack-protector -Wno-strict-aliasing -Wno-misleading-indentation -D__LINUX__ -D__VIRCSW__ -m32 -I../include -I../include/oslib
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: assemble transform compile output

assemble:
	$(foreach var,$(RMOBJECTS), $(ASFLAT) $(basename $(var)).s -o $(basename $(var)).o ;)
	$(foreach var,$(RMOBJECTS), $(ASELF) $(basename $(var))elf.s -o $(basename $(var))elf.o ;)

transform:
	$(foreach var,$(RMOBJECTS), $(BIN2C) -d $(basename $(var)).h -o $(basename $(var)).c $(var) ;)
	$(foreach var,$(RMOBJECTS), sed -ie 's/extern\ const/extern\ \"C\"\ const/g' $(basename $(var)).h ;)

compile:
	$(foreach var,$(RMOBJECTS), $(CC) -c $(basename $(var)).c -o $(basename $(var)).o ;)

output:
	$(MKDIR) ../include/x16
	$(CP) *.h ../include/x16
	$(MKDIR) ../obj/x16
	$(CP) *.o ../obj/x16/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(RMOBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(RMOBJECTS)

#--start-group -los --end-group -o
