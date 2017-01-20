#Porting to C++ (cleaner than legacy C)
CC=$(PREFIX32)gcc
CXX=$(PREFIX32)g++
AS=nasm -felf
LD=$(PREFIX32)g++
CP=cp
MKDIR=mkdir -p

.SUFFIXES: .o .c .cpp

N_OPT= -felf

MUJSOBJECTS = jsarray.o \
	jsboolean.o \
	jsbuiltin.o \
	jscompile.o \
	jsdate.o \
	jsdtoa.o \
	jsdump.o \
	jserror.o \
	jsfunction.o \
	jsgc.o \
	jsintern.o \
	jslex.o \
	jsmath.o \
	jsnumber.o \
	jsobject.o \
	json.o \
	jsparse.o \
	jsproperty.o \
	jsregexp.o \
	jsrun.o \
	jsstate.o \
	jsstring.o \
	jsvalue.o \
	regexp.o \
	utf.o \
	utftype.o

CFLAGS = $(EXTRAFLAGS) -ffreestanding -nostdinc -O2 -Wall -Wextra -Wno-unused-parameter -Wno-varargs -Wno-strict-overflow -fno-stack-protector -D__LINUX__ -D__VIRCSW__ -D__STDC_LIMIT_MACROS -DSUBRACORE -DARCH=$(ARCH) -DNO_FILE_API -m32 -I../include -I../include/oslib -I../include/my -I../include/libm_core -I../include/mujs
CXXFLAGS=$(CFLAGS) -fno-rtti
LD_OPT= $(EXTRAFLAGS) -Bstatic -Tlink.ld -s -nostartfiles -nostdlib

all: compile

astnames.h: ../include/mujs/jsparse.h
	grep -E '(AST|EXP|STM)_' ../include/mujs/jsparse.h | sed 's/^[^A-Z]*\(AST_\)*/"/;s/,.*/",/' | tr A-Z a-z | sed '$$ s/.$$//' > ../include/mujs/$@

opnames.h: ../include/mujs/jscompile.h
	grep -E 'OP_' ../include/mujs/jscompile.h | sed 's/^[^A-Z]*OP_/"/;s/,.*/",/' | tr A-Z a-z | sed '$$ s/.$$//' > ../include/mujs/$@

compile: astnames.h opnames.h $(MUJSOBJECTS)
	$(MKDIR) ../obj/mujs
	$(CP) *.o ../obj/mujs/

.s.o: $(AS) $<
.c.o: $(CC) $(CFLAGS) -c $(MUJSOBJECTS)
.cpp.o: $(CXX) $(CXXFLAGS) -c $(MUJSOBJECTS)

#--start-group -los --end-group -o
