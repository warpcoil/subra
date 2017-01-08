BINUTILSVER=2.27
FFIVER=3.2.1
GCCVER=6.2.0
GDBVER=7.11.1
GLIBVER=2.51.0
GMPVER=5.1.3
ISLVER=0.16.1
MPCVER=1.0.3
MPFRVER=3.1.4
PKGCONFIGVER=0.28
QEMUVER=2.8.0-rc2
SDLVER=1.2.15
USBVER=1.0.20

ifndef ARCHIVE
ARCHIVE=$(abspath archive)
endif

ifndef ARCHES
$(error ARCHES is not set, this should contain a space delimited list of target architectures i.e. i386 x86_64 etc.)
endif

ifndef EMULATION
$(error EMULATION is not set, this should contain the binary format i.e. elf.)
endif

ifndef OUTBUILDDIR
OUTBUILDDIR=$(abspath build)
mkdir -p build
endif

ifndef OUTBINDIR
OUTBINDIR=$(abspath compiler)
mkdir -p compiler
endif

define check_extract #args - 0=Item (for display only), 1=tar.xx[x] filename
	if [ -d $(2) ]; then \
		echo -e "\033[0;32m$(1) already extracted\033[0m" ; \
	else \
		if [ ! -d $(2)-custom ]; then \
			echo -e "\033[0;32mExtracting $(1)\033[0m" ; \
			tar -xf $(ARCHIVE)/$(2).tar.* ; \
		else \
			echo -e "\033[0;32m$(1) (custom version) already extracted\033[0m" ; \
		fi ; \
	fi ;
endef

all:
	ARCHES="$(ARCHES)" EMULATION=$(EMULATION) OUTBUILDDIR=$(OUTBUILDDIR) OUTBINDIR=$(OUTBINDIR) make toolscui
	ARCHES="$(ARCHES)" EMULATION=$(EMULATION) OUTBUILDDIR=$(OUTBUILDDIR) OUTBINDIR=$(OUTBINDIR) make toolsgui
	ARCHES="$(ARCHES)" EMULATION=$(EMULATION) OUTBUILDDIR=$(OUTBUILDDIR) OUTBINDIR=$(OUTBINDIR) make clean
	echo -e "\033[0;32mTOOLCHAIN BUILT SUCCESSFULLY!\033[0m"

toolscui:
	#Source File Extraction Process
	$(call check_extract,Binutils,binutils-$(BINUTILSVER))
	
	$(call check_extract,GCC,gcc-$(GCCVER))
	
	$(call check_extract,GDB,gdb-$(GDBVER)-custom) #Custom version of gdb
	
	$(call check_extract,GMP,gmp-$(GMPVER))
	
	$(call check_extract,ISL,isl-$(ISLVER))
	
	$(call check_extract,MPC,mpc-$(MPCVER))
	
	$(call check_extract,MPFR,mpfr-$(MPFRVER))
	
	#Static Dependency Process
	echo -e "\033[0;32mCompiling STATIC GMP\033[0m"
	mkdir -p $(OUTBUILDDIR)/gmp
	cd $(OUTBUILDDIR)/gmp && $(abspath gmp-$(GMPVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/gmp
	
	echo -e "\033[0;32mCompiling STATIC MPFR\033[0m"
	mkdir -p $(OUTBUILDDIR)/mpfr
	cd $(OUTBUILDDIR)/mpfr && $(abspath mpfr-$(MPFRVER))/configure --prefix=$(OUTBUILDDIR)/static --with-gmp=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/mpfr
	
	echo -e "\033[0;32mCompiling STATIC MPC\033[0m"
	mkdir -p $(OUTBUILDDIR)/mpc
	cd $(OUTBUILDDIR)/mpc && $(abspath mpc-$(MPCVER))/configure --prefix=$(OUTBUILDDIR)/static --with-gmp=$(OUTBUILDDIR)/static --with-mpfr=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/mpc
	
	echo -e "\033[0;32mCompiling STATIC ISL\033[0m"
	mkdir -p $(OUTBUILDDIR)/isl
	cd $(OUTBUILDDIR)/isl && $(abspath isl-$(ISLVER))/configure --prefix=$(OUTBUILDDIR)/static --with-gmp-prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/isl
	
	#For all requested architectures build binutils
	$(foreach ARCH,$(ARCHES), echo -e "\033[0;32mCompiling binutils : $(ARCH)\033[0m"; mkdir -p $(OUTBUILDDIR)/binutils_$(ARCH); cd $(OUTBUILDDIR)/binutils_$(ARCH) && $(abspath binutils-$(BINUTILSVER))/configure --prefix=$(OUTBINDIR)/$(ARCH) --target=$(ARCH)-$(EMULATION) --with-gmp=$(OUTBUILDDIR)/static --with-mpc=$(OUTBUILDDIR)/static --with-mpfr=$(OUTBUILDDIR)/static --with-isl=$(OUTBUILDDIR)/static --with-sysroot --disable-nls --disable-werror && make -j4 && make install ; )
	$(foreach ARCH,$(ARCHES), rm -rf $(OUTBUILDDIR)/binutils_$(ARCH) ; )
	
	#For all requested architectures build gcc and g++
	$(foreach ARCH,$(ARCHES), echo -e "\033[0;32mCompiling GCC : $(ARCH)\033[0m"; export PATH=$(OUTBINDIR)/$(ARCH):$(PATH); mkdir -p $(OUTBUILDDIR)/gcc_$(ARCH); cd $(OUTBUILDDIR)/gcc_$(ARCH) && $(abspath gcc-$(GCCVER))/configure --prefix=$(OUTBINDIR)/$(ARCH) --target=$(ARCH)-$(EMULATION) --with-gmp=$(OUTBUILDDIR)/static --with-mpc=$(OUTBUILDDIR)/static --with-mpfr=$(OUTBUILDDIR)/static --with-isl=$(OUTBUILDDIR)/static --disable-nls --enable-languages=c,c++ --without-headers && make -j4 all-gcc && make -j4 all-target-libgcc && make install-gcc && make install-target-libgcc ; )
	$(foreach ARCH,$(ARCHES), rm -rf $(OUTBUILDDIR)/gcc_$(ARCH) ; )

	#For all requested architectures build gdb
	$(foreach ARCH,$(ARCHES), echo -e "\033[0;32mCompiling GDB : $(ARCH)\033[0m"; mkdir -p $(OUTBUILDDIR)/gdb_$(ARCH); cd $(OUTBUILDDIR)/gdb_$(ARCH) && $(abspath gdb-$(GDBVER))/configure --prefix=$(OUTBINDIR)/$(ARCH) --target=$(ARCH)-$(EMULATION) --with-gmp=$(OUTBUILDDIR)/static --with-mpc=$(OUTBUILDDIR)/static --with-mpfr=$(OUTBUILDDIR)/static --with-isl=$(OUTBUILDDIR)/static && make -j4 all && make install ; )
	$(foreach ARCH,$(ARCHES), rm -rf $(OUTBUILDDIR)/gdb_$(ARCH) ; )
	
	echo -e "\033[0;32mCleaning Up . . .\033[0m"
	
toolsgui:
	#Source File Extraction Process
	$(call check_extract,FFI,libffi-$(FFIVER))
	
	$(call check_extract,GLIB,glib-$(GLIBVER))
	
	$(call check_extract,PkgConfig,pkg-config-$(PKGCONFIGVER))
	
	$(call check_extract,Qemu,qemu-$(QEMUVER)-custom) #custom version of qemu
	
	$(call check_extract,SDL,SDL-$(SDLVER))
	
	$(call check_extract,USB,libusb-$(USBVER))
	
	#Static Dependency Process
	echo -e "\033[0;32mCompiling STATIC FFI\033[0m"
	mkdir -p $(OUTBUILDDIR)/libffi
	cd $(OUTBUILDDIR)/libffi && $(abspath libffi-$(FFIVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/libffi

	echo -e "\033[0;32mCompiling STATIC GLIB\033[0m"
	mkdir -p $(OUTBUILDDIR)/glib
	cd $(OUTBUILDDIR)/glib && LIBFFI_CFLAGS="-I$(OUTBUILDDIR)/static/lib/libffi-$(FFIVER)/include" LIBFFI_LIBS="-L$(OUTBUILDDIR)/static/lib -lffi" $(abspath glib-$(GLIBVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared --disable-libmount --with-pcre=internal && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/glib
	
	echo -e "\033[0;32mCompiling STATIC USB\033[0m"
	mkdir -p $(OUTBUILDDIR)/usb
	cd $(OUTBUILDDIR)/usb && $(abspath libusb-$(USBVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared --disable-udev && make && make install
	rm -rf $(OUTBUILDDIR)/usb
	
	echo -e "\033[0;32mCompiling STATIC SDL\033[0m"
	mkdir -p $(OUTBUILDDIR)/sdl
	cd $(OUTBUILDDIR)/sdl && $(abspath SDL-$(SDLVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/sdl
	
	echo -e "\033[0;32mCompiling STATIC PKGCONFIG\033[0m"
	mkdir -p $(OUTBUILDDIR)/pkgconfig
	cd $(OUTBUILDDIR)/pkgconfig && PKG_CONFIG_PATH=$(OUTBUILDDIR)/static/lib/pkgconfig $(abspath pkg-config-$(PKGCONFIGVER))/configure --prefix=$(OUTBUILDDIR)/static --enable-static --disable-shared --with-pc-path=$(OUTBUILDDIR)/static/lib/pkgconfig && make -j4 && make install
	rm -rf $(OUTBUILDDIR)/pkgconfig
	
	#For all requested architectures build qemu, to do this replace "i386 x86_64" with "i386-softmmu,x86_64-softmmu"
	TARGETLIST=echo $ARCHES | sed 's/ /,softmmu/g'
	TARGETLIST=$(TARGETLIST)-softmmu
	echo -e "\033[0;32mCompiling Qemu : $(TARGETLIST)\033[0m"
	mkdir -p $(OUTBUILDDIR)/qemu_generic
	cd $(OUTBUILDDIR)/qemu_generic && PATH=$(OUTBUILDDIR)/static/bin:$(PATH) $(abspath qemu-$(QEMUVER))/configure --prefix=$(OUTBINDIR)/generic --target-list=$(TARGETLIST) --disable-tools --extra-cflags="-I$(OUTBUILDDIR)/static/include -I$(OUTBUILDDIR)/static/include/libusb-1.0" --extra-ldflags="-Wl,--no-as-needed -L$(OUTBUILDDIR)/static/lib -lSDL -lusb-1.0 -lglib-2.0 -lgio-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lm -lc -lrt -ldl -pthread " --enable-sdl --with-sdlabi=1.2 --disable-kvm --enable-libusb && make && make install ; )
	rm -rf $(OUTBUILDDIR)/qemu_generic

getarchive:
	tar -xf $(abspath archive.tar.gz)

clean:
	#Delete all sources (clean up)
	echo -e "\033[0;32mCleaning Up . . .\033[0m"

	rm -rf binutils-$(BINUTILSVER)
	rm -rf libffi-$(FFIVER)
	rm -rf gcc-$(GCCVER)
	rm -rf gdb-$(GDBVER)
	rm -rf glib-$(GLIBVER)
	rm -rf gmp-$(GMPVER)
	rm -rf isl-$(ISLVER)
	rm -rf mpfr-$(MPFRVER)
	rm -rf mpc-$(MPCVER)
	rm -rf pkg-config-$(PKGCONFIGVER)
	rm -rf qemu-$(QEMUVER)
	rm -rf SDL-$(SDLVER)
	rm -rf libusb-$(USBVER)


	
