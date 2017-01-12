MKFILEPATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENTDIR := $(notdir $(patsubst %/,%,$(dir $(MKFILEPATH))))

PREFIX32=$(abspath tools/compiler/i386/bin)/i386-elf-
PREFIX64=$(abspath tools/compiler/x86_64/bin)/x86_64-elf-
GENERIC=$(abspath tools/compiler/generic/bin)
GCC32=$(PREFIX32)gcc
GCC64=$(PREFIX64)gcc
LD32=$(PREFIX32)ld
LD64=(PREFIX64)ld
NASM=nasm

define check_and_warn
	if [ -d $(abspath tools/compiler) ]; then \
		export PATH=$(abspath tools/compiler/i386/bin):$(abspath tools/compiler/x86_64/bin):$(abspath tools/compiler/generic/bin):$(PATH) ; \
	else \
		echo -e $(1) ; \
		echo -e "\033[0;32mContinuing in 10 seconds . . ." ; \
		sleep 10 ; \
	fi ; 
endef

all: release

run: releaserun

release:
	$(call check_extract,"\033[0;31mWARNING: Using the builtin compiler is highly recommended, use "make toolchain" first")
	PREFIX32=$(PREFIX32) PREFIX64=$(PREFIX64) make -C core
	cd rt && zip -r -D ../iso/boot/kmods.zip *
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -A SubraX -o SubraX.iso iso

#debug:
#	$(call check_extract,"\033[0;31mWARNING: Using the builtin compiler is highly recommended, use "make toolchain" first")
#	PREFIX32=$(PREFIX32) PREFIX64=$(PREFIX64) EXTRAFLAGS="-g" make -C core
#	$(PREFIX32)objcopy --only-keep-debug $(abspath iso/boot/kernel32.sys) $(abspath kernel32.sym)
#	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -A SubraX -o SubraX.iso iso

clean:
	find core/ -name "*.o" -delete
	find core/ -name "*.sys" -delete

releaserun:
	$(call check_extract,"\033[0;31mWARNING: Using the builtin emulator is highly recommended, use "make toolchain" first")
	qemu-system-i386 -m size=128 -cdrom SubraX.iso -boot d -sdl

# debugrun:
#	$(call check_extract,"\033[0;31mWARNING: Using the builtin debugger is highly recommended, use "make toolchain" first")
#	$(GENERIC)/qemu-system-i386 -m size=128 -cdrom SubraX.iso -boot d -sdl -s -S & i386-elf-gdb --symbols=$(abspath kernel.sym) --eval-command="target remote localhost:1234"

getarchive:
	make -C tools/toolbuilder getarchive

toolchain:
	mkdir -p $(abspath tools/compiler)
	mkdir -p $(abspath tools/build)
	cd tools/toolbuilder && tar -xf archive.tar.gz
	ARCHES="i386 x86_64" EMULATION="elf" OUTBUILDDIR="$(abspath tools/build)" OUTBINDIR="$(abspath tools/compiler)" make -C tools/toolbuilder toolscui

devenv:
	mkdir -p $(abspath tools/compiler)
	mkdir -p $(abspath tools/build)
	ARCHES="i386 x86_64" EMULATION="elf" OUTBUILDDIR="$(abspath tools/build)" OUTBINDIR="$(abspath tools/compiler)" make -C tools/toolbuilder toolsgui
