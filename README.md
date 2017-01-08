# Subra OS
Subra JavaScript Kernel

## History
Many years ago, I wrote a little educational kernel, it didn't do much, just print a big picture on the screen of a sunflower and write some text.

Due to having a lot of spare time available I very recently went back to my pet project and reported from predominantly C to C++ and JavaScript.

It is in no way complete, but it's on it's way.

## Components
A more proactive approach to writing less code has taken place, so in many cases GPL, BSD and MIT licensed code from other projects have been adapted and integrated.

* [libgcc](https://gcc.gnu.org/onlinedocs/gccint/Libgcc.html) ported for __div* and __udiv
* [libc](https://www.gnu.org/software/libc/) portions of GNU C Library used for definitions (headers only)
* [my namespace](https://sourceforge.net/projects/simplecstringclass/) string and vector classes
* [oslib](http://oslib.sourceforge.net/) a heavily modified and reduced GCC 6 compatible OS Support Library
* [v7](https://github.com/cesanta/v7) a modified JavaScript Engine targetting Subra Core

## What does it do?

Absolutely nothing yet, although it boots, checks VESA presence and runs a little bit of JavaScript (i.e. "Hello World!")

## How do I compile

A number of dependencies are required to build the toolchain that is capable of building the platform, assuming a Ubuntu 64 bit host:

~~~~
sudo apt-get install python-software-properties curl
sudo add-apt-repository ppa:git-core/ppa
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
sudo apt-get install g++ nasm git libtool-bin autoconf make gettext bison flex genisoimage textinfo python-mako cmake git git-lfs
git lfs install
~~~~

TODO::  Upload Compiler Sources to a Web Server

Within a bash shell:

~~~~
git clone https://github.com/warpcoil/subra.git
make toolchain
make
~~~~

TODO::  Give instructions on how to compile compiler and then build the platform

## How do I run

TODO::  Explain how to launch iso in virtualbox and/or qemu
