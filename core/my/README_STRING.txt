Release Notes
=============

Sourceforge Project "Simple C++ string and vector classes", http://sourceforge.net/projects/simplecstringclass


my_string: A simple C++ string class, a replacement for std::string.

my_vector: A simple C++ vector class, a replacement for std::vector.


The classes are simple and can easily be extended. The string class is based on the class my_string
by Christian Stigen Larsen, 2007, csl.name/programming/my_string/

A lot of stuff is missing, but I found these two classes interesting:

 - for novices to learn
 
 - for avoiding use of std::string and std::vector because of link problems in an
   application using mixed libraries, especially one compiled with an old Intel compiler 
   icc 7 (for Linux), and other libs compiled with newer compilers. Icc 7 uses
   an incompatible c++ standard library implementation.
 



v1.2:  2014-05-14
	- additional string compare() function 

v1.1:  2014-05-09 
	- much faster execution
	- some functions inlined
	- more functions added, as swap(), resize(), reserve(), clearMemory(), capacity()
	- less memory allocations when repeatedly adding data to a string. Memory is kept when a
          string is "clear"ed.

v1.0:  2014-05-08 
	- fixed heap memory use errors in string class methods erase() and substr()
	- fixed memory leak in vector class.

v0.2:   
	Archive my_string.tgz now contains a string class without reference to any C++ standard library content.


v0.1:   first version: a string class using std::vector.



